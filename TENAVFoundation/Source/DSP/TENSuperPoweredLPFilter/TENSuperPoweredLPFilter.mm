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
    static float step = 3800.0;
    static float coefficient = 200;
    static float sign = 1.0;
    
    static float resonance = 3.0;
    
    coefficient += sign * step;
    
    if (coefficient >= 4000 || coefficient <= 200) {
        sign *= -1.0;
        
        if (resonance == 4.0) {
            resonance = 1.0;
        } else {
            resonance = 4.0;
        }
    }
    
        NSLog(@"superPowered lp freq:%f, resonance:%f", coefficient, resonance);
    
    lpFilterPtr->setResonantParametersAndType(coefficient, resonance, SuperpoweredFilter_Resonant_Lowpass);
}

@end
