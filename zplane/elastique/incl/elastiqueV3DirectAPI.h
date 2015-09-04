////////////////////////////////////////////////////////////////////////////////////
// /*! \file elastiqueDirectAPI.h */ 
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
//  $RCSfile: elastiqueDirectAPI.h,v $
//  $Author: flea $
//  $Date: 2008-08-05 11:20:24 $
//
//
////////////////////////////////////////////////////////////////////////////////////


#if !defined(__libELASTIQUEV3DIRECTAPI_HEADER_INCLUDED__)
#define __libELASTIQUEV3DIRECTAPI_HEADER_INCLUDED__



/*!
CLASS
    

    This class provides the interface for zplane's élastique class. 

USAGE
    
*/
class CElastiqueV3DirectIf
{
public:
    
    virtual ~CElastiqueV3DirectIf() {};  

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
     * @param iNumOfChannels : number of channels (1..2)
     * @param fSampleRate : input samplerate
     * @param fMinCombinedFactor: minimum combined factor to be used: the combined minimum is minstretch*minpitch
     *
     * @return static int  : returns some error code otherwise NULL 
     */
    static int      CreateInstance (CElastiqueV3DirectIf*& cCElastique, int iNumOfChannels, float fSampleRate, ElastiqueMode_t eElastiqueMode = kV3, float fMinCombinedFactor = 0.1f);
    
    /*!
     * destroys an instance of the zplane's élastique class
     *
     * @param cCElastique : pointer to the instance to be destroyed
     *
     * @return static int  : returns some error code otherwise NULL
     */
    static int      DestroyInstance(CElastiqueV3DirectIf*  cCElastique);
  /*!
    * does the initial buffer filling if the number of frames provided is as retrieved by CElastiqueDirectIf::GetPreFramesNeeded()
    * returns first output buffer
    * should be used for equidistant stretch/pitch factor setting
    *
    * @param ppInSampleData : double pointer to the input buffer of samples [channels][samples]
    * @param iNumOfInFrames :  the number of input frames
    * @param ppOutSampleData : double pointer to the output buffer of samples [channels][samples]
    *
    * @return int  : returns number of processed frames
    */
    virtual int     PreFillData(float** ppInSampleData, int iNumOfInFrames, float** ppOutSampleData) = 0;
    
    /*!
    * when using PreFillData() for equidistant processing this returns the number of frames that have to be omitted in the beginning
    *
    * @return int  : returns the number of frames to be ommitted
     */
    virtual int     GetNumOfInitialUnusedFrames() = 0;

    /*!
     * does the initial buffer filling if the number of frames provided is as retrieved by CElastiqueDirectIf::GetPreFramesNeeded()
     * immediately returns some unprocessed frames to give some time to do the first processing, overlapping is done internally
     * should be used for distributing processing over time      
     *
     * @param ppInSampleData : double pointer to the input buffer of samples [channels][samples]
     * @param iNumOfInFrames :  the number of input frames
     * @param ppOutSampleData : double pointer to the output buffer of samples [channels][samples]
     *
     * @return int  : returns number of processed frames
     */
    virtual int     PreProcessData(float** ppInSampleData, int iNumOfInFrames, float** ppOutSampleData) = 0;

    /*!
     * does the actual processing if the number of frames provided is as retrieved by CElastiqueDirectIf::GetFramesNeeded()
     *
     * @param ppInSampleData : double pointer to the input buffer of samples [channels][samples]
     * @param iNumOfInFrames :  the number of input frames
     *
     * @return int  : returns the error code, otherwise 0
     */
    virtual int     ProcessData(float** ppInSampleData, int iNumOfInFrames) = 0;


    /*!
     * call as often as GetNumOfProcessCalls() tells you
     *
     * @return int  : returns the error code, otherwise 0
     */
    virtual int     ProcessData() = 0;


