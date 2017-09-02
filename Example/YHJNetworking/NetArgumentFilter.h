//
//  NetArgumentFilter.h
//  YHJNetwork
//
//  Created by MRJ on 2017/9/2.
//  Copyright © 2017年 mrjlovetian@gmail.com. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <MRJ_Network/MRJ_NetworkConfig.h>

@interface NetArgumentFilter : NSObject<MRJ_UrlFilterProtocol>

+ (NetArgumentFilter *)filterWithArguments:(NSDictionary *)arguments;

- (NSString *)filterUrl:(NSString *)originUrl withRequest:(MRJ_BaseRequest *)request;

@end


@interface AppContext : NSObject

@property (nonatomic, copy) NSString *userAgent;

@property (nonatomic, copy) NSString *gatewayVersion;

@property (nonatomic, copy) NSString *consumerKey;

@property (nonatomic, copy) NSString *returnInfo;

@property (nonatomic, copy) NSString *dateString;

@property (nonatomic, copy) NSString *authorization;

@property (nonatomic,assign) NSTimeInterval  timeIntervalFromNet;//与服务端的时间差

+ (instancetype)sharedInstance;

@end
