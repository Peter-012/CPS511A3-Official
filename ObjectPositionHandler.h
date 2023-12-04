#pragma once
#ifndef OBJ_POS_HANDLER_H
#define OBJ_POS_HANDLER_H

#include "Blimp.h"
#include "Enemy.h"

class ObjectPositionHandler {


public:
	Blimp* player;
	Enemy* enemy;

	void setPlayer(Blimp* player);
	void setEnemy(Enemy* enemy);
	void getEnemyRef(Enemy* enemy);
	void getPlayerRef(Blimp* player);

};




#endif