////////////////////////////////////////////////////////////////////////////////////
//     /*! \file DynamicsIf.h: \brief interface of the CDynamicsIf class. */
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

#if !defined(__libDynamicsIf_HEADER_INCLUDED__)
#define __libDynamicsIf_HEADER_INCLUDED__

#include "fxpack.h"

class CDynamicsIf
{
public:

    /*! available Dyn parameters */
    enum DynParameter_t
    {     
        kDynParamThreshold, //!< the operation threshold in dB (0..-120)
        kDynParamRatio,     //!< the ration of dynamic processing, only available for the compressor and expander modes (1..100), in compressor mode it is 1:r ratio and in expander mode r:1.
        kDynParamAttack,    //!< the attack time of the dynamic processor in sec (0.00001..0.1), the lowest value is recommended for limiter operation
        kDynParamRelease,   //!< the release time of the dynamic processor in sec (0.001..4.0)
        kDynParamGain,      //!< the post processing gain in dB (-80..80)
        kDynParamLookAhead, //!< the look ahead time in sec (0.0..0.01)
 
        kNumOfDynParameters
    };

    /*! available Dyn modes */
    enum DynMode_t
    {     
        kDynModeCompressor, //!< compressor mode (all parameters)
        kDynModeLimiter,    //!< limiter mode (ratio parameter is not available)
        kDynModeExpander,   //!< expander mode (all parameters)
        kDynModeGate,       //!< noise gate mode (ratio parameter is not available)

        kNumOfDynModes
    };
  

    /*!
    * create new instance
    *
    * @param pCDynamics : handle to new instance
    * @param fSampleRate : sample rate of input/output audio data
    * @param iNumberOfChannels : number of channels of input/output audio data
    *
    * @return zfxError_t : kNoError if no error
    */
    static zfxError_t       CreateInstance (CDynamicsIf*& pCDynamics, float fSampleRate, int iNumberOfChannels);

    /*!
    * destroy an instance 
    *
    * @param pCDynamics : handle to instance to be destroyed
    *
    * @return zfxError_t : kNoError if no error
    */
    static zfxError_t       DestroyInstance (CDynamicsIf*& pCDynamics);



    /*!
    * processing function 
    *
    * @param ppfInputBuffer : input audio buffer of dimension [channels][frames]
    * @param ppfSideChainBuffer : side chain audio buffer of dimension [channels][frames], set to NULL if there is no side chain
    * @param ppfOutputBuffer : output audio buffer of dimension [channels][frames], may be identical with ppfInputBuffer for inplace processing
    * @param iNumberOfFrames : number of frames (=samples per channel)
    *
    * @return zfxError_t : kNoError if no error
    */
    virtual zfxError_t      Process (float **ppfInputBuffer, float **ppfSideChainBuffer, float **ppfOutputBuffer, int iNumberOfFrames) = 0;

    /*!
    * returns max. level and max reduction for requested channel for each block
    *
    * @param iChannelIdx    : index of channel of interest
    * @param fReduction     : current reduction in dB
    * @param fLevel         : current level in dB
    *
    * @return zfxError_t : kNoError if no error
    */
    virtual zfxError_t      GetCurrentLevels(int iChannelIdx, float& fReduction, float& fLevel) = 0;

    /*!
    * set operation mode
    *
    * @param eDynMode : operation mode
    *
    * @return zfxError_t : kNoError if no error
    */
    virtual zfxError_t      SetMode(DynMode_t eDynMode) = 0;

    /*!
    * return current operation mode
    *
    * @return zfxError_t : kNoError if no error
    */
    virtual DynMode_t       GetMode() = 0;

    /*!
    * enable/disable channel link
    *
    * @param bLinkChannels : enable/disable
    *
    * @return zfxError_t : kNoError if no error
    */
    virtual zfxError_t      SetChannelLink(bool bLinkChannels) = 0;

    /*!
    * return channel link state
    *
    * @return zfxError_t : kNoError if no error
    */
    virtual bool            GetChannelLink() = 0;

    /*!
    * set parameter 
    *
    * @param eDynParameterIdx : parameter index as defined in CDynamicsIf::DynParameter_t
    * @param fParamValue : parameter value
    *
    * @return zfxError_t : kNoError if no error
    */
    virtual zfxError_t      SetParam (DynParameter_t eDynParameterIdx, float fParamValue) = 0;

    /*!
    * return current parameter 
    *
    * @param eDynParameterIdx : parameter index as defined in CDynamicsIf::DynParameter_t
    *
    * @return float : corresponding parameter value
    */
    virtual float           GetParam (DynParameter_t eDynParameterIdx)               = 0;

    /*!
    * set effect to bypass mode
    *
    * @return zfxError_t : kNoError if no error
    */
    virtual zfxError_t      SetBypass (bool eBypass = false)                           = 0;

    /*!
    * return bypass state
    *
    * @return zfxError_t : kNoError if no error
    */
    virtual bool            GetBypass ( )                                              = 0;

    /*!
    * reset internal buffers
    *
    * @return zfxError_t : kNoError if no error
    */
    virtual zfxError_t      Reset ()                                                   = 0;

};

#endif // #if !defined(__libDynamicsIf_HEADER_INCLUDED__)



