////////////////////////////////////////////////////////////////////////////////////
//     /*! \file BitCrushIf.h: \brief interface of the BitCrushIf class. */
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

#if !defined(__libBitCrushIf_HEADER_INCLUDED__)
#define __libBitCrushIf_HEADER_INCLUDED__

#include "fxpack.h"

class CBitCrushIf
{
public:

    /*! available BitCrush parameters */
    enum BitCrushParameter_t
    {
        kBitCrushOutputBits,			//!< Output bit depth as int value
        kBitCrushDownsampleFactor,      //!< Downsample factor as int value
        kBitCrushClippingThreshold,		//!< Clipping threshold in dB as float
        kBitCrushWrapAround,			//!< Enable overflow of values above clipping threshold
                                        //	 false: hard clipping
                                        //   true:  wrap around clipping

        kNumOfBitCrushParameters
    };

    /*!
    * create new instance of one filter
    *
    * @param pCBitCrushIf : handle to new instance
    * @param fSampleRate : sample rate of input/output audio data
    * @param iNumberOfChannels : number of channels of input/output audio data
    *
    * @return zfxError_t : kNoError if no error
    */
    static zfxError_t   CreateInstance (CBitCrushIf*& pCBitCrushIf, float fSampleRate, int iNumberOfChannels);

    /*!
    * destroy an instance 
    *
    * @param pCBitCrushIf : handle to instance to be destroyed
    *
    * @return zfxError_t : kNoError if no error
    */
    static zfxError_t   DestroyInstance (CBitCrushIf*& pCBitCrushIf);


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
    * @param eBitCrushParameterIdx : parameter index as defined in CBitCrushIf::BitCrushParameter_t
    * @param fParamValue : parameter value
    *
    * @return zfxError_t : kNoError if no error
    */
    virtual zfxError_t  SetParam (BitCrushParameter_t eBitCrushParameterIdx, float fParamValue) = 0;

    /*!
    * return current filter parameter 
    *
    * @param eBitCrushParameterIdx : parameter index as defined in CBitCrushIf::BitCrushParameter_t
    *
    * @return float : corresponding parameter value
    */
    virtual float       GetParam (BitCrushParameter_t eBitCrushParameterIdx) = 0;

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

#endif // #if !defined(__libBitCrushIf_HEADER_INCLUDED__)



