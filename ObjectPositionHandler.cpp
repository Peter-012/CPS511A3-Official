
#include "header/ObjectPositionHandler.h"

void ObjectPositionHandler::setPlayer(Blimp* player) {
	this->player = player;

}


void ObjectPositionHandler::setEnemy(Enemy* enemy) {
	this->enemy = enemy;
}


void ObjectPositionHandler::getEnemyRef(Enemy* enemy) {
	enemy = this->enemy;
}
void ObjectPositionHandler::getPlayerRef(Blimp* player) {
	player = this->player;
}