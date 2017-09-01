//
//  MRJSubRequest.h
//  YHJNetwork
//
//  Created by Mr on 2017/9/1.
//  Copyright © 2017年 mrjlovetian@gmail.com. All rights reserved.
//

#import "MRJ_Request.h"

@interface MRJSubRequest : MRJ_Request
@property (nonatomic, copy)NSString *defaultUrl;
@property (nonatomic, assign)MRJ_RequestMethod method;
@property (nonatomic, assign)BOOL isDotNet;
@end
