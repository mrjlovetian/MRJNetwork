//
//  ViewController.m
//  YHJNetwork
//
//  Created by Mr on 2017/8/21.
//  Copyright © 2017年 mrjlovetian@gmail.com. All rights reserved.
//

#import "RootViewController.h"
#import "ViewController.h"
#import "MRJSubRequest.h"
#import <MRJ_Network/MRJ_NetworkConfig.h>
#import "NetArgumentFilter.h"

@implementation RootViewController
- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.view.backgroundColor = [UIColor whiteColor];
    
    CGSize screenSize = [UIScreen mainScreen].bounds.size;
    [self setTabBarFrame:CGRectMake(0, 20, screenSize.width, 44)
        contentViewFrame:CGRectMake(0, 64, screenSize.width, screenSize.height - 64 - 50)];
    
    self.tabBar.itemTitleColor = [UIColor lightGrayColor];
    self.tabBar.itemTitleSelectedColor = [UIColor redColor];
    self.tabBar.itemTitleFont = [UIFont systemFontOfSize:17];
    self.tabBar.itemTitleSelectedFont = [UIFont systemFontOfSize:22];
    self.tabBar.leftAndRightSpacing = 20;
    
    self.tabBar.itemFontChangeFollowContentScroll = YES;
    self.tabBar.itemSelectedBgScrollFollowContent = YES;
    self.tabBar.itemSelectedBgColor = [UIColor redColor];
    
    [self.tabBar setItemSelectedBgInsets:UIEdgeInsetsMake(40, 15, 0, 15) tapSwitchAnimated:NO];
    [self.tabBar setScrollEnabledAndItemFitTextWidthWithSpacing:40];
    
    [self setContentScrollEnabledAndTapSwitchAnimated:NO];
    self.loadViewOfChildContollerWhileAppear = YES;

    
    [self initViewControllers];
    
    
    UIButton *btn = [UIButton buttonWithType:UIButtonTypeCustom];
    btn.frame = CGRectMake(100, 100, 100, 100);
    btn.backgroundColor = [UIColor whiteColor];
    [btn addTarget:self action:@selector(click) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:btn];
}

#pragma mark click
- (void)click
{
    MRJ_NetworkConfig *config = [MRJ_NetworkConfig sharedConfig];
    NetArgumentFilter *fileter = [NetArgumentFilter filterWithArguments:nil];
    [config addUrlFilter:fileter];
    
    MRJSubRequest *request = [[MRJSubRequest alloc] init];
    request.method = MRJ_RequestMethodPOST;
    request.defaultUrl = @"http://client.vgabc.com/clientapi";
    request.requestAccessories = [NSMutableArray arrayWithObjects:self, nil];
    request.requestArgument = @{@"action":@"gamelist",
                                @"regionId":@"gamelist",
                                @"clientparams":@"beta%7C10.1.1%7Czh%7CiPad5%2C3%7C768%2A1024%7Cios1.1%7Chandleshenzhen",
                                @"model":@"appstore",
                                @"orderby":@"hot",
                                @"page":@1,
                                @"pagesize":@10
                                };
    [request startWithCompletionBlockWithSuccess:^(__kindof MRJ_BaseRequest * _Nonnull request) {
        
        NSLog(@"接受到的消息%@", request);
        
        
    } failure:^(__kindof MRJ_BaseRequest * _Nonnull request) {
        
    }];
}

- (void)initViewControllers
{
    ViewController *controller1 = [[ViewController alloc] init];
    controller1.yhj_tabItemTitle = @"推荐";
    
    ViewController *controller2 = [[ViewController alloc] init];
    controller2.yhj_tabItemTitle = @"化妆品";
    
    ViewController *controller3 = [[ViewController alloc] init];
    controller3.yhj_tabItemTitle = @"海外淘";
    
    ViewController *controller4 = [[ViewController alloc] init];
    controller4.yhj_tabItemTitle = @"第四";
    
    ViewController *controller5 = [[ViewController alloc] init];
    controller5.yhj_tabItemTitle = @"电子产品";
    
    ViewController *controller6 = [[ViewController alloc] init];
    controller6.yhj_tabItemTitle = @"第六";
    
    ViewController *controller7 = [[ViewController alloc] init];
    controller7.yhj_tabItemTitle = @"第七个";
    
    self.viewControllers = [NSMutableArray arrayWithObjects:controller1, controller2, controller3, controller4, controller5, controller6, controller7, nil];
}
@end
