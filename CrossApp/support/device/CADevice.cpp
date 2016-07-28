//
//  CADevice.cpp
//  PublishPhoto
//
//  Created by lihui on 14-10-21.
//
//

#include "CADevice.h"

NS_CC_BEGIN

namespace CADevice
{
    
    const char* getSystemVersionWithIOS()
    {
        return "";
    }
    
    const char* getAppVersion()
    {
        return "";
    }
    
    void openCamera(CAMediaDelegate* target)
    {
        
    }
    
    void openAlbum(CAMediaDelegate* target)
    {
        
    }
    
    std::vector<CAAddressBookRecord> getAddressBook()
    {
        std::vector<CAAddressBookRecord> _addressBookArr;
        return _addressBookArr;
    }

    float getScreenBrightness()
    {
        return 1.0f;
    }
    void setScreenBrightness(float brightness)
    {
        
    }
    
    void updateVersion(const std::string &url
                       ,unsigned int versionNumber
                       ,const std::string &appId)
    {
        
    }
    
    CANetWorkType getNetWorkType()
    {
        return CANetWorkTypeWifi;
    }
    
	void setVolume(float sender, CAVolumeType type)
    {
        
    }
    
	float getVolume(CAVolumeType type)
    {
        return 1.0f;
    }
    void OpenURL(const std::string &url)
    {
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
        ::ShellExecuteA(NULL, "open", url.c_str(), NULL, NULL, SW_SHOW);
#else
        
#endif
		
    }
    
    float getBatteryLevel()
    {
        return 0.0f;
    }
    
    bool isNetWorkAvailble()
    {
        return 0;
    }
    
    void sendLocalNotification(const char* title,const char* content,int time)
    {
        
    }

	void getWifiListWithAndroid(CAWifiDelegate *target)
	{

	}

    CAWifiInfo getWifiConnectionInfo()
    {
        return CAWifiInfo();
    }
    
    void initBlueTooth(CABlueToothDelegate *target)
    {
        
    }
    
    void setBlueToothType(CABlueToothType type)
    {
        
    }
	
	void writeToSavedPhotosAlbum(CAImage* image, const std::string &imageName)
	{
		
	}

	void startUpdateLocation(CALocationDelegate* gpsDelegate)
	{

	}

	void stopUpdateLocation()
	{

	}
    
    void startAccelerometer(CAAccelerometerDelegate* delegate)
    {
        
    }
    
    void setAccelerometerInterval(float interval)
    {
        
    }

    void stopAccelerometer()
    {
        
    }

	CC_DLL void startGyroscope(CAGyroDelegate* delegate)
	{

	}

	CC_DLL void setGyroInterval(float interval)
	{

	}

	CC_DLL void stopGyroscope()
	{

	}

}

NS_CC_END