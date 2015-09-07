//
//  WADSP.m
//  WrapperAE
//
//  Created by Viatcheslaw Soroka on 5/27/15.
//  Copyright (c) 2015 IDAP Group. All rights reserved.
//

#import "WADSP.h"

@interface WADSP ()
@property (nonatomic, assign)   float       *tInputBuffer;
@property (nonatomic, assign)   float       *tOutputBuffer;
@property (nonatomic, assign)   NSUInteger  tBufferCount;

@end

@implementation WADSP

- (id)initWithSamplingRate:(float)sr {
    if (self = [super initWithSamplingRate:sr]) {

    }
    
    return self;
}

- (void)dealloc {
    self.tInputBuffer = nil;
    self.tOutputBuffer = nil;
}

- (void)setBuffer:(float *)buffer atIvar:(float **)ivar {
    if (*ivar != buffer) {
        if (NULL != *ivar) {
            free(*ivar);
        }
        
        *ivar = buffer;
    }
}

- (void)filterContiguousData: (float *)data numFrames:(UInt32)numFrames channel:(UInt32)channel {
    if (self.tBufferCount != numFrames) {
        self.tInputBuffer = (float*) malloc((numFrames + 2) * sizeof(float));
        self.tOutputBuffer = (float*) malloc((numFrames + 2) * sizeof(float));
        self.tBufferCount = numFrames;
    }
    
    // Provide buffer for processing
    float *tInputBuffer = self.tInputBuffer;
    float *tOutputBuffer = self.tOutputBuffer;
    
    // Copy the data
    memcpy(tInputBuffer, gInputKeepBuffer[channel], 2 * sizeof(float));
    memcpy(tOutputBuffer, gOutputKeepBuffer[channel], 2 * sizeof(float));
    memcpy(&(tInputBuffer[2]), data, numFrames * sizeof(float));
    
    // Do the processing
    vDSP_deq22(tInputBuffer, 1, coefficients, tOutputBuffer, 1, numFrames);
    
    // Copy the data
    memcpy(data, tOutputBuffer, numFrames * sizeof(float));
    memcpy(gInputKeepBuffer[channel], &(tInputBuffer[numFrames]), 2 * sizeof(float));
    memcpy(gOutputKeepBuffer[channel], &(tOutputBuffer[numFrames]), 2 * sizeof(float));
}

@end
