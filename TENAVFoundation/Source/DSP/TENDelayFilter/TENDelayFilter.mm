//
//  TENDelayFilter.m
//  TENAVFoundation
//
//  Created by 444ten on 9/7/15.
//  Copyright (c) 2015 444ten. All rights reserved.
//

#import "TENDelayFilter.h"

#import "DelayIf.h"

static CDelayIf *delayPtr;

@implementation TENDelayFilter

#pragma mark -
#pragma mark Initializations and Deallocations

- (instancetype)init {
    self = [super init];
    if (self) {
        float sampleRate = 44100;
        int numberOfChanel = 2;
        
        CDelayIf::CreateInstance(delayPtr, sampleRate, numberOfChanel);

//        delayPtr->SetBypass(true);
//        delayPtr->SetBypass(false);
//        
//        delayPtr->SetLfoType(kNoLfo);
//        
//        delayPtr->SetParam(CDelayIf::kDelParamLfoDepthRel, 0.0);
//        delayPtr->SetParam(CDelayIf::kDelParamLfoFreqInHz,0.1);
//        delayPtr->SetParam(CDelayIf::kDelParamFeedbackRel, 0.0);
//        delayPtr->SetParam(CDelayIf::kDelParamBlendRel, 0.707);
//        delayPtr->SetParam(CDelayIf::kDelParamFeedForwardRel, 0.707);
//        delayPtr->SetParam(CDelayIf::kDelParamLpInFeedbackRel, 0.2);
//        delayPtr->SetParam(CDelayIf::kDelParamStereoFadeRel, 0.3);
//        delayPtr->SetParam(CDelayIf::kDelParamWetnessRel, 1.0);

//        kDelParamLfoDepthRel,   //!< modulation range in relation to current delay, (0...1) leads to (0s...2*delaytime)
//        kDelParamLfoFreqInHz,   //!< modulation frequency
//        kDelParamFeedbackRel,   //!< amount of feedback (0...0.9999)
//        kDelParamBlendRel,      //!< amount of dry (i.e. input + feedback) path (0...1)
//        kDelParamFeedForwardRel,//!< amount of wet path (0...1)
//        kDelParamLpInFeedbackRel, //!< low pass amount in feedback path (0...0.9999)
//        kDelParamStereoFadeRel, //!< amount of cross feedback between channels (0...1)
//        kDelParamWetnessRel,    //!< amount of wetness (0...1), (this is closely related to blend)

//        kDelParamDelayInS,      //!< mean delay (0...fMaxDelay in CreateInstance)
    }
    
    return self;
}


#pragma mark -
#pragma mark Public

- (void)setup {
    delayPtr->SetParam(CDelayIf::kDelParamDelayInS, 0.0);
}

- (void)processWithAudioBufferList:(AudioBufferList *)audioBufferList framesCount:(UInt32)framesCount {
    UInt32 channelsCount = audioBufferList->mNumberBuffers;
    
    float *planes[channelsCount];
    
    for (UInt32 channel = 0; channel < channelsCount; channel++) {
        planes[channel] = (float *)audioBufferList->mBuffers[channel].mData;
    }
    
    [self update];
    zfxError_t succes = delayPtr->Process(planes, planes, framesCount);

}

- (void)update {
    static float step = 0.01;
    static float coefficient =  0.0;
    static float sign = 1.0;
    
    coefficient += sign * step;
    
    if (coefficient > 0.999 || coefficient < 0.001) {
        sign *= -1.0;
    }
    
    delayPtr->SetParam(CDelayIf::kDelParamDelayInS, coefficient);
}

@end
