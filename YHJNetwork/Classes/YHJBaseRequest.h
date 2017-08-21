//
//  YHJBaseRequest.h
//  TopsTechNetWorking
//
//  Created by YHJ on 2017/2/17.
//  Copyright © 2017年 YHJ. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AFNetworking/AFNetworking.h>

NS_ASSUME_NONNULL_BEGIN


FOUNDATION_EXPORT NSString *const YHJRequestValidationErrorDomain ;

NS_ENUM(NSInteger) {
    YHJRequestValidationErrorInvalidStatusCode = -8,
    YHJRequestValidationErrorInvalidJSONFormat = -9,
};

    
///  HTTP Request method. 请求方式
typedef NS_ENUM(NSInteger, YHJRequestMethod) {
        YHJRequestMethodGET = 0,
        YHJRequestMethodPOST,
        YHJRequestMethodHEAD,
        YHJRequestMethodPUT,
        YHJRequestMethodDELETE,
        YHJRequestMethodPATCH,
};
    
///  Request serializer type. 网络请求类型
typedef NS_ENUM(NSInteger, YHJRequestSerializerType) {
        YHJRequestSerializerTypeHTTP = 0,
        YHJRequestSerializerTypeJSON,
};

///  Response serializer type, which determines response serialization process and
///  the type of `responseObject`. 网络响应输出类型
typedef NS_ENUM(NSInteger, YHJResponseSerializerType) {
    /// NSData type
    YHJResponseSerializerTypeHTTP,
    /// JSON object type
    YHJResponseSerializerTypeJSON,
    /// NSXMLParser type
    YHJResponseSerializerTypeXMLParser,
};

    
///  Request priority 请求优先级
typedef NS_ENUM(NSInteger, YHJRequestPriority) {
    YHJRequestPriorityLow = -4L,
    YHJRequestPriorityDefault = 0,
    YHJRequestPriorityHigh = 4,
};

@protocol AFMultipartFormData;
    
typedef void (^AFConstructingBlock)(id<AFMultipartFormData> formData);
typedef void (^AFURLSessionTaskProgressBlock)(NSProgress *);
   
@class YHJBaseRequest;
typedef void(^YHJRequestCompletionBlock)(__kindof YHJBaseRequest *request);
typedef void(^YHJRequestFinishBlock)();
    
///  The YTKRequestDelegate protocol defines several optional methods you can use
///  to receive network-related messages. All the delegate methods will be called
///  on the main queue.
@protocol YHJRequestDelegate <NSObject>
    
    @optional
    ///  Tell the delegate that the request has finished successfully.
    ///
    ///  @param request The corresponding request.
    - (void)requestFinished:(__kindof YHJBaseRequest *)request;
    
    ///  Tell the delegate that the request has failed.
    ///
    ///  @param request The corresponding request.
    - (void)requestFailed:(__kindof YHJBaseRequest *)request;
    
@end
    

///  The YHJRequestAccessory protocol defines several optional methods that can be
///  used to track the status of a request. Objects that conforms this protocol
///  ("accessories") can perform additional configurations accordingly. All the
///  accessory methods will be called on the main queue.
@protocol YHJRequestAccessory <NSObject>
    
    @optional
    
    ///  Inform the accessory that the request is about to start.
    ///
    ///  @param request The corresponding request.
    - (void)requestWillStart:(id)request;
    
    ///  Inform the accessory that the request is about to stop. This method is called
    ///  before executing `requestFinished` and `successCompletionBlock`.
    ///
    ///  @param request The corresponding request.
    - (void)requestWillStop:(id)request;
    
    ///  Inform the accessory that the request has already stoped. This method is called
    ///  after executing `requestFinished` and `successCompletionBlock`.
    ///
    ///  @param request The corresponding request.
    - (void)requestDidStop:(id)request;
    
@end

    
@interface YHJBaseRequest : NSObject

#pragma mark - Request and Response Information
///=============================================================================
/// @name Request and Response Information
///=============================================================================

///  The underlying NSURLSessionTask.
///
///  @warning This value is actually nil and should not be accessed before the request starts.
@property (nonatomic, strong, readonly) NSURLSessionTask *requestTask;

///  Shortcut for `requestTask.currentRequest`.
@property (nonatomic, strong, readonly) NSURLRequest *currentRequest;

