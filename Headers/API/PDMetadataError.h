/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2000-2002 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 PDMetadataError.h

 - Definition of error codes for PDMetadata.

*********************************************************************/

DefineErr(pdMetadataErrBadXAP, "The given metadata was not in the XMP format")
DefineErr(pdMetadataErrBadPDF, "XMP metadata processing found a syntax error in PDF")
DefineErr(pdMetadataErrCouldntCreateMetaXAP, "Could not create internal representation of XMP metadata")
DefineErr(pdMetadataErrInternalError, "An internal error occurred while processing XMP metadata")


