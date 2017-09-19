//
//  MRJ_NetworkConfig.m
//  MRJ
//
//  Created by MRJ_ on 2017/2/20.
//  Copyright © 2017年 MRJ_. All rights reserved.
//

#import "MRJ_NetworkConfig.h"

@implementation MRJ_NetworkConfig{
    NSMutableArray<id<MRJ_UrlFilterProtocol>> *_urlFilters;
    NSMutableArray<id<MRJ_CacheDirPathFilterProtocol>> *_cacheDirPathFilters;
}

+ (MRJ_NetworkConfig *)sharedConfig {
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

- (void)addUrlFilter:(id<MRJ_UrlFilterProtocol>)filter {
    [_urlFilters addObject:filter];
}

- (void)clearUrlFilter {
    [_urlFilters removeAllObjects];
}

- (void)addCacheDirPathFilter:(id<MRJ_CacheDirPathFilterProtocol>)filter {
    [_cacheDirPathFilters addObject:filter];
}

- (void)clearCacheDirPathFilter {
    [_cacheDirPathFilters removeAllObjects];
}

- (NSArray<id<MRJ_UrlFilterProtocol>> *)urlFilters {
    return [_urlFilters copy];
}

- (NSArray<id<MRJ_CacheDirPathFilterProtocol>> *)cacheDirPathFilters {
    return [_cacheDirPathFilters copy];
}

#pragma mark - NSObject

- (NSString *)description {
    return [NSString stringWithFormat:@"<%@: %p>{ baseURL: %@ } { cdnURL: %@ }", NSStringFromClass([self class]), self, self.baseUrl, self.cdnUrl];
}

@end
