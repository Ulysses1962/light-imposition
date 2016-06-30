/*
** AcroColorExpT.h
**
** Types, macros, structures, etc. required to use the SpringTide Host Function Table.
**
Copyright (C) 2002-2003 Adobe Systems Inc.  All Rights Reserved.
**
*/

#ifndef _H_AcroColorExpT
#define _H_AcroColorExpT

#include "Environ.h"

#if PLUGIN
#include "CoreExpT.h"
#include "ASExpT.h"
#else
/* 605040: this should not be included here: #include "ASEnv.h" */
#endif /* PLUGIN */


#if PRAGMA_STRUCT_ALIGN
	#if __MWERKS__
		#pragma options align=power_gcc
	#elif __GNUC__
		#pragma options align=power
	#else
		#error
	#endif
#endif

#ifndef FOUR_CHAR_CODE
#define FOUR_CHAR_CODE(x)			(x)
#endif

#ifdef __cplusplus
extern "C" {
#endif


#define AcroColorHFTNAME "AcroColorHFT"

#ifdef __cplusplus

/**
	A string value type for use with AcroColor functions.
	@see ACEngineInfo
	@see ACGetSettingsString
	@see ACMakeCalGray
	@see ACMakeCalLab
	@see ACMakeCalRGB
	@see ACMakeString
	@see ACPresetFileToName
	@see ACProfileDescription
	@see ACProfileFromDescription
	@see ACProfileListItemDescription
	@see ACStringASCII
	@see ACStringLocalized
	@see ACStringUnicode
	@see ACUnReferenceString
*/
class ACString;

typedef ACString *AC_String;

#else

typedef struct ACString *AC_String;

#endif

#ifdef __cplusplus

/**
	An opaque object representing an list of device color profiles.
	@see ACMakeProfileList
	@see ACProfileListCount
	@see ACProfileListItemCode
	@see ACProfileListItemDescription
	@see ACUnReferenceProfileList
*/
class ACProfileList;
typedef ACProfileList *AC_ProfileList;

#else

typedef struct ACProfileList *AC_ProfileList;

#endif

#ifdef __cplusplus

/** An opaque object representing a color tranformation.
	@see ACApplyTransform
	@see ACMakeColorTransform
	@see ACUnReferenceTranform
*/
class ACTransform;

typedef ACTransform *AC_Transform;

#else

typedef struct ACTransform *AC_Transform;

#endif

#ifdef __cplusplus

/**
	An opaque object representing the settings for the AcroColor engine (ACE).
	@see ACGetSettingsProfile
	@see ACGetSettingsString
	@see ACGetSettingsUnsigned32
	@see ACLoadSettings
	@see ACMakeSettings
	@see ACUnReferenceSettings
*/
class ACSettings;

typedef ACSettings *AC_Settings;

#else

typedef struct ACSettings *AC_Settings;

#endif

#ifdef __cplusplus

/**
	An opaque object representing a device color profile.
	@see ACGetSettingsProfile
	@see ACGetWorkingSpaceProfile
	@see ACMakeBufferProfile
	@see ACMakeCalGray
	@see ACMakeCalLab
	@see ACMakeCalRGB
	@see ACMakeColorTransform
	@see ACMonitorProfile
	@see ACProfileColorSpace
	@see ACProfileData
	@see ACProfileDescription
	@see ACProfileFromCode
	@see ACProfileFromDescription
	@see ACProfileSize
	@see ACProfilesMatch
	@see ACUnReferenceProfile
*/
class ACProfile;

typedef ACProfile *AC_Profile;

#else

typedef struct ACProfile *AC_Profile;

#endif

#ifdef __cplusplus
/**
	An opaque object representing a preset list. A preset list is a list of predefined color settings
	that specifies the source and destination working color profiles to be used for color
	conversion.
	@see ACMakePresetList
	@see ACPresetListCount
	@see ACPresetListItemFile
	@see ACUnReferencePresetList
*/
class ACPresetList;

typedef ACPresetList *AC_PresetList;

#else

typedef struct ACPresetList *AC_PresetList;

#endif


/** Constants that specify types of device profiles to include in a profile list. 
	@see ACMakeProfileList
*/
typedef enum
{

	/** Standard ("recommended") RGB profiles.  These profiles are always
	   bi-directional. */
	AC_Selector_RGB_Standard				= FOUR_CHAR_CODE ('rStd'),
	

	/** RGB profiles that can be used as a source.  These profiles may or
	   may not be useable as a destination.  Does not include profiles selected
	   by AC_Selector_RGB_Standard. */
    AC_Selector_RGB_OtherInputCapable		= FOUR_CHAR_CODE ('rInp'),

	/** RGB profiles that can be used as a destination.  These profiles are
	   also useable as a source.  Does not include profiles selected
	   by AC_Selector_RGB_Standard. */
	AC_Selector_RGB_OtherOutputCapable		= FOUR_CHAR_CODE ('rOut'),

	/** Standard ("recommended") CMYK profiles that can be used as a source.
	   These profiles may or may not be useable as a destination. */
	AC_Selector_CMYK_StandardInput			= FOUR_CHAR_CODE ('cSIn'),

	/** Standard ("recommended") CMYK profiles that can be used as a destination.
	   These profiles are also useable as a source. */
	AC_Selector_CMYK_StandardOutput		= FOUR_CHAR_CODE ('cStd'),

	/** CMYK profiles that can be used as a source.  These profiles may or
	   may not be useable as a destination.  Does not include profiles selected
	   by AC_Selector_CMYK_StandardInput or AC_Selector_CMYK_StandardOutput. */
	AC_Selector_CMYK_OtherInputCapable		= FOUR_CHAR_CODE ('cInp'),

    /** CMYK profiles that can be used as a destination.  These profiles are
	   also useable as a source.  Does not include profiles selected
	   by AC_Selector_CMYK_StandardOutput. */
	AC_Selector_CMYK_OtherOutputCapable	= FOUR_CHAR_CODE ('cOut'),

	/** Standard ("recommended") grayscale profiles.  These profiles are always
	   bi-directional. */
	AC_Selector_Gray_Standard				= FOUR_CHAR_CODE ('gStd'),

	/** Grayscale profiles that can be used as a source.  These profiles may
	   or may not be useable as a destination.  Does not include profiles selected
	   by AC_Selector_Gray_Standard. */
	AC_Selector_Gray_OtherInputCapable		= FOUR_CHAR_CODE ('gInp'),

    /** Grayscale profiles that can be used as a destination.  These profiles
       are also useable as a source.  Does not include profiles selected
	   by AC_Selector_Gray_Standard. */
	AC_Selector_Gray_OtherOutputCapable	= FOUR_CHAR_CODE ('gOut'),

	/** Standard dot gain profiles.  Uses by Photoshop to represent a single 
	   ink's dot gain curve.  Stored as an ICC gray output profile. */
	AC_Selector_DotGain_Standard			= FOUR_CHAR_CODE ('dStd'),

	/** Other grayscale printer profiles.  Does not include profiles selected by
	   AC_Selector_DotGain_Standard, and does not include grayscale display
	   profiles. */
	AC_Selector_DotGain_Other				= FOUR_CHAR_CODE ('dOth'),

	/** PhotoYCC profiles that can be used as a source. */
	AC_Selector_PhotoYCC_InputCapable		= FOUR_CHAR_CODE ('iYCC'),

	/** Force enum to be 32-bits wide. */
    AC_Selector_MaxEnum			 		= 0xFFFFFFFFL
	} AC_SelectorCode;


/** 
	Error codes returned by AcroColor functions.
*/
typedef enum
{

	/** No error. */
	AC_Error_None				= 0,	

	/** Other error. */
	AC_Error_General			= FOUR_CHAR_CODE ('gen '),
	
	/** Bad parameters to an API call. */
	AC_Error_Param				= FOUR_CHAR_CODE ('parm'),

	/** Application and ACE library mismatch. */
	AC_Error_Version			= FOUR_CHAR_CODE ('ver '),

	/** The user aborted the operation.  Returned by ACE when the
	   client progress callback returns false. */
	AC_Error_UserAbort			= FOUR_CHAR_CODE ('abrt'),

	/** Out of memory. */
	AC_Error_Memory			= FOUR_CHAR_CODE ('memF'),
	
	/** Out of stack stace. */
	AC_Error_StackFull			= FOUR_CHAR_CODE ('stkF'),

	/** Client callback ran out of scratch space. */
	AC_Error_ScratchFull		= FOUR_CHAR_CODE ('scrF'),
		
	/** String does not fit in supplied buffer. */
	AC_Error_StringOverflow	= FOUR_CHAR_CODE ('strO'),

	/** String does not contain ASCII data. */
	AC_Error_NoASCII			= FOUR_CHAR_CODE ('noA '),

	/** String does not contain Unicode data. */
	AC_Error_NoUnicode			= FOUR_CHAR_CODE ('noU '),
	
	/** String does not contain localized data. */
	AC_Error_NoLocalized		= FOUR_CHAR_CODE ('noL '),

	/** Data is not correctly byte aligned. */
	AC_Error_BadAlignment		= FOUR_CHAR_CODE ('alig'),

	/** Invalid profile description. */
	AC_Error_BadDescription	= FOUR_CHAR_CODE ('bDes'),

	/** Unable to concatenate transforms. */
	AC_Error_BadConcat			= FOUR_CHAR_CODE ('bCat'),

	/** Unable to merge transforms. */
	AC_Error_BadMerge			= FOUR_CHAR_CODE ('bMrg'),

	/** Invalid profile. */
	AC_Error_BadProfile		= FOUR_CHAR_CODE ('bPro'),

	/** Unsupported CMS. */
	AC_Error_UnsupCMS			= FOUR_CHAR_CODE ('uCMS'),

	/** Unsupported ACE option. */
	AC_Error_UnsupOption		= FOUR_CHAR_CODE ('uOpt'),

	/** Unsupported packing code. */
	AC_Error_UnsupPacking		= FOUR_CHAR_CODE ('uPac'),

	/** Unsupported profile version. */
	AC_Error_UnsupProfile		= FOUR_CHAR_CODE ('uPro'),

	/** Unsupported profile code. */
	AC_Error_UnsupProfileCode	= FOUR_CHAR_CODE ('uPrC'),

	/** Unsupported color space. */
	AC_Error_UnsupSpace		= FOUR_CHAR_CODE ('uSpc'),
	
	/** Unsupported query code. */
	AC_Error_UnsupQuery		= FOUR_CHAR_CODE ('uQry'),
	
	/** A profile was missing from the disk. */
	AC_Error_MissingProfile	= FOUR_CHAR_CODE ('misP'),

	/** The profile on disk has been modified. */
	AC_Error_ModifiedProfile	= FOUR_CHAR_CODE ('modP'),

	/** File is missing from disk. */
	AC_Error_FileNotFound		= FOUR_CHAR_CODE ('fnf '),	

	/** End of file error. */
	AC_Error_EOF				= FOUR_CHAR_CODE ('eof '),

	/** File locked error. */
	AC_Error_FileLocked		= FOUR_CHAR_CODE ('flck'),

	/** Disk I/O error. */
	AC_Error_DiskIO			= FOUR_CHAR_CODE ('io  '),

	/** A problem using ColorSync. */
	AC_Error_ColorSync			= FOUR_CHAR_CODE ('csE '),

	/** A problem using ICM. */
	AC_Error_ICM				= FOUR_CHAR_CODE ('icmE'),

	/** The color settings does not contain this key. */
	AC_Error_MissingKey		= FOUR_CHAR_CODE ('mKey'),
	
	/** The color settings file is invalid. */
	AC_Error_InvalidSettings	= FOUR_CHAR_CODE ('iSet'),
	
	/** The color settings file is an incompatible version. */
	AC_Error_SettingsVersion	= FOUR_CHAR_CODE ('vSet'),

	/** The function is not implemented (subsetted library). */
	AC_Error_NotImplemented	= FOUR_CHAR_CODE ('nImp'),

	/** Force enum to be 32-bits wide. */
    AC_Error_MaxEnum			= 0xFFFFFFFFL
	
} AC_Error;


/** Constants that describe the type of a device color profile. 
	@see ACProfileFromCode
	@see ACProfileListItemCode
*/
typedef enum
	{
	
	/** A null result, indication that a profile is not a built-in profile. */
	AC_Profile_Null				= 0,
	
	/** Adobe's standard Lab profile.  It has a white point of D50, and
	   exactly matches the ICC's Lab PCS space. */
	AC_Profile_Lab_D50				= FOUR_CHAR_CODE ('LD50'),
	
	/** An XYZ profile that exactly matches the ICC's XYZ PCS space. */
	AC_Profile_PCS_XYZ				= FOUR_CHAR_CODE ('pXYZ'),	

	/** An XYZ profile with a "flat" white point encoding (X = Y = Z = 1.0).
	   Photoshop uses this as an intermediate space in its display loop. */
	AC_Profile_Flat_XYZ			= FOUR_CHAR_CODE ('fXYZ'),

	/** HP's sRGB profile. The default Windows monitor profile. */
	AC_Profile_sRGB				= FOUR_CHAR_CODE ('sRGB'),
       
	/** Default RGB space using by Photoshop 4.0 and earlier. The
	   default Macintosh monitor profile.  */
	AC_Profile_AppleRGB			= FOUR_CHAR_CODE ('aRGB'),
	
	/** A wider gamut RGB space recommended by Adobe. */
	AC_Profile_AdobeRGB1998		= FOUR_CHAR_CODE ('AS98'),
       
	/** A simplified version of Radius' ColorMatch RGB space, without
	   Radius' non-zero black point. */
	AC_Profile_ColorMatchRGB		= FOUR_CHAR_CODE ('cmat'),   

    /** Grayscale display profiles with various gammas (18). */
    AC_Profile_Gamma18 			= FOUR_CHAR_CODE ('GG18'),

    /** Grayscale display profiles with various gammas (22). */
    AC_Profile_Gamma22 			= FOUR_CHAR_CODE ('GG22'),
    
    /** Grayscale printer profiles with various dot gains (10). */
    AC_Profile_DotGain10 			= FOUR_CHAR_CODE ('DG10'),

    /** Grayscale printer profiles with various dot gains (15). */
    AC_Profile_DotGain15 			= FOUR_CHAR_CODE ('DG15'),

    /** Grayscale printer profiles with various dot gains (20). */
    AC_Profile_DotGain20 			= FOUR_CHAR_CODE ('DG20'),

    /** Grayscale printer profiles with various dot gains (25). */
    AC_Profile_DotGain25 			= FOUR_CHAR_CODE ('DG25'),

    /** Grayscale printer profiles with various dot gains (30). */
    AC_Profile_DotGain30 			= FOUR_CHAR_CODE ('DG30'),

    /** The system "Monitor RGB" profile, which is the same profile as that
       returned by AC_MainMonitorProfile. */
    AC_Profile_MonitorRGB			= FOUR_CHAR_CODE ('mRGB'),

    /** The system default profiles for RGB color space.  (Currently a ColorSync 3.0 only feature.) */
    AC_Profile_SystemRGB			= FOUR_CHAR_CODE ('sysR'),

    /** The system default profiles for CMYK color space.  (Currently a ColorSync 3.0 only feature.) */
    AC_Profile_SystemCMYK			= FOUR_CHAR_CODE ('sysC'),

    /** The system default profiles for Gray color space.  (Currently a ColorSync 3.0 only feature.) */
    AC_Profile_SystemGray			= FOUR_CHAR_CODE ('sysG'),
       
    /** The system default profiles for input device type.  (Currently a ColorSync 3.0 only feature.) */
    AC_Profile_SystemInput			= FOUR_CHAR_CODE ('sysI'),

    /** The system default profiles for output device type.  (Currently a ColorSync 3.0 only feature.) */
    AC_Profile_SystemOutput		= FOUR_CHAR_CODE ('sysO'),
  
    /** The system default profiles for proofer device type.  (Currently a ColorSync 3.0 only feature.) */
    AC_Profile_SystemProofer		= FOUR_CHAR_CODE ('sysP'),
    
    /** The application working (RGB) color space profiles.  (For use as
       abstact values only, since ACE does not keep track of these
       profiles, and thus cannot make profiles from these codes.) */
    AC_Profile_WorkingRGB			= FOUR_CHAR_CODE ('wRGB'),

    /** The application working (CMYK) color space profiles.  (For use as
       abstact values only, since ACE does not keep track of these
       profiles, and thus cannot make profiles from these codes.) */
    AC_Profile_WorkingCMYK			= FOUR_CHAR_CODE ('wCMY'),

    /** The application working (gray) color space profiles.  (For use as
       abstact values only, since ACE does not keep track of these
       profiles, and thus cannot make profiles from these codes.) */
    AC_Profile_WorkingGray			= FOUR_CHAR_CODE ('wGry'),
       
	/** Force enum to be 32-bits wide. */
	AC_Profile_MaxEnum			 	= 0xFFFFFFFFL
	
} AC_ProfileCode;


/** 
	Constants that specify the packing used in a color transformation.
*/
typedef enum
{
	
	/** 8 bit RGB (or grayscale destination), with a leading pad byte.
	   When grayscale is output in this format, the gray value is replicated
	   into to the R, G, and B values.
	   R, G, B = 0 is black.
	   R, G, B = 255 is white.
	   Data must be 32-bit aligned. */
    AC_Packing_pRGB8			= FOUR_CHAR_CODE ('prgb'),

    /** Same as AC_Packing_pRGB8, without the leading pad byte.
       Data need only be 8 bit aligned. */
    AC_Packing_RGB8			= FOUR_CHAR_CODE ('rgb '),  

    /** 15+ bit RGB (or grayscale destination), with a leading pad word.
	   When grayscale is output in this format, the gray value is replicated
	   into to the R, G, and B values.
       R, G, B = 0 is black.
       R, G, B = 32768 is white.
       Values greater than 32768 are invalid.
 	   Data must be 64-bit aligned. */
   AC_Packing_pRGB15			= FOUR_CHAR_CODE ('PRGB'),
   
	/** 8 bit CMYK.
	   C, M, Y, K = 0 is 100% ink.
	   C, M, Y, K = 255 is 0% ink.
	   Data must be 32-bit aligned. */
	AC_Packing_CMYK8_Black0	= FOUR_CHAR_CODE ('cmyk'),
    
	/** Same as AC_Packing_CMYK8_Black0 with inverse encoding. */
	AC_Packing_CMYK8_White0	= FOUR_CHAR_CODE ('cmyw'),
    
    /** 15+ bit CMYK.
	   C, M, Y, K = 0 is 100% ink.
	   C, M, Y, K = 32768 is 0% ink.
       Values greater than 32768 are invalid.
 	   Data must be 64-bit aligned. */
    AC_Packing_CMYK15_Black0	= FOUR_CHAR_CODE ('CMYK'), 

    /** 8 bit LAB, with a leading pad byte.
       L = 0      -> L* = 0.0
       L = 255 	  -> L* = 100.0
       a, b = 0   -> a*, b* = -128.0
       a, b = 128 -> a*, b* = 0.0
       a, b = 255 -> a*, b* = +127.0
	   Data must be 32-bit aligned. */
    AC_Packing_pLab8			= FOUR_CHAR_CODE ('plab'),

    /** Same as AC_Packing_pLab8, without the leading pad byte.
       Data need only be 8 bit aligned. */
    AC_Packing_Lab8			= FOUR_CHAR_CODE ('lab '),
    
    /** 15+ bit LAB, with a leading pad word.
       L = 0        -> L* = 0.0
       L = 32768    -> L* = 100.0
       a, b = 0     -> a*, b* = -128.0
       a, b = 16384 -> a*, b* = 0.0
       a, b = 32768 -> a*, b* = +128.0
       Values greater than 32768 are invalid.
	   Data must be 64-bit aligned. */
    AC_Packing_pLab15			= FOUR_CHAR_CODE ('PLAB'),
	
	/** 8 bit Grayscale or gamut test results, no padding.
	   G = 0 is 100% ink or black or fully out of gamut.
	   G = 255 is 0% ink or white or fully in gamut.
	   When used for gamut test results, any value >= 128 should be
	   considered to be in gamut. */
	AC_Packing_Gray8_Black0	= FOUR_CHAR_CODE ('g8k0'),

	/** Same as AC_Packing_Gray8_Black0 with inverse encoding. */
	AC_Packing_Gray8_White0	= FOUR_CHAR_CODE ('g8w0'),
	
	/** 15+ bit Grayscale or gamut test results, no padding.
	   G = 0 is 100% ink or black or fully out of gamut.
	   G = 32768 is 0% ink or white or fully in gamut.
       Values greater than 32768 are invalid.
	   Data must be 16 bit aligned. */
	 AC_Packing_Gray15_Black0	= FOUR_CHAR_CODE ('G15K'),
	
    /** 16 bit XYZ, with a leading pad word.
       X, Y, Z = 0        -> X, Y, Z = 0.0
       X, Y, Z = 32768    -> X, Y, Z = 1.0
       X, Y, Z = 65535    -> X, Y, Z = 1.9999694824.
	   Data must be 64-bit aligned. */
    AC_Packing_pXYZ16			= FOUR_CHAR_CODE ('PXYZ'),
	
	/** Generic padded 3-component 8 bit packing.
	   Data must be 32-bit aligned. */
	AC_Packing_pABC8			= FOUR_CHAR_CODE ('pabc'),
	
    /** Same as AC_Packing_pABC8, without the leading pad byte.
       Data need only be 8 bit aligned. */
    AC_Packing_ABC8			= FOUR_CHAR_CODE ('abc '),
    
	/** Generic padded 3-component 15+ bit packing.
	   Data must be 64-bit aligned. */
	AC_Packing_pABC15			= FOUR_CHAR_CODE ('pABC'),
	
	/** Generic 4-component 8 bit packing.
	   Data must be 32-bit aligned. */
	AC_Packing_ABCD8			= FOUR_CHAR_CODE ('abcd'),
	
	/** Generic 4-component 15+ bit packing.
	   Data must be 64-bit aligned. */
	AC_Packing_ABCD15			= FOUR_CHAR_CODE ('ABCD'),
	
	/** Packing codes for native 64-bit (gray) ColorSync formats (type "CMColor").
	   ICM2 also uses these packings formats (type "COLOR").
	   See the Apple ColorSync documentation for the details of these
	   formats.  These are mostly intended for internal use by ACE,
	   and are not intended for use by most ACE clients.
	   Data must be 16 bit aligned. */
	AC_Packing_CS64_Gray		= FOUR_CHAR_CODE ('CS01'),

	/** Packing codes for native 64-bit (RGB) ColorSync formats (type "CMColor").
	   ICM2 also uses these packings formats (type "COLOR").
	   See the Apple ColorSync documentation for the details of these
	   formats.  These are mostly intended for internal use by ACE,
	   and are not intended for use by most ACE clients.
	   Data must be 16 bit aligned. */
	AC_Packing_CS64_RGB		= FOUR_CHAR_CODE ('CS02'),

	/** Packing codes for native 64-bit (CMYK) ColorSync formats (type "CMColor").
	   ICM2 also uses these packings formats (type "COLOR").
	   See the Apple ColorSync documentation for the details of these
	   formats.  These are mostly intended for internal use by ACE,
	   and are not intended for use by most ACE clients.
	   Data must be 16 bit aligned. */
	AC_Packing_CS64_CMYK		= FOUR_CHAR_CODE ('CS03'),

	/** Packing codes for native 64-bit (lab) ColorSync formats (type "CMColor").
	   ICM2 also uses these packings formats (type "COLOR").
	   See the Apple ColorSync documentation for the details of these
	   formats.  These are mostly intended for internal use by ACE,
	   and are not intended for use by most ACE clients.
	   Data must be 16 bit aligned. */
	AC_Packing_CS64_Lab		= FOUR_CHAR_CODE ('CS04'),

	/** Packing codes for native 64-bit (xyz) ColorSync formats (type "CMColor").
	   ICM2 also uses these packings formats (type "COLOR").
	   See the Apple ColorSync documentation for the details of these
	   formats.  These are mostly intended for internal use by ACE,
	   and are not intended for use by most ACE clients.
	   Data must be 16 bit aligned. */
	AC_Packing_CS64_XYZ		= FOUR_CHAR_CODE ('CS05'),

	/** Packing codes for native 64-bit (abc) ColorSync formats (type "CMColor").
	   ICM2 also uses these packings formats (type "COLOR").
	   See the Apple ColorSync documentation for the details of these
	   formats.  These are mostly intended for internal use by ACE,
	   and are not intended for use by most ACE clients.
	   Data must be 16 bit aligned. */
	AC_Packing_CS64_ABC		= FOUR_CHAR_CODE ('CS06'),

	/** Packing codes for native 64-bit (abcd)ColorSync formats (type "CMColor").
	   ICM2 also uses these packings formats (type "COLOR").
	   See the Apple ColorSync documentation for the details of these
	   formats.  These are mostly intended for internal use by ACE,
	   and are not intended for use by most ACE clients.
	   Data must be 16 bit aligned. */
	AC_Packing_CS64_ABCD		= FOUR_CHAR_CODE ('CS07'),
	
	/** Null data, for use with data in AC_Space_Null. */
	AC_Packing_Null			= FOUR_CHAR_CODE ('null'),
	
	/** None of the above, use general packing specification. */
	AC_Packing_General			= 0,
	
	/** Force enum to be 32-bits wide. */
    AC_Packing_MaxEnum			= 0xFFFFFFFFL
	
} AC_PackingCode;


/** Constants that specify a standard ICC rendering intent for a device color profile. The
	rendering intent specifies the color translation method for colors that are outside the
	gamut of the color profile.
	@see ACMakeCalGray
	@see ACMakeCalLab
	@see ACMakeCalRGB
*/
typedef enum
{
	
	/** Tries to preserve the visual relationship between colors in
		a way that is perceived as natural to the human eye,
		although the color values themselves may change. This is
		the same as the Image intent in Adobe PageMaker and
		Illustrator. This option is suitable for photographic,
		continuous tone images.
	 */
	AC_Perceptual				= 0,

   	/** The same as absolute colorimetric, except that it
		compares the white point of the source color space to
		that of the destination color space and shifts all other
		colors accordingly, rather than comparing individual
		colors.
	*/
	AC_RelColorimetric			= 1,

	/** Tries to create vivid color at the expense of accurate
		color. It scales the source gamut to the destination
		gamut, but preserves relative saturation instead of hue,
		so that when scaling to a smaller gamut, hues may shift.
		This is the same as the Graphics intent in Adobe
		PageMaker and Illustrator. This option is suitable for
		business graphics and charts, where the exact
		relationship between colors is not as important as having
		bright saturated colors.
	 */
    AC_Saturation				= 2,

	/** Tries to maintain color accuracy at the expense of
		preserving relationships between colors. Leaves colors
		that fall inside the destination gamut unchanged. When
		translating to a smaller gamut, two colors that are
		distinct in the source space may be mapped to the same
		color in the destination space. This type of rendering can
		be more accurate than relativeColorimetric if the color
		profile of the image contains correct white point
		(extreme highlight) information.
	*/
    AC_AbsColorimetric			= 3,
    
    /** Use source profile's rendering intent. */
    AC_UseProfileIntent		= 0xFFFFFFFFL
} AC_RenderIntent;


#ifdef WIN_ENV
#define kACMaxPathLength	260
#else
#define kACMaxPathLength	256
#endif


/** 
	Contains a platform-specific version of a file specification. This is an FSSpec on the
	Macintosh, or a full path name on other platforms.
	@see ACLoadSettings
	@see ACPresetFileToName
	@see ACPresetListItemFile
*/
typedef struct _t_AC_FileSpec
{
	#ifdef MAC_ENV
	
	/* On the Macintosh, use an FSSpec. */
	
	FSSpec spec;
	
	#else
	
	/* Else just use a full path name. */
	
	char path [kACMaxPathLength];
	
	#endif
	
} AC_FileSpec;

/* color types */

/** Floating point XYZ color.  A pure black would be encoded as 0.0, 0.0, 0.0, 
   while a D50 based pure white would be encoded as 0.9642, 1.0, 0.8249. 
   @see ACCalCMYK
   @see ACCalGray
   @see ACCalLab
   @see ACCalRGB
*/
typedef struct _t_ACXYZColor
{
	/** */	
	double		X;
	/** */	
	double		Y;
	/** */	
	double		Z;	
} AC_XYZColor;

/******************************************************************************/


/** Floating point xy chromaticity coordinate.  These can be computed from XYZ
   colors using x = X / (X + Y + Z) and y = Y / (X + Y + Z). */
typedef struct _t_ACXYColor
	{
	/** */	
	double		x;
	/** */	
	double		y;
	} AC_XYColor;



/** A tone curve value for use in a calibrated CMYK color space specification. 
	@see ACCalCMYK
*/	
typedef struct _t_ACToneCurve
	{
	/** The number of bytes per value, 1 or 2. */
	ASUns32		bytesPerValue;		
	/** Number of samples (often 256). */
	ASUns32		count;				
	/** A pointer to the samples. */
	void			*data;				
	} AC_ToneCurve;

/******************************************************************************/


/** A simple 16-patch calibrated CMYK color space specification. */
typedef struct _t_ACCalCMYK
	{
	/** */	
	AC_XYZColor	w;
	/** */	
	AC_XYZColor	k;
	/** */	
	AC_XYZColor	c;
	/** */	
	AC_XYZColor	m;
	/** */	
	AC_XYZColor	y;
	/** */	
	AC_XYZColor	cm;
	/** */	
	AC_XYZColor	cy;
	/** */	
	AC_XYZColor	ck;
	/** */	
	AC_XYZColor	my;
	/** */	
	AC_XYZColor	mk;
	/** */	
	AC_XYZColor	yk;
	/** */	
	AC_XYZColor	cmy;
	/** */	
	AC_XYZColor	cmk;
	/** */	
	AC_XYZColor	cyk;
	/** */	
	AC_XYZColor	myk;
	/** */	
	AC_XYZColor	cmyk;
	/** */	
	AC_ToneCurve	cTRC;
	/** */	
	AC_ToneCurve	mTRC;
	/** */	
	AC_ToneCurve	yTRC;
	/** */	
	AC_ToneCurve	kTRC;
	/** */	
	double			opticalGamma;
	/** */	
	AC_XYZColor	white;
	/** */	
	AC_XYZColor	black;
	} ACCalCMYK;


/******************************************************************************/


/** Floating point Lab color.  Pure white is encoded as 100.0, 0.0, 0.0.  The
   usual a and b range is -128.0 to +127.0, but this structure supports any
   a and b range.
   @see ACMakeCalLab
*/
typedef struct _t_AC_LabColor
{
	/** */	
	double		L;
	/** */	
	double		a;
	/** */	
	double		b;
	} AC_LabColor;

/******************************************************************************/


/** A calibrated gray scale space specification.
	@see ACMakeCalGray
*/
typedef struct _t_ACCalGray
	{
	/** */	
	double			gamma;
	/** */	
	AC_XYZColor	white;
	/** */	
	AC_XYZColor	black;
	} ACCalGray;					

/******************************************************************************/


/** A calibrated RGB space specification.
	@see ACMakeCalRGB
*/
typedef struct _t_ACCalRGB
	{
	/** */	
	double			redGamma;
	/** */	
	double			greenGamma;
	/** */	
	double			blueGamma;
	/** */	
	AC_XYZColor	red;
	/** */	
	AC_XYZColor	green;
	/** */	
	AC_XYZColor	blue;
	/** */	
	AC_XYZColor	white;
	/** */	
	AC_XYZColor	black;
	} ACCalRGB;					

/******************************************************************************/


/** 
	A calibrated LAB color space specification. The usual min and max values for rangeA and
	rangeB are -128 and +127.
	@see ACMakeCalLab
*/
typedef struct _t_ACCalLab
	{
	/** */	
	AC_XYZColor	white;
	/** */	
	AC_XYZColor	black;
	/** */	
	struct
		{
		/** Usual value -128 */
		ASInt32 min;		
		/** Usual value +127 */
		ASInt32 max;		
		} rangeA;

	/** */	
	struct
		{
		/** Usual value -128 */
		ASInt32 min;		
		/** Usual value +127 */
		ASInt32 max;		
		} rangeB;
	} ACCalLab;					

/** Constant key values for an AC_Settings object. 
	@see ACGetSettingsProfile
	@see ACGetSettingsString
	@see ACGetSettingsUnsigned32
*/
typedef enum
	{
	/** Settings file name string (if different that file name). */
	AC_Key_Name				= FOUR_CHAR_CODE ('name'),

	/** Settings file description string. */
	AC_Key_Description			= FOUR_CHAR_CODE ('desc'),
	
	/** Name of application to write this settings file. */
	AC_Key_WriterName			= FOUR_CHAR_CODE ('wNam'),
	
	/** Working RGB profile. */
	AC_Key_WorkingRGB			= FOUR_CHAR_CODE ('wRGB'),
	
	/** Working CMYK profile. */
	AC_Key_WorkingCMYK			= FOUR_CHAR_CODE ('wCMY'),
	
	/** Working gray profile. */
	AC_Key_WorkingGray			= FOUR_CHAR_CODE ('wGry'),
	
	/** Working spot profile. */
	AC_Key_WorkingSpot			= FOUR_CHAR_CODE ('wSpt'),
	
	/** RGB color management policy (AC_Policy enum). */
	AC_Key_PolicyRGB			= FOUR_CHAR_CODE ('pRGB'),
	
	/** CMYK color management policy (AC_Policy enum). */
	AC_Key_PolicyCMYK			= FOUR_CHAR_CODE ('pCMY'),
	
	/** Gray color management policy (AC_Policy enum). */
	AC_Key_PolicyGray			= FOUR_CHAR_CODE ('pGry'),
	
	/** Ask about profile mismatches when opening (0 = no, 1 = yes). */
	AC_Key_MismatchAskOpening	= FOUR_CHAR_CODE ('mAsk'),
	
	/** Ask about profile mismatches when pasting (0 = no, 1 = yes). */
	AC_Key_MismatchAskPasting	= FOUR_CHAR_CODE ('pAsk'),
	
	/** Ask about missing profile when opening (0 = no, 1 = yes). */
	AC_Key_MissingAskOpening	= FOUR_CHAR_CODE ('misA'),
	
	/** Conversion engine CMS code (4-char code, stored as unsigned32). */
	AC_Key_EngineCMS			= FOUR_CHAR_CODE ('eCMS'),
	
	/** Conversion engine CMM code (4-char code, stored as unsigned32). */
	AC_Key_EngineCMM			= FOUR_CHAR_CODE ('eCMM'),
	
	/** Conversion intent (standard ICC integer encoding). */
	AC_Key_Intent				= FOUR_CHAR_CODE ('cInt'),
	
	/** Conversion black point compensation (0 = no, 1 = yes). */
	AC_Key_KPC					= FOUR_CHAR_CODE ('kpc '),
	
	/** Dither conversions between 8-bit color spaces (0 = no, 1 = yes). */
	AC_Key_Dither				= FOUR_CHAR_CODE ('dith'),
	
	/** Enable/disable monitor compression (0 = off, 1 = on). */
	AC_Key_CompressionEnabled	= FOUR_CHAR_CODE ('mce '),
	
	/** Monitor compression percent (in percent). */
	AC_Key_CompressionPercent	= FOUR_CHAR_CODE ('mcp '),
	
	/** Enable/disable RGB blending gamma (0 = off, 1 = on). */
	AC_Key_BlendGammaEnabled	= FOUR_CHAR_CODE ('bge '),
	
	/** RGB blending gamma value (100 = gamma 1.00). */
	AC_Key_BlendGammaValue		= FOUR_CHAR_CODE ('bgv '),
	
	/** Proof type (AC_ProofType enum). */
	AC_Key_ProofType			= FOUR_CHAR_CODE ('pTyp'),
	
	/** Proof profile. */
	AC_Key_ProofProfile		= FOUR_CHAR_CODE ('pPrf'),
	
	/** Display simulation (AC_Simulate enum). */
	AC_Key_Simulate			= FOUR_CHAR_CODE ('dSim'),
	
	/** Force enum to be 32-bits wide. */
    AC_Key_MaxEnum				= 0xFFFFFFFFL
	
} AC_SettingsKey;


/** Constant values that determine the type of an AC_Settings object.
	@see ACMakePresetList
	@see ACMakeSettings
*/
typedef enum
{
	/** Used to hold global color settings, such as working spaces, etc. */
	AC_SettingsType_Color		= FOUR_CHAR_CODE ('AsCs'),
	
	/** Used to specify the parameters for proofing a document. 
		The Proof Setup Files generally control a per-window setting */
	AC_SettingsType_Proof		= FOUR_CHAR_CODE ('AsPs'),
	
	/** Force enum to be 32-bits wide. */
    AC_SettingsType_MaxEnum	= 0xFFFFFFFFL
} AC_SettingsType;

/** 
	Constant values for ICC color space signatures.
	@see ACPRofileColorSpace
*/
typedef enum
{

	/**@name Standard ICC color space signatures. */
	/*@{*/
	/** */
    AC_Space_XYZ				= FOUR_CHAR_CODE ('XYZ '),
	/** */
    AC_Space_Lab				= FOUR_CHAR_CODE ('Lab '),
	/** */
    AC_Space_RGB				= FOUR_CHAR_CODE ('RGB '),
	/** */
    AC_Space_Gray				= FOUR_CHAR_CODE ('GRAY'),
	/** */
    AC_Space_CMYK				= FOUR_CHAR_CODE ('CMYK'),
	/** */
    AC_Space_Luv				= FOUR_CHAR_CODE ('Luv '),
	/** */
    AC_Space_YCbCr				= FOUR_CHAR_CODE ('YCbr'),
	/** */
    AC_Space_HSV				= FOUR_CHAR_CODE ('HSV '),
	/** */
    AC_Space_HLS				= FOUR_CHAR_CODE ('HLS '),
	/** */
    AC_Space_CMY				= FOUR_CHAR_CODE ('CMY '),
    /*@}*/

	/**@name ICC Color space signatures for generic color spaces. */
    /*@{*/
	/** */
    AC_Space_2Component		= FOUR_CHAR_CODE ('2CLR'),
	/** */
    AC_Space_3Component		= FOUR_CHAR_CODE ('3CLR'),
	/** */
    AC_Space_4Component		= FOUR_CHAR_CODE ('4CLR'),
	/** */
    AC_Space_5Component		= FOUR_CHAR_CODE ('5CLR'),
	/** */
    AC_Space_6Component		= FOUR_CHAR_CODE ('6CLR'),
	/** */
    AC_Space_7Component		= FOUR_CHAR_CODE ('7CLR'),
	/** */
    AC_Space_8Component		= FOUR_CHAR_CODE ('8CLR'),
	/** */
    AC_Space_9Component		= FOUR_CHAR_CODE ('9CLR'),
	/** */
    AC_Space_10Component		= FOUR_CHAR_CODE ('ACLR'),
	/** */
    AC_Space_11Component		= FOUR_CHAR_CODE ('BCLR'),
	/** */
    AC_Space_12Component		= FOUR_CHAR_CODE ('CCLR'),
	/** */
    AC_Space_13Component		= FOUR_CHAR_CODE ('DCLR'),
	/** */
    AC_Space_14Component		= FOUR_CHAR_CODE ('ECLR'),
	/** */
    AC_Space_15Component		= FOUR_CHAR_CODE ('FCLR'),
	/** Kodak's PhotoYCC space is stored as a generic 3 component space. */
    AC_Space_PhotoYCC			= AC_Space_3Component,
	/** A null color space.  Used to represent "spot-only" color spaces. */
	AC_Space_Null				= 0,
	/*@}*/

	/** Force enum to be 32-bits wide. */
	AC_Space_MaxEnum			= 0xFFFFFFFFL
	
} AC_ColorSpace;


/** Constants that specify the color space of working profiles.
	This enumeration added for the purpose of ACGetWorkingSpaceProfile().
	Profile returned by this function must be unreferenced by the caller.
	@see ACGetWorkingSpaceProfile
 */
typedef enum
{
	/** Grayscale profile. */
	kACWorkingGray = 0,
	/** RGB profile. */
	kACWorkingRGB = 1,
	/** CMYK profile */
	kACWorkingCMYK = 2,
	kACWorkingSpaces = 3
}ACWorkingSpace;

#ifdef WIN_ENV
#define kACEMaxPathLength	260
#else
#define kACEMaxPathLength	256
#endif
	
#ifdef __cplusplus
}
#endif

#if PRAGMA_STRUCT_ALIGN
	#pragma options align=reset
#endif

#endif /* _H_AcroColorExpT */


