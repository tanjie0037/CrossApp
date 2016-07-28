//
//  CDNewsImageController.cpp
//  Test
//
//  Created by renhongguang on 15/4/17.
//
//

#include "CDNewsImageController.h"
#include "CDShowNewsImage.h"

CDNewsImagecollectionCell::CDNewsImagecollectionCell()
{
    cell_tag = 0;
    this->setAllowsSelected(false);
}

CDNewsImagecollectionCell::~CDNewsImagecollectionCell()
{
    
}

CDNewsImagecollectionCell* CDNewsImagecollectionCell::create(const std::string& identifier)
{
    CDNewsImagecollectionCell* tableViewCell = new CDNewsImagecollectionCell();
    if(tableViewCell&&tableViewCell->initWithReuseIdentifier(identifier))
    {
        tableViewCell->autorelease();
        return tableViewCell;
    }
    CC_SAFE_DELETE(tableViewCell);
    return NULL;
}

void CDNewsImagecollectionCell::highlightedcollectionCell()
{
    this->setBackgroundView(CAView::createWithColor(ccc4(0, 0, 0, 128)));
}

void CDNewsImagecollectionCell::selectedcollectionCell()
{
    this->setBackgroundView(CAView::createWithColor(ccc4(0, 0, 0, 128)));
}
void CDNewsImagecollectionCell::initWithCell(int num)
{
    cell_tag = CCRANDOM_0_1()*4+1;
    CALabel* test = CALabel::createWithLayout(DLayout(DHorizontalLayout_L_R(40, 90), DVerticalLayout_B_H(100, 40)));
	test->setColor(CAColor_black);
    test->setTextAlignment(CATextAlignmentLeft);
    test->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
    test->setFontSize(32);
    test->setBold(true);
    test->setTag(100);
    this->getContentView()->addSubview(test);
    
    CALabel* dsc = CALabel::createWithLayout(DLayout(DHorizontalLayout_L_R(40, 40), DVerticalLayout_B_H(15, 80)));
	dsc->setColor(CAColor_black);
    dsc->setTextAlignment(CATextAlignmentLeft);
    dsc->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
    dsc->setFontSize(26);

    dsc->setTag(101);
	dsc->setColor(ccc4(200, 200, 200, 255));
    this->getContentView()->addSubview(dsc);
    
    m_pImage1 = CommonUrlImageView::createWithLayout(DLayout(DHorizontalLayout_L_R(0, 0), DVerticalLayout_T_B(5, 150)));
    m_pImage1->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    
    m_pImage2 = CommonUrlImageView::createWithLayout(DLayout(DHorizontalLayout_L_R(0, 0), DVerticalLayout_T_B(5, 150)));
    m_pImage2->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    
    m_pImage3 = CommonUrlImageView::createWithLayout(DLayout(DHorizontalLayout_L_R(0, 0), DVerticalLayout_T_B(5, 150)));
    m_pImage3->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    
    m_pImage4 = CommonUrlImageView::createWithLayout(DLayout(DHorizontalLayout_L_R(0, 0), DVerticalLayout_T_B(5, 150)));
    m_pImage4->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    
    CAView* view = CAView::createWithLayout(DLayout(DHorizontalLayoutFill, DVerticalLayout_T_B(3, 148)), CAColor_clear);
    view->addSubview(m_pImage1);
    view->addSubview(m_pImage2);
    view->addSubview(m_pImage3);
    view->addSubview(m_pImage4);
    this->getContentView()->addSubview(view);
}

CDNewsImageController::CDNewsImageController(int index)
:p_AutoCollectionView(NULL)
,p_pLoading(NULL)
,p_section(1)
{
    urlID = index;
}

CDNewsImageController::~CDNewsImageController()
{

}

string CDNewsImageController::getSign(std::map<std::string,std::string> key_value)
{
    string appsecret = "c174cb1fda3491285be953998bb867a0";
    string tempStr = "";
    std::map<std::string,std::string>::iterator itr;
    for (itr=key_value.begin(); itr!=key_value.end(); itr++) {
        tempStr = tempStr+itr->first+itr->second;
    }
    tempStr = appsecret+tempStr+appsecret;
    CCLog("tempStr===%s",tempStr.c_str());
    string sign = MD5(tempStr).md5();
    for(int i=0;i<sign.length();i++)
    {
        if(sign[i]>='a'&&sign[i]<='z')
            sign[i]-=32;
        else if
            (sign[i]>='A'&&sign[i]<='Z')sign[i]+=32;
    }
    return sign;
}

