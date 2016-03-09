// TMapManagerTest.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include "TMapManager.h"
#include <vector>

class CMaterial
{
private:
	std::string name;

public:
	CMaterial(const std::string& name)
		: name(name)
	{
	}
	~CMaterial() {}
};

class CEnemy
{
private:
	int i;
public:
	std::vector<CMaterial *> materials;
	CEnemy(int i) : i(i) {}
	~CEnemy() {}
};

class CMaterialManager : public TMapManager<CMaterial>
{
public:
	CMaterialManager() {}
	virtual ~CMaterialManager() {}
};

class CEnemyManager : public TMapManager<CEnemy>
{
public:
	CEnemyManager() {}
	virtual ~CEnemyManager() {}
};

int _tmain(int argc, _TCHAR* argv[])
{
	CEnemyManager * enemyManager = new CEnemyManager();
	CMaterialManager * materialManager = new CMaterialManager();

	CMaterial * material1 = new CMaterial("material1");
	CMaterial * material2 = new CMaterial("material2");

	materialManager->add("material1", material1);
	materialManager->add("material2", material2);

	enemyManager->add("enemy1", new CEnemy(1));

	CEnemy * enemy1 = enemyManager->get("enemy1").operator->();

	enemy1->materials.push_back(material1);
	enemy1->materials.push_back(material2);

	materialManager->destroy();

	materialManager->add("material1", new CMaterial("material1"));
	materialManager->add("material2", new CMaterial("material2"));

	return 0;
}

