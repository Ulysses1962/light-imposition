/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2004 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 AcroErr.h

 - Error codes are used in the ASRaise/DURING/HANDLER mechanism
   established in Except.h and also as error code return values from
   certain lower-level routines that do not raise. Error codes are
   32-bit integers (ASInt32). An error code can be built using the
   ErrBuildCode macro.

 - An error flag has three components:

   Severity: none, warning, severe			; 4 bits 	=> 16 severities
   System: Cos, PDDoc, etc.					; 8 bits 	=> 256 systems
   Error: FileOpen, Syntax, etc.			; 16 bits   => 65535 errors

*********************************************************************/

#ifndef _H_AcroErr
#define _H_AcroErr

#include "CoreExpT.h"

/* These are used so frequently they have their own macro */
#define ERR_GENERAL			1
#define ERR_NOMEMORY		2

#define ERR_SEVERITY_MASK	0xf0000000	/* upper four bits */
#define ERR_UNUSED_MASK		0x0f000000	/* reserved */
#define ERR_SYSTEM_MASK		0x00ff0000
#define ERR_ERROR_MASK		0x0000ffff	/* put this last to access it fastest */

#define ERR_SEVERITY_SHIFT	28
#define ERR_SYSTEM_SHIFT	16
#define ERR_ERROR_SHIFT		0

/* Errors but not severities or systems may be negative */
#define ErrBuildCode(xseverity,xsys,xerror) \
			( \
			 (((ASUns32) (xseverity))	<< ERR_SEVERITY_SHIFT)	| \
			 (((ASUns32) (xsys))		<< ERR_SYSTEM_SHIFT)	| \
			((((ASUns32) (xerror))	<< ERR_ERROR_SHIFT) & ERR_ERROR_MASK) \
			)

/* ErrGetCode will return a value between 0 & 65535 inclusive.
 * If the platform considers errors to be signed, use ErrGetSignedCode.
 */
#define ErrGetSeverity(xcode)	((((ASUns32) (xcode)) & ERR_SEVERITY_MASK)	>> ERR_SEVERITY_SHIFT)
#define ErrGetSystem(xcode)		((((ASUns32) (xcode)) & ERR_SYSTEM_MASK)		>> ERR_SYSTEM_SHIFT)
#define ErrGetCode(xcode)		((((ASUns32) (xcode)) & ERR_ERROR_MASK)		>> ERR_ERROR_SHIFT)
#define ErrGetSignedCode(xcode)	((ASInt16) ErrGetCode(xcode))

/* Error severity */
enum {
	ErrNoError = 0,
	ErrWarning,
	ErrSuppressable,	/* display a message if user hasn't suppresed errors */
	ErrSilent,			/* never display a message */
	ErrAlways			/* always display message even if others are suppressed */
};

typedef ASEnum8 ErrSeverity;

/* Systems */
enum {
	ErrSysNone = 0,	/* general error and out of memory error */
	ErrSysCos,		/* CosStore, filters */
	ErrSysCosSyntax,/* Cos syntax errors */
	ErrSysPDDoc,	/* PDDoc and family, Page tree, outlines*/
	ErrSysPDPage,	/* PDPage and family, thumbs, annots */
	ErrSysPDModel,	/* Global PD */
	ErrSysAcroView,	/* AcroView */
	ErrSysPage,		/* Page parsing and ripping */
	ErrSysFontSvr,	/* Font Server */
	ErrSysRaster,	/* AGM rasterizer */
	ErrSysASFile,	/* ASFile io errors */
	ErrSysXtnMgr,	/* Extension Manager errors */
	ErrSysXtn,		/* "New" error codes added by extensions */
	ErrSysMDSystem,	/* platform-specific system errors */
	ErrSysMDApp,	/* platform-specific application errors */
	ErrSysPDFX,		/* PDFX specific errors */
	ErrSysPDFEdit,	/* PDFEdit errors */
	ErrSysPDSEdit,	/* PDSEdit (structure) errors */
    ErrSysPDMetadata, /* XAP Metadata errors */
	ErrSysLast		/* no more than 256 systems allowed */
};

typedef ASEnum8 ErrSystem;

/* Macros for building system specific codes.
** Use these macros with the errors defined here.
** For example: ASRaise(PDDocError(pdErrTooManyPagesForOpen));
*/

#define GenError(e)			ErrBuildCode(ErrAlways, ErrSysNone, e)
#define CosError(e)			ErrBuildCode(ErrSuppressable, ErrSysCos, e)
#define CosErrorAlways(e)	ErrBuildCode(ErrAlways, ErrSysCos, e)
#define CosSyntaxError(e)	ErrBuildCode(ErrSuppressable, ErrSysCosSyntax, e)
#define PDDocError(e)		ErrBuildCode(ErrSuppressable, ErrSysPDDoc, e)
#define PDDocErrorAlways(e)	ErrBuildCode(ErrAlways, ErrSysPDDoc, e)
#define PDPageError(e)		ErrBuildCode(ErrSuppressable, ErrSysPDPage, e)
#define PDPageErrorAlways(e) ErrBuildCode(ErrAlways, ErrSysPDPage, e)
#define PDPageErrorSilent(e) ErrBuildCode(ErrSilent, ErrSysPDPage, e)
#define PDModelError(e)		ErrBuildCode(ErrAlways, ErrSysPDModel, e)
#define AcroViewError(e)	ErrBuildCode(ErrAlways, ErrSysAcroView, e)
#define PageError(e)		ErrBuildCode(ErrSuppressable, ErrSysPage, e)
#define PageErrorSilent(e)	ErrBuildCode(ErrSilent, ErrSysPage, e)
#define FontSvrError(e)		ErrBuildCode(ErrAlways, ErrSysFontSvr, e)
#define RasterError(e)		ErrBuildCode(ErrAlways, ErrSysRaster, e)
#if !defined(WEBBUY_LIB)
#define ASFileError(e)		ErrBuildCode(ErrAlways, ErrSysASFile, e)
#endif
#define XtnMgrError(e)		ErrBuildCode(ErrAlways, ErrSysXtnMgr, e)
#define XtnError(e)			ErrBuildCode(ErrAlways, ErrSysXtn, e)
#define MDSysError(e)		ErrBuildCode(ErrAlways, ErrSysMDSystem, e)
#define MDAppError(e)		ErrBuildCode(ErrAlways, ErrSysMDApp, e)
#define PDFXError(e)		ErrBuildCode(ErrAlways, ErrSysPDFX, e)
#define PDFEditError(e)		ErrBuildCode(ErrAlways, ErrSysPDFEdit, e)
#define PDSEditError(e)		ErrBuildCode(ErrAlways, ErrSysPDSEdit, e)
#define PDMetadataError(e)	ErrBuildCode(ErrAlways, ErrSysPDMetadata, e)


