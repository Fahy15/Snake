#pragma once
#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;
class MainMenu :
	public cocos2d::Layer
{
public:
	static Scene* createScene();
	CREATE_FUNC(MainMenu);
	virtual bool init();
	void menuCallBack(Ref* object);
};

#endif
