
#include "AutoCollectionViewVerticalTest.h"
#include "CDUIShowAutoCollectionView.h"

std::vector<CAColor4B> VerticalcolorArr;

ECollectionViewVerticalCell::ECollectionViewVerticalCell()
{
    this->setAllowsSelected(false);
}

ECollectionViewVerticalCell::~ECollectionViewVerticalCell()
{
    
}

ECollectionViewVerticalCell* ECollectionViewVerticalCell::create(const std::string& reuseIdentifier)
{
    ECollectionViewVerticalCell* cell = new ECollectionViewVerticalCell();
    if (cell && cell->initWithReuseIdentifier(reuseIdentifier))
    {
        cell->autorelease();
        return cell;
    }
    CC_SAFE_DELETE(cell);
    return NULL;
}

void ECollectionViewVerticalCell::initWithCell()
{
    CALabel* test = CALabel::createWithLayout(DLayoutFill);
    test->setColor(ccc4(34, 151, 254, 255));
    test->setTextAlignment(CATextAlignmentCenter);
    test->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    test->setFontSize(28);
    test->setTag(100);
    this->addSubview(test);
}

void ECollectionViewVerticalCell::highlightedCollectionViewCell()
{
    this->setBackgroundView(CAView::createWithColor(CAColor_clear));
}

void ECollectionViewVerticalCell::selectedCollectionViewCell()
{
    this->setBackgroundView(CAView::createWithColor(CAColor_clear));
}

EAutoCollectionViewVertical::EAutoCollectionViewVertical()
{
    
}

EAutoCollectionViewVertical::~EAutoCollectionViewVertical()
{
    
}

bool EAutoCollectionViewVertical::init()
{
    p_AutoCollection = CAAutoCollectionView::createWithLayout(DLayoutFill);
    p_AutoCollection->setAllowsSelection(true);
    p_AutoCollection->setAllowsMultipleSelection(true);
    p_AutoCollection->setCollectionViewDelegate(this);
    p_AutoCollection->setCollectionViewDataSource(this);
    p_AutoCollection->setOrientation(CAAutoCollectionView::Vertical);
    p_AutoCollection->setCellVertAlign(CAAutoCollectionView::VertAlignCenter);
    p_AutoCollection->setHoriCellInterval(20);
    p_AutoCollection->setVertCellInterval(20);
    this->addSubview(p_AutoCollection);
    return true;
}

EAutoCollectionViewVertical* EAutoCollectionViewVertical::createWithLayout(const DLayout& layout)
{
    EAutoCollectionViewVertical* collectionView = new EAutoCollectionViewVertical();
    if (collectionView && collectionView->initWithLayout(layout))
    {
        collectionView->autorelease();
        return collectionView;
    }
    CC_SAFE_DELETE(collectionView);
    return NULL;
}

void EAutoCollectionViewVertical::scrollViewHeaderBeginRefreshing(CAScrollView* view)
{
    //    CAScheduler::schedule(schedule_selector(AutoCollectionViewVerticalTest::refreshData1), this, 0.1, 0, 1.0f + CCRANDOM_0_1() * 2);
    CAView* viewColor = createWithColor(CAColor_blue);
    viewColor->setLayout(DLayoutFill);
    
    this->addSubview(viewColor);
}

void EAutoCollectionViewVertical::scrollViewFooterBeginRefreshing(CAScrollView* view)
{
    //    CAScheduler::schedule(schedule_selector(AutoCollectionViewVerticalTest::refreshData2), this, 0.1, 0, 1.0f + CCRANDOM_0_1() * 2);
    
}

void EAutoCollectionViewVertical::collectionViewDidSelectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item)
{
    CACollectionViewCell* cell = collectionView->cellForRowAtIndexPath(section, item);
    cell->getContentView()->setRotation(-360);
    cell->getContentView()->setScale(0.5f);
    CAViewAnimation::beginAnimations("", NULL);
    cell->getContentView()->setRotation(0);
    cell->getContentView()->setScale(1.0f);
    CAViewAnimation::commitAnimations();
}

void EAutoCollectionViewVertical::collectionViewDidDeselectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item)
{
}

