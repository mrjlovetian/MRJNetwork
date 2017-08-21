//
//  YHJChainRequestAgent.m
//  MRJ
//
//  Created by YHJ on 2017/3/15.
//  Copyright © 2017年 YHJ. All rights reserved.
//

#import "YHJChainRequestAgent.h"

@interface YHJChainRequestAgent()

@property(strong,nonatomic) NSMutableArray<YHJChainRequest *> *requestArray;

@end

@implementation YHJChainRequestAgent

+ (YHJChainRequestAgent *)sharedAgent {
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

- (void)addChainRequest:(YHJChainRequest *)request {
    @synchronized(self) {
        [_requestArray addObject:request];
    }
}

- (void)removeChainRequest:(YHJChainRequest *)request {
    @synchronized(self) {
        [_requestArray removeObject:request];
    }
}


@end
