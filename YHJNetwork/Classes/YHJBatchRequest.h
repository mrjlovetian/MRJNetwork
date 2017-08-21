//
//  YHJBatchRequest.h
//  TopsTechNetWorking
//
//  Created by YHJ on 2017/2/27.
//  Copyright © 2017年 YHJ. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class YHJBatchRequest;
@class YHJRequest;
@protocol YHJRequestAccessory;

///  The YHJBatchRequestDelegate protocol defines several optional methods you can use
///  to receive network-related messages. All the delegate methods will be called
///  on the main queue. Note the delegate methods will be called when all the requests
///  of batch request finishes.
@protocol YHJBatchRequestDelegate <NSObject>

@optional
///  Tell the delegate that the batch request has finished successfully/
///
///  @param batchRequest The corresponding batch request.
- (void)batchRequestFinished:(YHJBatchRequest *)batchRequest;

///  当一个请求完成后 向客户端反馈请求完成百分比
///  Tell the delegate that the batch request had percent.
- (void)batchRequestPercentFinished:(float)percent;


///  Tell the delegate that the batch request has failed.
///
///  @param batchRequest The corresponding batch request.
- (void)batchRequestFailed:(YHJBatchRequest *)batchRequest;

@end

@interface YHJBatchRequest : NSObject


///  All the requests are stored in this array.
@property (nonatomic, strong, readonly) NSArray<YHJRequest *> *requestArray;

///  The delegate object of the batch request. Default is nil.
@property (nonatomic, weak, nullable) id<YHJBatchRequestDelegate> delegate;

///  The success callback. Note this will be called only if all the requests are finished.
///  This block will be called on the main queue.
@property (nonatomic, copy, nullable) void (^successCompletionBlock)(YHJBatchRequest *);

///  当一个请求完成后 向客户端反馈请求完成百分比
@property (nonatomic, copy, nullable) void (^percentCompletionBlock)(float);

///  The failure callback. Note this will be called if one of the requests fails.
///  This block will be called on the main queue.
@property (nonatomic, copy, nullable) void (^failureCompletionBlock)(YHJBatchRequest *);

///  Tag can be used to identify batch request. Default value is 0.
@property (nonatomic) NSInteger tag;

///  This can be used to add several accossories object. Note if you use `addAccessory` to add acceesory
///  this array will be automatically created. Default is nil.
@property (nonatomic, strong, nullable) NSMutableArray<id<YHJRequestAccessory>> *requestAccessories;

///  The first request that failed (and causing the batch request to fail).
@property (nonatomic, strong, readonly, nullable) YHJRequest *failedRequest;

///  Creates a `YTKBatchRequest` with a bunch of requests.
///
///  @param requestArray requests useds to create batch request.
///
- (instancetype)initWithRequestArray:(NSArray<YHJRequest *> *)requestArray;

///  Set completion callbacks
- (void)setCompletionBlockWithSuccess:(nullable void (^)(YHJBatchRequest *batchRequest))success progress:(void(^)(float percent))progress
                              failure:(nullable void (^)(YHJBatchRequest *batchRequest))failure;

///  Nil out both success and failure callback blocks.
- (void)clearCompletionBlock;

///  Convenience method to add request accessory. See also `requestAccessories`.
- (void)addAccessory:(id<YHJRequestAccessory>)accessory;

///  Append all the requests to queue.
- (void)start;

///  Stop all the requests of the batch request.
- (void)stop;

///  Convenience method to start the batch request with block callbacks.
- (void)startWithCompletionBlockWithSuccess:(nullable void (^)(YHJBatchRequest *batchRequest))success progress:(void(^)(float percent))progress
                                    failure:(nullable void (^)(YHJBatchRequest *batchRequest))failure;

///  Whether all response data is from local cache.
- (BOOL)isDataFromCache;


@end

NS_ASSUME_NONNULL_END
