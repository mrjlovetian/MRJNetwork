//
//  MRJSubRequest.h
//  YHJNetwork
//
//  Created by Mr on 2017/9/1.
//  Copyright © 2017年 mrjlovetian@gmail.com. All rights reserved.
//

#import "MRJRequest.h"

@interface MRJSubRequest : MRJRequest
@property (nonatomic, copy)NSString *defaultUrl;
@property (nonatomic, assign)MRJRequestMethod method;
@property (nonatomic, assign)BOOL isDotNet;
@end
