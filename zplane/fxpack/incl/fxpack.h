////////////////////////////////////////////////////////////////////////////////////
//     /*! \file fxpack.h: \brief global enums and functions for fxpack */
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

#if !defined(__libfxpack_HEADER_INCLUDED__)
#define __libfxpack_HEADER_INCLUDED__

/*! lfo specification for modulated effects */
enum zfxLfoType_t
{
    kSine,              //!< sinusoidal oscillator
    kSaw,               //!< saw oscillator
    kTriang,            //!< triangular oscillator
    kNoLfo,             //!< no oscillator

    kNumOfLfoTypes
};

/*! phase offset between channels for modulated effects */
enum zfxPhaseOffsetBetweenChannels_t
{
    k0Degree,           //!< no phase difference between channels
    k90Degree,          //!< phase difference = pi/2
    k180Degree,         //!< phase difference = pi
    k270Degree,         //!< phase difference = 3pi/2

    kNumOfPhaseOffsets
};

/*! error return values */
enum zfxError_t
{
    kNoError,                       //!< no error occurred
    kMemError,                      //!< memory allocation failed
    kInvalidFunctionParamError,     //!< one or more function parameters are not valid
    kUnknownError,                  //!< unknown error occurred

    kNumOfErrors
};

enum zfxVersion_t
{
    kzfxVersionMajor,
    kzfxVersionMinor,
    kzfxVersionPatch,
    kzfxVersionBuild,

    kNumzfxVersionInts
};

const int  zfxGetVersion (const zfxVersion_t eVersionIdx);
const char* zfxGetBuildDate ();

#endif // #if !defined(__libfxpack_HEADER_INCLUDED__)



