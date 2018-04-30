//
//  MRJNetworkAgent.m
//
//  Created by MRJ on 2017/2/17.
//  Copyright © 2017年 MRJ. All rights reserved.
//

#import "MRJNetworkAgent.h"
#import <AFNetworking/AFNetworking.h>
#import "MRJNetworkConfig.h"
#import <pthread/pthread.h>
#import "MRJNetworkPrivate.h"

#define Lock() pthread_mutex_lock(&_lock)
#define Unlock() pthread_mutex_unlock(&_lock)

#define MRJKNetworkIncompleteDownloadFolderName @"Incomplete"

@implementation MRJNetworkAgent{
    AFHTTPSessionManager *_manager;
    MRJNetworkConfig *_config;
    AFJSONResponseSerializer *_jsonResponseSerializer;
    AFXMLParserResponseSerializer *_xmlParserResponseSerialzier;
    NSMutableDictionary<NSNumber *, MRJBaseRequest *> *_requestsRecord;
    pthread_mutex_t _lock;
}

+ (MRJNetworkAgent *)sharedAgent {
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
        _config = [MRJNetworkConfig sharedConfig];
        _manager = [[AFHTTPSessionManager alloc] initWithSessionConfiguration:_config.sessionConfiguration];
        _requestsRecord = [NSMutableDictionary dictionary];
        /// 接收返回状态码
        _manager.securityPolicy = _config.securityPolicy;
        _manager.responseSerializer = [AFHTTPResponseSerializer serializer];
    }
    return self;
}

- (AFJSONResponseSerializer *)jsonResponseSerializer {
    if (!_jsonResponseSerializer) {
        _jsonResponseSerializer = [AFJSONResponseSerializer serializer];
    }
    return _jsonResponseSerializer;
}

- (AFXMLParserResponseSerializer *)xmlParserResponseSerialzier {
    if (!_xmlParserResponseSerialzier) {
        _xmlParserResponseSerialzier = [AFXMLParserResponseSerializer serializer];
    }
    return _xmlParserResponseSerialzier;
}

#pragma mark -
/// 构建网络请求路径
- (NSString *)buildRequestUrl:(MRJBaseRequest *)request {
    NSParameterAssert(request != nil);
    NSString *detailUrl = [request requestUrl];
    NSURL *temp = [NSURL URLWithString:detailUrl];
    /// Filter URL if needed
    NSArray *filters = [_config urlFilters];
    for (id<MRJUrlFilterProtocol> f in filters) {
        detailUrl = [f filterUrl:detailUrl withRequest:request];
    }
    
    /// If detailUrl is valid URL
    if (temp && temp.host && temp.scheme) {
        return detailUrl;
    }
    
    NSString *baseUrl;
    if ([request useCDN]) {
        if ([request cdnUrl].length > 0) {
            baseUrl = [request cdnUrl];
        } else {
            baseUrl = [_config cdnUrl];
        }
    } else {
        if ([request baseUrl].length > 0) {
            baseUrl = [request baseUrl];
        } else {
            baseUrl = [_config baseUrl];
        }
    }
    
    /// URL slash compability
    NSURL *url = [NSURL URLWithString:baseUrl];
    
    if (baseUrl.length > 0 && ![baseUrl hasSuffix:@"/"]) {
        url = [url URLByAppendingPathComponent:@""];
    }
    
    NSString *finalDetailUrl = [NSMutableString stringWithString:detailUrl];
    if([detailUrl rangeOfString:@"？"].location ==NSNotFound) {
        if(request.resetfulArguments) {
            if(request.resetfulArguments != NULL ){
                for(NSString *arg in request.resetfulArguments) {
                    finalDetailUrl = [finalDetailUrl  stringByAppendingFormat:@"/%@",arg];
                }
            }
        }
    }
    return [NSURL URLWithString:finalDetailUrl relativeToURL:url].absoluteString;
}

