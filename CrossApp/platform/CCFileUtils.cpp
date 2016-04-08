

#include "CCFileUtils.h"
#include "basics/CAApplication.h"
#include "CCSAXParser.h"
#include "support/tinyxml2/tinyxml2.h"
#include "support/zip_support/unzip.h"
#include <stack>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <dirent.h>
#endif

using namespace std;

NS_CC_BEGIN

CCFileUtils* CCFileUtils::s_sharedFileUtils = NULL;

void CCFileUtils::purgeFileUtils()
{
    CC_SAFE_DELETE(s_sharedFileUtils);
}

CCFileUtils::CCFileUtils()
, _indexBuild(0)
{
}

CCFileUtils::~CCFileUtils()
{
    m_mFilenameLookupDict.clear();
}

bool CCFileUtils::init()
{
    m_searchPathArray.push_back(m_strDefaultResRootPath);
    m_searchResolutionsOrderArray.push_back("");
    return true;
}

void CCFileUtils::purgeCachedEntries()
{
    m_fullPathCache.clear();
}

unsigned char* CCFileUtils::getFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize)
{
    unsigned char * pBuffer = NULL;
    CCAssert(pszFileName != NULL && pSize != NULL && pszMode != NULL, "Invalid parameters.");
    *pSize = 0;
    do
    {
        // read the file from hardware
        std::string fullPath = fullPathForFilename(pszFileName);
        FILE *fp = fopen(fullPath.c_str(), pszMode);
        CC_BREAK_IF(!fp);
        
        fseek(fp,0,SEEK_END);
        *pSize = ftell(fp);
        fseek(fp,0,SEEK_SET);
        pBuffer = new unsigned char[*pSize];
        *pSize = fread(pBuffer,sizeof(unsigned char), *pSize,fp);
        fclose(fp);
    } while (0);
    
    if (! pBuffer)
    {
        std::string msg = "Get data from file(";
        msg.append(pszFileName).append(") failed!");
        
        CCLOG("%s", msg.c_str());
    }
    return pBuffer;
}

unsigned char* CCFileUtils::getFileDataFromZip(const char* pszZipFilePath, const char* pszFileName, unsigned long * pSize)
{
    unsigned char * pBuffer = NULL;
    unzFile pFile = NULL;
    *pSize = 0;

    do 
    {
        CC_BREAK_IF(!pszZipFilePath || !pszFileName);
        CC_BREAK_IF(strlen(pszZipFilePath) == 0);

        pFile = unzOpen(pszZipFilePath);
        CC_BREAK_IF(!pFile);

        int nRet = unzLocateFile(pFile, pszFileName, 1);
        CC_BREAK_IF(UNZ_OK != nRet);

        char szFilePathA[260];
        unz_file_info FileInfo;
        nRet = unzGetCurrentFileInfo(pFile, &FileInfo, szFilePathA, sizeof(szFilePathA), NULL, 0, NULL, 0);
        CC_BREAK_IF(UNZ_OK != nRet);

        nRet = unzOpenCurrentFile(pFile);
        CC_BREAK_IF(UNZ_OK != nRet);

        pBuffer = new unsigned char[FileInfo.uncompressed_size];
        int CC_UNUSED nSize = unzReadCurrentFile(pFile, pBuffer, FileInfo.uncompressed_size);
        CCAssert(nSize == 0 || nSize == (int)FileInfo.uncompressed_size, "the file size is wrong");

        *pSize = FileInfo.uncompressed_size;
        unzCloseCurrentFile(pFile);
    } while (0);

    if (pFile)
    {
        unzClose(pFile);
    }

    return pBuffer;
}

std::string CCFileUtils::getFileString(const char* pszFilePath)
{
    std::string data;
    
    unsigned long pSize = 0;
    unsigned char* pData = this->getFileData(pszFilePath, "rb", &pSize);

	if (pSize > 0)
	{
		data.resize(pSize);
		memcpy(&data[0], pData, pSize);
	}
    return data;
}

std::string CCFileUtils::getNewFilename(const std::string& pszFileName)
{
    std::string pszNewFileName = m_mFilenameLookupDict.count(pszFileName) > 0 ? m_mFilenameLookupDict.at(pszFileName) : "";
    if(pszNewFileName.empty())
    {
        return pszFileName;
    }
    return pszNewFileName;
}


