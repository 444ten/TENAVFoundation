////////////////////////////////////////////////////////////////////////////////////
// /*! \file elastiqueAPI.h */ 
//
//  Copyright (c) 2000-2011  
//      zplane.development GmbH & Co. KG
//
//  CONFIDENTIALITY:
//
//      This file is the property of zplane.development.
//      It contains information that is regarded as privilege
//      and confidential by zplane.development.
//      It may not be publicly disclosed or communicated in any way without 
//      prior written authorization by zplane.development.
//      It cannot be copied, used, or modified without obtaining
//      an authorization from zplane.development.
//      If such an authorization is provided, any modified version or
//      copy of the software has to contain this header.
//
//  WARRANTIES: 
//      This software is provided as << is >>, zplane.development 
//      makes no warranty express or implied with respect to this software, 
//      its fitness for a particular purpose or its merchantability. 
//      In no case, shall zplane.development be liable for any 
//      incidental or consequential damages, including but not limited 
//      to lost profits.
//
//      zplane.development shall be under no obligation or liability in respect of 
//      any infringement of statutory monopoly or intellectual property 
//      rights of third parties by the use of elements of such software 
//      and User shall in any case be entirely responsible for the use 
//      to which he puts such elements.	
//
////////////////////////////////////////////////////////////////////////////////////
//  CVS INFORMATION
//
//  $RCSfile: elastiqueAPI.h,v $
//  $Author: flea $
//  $Date: 2008-07-29 10:54:11 $
//
//
//
////////////////////////////////////////////////////////////////////////////////////


#if !defined(__libELASTIQUEV3API_HEADER_INCLUDED__)
#define __libELASTIQUEV3API_HEADER_INCLUDED__



/*!
CLASS
    

    This class provides the interface for zplane's élastique class. 

USAGE
    
*/
class CElastiqueV3If
{
public:
    
    virtual ~CElastiqueV3If() {};

    enum ElastiqueStereoInputMode_t
    {
        kPlainStereoMode   = 0,     //!< normal LR stereo mode
        kMSMode                     //!< MS stereo mode M must be in channel 0 and S in channel 1
    } ;
    
    enum ElastiqueMode_t
    {
        kV3 = 0,
        kV3mobile,        
    };

    enum Version_t
    {
        kMajor,
        kMinor,
        kPatch,
        kRevision,

        kNumVersionInts
    };

    

    static const int  GetVersion (const Version_t eVersionIdx);
    static const char* GetBuildDate ();

    /*!
     * creates an instance of zplane's élastique class
     *
     * @param cCElastique : returns a pointer to the class instance
     * @param iOutputBufferSize : desired max number of frames at the output, if not changed by GetFramesNeeded(.) this one is the default output block size, maximum is 1024 sample frames
     * @param iNumOfChannels : number of channels (1..2)
     * @param fSampleRate : input samplerate
     * @param fMinCombinedFactor: minimum combined factor of stretch * pitch to be used
     *
     * @return static int  : returns some error code otherwise NULL 
     */
    static int      CreateInstance (CElastiqueV3If*& cCElastique, int iMaxOutputBufferSize, int iNumOfChannels, float fSampleRate, ElastiqueMode_t eElastiqueMode = kV3, float fMinCombinedFactor = 0.1f);
    
    /*!
     * destroys an instance of the zplane's élastique class
     *
     * @param cCElastique : pointer to the instance to be destroyed
     *
     * @return static int  : returns some error code otherwise NULL
     */
    static int      DestroyInstance(CElastiqueV3If*  cCElastique);
    

    /*!
     * does the actual processing if the number of frames provided is as retrieved by CElastiqueIf::GetFramesNeeded()
     * this function always returns the number of frames as specified when calling CElastiqueIf::CreateInstance(..)
     *
     * @param ppInSampleData : double pointer to the input buffer of samples [channels][samples]
     * @param iNumOfInFrames :  the number of input frames
     * @param ppOutSampleData : double pointer to the output buffer of samples [channels][samples]
     *
     * @return virtual int  : returns the error code, otherwise 0
     */
    virtual int     ProcessData(float** ppInSampleData, int iNumOfInFrames, float** ppOutSampleData) = 0;


