#ifdef __OBJC__
#import <UIKit/UIKit.h>
#else
#ifndef FOUNDATION_EXPORT
#if defined(__cplusplus)
#define FOUNDATION_EXPORT extern "C"
#else
#define FOUNDATION_EXPORT extern
#endif
#endif
#endif

#import "YHJBaseRequest.h"
#import "YHJBatchRequest.h"
#import "YHJBatchRequestAgent.h"
#import "YHJChainRequest.h"
#import "YHJChainRequestAgent.h"
#import "YHJNetworkAgent.h"
#import "YHJNetworkConfig.h"
#import "YHJNetworkPrivate.h"
#import "YHJRequest.h"

FOUNDATION_EXPORT double YHJNetworkVersionNumber;
FOUNDATION_EXPORT const unsigned char YHJNetworkVersionString[];

