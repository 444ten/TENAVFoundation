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
    static float step = 8000.0;
    static float coefficient = 200;
    static float sign = 1.0;
    
    coefficient += sign * step;
    
    if (coefficient >= 8200 || coefficient <= 200) {
        sign *= -1.0;
    }
    
    NSLog(@"zPlane lp frequency %f", coefficient);
    
    lpPtr->SetParam(CResonanceFilterIf::kRFilterParamFrequencyInHz, coefficient);
}

@end
