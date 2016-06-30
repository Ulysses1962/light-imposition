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

 PDSError.h

 - Definition of error codes for PDSEdit.

*********************************************************************/

DefineErr(pdsErrRequiredMissing, "A required field was missing from a dictionary")
DefineErr(pdsErrBadPDF, "An incorrect structure was found in the PDF file")
DefineErr(pdsErrWrongTypeEntry, "Dictionary entry has wrong Cos type")
DefineErr(pdsErrWrongTypeParameter, "Wrong type parameter supplied to a PDS procedure")
DefineErr(pdsErrAlreadyExists, "There is already a table entry with the same name")
DefineErr(pdsErrCantDo, "Some software required to perform this operation is not present in this version of Adobe Acrobat")