std::string CCFileUtils::getPathForFilename(const std::string& filename, const std::string& resolutionDirectory, const std::string& searchPath)
{
    std::string file = filename;
    std::string file_path = "";
    size_t pos = filename.find_last_of("/");
    if (pos != std::string::npos)
    {
        file_path = filename.substr(0, pos+1);
        file = filename.substr(pos+1);
    }
    
    // searchPath + file_path + resourceDirectory
    std::string path = searchPath;
    path += file_path;
    path += resolutionDirectory;
    
    path = getFullPathForDirectoryAndFilename(path, file);
    
    return path;
}


std::string CCFileUtils::fullPathForFilename(const std::string& pszFileName)
{
    std::string strFileName = pszFileName;
    if (isAbsolutePath(pszFileName))
    {
        //CCLOG("Return absolute path( %s ) directly.", pszFileName);
        return pszFileName;
    }
    
    //jie.tan: 使用__index匹配实际文件名
#if defined(__FILE_INDEX) && (__FILE_INDEX == 1)
    strFileName = getIndexMap()[pszFileName];
    
    if (strFileName == "") {
        strFileName = std::string(pszFileName);
    }
#endif
    //jie.tan: 使用__index匹配实际文件名
    
    // Already Cached ?
    std::map<std::string, std::string>::iterator cacheIter = m_fullPathCache.find(strFileName);
    if (cacheIter != m_fullPathCache.end())
    {
        //CCLOG("Return full path from cache: %s", cacheIter->second.c_str());
        return cacheIter->second;
    }
    // Get the new file name.
    std::string newFilename = getNewFilename(strFileName);
    
    string fullpath = "";
    
    std::vector<std::string>::iterator searchPathsIter;
    for (searchPathsIter = m_searchPathArray.begin();
         searchPathsIter != m_searchPathArray.end();
         ++searchPathsIter)
    {
        std::vector<std::string>::iterator resOrderIter;
        for (resOrderIter = m_searchResolutionsOrderArray.begin();
             resOrderIter != m_searchResolutionsOrderArray.end();
             ++resOrderIter)
        {
            fullpath = this->getPathForFilename(newFilename, *resOrderIter, *searchPathsIter);
            
            if (fullpath.length() > 0)
            {
                // Using the filename passed in as key.
                m_fullPathCache.insert(std::pair<std::string, std::string>(strFileName, fullpath));
                return fullpath;
            }
        }
    }
    
    return strFileName;
}

const char* CCFileUtils::fullPathFromRelativeFile(const char *pszFilename, const char *pszRelativeFile)
{
    std::string relativeFile = pszRelativeFile;
    std::string ret = relativeFile.substr(0, relativeFile.rfind('/')+1);
    ret += getNewFilename(pszFilename);
    return ret.c_str();
}

void CCFileUtils::setSearchResolutionsOrder(const std::vector<std::string>& searchResolutionsOrder)
{
    bool bExistDefault = false;
    m_fullPathCache.clear();
    m_searchResolutionsOrderArray.clear();
    for (std::vector<std::string>::const_iterator iter = searchResolutionsOrder.begin(); iter != searchResolutionsOrder.end(); ++iter)
    {
        std::string resolutionDirectory = *iter;
        if (!bExistDefault && resolutionDirectory == "")
        {
            bExistDefault = true;
        }
        
        if (resolutionDirectory.length() > 0 && resolutionDirectory[resolutionDirectory.length()-1] != '/')
        {
            resolutionDirectory += "/";
        }
        
        m_searchResolutionsOrderArray.push_back(resolutionDirectory);
    }
    if (!bExistDefault)
    {
        m_searchResolutionsOrderArray.push_back("");
    }
}

void CCFileUtils::addSearchResolutionsOrder(const char* order)
{
    m_searchResolutionsOrderArray.push_back(order);
}

const std::vector<std::string>& CCFileUtils::getSearchResolutionsOrder()
{
    return m_searchResolutionsOrderArray;
}

const std::vector<std::string>& CCFileUtils::getSearchPaths()
{
    return m_searchPathArray;
}

