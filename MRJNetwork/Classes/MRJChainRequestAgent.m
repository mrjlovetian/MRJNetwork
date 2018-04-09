//
//  MRJChainRequestAgent.m
//  MRJ
//
//  Created by MRJ on 2017/3/15.
//  Copyright © 2017年 MRJ. All rights reserved.
//

#import "MRJChainRequestAgent.h"

@interface MRJChainRequestAgent()

@property(strong,nonatomic) NSMutableArray<MRJChainRequest *> *requestArray;

@end

@implementation MRJChainRequestAgent

+ (MRJChainRequestAgent *)sharedAgent {
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

- (void)addChainRequest:(MRJChainRequest *)request {
    @synchronized(self) {
        [_requestArray addObject:request];
    }
}

- (void)removeChainRequest:(MRJChainRequest *)request {
    @synchronized(self) {
        [_requestArray removeObject:request];
    }
}

@end
