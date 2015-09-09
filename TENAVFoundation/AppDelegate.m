//
//  AppDelegate.m
//  TENAVFoundation
//
//  Created by 444ten on 8/28/15.
//  Copyright (c) 2015 444ten. All rights reserved.
//

#import "AppDelegate.h"

#import "TENStartViewController.h"

@interface AppDelegate ()

@end

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    UIWindow *window = [[UIWindow alloc] initWithFrame:[UIScreen mainScreen].bounds];

    TENStartViewController *controller = [TENStartViewController new];
    window.rootViewController = controller;
    
    [window makeKeyAndVisible];

    self.window = window;
    
    return YES;
}

@end