CACollectionViewCell* EAutoCollectionViewVertical::collectionCellAtIndex(CAAutoCollectionView *collectionView, const DSize& cellSize, unsigned int section, unsigned int item)
{
    ECollectionViewVerticalCell* Cell = dynamic_cast<ECollectionViewVerticalCell*>(collectionView->dequeueReusableCellWithIdentifier("CrossApp"));
    
    if (Cell == NULL)
    {
        Cell = ECollectionViewVerticalCell::create("CrossApp");
        
        CAView* itemImage = CAView::createWithLayout(DLayoutFill);
        itemImage->setTag(99);
        Cell->getContentView()->addSubview(itemImage);
        
        CALabel* itemText = CALabel::createWithLayout(DLayoutFill);
        itemText->setTag(100);
        itemText->setFontSize(29);
        itemText->setTextAlignment(CATextAlignmentCenter);
        itemText->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        Cell->getContentView()->addSubview(itemText);
    }
    
    CAView* itemImageView = Cell->getContentView()->getSubviewByTag(99);
    itemImageView->setColor(VerticalcolorArr.at(item));
    CCLog("row = %d", item);
    
    char pos[20] = "";
    sprintf(pos, "(%d,%d)", section, item);
    CALabel* itemText = (CALabel*)Cell->getContentView()->getSubviewByTag(100);
    itemText->setText(pos);
    
    return Cell;
}

DSize EAutoCollectionViewVertical::collectionViewSizeForItemAtIndexPath(CAAutoCollectionView* collectionView, unsigned int section, unsigned int item)
{
    return DSize(CCRANDOM_0_1() * 200 + 130, CCRANDOM_0_1() * 200 + 130);
}

unsigned int EAutoCollectionViewVertical::numberOfItemsInSection(CAAutoCollectionView *collectionView, unsigned int section)
{
    return (unsigned int)VerticalcolorArr.size();
}

unsigned int EAutoCollectionViewVertical::numberOfSections(CAAutoCollectionView *collectionView)
{
    return 3;
}

CAView* EAutoCollectionViewVertical::collectionViewSectionViewForHeaderInSection(CAAutoCollectionView *collectionView, const DSize& viewSize, unsigned int section)
{
    CAView* view = CAView::createWithColor(CAColor_gray);
    return view;
}

unsigned int EAutoCollectionViewVertical::collectionViewHeightForHeaderInSection(CAAutoCollectionView *collectionView, unsigned int section)
{
    return 30;
}

CAView* EAutoCollectionViewVertical::collectionViewSectionViewForFooterInSection(CAAutoCollectionView *collectionView, const DSize& viewSize, unsigned int section)
{
    return NULL;
}

unsigned int EAutoCollectionViewVertical::collectionViewHeightForFooterInSection(CAAutoCollectionView *collectionView, unsigned int section)
{
    return 0;
}



int AutoCollectionViewVerticalNum = 0;

AutoCollectionViewVerticalTest::AutoCollectionViewVerticalTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);
    
    VerticalcolorArr.clear();
}

AutoCollectionViewVerticalTest::~AutoCollectionViewVerticalTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);
    AutoCollectionViewVerticalNum = 0;
}

void CDUIShowAutoCollectionView::AutoCollectionVerticalRightBtnRightcallback(CAControl* control, DPoint point)
{
    if (showAutoCollectionVerticalNavigationBar >= 1)
    {
        showAutoCollectionVerticalNavigationBar = 0;
        AutoCollectionViewVerticalNum = showAutoCollectionVerticalNavigationBar;
    }
    else
    {
        AutoCollectionViewVerticalNum = ++showAutoCollectionVerticalNavigationBar;
    }
    
    AutoCollectionViewVerticalTest* ViewContrllerAutoCollectionViewVerticalTest = new AutoCollectionViewVerticalTest();
    ViewContrllerAutoCollectionViewVerticalTest->init();
    ViewContrllerAutoCollectionViewVerticalTest->setNavigationBarItem(AutoCollectionVerticalNavigationBar);
    ViewContrllerAutoCollectionViewVerticalTest->autorelease();
    RootWindow::getInstance()->getRootNavigationController()->replaceViewController(ViewContrllerAutoCollectionViewVerticalTest, false);
}

void AutoCollectionViewVerticalTest::viewDidLoad()
{
    for (int i = 0; i < 40; i++)
    {
        char r = CCRANDOM_0_1() * 255;
        char g = CCRANDOM_0_1() * 255;
        char b = CCRANDOM_0_1() * 255;
        
        VerticalcolorArr.push_back(ccc4(r, g, b, 255));
    }
    
    headerRefreshView = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeHeader);
    footerRefreshView = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeFooter);
    
    if (AutoCollectionViewVerticalNum == 0)
    {
        p_AutoCollection = CAAutoCollectionView::createWithLayout(DLayoutFill);
        p_AutoCollection->setAllowsSelection(true);
        p_AutoCollection->setAllowsMultipleSelection(true);
        p_AutoCollection->setCollectionViewDelegate(this);
        p_AutoCollection->setCollectionViewDataSource(this);
        p_AutoCollection->setOrientation(CAAutoCollectionView::Vertical);
        p_AutoCollection->setHeaderRefreshView(headerRefreshView);
        p_AutoCollection->setFooterRefreshView(footerRefreshView);
        p_AutoCollection->setScrollViewDelegate(this);
        p_AutoCollection->setHoriCellInterval(20);
        p_AutoCollection->setVertCellInterval(20);
    
        this->getView()->addSubview(p_AutoCollection);
    }
    else
    {
        EAutoCollectionViewVertical* p_AutoCollection1 = EAutoCollectionViewVertical::createWithLayout(DLayoutFill);
        p_AutoCollection1->init();
        p_AutoCollection1->setEAutoCollectionViewVerticalDelegate(this);
        this->getView()->addSubview(p_AutoCollection1);
    }
}

void AutoCollectionViewVerticalTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void AutoCollectionViewVerticalTest::refreshData1(float interval)
{
    VerticalcolorArr.clear();
    for (int i = 0; i < 40; i++)
    {
        char r = CCRANDOM_0_1() * 255;
        char g = CCRANDOM_0_1() * 255;
        char b = CCRANDOM_0_1() * 255;
        VerticalcolorArr.push_back(ccc4(r, g, b, 255));
    }
    p_AutoCollection->reloadData();
}

void AutoCollectionViewVerticalTest::refreshData2(float interval)
{
    for (int i = 0; i < 40; i++)
    {
        char r = CCRANDOM_0_1() * 255;
        char g = CCRANDOM_0_1() * 255;
        char b = CCRANDOM_0_1() * 255;
        VerticalcolorArr.push_back(ccc4(r, g, b, 255));
    }
    p_AutoCollection->reloadData();
}

void AutoCollectionViewVerticalTest::scrollViewHeaderBeginRefreshing(CAScrollView* view)
{
    CAScheduler::schedule(schedule_selector(AutoCollectionViewVerticalTest::refreshData1), this, 0.1, 0, 1.0f + CCRANDOM_0_1() * 2);
}

void AutoCollectionViewVerticalTest::scrollViewFooterBeginRefreshing(CAScrollView* view)
{
    CAScheduler::schedule(schedule_selector(AutoCollectionViewVerticalTest::refreshData2), this, 0.1, 0, 1.0f + CCRANDOM_0_1() * 2);
}

void AutoCollectionViewVerticalTest::collectionViewDidSelectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item)
{
    CACollectionViewCell* cell = collectionView->cellForRowAtIndexPath(section, item);
    cell->getContentView()->setRotation(-360);
    cell->getContentView()->setScale(0.5f);
    CAViewAnimation::beginAnimations("", NULL);
    cell->getContentView()->setRotation(0);
    cell->getContentView()->setScale(1.0f);
    CAViewAnimation::commitAnimations();
}

void AutoCollectionViewVerticalTest::collectionViewDidDeselectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item)
{
}

CACollectionViewCell* AutoCollectionViewVerticalTest::collectionCellAtIndex(CAAutoCollectionView *collectionView, const DSize& cellSize, unsigned int section, unsigned int item)
{
    CACollectionViewCell* p_Cell = collectionView->dequeueReusableCellWithIdentifier("CrossApp");
    
    if (p_Cell == NULL)
    {
        p_Cell = CACollectionViewCell::create("CrossApp");
        
        CAView* itemImage = CAView::createWithLayout(DLayoutFill);
        itemImage->setTag(99);
        p_Cell->getContentView()->addSubview(itemImage);
        
        CALabel* itemText = CALabel::createWithLayout(DLayoutFill);
        itemText->setTag(100);
        itemText->setFontSize(29);
        itemText->setTextAlignment(CATextAlignmentCenter);
        itemText->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        p_Cell->getContentView()->addSubview(itemText);
    }
    
    CAView* itemImageView = p_Cell->getContentView()->getSubviewByTag(99);
    itemImageView->setColor(VerticalcolorArr.at(item));
    CCLog("row = %d", item);
    
    char pos[20] = "";
    sprintf(pos, "(%d,%d)", section, item);
    CALabel* itemText = (CALabel*)p_Cell->getContentView()->getSubviewByTag(100);
    itemText->setText(pos);
    return  p_Cell;
    
}

DSize AutoCollectionViewVerticalTest::collectionViewSizeForItemAtIndexPath(CAAutoCollectionView* collectionView, unsigned int section, unsigned int item)
{
    return DSize(230, 230);
}

unsigned int AutoCollectionViewVerticalTest::numberOfItemsInSection(CAAutoCollectionView *collectionView, unsigned int section)
{
    return (unsigned int)VerticalcolorArr.size();
}

unsigned int AutoCollectionViewVerticalTest::numberOfSections(CAAutoCollectionView *collectionView)
{
    return 1;
}
