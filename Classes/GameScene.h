#pragma once
#ifndef __SNAKE__GAMESCENE__
#define __SNAKE__GAMESCENE__

#include "cocos2d.h"

USING_NS_CC;
//�ƶ�����ö��
enum ENUM_DIR
{
	DIR_UP, DIR_DOWN, DIR_RIGHT, DIR_LEFT, DIR_STOP
};

class SnakeNode:
	public cocos2d::Sprite
{
public:
	ENUM_DIR m_dir;    //�ƶ�����
	int nodeType;    //�ڵ����� 1��ͷ 2���� 3ʳ��
	int m_row, m_col;    //��ǰ�ڵ������
public:
	static SnakeNode* create(int type);
	virtual bool init(int type);
	void setPositionRC(int row, int col);    //���ýڵ�����
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
	void newBody();    //���һ���µ�����ڵ�
	void moveBody();    //�ƶ����е�����ڵ�
public:
	SnakeNode* spFood;    //ʳ��
	SnakeNode* spHead;    //��ͷ
	int m_score;
	Vector<SnakeNode*> allBody;    //��������ڵ�
	static int flag;
	CCString m_scoreKey;    //"����: "
	CCString m_helpTitle;    //"������Ϣ"
	CCString m_helpMessage;    //������Ϣ����
};

#endif

