//
//  TENFlangerFilter.m
//  TENAVFoundation
//
//  Created by 444ten on 9/28/15.
//  Copyright © 2015 444ten. All rights reserved.
//

#import "TENFlangerFilter.h"

#import "ChorusFlangerIf.h"

static CChorusFlangerIf *flangerPtr;
static NSUInteger zfxError;


@implementation TENFlangerFilter
#pragma mark -
#pragma mark Initializations and Deallocations

- (instancetype)init {
    self = [super init];
    if (self) {
        float sampleRate = 44100;
        int numberOfChannel = 2;
        
        CChorusFlangerIf::CreateInstance (flangerPtr, sampleRate, numberOfChannel);
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
    
    flangerPtr->Process(planes, planes, framesCount);
}

- (void)update {
    static float stepPercent = 5.0; // 0.0 .. 100.0

    static float minDepth= 0.0;
    static float maxDepth = 0.1;
    static float stepDepth = (maxDepth - minDepth) / 100.0 * stepPercent;
    
    static float depth = minDepth;

    static float sign = 1.0;

    NSLog(@"zPlane flanger. depth: %f", depth);
    
    zfxError = flangerPtr->SetParam(CChorusFlangerIf::kChFlParamLfoDepthInS, depth);
    NSAssert(kNoError == zfxError, @"%@: %@ error", NSStringFromClass([self class]), NSStringFromSelector(_cmd));
    
    depth += sign * stepDepth;
    if (depth > (maxDepth - stepDepth) || depth < (minDepth + stepDepth)) {
        sign *= -1.0;
    }
}

@end