void CCFileUtils::setSearchPaths(const std::vector<std::string>& searchPaths)
{
    bool bExistDefaultRootPath = false;

    m_fullPathCache.clear();
    m_searchPathArray.clear();
    for (std::vector<std::string>::const_iterator iter = searchPaths.begin(); iter != searchPaths.end(); ++iter)
    {
        std::string strPrefix;
        std::string path;
        if (!isAbsolutePath(*iter))
        { // Not an absolute path
            strPrefix = m_strDefaultResRootPath;
        }
        path = strPrefix+(*iter);
        if (path.length() > 0 && path[path.length()-1] != '/')
        {
            path += "/";
        }
        if (!bExistDefaultRootPath && path == m_strDefaultResRootPath)
        {
            bExistDefaultRootPath = true;
        }
        m_searchPathArray.push_back(path);
    }
    
    if (!bExistDefaultRootPath)
    {
        //CCLOG("Default root path doesn't exist, adding it.");
        m_searchPathArray.push_back(m_strDefaultResRootPath);
    }
}

void CCFileUtils::addSearchPath(const char* path_)
{
    std::string strPrefix;
    std::string path(path_);
    if (!isAbsolutePath(path))
    { // Not an absolute path
        strPrefix = m_strDefaultResRootPath;
    }
    path = strPrefix + path;
    if (path.length() > 0 && path[path.length()-1] != '/')
    {
        path += "/";
    }
    m_searchPathArray.push_back(path);
}

void CCFileUtils::removeSearchPath(const char *path_)
{
	std::string strPrefix;
	std::string path(path_);
	if (!isAbsolutePath(path))
	{ // Not an absolute path
		strPrefix = m_strDefaultResRootPath;
	}
	path = strPrefix + path;
	if (path.length() > 0 && path[path.length()-1] != '/')
	{
		path += "/";
	}
	std::vector<std::string>::iterator iter = std::find(m_searchPathArray.begin(), m_searchPathArray.end(), path);
	m_searchPathArray.erase(iter);
}

void CCFileUtils::removeAllPaths()
{
	m_searchPathArray.clear();
}

void CCFileUtils::setFilenameLookupDictionary(const std::map<std::string, std::string> &pFilenameLookupDict)
{
    m_fullPathCache.clear();
    m_mFilenameLookupDict.clear();
    m_mFilenameLookupDict = pFilenameLookupDict;
}

std::string CCFileUtils::getFullPathForDirectoryAndFilename(const std::string& strDirectory, const std::string& strFilename)
{
    std::string ret = strDirectory;
    if (strDirectory.size() && strDirectory[strDirectory.size()-1] != '/')
    {
        ret += '/';
    }
    ret += strFilename;
    
    if (!isFileExist(ret)) {
        ret = "";
    }
    return ret;
}

bool CCFileUtils::isAbsolutePath(const std::string& strPath)
{
    return strPath[0] == '/' ? true : false;
}

//////////////////////////////////////////////////////////////////////////
// Notification support when getFileData from invalid file path.
//////////////////////////////////////////////////////////////////////////
static bool s_bPopupNotify = true;

void CCFileUtils::setPopupNotify(bool bNotify)
{
    s_bPopupNotify = bNotify;
}

bool CCFileUtils::isPopupNotify()
{
    return s_bPopupNotify;
}

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#endif
/*
 * Create a direcotry is platform depended.
 */
bool CCFileUtils::createDirectory(const char *path)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    DIR *dir=NULL;
    dir = opendir(path);
    if (dir)
    {
        closedir(dir);
        return false;
    }
    
    mode_t processMask = umask(0);
    int ret = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
    umask(processMask);
    if (ret != 0 && (errno != EEXIST))
    {
        return false;
    }
    return true;
#else
    BOOL ret = CreateDirectoryA(path, NULL);
	if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
	{
		return false;
	}
    return true;
#endif
}

void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c)
{
    std::string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while(std::string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2-pos1));
        
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length())
        v.push_back(s.substr(pos1));
}

//add by jie.tan
void CCFileUtils::cleanIndex() {
    _indexBuild = 0;
    _indexVersion = "";
    _indexMap.clear();
}

