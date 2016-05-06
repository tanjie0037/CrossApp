//
//  CAImageView.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-5-18.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#include "CAImageView.h"
#include "images/CAImageCache.h"
#include "shaders/CAShaderCache.h"
#include "shaders/CAGLProgram.h"
#include "shaders/CATransformation.h"
#include "animation/CAViewAnimation.h"
#include "basics/CAScheduler.h"

NS_CC_BEGIN

CAImageView* CAImageView::createWithImage(CAImage* image)
{
    CAImageView *pobSprite = new CAImageView();
    if (pobSprite && pobSprite->initWithImage(image))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

CAImageView* CAImageView::createWithFrame(const DRect& rect)
{
	CAImageView * pRet = new CAImageView();
    if (pRet && pRet->initWithFrame(rect))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
	return NULL;
}

CAImageView* CAImageView::createWithCenter(const DRect& rect)
{
	CAImageView * pRet = new CAImageView();
    if (pRet && pRet->initWithCenter(rect))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
	return NULL;
}

CAImageView* CAImageView::createWithLayout(const CrossApp::DLayout &layout)
{
    CAImageView * pRet = new CAImageView();
    if (pRet && pRet->initWithLayout(layout))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

CAImageView* CAImageView::create()
{
    CAImageView *pSprite = new CAImageView();
    if (pSprite && pSprite->init())
    {
        pSprite->autorelease();
        return pSprite;
    }
    CC_SAFE_DELETE(pSprite);
    return NULL;
}

bool CAImageView::init(void)
{
    return CAImageView::initWithImage(NULL);
}

bool CAImageView::initWithImage(CAImage* image)
{
	DRect rect = DRectZero;
	if (image)
	{
		rect.size = image->getContentSize();
	}
	this->setImage(image);
	this->setImageRect(rect);
    //jie.tan: 方便初始化图片
    this->setBounds(rect);

    return true;
}

CAImageView::CAImageView(void)
:m_eImageViewScaleType(CAImageViewScaleTypeFitImageXY)
,m_bUpdateByImageViewScaleType(false)
,m_bAnimating(false)
,m_iAnimationRepeatCount(0)
,m_fAnimationDuration(1/30.0f)
,m_fAnimationRunTime(0)
{
    
}

CAImageView::~CAImageView(void)
{
    m_vAnimationImages.clear();
}

void CAImageView::updateByImageViewScaleType()
{
    CC_RETURN_IF(m_pobImage == NULL);
    CC_RETURN_IF(m_bUpdateByImageViewScaleType);
    m_bUpdateByImageViewScaleType = true;
    
    DSize viewSize = m_obContentSize;
    DSize imageSize = m_pobImage->getContentSize();
    DRect rect = DRect(m_obRect);
    float viewRatio = viewSize.width / viewSize.height;
    float imageRatio = imageSize.width / imageSize.height;
    
    switch (m_eImageViewScaleType)
    {
        case CAImageViewScaleTypeFitViewByHorizontal:
        {
            viewSize.width = viewSize.height * imageRatio;
        }
            break;
        case CAImageViewScaleTypeFitViewByVertical:
        {
            viewSize.height = viewSize.width / imageRatio;
        }
            break;
        default:
            break;
    }
    
    m_fLeft = 0;
    m_fTop = 0;
    m_fRight = viewSize.width;
    m_fBottom = viewSize.height;
    
    switch (m_eImageViewScaleType)
    {
        case CAImageViewScaleTypeFitImageCrop:
        {
            if (imageRatio > viewRatio)
            {
                rect.size.height = imageSize.height;
                rect.size.width = imageSize.height * viewRatio;
                rect.origin.x = (imageSize.width - rect.size.width) / 2;
                rect.origin.y = 0;
            }
            else if (imageRatio < viewRatio)
            {
                rect.size.width = imageSize.width;
                rect.size.height = imageSize.width / viewRatio;
                rect.origin.y = (imageSize.height - rect.size.height) / 2;
                rect.origin.x = 0;
            }
        }
            break;
        case CAImageViewScaleTypeFitImageInside:
        {
            if (imageRatio > viewRatio)
            {
                m_fTop = (viewSize.height - viewSize.width / imageRatio) / 2;
                m_fBottom = m_fTop + viewSize.width / imageRatio;
            }
            else if (imageRatio < viewRatio)
            {
                m_fLeft = (viewSize.width - viewSize.height * imageRatio) / 2;
                m_fRight = m_fLeft + viewSize.height * imageRatio;
            }
        }
            break;
        default:
            break;
    }
    this->setImageRect(rect);
    if (!viewSize.equals(m_obContentSize))
    {
        if (m_eLayoutType == 0)
        {
            DRect rect = this->getFrame();
            rect.size = viewSize;
            this->setFrame(rect);
        }
        else if (m_eLayoutType == 1)
        {
            DRect rect = this->getCenter();
            rect.size = viewSize;
            this->setCenter(rect);
        }
    }
    m_bUpdateByImageViewScaleType = false;
}

void CAImageView::setContentSize(const DSize & contentSize)
{
    if (CAViewAnimation::areAnimationsEnabled()
        && CAViewAnimation::areBeginAnimations())
    {
        CAViewAnimation::getInstance()->setContentSize(contentSize, this);
    }
    else if (!contentSize.equals(m_obContentSize))
    {
        m_obContentSize = contentSize;
        m_obAnchorPointInPoints.x = m_obContentSize.width * m_obAnchorPoint.x;
        m_obAnchorPointInPoints.y = m_obContentSize.height * m_obAnchorPoint.y;

        CAVector<CAView*>::iterator itr;
        for (itr=m_obSubviews.begin(); itr!=m_obSubviews.end(); itr++)
        {
            (*itr)->reViewlayout(m_obContentSize);
        }
        
        this->updateByImageViewScaleType();
        this->updateDraw();
    }
}

void CAImageView::setImage(CAImage* image)
{
    CAView::setImage(image);
    if (image)
    {
        DRect rect = DRectZero;
        rect.size = image->getContentSize();
        this->setVertexRect(rect);
        this->updateByImageViewScaleType();
    }
}

CAImage* CAImageView::getImage()
{
    return CAView::getImage();
}

void CAImageView::updateImageRect()
{
    // Don't update Z.
    m_sQuad.bl.vertices = vertex3(  m_fLeft,    m_fTop, m_fVertexZ);
    m_sQuad.br.vertices = vertex3( m_fRight,    m_fTop, m_fVertexZ);
    m_sQuad.tl.vertices = vertex3(  m_fLeft, m_fBottom, m_fVertexZ);
    m_sQuad.tr.vertices = vertex3( m_fRight, m_fBottom, m_fVertexZ);
}

void CAImageView::setImageViewScaleType(const CAImageViewScaleType &var)
{
    CC_RETURN_IF(m_eImageViewScaleType == var);
    m_eImageViewScaleType = var;
    this->updateByImageViewScaleType();
}

const CAImageViewScaleType& CAImageView::getImageViewScaleType()
{
    return m_eImageViewScaleType;
}

void CAImageView::startAnimating()
{
    CC_RETURN_IF(m_vAnimationImages.empty());
    CC_RETURN_IF(m_bAnimating);
    m_bAnimating = true;
    CAScheduler::schedule(schedule_selector(CAImageView::update), this, m_fAnimationDuration);
}

void CAImageView::stopAnimating()
{
    CC_RETURN_IF(!m_bAnimating);
    m_bAnimating = false;
    m_fAnimationRunTime = 0;
    CAScheduler::unschedule(schedule_selector(CAImageView::update), this);
}

bool CAImageView::isAnimating()
{
    return m_bAnimating;
}

void CAImageView::update(float dt)
{
    do
    {
        CC_BREAK_IF(m_vAnimationImages.empty());
        m_fAnimationRunTime += dt * 10000;
        int count = (int)m_vAnimationImages.size();
        int index = m_fAnimationRunTime / (m_fAnimationDuration * 10000);
        bool isFinished = (m_iAnimationRepeatCount > 0 && index >= count * m_iAnimationRepeatCount);
        if (isFinished)
        {
            index = count - 1;
        }
        else
        {
            index -= 1;
        }
        index = index % count;
        index = MAX(index, 0);
        this->setImage(m_vAnimationImages.at(index));
        
        if (isFinished)
        {
            this->stopAnimating();
            break;
        }
    }
    while (0);
}

CAView* CAImageView::copy()
{
    CAImageView* pReturn = CAImageView::create();
    if (m_eLayoutType == 0)
    {
        pReturn->setFrame(this->getFrame());
    }
    else if (m_eLayoutType == 1)
    {
        pReturn->setCenter(this->getCenter());
    }
    pReturn->setImageViewScaleType(this->getImageViewScaleType());
    pReturn->setImage(this->getImage());
    pReturn->setColor(this->getColor());
    return pReturn;
}

void CAImageView::setImageAsyncWithFile(const std::string& path)
{
    CAImageCache::sharedImageCache()->addImageFullPathAsync(path, this, callfuncO_selector(CAImageView::asyncFinish));
}

void CAImageView::asyncFinish(CrossApp::CAObject *var)
{
    CAImage* image = dynamic_cast<CAImage*>(var);
    this->setImage(image);
}

NS_CC_END
