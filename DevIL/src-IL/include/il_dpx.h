//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 02/26/2009
//
// Filename: src-IL/include/il_dpx.h
//
// Description: Reads from a Digital Picture Exchange (.dpx) file.
//				Specifications for this format were	found at
//				http://www.cineon.com/ff_draft.php.
//
//-----------------------------------------------------------------------------


#ifndef DPX_H
#define DPX_H

#include "il_internal.h"


#ifdef _WIN32
#pragma pack(push, packed_struct, 1)
#endif
typedef struct R32
{
	ILubyte	r, g, b, a;
} IL_PACKSTRUCT R32;
#ifdef _WIN32
#pragma pack(pop, packed_struct)
#endif


typedef struct DPX_FILE_INFO
{
    ILuint	magic_num;        /* magic number 0x53445058 (SDPX) or 0x58504453 (XPDS) */
    ILuint	offset;           /* offset to image data in bytes */
    ILbyte	vers[8];          /* which header format version is being used (v1.0)*/
    ILuint	file_size;        /* file size in bytes */
    ILuint	ditto_key;        /* read time short cut - 0 = same, 1 = new */
    ILuint	gen_hdr_size;     /* generic header length in bytes */
    ILuint	ind_hdr_size;     /* industry header length in bytes */
    ILuint	user_data_size;   /* user-defined data length in bytes */
    ILbyte	file_name[100];   /* iamge file name */
    ILbyte	create_time[24];  /* file creation date "yyyy:mm:dd:hh:mm:ss:LTZ" */
    ILbyte	creator[100];     /* file creator's name */
    ILbyte	project[200];     /* project name */
    ILbyte	copyright[200];   /* right to use or copyright info */
    ILuint	key;              /* encryption ( FFFFFFFF = unencrypted ) */
    ILbyte	Reserved[104];    /* reserved field TBD (need to pad) */
} DPX_FILE_INFO;

typedef struct DPX_IMAGE_ELEMENT
{
    ILuint    DataSign;        /* data sign (0 = unsigned, 1 = signed ) */
			 /* "Core set images are unsigned" */
    ILuint    RefLowData;     /* reference low data code value */
    R32    ref_low_quantity; /* reference low quantity represented */
    ILuint    ref_high_data;    /* reference high data code value */
    R32    ref_high_quantity;/* reference high quantity represented */
    ILubyte     descriptor;       /* descriptor for image element */
    ILubyte     transfer;         /* transfer characteristics for element */
    ILubyte     colorimetric;     /* colormetric specification for element */
    ILubyte     bit_size;         /* bit size for element */
	ILushort    packing;          /* packing for element */
    ILushort    encoding;         /* encoding for element */
    ILuint    data_offset;      /* offset to data of element */
    ILuint    eol_padding;      /* end of line padding used in element */
    ILuint    eo_image_padding; /* end of image padding used in element */
    ILbyte  description[32];  /* description of element */
} DPX_IMAGE_ELEMENT;          /* NOTE THERE ARE EIGHT OF THESE */


typedef struct DPX_IMAGE_INFO
{
    ILushort	Orientation;          /* image orientation */
    ILushort	NumElements;       /* number of image elements */
    ILuint		Width;      /* or x value */
    ILuint		Height;  /* or y value, per element */
	DPX_IMAGE_ELEMENT	ImageElement[8];
    ILubyte		reserved[52];             /* reserved for future use (padding) */
} DPX_IMAGE_INFO;


typedef struct DPX_IMAGE_ORIENT
{
    ILuint   x_offset;               /* X offset */
    ILuint   y_offset;               /* Y offset */
    R32   x_center;               /* X center */
    R32   y_center;               /* Y center */
    ILuint   x_orig_size;            /* X original size */
    ILuint   y_orig_size;            /* Y original size */
    ILbyte file_name[100];         /* source image file name */
    ILbyte creation_time[24];      /* source image creation date and time */
    ILbyte input_dev[32];          /* input device name */
    ILbyte input_serial[32];       /* input device serial number */
    ILushort   border[4];              /* border validity (XL, XR, YT, YB) */
    ILuint   pixel_aspect[2];        /* pixel aspect ratio (H:V) */
    ILubyte    reserved[28];           /* reserved for future use (padding) */
} DPX_IMAGE_ORIENT;


typedef struct DPX_MOTION_PICTURE_HEAD
{
    ILbyte film_mfg_id[2];    /* film manufacturer ID code (2 digits from film edge code) */
    ILbyte film_type[2];      /* file type (2 digits from film edge code) */
    ILbyte offset[2];         /* offset in perfs (2 digits from film edge code)*/
    ILbyte prefix[6];         /* prefix (6 digits from film edge code) */
    ILbyte count[4];          /* count (4 digits from film edge code)*/
    ILbyte format[32];        /* format (i.e. academy) */
    ILuint   frame_position;    /* frame position in sequence */
    ILuint   sequence_len;      /* sequence length in frames */
    ILuint   held_count;        /* held count (1 = default) */
    R32   frame_rate;        /* frame rate of original in frames/sec */
    R32   shutter_angle;     /* shutter angle of camera in degrees */
    ILbyte frame_id[32];      /* frame identification (i.e. keyframe) */
    ILbyte slate_info[100];   /* slate information */
    ILubyte    reserved[56];      /* reserved for future use (padding) */
} DPX_MOTION_PICTURE_HEAD;


typedef struct DPX_TELEVISION_HEAD
{
    ILuint tim_code;            /* SMPTE time code */
    ILuint userBits;            /* SMPTE user bits */
    ILubyte  interlace;           /* interlace ( 0 = noninterlaced, 1 = 2:1 interlace*/
    ILubyte  field_num;           /* field number */
    ILubyte  video_signal;        /* video signal standard (table 4)*/
    ILubyte  unused;              /* used for byte alignment only */
    R32 hor_sample_rate;     /* horizontal sampling rate in Hz */
    R32 ver_sample_rate;     /* vertical sampling rate in Hz */
    R32 frame_rate;          /* temporal sampling rate or frame rate in Hz */
    R32 time_offset;         /* time offset from sync to first pixel */
    R32 gamma;               /* gamma value */
    R32 black_level;         /* black level code value */
    R32 black_gain;          /* black gain */
    R32 break_point;         /* breakpoint */
    R32 white_level;         /* reference white level code value */
    R32 integration_times;   /* integration time(s) */
    ILubyte  reserved[76];        /* reserved for future use (padding) */
} DPX_TELEVISION_HEAD;


// For checking and reading
ILboolean iIsValidDpx(void);
//ILboolean iCheckDpx(DPXHEAD *Header);
ILboolean iLoadDpxInternal(void);

#endif//PCX_H
