#include "AppDelegate.h"
#include "GameScene.h"
#include "TitleScene.h"

//#define USE_AUDIO_ENGINE 1
#define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

Size AppDelegate::mResolutionSize = Size(1920, 1080);

AppDelegate::AppDelegate()
{}

AppDelegate::~AppDelegate() 
{
#if USE_AUDIO_ENGINE
	AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
	SimpleAudioEngine::end();
#endif
}

void AppDelegate::initGLContextAttrs()
{
	GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
	GLView::setGLContextAttrs(glContextAttrs);
}

static int register_all_packages()
{
	return 0;
}

bool AppDelegate::applicationDidFinishLaunching() 
{  
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();

	if(!glview) 
	{		
#ifdef _DEBUG
		glview = GLViewImpl::createWithRect("Cult of The Eye", cocos2d::Rect(0, 0, 1920, 1080));
#else	
		glview = GLViewImpl::createWithFullScreen("Cult of The Eye");
#endif

		director->setOpenGLView(glview);
	}	
	
#ifdef _DEBUG
	director->setDisplayStats(true);
#endif

	//Set FPS
	director->setAnimationInterval(1.0f / 60);
	
	// Set the design resolution
	glview->setDesignResolutionSize(mResolutionSize.width, mResolutionSize.height, ResolutionPolicy::SHOW_ALL);	
	
	register_all_packages();

	auto mTitleScene = TitleScene::create();
	director->runWithScene(mTitleScene);

	return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}
