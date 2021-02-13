//
//  SHPathUtils.m
//  Sourcehold
//
//  Created by apodrugin on 08.02.2021.
//

#import <Foundation/Foundation.h>
#import "SHPathUtils.h"


bool SHGetUserDocumentsDirectoryPath(char *buffer, unsigned long bufferSize) {
    if (NULL == buffer) {
        return false;
    }
    
    @autoreleasepool {
        NSArray<NSString *> *pathsList = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,
                                                                             NSUserDomainMask,
                                                                             YES);
        
        if (0 == pathsList.count) {
            return false;
        }
        
        NSString *path = pathsList.firstObject;
        const NSUInteger pathLength = [path lengthOfBytesUsingEncoding:NSUTF8StringEncoding] + 1 /*+1 for '\0'*/;
        
        if (bufferSize < pathLength) {
            return false;
        }
        
        strcpy(buffer, path.UTF8String);
        return true;
    }
}
