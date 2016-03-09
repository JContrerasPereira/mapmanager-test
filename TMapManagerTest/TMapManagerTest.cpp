// TMapManagerTest.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include "TMapManager.h"

class CEnemy
{
public:
	CEnemy() {}
	~CEnemy() {}
};

class CEnemyManager : TMapManager<CEnemy>
{
public:
	CEnemyManager() {}
	virtual ~CEnemyManager() {}
};

int _tmain(int argc, _TCHAR* argv[])
{
	CEnemyManager * enemyManager = new CEnemyManager();

	return 0;
}