/* General Errors */
enum
{
	genErrNoError,                /* No error. */
	genErrGeneral,                /* An internal error occurred. */
	genErrNoMemory,               /* Out of memory. */
	genErrBadParm,                /* Bad parameter. */
	genErrListOverflow,           /* Operation or data is too complex. */
	genErrBadUnlock,              /* Attempt to release an unlocked object. */
	genErrExceptionStackOverflow, /* Exception stack overflow. */
	genErrResourceLoadFailed,     /* Failed to load an application resource (internal error). */
	genErrNameAlreadyRegistered,  /* Attempt to register an object with a name already in use. */
	genErrMethodNotImplemented,   /* Attempt to call a method that has not been implemented. */
	genErrCanceled,               /* User canceled operation. */
	genErrNoValidSerialNoFound	  /* No valid Acrobat serial number found. Acrobat will now quit. */
};

/* General Cos Errors */
enum
{
	cosErrNoError,                /* No error */
	cosErrReadError,              /* Read error. */
	cosErrWriteError,             /* Write error. */
	cosErrBadSyntax,              /* Syntax error. */
	cosErrNeedRebuild,            /* The file needs to be repaired. */
	cosErrRebuildFailed,          /* Could not repair file. */
	cosErrCantOpenTempFile,       /* A temporary file could not be opened. */
	cosErrTempFileFull,           /* The temporary file is full or nearly full. Close or save any modified documents. */
	cosErrStreamTooShort,         /* Stream source is shorter than specified length. */
	cosErrBadFilterName,          /* A stream specifies an unknown filter. */
	cosErrListOverflow,           /* Operation or data is too complex. */
	cosErrDocTableFull,           /* Cos document table full. */
	cosErrInt16OutOfRange,        /* A number is out of range. */
	cosErrExpectedNull,           /* Expected a null object. */
	cosErrExpectedDict,           /* Expected a dict object. */
	cosErrExpectedArray,          /* Expected an array object. */
	cosErrExpectedNumber,         /* Expected a number object. */
	cosErrExpectedBoolean,        /* Expected a boolean object. */
	cosErrExpectedName,           /* Expected a name object. */
	cosErrExpectedString,         /* Expected a string object. */
	cosErrExpectedStream,         /* Expected a stream object. */
	cosErrInvalidAssignment,      /* This direct object already has a container. */
	cosErrAfterSave,              /* Implementation failure: this document is now invalid. */
	cosErrInvalidObj,             /* Desired operation cannot be performed on this object. */
	cosErrArrayBounds,            /* Array out-of-bounds error. */
	cosErrDictKeyNotName,         /* Dict key must be a name object. */
	cosErrNeedFullSave,           /* This file must be saved with a full save. */
	cosErrEncryptionErr,          /* Error in encryption filter. */
	cosErrDCTError,               /* Error in JPEG data filter. */
	cosErrCCFError,               /* Error in CCITT fax data filter. */
	cosErrLZWError,               /* Error in LZW data filter. */
	cosErrExpectedDirect,         /* Expected a direct object. */
	cosErrOldLinFormat,           /* Obsolete linearized format */
	cosErrTempTooShort,			  /* Temp file unexpectedly short. */
	cosErrCancelSave,			  /* The Save operation was cancelled */
	cosErrEncryptionNotSupported, /* Encryption and decryption are not supported. */
	cosErrNoEncryptionKeySupplied,/* Encryption key is not supplied for a stream. */
	/* Defined for Acrobat 6.0 */
	cosErrDuplicateFilterName,    /* A filter with the same name is already registered. */
	cosErrNoDecodeFilter,         /* Attempted to decode without associated filter. */
	cosErrNoEncodeFilter,         /* Attempted to encode without associated filter. */
	cosErrCryptAuthFailed,        /* Decryption authorization failed during data access. */
	cosErrExpectedObjectStream,   /* Expected an object stream. */
	cosErrExpectedIndirect,       /* Expected an indirect object. */
	cosErrExpectedProc,           /* Expected a procedure. */
	cosErrExpectedCollection,     /* Expected an object collection. */
	cosErrNotCompressed,          /* Expected a compressed object. */
	cosErrCompressed,             /* Expected an object that was not compressed. */
	/* Skip four "message" codes. */
	cosErrBadRefcount = 51,		  /* An acquired object was already released. */
	cosErrObjFreed,				  /* An object has been replaced or destroyed. */
    cosErrMemMgrError,			  /* Internal error in the memory manager. */
    cosErrNeedXrefStm			  /* This file requires a cross-reference stream (PDF 1.5) */
};

