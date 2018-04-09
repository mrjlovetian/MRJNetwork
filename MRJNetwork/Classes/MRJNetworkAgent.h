//
//  MRJBetworkAgent.h
//
//  Created by MRJ on 2017/2/17.
//  Copyright © 2017年 MRJ. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "MRJBaseRequest.h"

NS_ASSUME_NONNULL_BEGIN

@interface MRJNetworkAgent : NSObject

- (instancetype)init;

+ (MRJNetworkAgent *)sharedAgent;

- (void)addRequest:(MRJBaseRequest *)request;

- (void)cancelRequest:(MRJBaseRequest *)request;

- (void)cancelAllRequests;

- (NSString *)buildRequestUrl:(MRJBaseRequest *)request;

@end

NS_ASSUME_NONNULL_END
