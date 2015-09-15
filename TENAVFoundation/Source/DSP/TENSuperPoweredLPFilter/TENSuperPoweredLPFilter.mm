//
//  TENSuperPoweredLPFilter.m
//  TENAVFoundation
//
//  Created by 444ten on 9/10/15.
//  Copyright (c) 2015 444ten. All rights reserved.
//

#import "TENSuperPoweredLPFilter.h"


#import "SuperpoweredFilter.h"

static SuperpoweredFilter *lpFilterPtr;

@implementation TENSuperPoweredLPFilter

#pragma mark -
#pragma mark Initializations and Deallocations

- (instancetype)init {
    self = [super init];
    if (self) {
        lpFilterPtr = new SuperpoweredFilter(SuperpoweredFilter_Resonant_Lowpass, 44100);
    }
    
    return self;
}

#pragma mark -
#pragma mark Public

- (void)setup {
    lpFilterPtr->enable(YES);
}

- (void)processWithAudioBufferList:(AudioBufferList *)audioBufferList framesCount:(UInt32)framesCount {
    NSAssert(1 == audioBufferList->mNumberBuffers,
             @"%@: %@ error", NSStringFromClass([self class]), NSStringFromSelector(_cmd));
    
    [self update];
    
    float *buffer = (float *)audioBufferList->mBuffers[0].mData;
    lpFilterPtr->process(buffer, buffer, framesCount);
}

- (void)update {
    static float step = 500;
    static float coefficient = 1000;
    static float sign = 1.0;
    
    coefficient += sign * step;
    
    if (coefficient >= 4000 || coefficient <= 1000) {
        sign *= -1.0;
    }
    
        NSLog(@"superPowered lp frequency%f", coefficient);
    
    lpFilterPtr->setResonantParametersAndType(coefficient, 0.0, SuperpoweredFilter_Resonant_Lowpass);
}

@end
