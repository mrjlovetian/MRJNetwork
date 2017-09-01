//
//  MRJ_NetworkConfig.h
//  MRJ
//
//  Created by MRJ_ on 2017/2/20.
//  Copyright © 2017年 MRJ_. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <AFNetworking/AFNetworking.h>

@class MRJ_NetworkConfig;
@class MRJ_BaseRequest;

///  MRJ_UrlFilterProtocol can be used to append common parameters to requests before sending them.
@protocol MRJ_UrlFilterProtocol <NSObject>
///  Preprocess request URL before actually sending them.
///
///  @param originUrl request's origin URL, which is returned by `requestUrl`
///  @param request   request itself
///
///  @return A new url which will be used as a new `requestUrl`
- (NSString *)filterUrl:(NSString *)originUrl withRequest:(MRJ_BaseRequest *)request;
@end

///  MRJ_CacheDirPathFilterProtocol can be used to append common path components when caching response results
@protocol MRJ_CacheDirPathFilterProtocol <NSObject>
///  Preprocess cache path before actually saving them.
///
///  @param originPath original base cache path, which is generated in `MRJ_Request` class.
///  @param request    request itself
///
///  @return A new path which will be used as base path when caching.
- (NSString *)filterCacheDirPath:(NSString *)originPath withRequest:(MRJ_BaseRequest *)request;
@end


@interface MRJ_NetworkConfig : NSObject

- (instancetype)init NS_UNAVAILABLE ;
+ (instancetype)new NS_UNAVAILABLE;

///  Return a shared config object.
+ (MRJ_NetworkConfig *)sharedConfig;

///  Request base URL, such as "http://www.mrj.com". Default is empty string.
@property (nonatomic, strong) NSString *baseUrl;
///  Request CDN URL. Default is empty string.
@property (nonatomic, strong) NSString *cdnUrl;


///  URL filters. See also `MRJ_UrlFilterProtocol`.
@property (nonatomic, strong, readonly) NSArray<id<MRJ_UrlFilterProtocol>> *urlFilters;
///  Cache path filters. See also `MRJ_CacheDirPathFilterProtocol`.
@property (nonatomic, strong, readonly) NSArray<id<MRJ_CacheDirPathFilterProtocol>> *cacheDirPathFilters;
///  Security policy will be used by AFNetworking. See also `AFSecurityPolicy`.
@property (nonatomic, strong) AFSecurityPolicy *securityPolicy;
///  Whether to log debug info. Default is NO;
@property (nonatomic) BOOL debugLogEnabled;
///  SessionConfiguration will be used to initialize AFHTTPSessionManager. Default is nil.
@property (nonatomic, strong) NSURLSessionConfiguration* sessionConfiguration;

///  Add a new URL filter.
- (void)addUrlFilter:(id<MRJ_UrlFilterProtocol>)filter;
///  Remove all URL filters.
- (void)clearUrlFilter;
///  Add a new cache path filter
- (void)addCacheDirPathFilter:(id<MRJ_CacheDirPathFilterProtocol>)filter;
///  Clear all cache path filters.
- (void)clearCacheDirPathFilter;


@end
