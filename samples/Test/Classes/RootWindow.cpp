﻿

#include "RootWindow.h"
#include "MenuViewController.h"
#include "CDUIShowCollectionView.h"
#include "CDNewsViewController.h"
#include "CDNewsImageController.h"
#include "CDNewsAboutController.h"

static RootWindow* _window = NULL;

RootWindow* RootWindow::getInstance()
{
    if (_window == NULL)
    {
        _window = new RootWindow();
        _window->init();
        _window->autorelease();
    }
    return _window;
    
}

RootWindow::RootWindow()
:m_pRootNavigationController(NULL)
,m_pRootDrawerController(NULL)
{
}

RootWindow::~RootWindow()
{

}

bool RootWindow::init()
{
    if (!CAWindow::init())
    {
        return false;
    }
    CCSize winSize = this->getBounds().size;
    
    CDUIShowCollectionView* tabBarController = new CDUIShowCollectionView();
    tabBarController->init();
    tabBarController->setTitle(UTF8("控件展示"));
    CANavigationController *nav = new CANavigationController();
    nav->initWithRootViewController(tabBarController);
    nav->setNavigationBarBackGroundImage(CAImage::create("image/navbg.jpg"));
    tabBarController->release();
    
    MenuViewController* _menuview = MenuViewController::create();
    
    CADrawerController* drawer = new CADrawerController();
    drawer->initWithController(_menuview, nav, this->getBounds().size.width/6*5);
    drawer->setBackgroundView(CAImageView::createWithImage(CAImage::create("image/bg.jpg")));
    drawer->setEffect3D(true);
    nav->release();
    
    this->setRootViewController(drawer);
    drawer->release();
    
    m_pRootNavigationController = nav;
    m_pRootDrawerController = drawer;

    return true;
}

void RootWindow::initUIView()
{
    do
    {
        CAViewController* viewController = m_pRootNavigationController->getViewControllerAtIndex(0);
        CC_BREAK_IF(dynamic_cast<CDUIShowCollectionView*>(viewController));

        CDUIShowCollectionView* tabBarController = new CDUIShowCollectionView();
        tabBarController->init();
        tabBarController->setTitle(UTF8("控件展示"));
        tabBarController->autorelease();
        m_pRootNavigationController->replaceViewController(tabBarController, false);
    }
    while (0);

    m_pRootDrawerController->hideLeftViewController(true);
}

