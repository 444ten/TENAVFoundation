////////////////////////////////////////////////////////////////////////////////////
//     /*! \file tONaRT.h: \brief interface of the CtONaRT_If class. */
//
//  Copyright (c) 2004-2013
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
//  $RCSfile: tONaRT.h,v $
//  $Author: lerch $
//  $Date: 2008-10-01 12:31:09 $
//
//
//
////////////////////////////////////////////////////////////////////////////////////


#if !defined(__tONaRT_IF_HEADER_INCLUDED__)
#define __tONaRT_IF_HEADER_INCLUDED__

#ifndef DLLAPI
#ifdef WIN32
#define DLLAPI __declspec(dllexport)
#else
#define DLLAPI
#endif
#endif //DLLAPI


class CtONaRTResultIf;

/*!
CLASS
    

    This class provides the interface for the [tONaRT] key detection.


    The key detection is stream based and processes a block of data at each time.
    After successful processing, the results can be obtained with a defined structure.
*/
class CtONaRT_If
{
public:

    //! structure that holds the results
    typedef struct Results_t_tag
    {
        char    acKeyName [1024];           //!< string containing the name of the detected key (e.g. "A  Maj")
        int     iKeyIdx;                    //!< index of detected key, possible values are in the range from 0...24; indices 0..11 mark the keys "A Maj" to "G# Maj", indices 12..23 mark the keys "a min" to "g# min"
        float   fKeyDetectionProbability;   //!< probability of this detection (experimental!)
        float   fStandardPitchFrequency;    //!< frequency of concert pitch in Hz

        char    ac2ndGuessKeyName [1024];
        int     i2ndGuessKeyIdx;
        float   f2ndGuessKeyDetectProb;
    } Results_t;

    CtONaRT_If () {};
    virtual ~CtONaRT_If () {};


    enum Version_t
    {
        kMajor,
        kMinor,
        kPatch,
        kBuild,

        kNumVersionInts
    };

    static const int  GetVersion (const Version_t eVersionIdx);
    static const char* GetBuildDate ();

    /*!
     * creates a new instance of [tONaRT] key detection
     *
     * @param pCInstancePointer : handle to the new instance
     * @param iSampleRate : sample rate of audio signal to be analyzed (in Hz)
     * @param iNumberOfChannels : number of channels of audio signal to be analyzed
     * @param fStartPitch : initialization frequency of concert/standard pitch (in Hz, optional)
     * @param bUseFastProcessing : enable fast processing mode (less accurate)
     *
     * @return static int  : 0 when no error
     */
    static DLLAPI int CreateInstance (CtONaRT_If*& pCInstancePointer, int iSampleRate, int iNumberOfChannels, float fStartPitch = 0, bool bUseFastProcessing = false);

    /*!
     * destroys an instance of [tONaRT] key detection
     *
     * @param pCInstancePointer : handle to the instance to be destroyed
     *
     * @return static int  : 0 when no error
     */
    static DLLAPI int DestroyInstance (CtONaRT_If*& pCInstancePointer);


    /*!
     * does the key analysis in blocks
     * BE CAREFUL: input data is being modified, so use process as last call in process chain
     *
     * @param *pfInputBufferInterleaved : pointer to interleaved audio data in floating point format
     * @param iNumberOfFrames : number of frames per block (a frame equals one sample for mono signals and two samples for stereo signals), must not be higher than 16384
     *
     * @return virtual int  : 0 when no error
     */
    DLLAPI virtual int Process (float *pfInputBufferInterleaved, int iNumberOfFrames) = 0;


    /*!
    * alternative interface, does the key analysis in blocks
    * BE CAREFUL: input data is being modified, so use process as last call in process chain
    *
    * @param **ppfInputBuffer : double pointer to separate audio data buffers in floating point format
    * @param iNumberOfFrames : number of frames per block (a frame equals one sample for mono signals and two samples for stereo signals), must not be higher than 16384
    *
    * @return virtual int  : 0 when no error
    */
    DLLAPI virtual int Process (float **ppfInputBuffer, int iNumberOfFrames) = 0;


    /*!
     * writes the results until this point of the analysis (may be unreliable!)
     *
     * @param *pstResults : pointer to structure for results
     *
     * @return virtual int  : 0 when no error
     */
    DLLAPI virtual int GetTempResults (Results_t *pstResults, float *pfPitchProbability = 0) = 0;

    /*!
     * signals the library that no more input data is expected and writes the overall results
     *
     * @param *pstResults : pointer to structure for results
     * @param *pfPitchProbability : optional pointer to 12-dimensional pitch energy
     * @param *pfKeyProbability : optional pointer 24 dimensional key probability vector
     *
     * @return virtual int  : 0 when no error
     */
    DLLAPI virtual int GetOverallResults (Results_t *pstResults, float *pfPitchProbability = 0, float *pfKeyProbability = 0) = 0;

    // this function is experimental - do not use without contacting support before!
    DLLAPI virtual int GetResult (CtONaRTResultIf *pCResult) = 0;

};
#endif // #if !defined(__tONaRT_IF_HEADER_INCLUDED__)