/* Cos Syntax Errors */
enum
{
	cosSynErrNoError,             /* No syntax error. */
	cosSynErrNoHeader,            /* File does not begin with '%PDF-'. */
	cosSynErrNoEOF,               /* Missing %%EOF. */
	cosSynErrNoStartXRef,         /* Could not find startxref address. */
	cosSynErrNoStartAddress,      /* Value of startxref address not an integer. */
	cosSynErrBadXref,             /* Missing 'xref'. */
	cosSynErrBadXrefHeader,       /* Xref header should be two integers. */
	cosSynErrBadXrefEntry,        /* Error reading xref entry. */
	cosSynErrBadTrailerStart,     /* Trailer dictionary start missing '<<'. */
	cosSynErrBadObjectLabel,      /* Object label badly formatted. */
	cosSynErrUnknownName,         /* Unrecognized object name. */
	cosSynErrUnknownTokenType,    /* Unrecognized token type. */
	cosSynErrNoEndStream,         /* Missing endstream. */
	cosSynErrExtraEndStream,      /* Unexpected endstream. */
	cosSynErrUnterminatedString,  /* Unterminated string. */
	cosSynErrStringTooLong,       /* String too long. */
	cosSynErrTokenTooLong,        /* Token too long. */
	cosSynErrBadCharInHexString,  /* Non-hex character in a hex string. */
	cosSynErrUnexpectedType,      /* Unexpected token type. */
	cosSynErrImageNeverEnded,     /* End of image not found. */
	cosSynErrUnexpectedDict,      /* Unexpected end of dictionary. */
	cosSynErrUnexpectedArray,     /* Unexpected end of array. */
	cosSynErrBadDict,             /* Error reading dictionary. */
	cosSynErrBadObject,           /* Error reading object. */
	cosSynErrBadArrayDict,        /* Expected dictionary or array. */
	cosSynErrBadFRef,             /* Bad foreign object reference. */
	cosSynErrPStackUnderflow,     /* Parse stack underflow while reading object. */
	cosSynErrBadLinearized,       /* Error reading linearized hint data. */
	cosSynErrBadHexCharInName,	  /* Non-hex character after # in a name. */
	cosSynErrBadName,			  /* Illegal characters in a name. */
	cosSynErrBadObjectRef,		  /* An object reference is invalid. */
	/* Defined for Acrobat 6.0 */
	cosSynErrBadXrefStream,       /* Error in XRef stream. */
	cosSynErrPrematureEOF,	      /* Unexpected end of file. */
	cosSynErrBadStreamStart,      /* Expected CR and/or LF after 'stream'. */
	cosSynErrBadObjStream,	      /* Error in object stream. */
	cosSynErrDictKeyNotName,      /* Dictionary keys must be direct name objects. */
	cosSynErrExpectedNull,        /* Expected the null object. */
	cosSynErrExpectedNumber,      /* Expected a number. */
	cosSynErrExpectedInteger,     /* Expected an integer. */
	cosSynErrExpectedReal,        /* Expected a real number. */
	cosSynErrExpectedUnsigned,    /* Expected a non-negative integer. */
	cosSynErrExpectedBoolean,     /* Expected true or false. */
	cosSynErrExpectedName,        /* Expected a name. */
	cosSynErrExpectedString,      /* Expected a string. */
	cosSynErrExpectedDict,        /* Expected a dictionary. */
	cosSynErrExpectedArray,       /* Expected an array. */
	cosSynErrExpectedStream,      /* Expected a stream. */
	cosSynErrIllegalStream,       /* Stream found in an illegal context. */
	cosSynErrNoLength,            /* Stream is missing a Length key. */
	cosSynErrExpectedIndirect,    /* Expected an indirect object */
	cosSynErrExpectedDirect,      /* Expected a direct object */
	cosSynErrIllegalIndRef        /* Illegal indirect reference */
};

