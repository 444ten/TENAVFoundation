//
//  TENStartViewController.m
//  TENAVFoundation
//
//  Created by 444ten on 8/28/15.
//  Copyright (c) 2015 444ten. All rights reserved.
//

#import "TENStartViewController.h"

#import <AVFoundation/AVFoundation.h>

static NSString * const kTENSourceName      = @"test";
static NSString * const kTENSourceExtension = @"mp3";

static NSString * const kTENProcessedFileNameFormat = @"%@_processed.m4a";
static NSString * const kTENDateFormat              = @"yyyy-MM-dd HH:mm:ss";


@interface TENStartViewController ()
@property (nonatomic, strong) NSURL                 *sourceUrl;
@property (nonatomic, strong) AVURLAsset            *sourceAsset;
@property (nonatomic, strong) AVPlayerItem          *playerItem;
@property (nonatomic, strong) AVPlayer              *player;
@property (nonatomic, strong) AVAssetReader         *reader;
@property (nonatomic, strong) AVAssetWriter         *writer;
@property (nonatomic, strong) AVAssetWriterInput    *input;
@property (nonatomic, strong) dispatch_queue_t      mediaInputQueue;

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

- (AVURLAsset *)sourceAsset {
    if (!_sourceAsset) {
        _sourceAsset = [[AVURLAsset alloc] initWithURL:self.sourceUrl options:nil];
    }
    
    return _sourceAsset;
}

- (AVPlayerItem *)playerItem {
    if (!_playerItem) {
        _playerItem = [AVPlayerItem playerItemWithAsset:self.sourceAsset];
    }
    
    return _playerItem;
}

- (AVPlayer *)player {
    if (!_player) {
        _player = [AVPlayer playerWithPlayerItem:self.playerItem];
    }
    
    return _player;
}

#pragma mark -
#pragma mark View Handling

- (IBAction)onPlaySource:(id)sender {
    NSLog(@"%@", NSStringFromSelector(_cmd));
    
    [self.player play];
}

- (IBAction)onProcess:(id)sender {
    NSLog(@"%@", NSStringFromSelector(_cmd));
    
    NSError *error = nil;
    AVAssetReader *reader = [[AVAssetReader alloc] initWithAsset:self.sourceAsset error:&error];
    if (error) {
        abort();
    }
    
    self.reader = reader;
    
//    reader.timeRange = CMTimeRangeMake(kCMTimeZero, kCMTimePositiveInfinity);

    AVAsset *asset = reader.asset;
    
    AVAssetTrack *track = [[asset tracksWithMediaType:AVMediaTypeAudio] objectAtIndex:0];
    
    NSDictionary *decompressingAudioSetting = @{ AVFormatIDKey : @(kAudioFormatLinearPCM) };
    
    AVAssetReaderTrackOutput *output = [AVAssetReaderTrackOutput assetReaderTrackOutputWithTrack:track
                                                                                  outputSettings:decompressingAudioSetting];
    output.alwaysCopiesSampleData = NO;
  
    if (![reader canAddOutput:output]) {
        abort();
    }
    [reader addOutput:output];
    
    if (![reader startReading]) {
        abort();
    }
    
    NSString *documentsPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES)
                               firstObject];
    
    NSURL *outputUrl = [NSURL fileURLWithPath:[documentsPath stringByAppendingPathComponent:[self processedFileName]]];
    
    AVAssetWriter *writer = [[AVAssetWriter alloc] initWithURL:outputUrl fileType:AVFileTypeQuickTimeMovie error:&error];
    if (error) {
        abort();
    }
    self.writer = writer;
    
//    AudioChannelLayout channelLayout;
//    memset(&channelLayout, 0, sizeof(AudioChannelLayout));
//    channelLayout.mChannelLayoutTag = kAudioChannelLayoutTag_Stereo;
    
    AudioChannelLayout stereoChannelLayout = {
        .mChannelLayoutTag = kAudioChannelLayoutTag_Stereo,
        .mChannelBitmap = 0,
        .mNumberChannelDescriptions = 0
    };
    
    NSData *channelLayoutAsData = [NSData dataWithBytes:&stereoChannelLayout
                                                 length:offsetof(AudioChannelLayout, mChannelDescriptions)];
    
    NSDictionary *settings = @{AVFormatIDKey            : @(kAudioFormatMPEG4AAC),
                               AVSampleRateKey          : @44100,
                               AVNumberOfChannelsKey    : @2,
                               AVChannelLayoutKey       : channelLayoutAsData,
                               AVEncoderBitRateKey      : @64000
                               };

    
    AVAssetWriterInput *input = [[AVAssetWriterInput alloc] initWithMediaType:AVMediaTypeAudio outputSettings:settings];
    self.input = input;

    if (![writer canAddInput:input]) {
        abort();
    }
    [writer addInput:input];

    [writer startWriting];
    [writer startSessionAtSourceTime:kCMTimeZero];
    
    dispatch_queue_t mediaInputQueue = dispatch_queue_create("mediaInputQueue", DISPATCH_QUEUE_SERIAL);
    self.mediaInputQueue = mediaInputQueue;

    [input requestMediaDataWhenReadyOnQueue:mediaInputQueue usingBlock:^{
        while (input.readyForMoreMediaData) {
            CMSampleBufferRef sampleBuffer = [output copyNextSampleBuffer];

            if (sampleBuffer) {
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

- (IBAction)onPlayResult:(id)sender {
    NSLog(@"%@", NSStringFromSelector(_cmd));
    
}

#pragma mark -
#pragma mark Private

- (NSString *)processedFileName {
    NSDateFormatter *dateFormatter = [NSDateFormatter new];
    dateFormatter.dateFormat = kTENDateFormat;
    return [NSString stringWithFormat:kTENProcessedFileNameFormat, [dateFormatter stringFromDate:[NSDate date]]];
}

@end
