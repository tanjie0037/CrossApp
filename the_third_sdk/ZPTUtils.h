//
//  ZPTUtils.h
//  AppGift
//
//  Created by tanjie on 15/8/24.
//
//

#ifndef AppGift_ZPTUtils_h
#define AppGift_ZPTUtils_h

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <vector>

using namespace std;

static char *str_copy(const char *str) {
    int len = (int)strlen(str) + 1;
    char *buf = (char *)malloc(len);
    if (NULL == buf) return NULL;
    memcpy(buf, str, len);
    return buf;
}

static char* str_replace(const char *str, const char *sub, const char *replace) {
    char *pos = (char *) str;
    char *find = strstr(str, sub);
    
    if (!find) return str_copy(str);
    int size = 128;
    
    char *result = (char *) malloc(size);
    if (NULL == result) return NULL;
    memset(result, '\0', size);
    char *current;
    
    while ((current = strstr(pos, sub))) {
        size_t len = current - pos;
        strncat(result, pos, len);
        strncat(result, replace, strlen(replace));
        pos = current + strlen(sub);
    }
    
    if (pos != (str + strlen(str))) {
        strncat(result, pos, (str - pos));
    }
    
    return result;
}

static string encodeURL( const string& s ) {
    
    string result;
    for ( size_t i = 0; i < s.length(); ++i ) {
        char c = s[i];
        
        if ( isalnum(c) ) {
            result += (char)c;
        } else {
            
            int8_t lowerBit = int8_t(c & 0x0f);
            int8_t upperBit = int8_t( (( c & 0xf0 ) >> 4) & 0x0f );
            
            char lowerBitChar = lowerBit >= 10 ? ('A' + (lowerBit - 10)) : ('0' + lowerBit);
            char upperBitChar = upperBit >= 10 ? ('A' + (upperBit - 10)) : ('0' + upperBit);
            result += "%";
            result += upperBitChar;
            result += lowerBitChar;
        }
    }
    
    return result;
}

static string decodeURL( const string& s ) {
    string result;
    
    for ( size_t i = 0; i < s.length(); ) {
        char c = s[i];
        
        if ( c == '%' ) {
            
            char upperBitChar = 0, lowerBitChar = 0;
            size_t skip = 1;
            if ( i + 1 < s.length() ) {
                upperBitChar = s[i+1];
                ++skip;
            }
            
            if ( i + 2 < s.length() ) {
                lowerBitChar = s[i+2];
                ++skip;
            }
            
            if ( upperBitChar >= '0' && upperBitChar <= '9' ) {
                upperBitChar -= '0';
            } else if ( upperBitChar >= 'A' && upperBitChar <= 'F' ) {
                upperBitChar = 0xa + upperBitChar - 'A';
            } else if ( upperBitChar >= 'a' && upperBitChar <= 'f' ) {
                upperBitChar = 0xa + upperBitChar - 'a';
            }
            
            if ( lowerBitChar >= '0' && lowerBitChar <= '9' ) {
                lowerBitChar -= '0';
            } else if ( lowerBitChar >= 'A' && lowerBitChar <= 'F' ) {
                lowerBitChar = 0xa + lowerBitChar - 'A';
            } else if ( lowerBitChar >= 'a' && lowerBitChar <= 'f' ) {
                lowerBitChar = 0xa + lowerBitChar - 'a';
            }
            
            if ( skip > 1 ) {
                char orgChar = lowerBitChar | (upperBitChar << 4);
                result += orgChar;
            }
            
            i += skip;
        } else {
            result += c;
            ++i;
        }
    }
    
    return result;
}

static void split(const string &text, const string &delimit, vector<std::string> &elems) {
    size_t start = 0;
    string::size_type pos = text.find(delimit);
    elems.clear();

    while (string::npos != pos) {
        elems.push_back(text.substr(start, pos - start));
        start = pos + delimit.size();
        pos = text.find(delimit, start);
    }
    
    if (start < text.size() - 1) {
        elems.push_back(text.substr(start, text.size() - start));
    }
}

static void _test_split(const string &str, const string &delimit) {
    vector<string> elems;
    
    split(str, delimit, elems);
    string result = "[" + str + "]---split:";
    
    size_t i = 0;
    while (i <  elems.size()) {
        result += "," + elems[i];
        i++;
    }
    
    CCLOG("%s", result.c_str());
}


static void parseTime(const string &timeStr, int &day, int &hour, int &minute, int &second) {
    vector<string> elems;
    split(timeStr, ":", elems);
    
    assert(elems.size() == 4);
    
    day = atoi(elems[0].c_str());
    hour = atoi(elems[1].c_str());
    minute = atoi(elems[2].c_str());
    second = atoi(elems[3].c_str());
}

static uint64_t getNotiConfigTime(int day, int hour, int minute, int second, bool isLocalTime) {
    time_t now = time(0);
    
    CCLOG("now:%ld", now);
    
    now += day * 24 * 3600;
    tm *localTimeNow = localtime(&now);
    
    localTimeNow->tm_hour = hour;
    localTimeNow->tm_min = minute;
    localTimeNow->tm_sec = second;
    
    if (!isLocalTime) {
        now = mktime(localTimeNow) + localTimeNow->tm_gmtoff;
    } else {
        now = mktime(localTimeNow);
    }
    
    CCLOG("targetTime:%s", asctime(localtime(&now)));
    
    return now;
}

#endif
