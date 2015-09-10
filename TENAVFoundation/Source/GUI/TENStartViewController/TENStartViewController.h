//
//  TENStartViewController.h
//  TENAVFoundation
//
//  Created by 444ten on 8/28/15.
//  Copyright (c) 2015 444ten. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface TENStartViewController : UIViewController
@property (nonatomic, strong)   IBOutlet UIButton   *processButton;
@property (nonatomic, strong)   IBOutlet UIButton   *playResultButton;

- (IBAction)onPlaySource:(id)sender;
- (IBAction)onProcess:(id)sender;
- (IBAction)onPlayResult:(id)sender;

- (IBAction)onSourceVolumeSliderValueChanged:(UISlider *)sender;

@end
