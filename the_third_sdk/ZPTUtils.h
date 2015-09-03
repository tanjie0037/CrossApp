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

#endif