/* PDDoc Errors */
enum
{
	pdErrNoError,                 /* No error. */
	pdErrBadFont,                 /* Bad font object or font descriptor object. */
	pdErrEmbeddingFont,           /* Error while trying to embed a font. */
	pdErrBadRootObj,              /* The root object is missing or invalid. */
	pdErrBadBaseObj,              /* The base pages object is missing or invalid. */
	pdErrBadOutlineObj,           /* The outlines object is missing or invalid. */
	pdErrBadResMetrics,           /* Invalid or corrupt font metrics in the resource file. */
	pdErrBadPageObj,              /* A page object is missing or invalid. */
	pdErrThumbError,              /* Error while processing thumbnail. */
	pdErrBadAnnotation,           /* Invalid annotation object. */
	pdErrBadPageTree,             /* The document's page tree contains an invalid node. */
	pdErrUnknownProcsets,         /* Information needed to print a page is unavailable. */
	pdErrUnableToOpenDoc,         /* This file could not be opened. */
	pdErrIsFileLocked,            /* Unable to open file for writing. It may be locked or unavailable. */
	pdErrUnableToWrite,           /* Unable to write file. */
	pdErrUnableToRenameTemp,      /* Unable to rename temporary file to Save As name. */
	pdErrUnableToRecover,         /* Unable to recover original file. */
	pdErrUnableToRead,            /* Unable to read file. */
	pdErrUnknownFileType,         /* This is not a valid Portable Document File (PDF) document. It cannot be opened. */
	pdErrAlreadyOpen,             /* This file is already open. */
	pdErrTooManyPagesForOpen,     /* This file cannot be opened because it contains too many pages. */
	pdErrNotEnoughSpaceForTempFile, /* There is not enough temporary disk space for this operation. */
	pdErrTooManyPagesForInsert,   /* Inserting this file would result in a document with too many pages. */
	pdErrBookmarksError,          /* There is an error in the bookmarks. */
	pdErrCannotOpenMoreBkMark,    /* Cannot open more bookmarks.  */
	pdErrUnableToExtractFontErr,  /* Unable to extract embedded font. */
	pdErrCannotOpenNotes,         /* An error occurred while creating the document notes file. */
	pdErrNoNotes,                 /* This document has no notes. */
	pdErrCopyPageFailed,          /* The copy of a page failed. */
	pdErrNeedRebuild,             /* This file is damaged. */
	pdErrBadFontFlags,            /* The font '%s' contains bad /Flags. */
	pdErrBadFontBBox,             /* The font '%s' contains a bad /BBox. */
	pdErrBadFontWidths,           /* The font '%s' contains bad /Widths. */
	pdErrOldCosFileOBSOLETE,      /* OBSOLETE */
	pdErrTrySaveAs,               /* This file can only be saved using Save As. */
	pdErrAbortNotes,              /* Creation of the notes file was cancelled. */
	pdErrPagesLockedNotDeleted,   /* One or more pages are in use and could not be deleted. */
	pdErrNotEnoughMemoryToOpenDoc, /* There is not enough memory available to open the document. */
	pdErrUnableToCloseDueToRefs,  /* Unable to close document due to outstanding references. */
	pdErrNeedPassword,            /* This document requires a password. */
	pdErrOpNotPermitted,          /* This operation is not permitted. */
	pdErrNoCryptHandler,          /* The security plug-in required by this command is unavailable. */
	pdErrBadThread,               /* Invalid article object. */
	pdErrBadBead,                 /* Invalid article element. */
	pdErrThreadProcessing,        /* Error while processing articles. */
	pdErrUnknownAction,           /* Unknown action type. */
	pdErrBadAction,               /* Invalid action object. */
	pdErrCantUseNewVersion,       /* This file contains information not understood by the viewer. It cannot be used for this operation. */
	pdErrOldEncryption,           /* This viewer cannot decrypt this document. */
	pdErrUnableToExtractFont,     /* Unable to extract the embedded font '%s'. Some characters may not display or print correctly. */
	pdErrUnableToFindFont,        /* Unable to find or create the font '%s'. Some characters may not display or print correctly. */
	pdErrBadAnnotColor,           /* Invalid annotation color (only RGB colors are allowed). */
	pdErrNeedCryptHandler,        /* Cannot execute this command on an unsecured document. */
	pdErrBadFontDescMetrics,      /* The font '%s' contains bad /FontDescriptor metrics. */
	pdErrWhileRecoverInsertPages, /* There was an error while inserting or extracting pages and another error while trying to recover. */
	pdErrBadBookmark,             /* Invalid bookmark object. */
	pdErrBadFileSpec,             /* Invalid file specification object. */
	pdErrAfterSave,               /* This document was successfully saved, but an error occurred after saving the document. Please close and reopen the document. */
	pdErrUnableToXlateText,       /* Some text in the font and character '%s' could not be displayed or printed correctly. The font could not be reencoded. */
	/* Defined after Acrobat 2.1 */
	pdErrTextStringTooShort,		/* Not enough bytes in text string for multibyte character code. */
	pdErrBadCMap,					/* A font contains a bad CMap /Encoding.*/
	pdErrOldATMVersion,				/* The font '%s' cannot be displayed with the installed version of ATM.*/
	pdErrZeroPageFile,				/* This file cannot be opened because it has no pages. */
	pdErrATMMemory,					/* ATM is running out of memory. Text in font '%s' may not render properly.*/
	pdErrOptMemory,					/*	There is not enough memory to optimize this file. */
	pdErrCancelSave, 				/* The Save operation was cancelled. */
	/* vvv previously missing vvv */
	pdErrCannotMergeWithSubsetFonts, /* These documents contain subset fonts that have the same name and cannot be merged. */
	pdErrCannotReopenDoc,	/* This document was successfully saved, but an error occurred after saving the document. Please close and reopen the document. */			
	pdErrNoPDDocForCosDoc,	/* No PDDoc associated with CosDoc */
	pdErrHostEncodingNotSet,	/* The application has not set the host encoding. */
	pdErrInvalidEmbeddedFont,	/* Invalid font '%s' was removed from the document. */
	/* Defined for and after Acrobat 5.0 */
	pdErrCannotDeleteAllPages,       /* You cannot delete all pages.  At least one page must remain. */
	pdErrStartLessThanEnd,           /* The starting page number must be less than or the same as the ending page number. */
	pdErrNotValidPage,               /* There is no page numbered '%s' in this document. */
	pdErrCannotBeBlankPage,          /* The page number cannot be left blank. */
	pdErrInvalidPageNumber,          /* '%s' is an invalid page number. */
	pdErrExceedEncryptionLength,     /* Exceeds support encryption key length */
	pdErrExceedEncryptionVersion,    /* This version of encryption is not supported. */
	pdErrRequireTrustedMode,         /* Only Adobe certified Acrobat plug-ins are allowed while viewing this document. */
	pdErrMissingGlyphs,              /* Unable to find a substitution font with all the characters used by the font named: '%s, some characters may not be displayed or printed. */
	pdErrNeedTradChinese,            /* An error has occurred that may be fixed by installing the latest version of the Traditional Chinese Language Support package. */
	pdErrNeedSimpChinese,            /* An error has occurred that may be fixed by installing the latest version of the Traditional Chinese Language Support package. */
	pdErrNeedKorean,                 /* An error has occurred that may be fixed by installing the latest version of the Korean Language Support package. */
	pdErrNeedJapanese,               /* An error has occurred that may be fixed by installing the latest version of the Japanese Language Support package. */
	pdErrMissingSubsetFont,          /* A font required for font substitution is missing. */
	pdErrCMapNotFound,               /* The encoding (CMap) specified by a font is missing. */
	pdErrLimitcheck = 92,            /* Implementation limit exceeded */
	pdErrPrintAsImageSpoolFileFull,  /* There is insufficient disk space for the print job spool file to hold the entire print job. Try freeing up disk space on the startup volume and print the remaining pages of the document again. */
	pdErrInvalidMediaBox,            /* Invalid MediaBox */
	/* Defined for Acrobat 6.0 */
	pdEnumCanceled = 97,             /* Enumeration process canceled by the callback function */
	pdErrFontEmbeddingFailed,        /* Could not embed '%s' */
	pdErrFontEmbeddingCanceled = 101,/* Font embedding canceled */
	pdErrMultipleDocuments,          /* The operation could not be performed because the objects belong to different documents. */
	pdErrBadOCObject,                /* Invalid type or value in an Optional Content object. */
	pdErrNoInlineImage,              /* The operation could not be performed because the image is inline. */
	pdErrNoCryptFilterHandler,       /* The required Crypt Filter is not registed by the security handler plug-in. */
	pdErrBadEncryptDict,             /* Bad encrypt dictionary */
	pdErrNoPermHandler = 108,        /* The permission handler required by this command is unavailable. */
	pdErrDuplicatePermHandler,       /* The permission handler has already been added for this doc. */
	pdErrExceedMaxPermHandlers,      /* Have reached the max number of permission handlers for this doc. */
	pdErrBadEncoding = 112,          /* The font '%s' contains an invalid encoding. Some characters may not display. */
	pdErrMatrixTooBig,               /* Implementation limit exceeded. Please try decreasing magnification level. */