void CDNewsImageController::viewDidLoad()
{
    CCLog("NewsImageController");
    
    m_ImageMsg.clear();
    m_ImageNum.clear();

    if (m_ImageMsg.empty()) {
        
        std::map<std::string,std::string> key_value;
        key_value["tag"] = imageTag[urlID];
        key_value["page"]= "1";
        key_value["limit"]= "20";
        key_value["appid"]="10000";
        key_value["sign_method"]="md5";
        string tempSign = getSign(key_value);
        CCLog("sign===%s",tempSign.c_str()); 
        key_value["sign"] = tempSign;
        string tempUrl = "http://api.9miao.com/newsgirlpic/";
        CommonHttpManager::getInstance()->send_get(tempUrl, key_value, this,CommonHttpJson_selector(CDNewsImageController::onRequestFinished));
        {
            p_pLoading = CAActivityIndicatorView::createWithLayout(DLayout(DHorizontalLayout_W_C(50, 0.5), DVerticalLayout_H_C(0, 0.5)));
            this->getView()->insertSubview(p_pLoading, CAWindowZOderTop);
            p_pLoading->setLoadingMinTime(0.5f);
            p_pLoading->setTargetOnCancel(this, callfunc_selector(CDNewsImageController::initImageCollectionView));
        }
    }else{
        this->initImageCollectionView();
    }

}

void CDNewsImageController::initImageCollectionView()
{
    if (m_ImageMsg.empty()) {
        showAlert();
        return;
    }

    if (p_AutoCollectionView!=NULL)
    {
        this->getView()->removeSubview(p_AutoCollectionView);
        p_AutoCollectionView = NULL;
    }
    CAPullToRefreshView *refreshDiscount = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeFooter);
    refreshDiscount->setLabelColor(CAColor_black);
    CAPullToRefreshView *refreshDiscount1 = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeHeader);
    refreshDiscount1->setLabelColor(CAColor_black);
    
    p_AutoCollectionView = CAAutoCollectionView::createWithLayout(DLayoutFill);
    p_AutoCollectionView->setCollectionViewDataSource(this);
    p_AutoCollectionView->setCollectionViewDelegate(this);
    p_AutoCollectionView->setScrollViewDelegate(this);
    p_AutoCollectionView->setAllowsSelection(true);
    p_AutoCollectionView->setAllowsMultipleSelection(false);
    p_AutoCollectionView->setHoriCellInterval(20);
    p_AutoCollectionView->setVertCellInterval(20);
    p_AutoCollectionView->setFooterRefreshView(refreshDiscount);
    p_AutoCollectionView->setHeaderRefreshView(refreshDiscount1);
    p_AutoCollectionView->setBackgroundColor(ccc4(240,240,240,255));
    this->getView()->addSubview(p_AutoCollectionView);
}

void CDNewsImageController::showAlert()
{
    p_alertView = CAView::createWithFrame(this->getView()->getBounds());
    this->getView()->addSubview(p_alertView);
    
    CAImageView* bg = CAImageView::createWithLayout(DLayoutFill);
    bg->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    bg->setImage(CAImage::create("image/HelloWorld.png"));
    
    CAButton* btn5 = CAButton::create(CAButtonTypeSquareRect);
    btn5->setTag(100);
    btn5->setLayout(DLayout(DHorizontalLayout_L_R(0, 0), DVerticalLayout_T_B(0, 0)));
    btn5->setTitleColorForState(CAControlStateNormal,CAColor_white);
    btn5->setBackgroundViewForState(CAControlStateNormal, bg);
    btn5->setBackgroundViewForState(CAControlStateHighlighted, bg);
    btn5->addTarget(this, CAControl_selector(CDNewsImageController::buttonCallBack), CAControlEventTouchUpInSide);
    p_alertView->addSubview(btn5);
    
    CALabel* test = CALabel::createWithLayout(DLayout(DHorizontalLayout_L_C(0, 0.5), DVerticalLayout_H_C(40, 0.1)));
	test->setColor(CAColor_gray);
    test->setTextAlignment(CATextAlignmentCenter);
    test->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
    test->setFontSize(24);
    test->setText("网络不给力，请点击屏幕重新加载～");
    p_alertView->addSubview(test);
    
}

