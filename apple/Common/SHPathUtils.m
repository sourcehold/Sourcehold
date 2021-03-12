//
//  SHPathUtils.m
//  Sourcehold
//
//  Created by apodrugin on 08.02.2021.
//

#import <Foundation/Foundation.h>
#import "SHPathUtils.h"


bool SHQueryUserDocumentsDirectoryPath(char *outBuffer, unsigned long outBufferSize) {
    if (NULL == outBuffer) {
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
        
        if (outBufferSize < pathLength) {
            return false;
        }
        
        strcpy(outBuffer, path.UTF8String);
        return true;
    }
}

bool SHQueryMainBundleResourcesDirectoryPath(char *outBuffer, unsigned long outBufferSize) {
    if (NULL == outBuffer) {
        return false;
    }
    
    @autoreleasepool {
        NSString *path = NSBundle.mainBundle.resourcePath;
        const NSUInteger pathLength = [path lengthOfBytesUsingEncoding:NSUTF8StringEncoding] + 1 /*+1 for '\0'*/;
        
        if (outBufferSize < pathLength) {
            return false;
        }
        
        strcpy(outBuffer, path.UTF8String);
        return true;
    }
}
