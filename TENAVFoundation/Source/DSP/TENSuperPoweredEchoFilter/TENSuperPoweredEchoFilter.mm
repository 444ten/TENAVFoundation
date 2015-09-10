//
//  TENSuperPoweredEchoFilter.m
//  TENAVFoundation
//
//  Created by 444ten on 9/10/15.
//  Copyright (c) 2015 444ten. All rights reserved.
//

#import "TENSuperPoweredEchoFilter.h"

#import "SuperpoweredEcho.h"

static SuperpoweredEcho *echoFilterPtr;

@implementation TENSuperPoweredEchoFilter

#pragma mark -
#pragma mark Initializations and Deallocations

- (instancetype)init {
    self = [super init];
    if (self) {
        echoFilterPtr = new SuperpoweredEcho(44100);
    }
    
    return self;
}

#pragma mark -
#pragma mark Public

- (void)setup {
    echoFilterPtr->enable(YES);

    echoFilterPtr->setMix(1.0);
    
    echoFilterPtr->bpm = 256.0;
    echoFilterPtr->beats = 0.125;
    echoFilterPtr->decay = 1.0;
    
}

- (void)processWithAudioBufferList:(AudioBufferList *)audioBufferList framesCount:(UInt32)framesCount {
    NSAssert(1 == audioBufferList->mNumberBuffers,
             @"%@: %@ error", NSStringFromClass([self class]), NSStringFromSelector(_cmd));
    
//    [self update];
    
    float *buffer = (float *)audioBufferList->mBuffers[0].mData;
    
    NSLog(@"echo %f", *buffer);
    
    echoFilterPtr->process(buffer, buffer, framesCount);
    
    NSLog(@"echo ** %f", *buffer);

}

- (void)update {
    static float step = 1.0;
    static float gain = 1.0;
    
    static float sign = 1.0;
    
    gain += sign * step;
    
    if (gain >= 3.0 || gain < step) {
        sign *= -1.0;
    }
    
    NSLog(@"%f", gain);
    
//    echoFilterPtr->bands[0] = gain;
}

@end