void CDNewsImageController::buttonCallBack(CAControl* btn,DPoint point)
{
    this->getView()->removeSubview(p_alertView);
    p_alertView = NULL;
//    std::map<std::string,
//    std::string> key_value;
//    char temurl[200];
//    sprintf(temurl, "http://h5.9miao.com/pic?num=1&tag=%s",imageTag[urlID]);
    std::map<std::string,std::string> key_value;
    key_value["tag"] = imageTag[urlID];
    key_value["page"]= "1";
    key_value["limit"]= "20";
    key_value["appid"]="10000";
    key_value["sign_method"]="md5";
    string tempSign = getSign(key_value);
    CCLog("sign===%s",tempSign.c_str()); 
    key_value["sign"] = tempSign;
    string tempUrl = "http://api.9miao.com/newsgirlpic/";
    CommonHttpManager::getInstance()->send_get(tempUrl, key_value, this,
                                               CommonHttpJson_selector(CDNewsImageController::onRequestFinished));
    {
        p_pLoading = CAActivityIndicatorView::createWithLayout(DLayout(DHorizontalLayout_W_C(50, 0.5), DVerticalLayout_T_H(50, 0.5)));
        this->getView()->insertSubview(p_pLoading, CAWindowZOderTop);
        p_pLoading->setLoadingMinTime(0.5f);
        p_pLoading->setTargetOnCancel(this, callfunc_selector(CDNewsImageController::initImageCollectionView));
    }
}

void CDNewsImageController::scrollViewHeaderBeginRefreshing(CrossApp::CAScrollView *view)
{
    std::map<std::string,std::string> key_value;
    key_value["tag"] = imageTag[urlID];
    key_value["page"]= "1";
    key_value["limit"]= "20";
    key_value["appid"]="10000";
    key_value["sign_method"]="md5";
    string tempSign = getSign(key_value);
    CCLog("sign===%s",tempSign.c_str()); 
    key_value["sign"] = tempSign;
    string tempUrl = "http://api.9miao.com/newsgirlpic/";
    CommonHttpManager::getInstance()->send_get(tempUrl, key_value, this,
                                               CommonHttpJson_selector(CDNewsImageController::onRequestFinished));
}

void CDNewsImageController::scrollViewFooterBeginRefreshing(CAScrollView* view)
{
    p_section++;
    std::map<std::string,std::string> key_value;
    key_value["tag"] = imageTag[urlID];
    key_value["page"]= crossapp_format_string("%d",p_section);
    key_value["limit"]= "20";
    key_value["appid"]="10000";
    key_value["sign_method"]="md5";
    string tempSign = getSign(key_value);
    CCLog("sign===%s",tempSign.c_str());
    key_value["sign"] = tempSign;
    string tempUrl = "http://api.9miao.com/newsgirlpic/";
    CommonHttpManager::getInstance()->send_get(tempUrl, key_value, this,
                                               CommonHttpJson_selector(CDNewsImageController::onRefreshRequestFinished));
}

void CDNewsImageController::scrollViewStopMoved(CrossApp::CAScrollView *view)
{
    CAVector<CACollectionViewCell*> temVec = p_AutoCollectionView->displayingCollectionCell();
    
    
    for(int i=0;i<temVec.size();i++)
    {
        CDNewsImagecollectionCell* cell = (CDNewsImagecollectionCell*)temVec.at(i);
        int row = cell->getItem();
        int img_num = m_ImageNum[row];
        
        if (img_num == 1)
        {
            cell->getImage1()->setUrl(m_ImageMsg[row].m_imageUrl[0]);
        }
        else if (img_num == 2)
        {
            cell->getImage1()->setUrl(m_ImageMsg[row].m_imageUrl[0]);
            cell->getImage2()->setUrl(m_ImageMsg[row].m_imageUrl[1]);
        }
        else if (img_num == 3)
        {
            cell->getImage1()->setUrl(m_ImageMsg[row].m_imageUrl[0]);
            cell->getImage2()->setUrl(m_ImageMsg[row].m_imageUrl[1]);
            cell->getImage3()->setUrl(m_ImageMsg[row].m_imageUrl[2]);
        }
        else
        {
            cell->getImage1()->setUrl(m_ImageMsg[row].m_imageUrl[0]);
            cell->getImage2()->setUrl(m_ImageMsg[row].m_imageUrl[1]);
            cell->getImage3()->setUrl(m_ImageMsg[row].m_imageUrl[2]);
            cell->getImage4()->setUrl(m_ImageMsg[row].m_imageUrl[3]);
        }
    }
}

