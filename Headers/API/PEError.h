/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1998-2002 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 PEError.h

 - PDF Edit Error codes used with ASRaise(PDFEditError(peErrMumble)).

*********************************************************************/

DefineErr(peErrNoError,                 "No error")
DefineErr(peErrUnknownPDEColorSpace,    "Unknown PDEColorSpace value.")
DefineErr(peErrWrongPDEObjectType,      "Incorrect PDEObject type.")
DefineErr(peErrUnknownResType,          "Unknown PDEObject resource type.")
DefineErr(peErrPStackUnderflow,         "PDFEdit parse stack underflow while reading object.")
DefineErr(peErrCantCreateFontSubset,    "Unable to create embedded font subset.")
DefineErr(peErrBadBlockHeader,          "Bad block header for type 1 embedded stream.")
DefineErr(peErrCantGetAttrs,            "Unable to get attributes for font.")
DefineErr(peErrCantGetWidths,           "Unable to get widths for font.")
DefineErr(peErrFontToEmbedNotOnSys,     "Unable to find font to embed on this system.")
DefineErr(peErrCantEmbedFont,           "This font is licensed and cannot be embedded.")
DefineErr(peErrCantGetImageDict,        "Unable to get image dictionary.")
DefineErr(peErrCantReadImage,           "Unable to read image data.")
DefineErr(peErrCantGetShading, "Unable to get shading resource.")

/* Defined from page parser July 8, 1999 */
DefineErr(peErrWrongOpType,             "Wrong operand type.")
DefineErr(peErrTooFewPathOps,           "Too few operands in path.")
DefineErr(peErrErrorParsingImage,       "There was an error while trying to parse an image.")
DefineErr(peErrReadLessImageColor,      "Read less image color data than expected.")
DefineErr(peErrReadLessImageData,       "Read less image data than expected.")
DefineErr(peErrBadResMetrics,			"Invalid or corrupt font metrics in the resource file.")
DefineErr(peErrBadType3Font,			"Invalid Type 3 font.")