    /*!
     * returns the number of frames needed for the next processing step according to the required buffersize, this function should be always called directly before CElastiqueIf::ProcessData(..)
     * this function changes the internal output buffer size as specified when calling CElastiqueIf::CreateInstance(..)
     * use this function if want to change the output size.
     *
     * @param iOutBufferSize : required output buffer size
     *
     * @return virtual int  : returns the number of frames required or -1 if OutBufferSize exceeds the maxbuffersize defined upon createinstance
     */
    virtual int     GetFramesNeeded(int iOutBufferSize) = 0;


    /*!
     * returns the number of frames needed for the next processing step, this function should be always called directly before CElastiqueIf::ProcessData(..)
     *
     * @return virtual int  : returns the number of frames required
     */
    virtual int     GetFramesNeeded() = 0;

    /*!
    * returns the maximum number of frames needed based on the minimum combined factor passed on CreateInstance
    *
    * @return virtual int : returns number of frames
    */
    virtual int     GetMaxFramesNeeded() = 0;

    /*!
    * sets the internal stretch & pitch factor. The stretch factor is quantized. 
    * The product of the stretch factor and the pitch factor must be larger than the fMinCombinedFactor defined upon instance creation.
    *
    * @param fStretchFactor : stretch factor 
    * @param fPitchFactor: pitch factor 
    * @param bUsePitchSync: synchronizes timestretch and pitchshifting (default is set to _FALSE in order to preserve old behavior, see documentation)
    *
    * @return int  : returns the error code, otherwise 0
    */
    virtual int     SetStretchQPitchFactor(float& fStretchFactor, float fPitchFactor, bool bUsePitchSync= false) = 0;

    /*!
    * sets the internal stretch & pitch factor.  The pitch factor is quantized
    * The product of the stretch factor and the pitch factor must be larger than the fMinCombinedFactor defined upon instance creation.
    *
    * @param fStretchFactor : stretch factor 
    * @param fPitchFactor: pitch factor 
    * @param bUsePitchSync: synchronizes timestretch and pitchshifting (default is set to _FALSE in order to preserve old behavior, see documentation)
    *
    * @return int  : returns the error code, otherwise 0
    */
    virtual int     SetStretchPitchQFactor(float fStretchFactor, float& fPitchFactor, bool bUsePitchSync= false) = 0;

    /*!
     * resets the internal state of the élastique algorithm
     *
     * @return int  : returns some error code otherwise NULL
     */
    virtual void    Reset() = 0;
    

    /*!
     * gets the last frames in the internal buffer, ProcessData must have returned -1 before.
     *
     * @param ppfOutSampleData: double pointer to the output buffer of samples [channels][samples]
     *
     * @return int  : returns some error code otherwise NULL
     */
    virtual int     FlushBuffer(float** ppfOutSampleData) = 0;

    
    /*!
     * returns the number of frames already buffered in the output buffer. 
     * The number of frames is divided by the stretch factor, so that one is able to calculate the current position within the source file.
     *
     * @return int  : returns the number of frames buffered 
     */
    virtual int     GetFramesBuffered() = 0;

  
    /*!
     * sets the stereo input mode
     *
     * @param eStereoInputMode : sets the mode according to _ElastiqueStereoInputMode_
     * 
     * @return int  : returns some error code otherwise NULL 
     */
    virtual int SetStereoInputMode (ElastiqueStereoInputMode_t eStereoInputMode) = 0;

    /*!
     * sets a cutoff frequency in order to save processing time
     *
     * @param fFreq : cutoff freq in Hz
     *
     * @return int  : returns some error code otherwise NULL 
     */
    virtual int SetCutOffFreq(float fFreq) = 0;

    /*!
     * gets the current input time position in sample frames
     *
     * @return long long: returns the time position
     */
    virtual long long GetCurrentTimePos() = 0;

     /*!
     * if set to true holds the current audio endlessly until set to false
     * may either resume from the point when hold was enabled or bypass the current audio and resume in time as if the input audio had been playing
     *
     * @param bSetHold : enable or disable hold 
     * @param bKeepTime: if set to true the input audio stream will continue but will be bypassed
     *    
     */
    virtual int SetHold(bool bSetHold, bool bKeepTime = false) = 0;
};


#endif // #if !defined(__libELASTIQUEAPI_HEADER_INCLUDED__)



