//
//  AEEqualizerFilter.h
//  WrapperAE
//
//  Created by Viatcheslaw Soroka on 5/25/15.
//  Copyright (c) 2015 IDAP Group. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

@interface AEEqualizerFilter : NSObject

- (void)setup;

- (void)update;

- (void)filterAudioBuffer:(AudioBufferList *)buffer framesCount:(UInt32)framesCount;

@end
