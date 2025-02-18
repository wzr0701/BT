/* ***** BEGIN LICENSE BLOCK ***** 
 * Version: RCSL 1.0/RPSL 1.0 
 *  
 * Portions Copyright (c) 1995-2002 RealNetworks, Inc. All Rights Reserved. 
 *      
 * The contents of this file, and the files included with this file, are 
 * subject to the current version of the RealNetworks Public Source License 
 * Version 1.0 (the "RPSL") available at 
 * http://www.helixcommunity.org/content/rpsl unless you have licensed 
 * the file under the RealNetworks Community Source License Version 1.0 
 * (the "RCSL") available at http://www.helixcommunity.org/content/rcsl, 
 * in which case the RCSL will apply. You may also obtain the license terms 
 * directly from RealNetworks.  You may not use this file except in 
 * compliance with the RPSL or, if you have a valid RCSL with RealNetworks 
 * applicable to this file, the RCSL.  Please see the applicable RPSL or 
 * RCSL for the rights, obligations and limitations governing use of the 
 * contents of the file.  
 *  
 * This file is part of the Helix DNA Technology. RealNetworks is the 
 * developer of the Original Code and owns the copyrights in the portions 
 * it created. 
 *  
 * This file, and the files included with this file, is distributed and made 
 * available on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER 
 * EXPRESS OR IMPLIED, AND REALNETWORKS HEREBY DISCLAIMS ALL SUCH WARRANTIES, 
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS 
 * FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT. 
 * 
 * Technology Compatibility Kit Test Suite(s) Location: 
 *    http://www.helixcommunity.org/content/tck 
 * 
 * Contributor(s): 
 *  
 * ***** END LICENSE BLOCK ***** */ 

/**************************************************************************************
 * Fixed-point MP3 decoder
 * Jon Recker (jrecker@real.com), Ken Cooke (kenc@real.com)
 * June 2003
 *
 * mp3dec.h - public C API for MP3 decoder
 **************************************************************************************/

#ifndef _MP3DEC_H
#define _MP3DEC_H
#include "driver_beken_includes.h"
#if (CONFIG_APP_MP3PLAYER == 1)
// Must be moved KJ
//#define __GNUC__
#define ARM
//__attribute__((section("mp3call")))

#if defined(_WIN32) && !defined(_WIN32_WCE)
#
#elif defined(_WIN32) && defined(_WIN32_WCE) && defined(ARM)
#
#elif defined(_WIN32) && defined(WINCE_EMULATOR)
#
#elif defined(ARM_ADS)
#
#elif defined(_SYMBIAN) && defined(__WINS__)	/* Symbian emulator for Ix86 */
#
#elif defined(__GNUC__) && defined(ARM)
#
#elif defined(__GNUC__) && defined(__i386__)
#
#elif defined(_OPENWAVE_SIMULATOR) || defined(_OPENWAVE_ARMULATOR)
#
#else
#error No platform defined. See valid options in mp3dec.h
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* determining MAINBUF_SIZE:
 *   max mainDataBegin = (2^9 - 1) bytes (since 9-bit offset) = 511
 *   max nSlots (concatenated with mainDataBegin bytes from before) = 1440 - 9 - 4 + 1 = 1428
 *   511 + 1428 = 1939, round up to 1940 (4-byte align)
 */

#define MAINBUF_SIZE	1940

#define BIT_RESVOR_SIZE   (512) 
#define READBUF_SIZE      (1940)    
#define MAX_DECODE_ERR_TIMES      2000   

#define MAX_NGRAN		2		/* max granules */
#define MAX_NCHAN		2		/* max channels */
#define MAX_NSAMP		576		/* max samples per channel, per granule */

/* map to 0,1,2 to make table indexing easier */
typedef enum {
	MPEG1 =  0,
	MPEG2 =  1,
	MPEG25 = 2
} MPEGVersion;

