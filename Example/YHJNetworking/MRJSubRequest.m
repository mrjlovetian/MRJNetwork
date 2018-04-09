//
//  MRJSubRequest.m
//  YHJNetwork
//
//  Created by Mr on 2017/9/1.
//  Copyright © 2017年 mrjlovetian@gmail.com. All rights reserved.
//

#import "MRJSubRequest.h"

@implementation MRJSubRequest
- (NSString *)requestUrl{
    
    return self.defaultUrl;
}

- (MRJRequestMethod)requestMethod {
    if (self.method == MRJRequestMethodHEAD)
    {
        return MRJRequestMethodHEAD;
    } else if (self.method == MRJRequestMethodPATCH) {
        return MRJRequestMethodPATCH;
    } else if (self.method == MRJRequestMethodDELETE) {
        return MRJRequestMethodDELETE;
    } else if (self.method == MRJRequestMethodPOST) {
        return MRJRequestMethodPOST;
    } else if (self.method == MRJRequestMethodPUT) {
        return MRJRequestMethodPUT;
    }
    return MRJRequestMethodGET;
}

- (MRJRequestSerializerType)requestSerializerType {
    if (_isDotNet) {
        return MRJRequestSerializerTypeHTTP;
    }
    return MRJRequestSerializerTypeJSON;
}
@end
