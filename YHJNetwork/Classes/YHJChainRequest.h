//
//  KKChainRequest.h
//  TopsTechNetWorking
//
//  Created by YHJ on 2017/3/15.
//  Copyright © 2017年 YHJ. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class KKChainRequest;
@class YHJBaseRequest;
@protocol YHJRequestAccessory;

///  The KKChainRequestDelegate protocol defines several optional methods you can use
///  to receive network-related messages. All the delegate methods will be called
///  on the main queue. Note the delegate methods will be called when all the requests
///  of chain request finishes.
@protocol KKChainRequestDelegate <NSObject>

@optional
///  Tell the delegate that the chain request has finished successfully.
///
///  @param chainRequest The corresponding chain request.
- (void)chainRequestFinished:(KKChainRequest *)chainRequest;

///  Tell the delegate that the chain request has failed.
///
///  @param chainRequest The corresponding chain request.
///  @param request      First failed request that causes the whole request to fail.
- (void)chainRequestFailed:(KKChainRequest *)chainRequest failedBaseRequest:(YHJBaseRequest*)request;

@end

typedef void (^KKChainCallback)(KKChainRequest *chainRequest, YHJBaseRequest *baseRequest);

///  YHJBatchRequest can be used to chain several YHJRequest so that one will only starts after another finishes.
///  Note that when used inside KKChainRequest, a single YHJRequest will have its own callback and delegate
///  cleared, in favor of the batch request callback.

@interface KKChainRequest : NSObject

///  All the requests are stored in this array.
- (NSArray<YHJBaseRequest *> *)requestArray;

///  The delegate object of the chain request. Default is nil.
@property (nonatomic, weak, nullable) id<KKChainRequestDelegate> delegate;

///  This can be used to add several accossories object. Note if you use `addAccessory` to add acceesory
///  this array will be automatically created. Default is nil.
@property (nonatomic, strong, nullable) NSMutableArray<id<YHJRequestAccessory>> *requestAccessories;

///  Convenience method to add request accessory. See also `requestAccessories`.
- (void)addAccessory:(id<YHJRequestAccessory>)accessory;

///  Start the chain request, adding first request in the chain to request queue.
- (void)start;

///  Stop the chain request. Remaining request in chain will be cancelled.
- (void)stop;

///  Add request to request chain.
///
///  @param request  The request to be chained.
///  @param callback The finish callback
- (void)addRequest:(YHJBaseRequest *)request callback:(nullable KKChainCallback)callback;


@end

NS_ASSUME_NONNULL_END
