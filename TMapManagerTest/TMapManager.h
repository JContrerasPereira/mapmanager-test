#include <map>
#include <string>
#include <set>

#include <Windows.h>
#define DEBUG_ASSERT(expression)		\
	do {								\
		if(!(expression)				\
			&& IsDebuggerPresent())		\
			DebugBreak();				\
		} while(0)


struct BoolRefCount_t
{
	bool b;
	unsigned int c;

	BoolRefCount_t()
		: b(false)
		, c(0)
	{}
};

template<class T>
class TMapManager
{
protected:
	typedef std::map<std::string, T*> resourceMap_t;
	resourceMap_t m_resources;

	typedef std::set<BoolRefCount_t*> dirtyReferencesSet_t;
	dirtyReferencesSet_t m_dirtyReferences;

	T* get_internal(const std::string& name) const;

	//Ref(const Ref& r) {}

public:
	typedef TMapManager<T> MapManagerType_t;


	class Ref
	{
		std::string name;
		T* ref;
		BoolRefCount_t* dirty;
		TMapManager<T> * manager;

	public:
		Ref(const std::string& name, T* ref, BoolRefCount_t* dirty, TMapManager<T> * manager)
			: name(name)
			, ref(ref)
			, dirty(dirty)
			, manager(manager)
		{
			dirty->c++;
		}

		Ref(Ref&& r)
			: name(std::move(r->name))
			, ref(std::move(r->ref))
			, dirty(std::move(r->dirty))
			, manager(std::move(r->manager))
		{
		}

		~Ref()
		{
			if (dirty != nullptr)
			{
				manager->release(dirty);
			}
		}


		T* operator-> ()
		{
			if (dirty->b)
			{
				ref = manager->get_internal(name);
				dirty->b = false;
			}
			return ref;
		}

		T& operator* ()
		{
			if (dirty->b)
			{
				ref = manager->get_internal(name);
				dirty->b = false;
			}
			return *ref;
		}
	};

	virtual ~TMapManager();

	virtual Ref get(const std::string& name)
	{
		BoolRefCount_t* dirty = new BoolRefCount_t();
		m_dirtyReferences.insert(dirty);
		T* ref = get_internal(name);
		return Ref(name, ref, dirty, this);
	}

	virtual void add(const std::string& name, T* instance);
	virtual void remove(const std::string& name);
	virtual void destroy();
	virtual void release(BoolRefCount_t* dirty);
};

template<class T>
void TMapManager<T>::add(const std::string& name, T* instance)
{
	DEBUG_ASSERT(m_resources.find(name) == m_resources.end());
	m_resources[name] = instance;
}

template<class T>
void TMapManager<T>::remove(const std::string& name)
{
	DEBUG_ASSERT(m_resources.find(name) != m_resources.end());
	m_resources.erase(name);
}

template<class T>
TMapManager<T>::~TMapManager()
{
	DEBUG_ASSERT(m_resources.size() == 0 || "Resources not destroyed!");
}

template<class T>
void TMapManager<T>::destroy()
{
	for (auto &r : m_resources)
	{
		delete r.second;
	}
	m_resources.clear();

	for (auto d : m_dirtyReferences)
	{
		d->b = true;
	}
}

template<class T>
void TMapManager<T>::release(BoolRefCount_t* dirty)
{
	dirty->c--;
	if (!dirty->c)
	{
		m_dirtyReferences.erase(dirty);
		delete dirty;
	}
}

template<class T>
T* TMapManager<T>::get_internal(const std::string& name) const
{
	auto it = m_resources.find(name);
	if (it != m_resources.end())
	{
		return it->second;
	}
	return nullptr;
}