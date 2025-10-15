#include "AppDelegate.h"
#include "controllers/GameController.h"

USING_NS_CC;

AppDelegate::AppDelegate() {}
AppDelegate::~AppDelegate() {}

bool AppDelegate::applicationDidFinishLaunching()
{
    // ��ȡ���ݺ�OpenGL��ͼ
    auto director = Director::getInstance();
    auto glView = director->getOpenGLView();

    // �����ͼ�����ڣ��򴴽�
    if (!glView)
    {
        glView = GLViewImpl::createWithRect("CardGame", Rect(0, 0, 1080, 2080), 0.5f);
        director->setOpenGLView(glView);
    }

    // ������Ʒֱ��ʣ��̶����
    glView->setDesignResolutionSize(1080, 2080, ResolutionPolicy::FIXED_WIDTH);

    // ����֡��60FPS
    director->setAnimationInterval(1.0f / 60.0f);

    // �����Դ����·��
    FileUtils::getInstance()->addSearchPath("res");

    // ����������������
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