    /*!
     * after processing cal that function to retreive the processed data
     *
     * @param ppOutSampleData : 
     *
     * @return int  : returns the number of frames returned
     */
    virtual int     GetProcessedData(float** ppOutSampleData) = 0;


    /*!
     * returns the number of process calls needed 
     *
     * @return int  : returns the number of process calls required
     */
    virtual int     GetNumOfProcessCalls() = 0;

    /*!
     * returns the number of frames that will be put out by the ProcessData function
     *
     * @return int  : returns the number of frames required for the output buffer
     */
    virtual int     GetFramesProcessed() = 0;
    
   
   /*!
     * returns the number of frames needed for the first pre-processing step, this function should be always called directly before CElastiqueDirectIf::PreProcessData(..)
     *
     * @return int  : returns the number of frames required
     */
    virtual int     GetPreFramesNeeded() = 0;

    
 
    /*!
     * returns the number of frames needed for the next processing step, this function should be always called directly before CElastiqueProDirectIf::ProcessData(..)
     * this function always returns the same value
     *
     * @return int  : returns the number of frames required
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
    * @param bUsePitchSync: synchronizes timestretch and pitchshifting (default is set to _FALSE in order to preserve old behavior, see documentation), this is ignored in SOLO modes
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
    * @param bUsePitchSync: synchronizes timestretch and pitchshifting (default is set to _FALSE in order to preserve old behavior, see documentation), this is ignored in SOLO modes
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
     * returns the current input time position 
     *
     * \returns
     * returns the current input time position 
     *
     */
    virtual long long GetCurrentTimePos() = 0;


    /*!
     * gets the last frames in the internal buffer, ProcessData must have returned -1 before.
     *
     * @param ppfOutSampleData: double pointer to the output buffer of samples [channels][samples]
     *
     * @return int  : returns some error code otherwise number of frames returned
     */
    virtual int     FlushBuffer(float** ppfOutSampleData) = 0;
  
    /*!
     * sets the stereo input mode
     *
     * @param eStereoInputMode : sets the mode according to _ElastiqueStereoInputMode_
     * 
     * @return static int  : returns some error code otherwise NULL 
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
     * if set to true holds the current audio endlessly until set to false
     * may either resume from the point when hold was enabled or bypass the current audio and resume in time as if the input audio had been playing
     *
     * @param bSetHold : enable or disable hold 
     * @param bKeepTime: if set to true the audio stream will continue but will be bypassed
     *    
     */
    virtual int SetHold(bool bSetHold, bool bKeepTime = false) = 0;

    /*!
     * This method is an alternative way of stretching. Instead setting an explicit stretch factor time stamps for input and output are set and the stretch factor is determined internally in order 
     * to achieve the highest timing precision possible for that sync point. Time stamps are always relative to the last play start position (input time resp. output time after Reset). Additionally a pitch factor can be set. 
     * Using the SetStretchPitchQFactor(.) or SetStretchQPitchFactor(.) will break the process, therefore it is advised to stick on method or the other.
     *
     * @param iInputTimeInSamples : the next input time stamp starting from the current input play position
     * @param iOutputTimeInSamples : the corresponding output time stamp from the current output play position
     * @param fPitchFactor : the pitch factor for that part to be stretched
     *
     * @return int  : returns some error code if input or output time stamps are invalid otherwise NULL 
     */
    virtual int SetNextSyncPoint(int iInputTimeInSamples, int iOutputTimeInSamples, float &fPitchFactor) = 0;

    /*!
     * Returns true if the last set sync point has been reached and been processed. It is advised to wait for this method to be true in order to set the next sync point. 
     * Otherwise the whole process may be broken.
     *     
     * @return bool  : returns true is the last sync point has been reached and processed, otherwise false
     */
    virtual bool IsReadyForNextSyncPoint() const = 0; 
};


#endif // #if !defined(__libELASTIQUEV3DIRECTAPI_HEADER_INCLUDED__)



