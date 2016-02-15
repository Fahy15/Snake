#include "MainMenu.h"
#include "GameScene.h"
#include "HelpScene.h"

Scene * MainMenu::createScene()
{
	auto scene = Scene::create();
	auto layer = MainMenu::create();
	scene->addChild(layer);
	return scene;
}

bool MainMenu::init()
{
	//初始化父类
	if (!Layer::init())
	{
		return false;
	}
	//获取背景大小
	auto size = Director::getInstance()->getWinSize();
	auto vis = Director::getInstance()->getVisibleSize();
	auto org = Director::getInstance()->getVisibleOrigin();
	//添加背景
	auto spriteBK = Sprite::create("menuBackGround.png");
	spriteBK->setScale(2);
	spriteBK->setPosition(Point(size.width / 2, size.height / 2));
	this->addChild(spriteBK,0);

	//添加2个菜单条目
	auto menuItemStart = MenuItemFont::create("Start",CC_CALLBACK_1(MainMenu::menuCallBack,this));
	menuItemStart->setTag(1);
	auto menuItemHelp = MenuItemFont::create("Help", CC_CALLBACK_1(MainMenu::menuCallBack, this));
	menuItemHelp->setTag(2);
	auto menu = Menu::create(menuItemStart,menuItemHelp, NULL);
	menu->setPosition(Point::ZERO);

	menuItemStart->setPosition(Point(
		(vis.width / 2 + org.x),
		vis.height/2+org.y) );
	menuItemHelp->setPosition(Point(
		(vis.width / 2 + org.x),
		vis.height / 2 + org.y- menuItemStart->getContentSize().height));
	this->addChild(menu,1);
	return true;
}

void MainMenu::menuCallBack(Ref * object)
{
	auto target = (Node*)object;
	Scene* scene;
	switch (target->getTag())
	{
	case 1:
		scene = GameScene::createScene();
		break;
	case 2:
		scene = HelpScene::createScene();
		break;
	default:
		break;
	}
	Director::getInstance()->replaceScene(scene);
}
