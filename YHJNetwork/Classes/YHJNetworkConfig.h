//
//  YHJNetworkConfig.h
//  MRJ
//
//  Created by YHJ on 2017/2/20.
//  Copyright © 2017年 YHJ. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <AFNetworking/AFNetworking.h>

@class YHJNetworkConfig;
@class YHJBaseRequest;

///  YHJUrlFilterProtocol can be used to append common parameters to requests before sending them.
@protocol YHJUrlFilterProtocol <NSObject>
///  Preprocess request URL before actually sending them.
///
///  @param originUrl request's origin URL, which is returned by `requestUrl`
///  @param request   request itself
///
///  @return A new url which will be used as a new `requestUrl`
- (NSString *)filterUrl:(NSString *)originUrl withRequest:(YHJBaseRequest *)request;
@end

///  YHJCacheDirPathFilterProtocol can be used to append common path components when caching response results
@protocol YHJCacheDirPathFilterProtocol <NSObject>
///  Preprocess cache path before actually saving them.
///
///  @param originPath original base cache path, which is generated in `YHJRequest` class.
///  @param request    request itself
///
///  @return A new path which will be used as base path when caching.
- (NSString *)filterCacheDirPath:(NSString *)originPath withRequest:(YHJBaseRequest *)request;
@end


@interface YHJNetworkConfig : NSObject

- (instancetype)init NS_UNAVAILABLE ;
+ (instancetype)new NS_UNAVAILABLE;

///  Return a shared config object.
+ (YHJNetworkConfig *)sharedConfig;

///  Request base URL, such as "http://www.mrj.com". Default is empty string.
@property (nonatomic, strong) NSString *baseUrl;
///  Request CDN URL. Default is empty string.
@property (nonatomic, strong) NSString *cdnUrl;


///  URL filters. See also `YHJUrlFilterProtocol`.
@property (nonatomic, strong, readonly) NSArray<id<YHJUrlFilterProtocol>> *urlFilters;
///  Cache path filters. See also `YHJCacheDirPathFilterProtocol`.
@property (nonatomic, strong, readonly) NSArray<id<YHJCacheDirPathFilterProtocol>> *cacheDirPathFilters;
///  Security policy will be used by AFNetworking. See also `AFSecurityPolicy`.
@property (nonatomic, strong) AFSecurityPolicy *securityPolicy;
///  Whether to log debug info. Default is NO;
@property (nonatomic) BOOL debugLogEnabled;
///  SessionConfiguration will be used to initialize AFHTTPSessionManager. Default is nil.
@property (nonatomic, strong) NSURLSessionConfiguration* sessionConfiguration;

///  Add a new URL filter.
- (void)addUrlFilter:(id<YHJUrlFilterProtocol>)filter;
///  Remove all URL filters.
- (void)clearUrlFilter;
///  Add a new cache path filter
- (void)addCacheDirPathFilter:(id<YHJCacheDirPathFilterProtocol>)filter;
///  Clear all cache path filters.
- (void)clearCacheDirPathFilter;


@end
