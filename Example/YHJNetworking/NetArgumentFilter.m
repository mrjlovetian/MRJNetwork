//
//  NetArgumentFilter.m
//  YHJNetwork
//
//  Created by MRJ on 2017/9/2.
//  Copyright © 2017年 mrjlovetian@gmail.com. All rights reserved.
//

#import "NetArgumentFilter.h"
#import <MRJ_Network/MRJ_Request.h>

@implementation NetArgumentFilter{
    NSDictionary *_arguments;
}

+ (NetArgumentFilter *)filterWithArguments:(NSDictionary *)arguments {
    return [[self alloc] initWithArguments:arguments];
}

- (id)initWithArguments:(NSDictionary *)arguments {
    self = [super init];
    if (self) {
        _arguments = arguments;
    }
    return self;
}

- (NSString *)filterUrl:(NSString *)originUrl withRequest:(MRJ_BaseRequest *)request {
    return [self urlStringWithOriginUrlString:originUrl appendParameters:_arguments withRequest:request];
}

- (NSString *)urlStringWithOriginUrlString:(NSString *)originUrlString appendParameters:(NSDictionary *)parameters  withRequest:(MRJ_BaseRequest *)request{
    
    NSMutableDictionary * requestArguments = nil;
    if ([request.requestArgument isKindOfClass:[NSDictionary class]]) {
        requestArguments = [NSMutableDictionary dictionaryWithDictionary: (NSDictionary *)request.requestArgument];
    }
    if (parameters && parameters.count > 0) {
        [requestArguments addEntriesFromDictionary:parameters];
        request.requestArgument = requestArguments;
    }
    
    if ([request.requestArgument isKindOfClass:[NSArray class]]) {
        
    }
    AppContext *app = [AppContext sharedInstance];
    NSString *dateStr = app.dateString;
    
    NSDictionary *header = @{
                             @"Date":dateStr,
                             @"Cache-Control":@"application/x-www-form-urlencoded; charset=utf-8",
                             @"User-Agent":app.userAgent
                             };
    
    request.requestHeaderFieldValueDictionary = header;
    
    return originUrlString;
    
}
@end

@implementation AppContext

+ (instancetype)sharedInstance
{
    static AppContext *sharedInstance;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedInstance = [[AppContext alloc] init];
        
    });
    return sharedInstance;
}

-(NSString *)userAgent
{
    if (!_userAgent) {
        _userAgent = @"xjappstorios";
    }
    return _userAgent;
}

-(NSString *)gatewayVersion
{
    if (!_gatewayVersion) {
        _gatewayVersion = @"1";
    }
    return _gatewayVersion;
}

-(NSString*)consumerKey
{
    if (!_consumerKey) {
        _consumerKey = @"relase";
    }
    return _consumerKey;
}

-(NSString*)returnInfo
{
    if (!_returnInfo) {
        _returnInfo = @"Timing ServerName ServerIP";
    }
    return _returnInfo;
}

-(NSString *)dateString
{
    NSDate* now = [NSDate date];
    if (self.timeIntervalFromNet) {
        now = [NSDate dateWithTimeIntervalSinceNow:self.timeIntervalFromNet];
    }
//    now = [now destinationDateNow];
    NSString *dateStr = [NSString stringWithFormat:@"%@", now];
    return dateStr;
}

-(NSString *)authorization
{
    if (!_authorization) {
        _authorization = @"ak";
    }
    NSLog(@"Authorization = %@",_authorization);
    return _authorization;
}

@end
