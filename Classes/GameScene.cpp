#include "GameScene.h"
#include "MainMenu.h"
#include "GameOverScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

int GameScene::flag = 1;

Scene * GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	//添加地图
	auto draw = DrawNode::create();
	draw->setAnchorPoint(Point::ZERO);
	draw->setPosition(Point::ZERO);
	this->addChild(draw, 1);
	for (int i = 0; i < 21; i++)
	{
		draw->drawSegment(Point(0,16*i), Point(320, 16*i), 1, Color4F(1, 1, 1, 1));
		draw->drawSegment(Point(16*i,0), Point(16*i, 320), 1, Color4F(1, 1, 1, 1));
	}
	
	//添加蛇头
	spHead = SnakeNode::create(1);
	spHead->setPosition(Point::ZERO);
	this->addChild(spHead);
	//添加身体
	//添加食物
	spFood = SnakeNode::create(3);
	int row = rand() % 20;
	int col = rand() % 20;
	spFood->setPositionRC(row, col);
	
	this->addChild(spFood);

	auto size = Director::getInstance()->getWinSize();
	auto visible = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto spriteBK = Sprite::create("menuBackGround.png");
	spriteBK->setPosition(size.width / 2, size.height / 2);
	spriteBK->setOpacity(75);
	this->addChild(spriteBK,0);
	//读取xml字符
	CCDictionary* message = CCDictionary::createWithContentsOfFile("chinese.xml");
	m_scoreKey = message->valueForKey("score")->getCString();

	auto scoreText = CCString::createWithFormat("%s%d", m_scoreKey.getCString(),m_score)->getCString();
	//分数显示
	auto labelScore = Label::createWithTTF(scoreText, "fonts/newSongTi.ttf", 25);
	labelScore->setTag(110);
	labelScore->setPosition(Point(
		size.width - labelScore->getContentSize().width,
		size.height - labelScore->getContentSize().height));
	this->addChild(labelScore,1);
	//返回按钮
	auto menuItemBack = MenuItemFont::create("Back", CC_CALLBACK_1(GameScene::gameCallBack,this));
	auto bgMusicButton = MenuItemFont::create("Bgmusic", CC_CALLBACK_0(GameScene::setBGMusic, this));
	auto menu = Menu::create(menuItemBack, bgMusicButton, NULL);
	menu->setPosition(Point::ZERO);
	menuItemBack->setAnchorPoint(Point(1, 0));
	bgMusicButton->setAnchorPoint(Point(1, 0));
	menuItemBack->setPosition(Point(
		size.width,
		0));
	bgMusicButton->setPosition(Point(
		size.width,
		menuItemBack->getContentSize().height));
	this->addChild(menu);

	//添加计划任务，是蛇头自动移动
	this->schedule(schedule_selector(GameScene::gameLogic), 0.5);

	//加入用户触摸事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);    //设置为 true, 然后，在 onTouchBegan 方法发返回 'true' 将会吃掉触控事件, 防止其他监听器使用这个事件.
	listener->onTouchBegan = [&](Touch* t,Event* e) {
		//改变贪食蛇移动方向
		int col = t->getLocation().x / 16;
		int row = t->getLocation().y / 16;
		int spHeadCol = spHead->getPositionX() / 16;
		int spHeadRow = spHead->getPositionY() / 16;
		if (spHead->m_dir != ENUM_DIR::DIR_LEFT&&spHead->m_dir != ENUM_DIR::DIR_RIGHT)
		{
			if (spHeadCol < col)
			{
				spHead->m_dir = ENUM_DIR::DIR_RIGHT;
			}
			else
			{
				spHead->m_dir = ENUM_DIR::DIR_LEFT;
			}
		}
		else
		{
			if (spHeadRow < row)
			{
				spHead->m_dir = ENUM_DIR::DIR_UP;
			}
			else
			{
				spHead->m_dir = ENUM_DIR::DIR_DOWN;
			}
		}
		return true;
	};
	//事件分发器
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	
	return true;
}

void GameScene::gameCallBack(Ref * object)
{
	auto scene = MainMenu::createScene();
	Director::getInstance()->replaceScene(scene);
}
//背景音乐开关
void GameScene::setBGMusic()
{
	auto bgMusic = SimpleAudioEngine::getInstance();
	if (bgMusic->isBackgroundMusicPlaying())
	{
		GameScene::flag = 0;
		bgMusic->stopBackgroundMusic();
	}
	else
	{
		GameScene::flag = 1;
		bgMusic->rewindBackgroundMusic();
	}
}

