#include "AppDelegate.h"
#include "controllers/GameController.h"

USING_NS_CC;

AppDelegate::AppDelegate() {}
AppDelegate::~AppDelegate() {}

bool AppDelegate::applicationDidFinishLaunching()
{
    // 获取导演和OpenGL视图
    auto director = Director::getInstance();
    auto glView = director->getOpenGLView();

    // 如果视图不存在，则创建
    if (!glView)
    {
        glView = GLViewImpl::createWithRect("CardGame", Rect(0, 0, 1080, 2080), 0.5f);
        director->setOpenGLView(glView);
    }

    // 设置设计分辨率，固定宽度
    glView->setDesignResolutionSize(1080, 2080, ResolutionPolicy::FIXED_WIDTH);

    // 设置帧率60FPS
    director->setAnimationInterval(1.0f / 60.0f);

    // 添加资源搜索路径
    FileUtils::getInstance()->addSearchPath("res");

    // 创建并运行主场景
    auto mainScene = GameController::create();
    director->runWithScene(mainScene);

    return true;
}

void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();
}
