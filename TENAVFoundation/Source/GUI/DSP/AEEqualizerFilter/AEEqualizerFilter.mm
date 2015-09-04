//
//  AEEqualizerFilter.m
//  WrapperAE
//
//  Created by Viatcheslaw Soroka on 5/25/15.
//  Copyright (c) 2015 IDAP Group. All rights reserved.
//

#import "AEEqualizerFilter.h"

#import "NVPeakingEQFilter.h"
#import "NVLowShelvingFilter.h"
#import "NVHighShelvingFilter.h"

static const float kDJSamplingRate  = 44100.0f;
static const float kLowBandFreq     = 300.0f;
static const float kMidBandFreq     = 1000.0f;
static const float kHighBandFreq    = 1600.0f;
static const float kLowBandQ        = 0.6f;
static const float kMidBandQ        = 0.2f;
static const float kHighBandQ       = 0.6f;
static const float kMaxBandGainDb   = 12.0f;

static const NSUInteger kLowFilterIndex     = 0;
static const NSUInteger kMidFilterIndex     = 1;
static const NSUInteger kHighFilterIndex    = 2;

static const NSUInteger kBandCount          = 3;

static NVDSP *filters[kBandCount];

@implementation AEEqualizerFilter

#pragma mark -
#pragma mark Initialisations and Deallocations

- (instancetype)init {
    if (self = [super init]) {
        NVLowShelvingFilter *lowFilter = [[NVLowShelvingFilter alloc] initWithSamplingRate:kDJSamplingRate];
        lowFilter.Q = kLowBandQ;
        lowFilter.G = 0.0f;
        lowFilter.centerFrequency = kLowBandFreq;
        filters[kLowFilterIndex] = lowFilter;
        
        NVPeakingEQFilter *midFilter = [[NVPeakingEQFilter alloc] initWithSamplingRate:kDJSamplingRate];
        midFilter.Q = kMidBandQ;
        midFilter.G = 0.0f;
        midFilter.centerFrequency = kMidBandFreq;
        filters[kMidFilterIndex] = midFilter;
        
        NVHighShelvingFilter *highFilter = [[NVHighShelvingFilter alloc] initWithSamplingRate:kDJSamplingRate];
        highFilter.Q = kHighBandQ;
        highFilter.G = 0.0f;
        highFilter.centerFrequency = kHighBandFreq;
        filters[kHighFilterIndex] = highFilter;
    }
    
    return self;
}

#pragma mark -
#pragma mark Public Methods

- (void)setup {
    ((NVLowShelvingFilter *)filters[kLowFilterIndex]).G = 0.0 * kMaxBandGainDb;
    ((NVPeakingEQFilter *)filters[kMidFilterIndex]).G = 0.0 * kMaxBandGainDb;
    ((NVHighShelvingFilter *)filters[kHighFilterIndex]).G = 0.0 * kMaxBandGainDb;
}

- (void)update {
    static float step = 1.0;
    static float coefficient = - 1.0;
    static float sign = 1.0;
    
    ((NVLowShelvingFilter *)filters[kLowFilterIndex]).G = coefficient * kMaxBandGainDb;
    
    coefficient += sign * step;
    
    if (coefficient > 1.0 || coefficient < -1.0) {
        sign *= -1.0;
    }
}

- (void)filterAudioBuffer:(AudioBufferList *)buffer framesCount:(UInt32)framesCount {
    if (buffer) {
        for (NSUInteger bufferIndex = 0; bufferIndex < buffer->mNumberBuffers; bufferIndex++) {
            [self update];
            AudioBuffer audioBuffer = buffer->mBuffers[bufferIndex];
            for (NSUInteger index = 0; index < kBandCount; index++) {
                NVDSP *filter = filters[index];
                [filter filterData:(float *)audioBuffer.mData
                         numFrames:framesCount
                       numChannels:audioBuffer.mNumberChannels];
            }
        }
    }
}

@end
