//
//  TENSuperPoweredEQFilter.m
//  TENAVFoundation
//
//  Created by 444ten on 9/10/15.
//  Copyright (c) 2015 444ten. All rights reserved.
//

#import "TENSuperPoweredEQFilter.h"

#import "Superpowered3BandEQ.h"

static Superpowered3BandEQ *eqFilterPtr;

@implementation TENSuperPoweredEQFilter

#pragma mark -
#pragma mark Initializations and Deallocations

- (instancetype)init {
    self = [super init];
    if (self) {
        eqFilterPtr = new Superpowered3BandEQ(44100);
    }
    
    return self;
}

#pragma mark -
#pragma mark Public

- (void)setup {
    eqFilterPtr->enable(YES);
}

- (void)processWithAudioBufferList:(AudioBufferList *)audioBufferList framesCount:(UInt32)framesCount {
    NSAssert(1 == audioBufferList->mNumberBuffers,
             @"%@: %@ error", NSStringFromClass([self class]), NSStringFromSelector(_cmd));

    [self update];
    
    float *buffer = (float *)audioBufferList->mBuffers[0].mData;
    eqFilterPtr->process(buffer, buffer, framesCount);
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
    
    eqFilterPtr->bands[0] = gain;
}

@end
