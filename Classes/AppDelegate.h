#pragma once
#include "cocos2d.h"

using namespace cocos2d;

class AppDelegate : private cocos2d::Application
{
    static cocos2d::Size mResolutionSize;
public:
    AppDelegate();
    virtual ~AppDelegate();

    virtual void initGLContextAttrs();
	   
    virtual bool applicationDidFinishLaunching();
    virtual void applicationDidEnterBackground();
    virtual void applicationWillEnterForeground();
};