	/* Defined for Acrobat 7.0 */
	pdErrNeedExtendedLang = 120,     /* An error has occurred that may be fixed by installing the latest version of the Extended Language Support package. */
	pdErrUnknownCryptFilter,         /* This file is encrypted with an unsupported cryptograhpic algorithm. A later version of Acrobat may be needed in order to open this document. */
	pdErrNeed3D                      /* An error has occurred that may be fixed by installing the latest version of the 3D Support package. */
};

/* PDPage Errors */
enum
{
	pdPErrNoError,                 /* No error. */
	pdPErrPageDimOutOfRange,       /* The dimensions of this page are out-of-range. */
	pdPErrBadType3Font,            /* Invalid Type 3 font. */
	pdPErrType3TooComplex,         /* Type 3 font is too complex to print. */
	pdPErrFormTooComplex,          /* Form is too complex to print. */
	pdPErrUnableToCreateRasterPort /* Creation of a raster port failed. */
};

/* Other PDModel errors */
enum
{
	pdModErrNoError,              /* No error. */
	pdModErrEncTablesFailed,      /* Unable to initialize font encoding tables. */
	pdModErrDuplicateCryptName,   /* A security plug-in is already registered with this name. */
	pdModErrDuplicatePermName     /* A permission handler is already registered with this name. */
};

/* AcroView errors */
enum
{
	avErrNoError,                 /* No error. */
	avErrCantOpenMoreThanTenDocs, /* No more than ten documents can be opened at a time. */
	avErrPrintJobTooBig,          /* There are too many pages to print. */
	avErrTooManyChars,            /* There is too much text to display.  Can't display more than 32,000 characters */
	avErrNoText,                  /* There is no text. */
/* previously missing */
	avErrCantOpenDialog,		/* Acrobat can not open this file. There is a modal dialog open. */
	avErrActionExternal,		/* This action can not be performed from within an external window. */
	avErrActionFullScreen,		/* This action can not be performed during full screen mode. */
	avErrActionRestricted,		/* This action can not be performed. */
	avErrCantOpenPrinting,		/* Acrobat can not open this file while printing another document. */
/* new for 3.1 */
	avErrBadAnnotationCopy,     /* Unregistered copy proc for annotation object. */
	avErrBadActionCopy,         /* Unregistered copy proc for action object. */
	avErrUpdateInternalError,   /* Unable to determine if a new update for this product exists. */
	avErrUpdateInternetError,   /* Unable to access the internet */
	avErrUpdateNoWebServices,   /* Web Services are currently not available. */
	avErrBadThreadLinkError,    /* Unable to read the Article because it is damaged or missing. */
	avInvalidPageRange,         /* Invalid page range. */
/* Defined for Acrobat 6.0 */
	avErrDownloadHelpError,     /* Unable to download Adobe Reader Help file. */
	avSAInsufficientPermission, /* Insufficient permission for this operation. */
	avSA128EncryptionPresent    /* Operation failed due to presence of 128-bit encryption in the PDF file. */
};

/* Page Contents errors */
enum
{
	pageErrNoError,               /* No error. */
	pageErrTooFewOps,             /* Too few operands. */
	pageErrWrongOpType,           /* Wrong operand type. */
	pageErrOpTooLarge,            /* Operand too large. */
	pageErrBadContents,           /* The page contents object has the wrong type. */
	pageErrImageExpectedNumber,   /* Expected a number while parsing an image. */
	pageErrExpectedEndOfColor,    /* Expected end of color space. */
	pageErrExpectedHexOrASC85,    /* Expected AsciiHex or Ascii85 string. */
	pageErrErrorParsingImage,     /* There was an error while trying to parse an image. */
	pageErrBadTypeInXTextArray,   /* Bad object type within a text operator array. */
	pageErrUnexpectedOpInDisplay, /* Found an unexpected operator in the display list. */
	pageErrInvalidGRestore,       /* Invalid restore. */
	pageErrFontNotSet,            /* Font has not been set. */
	pageErrTooFewPathOps,         /* Too few operands in path. */
	pageErrImageTooBig,           /* Image in Form or Type 3 font is too big. */
	pageErrParseContextError,     /* Error while parsing a Form or Type 3 font. */
	pageErrBadType3Font,          /* Invalid Type 3 font. */
	pageErrFontNotInResources,    /* A font is not in the Resources dictionary. */
	pageErrInvalidDash,           /* Dash arguments are invalid. */
	pageErrArrayLenWrong,         /* Array length is out-of-range. */
	pageErrNumberOutOfRange,      /* A number value is out-of-range. */
	pageErrColorOutOfRange,       /* A color value is out-of-range. */
	pageErrIllegalOpInTextOutline, /* There is an illegal operator inside a text outline object. */
	pageErrWrongNumOpsInCurve,    /* A curve operator has the wrong number of operands. */
	pageErrSeveralParsingErrors,  /* There were several parsing errors on this page. */
	pageErrWrongOperand,          /* Wrong operand type - expected type '%s'. */
	pageErrFontNotInResDict,      /* Could not find a font in the Resources dictionary - using Helvetica instead. */
	pageErrXObjectNotFound,       /* Could not find the XObject named '%s'. */
	pageErrFormNotFound,          /* Could not find the Form named '%s'. */
	pageErrUnknownXObjectType,    /* Unknown XObject type '%s'. */
	pageErrReadLessImageData,     /* Read less image data than expected. */
	pageErrUnrecognizedToken,     /* An unrecognized token '%s' was found. */
	pageErrTokenTypeNotRec,       /* Token type not recognized. */
	pageErrTooFewArgs,            /* There were too few arguments. */
	pageErrTooManyArgs,           /* There were many arguments. */
	pageErrOperandTooLarge,       /* An operand is too large. */
	pageErrErrorReadingPage,      /* There was an error reading page %s near the contents:  */
	pageErrImageExpectedEI,       /* Expected 'EI' while parsing an image. */
	pageErrUnknownFilterName,     /* Unknown filter name. */
	pageErrBadDecodeArray,        /* Bad decode array. */
	pageErrIllegalOpInPath,       /* Illegal operation inside a path. */
	pageErrIllegalOpInTextObj,    /* Illegal operation '%s' inside a text object. */
	pageErrReadLessImageColor,    /* Read less image color data than expected. */
	pageErrWrongArgsForSetColor,  /* Wrong number of arguments for a setcolor operator. */
	pageErrUnknownColorSpace,     /* Unknown ColorSpace '%s'. */
	pageErrColorSpaceNotFound,    /* Could not find the ColorSpace named '%s'. */
	pageErrBadForm,               /* Invalid Form. */
	pageErrIllegalTextOp,         /* Illegal operation '%s' outside text object. */
	pageErrFormTypeNotAvailable,  /* Form type '%s' is not supported. */
	pageErrOBSOLETE,              /*  */
	pageErrRecursiveMachine,      /* Internal error - machine called recursively. */

