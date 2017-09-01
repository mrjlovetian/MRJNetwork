//
//  MRJ_ChainRequestAgent.h
//  MRJ
//
//  Created by MRJ_ on 2017/3/15.
//  Copyright © 2017年 MRJ_. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class MRJ_ChainRequest;

@interface MRJ_ChainRequestAgent : NSObject

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

///  Get the shared chain request agent.
+ (MRJ_ChainRequestAgent *)sharedAgent;

///  Add a chain request.
- (void)addChainRequest:(MRJ_ChainRequest *)request;

///  Remove a previously added chain request.
- (void)removeChainRequest:(MRJ_ChainRequest *)request;

@end

NS_ASSUME_NONNULL_END
