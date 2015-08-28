//
//  TENStartViewController.m
//  TENAVFoundation
//
//  Created by 444ten on 8/28/15.
//  Copyright (c) 2015 444ten. All rights reserved.
//

#import "TENStartViewController.h"

@interface TENStartViewController ()

@end

@implementation TENStartViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
}

#pragma mark -
#pragma mark View Handling

- (IBAction)onPlaySource:(id)sender {
    NSLog(@"%@", NSStringFromSelector(_cmd));
}

- (IBAction)onProcess:(id)sender {
    NSLog(@"%@", NSStringFromSelector(_cmd));
}

- (IBAction)onPlayResult:(id)sender {
    NSLog(@"%@", NSStringFromSelector(_cmd));
}

@end