- (AFHTTPRequestSerializer *)requestSerializerForRequest:(MRJBaseRequest *)request {
    AFHTTPRequestSerializer *requestSerializer = nil;
    if (request.requestSerializerType == MRJRequestSerializerTypeHTTP) {
        requestSerializer = [AFHTTPRequestSerializer serializer];
    } else if (request.requestSerializerType == MRJRequestSerializerTypeJSON) {
        requestSerializer = [AFJSONRequestSerializer serializer];
    }
    
    requestSerializer.timeoutInterval = [request requestTimeoutInterval];
    /// If api needs server username and password
    NSArray<NSString *> *authorizationHeaderFieldArray = [request requestAuthorizationHeaderFieldArray];
    if (authorizationHeaderFieldArray != nil) {
        [requestSerializer setAuthorizationHeaderFieldWithUsername:authorizationHeaderFieldArray.firstObject
                                                          password:authorizationHeaderFieldArray.lastObject];
    }
    
    /// If api needs to add custom value to HTTPHeaderField
    NSDictionary<NSString *, NSString *> *headerFieldValueDictionary = [request requestHeaderFieldValueDictionary];
    if (headerFieldValueDictionary != nil) {
        for (NSString *httpHeaderField in headerFieldValueDictionary.allKeys) {
            NSString *value = headerFieldValueDictionary[httpHeaderField];
            [requestSerializer setValue:value forHTTPHeaderField:httpHeaderField];
        }
    }
    return requestSerializer;
}

- (NSURLSessionTask *)sessionTaskForRequest:(MRJBaseRequest *)request error:(NSError **)error {
    MRJRequestMethod method = [request requestMethod];
    NSString *url = [self buildRequestUrl:request];
    id param = request.requestArgument;
    AFConstructingBlock constructingBlock = [request constructingBodyBlock];
    AFHTTPRequestSerializer *requestSerializer = [self requestSerializerForRequest:request];
    switch (method) {
        case MRJRequestMethodGET:
            if (request.resumableDownloadPath) {
                return [self downloadTaskWithDownloadPath:request.resumableDownloadPath requestSerializer:requestSerializer URLString:url parameters:param progress:request.resumableDownloadProgressBlock error:error];
            } else {
                return [self dataTaskWithHTTPMethod:@"GET" requestSerializer:requestSerializer URLString:url parameters:param error:error];
            }
        case MRJRequestMethodPOST:
            return [self dataTaskWithHTTPMethod:@"POST" requestSerializer:requestSerializer URLString:url parameters:param progress:request.uploadProgressBlock  constructingBodyWithBlock:constructingBlock error:error];
        case MRJRequestMethodHEAD:
            return [self dataTaskWithHTTPMethod:@"HEAD" requestSerializer:requestSerializer URLString:url parameters:param error:error];
        case MRJRequestMethodPUT:
            return [self dataTaskWithHTTPMethod:@"PUT" requestSerializer:requestSerializer URLString:url parameters:param error:error];
        case MRJRequestMethodDELETE:
            return [self dataTaskWithHTTPMethod:@"DELETE" requestSerializer:requestSerializer URLString:url parameters:param error:error];
        case MRJRequestMethodPATCH:
            return [self dataTaskWithHTTPMethod:@"PATCH" requestSerializer:requestSerializer URLString:url parameters:param error:error];
    }
}

/// 请求准备开始
- (void)addRequest:(MRJBaseRequest *)request {
    NSParameterAssert(request != nil);
    NSError * __autoreleasing requestSerializationError = nil;
    /// 取自定义路径
    NSURLRequest *customUrlRequest = [request buildCustomUrlRequest];
    if (customUrlRequest) {
        __block NSURLSessionDataTask *dataTask = nil;
        dataTask = [_manager dataTaskWithRequest:customUrlRequest completionHandler:^(NSURLResponse * _Nonnull response, id  _Nullable responseObject, NSError * _Nullable error) {
            [self handleRequestResult:dataTask responseObject:responseObject error:error];
        }];
        request.requestTask = dataTask;
    } else {
        request.requestTask = [self sessionTaskForRequest:request error:&requestSerializationError];
    }
    if (requestSerializationError) {
        [self requestDidFailWithRequest:request error:requestSerializationError];
        return;
    }
    NSAssert(request.requestTask != nil, @"requestTask should not be nil");
    // Set request task priority
    // !!Available on iOS 8 +
    if ([request.requestTask respondsToSelector:@selector(priority)]) {
        switch (request.requestPriority) {
            case MRJRequestPriorityHigh:
                request.requestTask.priority = 1.0;
                break;
            case MRJRequestPriorityLow:
                request.requestTask.priority = 0;
                break;
            case MRJRequestPriorityDefault:
                /*!!fall through*/
            default:
                // NSURLSession​Task​Priority​Default等于0.5，但NSURLSession​Task​Priority​Default在iOS8上会crash，报exc_bad_access
                request.requestTask.priority=0.5;
                break;
        }
    }
    /// Retain request
    MRJLog(@"Add request: %@", NSStringFromClass([request class]));
    [self addRequestToRecord:request];
    [request.requestTask resume];
}

- (void)cancelRequest:(MRJBaseRequest *)request {
    NSParameterAssert(request != nil);
    [request.requestTask cancel];
    [self removeRequestFromRecord:request];
    [request clearCompletionBlock];
}

