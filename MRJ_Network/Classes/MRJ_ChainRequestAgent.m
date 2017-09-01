//
//  MRJ_ChainRequestAgent.m
//  MRJ
//
//  Created by MRJ_ on 2017/3/15.
//  Copyright © 2017年 MRJ_. All rights reserved.
//

#import "MRJ_ChainRequestAgent.h"

@interface MRJ_ChainRequestAgent()

@property(strong,nonatomic) NSMutableArray<MRJ_ChainRequest *> *requestArray;

@end

@implementation MRJ_ChainRequestAgent

+ (MRJ_ChainRequestAgent *)sharedAgent {
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

- (void)addChainRequest:(MRJ_ChainRequest *)request {
    @synchronized(self) {
        [_requestArray addObject:request];
    }
}

- (void)removeChainRequest:(MRJ_ChainRequest *)request {
    @synchronized(self) {
        [_requestArray removeObject:request];
    }
}


@end