void CCFileUtils::loadIndex() {
    CCLOG("loadIndex!");
    
    do {
        string dataOut = getFileString((getWritablePath() + "__index").c_str());
        string dataIn = getFileString(getPathForFilename("__index", "", "Res/").c_str());
        string data;
        
        // 因为插入了一个'\0'
        if (dataOut.length() < 2 && dataIn.length() < 2) {
            CCAssert(0, "no index found.");
            return;
        }
            
        if (dataOut.length() < 2) {
            data = dataIn;
            
        } else {
            // 选择index build高的那个为准
            string buildStrIn, buildStrOut;
            
            size_t firstEnter = dataIn.find("\n");
            buildStrIn = dataIn.substr(firstEnter + 1, dataIn.find("\n", firstEnter + 1) - firstEnter - 1);
            
            firstEnter = dataOut.find("\n");
            buildStrOut = dataOut.substr(firstEnter + 1, dataOut.find("\n", firstEnter + 1) - firstEnter - 1);
            
            CCLog("compare index, out:[%s], in:[%s]", buildStrOut.c_str(), buildStrIn.c_str());
            
            long buildIn = atol(buildStrIn.c_str());
            long buildOut = atol(buildStrOut.c_str());
            
            data = buildIn > buildOut ? dataIn : dataOut;
        }
        
        data.find("\n");
        
        CCLOG("---index:------------\n%s\n------------", data.c_str());
        
        vector<string> v =  vector<string>();
        
        SplitString(data, v, "\n");
        
        if (v.size() < 2) {
            assert(0);
            break;
        }
        
        _indexVersion = v[0];
        _indexBuild = atol(v[1].c_str());
        _indexMap.clear();
        _fileSizeMap.clear();
        
        for (vector<string>::size_type i = 2; i != v.size(); ++i) {
            if (v[i].length() < 2) {
                continue;
            }
            
            vector<string> v_line =  vector<string>();
            SplitString(v[i], v_line, " ");
            
            if (v_line.size() != 3) {
                CCLOG("v[%lu]: [%s]", i, v[i].c_str());
                assert(0);
                continue;
            }
            
            _indexMap[v_line[0]] = v_line[1];
            _fileSizeMap[v_line[1]] = atol(v_line[2].c_str());
        }
    } while(0);
}

string CCFileUtils::getIndexVersion() {
#if defined(__FILE_INDEX) && (__FILE_INDEX == 1)
    if (_indexVersion == "") {
        loadIndex();
    }
    
    if (_indexVersion == "") {
        CCLOG("__index error!");
        assert(0);
    }
#endif
    
    return _indexVersion;
}

unsigned int CCFileUtils::getIndexBuild() {
#if defined(__FILE_INDEX) && (__FILE_INDEX == 1)
    if (_indexBuild == 0) {
        loadIndex();
    }
    
    if (_indexBuild == 0) {
        CCLOG("__index error!");
        assert(0);
    }
#endif
    
    return _indexBuild;
}

std::map<std::string, std::string> & CCFileUtils::getIndexMap() {
#if defined(__FILE_INDEX) && (__FILE_INDEX == 1)
    if (_indexMap.empty()) {
        loadIndex();
    }
    
    if (_indexMap.empty()) {
        CCLOG("__index error!");
        assert(0);
    }
#endif
    
    return _indexMap;
}

unsigned long CCFileUtils::getFileSize(const std::string &file) {
#if defined(__FILE_INDEX) && (__FILE_INDEX == 1)
    if (_fileSizeMap.empty()) {
        loadIndex();
    }
    
    if (_fileSizeMap.empty()) {
        CCLOG("__index error!");
        assert(0);
    }
#endif
    
    map<string, unsigned long>::iterator it = _fileSizeMap.find(file);
    
    if (it != _fileSizeMap.end()) {
        return it->second;
    } else {
        return 0;
    }
}

unsigned long CCFileUtils::saveFile(const std::string &path, const std::string &data) {
    string fullPath = CCFileUtils::sharedFileUtils()->getWritablePath();
    
    //check path
    if (path.find("/") == string::npos) {
        fullPath += path;
        
    } else {
        // remove '/'
        fullPath = fullPath.substr(0, fullPath.size() - 1);
        
        vector<std::string> v;
        SplitString(path, v, "/");
        vector<std::string>::size_type i = 0;
        
        while (i < v.size() - 1) {
            fullPath += "/" + v[i];
            createDirectory(fullPath.c_str());
            ++i;
        }
    
        fullPath += "/" + v[i];
    }
    
    CCLOG("---save file fullPath:%s", fullPath.c_str());
    
    ofstream file(fullPath.c_str(), ios::out);
    
    if (!file) {
        assert(0);
        CCLOG("file path not exist!");
        return 0;
    }
    
    file.write(data.c_str(), data.size());
    file.close();
    
    long nFileLen = 0;
    ifstream savedFile(fullPath.c_str(), ios::binary);
    
    if (savedFile) {
        savedFile.seekg(0, ios::end);
        nFileLen = savedFile.tellg();
        savedFile.close();
    }
    
    return nFileLen;
}

//add by jie.tan

NS_CC_END