void CDNewsImageController::onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        const CSJson::Value& value = json["result"];
        int length = value.size();
        
        m_ImageMsg.clear();
        for (int index = 0; index < length; index++)
        {
            newsImage temp_msg;
            temp_msg.m_title = value[index]["title"].asString();
            for (int i=0; i<value[index]["picon"].size(); i++) {
                string temp_pic = value[index]["picon"][i]["pic"].asString();
                string temp_dsc = value[index]["picon"][i]["desc"].asString();
                temp_msg.m_imageUrl.push_back(temp_pic);
                temp_msg.m_imageDesc.push_back(temp_dsc);
            }

            m_ImageMsg.push_back(temp_msg);
            m_ImageNum.push_back((int)getRandNum());
        }
        
    }
    
    do
    {
        CC_BREAK_IF(p_pLoading == NULL);
        if (p_pLoading->isAnimating())
        {
            p_pLoading->stopAnimating();
        }
        else
        {
            p_AutoCollectionView->reloadData();
        }
    }
    while (0);
}

void CDNewsImageController::onRefreshRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        const CSJson::Value& value = json["msg"];
        int length = value.size();
        for (int index = 0; index < length; index++)
        {
            newsImage temp_msg;
            temp_msg.m_title = value[index]["title"].asString();
            for (int i=0; i<value[index]["picon"].size(); i++) {
                string temp_pic = value[index]["picon"][i]["pic"].asString();
                string temp_dsc = value[index]["picon"][i]["desc"].asString();
                temp_msg.m_imageUrl.push_back(temp_pic);
                temp_msg.m_imageDesc.push_back(temp_dsc);
            }
            
            m_ImageMsg.push_back(temp_msg);
            m_ImageNum.push_back((int)getRandNum());
        }
        
    }else{
        p_section--;
    }
    
    do
    {
        CC_BREAK_IF(p_pLoading == NULL);
        if (p_pLoading->isAnimating())
        {
            p_pLoading->stopAnimating();
        }
        else
        {
            p_AutoCollectionView->reloadData();
        }
    }
    while (0);
}

void CDNewsImageController::viewDidUnload()
{
    
}

void CDNewsImageController::collectionViewDidSelectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item)
{
    CCLog("title====%d",(int)getRandNum());
    CDShowNewsImage* _controller = new CDShowNewsImage();
    _controller->init();
    _controller->setTitle(" ");
    
    _controller->autorelease();
    RootWindow::getInstance()->getDrawerController()->hideLeftViewController(true);
    RootWindow::getInstance()->getRootNavigationController()->pushViewController(_controller, true);
    _controller->initNewsImageView(m_ImageMsg[item]);
}

void CDNewsImageController::collectionViewDidDeselectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item)
{
    
}