void GameScene::gameLogic(float t)
{
	moveBody();    //移动身体节点
	//蛇头移动
	float speed = 0.3;
	switch (spHead->m_dir)
	{
	case ENUM_DIR::DIR_RIGHT:
		spHead->runAction(MoveBy::create(speed, Point(16, 0)));
		spHead->m_col++;
		break;
	case ENUM_DIR::DIR_LEFT:
		spHead->runAction(MoveBy::create(speed , Point(-16, 0)));
		spHead->m_col--;
		break;
	case ENUM_DIR::DIR_UP:
		spHead->runAction(MoveBy::create(speed, Point(0, 16)));
		spHead->m_row++;
		break;
	case ENUM_DIR::DIR_DOWN:
		spHead->runAction(MoveBy::create(speed, Point(0, -16)));
		spHead->m_row--;
		break;
	default:
		break;
	}
	//碰撞检测
	if (spHead->m_row == spFood->m_row && spHead->m_col == spFood->m_col)
	{
		//音效
		SimpleAudioEngine::getInstance()->playEffect("eat.wav");
		//分数增加
		this->m_score += 100;
		Label* label = (Label*)this->getChildByTag(110);

		auto scoreText = CCString::createWithFormat("%s%d", m_scoreKey.getCString(), m_score)->getCString();
		//分数显示
		label->setString(scoreText);
		int row = rand() % 20;
		int col = rand() % 20;
		spFood->setPositionRC(row, col);

		//添加新节点
		newBody();
	}
	if (spHead->m_row > 20 || 
		spHead->m_row < 0 || 
		spHead->m_col > 20 || 
		spHead->m_col < 0)
	{
		Scene* endScene = GameOverScene::createScene();
		Director::getInstance()->replaceScene(endScene);
	}
	for (auto bodynode:allBody)
	{
		if (bodynode->m_col == spHead->m_col&&bodynode->m_row == spHead->m_row)
		{
			Scene* endScene = GameOverScene::createScene();
			Director::getInstance()->replaceScene(endScene);
		}
	}
}

void GameScene::newBody()
{
	auto bodynode = SnakeNode::create(2);

	if (allBody.size() > 0)    //有身体节点
	{
		auto lastbody = allBody.at(allBody.size() - 1);
			bodynode->m_dir = lastbody ->m_dir;
		switch (bodynode->m_dir)
		{
		case ENUM_DIR::DIR_UP:
			bodynode->setPositionRC(lastbody->m_row - 1, lastbody->m_col);
			break;
		case ENUM_DIR::DIR_DOWN:
			bodynode->setPositionRC(lastbody->m_row + 1, lastbody->m_col);
			break;
		case ENUM_DIR::DIR_LEFT:
			bodynode->setPositionRC(lastbody->m_row, lastbody->m_col + 1);
			break;
		case ENUM_DIR::DIR_RIGHT:
			bodynode->setPositionRC(lastbody->m_row, lastbody->m_col - 1);
			break;
		default:
			break;
		}
	}
	else
	{
		bodynode->m_dir = spHead->m_dir;
		switch (bodynode->m_dir)
		{
		case ENUM_DIR::DIR_UP:
			bodynode->setPositionRC(spHead->m_row - 1, spHead->m_col);
			break;
		case ENUM_DIR::DIR_DOWN:
			bodynode->setPositionRC(spHead->m_row + 1, spHead->m_col);
			break;
		case ENUM_DIR::DIR_LEFT:
			bodynode->setPositionRC(spHead->m_row, spHead->m_col + 1);
			break;
		case ENUM_DIR::DIR_RIGHT:
			bodynode->setPositionRC(spHead->m_row, spHead->m_col - 1);
			break;
		default:
			break;
		}
	}
	//添加节点到当前图层
	this->addChild(bodynode);
	//添加节点到集合中
	allBody.pushBack(bodynode);
}

void GameScene::moveBody()
{
	if (0 == allBody.size())
	{
		return;
	}
	float speed = 0.3;
	for (auto bodynode:allBody)
	{
		switch (bodynode->m_dir)
		{
		case ENUM_DIR::DIR_RIGHT:
			bodynode->runAction(MoveBy::create(speed, Point(16, 0)));
			bodynode->m_col++;
			break;
		case ENUM_DIR::DIR_LEFT:
			bodynode->runAction(MoveBy::create(speed, Point(-16, 0)));
			bodynode->m_col--;
			break;
		case ENUM_DIR::DIR_UP:
			bodynode->runAction(MoveBy::create(speed, Point(0, 16)));
			bodynode->m_row++;
			break;
		case ENUM_DIR::DIR_DOWN:
			bodynode->runAction(MoveBy::create(speed, Point(0, -16)));
			bodynode->m_row--;
			break;
		default:
			break;
		}
	}
	//移动完成后，改变每个body的方向
	int i = allBody.size() - 1;
	for (; i > 0; i--)
	{
		allBody.at(i)->m_dir = allBody.at(i - 1)->m_dir;
	}
	allBody.at(i)->m_dir = spHead->m_dir;

}

SnakeNode * SnakeNode::create(int type)
{
	SnakeNode* pRet = new SnakeNode();
	if (pRet && pRet->init(type))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = nullptr;
	}
	return pRet;
}

bool SnakeNode::init(int type)
{
	if (!Sprite::init())
	{
		return false;
	}
	//根据类型初始化不同纹理
	switch (type)
	{
	case 1:
	{
		auto sprite = Sprite::create("head.png");
		sprite->setAnchorPoint(Point::ZERO);
		this->addChild(sprite, 3);
	}
		m_dir = ENUM_DIR::DIR_RIGHT;    //初始化蛇头移动方向
		break;
	case 2:
	{
		auto sprite = Sprite::create("body.png");
		sprite->setAnchorPoint(Point::ZERO);
		this->addChild(sprite);
	}
		m_dir = ENUM_DIR::DIR_STOP;
		break;
	case 3:
	{
		auto sprite = Sprite::create("food.png");
		sprite->setAnchorPoint(Point::ZERO);
		this->addChild(sprite, 3);
	}
		m_dir = ENUM_DIR::DIR_STOP;
		break;
	default:
		break;
	}
	return true;
}

void SnakeNode::setPositionRC(int row, int col)
{
	this->m_row = row;
	this->m_col = col;
	this->setPosition(col*16, row*16);
}