- (void)cancelAllRequests {
    Lock();
    NSArray *allKeys = [_requestsRecord allKeys];
    Unlock();
    if (allKeys && allKeys.count > 0) {
        NSArray *copiedKeys = [allKeys copy];
        for (NSNumber *key in copiedKeys) {
            Lock();
            MRJBaseRequest *request = _requestsRecord[key];
            Unlock();
            // We are using non-recursive lock.
            // Do not lock `stop`, otherwise deadlock may occur.
            [request stop];
        }
    }
}

- (BOOL)validateResult:(MRJBaseRequest *)request error:(NSError * _Nullable __autoreleasing *)error {
    BOOL result = [request statusCodeValidator];
    if (!result) {
        if (error) {
            *error = [NSError errorWithDomain:MRJRequestValidationErrorDomain code:MRJRequestValidationErrorInvalidStatusCode userInfo:@{NSLocalizedDescriptionKey:@"Invalid status code"}];
        }
        return result;
    }
    id json = [request responseJSONObject];
    id validator = [request jsonValidator];
    if (json && validator) {
        result = [MRJNetworkUtils validateJSON:json withValidator:validator];
        if (!result) {
            if (error) {
                *error = [NSError errorWithDomain:MRJRequestValidationErrorDomain code:MRJRequestValidationErrorInvalidJSONFormat userInfo:@{NSLocalizedDescriptionKey:@"Invalid JSON format"}];
            }
            return result;
        }
    }
    return YES;
}

#pragma http交互
#pragma mark -

- (NSURLSessionDataTask *)dataTaskWithHTTPMethod:(NSString *)method
                               requestSerializer:(AFHTTPRequestSerializer *)requestSerializer
                                       URLString:(NSString *)URLString
                                      parameters:(id)parameters
                                           error:(NSError * _Nullable __autoreleasing *)error {
    return [self dataTaskWithHTTPMethod:method requestSerializer:requestSerializer URLString:URLString parameters:parameters progress:nil constructingBodyWithBlock:nil  error:error];
}

- (NSURLSessionDataTask *)dataTaskWithHTTPMethod:(NSString *)method
                               requestSerializer:(AFHTTPRequestSerializer *)requestSerializer
                                       URLString:(NSString *)URLString
                                      parameters:(id)parameters
                                        progress:(nullable void (^)(NSProgress *uploadProgress))uploadProgressBlock
                       constructingBodyWithBlock:(nullable void (^)(id <AFMultipartFormData> formData))block
                                           error:(NSError * _Nullable __autoreleasing *)error {
    NSMutableURLRequest *request = nil;
    if (block) {
        request = [requestSerializer multipartFormRequestWithMethod:method URLString:URLString parameters:parameters constructingBodyWithBlock:block error:error];
    } else {
        request = [requestSerializer requestWithMethod:method URLString:URLString parameters:parameters error:error];
    }
    __block NSURLSessionDataTask *dataTask = nil;
    /// 是否有上传回调
    if(uploadProgressBlock){
        dataTask = [_manager uploadTaskWithStreamedRequest:request progress:^(NSProgress * _Nonnull uploadProgress) {
            uploadProgressBlock(uploadProgress);
        } completionHandler:^(NSURLResponse * _Nonnull response, id  _Nullable responseObject, NSError * _Nullable error) {
            [self handleRequestResult:dataTask responseObject:responseObject error:error];
        }];
    } else {
        dataTask = [_manager dataTaskWithRequest:request
                               completionHandler:^(NSURLResponse * __unused response, id responseObject, NSError *_error) {
                                   [self handleRequestResult:dataTask responseObject:responseObject error:_error];
                               }];
    }
    return dataTask;
}


