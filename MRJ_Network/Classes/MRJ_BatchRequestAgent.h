//
//  MRJ_BatchRequestAgent.h
//
//  Created by MRJ_ on 2017/2/27.
//  Copyright © 2017年 MRJ_. All rights reserved.
//

#import <Foundation/Foundation.h>
NS_ASSUME_NONNULL_BEGIN
@class MRJ_BatchRequest;

@interface MRJ_BatchRequestAgent : NSObject

///  单例方法.
+ (MRJ_BatchRequestAgent *)sharedAgent;

///  添加请求.
- (void)addBatchRequest:(MRJ_BatchRequest *)request;

///  移除请求.
- (void)removeBatchRequest:(MRJ_BatchRequest *)request;

@end
NS_ASSUME_NONNULL_END
