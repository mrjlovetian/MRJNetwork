//
//  KKChainRequestAgent.m
//  TopsTechNetWorking
//
//  Created by YHJ on 2017/3/15.
//  Copyright © 2017年 YHJ. All rights reserved.
//

#import "KKChainRequestAgent.h"

@interface KKChainRequestAgent()

@property(strong,nonatomic) NSMutableArray<KKChainRequest *> *requestArray;

@end

@implementation KKChainRequestAgent

+ (KKChainRequestAgent *)sharedAgent {
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

- (void)addChainRequest:(KKChainRequest *)request {
    @synchronized(self) {
        [_requestArray addObject:request];
    }
}

- (void)removeChainRequest:(KKChainRequest *)request {
    @synchronized(self) {
        [_requestArray removeObject:request];
    }
}


@end
