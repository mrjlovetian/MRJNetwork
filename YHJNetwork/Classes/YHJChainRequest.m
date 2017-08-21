//
//  KKChainRequest.m
//  TopsTechNetWorking
//
//  Created by YHJ on 2017/3/15.
//  Copyright © 2017年 YHJ. All rights reserved.
//

#import "KKChainRequest.h"
#import "KKChainRequestAgent.h"
#import "YHJBetworkPrivate.h"
#import "YHJBaseRequest.h"

@interface KKChainRequest()<YHJRequestDelegate>

@property (strong, nonatomic) NSMutableArray<YHJBaseRequest *> *requestArray;
@property (strong, nonatomic) NSMutableArray<KKChainCallback> *requestCallbackArray;
@property (assign, nonatomic) NSUInteger nextRequestIndex;
@property (strong, nonatomic) KKChainCallback emptyCallback;

@end

@implementation KKChainRequest

- (instancetype)init {
    self = [super init];
    if (self) {
        _nextRequestIndex = 0;
        _requestArray = [NSMutableArray array];
        _requestCallbackArray = [NSMutableArray array];
        _emptyCallback = ^(KKChainRequest *chainRequest, YHJBaseRequest *baseRequest) {
            // do nothing
        };
    }
    return self;
}

- (void)start {
    if (_nextRequestIndex > 0) {
        KKLog(@"Error! Chain request has already started.");
        return;
    }
    
    if ([_requestArray count] > 0) {
        [self toggleAccessoriesWillStartCallBack];
        [self startNextRequest];
        [[KKChainRequestAgent sharedAgent] addChainRequest:self];
    } else {
        KKLog(@"Error! Chain request array is empty.");
    }
}

- (void)stop {
    [self toggleAccessoriesWillStopCallBack];
    [self clearRequest];
    [[KKChainRequestAgent sharedAgent] removeChainRequest:self];
    [self toggleAccessoriesDidStopCallBack];
}

- (void)addRequest:(YHJBaseRequest *)request callback:(KKChainCallback)callback {
    [_requestArray addObject:request];
    if (callback != nil) {
        [_requestCallbackArray addObject:callback];
    } else {
        [_requestCallbackArray addObject:_emptyCallback];
    }
}

- (NSArray<YHJBaseRequest *> *)requestArray {
    return _requestArray;
}

- (BOOL)startNextRequest {
    if (_nextRequestIndex < [_requestArray count]) {
        YHJBaseRequest *request = _requestArray[_nextRequestIndex];
        _nextRequestIndex++;
        request.delegate = self;
        [request clearCompletionBlock];
        [request start];
        return YES;
    } else {
        return NO;
    }
}

#pragma mark - Network Request Delegate

- (void)requestFinished:(YHJBaseRequest *)request {
    NSUInteger currentRequestIndex = _nextRequestIndex - 1;
    KKChainCallback callback = _requestCallbackArray[currentRequestIndex];
    callback(self, request);
    if (![self startNextRequest]) {
        [self toggleAccessoriesWillStopCallBack];
        if ([_delegate respondsToSelector:@selector(chainRequestFinished:)]) {
            [_delegate chainRequestFinished:self];
            [[KKChainRequestAgent sharedAgent] removeChainRequest:self];
        }
        [self toggleAccessoriesDidStopCallBack];
    }
}

- (void)requestFailed:(YHJBaseRequest *)request {
    [self toggleAccessoriesWillStopCallBack];
    if ([_delegate respondsToSelector:@selector(chainRequestFailed:failedBaseRequest:)]) {
        [_delegate chainRequestFailed:self failedBaseRequest:request];
        [[KKChainRequestAgent sharedAgent] removeChainRequest:self];
    }
    [self toggleAccessoriesDidStopCallBack];
}

- (void)clearRequest {
    NSUInteger currentRequestIndex = _nextRequestIndex - 1;
    if (currentRequestIndex < [_requestArray count]) {
        YHJBaseRequest *request = _requestArray[currentRequestIndex];
        [request stop];
    }
    [_requestArray removeAllObjects];
    [_requestCallbackArray removeAllObjects];
}

#pragma mark - Request Accessoies

- (void)addAccessory:(id<YHJRequestAccessory>)accessory {
    if (!self.requestAccessories) {
        self.requestAccessories = [NSMutableArray array];
    }
    [self.requestAccessories addObject:accessory];
}


@end
