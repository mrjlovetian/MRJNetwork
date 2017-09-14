//
//  MRJ_BatchRequest.m
//  MRJ
//
//  Created by MRJ_ on 2017/2/27.
//  Copyright © 2017年 MRJ_. All rights reserved.
//

#import "MRJ_BatchRequest.h"
#import "MRJ_NetworkPrivate.h"
#import "MRJ_BatchRequestAgent.h"
#import "MRJ_Request.h"

@interface MRJ_BatchRequest() <MRJ_RequestDelegate>

@property (nonatomic) NSInteger finishedCount;

@end

@implementation MRJ_BatchRequest

- (instancetype)initWithRequestArray:(NSArray<MRJ_Request *> *)requestArray {
    self = [super init];
    if (self) {
        _requestArray = [requestArray copy];
        _finishedCount = 0;
        for (MRJ_Request * req in _requestArray) {
            if (![req isKindOfClass:[MRJ_Request class]]) {
                MRJ_Log(@"Error, request item must be MRJ_Request instance.");
                return nil;
            }
        }
    }
    return self;
}

- (void)start {
    if (_finishedCount > 0) {
        MRJ_Log(@"Error! Batch request has already started.");
        return;
    }
    _failedRequest = nil;
    [[MRJ_BatchRequestAgent sharedAgent] addBatchRequest:self];
    [self toggleAccessoriesWillStartCallBack];
    for (MRJ_Request * req in _requestArray) {
        req.delegate = self;
        [req clearCompletionBlock];
        [req start];
    }
}

- (void)stop {
    [self toggleAccessoriesWillStopCallBack];
    _delegate = nil;
    [self clearRequest];
    [self toggleAccessoriesDidStopCallBack];
    [[MRJ_BatchRequestAgent sharedAgent] removeBatchRequest:self];
}

- (void)startWithCompletionBlockWithSuccess:(nullable void (^)(MRJ_BatchRequest *batchRequest))success
                                   progress:(void(^)(float percent))progress
                                    failure:(nullable void (^)(MRJ_BatchRequest *batchRequest))failure{
    [self  setCompletionBlockWithSuccess:success progress:progress failure:failure];
    [self start];
}

- (void)setCompletionBlockWithSuccess:(void (^)(MRJ_BatchRequest *batchRequest))success
                             progress:(void(^)(float percent))progress
                              failure:(void (^)(MRJ_BatchRequest *batchRequest))failure {
    self.successCompletionBlock = success;
    self.percentCompletionBlock = progress;
    self.failureCompletionBlock = failure;
}

- (void)clearCompletionBlock {
    // nil out to break the retain cycle.
    self.successCompletionBlock = nil;
    self.percentCompletionBlock = nil;
    self.failureCompletionBlock = nil;
}

- (BOOL)isDataFromCache {
    BOOL result = YES;
    for (MRJ_Request *request in _requestArray) {
        if (!request.isDataFromCache) {
            result = NO;
        }
    }
    return result;
}


- (void)dealloc {
    [self clearRequest];
}

#pragma mark - Network Request Delegate

- (void)requestFinished:(MRJ_Request *)request {
    _finishedCount++;
    if (_finishedCount == _requestArray.count) {
        //回调完成
        if ([_delegate respondsToSelector:@selector(batchRequestPercentFinished:)]) {
            [_delegate batchRequestPercentFinished:1.0];
        }
        if(_percentCompletionBlock)
        {
            float percent = floorf(1.0);
            _percentCompletionBlock(percent);
        }
        
        //告诉调用者网络请求完成
        [self toggleAccessoriesWillStopCallBack];
        
        //回调完成
        if ([_delegate respondsToSelector:@selector(batchRequestFinished:)]) {
            [_delegate batchRequestFinished:self];
        }
        if (_successCompletionBlock) {
            _successCompletionBlock(self);
        }
        [self clearCompletionBlock];
        [self toggleAccessoriesDidStopCallBack];
        [[MRJ_BatchRequestAgent sharedAgent] removeBatchRequest:self];
    }else{
        
        float percent = [[NSString stringWithFormat:@"%.2f", floorf(_finishedCount) / floorf(_requestArray.count)] floatValue];
        //回调完成百分比
        if ([_delegate respondsToSelector:@selector(batchRequestPercentFinished:)]) {
            [_delegate batchRequestPercentFinished:percent];
        }
        
        if(_percentCompletionBlock)
        {
            
            _percentCompletionBlock(percent);
        }
    }
}

- (void)requestFailed:(MRJ_Request *)request {
    _failedRequest = request;
    [self toggleAccessoriesWillStopCallBack];
    // Stop
    for (MRJ_Request *req in _requestArray) {
        [req stop];
    }
    // Callback
    if ([_delegate respondsToSelector:@selector(batchRequestFailed:)]) {
        [_delegate batchRequestFailed:self];
    }
    if (_failureCompletionBlock) {
        _failureCompletionBlock(self);
    }
    // Clear
    [self clearCompletionBlock];
    
    [self toggleAccessoriesDidStopCallBack];
    [[MRJ_BatchRequestAgent sharedAgent] removeBatchRequest:self];
}

- (void)clearRequest {
    for (MRJ_Request * req in _requestArray) {
        [req stop];
    }
    [self clearCompletionBlock];
}

#pragma mark - Request Accessoies

- (void)addAccessory:(id<MRJ_RequestAccessory>)accessory {
    if (!self.requestAccessories) {
        self.requestAccessories = [NSMutableArray array];
    }
    [self.requestAccessories addObject:accessory];
}

@end
