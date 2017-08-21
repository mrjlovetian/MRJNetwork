//
//  KKChainRequestAgent.h
//  TopsTechNetWorking
//
//  Created by YHJ on 2017/3/15.
//  Copyright © 2017年 YHJ. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class KKChainRequest;

@interface KKChainRequestAgent : NSObject

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

///  Get the shared chain request agent.
+ (KKChainRequestAgent *)sharedAgent;

///  Add a chain request.
- (void)addChainRequest:(KKChainRequest *)request;

///  Remove a previously added chain request.
- (void)removeChainRequest:(KKChainRequest *)request;

@end

NS_ASSUME_NONNULL_END