///  Shortcut for `requestTask.originalRequest`.
@property (nonatomic, strong, readonly) NSURLRequest *originalRequest;

///  Shortcut for `requestTask.response`.
@property (nonatomic, strong, readonly) NSHTTPURLResponse *response;

///  The response status code.
@property (nonatomic, readonly) NSInteger responseStatusCode;

///  The response header fields.
@property (nonatomic, strong, readonly, nullable) NSDictionary *responseHeaders;

///  The raw data representation of response. Note this value can be nil if request failed.
@property (nonatomic, strong, readonly, nullable) NSData *responseData;

///  The string representation of response. Note this value can be nil if request failed.
@property (nonatomic, strong, readonly, nullable) NSString *responseString;

///  This serialized response object. The actual type of this object is determined by
///  `YTKResponseSerializerType`. Note this value can be nil if request failed.
///
///  @discussion If `resumableDownloadPath` and DownloadTask is using, this value will
///              be the path to which file is successfully saved (NSURL), or nil if request failed.
@property (nonatomic, strong, readonly, nullable) id responseObject;

///  If you use `YTKResponseSerializerTypeJSON`, this is a convenience (and sematic) getter
///  for the response object. Otherwise this value is nil.
@property (nonatomic, strong, readonly, nullable) id responseJSONObject;

///  This error can be either serialization error or network error. If nothing wrong happens
///  this value will be nil.
@property (nonatomic, strong, readonly, nullable) NSError *error;

///  Return cancelled state of request task.
@property (nonatomic, readonly, getter=isCancelled) BOOL cancelled;

///  Executing state of request task.
@property (nonatomic, readonly, getter=isExecuting) BOOL executing;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////请求参数
@property(nonatomic,strong) id requestArgument;

///用于封装restful地址的参数集合
@property(nonatomic,copy) NSArray<NSString *> *resetfulArguments;

///  Additional HTTP request header field.
@property(nonatomic,strong)  NSDictionary<NSString *, NSString *> *requestHeaderFieldValueDictionary;

#pragma mark - Request Configuration
///=============================================================================
/// @name Request Configuration
///=============================================================================

///  Tag can be used to identify request. Default value is 0.
@property (nonatomic) NSInteger tag;

///  The userInfo can be used to store additional info about the request. Default is nil.
@property (nonatomic, strong, nullable) NSDictionary *userInfo;

///  The delegate object of the request. If you choose block style callback you can ignore this.
///  Default is nil.
@property (nonatomic, weak, nullable) id<YHJRequestDelegate> delegate;

///  The success callback. Note if this value is not nil and `requestFinished` delegate method is
///  also implemented, both will be executed but delegate method is first called. This block
///  will be called on the main queue.
@property (nonatomic, copy, nullable) YHJRequestCompletionBlock successCompletionBlock;

///  The failure callback. Note if this value is not nil and `requestFailed` delegate method is
///  also implemented, both will be executed but delegate method is first called. This block
///  will be called on the main queue.
@property (nonatomic, copy, nullable) YHJRequestCompletionBlock failureCompletionBlock;

///  This can be used to add several accossories object. Note if you use `addAccessory` to add acceesory
///  this array will be automatically created. Default is nil.
@property (nonatomic, strong, nullable) NSMutableArray<id<YHJRequestAccessory>> *requestAccessories;

///  This can be use to construct HTTP body when needed in POST request. Default is nil.
@property (nonatomic, copy, nullable) AFConstructingBlock constructingBodyBlock;

///  This value is used to perform resumable download request. Default is nil.
///
///  @discussion NSURLSessionDownloadTask is used when this value is not nil.
///              The exist file at the path will be removed before the request starts. If request succeed, file will
///              be saved to this path automatically, otherwise the response will be saved to `responseData`
///              and `responseString`. For this to work, server must support `Range` and response with
///              proper `Last-Modified` and/or `Etag`. See `NSURLSessionDownloadTask` for more detail.
@property (nonatomic, strong, nullable) NSString *resumableDownloadPath;

///  You can use this block to track the download progress. See also `resumableDownloadPath`.
@property (nonatomic, copy, nullable) AFURLSessionTaskProgressBlock resumableDownloadProgressBlock;

