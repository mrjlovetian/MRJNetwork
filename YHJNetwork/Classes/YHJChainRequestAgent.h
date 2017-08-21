//
//  YHJChainRequestAgent.h
//  MRJ
//
//  Created by YHJ on 2017/3/15.
//  Copyright © 2017年 YHJ. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class YHJChainRequest;

@interface YHJChainRequestAgent : NSObject

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

///  Get the shared chain request agent.
+ (YHJChainRequestAgent *)sharedAgent;

///  Add a chain request.
- (void)addChainRequest:(YHJChainRequest *)request;

///  Remove a previously added chain request.
- (void)removeChainRequest:(YHJChainRequest *)request;

@end

NS_ASSUME_NONNULL_END
