//
//  TENStartViewController.m
//  TENAVFoundation
//
//  Created by 444ten on 8/28/15.
//  Copyright (c) 2015 444ten. All rights reserved.
//

#import "TENStartViewController.h"

#import <AVFoundation/AVFoundation.h>

#import "TENEqualizerFilter.h"
#import "TENDelayFilter.h"
#import "TENLPFilter.h"

#import "TENSuperPoweredEQFilter.h"
#import "TENSuperPoweredEchoFilter.h"
#import "TENSuperPoweredLPFilter.h"

//static const BOOL TENNonInterleaved = YES; 
static const BOOL TENNonInterleaved = NO;

//static NSString * const kTENSourceName      = @"didy";
//static NSString * const kTENSourceName      = @"test";
//static NSString * const kTENSourceName      = @"lady";
static NSString * const kTENSourceName      = @"klss";

static NSString * const kTENSourceExtension = @"mp3";

static NSString * const kTENProcessedFileNameFormat = @"%@_processed.m4a";
static NSString * const kTENDateFormat              = @"yyyy-MM-dd HH:mm:ss";

@interface TENStartViewController ()
@property (nonatomic, strong)   AVPlayer                    *sourcePlayer;
@property (nonatomic, strong)   AVPlayer                    *resultPlayer;

@property (nonatomic, strong)   NSURL                       *sourceUrl;
@property (nonatomic, strong)   NSURL                       *outputUrl;

@property (nonatomic, strong)   AVURLAsset                  *asset;

@property (nonatomic, strong)   AVAssetReader               *reader;
@property (nonatomic, strong)   AVAssetReaderTrackOutput    *output;

@property (nonatomic, strong)   AVAssetWriter               *writer;
@property (nonatomic, strong)   AVAssetWriterInput          *input;

@property (nonatomic, strong)   dispatch_queue_t            mediaInputQueue;

@property (nonatomic, assign)   CMBlockBufferRef            blockBuffer;
@property (nonatomic, assign)   AudioBufferList             *audioBufferList;

@property (nonatomic, strong)   id                          filter;

- (NSString *)processedFileName;
- (void)playResult;

@end

@implementation TENStartViewController

#pragma mark -
#pragma mark Accessors

- (NSURL *)sourceUrl {
    if (!_sourceUrl) {
        _sourceUrl = [[NSBundle mainBundle] URLForResource:kTENSourceName withExtension:kTENSourceExtension];
    }
    
    return _sourceUrl;
}

- (NSURL *)outputUrl {
    if (!_outputUrl) {
        
        NSString *documentsPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES)
                                   firstObject];
        
        _outputUrl = [NSURL fileURLWithPath:[documentsPath stringByAppendingPathComponent:[self processedFileName]]];
    }
    
    return _outputUrl;
}

- (AVURLAsset *)asset {
    if (!_asset) {
        _asset = [[AVURLAsset alloc] initWithURL:self.sourceUrl options:nil];
    }
    
    return _asset;
}

- (AVAssetReader *)reader {
    if (!_reader) {
        NSError *error = nil;
        _reader = [[AVAssetReader alloc] initWithAsset:self.asset error:&error];
        NSAssert(!error, @"error: readerInit");
    }
    
    return _reader;
}

- (AVAssetReaderTrackOutput *)output {
    if (!_output) {
        AVAssetTrack *track = [[self.asset tracksWithMediaType:AVMediaTypeAudio] objectAtIndex:0];
        NSDictionary *decompressingAudioSetting = @{ AVFormatIDKey : @(kAudioFormatLinearPCM),
                                                     AVNumberOfChannelsKey : @(2),
                                                     AVLinearPCMIsFloatKey : @(YES),
                                                     AVLinearPCMBitDepthKey : @(32),
                                                     AVLinearPCMIsNonInterleaved : @(TENNonInterleaved)
                                                     };
        
        _output = [AVAssetReaderTrackOutput assetReaderTrackOutputWithTrack:track
                                                             outputSettings:decompressingAudioSetting];
        _output.alwaysCopiesSampleData = NO;

        AVAssetReader *reader = self.reader;
        
        BOOL success = [reader canAddOutput:_output];
        NSAssert(success, @"error: output");

        [reader addOutput:_output];
    }
    
    return _output;
}

- (AVAssetWriter *)writer {
    if (!_writer) {
        NSError *error = nil;
        _writer = [[AVAssetWriter alloc] initWithURL:self.outputUrl fileType:AVFileTypeQuickTimeMovie error:&error];
        NSAssert(!error, @"error: writerInit");
    }
    
    return _writer;
}

- (AVAssetWriterInput *)input {
    if (!_input) {
        AudioChannelLayout stereoChannelLayout = {
            .mChannelLayoutTag = kAudioChannelLayoutTag_Stereo,
            .mChannelBitmap = 0,
            .mNumberChannelDescriptions = 0
        };
        
        NSData *channelLayoutAsData = [NSData dataWithBytes:&stereoChannelLayout
                                                     length:offsetof(AudioChannelLayout, mChannelDescriptions)];
        
        NSDictionary *settings = @{AVFormatIDKey : @(kAudioFormatMPEG4AAC),
                                   AVSampleRateKey : @44100,
                                   AVNumberOfChannelsKey : @2,
                                   AVChannelLayoutKey : channelLayoutAsData,
                                   AVEncoderBitRateKey : @128000
                                   };

        _input = [[AVAssetWriterInput alloc] initWithMediaType:AVMediaTypeAudio outputSettings:settings];
        
        AVAssetWriter *writer = self.writer;
        
        BOOL success = [writer canAddInput:_input];
        NSAssert(success, @"error: input");
        
        [writer addInput:_input];
    }
    
    return _input;
}