void RootWindow::intNewsView()
{
    do
    {
        CAViewController* viewController = m_pRootNavigationController->getViewControllerAtIndex(0);
        CC_BREAK_IF(dynamic_cast<CATabBarController*>(viewController));
        
        
        CAVector<CAViewController*> vec_news;
        
        CDNewsViewController* news_controller1 = new CDNewsViewController(0);
        news_controller1->autorelease();
        news_controller1->setTabBarItem(CATabBarItem::create(unicode_to_utf8(newsTitle[0]), CAImage::create(""), CAImage::create("")));
        vec_news.pushBack(news_controller1);
        
        CDNewsViewController* news_controller2 = new CDNewsViewController(1);
        news_controller2->autorelease();
        news_controller2->setTabBarItem(CATabBarItem::create(unicode_to_utf8(newsTitle[1]), CAImage::create(""), CAImage::create("")));
        vec_news.pushBack(news_controller2);
        
        CDNewsViewController* news_controller3 = new CDNewsViewController(2);
        news_controller3->autorelease();
        news_controller3->setTabBarItem(CATabBarItem::create(unicode_to_utf8(newsTitle[2]), CAImage::create(""), CAImage::create("")));
        vec_news.pushBack(news_controller3);
        
        CDNewsViewController* news_controller4 = new CDNewsViewController(3);
        news_controller4->autorelease();
        news_controller4->setTabBarItem(CATabBarItem::create(unicode_to_utf8(newsTitle[3]), CAImage::create(""), CAImage::create("")));
        vec_news.pushBack(news_controller4);
        
        CDNewsViewController* news_controller5 = new CDNewsViewController(4);
        news_controller5->autorelease();
        news_controller5->setTabBarItem(CATabBarItem::create(unicode_to_utf8(newsTitle[4]), CAImage::create(""), CAImage::create("")));
        vec_news.pushBack(news_controller5);
        
        
        CATabBarController * newsTabBarController = new CATabBarController();
        newsTabBarController->initWithViewControllers(vec_news,CABarVerticalAlignmentTop);
        newsTabBarController->setScrollEnabled(true);
        newsTabBarController->setTabBarItem(CATabBarItem::create("", CAImage::create("image/tab_news_btn_up.png"),CAImage::create("image/tab_news_btn_down.png")));
        newsTabBarController->setTabBarBackGroundImage(CAImage::create("source_material/tabbar_background.png"));
        newsTabBarController->setTabBarSelectedBackGroundColor(CAColor_clear);
        newsTabBarController->setTabBarTitleColorForNormal(CAColor_black);
        newsTabBarController->setTabBarTitleColorForSelected(ccc4(42,117,201,255));
        newsTabBarController->setTitle(" ");
        newsTabBarController->showTabBarSelectedIndicator();
        newsTabBarController->setTabBarSelectedIndicatorColor(ccc4(42,117,201,255));
        newsTabBarController->autorelease();
        
        CAVector<CAViewController*> vec;
        vec.pushBack(newsTabBarController);
        /////////////////NewsTabBar///////////////////////
        
        /////////////////ImageTabBar///////////////////////
        CAVector<CAViewController*> vec_image;
        
        CDNewsImageController* controller1 = new CDNewsImageController(0);
        controller1->autorelease();
        controller1->setTabBarItem(CATabBarItem::create(unicode_to_utf8(imageTitle[0]), CAImage::create(""), CAImage::create("")));
        vec_image.pushBack(controller1);
        
        CDNewsImageController* controller2 = new CDNewsImageController(1);
        controller2->setTabBarItem(CATabBarItem::create(unicode_to_utf8(imageTitle[1]), CAImage::create(""), CAImage::create("")));
        controller2->autorelease();
        vec_image.pushBack(controller2);
        
        CDNewsImageController* controller3 = new CDNewsImageController(2);
        controller3->setTabBarItem(CATabBarItem::create(unicode_to_utf8(imageTitle[2]), CAImage::create(""), CAImage::create("")));
        controller3->autorelease();
        vec_image.pushBack(controller3);
        
        CDNewsImageController* controller4 = new CDNewsImageController(3);
        controller4->setTabBarItem(CATabBarItem::create(unicode_to_utf8(imageTitle[3]), CAImage::create(""), CAImage::create("")));
        controller4->autorelease();
        vec_image.pushBack(controller4);
        
        CATabBarController * imageTabBarController = new CATabBarController();
        imageTabBarController->initWithViewControllers(vec_image,CABarVerticalAlignmentTop);
        imageTabBarController->setScrollEnabled(true);
        imageTabBarController->setTabBarItem(CATabBarItem::create("", CAImage::create("image/tab_image_btn_up.png"),CAImage::create("image/tab_image_btn_down.png")));
        imageTabBarController->setTabBarBackGroundImage(CAImage::create("source_material/tabbar_background.png"));
        imageTabBarController->setTabBarSelectedBackGroundColor(CAColor_clear);
        imageTabBarController->setTabBarTitleColorForNormal(CAColor_black);
        imageTabBarController->setTabBarTitleColorForSelected(ccc4(42,117,201,255));
        imageTabBarController->setTitle(" ");
        imageTabBarController->showTabBarSelectedIndicator();
        imageTabBarController->setTabBarSelectedIndicatorColor(ccc4(42,117,201,255));
        imageTabBarController->autorelease();
        
        vec.pushBack(imageTabBarController);
        
        /////////////////ImageTabBar///////////////////////
        
        CDNewsAboutController* _aboutController = CDNewsAboutController::create();
        _aboutController->setTabBarItem(CATabBarItem::create("", CAImage::create("image/tab_mine_btn_up.png"),CAImage::create("image/tab_mine_btn_down.png")));
        vec.pushBack(_aboutController);
        

        CATabBarController* tabBarController = new CATabBarController();
        tabBarController->initWithViewControllers(vec);
        tabBarController->setTitle(" ");
        tabBarController->setTabBarSelectedBackGroundColor(CAColor_clear);
        tabBarController->setTabBarBackGroundImage(CAImage::create("image/tab_news_bg.png"));
        CANavigationBarItem* temp_nav = CANavigationBarItem::create(" ");
        temp_nav->setNagigationBarHidden(true);
        tabBarController->setNavigationBarItem(temp_nav);
        tabBarController->autorelease();
        
        m_pRootNavigationController->replaceViewController(tabBarController, false);

    }
    while (0);
    
    m_pRootDrawerController->hideLeftViewController(true);
}