- (void)handleRequestResult:(NSURLSessionTask *)task responseObject:(id)responseObject error:(NSError *)error {
    Lock();
    MRJBaseRequest *request = _requestsRecord[@(task.taskIdentifier)];
    Unlock();
    if (!request) {
        return;
    }
    MRJLog(@"Finished Request: %@", NSStringFromClass([request class]));
    NSError * __autoreleasing serializationError = nil;
    NSError * __autoreleasing validationError = nil;
    NSError *requestError = nil;
    BOOL succeed = NO;
    request.responseObject = responseObject;
    if ([request.responseObject isKindOfClass:[NSData class]]) {
        request.responseData = responseObject;
        request.responseString = [[NSString alloc] initWithData:responseObject encoding:[MRJNetworkUtils stringEncodingWithRequest:request]];
        
        switch (request.responseSerializerType) {
            case MRJResponseSerializerTypeHTTP:
                // Default serializer. Do nothing.
                break;
            case MRJResponseSerializerTypeJSON:
                request.responseObject = [self.jsonResponseSerializer responseObjectForResponse:task.response data:request.responseData error:&serializationError];
                request.responseJSONObject = request.responseObject;
                break;
            case MRJResponseSerializerTypeXMLParser:
                request.responseObject = [self.xmlParserResponseSerialzier responseObjectForResponse:task.response data:request.responseData error:&serializationError];
                break;
        }
    }
    if (error) {
        succeed = NO;
        requestError = error;
    } else if (serializationError) {
        succeed = NO;
        requestError = serializationError;
    } else {
        succeed = [self validateResult:request error:&validationError];
        requestError = validationError;
    }
    
    if (succeed) {
        [self requestDidSucceedWithRequest:request];
    } else {
        [self requestDidFailWithRequest:request error:requestError];
    }
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [self removeRequestFromRecord:request];
        [request clearCompletionBlock];
    });
}

- (void)requestDidSucceedWithRequest:(MRJBaseRequest *)request {
    @autoreleasepool {
        [request requestCompletePreprocessor];
    }
    dispatch_async(dispatch_get_main_queue(), ^{
        [request toggleAccessoriesWillStopCallBack];
        [request requestCompleteFilter];
        if (request.delegate != nil) {
            [request.delegate requestFinished:request];
        }
        if (request.successCompletionBlock) {
            request.successCompletionBlock(request);
        }
        [request toggleAccessoriesDidStopCallBack];
    });
}

- (void)requestDidFailWithRequest:(MRJBaseRequest *)request error:(NSError *)error {
    request.error = error;
    MRJLog(@"Request %@ failed, status code = %ld, error = %@",
    NSStringFromClass([request class]), (long)request.responseStatusCode, error.localizedDescription);
    
    // Save incomplete download data.
    NSData *incompleteDownloadData = error.userInfo[NSURLSessionDownloadTaskResumeData];
    if (incompleteDownloadData) {
        [incompleteDownloadData writeToURL:[self incompleteDownloadTempPathForDownloadPath:request.resumableDownloadPath] atomically:YES];
    }
    // Load response from file and clean up if download task failed.
    if ([request.responseObject isKindOfClass:[NSURL class]]) {
        NSURL *url = request.responseObject;
        if (url.isFileURL && [[NSFileManager defaultManager] fileExistsAtPath:url.path]) {
            request.responseData = [NSData dataWithContentsOfURL:url];
            request.responseString = [[NSString alloc] initWithData:request.responseData encoding:[MRJNetworkUtils stringEncodingWithRequest:request]];
            
            [[NSFileManager defaultManager] removeItemAtURL:url error:nil];
        }
        request.responseObject = nil;
    }
    @autoreleasepool {
        [request requestFailedPreprocessor];
    }
    dispatch_async(dispatch_get_main_queue(), ^{
        [request toggleAccessoriesWillStopCallBack];
        [request requestFailedFilter];
        
        if (request.delegate != nil) {
            [request.delegate requestFailed:request];
        }
        if (request.failureCompletionBlock) {
            request.failureCompletionBlock(request);
        }
        [request toggleAccessoriesDidStopCallBack];
    });
}

- (void)addRequestToRecord:(MRJBaseRequest *)request {
    Lock();
    _requestsRecord[@(request.requestTask.taskIdentifier)] = request;
    Unlock();
}

- (void)removeRequestFromRecord:(MRJBaseRequest *)request {
    Lock();
    [_requestsRecord removeObjectForKey:@(request.requestTask.taskIdentifier)];
    MRJLog(@"Request queue size = %zd", [_requestsRecord count]);
    Unlock();
}

