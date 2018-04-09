//
//  NetArgumentFilter.h
//  YHJNetwork
//
//  Created by MRJ on 2017/9/2.
//  Copyright © 2017年 mrjlovetian@gmail.com. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <MRJNetwork/MRJNetworkConfig.h>

@interface NetArgumentFilter : NSObject<MRJUrlFilterProtocol>

+ (NetArgumentFilter *)filterWithArguments:(NSDictionary *)arguments;

- (NSString *)filterUrl:(NSString *)originUrl withRequest:(MRJBaseRequest *)request;

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
