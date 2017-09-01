//
//  MRJ_ChainRequest.h
//  MRJ
//
//  Created by MRJ_ on 2017/3/15.
//  Copyright © 2017年 MRJ_. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class MRJ_ChainRequest;
@class MRJ_BaseRequest;
@protocol MRJ_RequestAccessory;

///  The MRJ_ChainRequestDelegate protocol defines several optional methods you can use
///  to receive network-related messages. All the delegate methods will be called
///  on the main queue. Note the delegate methods will be called when all the requests
///  of chain request finishes.
@protocol MRJ_ChainRequestDelegate <NSObject>

@optional
///  Tell the delegate that the chain request has finished successfully.
///
///  @param chainRequest The corresponding chain request.
- (void)chainRequestFinished:(MRJ_ChainRequest *)chainRequest;

///  Tell the delegate that the chain request has failed.
///
///  @param chainRequest The corresponding chain request.
///  @param request      First failed request that causes the whole request to fail.
- (void)chainRequestFailed:(MRJ_ChainRequest *)chainRequest failedBaseRequest:(MRJ_BaseRequest*)request;

@end

typedef void (^MRJ_ChainCallback)(MRJ_ChainRequest *chainRequest, MRJ_BaseRequest *baseRequest);

///  MRJ_BatchRequest can be used to chain several MRJ_Request so that one will only starts after another finishes.
///  Note that when used inside MRJ_ChainRequest, a single MRJ_Request will have its own callback and delegate
///  cleared, in favor of the batch request callback.

@interface MRJ_ChainRequest : NSObject

///  All the requests are stored in this array.
- (NSArray<MRJ_BaseRequest *> *)requestArray;

///  The delegate object of the chain request. Default is nil.
@property (nonatomic, weak, nullable) id<MRJ_ChainRequestDelegate> delegate;

///  This can be used to add several accossories object. Note if you use `addAccessory` to add acceesory
///  this array will be automatically created. Default is nil.
@property (nonatomic, strong, nullable) NSMutableArray<id<MRJ_RequestAccessory>> *requestAccessories;

///  Convenience method to add request accessory. See also `requestAccessories`.
- (void)addAccessory:(id<MRJ_RequestAccessory>)accessory;

///  Start the chain request, adding first request in the chain to request queue.
- (void)start;

///  Stop the chain request. Remaining request in chain will be cancelled.
- (void)stop;

///  Add request to request chain.
///
///  @param request  The request to be chained.
///  @param callback The finish callback
- (void)addRequest:(MRJ_BaseRequest *)request callback:(nullable MRJ_ChainCallback)callback;


@end

NS_ASSUME_NONNULL_END
