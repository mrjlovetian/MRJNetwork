//
//  MRJ_BetworkAgent.h
//  TopsTechNetWorking
//
//  Created by MRJ_ on 2017/2/17.
//  Copyright © 2017年 MRJ_. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "MRJ_BaseRequest.h"

NS_ASSUME_NONNULL_BEGIN

@interface MRJ_NetworkAgent : NSObject

-(instancetype)init;

+(MRJ_NetworkAgent *)sharedAgent;

-(void)addRequest:(MRJ_BaseRequest *)request;

-(void)cancelRequest:(MRJ_BaseRequest *)request;

-(void)cancelAllRequests;

///  Return the constructed URL of request.
///
///  @param request The request to parse. Should not be nil.
///
///  @return The result URL.
- (NSString *)buildRequestUrl:(MRJ_BaseRequest *)request;

@end

NS_ASSUME_NONNULL_END
