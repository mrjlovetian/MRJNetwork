//
//  YHJBetworkConfig.m
//  TopsTechNetWorking
//
//  Created by YHJ on 2017/2/20.
//  Copyright © 2017年 YHJ. All rights reserved.
//

#import "YHJBetworkConfig.h"

@implementation YHJBetworkConfig{
    NSMutableArray<id<KKUrlFilterProtocol>> *_urlFilters;
    NSMutableArray<id<KKCacheDirPathFilterProtocol>> *_cacheDirPathFilters;
}


+ (YHJBetworkConfig *)sharedConfig {
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
        _baseUrl = @"";
        _cdnUrl = @"";
        _urlFilters = [NSMutableArray array];
        _cacheDirPathFilters = [NSMutableArray array];
        _securityPolicy = [AFSecurityPolicy defaultPolicy];
        _debugLogEnabled = YES;
    }
    return self;
}

- (void)addUrlFilter:(id<KKUrlFilterProtocol>)filter {
    [_urlFilters addObject:filter];
}

- (void)clearUrlFilter {
    [_urlFilters removeAllObjects];
}

- (void)addCacheDirPathFilter:(id<KKCacheDirPathFilterProtocol>)filter {
    [_cacheDirPathFilters addObject:filter];
}

- (void)clearCacheDirPathFilter {
    [_cacheDirPathFilters removeAllObjects];
}

- (NSArray<id<KKUrlFilterProtocol>> *)urlFilters {
    return [_urlFilters copy];
}

- (NSArray<id<KKCacheDirPathFilterProtocol>> *)cacheDirPathFilters {
    return [_cacheDirPathFilters copy];
}

#pragma mark - NSObject

- (NSString *)description {
    return [NSString stringWithFormat:@"<%@: %p>{ baseURL: %@ } { cdnURL: %@ }", NSStringFromClass([self class]), self, self.baseUrl, self.cdnUrl];
}

@end
