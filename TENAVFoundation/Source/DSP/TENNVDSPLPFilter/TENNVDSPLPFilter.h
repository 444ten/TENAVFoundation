//
//  TENNVDSPLPFilter.h
//  TENAVFoundation
//
//  Created by 444ten on 9/8/15.
//  Copyright (c) 2015 444ten. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <AVFoundation/AVFoundation.h>

@interface TENNVDSPLPFilter : NSObject

- (void)setup;

- (void)processWithAudioBufferList:(AudioBufferList *)audioBufferList framesCount:(UInt32)framesCount;

- (void)update;


@end