#pragma 文件下载
- (NSURLSessionDownloadTask *)downloadTaskWithDownloadPath:(NSString *)downloadPath
                                         requestSerializer:(AFHTTPRequestSerializer *)requestSerializer
                                                 URLString:(NSString *)URLString
                                                parameters:(id)parameters
                                                  progress:(nullable void (^)(NSProgress *downloadProgress))downloadProgressBlock
                                                     error:(NSError * _Nullable __autoreleasing *)error {
    // add parameters to URL;
    NSMutableURLRequest *urlRequest = [requestSerializer requestWithMethod:@"GET" URLString:URLString parameters:parameters error:error];
    
    NSString *downloadTargetPath;
    BOOL isDirectory;
    if(![[NSFileManager defaultManager] fileExistsAtPath:downloadPath isDirectory:&isDirectory]) {
        isDirectory = NO;
    }
    // If targetPath is a directory, use the file name we got from the urlRequest.
    // Make sure downloadTargetPath is always a file, not directory.
    if (isDirectory) {
        NSString *fileName = [urlRequest.URL lastPathComponent];
        downloadTargetPath = [NSString pathWithComponents:@[downloadPath, fileName]];
    } else {
        downloadTargetPath = downloadPath;
    }
    
    // AFN use `moveItemAtURL` to move downloaded file to target path,
    // this method aborts the move attempt if a file already exist at the path.
    // So we remove the exist file before we start the download task.
    // https://github.com/AFNetworking/AFNetworking/issues/3775
    if ([[NSFileManager defaultManager] fileExistsAtPath:downloadTargetPath]) {
        [[NSFileManager defaultManager] removeItemAtPath:downloadTargetPath error:nil];
    }
    BOOL resumeDataFileExists = [[NSFileManager defaultManager] fileExistsAtPath:[self incompleteDownloadTempPathForDownloadPath:downloadPath].path];
    NSData *data = [NSData dataWithContentsOfURL:[self incompleteDownloadTempPathForDownloadPath:downloadPath]];
    BOOL resumeDataIsValid = [MRJNetworkUtils validateResumeData:data];
    BOOL canBeResumed = resumeDataFileExists && resumeDataIsValid;
    BOOL resumeSucceeded = NO;
    __block NSURLSessionDownloadTask *downloadTask = nil;
    // Try to resume with resumeData.
    // Even though we try to validate the resumeData, this may still fail and raise excecption.
    if (canBeResumed) {
        @try {
            downloadTask = [_manager downloadTaskWithResumeData:data progress:downloadProgressBlock destination:^NSURL * _Nonnull(NSURL * _Nonnull targetPath, NSURLResponse * _Nonnull response) {
                return [NSURL fileURLWithPath:downloadTargetPath isDirectory:NO];
            } completionHandler:
                            ^(NSURLResponse * _Nonnull response, NSURL * _Nullable filePath, NSError * _Nullable error) {
                                [self handleRequestResult:downloadTask responseObject:filePath error:error];
                            }];
            resumeSucceeded = YES;
        } @catch (NSException *exception) {
            MRJLog(@"Resume download failed, reason = %@", exception.reason);
            resumeSucceeded = NO;
        }
    }
    if (!resumeSucceeded) {
        downloadTask = [_manager downloadTaskWithRequest:urlRequest progress:downloadProgressBlock destination:^NSURL * _Nonnull(NSURL * _Nonnull targetPath, NSURLResponse * _Nonnull response) {
            return [NSURL fileURLWithPath:downloadTargetPath isDirectory:NO];
        } completionHandler:
                        ^(NSURLResponse * _Nonnull response, NSURL * _Nullable filePath, NSError * _Nullable error) {
                            [self handleRequestResult:downloadTask responseObject:filePath error:error];
                        }];
    }
    return downloadTask;
}

#pragma mark - Resumable Download

- (NSString *)incompleteDownloadTempCacheFolder {
    NSFileManager *fileManager = [NSFileManager new];
    static NSString *cacheFolder;
    if (!cacheFolder) {
        NSString *cacheDir = NSTemporaryDirectory();
        cacheFolder = [cacheDir stringByAppendingPathComponent:MRJKNetworkIncompleteDownloadFolderName];
    }
    NSError *error = nil;
    if(![fileManager createDirectoryAtPath:cacheFolder withIntermediateDirectories:YES attributes:nil error:&error]) {
        MRJLog(@"Failed to create cache directory at %@", cacheFolder);
        cacheFolder = nil;
    }
    return cacheFolder;
}

- (NSURL *)incompleteDownloadTempPathForDownloadPath:(NSString *)downloadPath {
    NSString *tempPath = nil;
    NSString *md5URLString = [MRJNetworkUtils md5StringFromString:downloadPath];
    tempPath = [[self incompleteDownloadTempCacheFolder] stringByAppendingPathComponent:md5URLString];
    return [NSURL fileURLWithPath:tempPath];
}

#pragma mark - Testing

- (AFHTTPSessionManager *)manager {
    return _manager;
}

- (void)resetURLSessionManager {
    _manager = [AFHTTPSessionManager manager];
}

- (void)resetURLSessionManagerWithConfiguration:(NSURLSessionConfiguration *)configuration {
    _manager = [[AFHTTPSessionManager alloc] initWithSessionConfiguration:configuration];
}

@end
