//
//  YHJBatchRequestAgent.h
//  TopsTechNetWorking
//
//  Created by YHJ on 2017/2/27.
//  Copyright © 2017年 YHJ. All rights reserved.
//

#import <Foundation/Foundation.h>
NS_ASSUME_NONNULL_BEGIN
@class YHJBatchRequest;

@interface YHJBatchRequestAgent : NSObject

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

///  Get the shared batch request agent.
+ (YHJBatchRequestAgent *)sharedAgent;

///  Add a batch request.
- (void)addBatchRequest:(YHJBatchRequest *)request;

///  Remove a previously added batch request.
- (void)removeBatchRequest:(YHJBatchRequest *)request;


@end
NS_ASSUME_NONNULL_END