/// 文件上传进度
@property (nonatomic, copy, nullable) AFURLSessionTaskProgressBlock uploadProgressBlock;

///  The priority of the request. Effective only on iOS 8+. Default is `YTKRequestPriorityDefault`.
@property (nonatomic) YHJRequestPriority requestPriority;

///  Set completion callbacks
- (void)setCompletionBlockWithSuccess:(nullable YHJRequestCompletionBlock)success
                              failure:(nullable YHJRequestCompletionBlock)failure;

///  Nil out both success and failure callback blocks.
- (void)clearCompletionBlock;


#pragma mark - Request Action
///=============================================================================
/// @name Request Action
///=============================================================================

///  Append self to request queue and start the request.
- (void)start;

///  Remove self from request queue and cancel the request.
- (void)stop;

///  Convenience method to start the request with block callbacks.
- (void)startWithCompletionBlockWithSuccess:(nullable YHJRequestCompletionBlock)success
                                    failure:(nullable YHJRequestCompletionBlock)failure;


#pragma mark - Subclass Override
///=============================================================================
/// @name Subclass Override
///=============================================================================

///  Called on background thread after request succeded but before switching to main thread. Note if
///  cache is loaded, this method WILL be called on the main thread, just like `requestCompleteFilter`.
- (void)requestCompletePreprocessor;

///  Called on the main thread after request succeeded.
- (void)requestCompleteFilter;

///  Called on background thread after request succeded but before switching to main thread. See also
///  `requestCompletePreprocessor`.
- (void)requestFailedPreprocessor;

///  Called on the main thread when request failed.
- (void)requestFailedFilter;

///  The baseURL of request. This should only contain the host part of URL, e.g., http://www.example.com.
///  See also `requestUrl`
- (NSString *)baseUrl;

///  The URL path of request. This should only contain the path part of URL, e.g., /v1/user. See alse `baseUrl`.
///
///  @discussion This will be concated with `baseUrl` using [NSURL URLWithString:relativeToURL].
///              Because of this, it is recommended that the usage should stick to rules stated above.
///              Otherwise the result URL may not be correctly formed. See also `URLString:relativeToURL`
///              for more information.
///
///              Additionaly, if `requestUrl` itself is a valid URL, it will be used as the result URL and
///              `baseUrl` will be ignored.
- (NSString *)requestUrl;

///  Optional CDN URL for request.
- (NSString *)cdnUrl;

///  Requset timeout interval. Default is 60s.
///
///  @discussion When using `resumableDownloadPath`(NSURLSessionDownloadTask), the session seems to completely ignore
///              `timeoutInterval` property of `NSURLRequest`. One effective way to set timeout would be using
///              `timeoutIntervalForResource` of `NSURLSessionConfiguration`.
- (NSTimeInterval)requestTimeoutInterval;

/////  Additional request argument.
//- (nullable id)requestArgument;

///  Override this method to filter requests with certain arguments when caching.
- (id)cacheFileNameFilterForRequestArgument:(id)argument;



///  HTTP request method.
- (YHJRequestMethod)requestMethod;

///  Request serializer type.
- (YHJRequestSerializerType)requestSerializerType;

///  Response serializer type. See also `responseObject`.
- (YHJResponseSerializerType)responseSerializerType;

///  Username and password used for HTTP authorization. Should be formed as @[@"Username", @"Password"].
- (nullable NSArray<NSString *> *)requestAuthorizationHeaderFieldArray;

/////  Additional HTTP request header field.
//- (nullable NSDictionary<NSString *, NSString *> *)requestHeaderFieldValueDictionary;

///  Use this to build custom request. If this method return non-nil value, `requestUrl`, `requestTimeoutInterval`,
///  `requestArgument`, `allowsCellularAccess`, `requestMethod` and `requestSerializerType` will all be ignored.
- (nullable NSURLRequest *)buildCustomUrlRequest;

///  Should use CDN when sending request.
- (BOOL)useCDN;


///  The validator will be used to test if `responseJSONObject` is correctly formed.
- (nullable id)jsonValidator;

///  This validator will be used to test if `responseStatusCode` is valid.
- (BOOL)statusCodeValidator;



@end

    
NS_ASSUME_NONNULL_END