CACollectionViewCell* CDNewsImageController::collectionCellAtIndex(CAAutoCollectionView* collectionView, const DSize& cellSize, unsigned int section, unsigned int item)
{
    CDNewsImagecollectionCell* cell = dynamic_cast<CDNewsImagecollectionCell*>(collectionView->dequeueReusableCellWithIdentifier("CrossApp"));
    if (cell == NULL)
    {
        cell = CDNewsImagecollectionCell::create("CrossApp");
        cell->initWithCell(1);
        
    }
    CALabel* cellText = (CALabel*)cell->getContentView()->getSubviewByTag(100);
    cellText->setText(m_ImageMsg[item].m_title);
    
    CALabel* cellTextdsc = (CALabel*)cell->getContentView()->getSubviewByTag(101);
    cellTextdsc->setText(m_ImageMsg[item].m_imageDesc[1]);
    
    int img_num = m_ImageNum[item];
    
    if (img_num==1)
    {
        cell->getImage1()->setVisible(true);
        cell->getImage1()->setLayout(DLayout(DHorizontalLayout_L_C(1, 0.5), DVerticalLayout_T_C(1, 0.5)));
        cell->getImage1()->setImage(CAImage::create("image/HelloWorld.png"));
        //cell->getImage1()->setUrl(m_ImageMsg[item].m_imageUrl[0]);
        
        cell->getImage2()->setVisible(false);
        cell->getImage3()->setVisible(false);
        cell->getImage4()->setVisible(false);
    }
    else if(img_num==2)
    {
        cell->getImage1()->setVisible(true);
        cell->getImage1()->setLayout(DLayout(DHorizontalLayout_L_C(1, 0.25), DVerticalLayout_T_C(1, 0.5)));
        cell->getImage1()->setImage(CAImage::create("image/HelloWorld.png"));
        //cell->getImage1()->setUrl(m_ImageMsg[item].m_imageUrl[0]);
        
        cell->getImage2()->setVisible(true);
        cell->getImage2()->setLayout(DLayout(DHorizontalLayout_R_C(1, 0.75), DVerticalLayout_T_C(1, 0.5)));
        cell->getImage2()->setImage(CAImage::create("image/HelloWorld.png"));
        //cell->getImage2()->setUrl(m_ImageMsg[item].m_imageUrl[1]);
        
        cell->getImage3()->setVisible(false);
        cell->getImage4()->setVisible(false);
    }
    else if(img_num==3)
    {
        cell->getImage1()->setVisible(true);
        cell->getImage1()->setLayout(DLayout(DHorizontalLayout_L_C(1, 0.25), DVerticalLayout_T_C(1, 0.5)));
        cell->getImage1()->setImage(CAImage::create("image/HelloWorld.png"));
        //cell->getImage1()->setUrl(m_ImageMsg[item].m_imageUrl[0]);
        
        cell->getImage2()->setVisible(true);
        cell->getImage2()->setLayout(DLayout(DHorizontalLayout_R_C(1, 0.75), DVerticalLayout_T_C(1, 0.25)));
        cell->getImage2()->setImage(CAImage::create("image/HelloWorld.png"));
        //cell->getImage2()->setUrl(m_ImageMsg[item].m_imageUrl[1]);
        
        cell->getImage3()->setVisible(true);
        cell->getImage3()->setLayout(DLayout(DHorizontalLayout_R_C(1, 0.75), DVerticalLayout_B_C(1, 0.75)));
        cell->getImage3()->setImage(CAImage::create("image/HelloWorld.png"));
        //cell->getImage3()->setUrl(m_ImageMsg[item].m_imageUrl[2]);
        
        cell->getImage4()->setVisible(false);
    }
    else if(img_num==4)
    {
        cell->getImage1()->setVisible(true);
        cell->getImage1()->setLayout(DLayout(DHorizontalLayout_L_C(1, 0.25), DVerticalLayout_T_C(1, 0.25)));
        cell->getImage1()->setImage(CAImage::create("image/HelloWorld.png"));
        //cell->getImage1()->setUrl(m_ImageMsg[item].m_imageUrl[0]);
        
        cell->getImage2()->setVisible(true);
        cell->getImage2()->setLayout(DLayout(DHorizontalLayout_R_C(1, 0.75), DVerticalLayout_T_C(1, 0.25)));
        cell->getImage2()->setImage(CAImage::create("image/HelloWorld.png"));
        //cell->getImage2()->setUrl(m_ImageMsg[item].m_imageUrl[1]);
        
        cell->getImage3()->setVisible(true);
        cell->getImage3()->setLayout(DLayout(DHorizontalLayout_R_C(1, 0.75), DVerticalLayout_B_C(1, 0.75)));
        cell->getImage3()->setImage(CAImage::create("image/HelloWorld.png"));
        //cell->getImage3()->setUrl(m_ImageMsg[item].m_imageUrl[2]);
        
        cell->getImage4()->setVisible(true);
        cell->getImage4()->setLayout(DLayout(DHorizontalLayout_L_C(1, 0.25), DVerticalLayout_B_C(1, 0.75)));
        cell->getImage4()->setImage(CAImage::create("image/HelloWorld.png"));
        //cell->getImage4()->setUrl(m_ImageMsg[item].m_imageUrl[3]);
    }
    return cell;
}

DSize CDNewsImageController::collectionViewSizeForItemAtIndexPath(CAAutoCollectionView* collectionView, unsigned int section, unsigned int item)
{
    return DSize(640,560);
}

unsigned int CDNewsImageController::numberOfItemsInSection(CAAutoCollectionView *collectionView, unsigned int section)
{
    return (unsigned int)m_ImageMsg.size();
}

unsigned int CDNewsImageController::numberOfSections(CAAutoCollectionView *collectionView)
{
    return 1;
}

float CDNewsImageController::getRandNum()
{
    return CCRANDOM_0_1()*4+1;
}