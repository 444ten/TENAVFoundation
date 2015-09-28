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
static NSUInteger zfxError;

static const float      kDefaultLow     = 0.0f;

static const float      kLowBandFreq    = 300.0f;
static const float      kLowBandQ       = 0.6f;

static const float      kHighBandFreq    = 1600.0f;
static const float      kHighBandQ       = 0.6f;

static const float      kMaxBandGainDb  = 12.0f;

@implementation TENEqualizerFilter

#pragma mark -
#pragma mark Initializations and Deallocations

- (instancetype)init {
    self = [super init];
    if (self) {
        float sampleRate = 44100;
        int numberOfChanel = 2;
        
        zfxError = CParametricEqIf::CreateInstance(equalizerPtr, sampleRate, numberOfChanel);
        zfxError += equalizerPtr->SetBypass(false);
        
// Low Band
//        zfxError += equalizerPtr->SetType(CParametricEqIf::kLShelv12);
//        zfxError += equalizerPtr->SetParam(CParametricEqIf::kEqParamFrequency, kLowBandFreq);
//        zfxError += equalizerPtr->SetParam(CParametricEqIf::kEqParamQ, kLowBandQ);
        
// High Band
        zfxError += equalizerPtr->SetType(CParametricEqIf::kHShelv12);
        zfxError += equalizerPtr->SetParam(CParametricEqIf::kEqParamFrequency, kHighBandFreq);
        zfxError += equalizerPtr->SetParam(CParametricEqIf::kEqParamQ, kHighBandQ);
        
        zfxError += equalizerPtr->SetAddDenormalNoise(false);
        
        NSAssert(kNoError == zfxError, @"%@: %@ error", NSStringFromClass([self class]), NSStringFromSelector(_cmd));
    }
    
    return self;
}

#pragma mark -
#pragma mark Public

- (void)setup {
    zfxError = equalizerPtr->SetParam(CParametricEqIf::kEqParamGain, kDefaultLow);
    
    NSAssert(kNoError == zfxError, @"%@: %@ error", NSStringFromClass([self class]), NSStringFromSelector(_cmd));
}

- (void)processWithAudioBufferList:(AudioBufferList *)audioBufferList framesCount:(UInt32)framesCount {
    UInt32 channelsCount = audioBufferList->mNumberBuffers;
    
    float *planes[channelsCount];
    
    for (UInt32 channel = 0; channel < channelsCount; channel++) {
        planes[channel] = (float *)audioBufferList->mBuffers[channel].mData;
    }
    
    [self update];
    
    zfxError = equalizerPtr->Process(planes, planes, framesCount);
    NSAssert(kNoError == zfxError, @"%@: %@ error", NSStringFromClass([self class]), NSStringFromSelector(_cmd));
}

- (void)update {
    static float step = 0.05;
    static float coefficient =  0.0;
    static float sign = 1.0;
    
    coefficient += sign * step;
    
    if (coefficient > 0.95 || coefficient < 0.05) {
        sign *= -1.0;
    }

    NSLog(@"zPlane EQ highBand: %f", coefficient);
    
    zfxError = equalizerPtr->SetParam(CParametricEqIf::kEqParamGain, coefficient * kMaxBandGainDb);
    NSAssert(kNoError == zfxError, @"%@: %@ error", NSStringFromClass([self class]), NSStringFromSelector(_cmd));
}

@end
