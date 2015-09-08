//
//  TENNVDSPLPFilter.m
//  TENAVFoundation
//
//  Created by 444ten on 9/8/15.
//  Copyright (c) 2015 444ten. All rights reserved.
//

#import "TENNVDSPLPFilter.h"

#import "NVLowpassFilter.h"

static const float kDJSamplingRate  = 44100.0f;

static NVLowpassFilter *lpFilter;

@implementation TENNVDSPLPFilter

#pragma mark -
#pragma mark Initializations and Deallocations

- (instancetype)init {
    self = [super init];
    if (self) {
        lpFilter = [[NVLowpassFilter alloc] initWithSamplingRate:kDJSamplingRate];
    }
    
    return self;
}

#pragma mark -
#pragma mark Public Methods

- (void)setup {
    lpFilter.cornerFrequency = 2000.0;
    lpFilter.Q = 0.8;
}

- (void)processWithAudioBufferList:(AudioBufferList *)audioBufferList framesCount:(UInt32)framesCount {
    if (audioBufferList) {
        for (NSUInteger bufferIndex = 0; bufferIndex < audioBufferList->mNumberBuffers; bufferIndex++) {

            [self update];

            AudioBuffer audioBuffer = audioBufferList->mBuffers[bufferIndex];
                [lpFilter filterData:(float *)audioBuffer.mData
                           numFrames:framesCount
                         numChannels:audioBuffer.mNumberChannels];
        }
    }
}

- (void)update {
    
}

@end
