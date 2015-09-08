////////////////////////////////////////////////////////////////////////////////////
//     /*! \file ParamEQIf.h: \brief interface of the CParametricEqIf class. */
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

#if !defined(__libParamEQIf_HEADER_INCLUDED__)
#define __libParamEQIf_HEADER_INCLUDED__

#include "fxpack.h"

class CParametricEqIf
{
public:
    /*! provided filter shapes and orders*/
    enum EqType_t
    {	
        kLP6,           //!< first order low pass (6dB/oct), parameters: f_c
        kLP12,          //!< second order low pass (12dB/oct), parameters: f_c, Q (Resonance)
        kHP6,           //!< first order high pass (6dB/oct), parameters: f_c
        kHP12,          //!< second order high pass (12dB/oct), parameters: f_c, Q (Resonance)
        kBP12,          //!< second order band pass, parameters: f_c, Q
        kLShelv6,       //!< first order low shelving, parameters: f_c, Gain
        kLShelv12,      //!< second order low shelving, parameters: f_c, Q, Gain 
        kHShelv6,       //!< first order high shelving, parameters: f_c, Gain
        kHShelv12,      //!< second order high shelving, parameters: f_c, Q, Gain
        kPeak12,        //!< second order peak, parameters: f_c, Q, Gain
        kNotch12,       //!< second order notch, parameters: f_c, Q
        kAP6,           //!< first order all pass, parameters: f_c
        kAP12,          //!< second order all pass, parameters: f_c, Q

        kNumOfEqTypes
    };

    /*! available EQ parameters, not all are always available, see CParametricEqIf::EqTypes_t */
    enum EqParameter_t
    {
        kEqParamFrequency,  //!< Frequency in Hz, CutOff or Mid (20...20kHz)
        kEqParamQ,          //!< Q (0.1...24)
        kEqParamGain,       //!< amplification in dB (-24...24)

        kNumOfEqParameters
    };


    /*!
    * create new instance of one filter
    *
    * @param pCParamEQ : handle to new instance
    * @param fSampleRate : sample rate of input/output audio data
    * @param iNumberOfChannels : number of channels of input/output audio data
    *
    * @return zfxError_t : kNoError if no error
    */
    static zfxError_t   CreateInstance (CParametricEqIf*& pCParamEQ, float fSampleRate, int iNumberOfChannels);

    /*!
    * destroy an instance 
    *
    * @param pCParamEQ : handle to instance to be destroyed
    *
    * @return zfxError_t : kNoError if no error
    */
    static zfxError_t   DestroyInstance (CParametricEqIf*& pCParamEQ);


    /*!
    * processing function, if called with no input arguments, only the coefficients are updated internally (if necessary)
    *
    * @param ppfInputBuffer : input audio buffer of dimension [channels][frames]
    * @param ppfOutputBuffer : output audio buffer of dimension [channels][frames], may be identical with ppfInputBuffer for inplace processing
    * @param iNumberOfFrames : number of frames (=samples per channel)
    *
    * @return zfxError_t : kNoError if no error
    */
    virtual zfxError_t  Process (float **ppfInputBuffer = 0, float **ppfOutputBuffer = 0, int iNumberOfFrames = 0) = 0;

    /*!
    * set filter type 
    *
    * @param eEqType : filter type as defined in CParametricEqIf::EqType_t
    *
    * @return zfxError_t : kNoError if no error
    */
    virtual zfxError_t  SetType (EqType_t eEqType) = 0;

    /*!
    * return current filter type 
    *
    * @return EqType_t : current filter type as defined in CParametricEqIf::EqType_t
    */
    virtual EqType_t    GetType () = 0;

    /*!
    * set filter parameter (parameters are updated once per process call)
    *
    * @param eEqParameterIdx : parameter index as defined in CParametricEqIf::EqParameter_t
    * @param fParamValue : parameter value
    *
    * @return zfxError_t : kNoError if no error
    */
    virtual zfxError_t  SetParam (EqParameter_t eEqParameterIdx, float fParamValue) = 0;

    /*!
    * return current filter parameter 
    *
    * @param eEqParameterIdx : parameter index as defined in CParametricEqIf::EqParameter_t
    *
    * @return float : corresponding parameter value
    */
    virtual float       GetParam (EqParameter_t eEqParameterIdx) = 0;

    /*!
    * return magnitude response at specific frequency
    *
    * @param fFrequencyInHz : frequency in Hz to be evaluated
    *
    * @return float : magnitude response in dB
    */
    virtual float       GetMagnitudeInDb (float fFrequencyInHz) = 0;

    /*!
    * set optional noise addition to avoid denormals 
    *
    * @param bAddNoise : true if noise should be added
    *
    * @return zfxError_t : kNoError if no error
    */
    virtual zfxError_t  SetAddDenormalNoise (bool bAddNoise = true) = 0;

    /*!
    * return if noise addition to avoid denormals is avtivated or not
    *
    * @return bool : true if activated
    */
    virtual bool        GetAddDenormalNoise () = 0;

    /*!
    * set effect to bypass
    *
    * @return zfxError_t : kNoError if no error
    */
    virtual zfxError_t  SetBypass (bool bBypass = false) = 0;

    /*!
    * return bypass state
    *
    * @return zfxError_t : kNoError if no error
    */
    virtual bool        GetBypass ( ) = 0;

    /*!
    * reset internal buffers
    *
    * @return zfxError_t : kNoError if no error
    */
    virtual zfxError_t  Reset () = 0;

};

#endif // #if !defined(__libParamEQIf_HEADER_INCLUDED__)



