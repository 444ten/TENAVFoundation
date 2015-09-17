//
//  TENLPFilter.m
//  TENAVFoundation
//
//  Created by 444ten on 9/7/15.
//  Copyright (c) 2015 444ten. All rights reserved.
//

#import "TENLPFilter.h"

#import "ResonanceFilterIf.h"

static CResonanceFilterIf *lpPtr;
static NSUInteger zfxError;

@implementation TENLPFilter

#pragma mark -
#pragma mark Initializations and Deallocations

- (instancetype)init {
    self = [super init];
    if (self) {
        float sampleRate = 44100;
        int numberOfChannel = 2;
        
        CResonanceFilterIf::CreateInstance (lpPtr, sampleRate, numberOfChannel, CResonanceFilterIf::kRFilterLP);
    }
    
    return self;
}

#pragma mark -
#pragma mark Public

- (void)setup {

}

- (void)processWithAudioBufferList:(AudioBufferList *)audioBufferList framesCount:(UInt32)framesCount {
    UInt32 channelsCount = audioBufferList->mNumberBuffers;
    
    float *planes[channelsCount];
    
    for (UInt32 channel = 0; channel < channelsCount; channel++) {
        planes[channel] = (float *)audioBufferList->mBuffers[channel].mData;
    }
    
    [self update];
    
    lpPtr->Process(planes, planes, framesCount);
}

- (void)update {
    static float step = 150.0;
    static float coefficient = 1000;
    static float sign = 1.0;
    
    static float resonance = 3.0;
    
    coefficient += sign * step;
    
    resonance = coefficient / 1000;
    
    if (coefficient >= 4000 || coefficient <= 1000) {
        sign *= -1.0;
    }

    NSLog(@"zPlane lp frequency: %f, resonance: %f", coefficient, resonance);
    
    zfxError = lpPtr->SetParam(CResonanceFilterIf::kRFilterParamFrequencyInHz, coefficient);
    zfxError += lpPtr->SetParam(CResonanceFilterIf::kRFilterParamResonance, resonance);
    NSAssert(kNoError == zfxError, @"%@: %@ error", NSStringFromClass([self class]), NSStringFromSelector(_cmd));
}

@end
