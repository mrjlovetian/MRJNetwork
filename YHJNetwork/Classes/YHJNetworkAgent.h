//
//  YHJBetworkAgent.h
//  TopsTechNetWorking
//
//  Created by YHJ on 2017/2/17.
//  Copyright © 2017年 YHJ. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "YHJBaseRequest.h"

NS_ASSUME_NONNULL_BEGIN

@interface YHJBetworkAgent : NSObject

-(instancetype)init;

+(YHJBetworkAgent *)sharedAgent;

-(void)addRequest:(YHJBaseRequest *)request;

-(void)cancelRequest:(YHJBaseRequest *)request;

-(void)cancelAllRequests;

///  Return the constructed URL of request.
///
///  @param request The request to parse. Should not be nil.
///
///  @return The result URL.
- (NSString *)buildRequestUrl:(YHJBaseRequest *)request;

@end

NS_ASSUME_NONNULL_END
