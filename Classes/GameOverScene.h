#pragma once

#include "cocos2d.h"
#include "MainMenu.h"
#include "GameScene.h"

USING_NS_CC;
class GameOverScene :
	public Layer
{
public:
	static Scene* createScene();
	bool init();
	CREATE_FUNC(GameOverScene);
	void goBackMainMenu();
	void playAgain();
};

