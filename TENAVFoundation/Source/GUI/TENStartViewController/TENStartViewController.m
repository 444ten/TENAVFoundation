//
//  TENStartViewController.m
//  TENAVFoundation
//
//  Created by 444ten on 8/28/15.
//  Copyright (c) 2015 444ten. All rights reserved.
//

#import "TENStartViewController.h"

#import <AVFoundation/AVFoundation.h>

#import "NSObject+IDPExtensions.h"

#import "AEEqualizerFilter.h"

static NSString * const kTENSourceName      = @"test";
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


@property (nonatomic, strong)   AVAssetWriter       *writer;
@property (nonatomic, strong)   AVAssetWriterInput  *input;

@property (nonatomic, strong)   dispatch_queue_t    mediaInputQueue;

@property (nonatomic, assign)   CMBlockBufferRef    blockBuffer;
@property (nonatomic, assign)   AudioBufferList     *audioBufferList;

@property (nonatomic, strong)   AEEqualizerFilter   *filter;


- (NSString *)processedFileName;

@end

@implementation TENStartViewController

#pragma mark -
#pragma mark View Controller Life Cycle

- (void)viewDidLoad {
    [super viewDidLoad];
    
}

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
                                                     AVLinearPCMIsFloatKey : @(YES),
                                                     AVLinearPCMBitDepthKey : @(32)
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
                                   AVEncoderBitRateKey : @64000
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



- (IBAction)onProcess:(id)sender {
    NSLog(@"%@", NSStringFromSelector(_cmd));

    AEEqualizerFilter *filter = [AEEqualizerFilter object];
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
                
                self.audioBufferList = calloc(1, bufferSize);
                
                OSStatus result = CMSampleBufferGetAudioBufferListWithRetainedBlockBuffer(sampleBuffer,
                                                                                          NULL,
                                                                                          _audioBufferList,
                                                                                          bufferSize,
                                                                                          NULL,
                                                                                          NULL,
                                                                                          flags,
                                                                                          &_blockBuffer);
                NSAssert(0 == result, @"error:_audioBufferList");

                
                
                UInt32 sampleCount = (UInt32)CMSampleBufferGetNumSamples(sampleBuffer);
                
                [self.filter filterAudioBuffer:self.audioBufferList framesCount:sampleCount];
                
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
                }];
                
                break;
            }
        }
    }];
}


#pragma mark -
#pragma mark Private

- (NSString *)processedFileName {
    NSDateFormatter *dateFormatter = [NSDateFormatter new];
    dateFormatter.dateFormat = kTENDateFormat;
    return [NSString stringWithFormat:kTENProcessedFileNameFormat, [dateFormatter stringFromDate:[NSDate date]]];
}

@end
