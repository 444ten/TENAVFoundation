////////////////////////////////////////////////////////////////////////////////////
//     /*! \file LoopRollIf.h: \brief interface of the CLoopRollIf class. */
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

#if !defined(__libLoopRollIf_HEADER_INCLUDED__)
#define __libLoopRollIf_HEADER_INCLUDED__

#include "fxpack.h"

class CLoopRollIf
{
public:

    /*! available LoopRoll parameters */
    enum LoopRollParameter_t
    {
        kLRLoopLengthInBeats,		//!< loop length as multiple of one beat
        kLRCrossfadeLengthInMS,     //!< crossfade length in milliseconds at loop endpoint
        kLRDryWetRatio,             //!< mixing ratio between looped and non looped stream, 1 = wet, 0 = dry

        kNumOfLoopRollParameters
    };

    /*! available LoopRoll fade curves */
    enum LoopRollFadeType_t
    {
        kLRFadeLinear,			    //!< linear fade
        kLRFadeSquareRoot,		    //!< square root like fade with constant power
        kLRNFadeCosine,             //!< cosine fade with constant power

        kNumOfLoopRollFadeTypes
    };

    /*!
    * create new instance of one filter
    *
    * @param CLoopRollf : handle to new instance
    * @param fSampleRate : sample rate of input/output audio data
    * @param iNumberOfChannels : number of channels of input/output audio data
    *
    * @return zfxError_t : kNoError if no error
    */
    static zfxError_t   CreateInstance (CLoopRollIf*& pCLoopRollf, float fSampleRate, int iNumberOfChannels);


    /*!
    * destroy an instance 
    *
    * @param pCLoopRollf : handle to instance to be destroyed
    *
    * @return zfxError_t : kNoError if no error
    */
    static zfxError_t   DestroyInstance (CLoopRollIf*& pCBitCrushIf);


    /*!
    * init instance of one filter
    *
    * @param iBeatsInBuffer : number of beats to keep in bufer
    * @param iBPM : speed of input data as beats per minute
    * @param iNumberOfChannels : number of channels of input/output audio data
    *
    * @return zfxError_t : kNoError if no error
    */
    virtual zfxError_t   InitInstance (int iBPM = 120, int iMaxBufferLengthInBeats = 4, float fMaxCrossfadeLength = 100.0f) = 0;

    /*!
    * processing function
    *
    * @param ppfInputBuffer : input audio buffer of dimension [channels][frames]
    * @param ppfOutputBuffer : output audio buffer of dimension [channels][frames], may be identical with ppfInputBuffer for inplace processing
    * @param iNumberOfFrames : number of frames (=samples per channel)
    *
    * @return zfxError_t : kNoError if no error
    */
    virtual zfxError_t  Process (float **ppfInputBuffer = 0, float **ppfOutputBuffer = 0, int iNumberOfFrames = 0) = 0;


    /*!
    * set filter parameter (parameters are updated once per process call)
    *
    * @param eLoopRollParameterIdx : parameter index as defined in CLoopRoleIf::LoopRollParameter_t
    * @param fParamValue : parameter value
    *
    * @return zfxError_t : kNoError if no error
    */
    virtual zfxError_t  SetParam (LoopRollParameter_t eLoopRollParameterIdx, float fParamValue) = 0;


    /*!
    * return current filter parameter 
    *
    * @param eLoopRollParameterIdx : parameter index as defined in CLoopRoleIf::LoopRollParameter_t
    *
    * @return float : corresponding parameter value
    */
    virtual float       GetParam (LoopRollParameter_t eLoopRollParameterIdx) = 0;


    /*!
    * set crossfade function type (parameter is updated once per process call)
    *
    * @param eLoopRollFadeIdx :  crossfade function as defined in CLoopRoleIf::LoopRollFadeType_t 
    *
    * @return zfxError_t : kNoError if no error
    */
    virtual zfxError_t  SetCrossfadeType (LoopRollFadeType_t eLoopRollFadeIdx) = 0;


    /*!
    * return crossfade function type     
    *
    * @return LoopRollFadeType_t : current crossfade function type
    */
    virtual LoopRollFadeType_t  GetCrossfadeType () = 0;


    /*!
    * set effect to bypass
    *
    * @return zfxError_t : kNoError if no error
    */
    virtual zfxError_t  SetBypass (bool bBypass = false, int iStartLoopAtOffset = 0) = 0;

    /*!
    * return bypass state
    *
    * @return zfxError_t : kNoError if no error
    */
    virtual bool        GetBypass () = 0;

    /*!
    * reset internal buffers
    *
    * @return zfxError_t : kNoError if no error
    */
    virtual zfxError_t  Reset () = 0;

};

#endif // #if !defined(__libLoopRollIf_HEADER_INCLUDED__)