#if CALC_PLAY_TIME
enum {
    TPYE_NONE =    0,
    CBR_FILE,
    VBR_XING_FILE,
    VBR_VBRI_FILE
} FILETPYE;
#define PRECISION   10000 
#endif

typedef void *HMP3Decoder;


enum {
	ERR_MP3_NONE =                  0,		  //没有一个错误
	ERR_MP3_INDATA_UNDERFLOW =     1,		  //在数据下溢
	ERR_MP3_MAINDATA_UNDERFLOW =   2,		  //主数据下溢
	ERR_MP3_FREE_BITRATE_SYNC =    3,		  //
	ERR_MP3_OUT_OF_MEMORY =	       4,		  //
	ERR_MP3_NULL_POINTER =         5,		  //
	ERR_MP3_INVALID_FRAMEHEADER =  6,		  //
	ERR_MP3_INVALID_SIDEINFO =     7,		  //
	ERR_MP3_INVALID_SCALEFACT =    8,		  //
	ERR_MP3_INVALID_HUFFCODES =    9,		  //
	ERR_MP3_INVALID_DEQUANTIZE =   10,		  //
	ERR_MP3_INVALID_IMDCT =        11,		  //
	ERR_MP3_INVALID_SUBBAND =      12,		  //
	ERR_MP3_READ_DATA_FAILED =     13,
	ERR_MP3_DECODE_MAX_ERR = 14,
	ERR_MP3_FILE_END = 20,	//此值不能修改,值与FR_FILE_END要一致
	ERR_UNKNOWN =                  -9999
};

enum {
	MP3_DECODE_NONE =               0,		  //还没有开始解码
	MP3_DECODE_FIND_ID3_INFO    =   1,
	MP3_DECODE_FIND_SYNC_WORD   =   2,
	MP3_DECODE_HEADER_SIDEINFO  =   3,
	MP3_DECODE_COPY_MAIN_DATE   =   4,

	
	MP3_DECODE_CALU_CRUCIAL_DAT =   5,
	MP3_DECODE_CALU_SUBBAND     =   6,
	MP3_DECODE_WAITING_FOR_BUFF =   7,
	

	MP3_DECODE_UNKNOW =             9999
};

typedef struct _MP3FrameInfo {
	int bitrate;			 //  码率
	int nChans;				 //声道
	int samprate;			 //采样率
	int bitsPerSample;		 //位	
	int outputSamps;		 //	PCM数据个数
	int layer;				 //层
	int version;			 //版本
} MP3FrameInfo;

/* public API */
HMP3Decoder MP3_CALL MP3InitDecoder(void);
void MP3_CALL MP3FreeDecoder(HMP3Decoder hMP3Decoder);
int MP3_CALL MP3Decode(HMP3Decoder hMP3Decoder, short *outbuf, int *pcm_size);
void MP3_CALL MP3GetLastFrameInfo(HMP3Decoder hMP3Decoder, MP3FrameInfo *mp3FrameInfo);
int MP3_CALL MP3GetNextFrameInfo(HMP3Decoder hMP3Decoder, MP3FrameInfo *mp3FrameInfo, unsigned char *buf);
int MP3_CALL MP3FindSyncWord(HMP3Decoder hMP3Decoder,unsigned char *buf, int nBytes);
int MP3_CALL RB_MP3FindSyncWord(HMP3Decoder hMP3Decoder,driver_ringbuff_t *rb,uint8 *buf,int16 BytesLeft);
int16 MP3_CALL MP3FixDiv(int32 num,int32 denum);

#if CALC_PLAY_TIME    
void MP3_Restore_Useful_data_len(int len);
void MP3_Calc_Play_Time(HMP3Decoder hMP3Decoder, MP3FrameInfo *mp3FrameInfo);
int MP3_Calc_Current_Play_Time(HMP3Decoder hMP3Decoder);
void MP3_Show_Play_Time(int time);
#endif

#ifdef __cplusplus
}
#endif
#endif /* CONFIG_APP_MP3PLAYER */
#endif	/* _MP3DEC_H */
