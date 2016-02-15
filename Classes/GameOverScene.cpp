#include "GameOverScene.h"

Scene * GameOverScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameOverScene::create();
	scene->addChild(layer);
	return scene;
}

bool GameOverScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Sprite* bgPicture = Sprite::create("gameover.png");
	bgPicture->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bgPicture);

	auto mainMenu = MenuItemFont::create("MainMenu", CC_CALLBACK_0(GameOverScene::goBackMainMenu, this));
	auto again = MenuItemFont::create("Again", CC_CALLBACK_0(GameOverScene::playAgain, this));
	Menu* menu = Menu::create(mainMenu, again, NULL);
	menu->setPosition(Point::ZERO);
	again->setPosition(Point(mainMenu->getContentSize().width, mainMenu->getContentSize().height));
	mainMenu->setPosition(Point(visibleSize.width - mainMenu->getContentSize().width, mainMenu->getContentSize().height));
	this->addChild(menu);
	return true;
}

void GameOverScene::goBackMainMenu()
{
	Scene* secne = MainMenu::createScene();
	Director::getInstance()->replaceScene(secne);
}

void GameOverScene::playAgain()
{
	Scene* scene = GameScene::createScene();
	Director::getInstance()->replaceScene(scene);
}
