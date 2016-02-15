#pragma once
#ifndef __SNAKE__GAMESCENE__
#define __SNAKE__GAMESCENE__

#include "cocos2d.h"

USING_NS_CC;
//移动方向枚举
enum ENUM_DIR
{
	DIR_UP, DIR_DOWN, DIR_RIGHT, DIR_LEFT, DIR_STOP
};

class SnakeNode:
	public cocos2d::Sprite
{
public:
	ENUM_DIR m_dir;    //移动方向
	int nodeType;    //节点类型 1蛇头 2身体 3食物
	int m_row, m_col;    //当前节点的坐标
public:
	static SnakeNode* create(int type);
	virtual bool init(int type);
	void setPositionRC(int row, int col);    //设置节点坐标
};

class GameScene :
	public cocos2d::Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(GameScene);
	void gameCallBack(Ref* object);
	void setBGMusic();
	void gameLogic(float t);
	void newBody();    //添加一个新的身体节点
	void moveBody();    //移动所有的身体节点
public:
	SnakeNode* spFood;    //食物
	SnakeNode* spHead;    //蛇头
	int m_score;
	Vector<SnakeNode*> allBody;    //保存身体节点
	static int flag;
	CCString m_scoreKey;    //"分数: "
	CCString m_helpTitle;    //"帮助信息"
	CCString m_helpMessage;    //帮助信息内容
};

#endif

