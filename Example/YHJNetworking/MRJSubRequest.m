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

- (MRJ_RequestMethod)requestMethod {
    if (self.method == MRJ_RequestMethodHEAD)
    {
        return MRJ_RequestMethodHEAD;
    }else if (self.method == MRJ_RequestMethodPATCH)
    {
        return MRJ_RequestMethodPATCH;
    }else if (self.method == MRJ_RequestMethodDELETE)
    {
        return MRJ_RequestMethodDELETE;
    }else if (self.method == MRJ_RequestMethodPOST)
    {
        return MRJ_RequestMethodPOST;
    }else if (self.method == MRJ_RequestMethodPUT)
    {
        return MRJ_RequestMethodPUT;
    }
    return MRJ_RequestMethodGET;
}

- (MRJ_RequestSerializerType)requestSerializerType {
    if (_isDotNet) {
        return MRJ_RequestSerializerTypeHTTP;
    }
    return MRJ_RequestSerializerTypeJSON;
}
@end
