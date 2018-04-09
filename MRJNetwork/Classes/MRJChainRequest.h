//
//  MRJChainRequest.h
//  MRJ
//
//  Created by MRJ on 2017/3/15.
//  Copyright © 2017年 MRJ. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class MRJChainRequest;
@class MRJBaseRequest;
@protocol MRJRequestAccessory;

///  The MRJChainRequestDelegate protocol defines several optional methods you can use
///  to receive network-related messages. All the delegate methods will be called
///  on the main queue. Note the delegate methods will be called when all the requests
///  of chain request finishes.
@protocol MRJChainRequestDelegate <NSObject>

@optional
///  Tell the delegate that the chain request has finished successfully.
///
///  @param chainRequest The corresponding chain request.
- (void)chainRequestFinished:(MRJChainRequest *)chainRequest;

///  Tell the delegate that the chain request has failed.
///
///  @param chainRequest The corresponding chain request.
///  @param request      First failed request that causes the whole request to fail.
- (void)chainRequestFailed:(MRJChainRequest *)chainRequest failedBaseRequest:(MRJBaseRequest*)request;

@end

typedef void (^MRJChainCallback)(MRJChainRequest *chainRequest, MRJBaseRequest *baseRequest);

///  MRJBatchRequest can be used to chain several MRJRequest so that one will only starts after another finishes.
///  Note that when used inside MRJChainRequest, a single MRJRequest will have its own callback and delegate
///  cleared, in favor of the batch request callback.

@interface MRJChainRequest : NSObject

///  All the requests are stored in this array.
- (NSArray<MRJBaseRequest *> *)requestArray;

///  The delegate object of the chain request. Default is nil.
@property (nonatomic, weak, nullable) id<MRJChainRequestDelegate> delegate;

///  This can be used to add several accossories object. Note if you use `addAccessory` to add acceesory
///  this array will be automatically created. Default is nil.
@property (nonatomic, strong, nullable) NSMutableArray<id<MRJRequestAccessory>> *requestAccessories;

///  Convenience method to add request accessory. See also `requestAccessories`.
- (void)addAccessory:(id<MRJRequestAccessory>)accessory;

///  Start the chain request, adding first request in the chain to request queue.
- (void)start;

///  Stop the chain request. Remaining request in chain will be cancelled.
- (void)stop;

///  Add request to request chain.
///
///  @param request  The request to be chained.
///  @param callback The finish callback
- (void)addRequest:(MRJBaseRequest *)request callback:(nullable MRJChainCallback)callback;

@end

NS_ASSUME_NONNULL_END
