//
//  MRJBatchRequestAgent.m
//  MRJ
//
//  Created by MRJ on 2017/2/27.
//  Copyright © 2017年 MRJ. All rights reserved.
//

#import "MRJBatchRequestAgent.h"
#import "MRJBatchRequest.h"

@interface MRJBatchRequestAgent()

@property (strong, nonatomic) NSMutableArray<MRJBatchRequest *> *requestArray;

@end

@implementation MRJBatchRequestAgent

+ (MRJBatchRequestAgent *)sharedAgent {
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

- (void)addBatchRequest:(MRJBatchRequest *)request {
    @synchronized(self) {
        [_requestArray addObject:request];
    }
}

- (void)removeBatchRequest:(MRJBatchRequest *)request {
    @synchronized(self) {
        [_requestArray removeObject:request];
    }
}

@end