	pageErrInvalidImageMaskDepth,	/* An image is specified as an image mask with more than 1 bit per pixel. */

	/* Defined after Acrobat 2.1 */
	pageErrBadPattern,			/* Invalid Pattern. */
	pageErrPatternTypeNotAvailable,	/* Pattern type '%s' is not supported. */
	pageErrPatternNotFound,		/* Could not find the Pattern named '%s'. */
	pageErrBadColorSpace,		/* Invalid ColorSpace. */
	pageErrMissingResource,		/* A resource is missing */
	pageErrMissingKey,			/* Dictionary is missing the key '%s'. */
	pageErrEGStateNotFound,		/* Could not find the Extended Graphics State named '%s'. */
	pageErrBadEGS,				/* Invalid Extended Graphics State. */
	pageErrBadFunction,			/* Invalid Function resource. */

	/* Defined after Acrobat 3.0 */
	pageErrBadEPSColorSpace,	/* An image uses a color space which will not separate correctly in some applications. */
	pageErrBadShading,			/* Error in Shading dictionary. */
	pageErrBadMaskImage,		/* Error in Masked Image. */
	pageErrTooManyComps,		/* There were too many color components. */
	pageErrNotLevel3,			/* A feature requires PostScript 3. */
	pageErrBadAltXObject,		/* Invalid alternate image for the XObject named '%s'. */

	/* Defined after Acrobat 5.0 */
	pageErrBadTGroup,           /* Invalid Transparency Group */
	pageErrBadSoftMask,         /* Invalid Soft Mask */
	pageErrBadHalftone,         /* Invalid Halftone */
	pageErrIllegalColorOp       /* A color operator was used where it is not permitted. */
};

/* Font Server Errors */
enum
{
	fsErrNoError,                 /* No error. */
	fsErrInitFailed,              /* Initialization of the font server module failed. */
	fsErrNoMMFonts,               /* No Multiple Master fonts were found. */
	fsErrNoATM,                   /* Adobe Type Manager was not found. */
	fsErrNeedNewATM,              /* A new version of Adobe Type Manager is required. */
	fsErrNoT1ZapfDingbats,        /* The Type 1 font 'ZapfDingbats' must be installed. */
	fsErrDownloadFailed, 		  /* Font download failed. */
	fsErrDownloadAborted,		  /* Font download aborted. */
	fsErrBadParameter, 			  /* Bad parameter passed to font server. */
	fsErrMissingFont 			  /* No ZapfDingbats or Multiple Master fonts found. */
};

/* Rasterizer errors */
enum
{
	rasErrNoError,                /* No error. */
	rasErrInitFailed,             /* Initialization of the rasterizer module failed. */
	rasErrCreatePort,             /* Creation of rasterizer port failed. */
	rasErrDraw                    /* A rasterizer error occurred. */
};

/* ASFile errors */
enum
{
	fileErrNoErr,                 /* No error. */
	fileErrGeneral,               /* A file error has occurred. */
	fileErrObsolete,              /*  */
	fileErrDirFull,               /* The directory is full. */
	fileErrDiskFull,              /* The document's disk or the disk used for temporary files is full. */
	fileErrNSV,                   /* The disk containing this file is not available. */
	fileErrIO,                    /* A file I/O error has occurred. */
	fileErrRead,                  /* A file read error has occurred. */
	fileErrWrite,                 /* A file write error has occurred. */
	fileErrEOF,                   /* End of file was reached unexpectedly. */
	fileErrLocked,                /* This file is locked. */
	fileErrVLocked,               /* This disk is locked and cannot be written to. */
	fileErrBusy,                  /* This file is busy and cannot be deleted. */
	fileErrExists,                /* Another file already exists under the same name. */
	fileErrAlreadyOpen,           /* This file is already open or in use by another application. */
	fileErrPerm,                  /* You do not have access to this file. */
	fileErrWrPerm,                /* You do not have permission to write to this file. */
	fileErrFNF,                   /* This file cannot be found. */
	fileErrOpenFailed,            /* File open failed. */
	/* The following are new in 2.2 */
	fileErrBytesNotReady,		  /* Bytes not ready.*/
	fileErrUserRequestedStop, 	  /* User requested stop. */
	fileErrIOTimeout, 			  /* A file I/O error has occurred. The file connection timed out. */
	fileErrReadBlocked,			  /* A file I/O error has occurred. The file is blocked while reading. */
	fileErrNotADir,               /* This operation can only be performed on a folder. */
	fileErrTempCreate,            /* A uniquely named temporary file could not be created. Please restart the application and try again. */
	/* Acrobat 7.0 */
	fileErrTooBig				  /* This file is too big for the current operation. */
};

