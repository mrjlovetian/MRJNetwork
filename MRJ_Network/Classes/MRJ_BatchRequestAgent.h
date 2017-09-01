//
//  MRJ_BatchRequestAgent.h
//  TopsTechNetWorking
//
//  Created by MRJ_ on 2017/2/27.
//  Copyright © 2017年 MRJ_. All rights reserved.
//

#import <Foundation/Foundation.h>
NS_ASSUME_NONNULL_BEGIN
@class MRJ_BatchRequest;

@interface MRJ_BatchRequestAgent : NSObject

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

///  Get the shared batch request agent.
+ (MRJ_BatchRequestAgent *)sharedAgent;

///  Add a batch request.
- (void)addBatchRequest:(MRJ_BatchRequest *)request;

///  Remove a previously added batch request.
- (void)removeBatchRequest:(MRJ_BatchRequest *)request;


@end
NS_ASSUME_NONNULL_END
