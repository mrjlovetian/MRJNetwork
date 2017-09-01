//
//  MRJ_BaseRequest.h
//  MRJ
//
//  Created by MRJ_ on 2017/2/17.
//  Copyright © 2017年 MRJ_. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AFNetworking/AFNetworking.h>

NS_ASSUME_NONNULL_BEGIN


FOUNDATION_EXPORT NSString *const MRJ_RequestValidationErrorDomain ;

NS_ENUM(NSInteger) {
    MRJ_RequestValidationErrorInvalidStatusCode = -8,
    MRJ_RequestValidationErrorInvalidJSONFormat = -9,
};

    
///  HTTP Request method. 请求方式
typedef NS_ENUM(NSInteger, MRJ_RequestMethod) {
        MRJ_RequestMethodGET = 0,
        MRJ_RequestMethodPOST,
        MRJ_RequestMethodHEAD,
        MRJ_RequestMethodPUT,
        MRJ_RequestMethodDELETE,
        MRJ_RequestMethodPATCH,
};
    
///  Request serializer type. 网络请求类型
typedef NS_ENUM(NSInteger, MRJ_RequestSerializerType) {
        MRJ_RequestSerializerTypeHTTP = 0,
        MRJ_RequestSerializerTypeJSON,
};

///  Response serializer type, which determines response serialization process and
///  the type of `responseObject`. 网络响应输出类型
typedef NS_ENUM(NSInteger, MRJ_ResponseSerializerType) {
    /// NSData type
    MRJ_ResponseSerializerTypeHTTP,
    /// JSON object type
    MRJ_ResponseSerializerTypeJSON,
    /// NSXMLParser type
    MRJ_ResponseSerializerTypeXMLParser,
};

    
///  Request priority 请求优先级
typedef NS_ENUM(NSInteger, MRJ_RequestPriority) {
    MRJ_RequestPriorityLow = -4L,
    MRJ_RequestPriorityDefault = 0,
    MRJ_RequestPriorityHigh = 4,
};

@protocol AFMultipartFormData;
    
typedef void (^AFConstructingBlock)(id<AFMultipartFormData> formData);
typedef void (^AFURLSessionTaskProgressBlock)(NSProgress *);
   
@class MRJ_BaseRequest;
typedef void(^MRJ_RequestCompletionBlock)(__kindof MRJ_BaseRequest *request);
typedef void(^MRJ_RequestFinishBlock)();
    
///  The MRJ_RequestDelegate protocol defines several optional methods you can use
///  to receive network-related messages. All the delegate methods will be called
///  on the main queue.
@protocol MRJ_RequestDelegate <NSObject>
    
    @optional
    ///  Tell the delegate that the request has finished successfully.
    ///
    ///  @param request The corresponding request.
    - (void)requestFinished:(__kindof MRJ_BaseRequest *)request;
    
    ///  Tell the delegate that the request has failed.
    ///
    ///  @param request The corresponding request.
    - (void)requestFailed:(__kindof MRJ_BaseRequest *)request;
    
@end
    

///  The MRJ_RequestAccessory protocol defines several optional methods that can be
///  used to track the status of a request. Objects that conforms this protocol
///  ("accessories") can perform additional configurations accordingly. All the
///  accessory methods will be called on the main queue.
@protocol MRJ_RequestAccessory <NSObject>
    
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

    
@interface MRJ_BaseRequest : NSObject

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
///  `MRJ_ResponseSerializerType`. Note this value can be nil if request failed.
///
///  @discussion If `resumableDownloadPath` and DownloadTask is using, this value will
///              be the path to which file is successfully saved (NSURL), or nil if request failed.
@property (nonatomic, strong, readonly, nullable) id responseObject;

///  If you use `MRJ_ResponseSerializerTypeJSON`, this is a convenience (and sematic) getter
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
@property (nonatomic, weak, nullable) id<MRJ_RequestDelegate> delegate;

///  The success callback. Note if this value is not nil and `requestFinished` delegate method is
///  also implemented, both will be executed but delegate method is first called. This block
///  will be called on the main queue.
@property (nonatomic, copy, nullable) MRJ_RequestCompletionBlock successCompletionBlock;

///  The failure callback. Note if this value is not nil and `requestFailed` delegate method is
///  also implemented, both will be executed but delegate method is first called. This block
///  will be called on the main queue.
@property (nonatomic, copy, nullable) MRJ_RequestCompletionBlock failureCompletionBlock;

///  This can be used to add several accossories object. Note if you use `addAccessory` to add acceesory
///  this array will be automatically created. Default is nil.
@property (nonatomic, strong, nullable) NSMutableArray<id<MRJ_RequestAccessory>> *requestAccessories;

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

///  The priority of the request. Effective only on iOS 8+. Default is `MRJ_RequestPriorityDefault`.
@property (nonatomic) MRJ_RequestPriority requestPriority;

///  Set completion callbacks
- (void)setCompletionBlockWithSuccess:(nullable MRJ_RequestCompletionBlock)success
                              failure:(nullable MRJ_RequestCompletionBlock)failure;

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
- (void)startWithCompletionBlockWithSuccess:(nullable MRJ_RequestCompletionBlock)success
                                    failure:(nullable MRJ_RequestCompletionBlock)failure;


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
- (MRJ_RequestMethod)requestMethod;

///  Request serializer type.
- (MRJ_RequestSerializerType)requestSerializerType;

///  Response serializer type. See also `responseObject`.
- (MRJ_ResponseSerializerType)responseSerializerType;

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
