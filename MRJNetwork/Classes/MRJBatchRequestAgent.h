//
//  MRJBatchRequestAgent.h
//
//  Created by MRJ on 2017/2/27.
//  Copyright © 2017年 MRJ. All rights reserved.
//

#import <Foundation/Foundation.h>
NS_ASSUME_NONNULL_BEGIN
@class MRJBatchRequest;

@interface MRJBatchRequestAgent : NSObject

///  单例方法.
+ (MRJBatchRequestAgent *)sharedAgent;

///  添加请求.
- (void)addBatchRequest:(MRJBatchRequest *)request;

///  移除请求.
- (void)removeBatchRequest:(MRJBatchRequest *)request;

@end
NS_ASSUME_NONNULL_END
