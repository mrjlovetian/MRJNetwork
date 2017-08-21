//
//  YHJBatchRequestAgent.m
//  MRJ
//
//  Created by YHJ on 2017/2/27.
//  Copyright © 2017年 YHJ. All rights reserved.
//

#import "YHJBatchRequestAgent.h"

#import "YHJBatchRequest.h"

@interface YHJBatchRequestAgent()

@property (strong,nonatomic) NSMutableArray<YHJBatchRequest *> *requestArray;

@end

@implementation YHJBatchRequestAgent

+ (YHJBatchRequestAgent *)sharedAgent {
    static id sharedInstance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedInstance = [[self alloc] init];
    });
    return sharedInstance;
}

- (instancetype)init {
    self = [super init];
    if (self) {
        _requestArray = [NSMutableArray array];
    }
    return self;
}

- (void)addBatchRequest:(YHJBatchRequest *)request {
    @synchronized(self) {
        [_requestArray addObject:request];
    }
}

- (void)removeBatchRequest:(YHJBatchRequest *)request {
    @synchronized(self) {
        [_requestArray removeObject:request];
    }
}


@end
