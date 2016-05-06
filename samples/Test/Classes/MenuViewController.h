//
//  MenuViewController.h
//  Test
//
//  Created by renhongguang on 15/4/3.
//
//

#ifndef __Test__MenuViewController__
#define __Test__MenuViewController__

#include "RootWindow.h"

class MenuViewController : public CAViewController, CATableViewDelegate,CATableViewDataSource
{
public:
    
    MenuViewController();
    
    virtual ~MenuViewController();
    
    CREATE_FUNC(MenuViewController);
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
    void changeStatusBarOrientation(CAObject* obj);
    
public:
    virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
    virtual CATableViewCell* tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row);
    virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
    virtual unsigned int numberOfSections(CATableView *table);
    virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
private:

    CATableView* tableView;
    
    CAImageView* m_pLogo;
};

#endif /* defined(__Test__MenuViewController__) */
