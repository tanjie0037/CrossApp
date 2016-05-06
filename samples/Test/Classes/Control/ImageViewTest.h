
#ifndef __Test__ImageViewTest__
#define __Test__ImageViewTest__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

#define MAX_COUNT 5  

class ImageViewTest: public CAViewController
{
    
public:

	ImageViewTest();
    
	virtual ~ImageViewTest();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
public:
    
    CAImageView* AnimationImages();
    
};


#endif /* defined(__HelloCpp__ViewController__) */
