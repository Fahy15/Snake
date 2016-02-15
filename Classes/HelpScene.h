#pragma once
#ifndef __SNACKGAME__HELPSCENE__
#define __SNACKGAME__HELPSCENE__

#include "cocos2d.h"

USING_NS_CC;
class HelpScene :
	public cocos2d::Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(HelpScene);
	void helpCallBack(Ref* object);
};

#endif