- (dispatch_queue_t)mediaInputQueue {
    if (!_mediaInputQueue) {
        _mediaInputQueue = dispatch_queue_create("mediaInputQueue", DISPATCH_QUEUE_SERIAL);
    }
    
    return _mediaInputQueue;
}

#pragma mark -
#pragma mark View Handling

- (IBAction)onPlaySource:(id)sender {
    static BOOL isPlay = NO;
    
    AVPlayer *sourcePlayer = nil;
    
    if (!isPlay) {
        NSURL *sourceUrl = self.sourceUrl;
        
        NSLog(@"playSource %@", sourceUrl);
        
        sourcePlayer = [AVPlayer playerWithURL:sourceUrl];
        self.sourcePlayer = sourcePlayer;
        
        [sourcePlayer play];
        
        isPlay = YES;
    } else {
        sourcePlayer = self.sourcePlayer;
        
        sourcePlayer.rate = (sourcePlayer.rate > 0.0) ? 0.0 : 1.0;
    }
}

- (IBAction)onPlayResult:(id)sender {
    static BOOL isPlay = NO;
    
    AVPlayer *resultPlayer = nil;
    
    if (!isPlay) {
        NSURL *outputUrl = self.outputUrl;
        
        NSLog(@"playResult %@", outputUrl);
        
        resultPlayer = [AVPlayer playerWithURL:outputUrl];
        self.resultPlayer = resultPlayer;
        
        [resultPlayer play];
        
        isPlay = YES;
    } else {
        resultPlayer = self.resultPlayer;
        
        resultPlayer.rate = (resultPlayer.rate > 0.0) ? 0.0 : 1.0;
    }
}

- (IBAction)onSourceVolumeSliderValueChanged:(UISlider *)sender {
    self.sourcePlayer.volume = sender.value;
    self.resultPlayer.volume = sender.value;
}

- (IBAction)onProcess:(id)sender {
    NSLog(@"%@", NSStringFromSelector(_cmd));

//  set TENNonInterleaved = YES
    
//    TENEqualizerFilter *filter = [TENEqualizerFilter new];
//    TENDelayFilter *filter = [TENDelayFilter new];
//    TENLPFilter *filter = [TENLPFilter new];
    
//  set TENNonInterleaved = NO
//    TENSuperPoweredEQFilter *filter = [TENSuperPoweredEQFilter new];
//    TENSuperPoweredEchoFilter *filter = [TENSuperPoweredEchoFilter new];
    TENSuperPoweredLPFilter *filter = [TENSuperPoweredLPFilter new];
    
    [filter setup];
    self.filter = filter;
    
    BOOL success = NO;
    
    AVAssetReader *reader = self.reader;
    AVAssetReaderTrackOutput *output = self.output;
    
    success = [reader startReading];
    NSAssert(success, @"error: startReading");
    
    AVAssetWriter *writer = self.writer;
    AVAssetWriterInput *input = self.input;

    success = [writer startWriting];
    NSAssert(success, @"error: startWriting");

    [writer startSessionAtSourceTime:kCMTimeZero];

    [input requestMediaDataWhenReadyOnQueue:self.mediaInputQueue usingBlock:^{
        while (input.readyForMoreMediaData) {
            CMSampleBufferRef sampleBuffer = [output copyNextSampleBuffer];
            
            if (sampleBuffer) {
                uint32_t flags = kCMSampleBufferFlag_AudioBufferList_Assure16ByteAlignment;

                size_t bufferSize = sizeof(AudioBufferList);
                bufferSize += TENNonInterleaved ? sizeof(AudioBuffer) : 0;
                
                self.audioBufferList = (AudioBufferList *)calloc(1, bufferSize);
                
                OSStatus result = CMSampleBufferGetAudioBufferListWithRetainedBlockBuffer(sampleBuffer,
                                                                                          NULL,
                                                                                          _audioBufferList,
                                                                                          bufferSize,
                                                                                          NULL,
                                                                                          NULL,
                                                                                          flags,
                                                                                          &_blockBuffer);
                NSAssert(0 == result, @"error:_audioBufferList");

                UInt32 framesCount = (UInt32)CMSampleBufferGetNumSamples(sampleBuffer);

                [filter processWithAudioBufferList:_audioBufferList framesCount:framesCount];
                
                CMSampleBufferSetDataBufferFromAudioBufferList(sampleBuffer,
                                                               kCFAllocatorDefault,
                                                               kCFAllocatorDefault,
                                                               flags,
                                                               _audioBufferList);
                
                BOOL success = [input appendSampleBuffer:sampleBuffer];

                CFRelease(sampleBuffer);
                sampleBuffer = nil;
                
                if (!success && writer.status == AVAssetWriterStatusFailed) {
                    NSLog(@"writer error %@", writer.error);
                    abort();
                }
            } else {
                if (reader.status == AVAssetReaderStatusFailed) {
                    NSLog(@"reader error %@", reader.error);
                    abort();
                }
                
                [input markAsFinished];
                [writer finishWritingWithCompletionHandler:^{
                    NSLog(@"finish");
                    [self performSelectorOnMainThread:@selector(playResult) withObject:nil waitUntilDone:NO];
                }];
                
                break;
            }
        }
    }];
}

#pragma mark -
#pragma mark Private

- (void)playResult {
    self.processButton.enabled = NO;
    self.playResultButton.enabled = YES;
    [self onPlayResult:nil];
}

- (NSString *)processedFileName {
    NSDateFormatter *dateFormatter = [NSDateFormatter new];
    dateFormatter.dateFormat = kTENDateFormat;
    
    return [NSString stringWithFormat:kTENProcessedFileNameFormat, [dateFormatter stringFromDate:[NSDate date]]];
}

@end
