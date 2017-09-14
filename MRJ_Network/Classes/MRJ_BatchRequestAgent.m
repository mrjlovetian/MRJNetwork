//
//  MRJ_BatchRequestAgent.m
//  MRJ
//
//  Created by MRJ_ on 2017/2/27.
//  Copyright © 2017年 MRJ_. All rights reserved.
//

#import "MRJ_BatchRequestAgent.h"

#import "MRJ_BatchRequest.h"

@interface MRJ_BatchRequestAgent()

@property (strong,nonatomic) NSMutableArray<MRJ_BatchRequest *> *requestArray;

@end

@implementation MRJ_BatchRequestAgent

+ (MRJ_BatchRequestAgent *)sharedAgent {
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

- (void)addBatchRequest:(MRJ_BatchRequest *)request {
    @synchronized(self) {
        [_requestArray addObject:request];
    }
}

- (void)removeBatchRequest:(MRJ_BatchRequest *)request {
    @synchronized(self) {
        [_requestArray removeObject:request];
    }
}

@end
