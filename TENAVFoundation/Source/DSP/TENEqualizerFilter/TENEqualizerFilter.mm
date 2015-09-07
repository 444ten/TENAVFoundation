//
//  TENEqualizerFilter.m
//  TENAVFoundation
//
//  Created by 444ten on 9/7/15.
//  Copyright (c) 2015 444ten. All rights reserved.
//

#import "TENEqualizerFilter.h"

#import "ParamEQIf.h"

static CParametricEqIf *equalizerPtr;

static const float      kDefaultLow     = 0.0f;
static const float      kLowBandFreq    = 300.0f;
static const float      kLowBandQ       = 0.6f;
static const float      kMaxBandGainDb  = 12.0f;

@implementation TENEqualizerFilter

#pragma mark -
#pragma mark Initializations and Deallocations

- (instancetype)init {
    self = [super init];
    if (self) {
        float sampleRate = 44100;
        int numberOfChanel = 2;
        
        CParametricEqIf::CreateInstance(equalizerPtr, sampleRate, numberOfChanel);
        equalizerPtr->SetBypass(false);
        equalizerPtr->SetType(CParametricEqIf::kLShelv12);
        equalizerPtr->SetParam(CParametricEqIf::kEqParamFrequency, kLowBandFreq);
        equalizerPtr->SetParam(CParametricEqIf::kEqParamQ, kLowBandQ);
        equalizerPtr->SetAddDenormalNoise(false);
    }
    
    return self;
}

#pragma mark -
#pragma mark Public

- (void)setup {
    equalizerPtr->SetParam(CParametricEqIf::kEqParamGain, kDefaultLow);
}

- (void)processWithAudioBufferList:(AudioBufferList *)audioBufferList framesCount:(UInt32)framesCount {
    UInt32 channelsCount = audioBufferList->mNumberBuffers;
    
    float *planes[channelsCount];
    
    for (UInt32 channel = 0; channel < channelsCount; channel++) {
        planes[channel] = (float *)audioBufferList->mBuffers[channel].mData;
    }
    
    [self update];
    
    equalizerPtr->Process(planes, planes, framesCount);
}

- (void)update {
    static float step = 0.5;
    static float coefficient =  0.0;
    static float sign = 1.0;
    
    coefficient += sign * step;
    
    if (coefficient > 0.95 || coefficient < 0.05) {
        sign *= -1.0;
    }

    equalizerPtr->SetParam(CParametricEqIf::kEqParamGain, coefficient * kMaxBandGainDb);
}

@end