/* Extension-Manager errors */
enum
{
	xmErrNoError,                 /* No error. */
	xmErrOutOfDateHFT,            /* The plug-in was compiled with an out-of-date HFT. */
	xmErrNoPLUGResource,          /* The plug-in lacks a PLUG resource of ID 1. */
	xmErrPluginIncompatible,      /* The plug-in is incompatible with this version of the Viewer. */
	xmErrInitializationFailed,    /* The plug-in failed to initialize. */
	xmErrDuplicatePluginName,     /* Two plug-ins are attempting to register with the same name. */
	xmErrCannotReplaceSelector,   /* Attempt to replace an unreplaceable selector. */
	xmErrCalledObsoleteProc,      /* An unimplemented function was called. */
	xmErrPluginLoadFailed,        /* The plug-in failed to load. */
	xmErrNotPrivileged,           /* The Acrobat Reader cannot load this plug-in. */
	xmErr68KOnly,                 /* Only the 68K Viewer can load this plug-in. */
	xmErrPPCOnly,                 /* Only the PowerPC Viewer can load this plug-in. */
	xmErrPlugInNotTrusted,        /* This is not a trusted plug-in. */
	xmErrPlugInNotCarbonized,     /* This plugin cannot load on MacOS X. */
	xmErrPluginResourceMismatch	  /* The currently selected language resources are not supported by this plugin */
};

/* PDFX specific errors */
enum
{
	pdfxErrNoError,               	/* No error */
	pdfxErrWrongCallbacks,			/* the size of the passed callbacks struct is wrong (version) */
	pdfxErrDuringCallback,			/* tried to call a PDFXInstance function during a callback proc */
	pdfxErrCannotLaunchAcrobat,		/* PDFX could not launch Acrobat */
	pdfxErrCannotFindEWH			/* Could not find the External Window Handler plug-in */
};

/* PDFEdit specific errors */
enum
{
	peErrNoError,					/* No error*/
	peErrUnknownPDEColorSpace,		/* Unknown PDEColorSpace value.*/
	peErrWrongPDEObjectType,		/* Incorrect PDEObject type.*/
	peErrUnknownResType,			/* Unknown PDEObject resource type.*/
	peErrPStackUnderflow,			/* PDFEdit parse stack underflow while reading object.*/
	peErrCantCreateFontSubset,		/* Unable to create embedded font subset.*/
	peErrBadBlockHeader, 			/* Bad block header for type 1 embedded stream.*/
	peErrCantGetAttrs,				/* Unable to get attributes for font.*/
	peErrCantGetWidths,				/* Unable to get widths for font.*/
	peErrFontToEmbedNotOnSys,		/* Unable to find font to embed on this system.*/
	peErrCantEmbedFont,				/* This font is licensed and cannot be embedded.*/
	peErrCantGetImageDict,			/* Unable to get image dictionary.*/
	peErrCantReadImage,				/* Unable to read image data.*/
	peErrCantGetShading,			/* Unable to get shading resource. */
	peErrWrongOpType,				/* Wrong operand type. */
	peErrTooFewPathOps,				/* Too few operands in path. */
	peErrErrorParsingImage,			/* There was an error while trying to parse an image. */
	peErrReadLessImageColor,		/* Read less image color data than expected. */
	peErrReadLessImageData,			/* Read less image data than expected. */
	peErrBadResMetrics,				/* Invalid or corrupt font metrics in the resource file. */
	peErrBadType3Font				/* Invalid Type 3 font. */
};

/* PDSEdit (structure) specific errors */
enum
{
	pdsErrRequiredMissing,			/* A required field was missing from a dictionary. */
	pdsErrBadPDF,					/* An incorrect structure was found in the PDF file. */
	pdsErrWrongTypeEntry,			/* Dictionary entry has wrong Cos type. */
	pdsErrWrongTypeParameter,		/* Wrong type parameter supplied to a PDS procedure. */
	pdsErrAlreadyExists,			/* There is already a table entry with the same name. */
	pdsErrCantDo					/* Some software required to perform this operation is not present in this version of Adobe Acrobat. */
};

/* PDMetadata (XAP Metadata API) specific errors */
enum
{
    pdMetadataErrBadXAP,           /* The given metadata was not in the XAP format */
    pdMetadataErrBadPDF,           /* XAP metadata processing found a syntax error in PDF */
    pdMetadataErrCouldntCreateMetaXAP, /* Could not create internal representation of XAP metadata */
    pdMetadataErrInternalError     /* An internal error occurred while processing XAP metadata */
};

#if MAC_PLATFORM
/* Macintosh System errors */
enum
{
	cfMacNoErr = 0,                /* No error. (noErr) */
	cfMacdirFulErr = -33,          /* The directory is full. (dirFulErr) */
	cfMacdskFulErr = -34,          /* The document's disk or the disk used for temporary files is full. (dskFulErr) */
	cfMacnsvErr = -35,             /* There is no such volume available. (nsvErr) */
	cfMacioErr = -36,              /* A file I/O error has occurred. (ioErr) */
	cfMaceofErr = -39,             /* End of file was reached unexpectedly. (eofErr) */
	cfMacfLckdErr = -45,           /* This file is locked. (fLckdErr) */
	cfMacvLckdErr = -46,           /* This volume is locked and canÕt be written to. (vLckdErr) */
	cfMacfBsyErr = -47,            /* This file is busy and canÕt be deleted. (fBsyErr) */
	cfMacdupFNErr = -48,           /* Another file already exists under the same name. (dupFNErr) */
	cfMacopWrErr = -49,            /* This file is already open or in use by another application. (opWrErr) */
	cfMacvolOffLinErr = -53,       /* This fileÕs volume is not available. (volOffLinErr) */
	cfMacpermErr = -54,            /* You do not have permission to open this file. (permErr) */
	cfMacnoMacDskErr = -57,        /* This disk is not a Macintosh disk. (noMacDskErr) */
	cfMacwrPermErr = -61,          /* You do not have permission to write to this file. (wrPermErr) */
	cfMacdsMemFullErr = -26,       /* Out of memory (-26). (dsMemFullErr) */
	cfMacmemFullErr = -108,        /* Out of memory (-108). (memFullErr) */
	cfMacresNotFound = -192,       /* Tried to get a nonexistent resource (-192). (resNotFound) */
	cfMacGenPSErr = -8133,		   /* Postscript error (-8133). */
	cfMaciIOAbort = -27 ,          /* An I/O error has occurred (-27). (iIOAbort) */
	cfMaciPrSavPFil = -1,		   /* Error saving print file (-1). */
	cfMacServerLostConnection = -1070	/* This file's server connection has closed down. (aspParamErr) */
};

