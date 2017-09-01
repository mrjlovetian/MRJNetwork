//
//  MRJ_NetworkPrivate.h
//  MRJ
//
//  Created by MRJ_ on 2017/2/20.
//  Copyright © 2017年 MRJ_. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "MRJ_Request.h"
#import "MRJ_BaseRequest.h"


#import "MRJ_NetworkAgent.h"
#import "MRJ_NetworkConfig.h"

#import "MRJ_BatchRequest.h"
#import "MRJ_ChainRequest.h"

NS_ASSUME_NONNULL_BEGIN

FOUNDATION_EXPORT void MRJ_Log(NSString *format, ...) NS_FORMAT_FUNCTION(1,2);

@class AFHTTPSessionManager;

@interface MRJ_NetworkUtils : NSObject

+ (BOOL)validateJSON:(id)json withValidator:(id)jsonValidator;

+ (void)addDoNotBackupAttribute:(NSString *)path;

+ (NSString *)md5StringFromString:(NSString *)string;

+ (NSString *)appVersionString;

+ (NSStringEncoding)stringEncodingWithRequest:(MRJ_BaseRequest *)request;

+ (BOOL)validateResumeData:(NSData *)data;

@end

@interface MRJ_Request (Getter)

- (NSString *)cacheBasePath;

@end

@interface MRJ_BaseRequest (Setter)

@property (nonatomic, strong, readwrite) NSURLSessionTask *requestTask;
@property (nonatomic, strong, readwrite, nullable) NSData *responseData;
@property (nonatomic, strong, readwrite, nullable) id responseJSONObject;
@property (nonatomic, strong, readwrite, nullable) id responseObject;
@property (nonatomic, strong, readwrite, nullable) NSString *responseString;
@property (nonatomic, strong, readwrite, nullable) NSError *error;

@end

@interface MRJ_BaseRequest (RequestAccessory)

- (void)toggleAccessoriesWillStartCallBack;
- (void)toggleAccessoriesWillStopCallBack;
- (void)toggleAccessoriesDidStopCallBack;

@end

@interface MRJ_BatchRequest (RequestAccessory)

- (void)toggleAccessoriesWillStartCallBack;
- (void)toggleAccessoriesWillStopCallBack;
- (void)toggleAccessoriesDidStopCallBack;

@end

@interface MRJ_ChainRequest (RequestAccessory)

- (void)toggleAccessoriesWillStartCallBack;
- (void)toggleAccessoriesWillStopCallBack;
- (void)toggleAccessoriesDidStopCallBack;

@end

@interface MRJ_NetworkAgent (Private)

- (AFHTTPSessionManager *)manager;
- (void)resetURLSessionManager;
- (void)resetURLSessionManagerWithConfiguration:(NSURLSessionConfiguration *)configuration;

- (NSString *)incompleteDownloadTempCacheFolder;

@end

NS_ASSUME_NONNULL_END
