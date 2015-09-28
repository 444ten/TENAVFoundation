////////////////////////////////////////////////////////////////////////////////////
//     /*! \file ChorusFlangerIf.h: \brief interface of the CChorusFlangerIf class. */
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

#if !defined(__libChorusFlangerIf_HEADER_INCLUDED__)
#define __libChorusFlangerIf_HEADER_INCLUDED__

#include "fxpack.h"

class CChorusFlangerIf
{
public:
    /*! provided presets = parameter initialization*/
    enum ChFlPreset_t
    {	
        kChorus,            //!< default chorus
        kWhiteChorus,       //!< white chorus (feedback)
        kFlanger1,          //!< flanger
        kFlanger2,          //!< flanger (feedback)
        kVibrato,           //!< vibrato

        kNumOfChFlPresets
    };

    /*! available ChFl parameters */
    enum ChFlParameter_t
    {
        kChFlParamLfoDepthInS,   //!< modulation range peak to peak that is added to delay (0...0.1) leads to (delaytime...delaytime+LfoDepth)
        kChFlParamLfoFreqInHz,   //!< modulation frequency (0...20)
        kChFlParamDelayInS,      //!< delay (0...0.150)
        kChFlParamFeedbackRel,   //!< amount of feedback between (0...0.9999)
        kChFlParamFeedForwardRel,//!< amount of feedforward (modulation output) (0...1)
        kChFlParamBlendRel,      //!< amount of blend (modulation input) (0...1)
        kChFlParamLpInFeedbackRel, //!< amount of low pass filtering in feedback (0...0.9999F)
        kChFlParamStereoFadeRel, //!< amount of cross feedback between channels (0...1)
        kChFlParamWetnessRel,    //!< amount of effect (0...1) (this is closely related to blend)

        kNumOfChFlParameters
    };


    /*!
    * create new instance
    *
    * @param pCChorusFlanger : handle to new instance
    * @param fSampleRate : sample rate of input/output audio data
    * @param iNumberOfChannels : number of channels of input/output audio data
    *
    * @return zfxError_t  : kNoError if no error
    */
    static zfxError_t   CreateInstance (CChorusFlangerIf*& pCChorusFlanger, float fSampleRate, int iNumberOfChannels);

    /*!
    * destroy an instance 
    *
    * @param pCChorusFlanger : handle to instance to be destroyed
    *
    * @return zfxError_t  : kNoError if no error
    */
    static zfxError_t   DestroyInstance (CChorusFlangerIf*& pCChorusFlanger);



    /*!
    * processing function 
    *
    * @param ppfInputBuffer : input audio buffer of dimension [channels][frames]
    * @param ppfOutputBuffer : output audio buffer of dimension [channels][frames], may be identical with ppfInputBuffer for inplace processing
    * @param iNumberOfFrames : number of frames (=samples per channel)
    *
    * @return zfxError_t  : kNoError if no error
    */
    virtual zfxError_t  Process (float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames) = 0;

    /*!
    * set preset (different parameter initializations)
    *
    * @param eChFlPreset : filter type as defined in CChorusFlangerIf::ChFlPreset_t
    *
    * @return zfxError_t  : kNoError if no error
    */
    virtual zfxError_t  SetPreset (ChFlPreset_t eChFlPreset)= 0;

    /*!
    * return current preset (is also returned when modified)
    *
    * @return ChFlPreset_t : current filter type as defined in CChorusFlangerIf::ChFlPreset_t
    */
    virtual ChFlPreset_t    GetPreset () = 0;

    /*!
    * set lfo waveform
    *
    * @param eLfoType : filter type as defined in ::zfxLfoType_t
    *
    * @return zfxError_t  : kNoError if no error
    */
    virtual zfxError_t  SetLfoType (zfxLfoType_t eLfoType) = 0;

    /*!
    * return current lfo waveform
    *
    * @return zfxLfoType_t : lfo type
    */
    virtual zfxLfoType_t   GetLfoType () = 0;

    /*!
    * set phase difference between succeeding channels
    *
    * @param ePhase : filter type as defined in ::zfxPhaseOffsetBetweenChannels_t
    *
    * @return zfxError_t  : kNoError if no error
    */
    virtual zfxError_t  SetLfoPhaseBetweenChannels (zfxPhaseOffsetBetweenChannels_t ePhase) = 0;

    /*!
    * return current phase difference between channels
    *
    * @return zfxPhaseOffsetBetweenChannels_t : phase between succeeding channels
    */
    virtual zfxPhaseOffsetBetweenChannels_t    GetLfoPhaseBetweenChannels () = 0;

    /*!
    * set parameter 
    *
    * @param eChFlParameterIdx : parameter index as defined in CChorusFlangerIf::ChFlParameter_t
    * @param fParamValue : parameter value
    *
    * @return zfxError_t  : kNoError if no error
    */
    virtual zfxError_t  SetParam (ChFlParameter_t eChFlParameterIdx, float fParamValue) = 0;

    /*!
    * return current parameter 
    *
    * @param eChFlParameterIdx : parameter index as defined in CChorusFlangerIf::ChFlParameter_t
    *
    * @return float : corresponding parameter value
    */
    virtual float       GetParam (ChFlParameter_t eChFlParameterIdx) = 0;

    /*!
    * set optional noise addition to avoid denormals 
    *
    * @param bAddNoise : true if noise should be added
    *
    * @return EqError_t : kNoError if no error
    */
    virtual zfxError_t  SetAddDenormalNoise (bool bAddNoise = true) = 0;

    /*!
    * return if noise addition to avoid denormals is avtivated or not
    *
    * @return bool : true if activated
    */
    virtual bool        GetAddDenormalNoise () = 0;

    /*!
    * set effect to bypass mode
    *
    * @return zfxError_t  : kNoError if no error
    */
    virtual zfxError_t  SetBypass (bool bBypass = false) = 0;

    /*!
    * return bypass state
    *
    * @return zfxError_t  : kNoError if no error
    */
    virtual bool        GetBypass ( ) = 0;

    /*!
    * reset internal buffers
    *
    * @return zfxError_t  : kNoError if no error
    */
    virtual zfxError_t  Reset () = 0;

    float fxpackGetParameterSmooth();
};

#endif // #if !defined(__libChorusFlangerIf_HEADER_INCLUDED__)



