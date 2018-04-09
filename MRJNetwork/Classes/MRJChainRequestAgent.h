//
//  MRJChainRequestAgent.h
//  MRJ
//
//  Created by MRJ on 2017/3/15.
//  Copyright © 2017年 MRJ. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class MRJChainRequest;

@interface MRJChainRequestAgent : NSObject

///  Get the shared chain request agent.
+ (MRJChainRequestAgent *)sharedAgent;

///  Add a chain request.
- (void)addChainRequest:(MRJChainRequest *)request;

///  Remove a previously added chain request.
- (void)removeChainRequest:(MRJChainRequest *)request;

@end

NS_ASSUME_NONNULL_END