/* Macintosh Application errors */
enum
{
	mdAppErrNoError,              /* No error. */
	mdAppCantPrintToPDFWriter,    /* Cannot print to Acrobat PDFWriter. */
	mdAppNoDAsWhilePrint,         /* Please close all Desk Accessories before printing. */
	mdAppNoPrinter,               /* Printing is not possible until you have chosen a Printer using the Chooser. */
	mdAppAsstToolboxActive,		  /* Background printing is not possible with the Assistant Toolbox system
									 extension and loadable ATM. */
	mdAppIncorrectTTEmbed		  /* Some data in the embedded font '%s' was invalid. Some characters
									 may not display or print correctly. */
};
#endif

#if WIN_PLATFORM
enum
{
	WinBadFileErr = 2,             /* The file does not exist. */
	WinBadPathErr = 3,             /* The path does not exist. */
	WinTooManyErr = 4,             /* Too many open files. */
	WinAccessErr = 5,              /* Access denied. */
	WinBadHdlErr = 6,              /* Bad file handle. */
	WinMemErr = 8,                 /* Not enough memory. */
	WinBadDiskErr = 11,            /* Badly formatted disk. */
	WinBadDriveErr = 15,           /* Invalid drive. */
	WinWrPermErr = 19,             /* You do not have write permission. */
	WinNotDosErr = 26,             /* Not an MS-DOS disk. */
	WinGeneralErr = 31,            /* General failure. */
	WinShareErr = 32,              /* Sharing violation. */
	WinLockErr = 33,               /* Lock violation. */
	WinDeviceErr = 39,             /* Device does not exist. */
	WinExistsErr = 80              /* File already exists. */
};
#endif

#if OS2_PLATFORM
enum
{
	OS2BadFileErr = 2,             /* The file does not exist. */
	OS2BadPathErr = 3,             /* The path does not exist. */
	OS2TooManyErr = 4,             /* Too many open files. */
	OS2AccessErr = 5,              /* Access denied. */
	OS2BadHdlErr = 6,              /* Bad file handle. */
	OS2MemErr = 8,                 /* Not enough memory. */
	OS2BadDiskErr = 11,            /* Badly formatted disk. */
	OS2BadDriveErr = 15,           /* Invalid drive. */
	OS2WrPermErr = 19,             /* You do not have write permission. */
	OS2NotDosErr = 26,             /* Not an MS-DOS disk. */
	OS2GeneralErr = 31,            /* General failure. */
	OS2ShareErr = 32,              /* Sharing violation. */
	OS2LockErr = 33,               /* Lock violation. */
	OS2DeviceErr = 39,             /* Device does not exist. */
	OS2ExistsErr = 80              /* File already exists. */
};
#endif

#if UNIX_PLATFORM

#include <errno.h>

/* Unix System errors */
enum
{
	mdSysNoErr		= 0,		/* No error. */
	mdSysEDOM		= EDOM,		/* Math arg out of domain of func. */
	mdSysERANGE		= ERANGE,	/* Math result not representable. */
	mdSysEPERM		= EPERM,	/* Not super-user. */
	mdSysENOENT		= ENOENT,	/* No such file or directory. */
	mdSysESRCH		= ESRCH,	/* No such process. */
	mdSysEINTR		= EINTR,	/* Interrupted system call. */
	mdSysEIO		= EIO,		/* I/O error. */
	mdSysENXIO		= ENXIO,	/* No such device or address. */
	mdSysEBADF		= EBADF,	/* Bad file number. */
	mdSysEAGAIN		= EAGAIN,	/* No more processes allowed. */
	mdSysENOMEM		= ENOMEM,	/* Not enough core. */
	mdSysEACCES		= EACCES,	/* Permission denied. */
	mdSysEEXIST		= EEXIST,	/* File exists. */
	mdSysENOTDIR	= ENOTDIR,	/* Not a directory. */
	mdSysEISDIR		= EISDIR,	/* Is a directory. */
	mdSysENFILE		= ENFILE,	/* File table overflow. */
	mdSysEMFILE		= EMFILE,	/* Too many open files. */
	mdSysENOSPC		= ENOSPC,	/* No space left on device. */
	mdSysEROFS		= EROFS,	/* Read only file system. */
	mdSysEMLINK		= EMLINK,	/* Too many links. */
#ifdef EDQUOT
	mdSysEDQUOT		= EDQUOT,	/* Disc quota exceeded. */
#endif
#ifdef ELOOP
	mdSysELOOP		= ELOOP,	/* Too many levels of symbolic links. */
#endif
#ifdef EMULTIHOP
	mdSysEMULTIHOP	= EMULTIHOP,/* multihop attempted. */
#endif
#ifdef ENOLINK
	mdSysNOLINK		= ENOLINK,	/* Inactive link to a remote machine. */
#endif
#ifdef EOVERFLOW
	mdSysOVERFLOW	= EOVERFLOW,/* Stat buffer overflow. */
#endif /* EOVERFLOW */
	mdSysENAMETOOLONG = ENAMETOOLONG,	/* File name too long. */
	mdSysEFAULT		= EFAULT	/* Illegal address. */
};

/* Unix Application errors */
enum
{
	mdAppErrNoError,		/* No error. */
	mdAppOpenMsgPartFailed,	/* Open Message partially failed. */
	mdAppOpenMsgFailed,		/* Open Message failed. */
	mdAppModalWindowOpen,	/* Modal window open. */
	mdAppBadPropertyFormat,	/* Bad Property format. */
	mdAppGetPropertyError,	/* GetProperty error. */
	mdAppBadPlatformThing,	/* Bad AVWindow Platform Thing. */
	mdAppSIGBUS,			/* Bus Error Caught. */
	mdAppSIGILL,			/* Illegal Instruction Caught. */
	mdAppSIGSEGV,			/* Segmentation Violation Caught. */
	mdAppSIGUnknown,		/* Unknown signal caught. */
	mdAppLPTERM,			/* Print job terminated. */
	mdAppLPSTOP,			/* Print job stopped. */
	mdAppBadTmpDir			/* Bad Temporary Directory Resource. */
};
#endif /* UNIX_PLATFORM */

#endif /* _H_AcroErr */
