/*************************************************************************
 * DigSigHFT.h
 *
 * Copyright (c) 1998-2004 Adobe Systems Inc. All Rights Reserved.
 *
 * NOTICE: All information contained herein is, and remains the
 * property of Adobe Systems Incorporated and its suppliers, if any.
 * The intellectual and technical concepts contained herein are
 * proprietary to Adobe Systems Incorporated and its suppliers and may
 * be covered by U.S. and Foreign Patents, patents in process, and are
 * protected by trade secret or copyright law. Adobe permits you to
 * use, modify, and distribute this file in accordance with the terms
 * of the Adobe license agreement accompanying it. If you have
 * received this file from a source other than Adobe, then your use,
 * modification, or distribution of it requires the prior written
 * permission of Adobe.
 *
 * Description:
 *
 * Digital Signature interface for Acrobat Digital Signature handlers.
 * Handlers can register as DigSig handlers to provide digital
 * signature services.  Handlers can also call back into the DigSigHFT
 * for various services.
 *
 * Update History: (most recent first)
 * 21-Mar-2003 - Last known edit for Acrobat 6.0 SDK
 * 05-Feb-2003 - Cleanup for Acrobat 6.0 SDK
 * 10-Jan-2003 - Added handlerName parameter to some new calls
 * 16-Dec-2002 - Acrobat 6.0 cleanup
 * 31-Aug-2001 - Added SigSeedValue object
 * 25-Aug-2000 - Add DigSigAP helper routines
 * 01-Jul-2000 - Introduce EScript support
 * 12-Nov-1998 - Export Compare Pages/Words to allow scripting
 * 20-Mar-1998 - Created
 ************************************************************************/


/*************************************************************************
 * OVERVIEW OF USAGE
 *
 * Digital signatures use a generic DigSig plugin supplied by Adobe, and one 
 * or more specific method plugins supplied by Adobe and third parties. This 
 * file defines the interface between DigSig and a specific method plugin, 
 * XxxxSig. 
 *
 * Note that enriched support for public key digital signatures and encryption
 * is available using the PubSecHFT, introduced in Acrobat 6.0.
 * Developers are encouraged to use the PubSecHFT rather then DigSigHFT.
 * The PubSec code uses DigSig for digital signature operations, but provides
 * many additional benefits.
 *
 * The reader is assumed to be familiar with The Acrobat Viewer Plug-in API 
 * Tutorial, Technical Note #5169.
 *
 * Upon startup of Acrobat, there is a 3-step initialization sequence, 
 * consisting of
 *		1) all plugins export HFTs (Host Function tables)
 *		2) all plugins import HFTs
 *		3) all plugins perform initialization
 * This sequence allows plugins to establish communication among themselves 
 * without being dependent on the order of loading. For digital signatures, 
 * the sequence is:
 *
 *		1) DigSig exports its HFT under the name "DigSigHFT"
 *		2)		XxxxSig imports the DigSig HFT
 *				YyyySig imports the DigSig HFT
 *		3)		XxxxSig calls DigSigRegisterFilter to handle signatures of 
 *					type /Xxxx:PPK
 *				YyyySig calls DigSigRegisterFilter to handle signatures of 
 *					type /Yyyy:Finger
 *				These registrations include passing to DigSig a struct of 
 *				callback routines and other information.
 *		...
 * Eventually, the user opens a document.
 *		DigSig calls XxxxSig and YyyySig to notify them of the new document; 
 *		they might allocate some storage or choose to automatically validate 
 *		any of their respective signatures in the document. Note that auto-
 *		validation may encounter significant delays if it requires reading 
 *		the entirety of a large document off a CD-ROM or over a network, or 
 *		it requires accessing some signature registry or authority over a 
 *		network; Adobe software will only access signatures at user request.
 *		4) DigSig calls XxxxSig.docOpen()
 *		   DigSig calls YyyySig.docOpen()
 *
 * If the user creates a signature field and does not specify a default 
 * signing method, DigSig handles that with no communication to the method 
 * plugins. 
 *		DigSig creates the signature field dictionary
 *		DigSig creates the signature annotation dictionary
 *		DigSig creates the (blank) signature appearance dictionary
 *
 * Signaturre fields can also be created by the Forms plugin.
 *
 * If the user deletes a signature field, DigSig handles that with no 
 * communication to the method plugins. 
 *
 * If the user creates a signature field and specifies a default method, that 
 * plugin is called to fill in default values:
 *		DigSig creates the signature field dictionary
 *		DigSig creates the signature annotation dictionary
 *		DigSig creates the (blank) signature appearance dictionary
 *		5) DigSig calls XxxxSig.defaultValue
 *				XxxxSig creates the default signature value dictionary
 *				XxxxSig creates /DV in the signature field dictionary 
 *					pointing to this
 *
 * If the user asks to sign a specific signature field using method XxxxSig,
 * DigSig sequences the interaction. This sequence is now a four-step process 
 * (the routine names are all different so that you can write transitional code 
 * that includes both the old and new style interaction). The new interaction 
 * looks like this:
 *
 *		Call XxxxSig.dsNewSigData			-- Do any dialogs, gather 
 *											   signature, save in memory
 *											   YOU MAY USE THE DV (default value)
 *											   part of sigfield if sigfield is not COSNULL
 *		if !cancel
 *		  Call XxxxSig.dsUnValidateSig on each signature
 *		  Calculate /Changes
 *		  Do save-as dialog for file name and OK/Cancel
 *		  if !cancel
 *		    Call XxxxSig.dsCommitSign		-- Alter document in memory
 *		    if !cancel
 *		      Do any side effects on other fields	
 *		      Insert /Changes				-- Save document on disk
 *		      Do actual save				-- Save document on disk
 *		      if !cancel
 *		        Call XxxxSig.dsFinishSign	-- Update document on disk
 *		Call XxxxSig.dsFreeSigData			-- Free up memory
 *
 *		6a) DigSig calls XxxxSig.dsNewSigData
 *			XxxxSig interacts with the user, and allows Cancel
 *			XxxxSig acquires the signature itself in a method-specific 
 *			way. All information is saved in memory, without altering 
 *			the document itself. This allows a later backout.
 *		6b) If dsNewSigData does not cancel, DigSig prepares the document 
 *			for saving. It first calls dsUnValidateSig on every 
 *			signature in the document to put any over/underprinting in
 *			cannonical form. It then counts how many pages and fields 
 * 			have changed since any prior signature and records this.
 *		6c) For a first signature, DigSig does the save-as dialog, 
 *			allowing the user to select filename, optimization, and 
 *			encryption. Or the user may cancel. Other than fatal errors
 *			such as out-of-disk-space, this is the point of no return.
 *
 *		6d) If the user does not cancel, DigSig calls XxxxSig.dsCommitSign 
 *			to update the document with the actual signature.
 *			XxxxSig creates the signature dictionary, possibly using 
 *			 information in the signature field /DV dictionary; some 
 *			 portions have dummy values, at least including the /ByteRange 
 *			 and /Contents keys
 *			XxxxSig points /V in the signature field dictionary to this
 *			XxxxSig creates the /AP /N value in the signature annotation 
 *			 dictionary, using a method-specific visible representation of 
 *			 the signature, including a symbol signifying "unvalidated 
 *			 signature" 
 *			XxxxSig may allocate dynamic storage for a marked array
 *			XxxxSig returns an array of "marked" COS objects that it 
 *			 cares about; this array includes at least the /ByteRange and 
 *			 /Contents value objects
 *		7a) DigSig inserts the /Changes array from 6b.
 *		7b) DigSig saves the PDF document to a file. For each Cos object in 
 *			the marked array, DigSig records the objects byte offset and 
 *			length in the file as written. The saved file may have objects 
 *			encrypted by the Acrobat standard encryption handler, if the user 
 *			so chooses.
 *			The very first time a document is signed, this save may rename 
 *			the file and may invoke the Optimizer, Linearizer, and Garbage 
 *			Collector. Upon return from the save, all COS objects are invalid, 
 *			including those in the marked array. Also, all PD-level objects 
 *			except the PDDoc are invalid. Signing methods must not depend on 
 *			saving any such state between dsCommitSign and dsFinishSign. In 
 *			particular, the byte offsets and lengths in the marked array are
 *			valid upon entry to doSign, but the COS objects are not. The order 
 *			of entries is unchanged, however.
 *				[these COS objects will be rewritten by DigSig as 
 *				 CosNull before calling dsFinishSign.]
 *		7c) DigSig calls XxxxSig.dsFinishSign, passing back in the marked array
 *			XxxxSig calculates the /ByteRange that it desires, using the 
 *			 byte offsets and lengths in the marked array.
 *			XxxxSig overwrites the marked /ByteRange value in the saved 
 *			 file, using the DigSigOverwriteIntArray or 
 *			 DigSigOverwriteBytes callback 
 *			XxxxSig overwrites any other marked Cos objects it wants to
 *			XxxxSig calculates any document digest that it desires, using 
 *			 the DigSigFileGetEOF, DigSigFileSetPos, and DigSigFileRead 
 *			 callbacks; or it may use the DigSigMD5ByteRange callback
 *			XxxxSig obscures or encrypts this digest in a method-specific 
 *			 way
 *			XxxxSig overwrites the marked /Contents value in the saved 
 *			 file, using DigSigOverwriteHexstring or DigSigOverwriteBytes
 *			XxxxSig may delete dynamic storage for the marked array
 *			XxxxSig returns
 *		8) DigSig calls XxxxSig.dsFreeSigData, which may free up any
 *			remaining storage 
 *
 * If the user asks to validate one or more signature fields, DigSig sequences 
 * though them one at a time:
 *		9) DigSig calls XxxxSig.validateSign
 *			XxxxSig re-calculates any document digest that it desires, 
 *			 using the DigSigFileGetEOF, DigSigFileSetPos, and 
 *			 DigSigFileRead callbacks; or it may use the DigSigMD5ByteRange 
 *			 callback
 *			XxxxSig compares this result to the stored one, and does any 
 *			 other method-specific checks it desires.
 *			XxxxSig optionally does a validation against some stored 
 *			 (network) registry.
 *			XxxxSig updates the /AP /N value in the signature annotation 
 *			 dictionary to show doublechecked/pass/fail symbol
 *			XxxxSig returns doublechecked/pass/fail
 *
 * The user may open more than one document at a time, and may switch between 
 * open documents at will.
 *
 * The user may ask to show a signature panel containing summary information 
 * for each signature in an open document. If multiple documents are open, 
 * there may be multiple panels, or a single panel may be repainted as the 
 * user switches between documents. DigSig manages updating the panel(s), but 
 * may call the respective method plugin for each signature to get information 
 * to display on the panel. For each signature, the signature panel has two 
 * levels of detail:
 *		10a) CLOSED displays a doublechecked/pass/fail/unknown/blank icon and 
 *			a line of text for each signature field in the document. The 
 *			default text is the name of the person signing and the date and 
 *			time of signing, displayed in a language-independent way. 
 *		  DigSig calls XxxxSig.validState to choose which icon to show
 *
 *		10b) OPEN displays an icon and line of text for each signature, then
 *			indented lines of further text, currently consistingof the name 
 *			of the signer, date and time of signing, location of signing, 
 *			reason for signing, and signing method
 *		  DigSig calls XxxxSig.validState to choose which icon to show
 *
 * A method plugin may wish to asynchronously have the signature panel for 
 * a document updated (it might be doing validation as a background or 
 * idle-loop task). To do this, XxxxSig calls back to DigSigUpdatePanel
 *
 * Eventually, the user closes a document.
 *		11) DigSig calls XxxxSig.docClose()
 *		    DigSig calls YyyySig.docClose()
 * 
 * Whenever a signature is created or verified, the method plugin may 
 * optionally alter the appearance of the signature in the document, for
 * the purpose of displaying or printing. For example, it could change
 * an overprinted question mark on an unverified signature to an underprinted 
 * logo for a verified signature. To help with this, DigSig provides an HFT 
 * callback DigSigGetStdXObj that returns an XObject for for a blank appearance, 
 * a queston mark, or a cross. These are suitable as targets of the Do operator
 * in a signatures appearance stream.
 *
 * To avoid saving a signature to a file with an appearance of valid (rather 
 * than unvalidated), just before each file save DigSig loops through all the 
 * signature fields and calls the specific methods dsUnValidateSig entry. This
 * routine may choose to restore the signatures appearance to the unvalidated
 * state.
 *
 * The purpose of the /Changes array is to pre-calculate at each save whether
 * any changes (other than the signature being applied) have been made to the 
 * document since the prior signature. This allows at subsequent document-open
 * time the quick display of a warning symbol on the Signatures panel if the
 * document was changed between any two signatures. The user may choose to 
 * roll back to the document to just before such a change and study the
 * difference between that document and the final one. This warning was a strong
 * customer request.
 *
 * There is now a constraint on the values in the /ByteRange array. This 
 * constraint allows DigSig to implement rollback to prior signatures:
 *		The largest offset + length value in the /ByteRange array
 *		for a given signature must be equal to the length of the PDF file
 *		containing that signature; i.e. it must equal offset + 1 of the 
 *		"F" in the %%EOF at the end of the file. 
 * In addition, the following constraints are strongly encouraged:
 *		All offsets must be in the range 0..2147483647
 *		All lengths must be in the range 1..2147483647
 *		offset[n+1] must be strictly greater than offset[n] + length[n]
 * 
 * The AcroForms Widget Annot handler calls into DigSig using four entries. 
 * These calls all reflect user actions taken in the document view, not the 
 * Signatures panel view.
 *
 * When the user selects an annotaiton by tabbing to it or by clicking it with 
 * the mouse, and that annotation is for a signature field, AcroForms calls 
 * DigSigDraw. bIsSelected is true if the anotaiton is selected.
 *
 * When the user tabs to a signature annotation and activates it by hitting 
 * the spacebar or enter key, this is equivalent to a left mouse click. 
 * AcroForms calls DigSigKeyDown. The parameters parallel those of 
 * AVAnnotHandlerDoKeyDownProc.
 * 
 * When the user left-clicks inside a signature annotation, AcroForms calls 
 * DigSigClick. The parameters parallel those of DoClickProcType.
 * 
 * When the user right-clicks inside a signature annotation, AcroForms calls 
 * DigSigRightClick.
 *
 ************************************************************************/

#ifndef _H_DigSigHFT
#define _H_DigSigHFT

#include "AF_ExpT.h"	// Acroforms HFT


#if PRAGMA_STRUCT_ALIGN
	#if __MWERKS__
		#pragma options align=power_gcc
	#elif __GNUC__
		#pragma options align=power
	#else
		#error
	#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif


/** ESObject is an EScript object that is supported as of Acrobat 5.0
	by the escript plug-in. The escript HFT has not been exposed to
	third party developers, thus you should use or assume NULL for
	ESObject parameters.  Signature support for EScript is available
	only when using the PubSecHFT for public-key signatures.
*/
typedef struct _s_ESObjectRec	ESObjectRec,	*ESObject;

/** Return values DS calls (DSRetCode, or ASInt32).  All negative
	return values are considered exceptions.  If value is kDSException then
	exception text should be available from the called routine.  If
	this exception text is NULL then it should be considered an unknown error.
*/
typedef ASInt32 DSRetCode;

/** 
	Get exception string. When this is returned,
	exception text should be available from the called
	routine. If the exception text is NULL, it is
	considered to be an unknown error.
*/
#define kDSException		-2
/**
	Error in a parameter to the function call. No
	exception string.
*/
#define	kDSParameterError	-1
/** A boolean return value. */
#define kDSFalse			0
/** A boolean return value. */
#define kDSTrue				1
/** Success. */
#define kDSOk				1

/*************************************************************************
 * SigInfo properites
 * These are common properties found in the sigInfo ASCab
 * Plug-ins that want to add their own properties to this object
 * should do so within their own namespace (eg. 'Acme:contactInfo').
 ************************************************************************/

/* EScript SigInfo object name */
#define	SIGINFO_OBJ		"SigInfo"
/* Private data name of ASCab entry in sigInfo ESObject */
#define	ESOBJ_SigInfo	"signatureInfo"

/* ASText properties */
#define	PROP_SigInfo_Name				"name"
#define	PROP_SigInfo_Reason				"reason"
#define	PROP_SigInfo_Location			"location"
#define	PROP_SigInfo_Date				"date"				/* in PDF date format */
#define	PROP_SigInfo_VerifyDate			"verifyDate"		/* in PDF date format, new in A7 */
#define PROP_SigInfo_StatusText			"statusText"
#define PROP_SigInfo_Subfilter			"subFilter"
#define PROP_SigInfo_Handler			"handlerName"		/* name specifed by /Filter */
#define PROP_SigInfo_HandlerUIName		"handlerUIName"	
#define PROP_SigInfo_HandlerUserName	"handlerUserName"	/* same as handlerUIName */
#define PROP_SigInfo_VerifyHandler		"verifyHandlerName"	/* name of handler used to validate signature */
#define PROP_SigInfo_VerifyHandlerUIName	"verifyHandlerUIName"
#define PROP_SigInfo_MDP				"mdp"

/* ASText properties used only for Public key signatures */
#define	PROP_SigInfo_ContactInfo		"contactInfo"
#define PROP_SigInfo_Appearance			"appearance"		/* write-only */
#define PROP_SigInfo_Password			"password"			/* write-only */
#define PROP_SigInfo_SignFormat			"subFilter"
#define PROP_SigInfo_TimeStamp          "timeStamp"

/* ASInt32 properties */
#define PROP_SigInfo_Revision			"revision"
#define PROP_SigInfo_NumRevisions		"numRevisions"
#define PROP_SigInfo_Status				"status"
#define PROP_SigInfo_NumPagesAltered	"numPagesAltered"
#define PROP_SigInfo_NumFieldsAltered	"numFieldsAltered"
#define PROP_SigInfo_NumFieldsFilledIn	"numFieldsFilledIn"
#define PROP_SigInfo_ByteRange			"byteRange"			/* array of ASInt32 */
#define PROP_SigInfo_docValidity		"docValidity"		/* result of byte range test (value is DSSigValState) */
#define PROP_SigInfo_objValidity		"objValidity"		/* result of MDP test (value is DSSigValState) */
#define PROP_SigInfo_idValidity			"idValidity"		/* result of id validity test (value is DSValidState) */
#define PROP_SigInfo_idPrivValidity		"idPrivValidity"	/* result of handler id validity test (value is handler specific) */
#define PROP_SigInfo_trustFlags			"trustFlags"		/* contains a PSSigTrust, which is defined in PubSecHFT.h */

/* ESObjects properties. Implemented in PubSec. */
#define PROP_SigInfo_Certificates		"certificates"
#define	PROP_SigInfo_BuildInfo			"buildInfo"

/* Values for the mdp property of a SigInfo Object. See PSSigMDPType enum. */
#define PROP_MDPType_AllowAll			"allowAll"
#define PROP_MDPType_AllowNone			"allowNone"
#define PROP_MDPType_Default			"default"
#define PROP_MDPType_DefaultAndComments	"defaultAndComments"

/* Ubiquity Rights array of ASText. See PDF Reference. */
#define PROP_SigInfo_AppRightsDoc		"appRightsDocument"
#define PROP_SigInfo_AppRightsForm		"appRightsForm"
#define PROP_SigInfo_AppRightsSignature	"appRightsSignature"
#define PROP_SigInfo_AppRightsAnnots	"appRightsAnnots"
#define PROP_SigInfo_AppRightsFormEx	"appRightsFormEx"
#define PROP_SigInfo_AppRightsAnnotsEx	"appRightsAnnotsEx"
#define PROP_SigInfo_AppRightsEF	    "appRightsEF"

/* ASBool properties */
#define	PROP_SigInfo_DateTrusted		"dateTrusted"		/* boolean indicating if date is from a trusted source, new in A7 */

/** Enum of all possible PROP_SigInfo_docValidity and PROP_SigInfo_objValidity values (document/data/object hash validity states) */
typedef enum {
	/** Validity not yet determined */
	kDSSigValUnknown=0,				
	/** Validity could not be determined because of error */
	kDSSigValUnknownTrouble,		
	/** Validity could not be determined because all bytes are not ready */
	kDSSigValUnknownBytesNotReady,	
	/** Validation has been attempted, but there were errors that meant the signature
	    could not be validated, with the conclusion that the result is invalid */
	kDSSigValInvalidTrouble,		
	/** Validity for this digest is not used (eg no doc validity if no byte range) */
	kDSSigValUnused,				
	/** Sig was just signed, so implicitly valid */
	kDSSigValJustSigned,			
	/** Sig doc/obj digest invalid */
	kDSSigValFalse,					
	/** Sig doc/obj digest valid */
	kDSSigValTrue,					
	/** */
	kDSSigValEnumSize 
} DSSigValState;

   
/*************************************************************************
 * SigSeedValue properites
 * These are common properties found in the seedValue ASCab
 * This ASCab is destroyed when the seedValue object is destroyed,
 * so plug-ins should uses this object whenever adding private data.
 * Plug-ins that want to add their own properties to this object
 * should do so within their own namespace (eg. 'PPKAcme:issuerDN').
 * See the Acrobat JavaScript guide for a more complete description
 * of these properties.
 ************************************************************************/

/** Private data name of ASCab entry in seedValue ESObject */
#define	ESOBJ_SigSeedValue	                "sigSeedValue"

/** ASAtom giving name of required handler to use when signing */
#define	PROP_SigSeedValue_Filter			"filter"
/** ASAtom array giving name of acceptable signing formats */
#define PROP_SigSeedValue_SubFilter         "subFilter" 
/** Double indicating the required revision of the handler */
#define PROP_SigSeedValue_Version			"version"
/** ASText array of reasons for signing */
#define	PROP_SigSeedValue_Reasons			"reasons"    /* array */
/** String giving the MDP params */
#define PROP_SigSeedValue_MDP               "mdp"
/** ASText array of legal attestations for MDP signing */
#define PROP_SigSeedValue_Attestations      "legalAttestations" /* array */
/** ASInt32 flags */
#define PROP_SigSeedValue_Flags				"flags"

/*************************************************************************
 * GetSigDictProp properites
 * These are common properties found in the ASCab that is returned
 * by GetSigDictProp. These values are used by DigSig for display
 * purposes (eg AVPanel, tooltips, or whatever). If a value
 * is not present then DigSig will determine the value on its own,
 * using alternate mechanisms. For example if PROP_DSSigProp_Date is not present then DigSig
 * looks in the /M field of the SigDict to get the date. The ASCab
 * should be cached by the DigSig handler
 ************************************************************************/

/* ASText properties */
#define	PROP_DSSigProp_Date	                "M"					/* PDF Date format, time of signing */
#define PROP_DSSigProp_VerifyDate           "VerifyDate"        /* PDF Date format, time of verification, new in Acrobat 7 */
#define PROP_DSSigProp_Name					"Name"				/* Common name of signer */
#define PROP_DSSigProp_IssuerCN				"IssuerCN"			/* Common name of issuer */
#define PROP_DSSigProp_SubjectOrg			"SubjectOrg"		/* Organisation of signer */
#define PROP_DSSigProp_SigValTextSummary	"SigValTextTop"		/* Summary signature validity text, to append to AVPanel sig title */
#define PROP_DSSigProp_SigValTextDetail		"SigValTextDetail"	/* Sig validity detail title line */
#define PROP_DSSigProp_SigValTextDetails	"SigValTextDetails"	/* ASCab array of ASText sig validity details */
#define PROP_DSSigProp_SigValTextTooltip	"SigValTextTooltip"	/* Signature validity text to show in tooltip */
#define PROP_DSSigProp_ErrText              "ErrorText"         /* Error text, present only if there was error while validating, 
                                                                   new in Acrobat 7 */

/* ASBool properties */
#define	PROP_DSSigProp_TrustedDate	        "Trusted_M"          /* indicates whether the signing time was from a trusted time
                                                                   source or not. If the key is not present, its assumed it
                                                                   was from an untrusted (e.g. system) source, new in Acrobat 7 */

#define PROP_DSSigProp_IconState			"ADBE:sigIconState"     /* If you want to put this entry in your plugins' signature properties cab please make sure 
																	you use the DSSigPropIconState enum below for its values. This will ensure that DigSig use the
																	appropriate icons (in sync with your plugins') to represent the signature's state. 
																	If you do not provide this value, DigSig will use validity icons according to what 
																	it thinks is the validity status of the signature. */
#define PROP_DSSigProp_DisplayOnPageIcon "ADBE:sigDisplayOnPageIcon" /*	only use the values enumerated below. Default is kDSDisplayOnPageIconAlways. */
typedef enum { 
	kDSDisplayOnPageIconAlways = 0, 
	kDSDisplayOnPageIconExceptWhenValid, 
	kDSDisplayOnPageIconNever,
	kDSDisplayOnPageIconEnumSize
} DSSigPropDisplayOnPageIcon;


typedef	enum {
	kDSSigIconBlank=0,					// sig field is unsigned
	kDSSigIconSigUnknown,				// sig field is signed but not validated
	kDSSigIconSigInvalid,				// sig field is signed and failed validate
	kDSSigIconSigValidIdUnknown,		// sig field is signed and doc is valid, but identity is unknown or not trusted
	kDSSigIconSigValidIdUnknownModAfter,	// sig field is signed, but identity is unknown or not trusted and mods have been made since sig applied
	kDSSigIconSigValid,					// sig field is signed and double-checked valid
	kDSSigIconSigValidModAfter,			// sig field is signed and double check valid, but mods have been made since sig applied
	kDSSigIconAuthUnknown,				// sig field is auth signed but not validated
	kDSSigIconAuthInvalid,				// sig field is auth signed and failed validate
	kDSSigIconAuthValidIdUnknown,		// sig field is auth signed and doc is valid, but identity is unknown or not trusted
	kDSSigIconAuthValidIdUnknownModAfter,	// sig field is auth signed, but identity is unknown or not trusted and mods have been made since sig applied
	kDSSigIconAuthValid,				// sig field is auth signed and double-check valid
	kDSSigIconAuthValidModAfter,		// sig field is auth signed and double check valid, but mods have been made since sig applied
	kDSSigIconEnumSize
} DSSigPropIconState;

/*************************************************************************
 * DigSigOffset record
 * Used to tell XxxxSig where objects landed in saved file
 ************************************************************************/
/**
	Structure that indicates the location of objects in a saved 
	PDF document. 
	@see DSClearSigProc 
	@see DSFinishSignProc 
	@see DigSigCosObjOverwrite 
	@see DigSigOverwriteBytes 
	@see DigSigOverwriteHexstring 
	@see DigSigOverwriteIntArray 
*/
typedef struct _t_DigSigOffsetRec {
	/** The CosObj whose byte offset/length is desired. */
	CosObj	cosObj;					
	/** Byte offset in saved PDF file. Range is 0.. 2147483647 */
	ASInt32	byteOffset;				
	/**  Length in saved PDF file. Range is 1.. 2147483647      */
	ASInt32	byteLength;				
} DigSigOffsetRec, *DigSigOffset;


/** Signature annotation bounding box.
	@see DSNewSigDataProc
*/
typedef struct _t_DigSigBBoxRec {
	/** The width of the signature bounding box. */
	ASFixed	bbWidth;				
	/** The height of the signature bounding box. */
	ASFixed	bbHeight;				
} DigSigBBoxRec, *DigSigBBox;


/** A validity state constant for a signature field, resulting from verification.
	@see DigSigVerifySig
	@see DSGetValidStateProc
	@see DSValidateSigProc
*/
typedef enum {
	/** Signature field is unsigned. */
	DSSigBlank=0,					
	/** Signature field is signed but not validated. */
	DSSigUnknown,					
	/** Signature field is signed but failed validation. */
	DSSigInvalid,					
	/** Signature field is signed and valid. */
	DSSigValid,						
	/** Signature field is signed and double-checked valid. */
	DSSigDoubleChecked,				
	/**
	A validity state constant for a signature field, resulting 
	from verification. 
*/
	DSSigValidStateEnumSize			
} DSValidState;

/** Structure describing the appearance of a digital signature. 
	@see DSUnValidateSigProc
	@see DigSigGetStdXObj
*/
typedef enum { 
	/** Blank appearance. */
	DSBlankXObj=0,					
	/** Signed but not validated appearance. */
	DSUnknownXObj,					
	/** Signed and failed validate appearance. */
	DSInvalidXObj,					
	/** Signed and valid, but identity not verified. */
	DSValidXObj,					
	/** Signed and valid and identity verified. */
	DSDoubleCheckedXObj				
} DSXObjType;

/** A return type for DigSig and PubSec callback.
	@see DSNewSigDataExProc
	@see DSNewSigDataWithParamsProc
*/
typedef enum { 
	/** Do not proceed with signing. */
	DSSignCancel=0,					
	/** Save over old file. */
	DSSignSave,						
	/** Save into new file. */
	DSSignSaveAs,					
	/** No save option specified (new in Acrobat 6) */
//	DSSignNone,						
	/** */
	DSSaveTypeEnumSize
} DSSaveType;

/** Constants that specify the method to use for creating a signature digest. */
typedef enum {
	/** */
	kDSDigestNone=0,
	/** */
	kDSDigestMD5,
	/** */
	kDSDigestSHA1,
    /** */
    kDSDigestSHA256,
	/** */
	kDSDigestEnumSize
} DSDigestMethod;

/** Signature value encodings, for public key signatures.
   Parameter to DigSigDataSigSign and DigSigDataSigValidate. 
	@see DigSigDataSigSign
	@see DigSigDataSigValidate
*/
typedef enum {
	/** Not specified */
	kDSSigEncodeNone=0,			
	/** PKCS#1 encoding */
	kDSSigEncodePKCS1,			
	/** PKCS#7 encoding */
	kDSSigEncodePKCS7,			
	/** Digest is directly encrypted */
	kDSSigEncodeRawSig,			
	/** */
	kDSSigEncodeEnumSize
} DSSigEncode;

/** Modification Detection and Prevention (MDP) options 
*/
typedef enum {
    /** No MDP, or the document does not have an author signature. */
    kDSMDPNone = -1,          
	/** Allow any changes (disables MDP feature). */
	kDSMDPAllowAll=0,	      
	/** Allow no changes. */
	kDSMDPAllowNone,	      
	/** Only allow 'natural' changes (form field fill-in, page spawning). */	
	kDSMDPDefault,		      
	/** Allow all comment types edit/modify/delete operations in addition to default changes. */
	kDSMDPCommentsAndDefault, 
	/** */
	kDSMDPEnumSize    
} DSMDPType;

/** Boolean properties of a DigSigHandler, obtained with DSGetBoolPropertyProc,
	that show whether the handler supports specific functionality.
	@see DSGetBoolPropertyProc
*/
typedef enum {
	/** When true, the handler supports author signatures. */
	kDSHandlerCanDocAuthSign=0,	
	/** When true, the handler supports ordinary visible signatures. */
	kDSHandlerCanDocPDDocSignVisible,	
	/** When true, the handler supports EScript. */
	kDSHandlerEScriptAware,	
	/** */
	kDSHandlerPropEnumSize
} DSHandlerProperty;

/** Constants that specify what dialog to show.
*/
typedef enum {
	/** Do not show signature properties dialog. */
	kDSPropNone=0,
	/** Show general signature properties dialog. */
	kDSPropSignature,
	/** Show legal notice dialog. */
	kDSPropLegal,
	/** Show signature rollback (will actually rollback to this signature). */
	kDSPropViewVersion,
	/** Size of DSPropertyType enum */
	kDSPropTypeEnumSize
} DSPropertyType;

/** Parameters for DSNewSigDataWithParamsProc.
	@see DSNewSigDataWithParamsProc
*/
typedef struct _t_DigSigNewSigDataParamsRec {
	/** Size of the data structure. Must be set to
		sizeof(DigSigNewSigDataParamsRec). */
	ASSize_t size;				

	/** The PDF document that is being signed. */
	PDDoc pdDoc;				
	/** Signature form field that is being signed. */
	CosObj sigField;			
	/** Signature annotation that is being signed (usually equivalent to sigField). */
	CosObj sigAnnot;			
	/** Bounding box of signature appearance. */
	DigSigBBox bb;				

	/** Name of the selected handler to use when signing. */
	ASAtom filter;				
	/** Used internally. If specified then use to sign, else use engine that belongs to default UI */
	ESObject esSigEngine;		
	/** Used internally. If specified then provides parameters to use when signing */
	ESObject esSigParams;		
	/** Used internally. If not NULL and there is an error that needs reporting for escript, set it here and return DSSignCancel */ 
	ASText esErrorText;			
	/** If true, prompt to sign, otherwise silent. (Can be true with Escript.) */
	ASBool bUI;					
	/** When true, signing a document author signature. */
	ASBool bDocAuthSig;			
	/** Legal PDF scrubber warnings, populated only when signing a document author signature.*/
	CosObj legalPDFDict;		
	/** Modification, detection, and prevention (MDP) setting, populated
		only when signing a document author signature. One of:
			kDSMDPAllowAll: Allow all changes (disables MDP).
			kDSMDPAllowNone: Allow no changes.
			kDSMDPDefault: Allow only "natural" changes (form-field
		filling, page spawning).
			kDSMDPCommentsAndDefault: Allow all comment types
		edit/modify/delete, in addition to default changes.
*/
	DSMDPType mdpSetting;
	/** signifies whether we are ubiquitizing the document. mutually exclusive with bDocAuthSig*/
	ASBool bUBSig;
	/*	parameters for ub signing. */
	CosObj urdDict;
} DigSigNewSigDataParamsRec, *DigSigNewSigDataParams;


/** Type of CosDoc signature */
typedef enum {
	/** */
    kDSObjCosDoc=0,
	/** */
    kDSObjUbiquity,
	/** */
    kDSObjEnumSize
} DSCosDocSigObjType;

/** Parameters passed in to DSCosDocSigSign and DSCosDocSigValidate proc.
	Certain parameters vary depending on what DSCosDocSigObjType objSigType is set to.
	
	If objSigType is:
		kDSObjCosDoc: 
                    objSignParams - none
                     pdDoc - used for window parenting when available, otherwise NULL 
                     (this would be the active doc and NOT necessarily the doc 
                     to which the CosDoc belongs)
                     cosDoc - CosDoc that is to be signed
		kDSObjUbiquity:                   
                     objSignParams - CosDict containing the Ubiquity suboptions for 
                     Form filling, Annot changes, Page Template Spawning, etc. Look at
                     URD spec in PDF Manual 1,5, section 8.7.2 for precise 
                     syntax of CosDict and the keys within
                     pdDoc - indicates the PDDoc to be UBized
                    cosDoc - ignored
    @see DSCosDocSigSign
	@see DSCosDocSigValidate
*/
typedef struct _t_DigSigCosDocSigParamsRec {
	/** size of this struct */
	ASSize_t size;
	/** The associated PDDoc, if needed. See DigSigCosDocSigParamsRec 
		for more details.
	*/			
	PDDoc pdDoc;				
	/** The CosDoc to be signed, if needed. See DigSigCosDocSigParamsRec 
		for more details.
	*/
	CosDoc cosDoc;
	
	/** Name of the selected handler to use when signing or validating */
	ASAtom filter;				
	/** If specified then use to sign, else use engine that belongs to default UI */
	ESObject esSigEngine;		
	/** If specified then provides parameters to use when signing */
	ASCab sigParamsCab;			
	/** if 0 then silent, else prompt to sign (can be true even with escript). If 1 then simple UI, else complex UI */
	ASInt32 uiType;				

	/** Dialog title to use when signing. Object owned by DigSig */
	ASText signDialogTitle;		
	/** Message to use when acquiring resources for signing. Object owned by DigSig */
	ASText promptMessage;		
	/** If an exception then this returns the exception string. Caller must call ASTextDestroy(). */
	ASText asTextError;			

	/** Type of CosDoc signature. */
    DSCosDocSigObjType objSigType;
	/** The associated objSigParams, if needed. See DigSigCosDocSigParamsRec 
		for more details.
	*/
    void *objSigParams;
} DigSigCosDocSigParamsRec, *DigSigCosDocSigParams;

typedef struct _t_DigSigDataParamsRec {
	/** size of this struct */		                                
	ASSize_t size;					
	/** Pointer to data to be processed */
	ASUns8* inDataPtr;				
	/** Size of data to be processed */
	ASUns32 inDataSize;				
	/** Resulting object value, to be freed by caller using ASfree*/
	ASUns8* outDataPtr;			
	/** Size of resulting object */
	ASUns32 outDataSize;		
	/** If an exception then this returns the exception string. Caller must call ASTextDestroy(). */
	ASText outASTextError;				

} DigSigDataParamsRec, *DigSigDataParams;

typedef struct _t_DigSigDataBufferSigNewParamsRec {
	/** size of this struct */
	ASSize_t size;					

	/** Used for window parenting */				                                
	PDDoc inPDDoc;					
	/** Name of the selected handler to use when signing. Must set to ASAtomNull if not defined. 
		Upon signature being applied, this will be set to the name of the filter that was used to sign. */
	ASAtom filter;					
	/** If specified then use to sign, else use engine that belongs to default UI */
	ESObject inESSigEngine;			
	/** If specified then provides parameters to use when signing */
	ESObject inESSigParams;			
	/** When signing, if not none then data is to be digested and digest signed, otherwise directly sign data */
	DSDigestMethod inDigestMethod;	
	/** Specifies sig value encoding */
	DSSigEncode inSigValueEncoding;	
	/* Seed value information */
	ASCab inSeedValue;
	/** Name of data that is being signed */
	ASText inDataTitle;
	/** Description of data that is being signed */
	ASText inDataDescription;
	/** Dialog title to use when signing. Object owned by DigSig */
	ASText inSignDialogTitle;		
	/** Message to use when acquiring resources for signing. Object owned by DigSig */
	ASText inPromptMessage;		
	/** if >0 then prompt to sign, else silent (can be true even with escript) */
	ASInt32 inNUI;						
	/** XXX Callback to display data that is being signed */
	void* inDataDisplayCallbackProc;

	/** Cert chain of signer. Must be an initialized ASCab. Returned by sigNew call. */
	ASCab outCertChainCab;				
	/** Signature properties. Must be an initialized ASCab. 
		Refer to PROP_SigProp_* definitions in PubSecHFT.h for defined properties. 
		Returned by sigNew call. */
	ASCab outSigPropCab;				
	/** Signature build information. Must be an initialized ASCab. 
		Refer to PROP_SigBuild* definitions in PubSecHFT.h for defined properties. 
		Returned by sigNew call. */
	ASCab outSigBuildCab;				
	/** If an exception then this returns the exception string. Caller must call ASTextDestroy(). */
	ASText outASTextError;				

} DigSigDataBufferSigNewParamsRec, *DigSigDataBufferSigNewParams;

typedef struct _t_DigSigDataBufferSigValidateParamsRec {
	/** size of this struct */
	ASSize_t size;					

	/** Used for window parenting */				                                
	PDDoc inPDDoc;					
	/** Name of the selected handler to use when verifying. Must set to ASAtomNull if not defined. */
	ASAtom filter;					
	/** If specified then use to verify, else use engine that belongs to default UI */
	ESObject inESSigEngine;			
	/** If specified then provides parameters to use when signing */
	ESObject inESSigParams;			
	/** When signing, if not none then data is to be digested and digest signed, otherwise directly sign data */
	DSDigestMethod inDigestMethod;	
	/** Specifies sig value encoding */
	DSSigEncode inSigValueEncoding;	
	/** Name of data that is being verified */
	ASText inDataTitle;
	/** Description of data that is being verified */
	ASText inDataDescription;
	/** Dialog title to use when signing. Object owned by DigSig */
//	ASText inSignDialogTitle;		
	/** Message to use when acquiring resources for signing. Object owned by DigSig */
//	ASText inPromptMessage;		
	/** if >0 then allow UI, else silent (can be true even with escript) */
	ASInt32 inNUI;						
	/** XXX Callback to display data that is being signed */
	void* inDataDisplayCallbackProc;

	/** Cert chain of signer. Must be an initialized ASCab. Returned by sigNew call. */
	ASCab inCertChainCab;

	/** Pointer to data to be signed. If NULL then signing does not occur. */
	ASUns8* inDataPtr;				
	/** Size of data to be signed */
	ASUns32 inDataSize;				
	/** Resulting signature value, to be freed by caller using ASfree, or sig value if verifying */
	ASUns8* inSigValuePtr;			
	/** Size of resulting signature value */
	ASUns32 inSigValueSize;			

	/** If an exception then this returns the exception string. Caller must call ASTextDestroy(). */
	ASText outASTextError;				

} DigSigDataBufferSigValidateParamsRec, *DigSigDataBufferSigValidateParams;

/** Notification types for the DSNotifySigProc
*/
typedef enum {
	/** No notification. Not used */
	kDSNotifyNone=0,
	/** Mark the validity of this signature as unknown. This should mark validity 
	caches to indicate that the signature validity is unknown. */
	kDSNotifySetUnknown,
	/** The document may have been modified since this signature was applied. The handler
	   should update the signature status after calling DigSigDocModifiedAfterSig. 
	   This is not called in Acrobat 6.0, but may be used in an update release. */
	kDSNotifyModifiedAfterSig,
	/** Size of DSNotifySigType enum */
	kDSNotifySigTypeEnumSize
} DSNotifySigType;


/*************************************************************************
 * DigSigHFT Methods
 ************************************************************************/

/**
	Callback for DigSigHandler. Called when a new document is 
	opened. 
	@param pdDoc The document that has been opened.
	@see DSDocCloseProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DSDocOpenProc)
	(PDDoc pdDoc);
/** Callback for DigSigHandler. Called when a new document is closed.
	@param pdDoc The document being closed.
	@see DSDocOpenProc
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DSDocCloseProc)
	(PDDoc pdDoc);

/** This call was used only in Acrobat 5.x when the EScript security
   object was implemented in DigSig.  Returned the existing UI engine
   object (ESObject) if uiEngine, otherwise return a new signature
   engine object (ESObject).  handlerName is the name of the handler
   that is being asked to return the new engine. vData is reserved for
   future use and is currently not used.  This call is no longer
   used. */
typedef ACCBPROTO1 ESObject (ACCBPROTO2 *DSNewSigEngineProc)
	( void *context, ASAtom handlerName, ASBool uiEngine, void *vData );

/** Internal use. Add your engine-specific properties to existing signatureInfo
    ESObject.  Any private data should be added to the ASCab that is
    set as private data in ESOBJ_SigInfo ASCab (see above). This
    should have been written to pass an ASCab instead of ESObject, but
    we missed this.  handlerName is the name of the handler that is
    being asked to return the data (not necessarly the same as the
    value of the /Filter attribute in the sigDict).  Adobe use only,
    and subject to change.  Can only be used by EScript aware DigSig
    handlers (ie. PubSec). */
typedef ACCBPROTO1 void (ACCBPROTO2 *DSGetSigInfoProc)
	(PDDoc pdDoc, CosObj sigField, CosObj sigAnnot, ASAtom handlerName, ESObject sigInfo );

/** Internal use. Add your engine-specific properties to existing seedValue ESObject.
   Any private data should be added to ESOBJ_SigSeedValue ASCab (see
   above).  Adobe use only, and subject to change.  Can only be used
   by EScript aware DigSig handlers (ie. PubSec). *jsErrText will
   be NULL. The handler should set *jsErrText to a new ASText object,
   which will then be destroyed by DigSig.
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *DSGetSigSeedValueProc)
	(PDDoc pdDoc, CosObj sigField, CosObj sigAnnot, ESObject seedValue, ASText *jsErrText );

/** Internal use. Add your engine-specific properties to existing seedValue ESObject.
   Any private data should be added to ESOBJ_SigSeedValue ASCab (see
   above).  Adobe use only, and subject to change.  Can only be used
   by EScript aware DigSig handlers (ie. PubSec). *jsErrText will
   be NULL. The handler should set *jsErrText to a new ASText object,
   which will then be destroyed by DigSig.
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *DSSetSigSeedValueProc)
	(PDDoc pdDoc, CosObj sigField, CosObj sigAnnot, ESObject seedValue, ASText *jsErrText );

/** Internal use. Validate a signature (performs action).  If sigEngine is not NULL
    then use this engine, otherwise use the UI engine.  handlerName is
    the name of the handler that is being asked to validate the
    signature (not necessarly the same as the value of the /Filter
    attribute in the sigDict). If bUI then show an alert after
    validating.  Note that this alert may be supressed depending on
    pref settings Adobe use only, and subject to change.  Can only be
    used by EScript aware DigSig handlers (ie. PubSec).
*/
typedef ACCBPROTO1 DSValidState (ACCBPROTO2 *DSValidateSigExProc)
	(PDDoc pdDoc, CosObj sigField, CosObj sigAnnot, ASAtom handlerName, ESObject sigEngine, ASBool bUI );

/**
	Callback for DigSigHandler. Called to validate a signature. 
	Recalculates any document digest that it desires, possibly 
	using the DigSigFileGetEOF, DigSigMD5ByteRange, DigSigFileRead, 
	and DigSigFileSetPos methods. 

	May compare this result to the stored one, and do any other 
	signature-specific checks desired. Optionally does a validation 
	against a stored (network) registry. If necessary, updates 
	the AP dictionary in the signature annotation dictionary 
	to show the validation state of the signature. 
	@param pdDoc The document being signed. 
	@param sigField Signature field. 
	@param sigAnnot Cos object of the signature annotation.
	@return Validation state. 
	@see DSUnValidateSigProc 
	@see DSReValidateSigProc 
	@see DigSigFileGetEOF 
	@see DigSigMD5ByteRange 
	@see DigSigFileRead 
	@see DigSigFileSetPos 
*/
typedef ACCBPROTO1 DSValidState (ACCBPROTO2 *DSValidateSigProc)
	(PDDoc pdDoc, CosObj sigField, CosObj sigAnnot);

/**
	Callback for DigSigHandler. Called by signature panel display 
	to choose which of the double-checked, pass, fail, unknown, 
	or blank icons to show. May call DigSigUpdatePanel to update 
	the signature panel. 
	@param pdDoc The document being signed. 
	@param sigField Signature field. 
	@param sigAnnot Cos object of the signature annotation. 
	
	@return Validation state. 
	@see DigSigUpdatePanel 
	NOTE: Superceded in Acrobat 6.0 by DSNewSigDataWithParamsProc. 
*/
typedef ACCBPROTO1 DSValidState (ACCBPROTO2 *DSGetValidStateProc)
	(PDDoc pdDoc, CosObj sigField, CosObj sigAnnot);

/**
	This function is called to query the handler to find out 
	if it can validate this PDDoc signature field. The call 
	determines if the general signature format is supported 
	by the handler, or if the handler should be given the opportunity 
	to validate this signature. 

	If the response is false, the handler cannot be called to 
	validate the signature. In this event, the handler will 
	not be able to provide other information (for example that 
	an updated version of the handler should be obtained in 
	order to validate the signature). 
	@param pdDoc The document that contains the signature 
	field. 
	@param sigField The signature field. 
	@param sigAnnot The signature annotation that is being 
	verified (usually equivalent to sigField). 
	@param filter The name by which DigSig knows this handler. 
	This is not necessarily the value of /Filter.
	@return true if the handler can validate the field signature, false 
	otherwise. 
	@see DigSigVerifySig 
*/
typedef ACCBPROTO1 ASBool(ACCBPROTO2 *DSCanValidateProc)
	(PDDoc pdDoc, CosObj sigField, CosObj sigAnnot, ASAtom filter );

/**
	Return a string describing the status of the signature. 
	Must be PDDocEncoding or unicode, where unicode strings 
	must start with 0xFE, 0xFF. 

	NOTE: Superceded by DSGetSigPropProc in Acrobat 6.0, which 
	provides better granularity for signature status. 
	@param pdDoc The document being signed. 
	@param sigField Signature field. 
	@param sigAnnot Cos object of the signature annotation.
	@param bSilent
	@return None. 
*/
typedef ACCBPROTO1 ASText (ACCBPROTO2 *DSGetStatusTextProc)
	(PDDoc pdDoc, CosObj sigField, CosObj sigAnnot, ASBool bSilent );

/**
	NOTE: Deprecated in Acrobat 6.0. 

	Callback for DigSigHandler. Called when a new signature 
	field is created. This method creates the default signature 
	value dictionary and creates a default value (DV) entry 
	in the signature field dictionary pointing to this dictionary. 
	
	@param pdDoc The document being signed. 
	@param sigField Signature field. 
	@param sigAnnot Cos object of the signature annotation.
	Callback for DigSigHandler. Called when a new document is 
	closed. 
	@param pdDoc The document being closed.
	@see DSDocOpenProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DSDefaultValueProc)
	(PDDoc pdDoc, CosObj sigField, CosObj sigAnnot);

/**
	NOTE: Superceded in Acrobat 6.0 by DSNewSigDataWithParamsProc. 
	

	Callback for DigSigHandler. Called to gather signature data 
	or to cancel. May display dialogs. May use the default value 
	(DV) field of the signature field if it is not CosNull. 
	

	To allow for a user to cancel signing, the handler should 
	not modify the document during this callback. The handler 
	will commit the signature to the PDF file during DSClearSigProc. 
	
	@param pdDoc The document being signed. 
	@param bb Signature annotation bounding box. 
	@param sigData Signature data, as defined by the specific 
	signature plug-in. 
	@param sigField Signature field. 
	@param sigAnnot Cos object of the signature annotation.
	@return true if signature data gathered, false if canceled. 
	@see DSClearSigProc 
	@see DSNewSigDataWithParamsProc 
	@see DSFinishSignProc 
	@see DSFreeSigDataProc 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *DSNewSigDataProc)
	(PDDoc pdDoc,  DigSigBBox bb, void **sigData, CosObj sigField, CosObj sigAnnot);

/** Use DSNewSigDataWithParamsProc instead of this call.
   Support for this call will continue for at least the short term. */
typedef ACCBPROTO1 DSSaveType (ACCBPROTO2 *DSNewSigDataExProc)
	(PDDoc pdDoc,  DigSigBBox bb, void **sigData, CosObj sigField, CosObj sigAnnot,
	ASAtom filter, ESObject sigEngine, ESObject sigInfo);

/**
	NOTE: Supercedes DSNewSigDataProc in Acrobat 6.0. 

	Callback for DigSigHandler. Creates new signature data to 
	be used by Commit and Finish, then destroyed by DSFreeSigDataProc. 
	

	If a dialog is not used (bUI in parameters is false) then 
	exception strings are stored in the signature data, and 
	can be retrieved with DSSigDataGetErrorTextProc. 
	@param sigParams The structure containing signing parameters. 
	
	@param sigData (Filled by the method) Signature data, 
	as defined by the specific signature plug-in.
	@return A constant indicating the action to be performed on the 
	document; cancel signing, save to the same filename, or 
	save to a new filename. 
	@see DSCommitSignProc 
	@see DSNewSigDataProc 
	@see DSFinishSignProc 
	@see DSFreeSigDataProc 
	@see DSSigDataGetErrorTextProc 
*/
typedef ACCBPROTO1 DSSaveType (ACCBPROTO2 *DSNewSigDataWithParamsProc)
	( DigSigNewSigDataParams sigParams, void **sigData );

/**
	Callback for DigSigHandler. Called to put signature into 
	document in memory or to cancel. Modifies the signature 
	dictionary as needed. May allocate storage for an array 
	of signature objects. 

	The signature includes a signature dictionary written to 
	the /V attribute of the sigField and an optional appearance 
	written to the /AP dictionary of the sigAnnot. 

	See Section 8.7 in the PDF Reference for the format of the 
	signature dictionary. At least two objects in the signature 
	dictionary, ByteRange and Contents, will need to be overwritten 
	during the DSFinishSignProc callback. 
	@param pdDoc The document being signed. 
	@param sigData Signature data, as defined by the specific 
	signature plug-in. 
	@param sigField Signature field. 
	@param sigAnnot Cos object of the signature annotation. 
	
	@param offsetArray (Optional) An offset array pointing 
	to a set of 'marked' Cos objects that the signature type 
	cares about; this array includes at least the ByteRange 
	and Contents value objects. 
	@param arrayCount (Optional) Number of objects in offsetArray, 
	if used
	@return true if the signature was successfully placed in pdDoc, 
	false otherwise. 
	@see DSFinishSignProc 
	@see DSNewSigDataProc 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *DSCommitSignProc)
	(PDDoc pdDoc, void *sigData, CosObj sigField, CosObj sigAnnot, 
	 DigSigOffset *offsetArray, ASInt32 *arrayCount);

/**
	Callback for DigSigHandler. Called to calculate checksum 
	and update disk copy of document. May use information from 
	the offsetArray parameter. 
	@param pdDoc The document being signed. 
	@param sigData Signature data, as defined by the specific 
	signature plug-in. 
	@param sigField Signature field. 
	@param sigAnnot Cos object of the signature annotation. 
	
	@param offsetArray (Optional) An offset array pointing 
	to a set of 'marked' Cos objects that the signature type 
	cares about; this array includes at least the ByteRange 
	and Contents value objects. 
	@param arrayCount (Optional) Number of objects in offsetArray, 
	if used
	@return true if processing successful, false otherwise. 
	@see DSClearSigProc 
	@see DigSigOverwriteHexstring 
	@see DigSigOverwriteIntArray 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *DSFinishSignProc)
	(PDDoc pdDoc, void *sigData, CosObj sigField, CosObj sigAnnot, 
	 DigSigOffset offsetArray, ASInt32 arrayCount);

/**
	Returns an error string to describe failure during new, 
	commit, or free steps. If not NULL, then a JavaScript general 
	exception is thrown using this string. 

	The handler can set this procedure to NULL if desired. 

	The string is owned by sigData, so should be freed by DSFreeSigDataProc. 
	
	@param sigData A pointer to the signature for which the 
	error string is obtained.
	@return The error string as an ASText. 
	@see DSFreeSigDataProc 
	@see DSNewSigDataWithParamsProc 
*/
typedef ACCBPROTO1 ASText (ACCBPROTO2 *DSSigDataGetErrorTextProc)
	(void *sigData);

/**
	Callback for DigSigHandler. Frees signature data. 
	@param sigData Signature data, as defined by the specific 
	signature plug-in. The handler should check to see if sigData 
	is NULL.
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DSFreeSigDataProc)
	(void *sigData);

/**
	NOTE: Supercedes DSPropertiesProc in Acrobat 6.0. 

	Callback for DigSigHandler. Called when a user selects a 
	signature and asks for its properties. Brings up the properties 
	dialog for the signature. 
	@param pdDoc The document being signed. 
	@param sigField Signature field. 
	@param sigAnnot Cos object of the signature annotation.
	@param handlerName The sub-handler to use to validate, 
	in the event that the handler has more then one sub-handler 
	(as is the case with PubSec), and the handler is being asked 
	to validate a signature that has a different filter name 
	then its own. 
	@param sigEngine Used internally.
	@param propType One of the DSPropertyType values.  Default is kDSPropNone.
	@see DigSigUpdatePanel 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DSPropertiesExProc)
	(PDDoc pdDoc, CosObj sigField, CosObj sigAnnot, ASAtom handlerName, ESObject sigEngine, DSPropertyType propType );

/**
	NOTE: Superceded by DSPropertiesExProc in Acrobat 6.0. 

	Callback for DigSigHandler. Called when a user selects a 
	signature and asks for its properties. The handler should 
	present its own dialog that provides details concerning 
	the signature and its validation state. 
	@param pdDoc The document being signed. 
	@param sigField Signature field. 
	@param sigAnnot Cos object of the signature annotation.
	@see DigSigUpdatePanel 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DSPropertiesProc)
	(PDDoc pdDoc, CosObj sigField, CosObj sigAnnot);

/**
	Callback for DigSigHandler. Called to change appearance 
	key (AP) back to appearance that is used for the unvalidated 
	state. 
	@param pdDoc The document being signed. 
	@param sigField Signature field. 
	@param sigAnnot Cos object of the signature annotation.
	@see DSValidateSigProc 
	@see DSReValidateSigProc 
	@see DigSigGetStdXObj 
	@see DigSigFileGetEOF 
	@see DigSigMD5ByteRange 
	@see DigSigFileRead 
	@see DigSigFileSetPos 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DSUnValidateSigProc)
	(PDDoc pdDoc, CosObj sigField, CosObj sigAnnot);

/** Send a notification for this signature. The only current notifications is
	kDSNotifySetUnknown, which will cause handlers to mark the validity 
	for this signature as unknown. This is done if the document
	is dirtied or changed in a manner that would cause the current validity status
	to no longer be valid. This is not the same as the DSUnValidateProc. 
	The handler should update its cached validity values to the signature unknown state. */
typedef ACCBPROTO1 void (ACCBPROTO2 *DSNotifySigProc)
	(PDDoc pdDoc, CosObj sigField, CosObj sigAnnot, DSNotifySigType notifyType, ASBool bUI );

/* DigSig uses this callback to send notification that it has cleared the signature. This
	clears the local caching.
	@param IN/OUT?pdDoc The document being signed.
	@param IN/OUT?sigField Signature field.
	@param IN/OUT?sigAnnot Cos object of the signature annotation.
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DSClearSigProc)
	(PDDoc pdDoc, CosObj sigField, CosObj sigAnnot, ASBool bSilent );

/**
	Called to obtain an ASCab containing values that the handler 
	wants DigSig to use in the UI. This is called only after 
	the handler is called to validate the signature. 
	@param pdDoc The document being signed. 
	@param sigField Signature field. 
	@param sigAnnot Cos object of the signature annotation. 
	
	@param handlerName The name of the handler that is being 
	asked to return the data. This is not necessarly the same 
	as the value of the /Filter attribute in the signature dictionary.
	@return An ASCab object containing the properties. For a list of 
	properties, see DigSigHFT.h. 
*/
typedef ACCBPROTO1 ASCab (ACCBPROTO2 *DSGetSigPropProc)
	(PDDoc pdDoc, CosObj sigField, CosObj sigAnnot, ASAtom handlerName );

/** Sign a CosDoc (eg when signing FDF). Adobe use only. This call is
   subject to change in future releases. Introduced in Acrobat
   6.0. */
typedef ACCBPROTO1 DSRetCode (ACCBPROTO2 *DSCosDocSigSignProc)
	(DigSigCosDocSigParams sigParams );

/** Validate a CosDoc signature. Return valid state in sigInfoCab.
   Should populate the following entries in this cab:
		PROP_SigInfo_Status, PROP_SigInfo_StatusText, PROP_SigInfo_HandlerName
   Adobe use only. This call is subject to change in future releases.
   Introduced in Acrobat 6.0. */
typedef ACCBPROTO1 DSRetCode (ACCBPROTO2 *DSCosDocSigValidateProc)
	(DigSigCosDocSigParams sigParams, ASCab sigInfoCab );

/** Init signing of a data buffer, eg. when signing XML data.
	Adobe use only. This call is subject to change in future releases.
	Introduced in Acrobat 7.0.
*/
typedef ACCBPROTO1 DSRetCode (ACCBPROTO2 *DSDataBufferSigSignNewProc)
	(DigSigDataBufferSigNewParams sigParams, void** pOutDataSigContext  );

/** Finish signing of a data buffer, eg. when signing XML data.
	Must be called to destroy context, whenever signNew returns a context.
	Adobe use only. This call is subject to change in future releases.
	Introduced in Acrobat 7.0.
*/
typedef ACCBPROTO1 DSRetCode (ACCBPROTO2 *DSDataBufferSigSignFinishProc)
	( void* dataSigContext, DigSigDataParams sigParams );

/**	Validate a data buffer signature. Return valid state in sigInfoCab.
	Should populate the following entries in this cab:
		PROP_SigInfo_Status, PROP_SigInfo_StatusText, PROP_SigInfo_HandlerName.
	Adobe use only. This call is subject to change in future releases.
	Introduced in Acrobat 7.0.
*/
typedef ACCBPROTO1 DSRetCode (ACCBPROTO2 *DSDataBufferSigValidateProc)
	(DigSigDataBufferSigValidateParams sigParams, ASCab sigInfoCab );

/**
	Get the boolean value of a DigSig handler property. Return 
	true for those properties that represent functionality your 
	handler supports, false if it does not support the functionality. 
	(Some properties are available directly from the DigSigHandler.) 
	
	@param filter The name (filterKey value) of the handler 
	from which the property is obtained. 
	@param property The property whose value is obtained.
	@return The property value, true or false. 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *DSGetBoolPropertyProc)
	( ASAtom filter, DSHandlerProperty property);

/**
    Returns whether the current application user has the credential that 
    was used to create the passed in signature.

    @param handlerName  name of the handler that is being asked
    @param pdd          PDDoc containing the signature field
    @param sigField     Signature field
    @return 0 : Unknown, 1 : Yes, 2 : No
*/
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *DSIsSignerProc)
    ( ASAtom handlerName, PDDoc pdd, CosObj sigField );

/** Data structure containing callbacks that define a signature plug-in’s behavior. 
	Unused entries should be NULL
*/
typedef struct _t_DigSigHandlerRec {
	/** Size of the data structure. Must be set to sizeof(DigSigHandlerRec). */
	ASSize_t	size;
	/** Language-dependent name to show for signature type selection. */
	const char *uiName;				
	/** The value of the Filter key in the signature dictionary. */
	ASAtom filterKey;				
	/** true if this signature type can do no-appearance signatures, false otherwise. */
	ASBool canBlindSign;			
	/** true if this signature type can do encrypt-and-sign, false otherwise. */
	ASBool canEncrypt;				

	/** Called when a new document is opened. */
	DSDocOpenProc			dsDocOpen;		
	/** Called when a new document is closed. */
	DSDocCloseProc			dsDocClose;		
	/** Called when a new signature field is created. */
	DSDefaultValueProc		dsDefaultValue;	

	/** Called to gather signature or cancel. */
	DSNewSigDataProc		dsNewSigData;   
	/** Called to put signature into doc or cancel. */
	DSCommitSignProc		dsCommitSign;	
	/** Called to calculate checksum and overwrite. */
	DSFinishSignProc		dsFinishSign;	
	/** Called to free signature data. */
	DSFreeSigDataProc		dsFreeSigData;	
	/** Call to validate a signature. */
	DSValidateSigProc		dsValidateSig;
	/** Get the validity status for a signature. */
	DSGetValidStateProc		dsGetValidState;
	/** Show the signature properties dialog for this signature. */
	DSPropertiesProc		dsProperties;	
	/** Called to change AP back to unvalidated. */
	DSUnValidateSigProc		dsUnValidateSig; 

	/** Called to change AP back to whatever it was. */
	DSUnValidateSigProc		dsReValidateSig; 

	/* New in Acrobat 5.0 */
	/** Notification that signature has been cleared. */
	DSClearSigProc				dsClearSig;		
	/** Get a string describing status. */
	DSGetStatusTextProc			dsStatusText;	
	/** Used internally in Acrobat 5.0. Superceded in Acrobat 6.0 by dsNewSigDataWithParams.*/
	DSNewSigDataExProc			dsNewSigDataEx;
	/** Used internally by PubSec. */
	DSValidateSigExProc			dsValidateSigEx;
	/** Used internally by PubSec. */
	DSGetSigInfoProc			dsGetSigInfo;
	/** Used internally by EScript in Acrobat 5.0. No longer used. */
	DSNewSigEngineProc			dsNewSigEngine;
	/** Called to get a descriptive string for an error during new, commit, or free steps.*/
	DSSigDataGetErrorTextProc	dsSigDataGetErrorText;

	/* New in Acrobat 6.0 */
	/** Called to create new signature data to be used by Commit and Finish. Supercedes 
		dsNewSigDataEx. 
	*/
	DSNewSigDataWithParamsProc	dsNewSigDataWithParams;
	/** Called to show the signature properties dialog for the signature. */
	DSPropertiesExProc			dsPropertiesEx;
	/** For internal use only.*/
	DSCosDocSigSignProc			dsCosDocSigSign;
	/** For internal use only.*/
	DSCosDocSigValidateProc		dsCosDocSigValidate;
	
	/** Called to determine whether handler can validate a PDDoc field signature.*/
	DSCanValidateProc			dsCanValidate;
	/** */
	DSNotifySigProc				dsNotifySig;
	/** Used internally by PubSec. */
    DSGetSigSeedValueProc       dsGetSigSeedValue;   
	/** Used internally by PubSec. */
    DSSetSigSeedValueProc       dsSetSigSeedValue;
	/** Called to test whether your handler supports specific functionality. */
	DSGetBoolPropertyProc		dsGetBoolProperty;
	/** Called to get an ASCab containing signature properties to be used in the UI. */
	DSGetSigPropProc			dsGetSigProp;

	/* New in Acrobat 7.0 */
    /** Return whether the current user is the signer of the passed in signature */
    DSIsSignerProc              dsIsSigner;
	ASAtom						filterAlias;

	/* Data buffer signing and verification. For Adobe internal use only. */
	DSDataBufferSigSignNewProc	dsDataBufferSigSignNew;
	DSDataBufferSigSignFinishProc	dsDataBufferSigSignFinish;
	DSDataBufferSigValidateProc dsDataBufferSigValidate;

} DigSigHandlerRec, *DigSigHandler;



/*****************************************************************************\
|*																			 *|
|*				INTERFACE TO DIGSIG <--- SPECIFIC METHODS					 *|
|*																		 	 *|
\*****************************************************************************/

/*****************************************************************************\
|*								DigSigHFT types								 *|
\*****************************************************************************/

/* The DigSigHFT allows calls from a specific method plugin to the generic plugin */

enum
{
	DSDUMMYBLANKSELECTOR,
	DigSigRegisterFilterSEL,
	DigSigFileGetEOFSEL,
	DigSigFileSetPosSEL,
	DigSigFileReadSEL,
	DigSigOverwriteIntArraySEL,
	DigSigOverwriteHexstringSEL,
	DigSigOverwriteBytesSEL,
	DigSigMD5ByteRangeSEL,
	DigSigUpdatePanelSEL,
	DigSigByteToHexSEL,
	DigSigHexToByteSEL,
	DigSigSignDocSEL,
	DigSigGetStdXObjSEL,

	DigSigDrawSEL,
	DigSigKeyDownSEL,
	DigSigClickSEL,
	DigSigRightClickSEL,

	DigSigGetUniqueTitleSEL,
	DigSigDeletedSigSEL,
	DigSigAddedSigSEL,

	DigSigComparePagesSEL,				/* Export to scripting */
	DigSigCompareWordsSEL,				/* Export to scripting */
	DigSigCompareWordsRecentSEL,		/* Export for shell CompWord plug-in. */
	DigSigDoPropertiesSEL,				/* To allow Forms to do exec DigSigs properties dialog */
 	DigSigCompareWordsAndFontsRecentSEL,/* Export for shell CompWord plug-in. */

	/* New routines for Acrobat 5.0 */

	DigSigRollbackToSigSEL,
	DigSigEnumSignaturesSEL,
	DigSigDocModifiedAfterSigSEL,
	DigSigCreateStdXObjSEL,

	DSAPCreateLayeredStreamSEL,
	DSAPXObjectFromXObjListObsoleteSEL,
	DSAPXObjectFromLogoSEL,
	DSAPCreateCompositeTextXObjSEL,

	/* New routines for Acrobat 6.0 */
 
	DigSigDeletedSigExSEL,
	DigSigAddedSigExSEL,

    DigSigNewSigRefDictSEL,
    DigSigCommitSigRefDictSEL,
    DigSigFinishSigRefDictSEL,
    DigSigVerifySigRefDictSEL,
    DigSigClearSigRefDictSEL,
	
	DigSigUnregisterFilterSEL,
	DSAPCreateLayeredStreamExSEL,

    DigSigIsSigSignedSEL,          // This is called by AcroForms

    DigSigRegisterObserverSEL,
	DigSigUnregisterObserverSEL,

    DigSigGetDocAuthorSignatureSEL,

	DigSigComparePagesExSEL,
	DigSigCompareWordsExSEL,

    DigSigVerifySigSEL,
    DigSigClearSigSEL,
    DigSigGetDocMDPSettingSEL,
    DigSigGetUbiquitySigSEL,
	
	/* New routines for Acrobat 7.0 */
	DSAPXObjectFromXObjListSEL,

	DSNUMSELECTORSPlusOne
};

#define DigSigHFT_NUMSELECTORS (DSNUMSELECTORSPlusOne - 1)

/**
	Registers a signing method plug-in. A signing plug-in must 
	call this method before making any signatures. This object 
	should not be destroyed until after it is unregistered or 
	until exit. 
	@param owner The handler plug-in identifier, assigned 
	on initialization. 
	@param digSigHandler A structure specifying the name of 
	the filter and the functions to call to create and validate 
	signatures.
	@see DigSigUnregisterFilter 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DigSigRegisterFilterSELPROTO)
	(ExtensionID owner, DigSigHandler digSigHandler);
#define DigSigRegisterFilter (*((DigSigRegisterFilterSELPROTO)(gDigSigHFT[DigSigRegisterFilterSEL])))

/**
	Gets the number of bytes in an ASFile. Serves as a wrapper 
	for the ASFileGetEOF method. 
	@param asFile The ASFile whose length is obtained.
	@return Number of bytes in the ASFile, or 0 if an error is encountered. 
	
	@see DigSigFileSetPos 
	@see DigSigFileRead 
	@see DigSigOverwriteIntArray 
	@see DigSigOverwriteHexstring 
	@see DigSigOverwriteBytes 
*/
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *DigSigFileGetEOFSELPROTO)
	(ASFile asFile);
#define DigSigFileGetEOF (*((DigSigFileGetEOFSELPROTO)(gDigSigHFT[DigSigFileGetEOFSEL])))

/**
	Reopens an ASFile for reading and sets the file position. 
	
	@param asFile The ASFile to position. 
	@param pos The byte offset to a position in asFile
	@see DigSigFileGetEOF 
	@see DigSigFileRead 
	@see DigSigOverwriteIntArray 
	@see DigSigOverwriteHexstring 
	@see DigSigOverwriteBytes 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DigSigFileSetPosSELPROTO)
	(ASFile asFile, ASInt32 pos);
#define DigSigFileSetPos (*((DigSigFileSetPosSELPROTO)(gDigSigHFT[DigSigFileSetPosSEL])))

/**
	Reads from an ASFile. This is a wrapper function for the 
	ASFileRead method. 
	@param asFile The ASFile to read. 
	@param p Pointer to a buffer. 
	@param count Number of bytes to read into the buffer.
	@return Number of bytes read, or 0 if unsuccessful. 
	@see DigSigFileGetEOF 
	@see DigSigFileSetPos 
	@see DigSigOverwriteIntArray 
	@see DigSigOverwriteHexstring 
	@see DigSigOverwriteBytes 
*/
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *DigSigFileReadSELPROTO)
	(ASFile asFile, char *p, ASInt32 count);
#define DigSigFileRead (*((DigSigFileReadSELPROTO)(gDigSigHFT[DigSigFileReadSEL])))

/**
	Overwrites part of asFile with an integer array. Reopens 
	the file for write, positions to the specified place, formats 
	the array as characters, and writes. This method is used 
	to overwrite the /ByteRange entry in the signature dictionary. 
	
	@param asFile The ASFile to write to. 
	@param digSigOffset A structure specifying the byte position 
	and size to overwrite.
	- Writes padding blanks to the file 
	if formatted length is less than this length.
	- Does not 
	write and returns 0 if the formatted length is longer than 
	this length.
	- Does not write and returns 0 if the formatted 
	length is greater than 8200 bytes. 
	@param cosObj The Cos string to be written.
	@return The number of bytes written. 
	@see DigSigCosObjOverwrite 
	@see DigSigFileGetEOF 
	@see DigSigFileSetPos 
	@see DigSigFileRead 
	@see DigSigOverwriteHexstring 
	@see DigSigOverwriteBytes 
*/
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *DigSigOverwriteIntArraySELPROTO)
	(ASFile asFile, const DigSigOffset digSigOffset, const CosObj cosObj);
#define DigSigOverwriteIntArray (*((DigSigOverwriteIntArraySELPROTO)(gDigSigHFT[DigSigOverwriteIntArraySEL])))

/**
	Overwrites part of a file with a hex string. Reopens the 
	file for write, positions to the specified location, formats 
	the string as characters, and writes. This method is used 
	to overwrite the /Contents entry in the signature dictionary. 
	
	@param asFile The ASFile to write to. 
	@param digSigOffset A structure specifying the byte position 
	and size to overwrite.
	- Writes padding blanks to the file 
	if formatted length is less than this length.
	- Does not 
	write and returns 0 if the formatted length is longer than 
	this length.
	- Does not write and returns 0 if the formatted 
	length is greater than 8200 bytes. 
	@param cosObj The Cos string to be written.
	@return The number of bytes written. 
	@see DigSigCosObjOverwrite 
	@see DigSigFileGetEOF 
	@see DigSigFileSetPos 
	@see DigSigFileRead 
	@see DigSigOverwriteIntArray 
	@see DigSigOverwriteBytes 
*/
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *DigSigOverwriteHexstringSELPROTO)
	(ASFile asFile, const DigSigOffset digSigOffset, const CosObj cosObj);
#define DigSigOverwriteHexstring (*((DigSigOverwriteHexstringSELPROTO)(gDigSigHFT[DigSigOverwriteHexstringSEL])))

/**
	Overwrites any or all entries in a signature dictionary. 
	Reopens the file for write, positions to the specified place, 
	and writes exactly the number of bytes specified in digSigOffset. 
	

	NOTE: To overwrite a text string entry, such as /Reason, 
	the string must be in the format "< xxx >", not "xxx". 
	@param asFile The ASFile to write to. 
	@param digSigOffset A structure specifying the byte position 
	and size to overwrite. 
	@param p A byte string to be written. Must be in the correct 
	format, given the type of entry as defined in Section 8.7 
	in the PDF Reference.
	@return The number of bytes written, or 0 if unsuccessful. 
	@see DigSigCosObjOverwrite 
	@see DigSigFileGetEOF 
	@see DigSigFileSetPos 
	@see DigSigFileRead 
	@see DigSigOverwriteIntArray 
	@see DigSigOverwriteHexstring 
*/
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *DigSigOverwriteBytesSELPROTO)
	(ASFile asFile, const DigSigOffset digSigOffset, const char *p);
#define DigSigOverwriteBytes (*((DigSigOverwriteBytesSELPROTO)(gDigSigHFT[DigSigOverwriteBytesSEL])))

/**
	Calculates the MD5 hash function over a set of byte ranges 
	in a file. 
	@param asFile The file over which the hash function is 
	calculated. 
	@param byteRange An array of pairs of integers specifying 
	byte offset and length of one or more subsets of the file. 
	The hash function is calculated over all these subsets, 
	concatenated in order. 
	@param md5hash A pointer to a buffer that receives the 
	16-byte hash value.
	@return The MD-5 hash function over a set of byte ranges in asFile. 
	Also returns the total number of bytes over which the hash 
	is calculated. Returns 0 if the byte range array has an 
	odd number of elements or if unsuccessful. 
*/
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *DigSigMD5ByteRangeSELPROTO) 
	(ASFile asFile, CosObj byteRange, char *md5hash);
#define DigSigMD5ByteRange (*((DigSigMD5ByteRangeSELPROTO)(gDigSigHFT[DigSigMD5ByteRangeSEL])))

/**
	Updates the signature panel, if any, associated with pdDoc. 
	
	@param pdDoc The document whose panel is to be updated.
	Verifies a digital signature. 
	@param pdDoc The document. 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DigSigUpdatePanelSELPROTO)
	(PDDoc pdDoc);
#define DigSigUpdatePanel (*((DigSigUpdatePanelSELPROTO)(gDigSigHFT[DigSigUpdatePanelSEL])))

/**
	Converts a byte string to a PDF hex string. 
	@param byteP A pointer to the byte string. 
	@param hexP (Filled by the method) A pointer to an output 
	buffer to hold the hex string. Must be at least (length 
	* 2) + 3 bytes. The string begins with '\<' and ends with 
	'\>', followed by a NULL character. For example, the two 
	byte string 'A/' is converted to '\<412f\>\\0.' 
	@param length The length of the byte string, in bytes
	@see DigSigHexToByte 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DigSigByteToHexSELPROTO)
	(unsigned char * byteP, unsigned char * hexP, ASInt32 length);
#define DigSigByteToHex (*((DigSigByteToHexSELPROTO)(gDigSigHFT[DigSigByteToHexSEL])))

/**
	Converts a PDF hex string to a byte string. For example, 
	the 6-byte string <412f> converts to the 2-byte string (A/). 
	

	The first byte of the hex string is ignored. The last byte 
	of an even length hex string is ignored. 

	Middle bytes outside the ranges 0-9, a-f, and A-F are treated 
	as 0. Pairs of middle bytes are converted to byte values 
	0-255 and stored in the output buffer. 
	@param hexP Pointer to the hex string. The string must 
	begin with '<' and end with '>', optionally followed by 
	a NULL character. 
	@param byteP (Filled by the method) Pointer to an output 
	buffer to hold the byte string. Must be at least (length 
	- 2) / 2 bytes long. 
	@param length Length of the hex string in bytes, not including 
	any trailing NULL character, that is, strlen(hexP).
	@see DigSigByteToHex 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DigSigHexToByteSELPROTO)
	(unsigned char * hexP, unsigned char * byteP, ASInt32 length);
#define DigSigHexToByte (*((DigSigHexToByteSELPROTO)(gDigSigHFT[DigSigHexToByteSEL])))

/** Signs and saves a document, using the specified field and signing method. If
	sigField is NULL, makes a field of size 0x0 points.
	@param pdDoc IN/OUT? The document to be signed.
	@param sigField IN/OUT? The field to be signed or CosNull.
	@param filterKey IN/OUT? The signing method to use.
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DigSigSignDocSELPROTO)
	(PDDoc pdDoc, CosObj sigField, ASAtom filterKey);
#define DigSigSignDoc (*((DigSigSignDocSELPROTO)(gDigSigHFT[DigSigSignDocSEL])))

/* Return an XObject that is created and stored in the PDF file.
   Bounding box of XObject is 100 x 100.
   dsXObjType must be one of DSBlankXObj, DSUnknownXObj or DSInvalidXObj */
/**
	Gets a Cos XObject for one of the standard signature graphics: 
	blank, question mark, and cross. 

	Creates an AcroForm dictionary if none exists in the document, 
	makes a Default Resources (DR) dictionary if none exists, 
	creates an XObject dictionary if none exists, and creates 
	three standard XObjects: DSBlankXObj, DSUnknownXObj, and 
	DSInvalidXObj if they do not exist. These objects may be 
	used to modify the appearance of a digital signature. Each 
	object has a bounding box of 100 x 100 points and an identity 
	transformation matrix. 
	@param cosDoc The document in which to include the standard 
	XObject. 
	@param dsXObjType The type of object to get. 
	@return Returns the CosObj of the desired type, or CosNewNull if 
	unsuccessful. 
*/
typedef ACCBPROTO1 CosObj (ACCBPROTO2 *DigSigGetStdXObjSELPROTO)
	(CosDoc cosDoc, DSXObjType dsXObjType);
#define DigSigGetStdXObj (*((DigSigGetStdXObjSELPROTO)(gDigSigHFT[DigSigGetStdXObjSEL])))

/** Causes the signature to be redrawn.
	@param pdAnnot IN/OUT? The annotation the user clicked on or tabbed into
	@param avPV IN/OUT? The page view that contains the annotation
	@param bIsSelected IN/OUT? true if the annotation is selected, false otherwise
	@see DigSigHexToByte
	@see DigSigRightClick
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DigSigDrawSELPROTO)
	(PDAnnot pdAnnot, AVPageView avPV, ASBool bIsSelected);
#define DigSigDraw (*((DigSigDrawSELPROTO)(gDigSigHFT[DigSigDrawSEL])))
/**
	The AcroForm plug-in calls this method when the user tabs 
	to a signature annotation and activates it by pressing the 
	spacebar or Enter key, which is equivalent to a left-mouse 
	click. The parameters parallel those of the AVAnnotHandlerDoKeyDownProc 
	callback. If the key pressed is an ASCII \<CR\>, this method 
	selects the annotation. 
	@param pdAnnot The annotation the user clicked on. 
	@param avPV The current page view. 
	@param nKey The key pressed by the user. 
	@param nFlags Indicates which modifier keys are pressed, 
	if any. Must be an OR of the Modifier Keys values, which 
	are the following:
	- AV_COMMAND
	- AV_OPTION
	- AV_CONTROL 
	- AV_SHIFT 
	@see DigSigDraw 
	@see DigSigClick 
	@see DigSigRightClick 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DigSigKeyDownSELPROTO)
	(PDAnnot pdAnnot, AVPageView avPV, ASUns16 nKey, ASInt16 nFlags);
#define DigSigKeyDown (*((DigSigKeyDownSELPROTO)(gDigSigHFT[DigSigKeyDownSEL])))

/**
	The AcroForm plug-in calls this method when a user left-clicks 
	inside a signature annotation. 
	@param pdAnnot The annotation the user clicked on 
	@param avPV The page view that contains the annotation. 
	
	@param nX The x-coordinate of the mouse click, specified 
	in device space coordinates. 
	@param nY The y-coordinate of the mouse click, specified 
	in device space coordinates. 
	@param nFlags Indicates which modifier keys are pressed, 
	if any. Must be an OR of the Modifier Keys values, which 
	are the following:
	- AV_COMMAND
	- AV_OPTION
	- AV_CONTROL 
	- AV_SHIFT 
	@param nClicks The number of clicks.
	@see DigSigDraw 
	@see DigSigHexToByte 
	@see DigSigRightClick 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DigSigClickSELPROTO)
	(PDAnnot pdAnnot, AVPageView avPV, ASInt16 nX, ASInt16 nY, ASInt16 nFlags, ASInt16 nClicks);
#define DigSigClick (*((DigSigClickSELPROTO)(gDigSigHFT[DigSigClickSEL])))

/**
	Invokes the signature-panel pull-right menu and allows the 
	user to select an action. 
	@param pdAnnot The annotation the user clicked on. 
	@param avPV The page view the annotation is within. 
	@param nX The x-coordinate of the mouse click, specified 
	in device space coordinates. 
	@param nY The y-coordinate of the mouse click, specified 
	in device space coordinates 
	@param nFlags Indicates which modifier keys are pressed, 
	if any. Must be an OR of the Modifier Keys values, which 
	are the following:
	- AV_COMMAND
	- AV_OPTION
	- AV_CONTROL 
	- AV_SHIFT 
	@param nClicks The number of clicks.
	@see DigSigDraw 
	@see DigSigHexToByte 
	@see DigSigClick 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DigSigRightClickSELPROTO)
	(PDAnnot pdAnnot, AVPageView avPV, ASInt16 nX, ASInt16 nY, ASInt16 nFlags, ASInt16 nClicks);
#define DigSigRightClick (*((DigSigRightClickSELPROTO)(gDigSigHFT[DigSigRightClickSEL])))

/**
	Gets a character string that is suitable as the field name 
	for a new signature field. This is guaranteed not to duplicate 
	an existing field name. Typically, this name would be of 
	the form XXXXnnn, where XXXX is the word 'signature' in 
	a local Latin-alphabet language, and nnn is a unique integer. 
	
	@param cosDoc The document that will contain the new field.
	@return The unique field name string. 
*/
typedef ACCBPROTO1 char * (ACCBPROTO2 *DigSigGetUniqueTitleSELPROTO)
	(CosDoc cosDoc);
#define DigSigGetUniqueTitle (*((DigSigGetUniqueTitleSELPROTO)(gDigSigHFT[DigSigGetUniqueTitleSEL])))
/**
	Recalculates the number of signature fields and redraws 
	the signature panel after any signature is deleted. 

	Called by the AcroForm plug-in or any agent that deletes 
	a signature field. 
	@param pdDoc The document that contains a signature field.
	@see DigSigAddedSig 
	@see DigSigDeletedSigEx 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DigSigDeletedSigSELPROTO)
	(PDDoc pdDoc);
#define DigSigDeletedSig (*((DigSigDeletedSigSELPROTO)(gDigSigHFT[DigSigDeletedSigSEL])))
/**
	Recalculates the number of signature fields and redraws 
	the signature panel after any signature is added. 

	Called by the AcroForm plug-in or any agent that adds a 
	signature field. 
	@param pdDoc The document that contains the signature 
	field.
	@see DigSigAddedSigEx 
	@see DigSigDeletedSig 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DigSigAddedSigSELPROTO)
	(PDDoc pdDoc);
#define DigSigAddedSig (*((DigSigAddedSigSELPROTO)(gDigSigHFT[DigSigAddedSigSEL])))

/**
	Compares the pages of two documents, producing a third document 
	of the differences. 

	NOTE: Superceded by DigSigComparePagesEx in Acrobat 6.0. 
	
	@param docA First document to compare. 
	@param docB Second document to compare. 
	@param insertDiffs Not used.
	@return true if the pages of the documents are identical, false 
	otherwise. 
	@see DigSigCompareWords  
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *DigSigComparePagesSELPROTO)
	(PDDoc docA, PDDoc docB, ASBool insertDiffs);
#define DigSigComparePages (*((DigSigComparePagesSELPROTO)(gDigSigHFT[DigSigComparePagesSEL])))

/**
	Compares the words of two documents, producing a third document 
	of the differences. 

	NOTE: Superceded by DigSigCompareWordsEx in Acrobat 6.0. 
	
	@param docA First document being compared. 
	@param docB Second document being compared. 
	@param iUseFonts If true, the font name, size, and color 
	of each character in a word are considered when matching 
	against other words. If false, these attributes are ignored.
	@return true if the words of the documents are identical, false 
	otherwise. 
	@see DigSigComparePages 
	@see DigSigCompareWordsAndFontsRecent 
	@see DigSigCompareWordsEx 
	@see DigSigCompareWordsRecent 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *DigSigCompareWordsSELPROTO)
	(PDDoc docA, PDDoc docB, ASBool iUseFonts);
#define DigSigCompareWords (*((DigSigCompareWordsSELPROTO)(gDigSigHFT[DigSigCompareWordsSEL])))

/**
	Compares the words of two documents, producing a third document 
	of the differences. 

	NOTE: Superceded by DigSigCompareWordsEx in Acrobat 6.0. 
	
	@param docA First document being compared. 
	@param docB Second document being compared. 
	@param iUseFonts If true, the font name, size, and color 
	of each character in a word are considered when matching 
	against other words. If false, these attributes are ignored.
	@return true if the words of the documents are identical, false 
	otherwise. 
	@see DigSigComparePages 
	@see DigSigCompareWords 
	@see DigSigCompareWordsAndFontsRecent 
	@see DigSigCompareWordsEx 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DigSigCompareWordsRecentSELPROTO)
	(PDDoc docA, PDDoc docB, ASBool iUseFonts);
#define DigSigCompareWordsRecent (*((DigSigCompareWordsRecentSELPROTO)(gDigSigHFT[DigSigCompareWordsRecentSEL])))

/**
	Opens DigSig's property dialog. 
	@param avDoc The document that contained the signature 
	field. 
	@param sigField The signature field of the document.
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DigSigDoPropertiesSELPROTO)
	(AVDoc avDoc, CosObj sigField);
#define DigSigDoProperties (*((DigSigDoPropertiesSELPROTO)(gDigSigHFT[DigSigDoPropertiesSEL])))

/**
	Compares the words of two documents, taking into account 
	possible font changes, and produced a third document of 
	the differences. 

	NOTE: Superceded by DigSigCompareWordsEx in Acrobat 6.0. 
	
	@param docA First document being compared. 
	@param docB Second document being compared. 
	@param iUseFonts If true, the font name, size, and color 
	of each character in a word are considered when matching 
	against other words. If false, these attributes are ignored.
	@return true if the words of the documents are identical, false 
	otherwise. 
	@see DigSigComparePages 
	@see DigSigCompareWords 
	@see DigSigCompareWordsEx 
	@see DigSigCompareWordsRecent 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DigSigCompareWordsAndFontsRecentSELPROTO)
	(PDDoc docA, PDDoc docB, ASBool iUseFonts);
#define DigSigCompareWordsAndFontsRecent (*((DigSigCompareWordsAndFontsRecentSELPROTO)(gDigSigHFT[DigSigCompareWordsAndFontsRecentSEL])))

/*************************************************************************
 * New routines for Acrobat 5.0
 ************************************************************************/

/**
	Creates a new temporary file that corresponds to the state 
	of the file after the specified signature was applied. 
	@param pdDoc The document that contained the signature 
	field. 
	@param sigField The signature field of the document.
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DigSigRollbackToSigSELPROTO)
	(PDDoc pdDoc, CosObj sigField);
#define DigSigRollbackToSig (*((DigSigRollbackToSigSELPROTO)(gDigSigHFT[DigSigRollbackToSigSEL])))

/**
	User-supplied callback that is passed in the call to DigSigEnumSignatures. 
	DigSig calls this once for each existing signature. 
	@param pdDoc The document that contains the signature 
	field. 
	@param sigField The signature field. 
	@param clientData Pointer to user-supplied data to pass 
	to proc each time it is called.
	@return true to continue enumeration, false otherwise. 
	@see DigSigEnumSignatures 
*/
typedef ACCB1 ASBool ACCB2 (*DigSigEnumProc) ( PDDoc pdDoc, CosObj sigField, void *clientData );

/**
	Enumerates the signature fields (signed and unsigned) in 
	the file. 
	@param pdDoc The document that contains the signature 
	field. 
	@param proc The procedure to call once for each existing 
	signature. 
	@param clientData Pointer to user-supplied data to pass 
	to proc each time it is called.
	@see DigSigEnumProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DigSigEnumSignaturesSELPROTO) ( PDDoc pdDoc, DigSigEnumProc proc, void *clientData );
#define DigSigEnumSignatures (*((DigSigEnumSignaturesSELPROTO)(gDigSigHFT[DigSigEnumSignaturesSEL])))

/**
	Tests whether a document has been modified since being signed 
	with the specified signature. 
	@param doc The document containing the signature field. 
	@param sigField The signature field.
	@return ASBool true if the document has been modified, false otherwise. 
	
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *DigSigDocModifiedAfterSigSELPROTO) ( PDDoc doc, CosObj sigField );
#define DigSigDocModifiedAfterSig (*((DigSigDocModifiedAfterSigSELPROTO)(gDigSigHFT[DigSigDocModifiedAfterSigSEL])))

/**
	Returns a new XObject with the specified bounding box. Unlike 
	the XObject returned by DigSigGetStdXObj, DigSig does not 
	attach the XObject returned by this function to the CosDoc. 
	
	@param cosDoc The document containing the new object. 	
	@param pBBoxRec A pointer to desired bounding box. 
	@param dsXObjType The type of object to create. 
	@return A CosObj containing the new XObject form, or CosNewNull 
	if unsuccessful 
	@see DigSigGetStdXObj 
*/
typedef ACCBPROTO1 CosObj (ACCBPROTO2 *DigSigCreateStdXObjSELPROTO)
	( const CosDoc cosDoc, const ASFixedRect* const pBBoxRec, DSXObjType dsXObjType);
#define DigSigCreateStdXObj (*((DigSigCreateStdXObjSELPROTO)(gDigSigHFT[DigSigCreateStdXObjSEL])))

/**

NOTE: Superceded by DigSigAPCreateLayeredStreamEx in Acrobat 6.0.
Creates a stream that is composed of either four or five layers:

 * DigSigAPCreateLayeredStream
 * Creates a stream that is composed of four or five layers:
 *	layer n0 - background, inherited from form annot background
 *	layer n1 - middle, question mark or equivalent (uses default if layer1XObject is CosNull)
 *	layer n2 - main, text and appearance (must be provided)
 *	layer n3 - top, blank or X (uses default if layer1XObject is CosNull)
 *	layer nN - optional very top layer, used for text showing validity state (none if CosNull)
*/
typedef ACCBPROTO1 CosObj (ACCBPROTO2 *DSAPCreateLayeredStreamSELPROTO) ( const CosDoc cosDoc, 
							const CosObj layer1XObject, ASFixedMatrixP layer1Matrix,
							const CosObj layer2XObject, ASFixedMatrixP layer2Matrix,
							const CosObj layer3XObject, ASFixedMatrixP layer3Matrix,
							const CosObj layerNXObject, ASFixedMatrixP layerNMatrix, ASInt16 layerNNum,
							AFPDWidgetBorder border, 
							PDColorValue cBorder, PDColorValue cBackGnd,
							ASFixed width, ASFixed height,
							PDRotate pdr );
#define DigSigAPCreateLayeredStream (*((DSAPCreateLayeredStreamSELPROTO)(gDigSigHFT[DSAPCreateLayeredStreamSEL])))

/** Justification of XObjects.
@see DigSigAPXObjectFromXObjList
*/
typedef enum {
	/** Left justification. */
	DSLeftQ = 0,
	/**Center justification.  */
	DSCenterQ,
	/** Right justification.  */
	DSRightQ
} DSQuadding;

/**
	Structure used in DigSigAPXObjectFromXObjList. 
	@see DigSigAPXObjectFromXObjList
*/
typedef struct _t_DSAPXObjEntryRec {
	/** Next list entry.  */
	struct _t_DSAPXObjEntryRec *next;	
	/** If not null, use this XObject as entry.  */
	CosObj xobj;					
	/** Rect relative to bbox in which to render object.  */
	ASFixedRect rect;				
	/** If not empty, render this string (can contain new lines).  */
	ASText text;					
	/** Font size, 0 for auto */
	ASFixed textSize;				
	/** Justification of XObject (not all options supported). */
	DSQuadding xjustify;			
	/** Justification of XObject (not all options supported). */
	DSQuadding yjustify;			
	/** Scale XObject within rect. */
	ASFixed scale;					
	/** If XObject is Subtype XObject then XObject is merged up. */
	ASBool bMerge;					
	/** If true and bMerge and XObject then destroy XObject. */
	ASBool bDestroy;					
} DSAPXObjEntryRec, *DSAPXObjEntry;

/** Creates a new XObject from the list of DSAPXObjEntry objects. Each object in the list is
	either an existing stream or a string that will be laid out and reflowed into a new stream.
	The streams are then turned into XObjects. Controls allow you to adjust the vertical
	position of each stream within bbox. If bMerge is true then this method combines the
	new XObjects into one large XObject, and the sub XObjects are destroyed. Otherwise the
	method leaves the component XObjects intact and new XObjects are created but not
	destroyed for the text entries. Text streams use the text attributes of ta.
	@param cosDoc The document in which to include the standard XObject.
	@param bbox The bounding box for the signature.
	@param objEntry A list of DSAPXObjEntry objects.
	@param ta The text appearance. For an explanation of the TextAppearanceP type, see "AcroForm
	Declarations" in Acrobat Forms API Reference.
	@param border The border style for the signature, as returned by AFPDWidgetGetBorder.
	For an explanation of the AFPDWidgetBorder type, see "AcroForm Declarations" in Acrobat Forms API Reference.
	@param bMerge When false, keep sub XObjects. When true, combine them all into
	one large one.
	@return XObject composed from list.
	@see DigSigAPCreateLayeredStream
	@see DigSigAPXObjectFromLogo
*/

typedef enum {
	kDSMerge = 0x0001,
	kDSForceEmbed = 0x0002

} DSAPXObjectFromXObjectListFlags;

typedef ACCBPROTO1 CosObj (ACCBPROTO2 *DSAPXObjectFromXObjListSELPROTO) ( CosDoc cosDoc,
						const ASFixedRect* const bbox,
						DSAPXObjEntry objEntry,
						TextAppearanceP ta,
						size_t taSize,
						AFPDWidgetBorder border,
						ASUns32 flags);
#define DigSigAPXObjectFromXObjList (*((DSAPXObjectFromXObjListSELPROTO)(gDigSigHFT[DSAPXObjectFromXObjListSEL])))


/** 
	Takes text for a stream logoStr with bounding box logoBBox and fits it precisely to
	bbox. Performs uniform x and y scaling, and x-y translation. Can raise an exception. This
	method might throw, and should be wrapped in a DURING/HANDLER block.
	@param cosDoc The document.
	@param logoStr The stream containing the text.
	@param logoStrSize The length of the logo string, or 0 to calculate the length automatically.
	@param logoBBox The bounding box for logoStr.
	@param bbox The bounding rectangle of the result.
	@return A Cos object XObject that contains the logo.
	@see DigSigAPCreateLayeredStream
	@see DigSigAPXObjectFromXObjList
*/
typedef ACCBPROTO1 CosObj (ACCBPROTO2 *DSAPXObjectFromLogoSELPROTO) ( const CosDoc cosDoc, 
						   const char* logoStr, const ASInt32 logoStrSize,
						   const ASFixedRect* const logoBBox,
						   const ASFixedRect* const bbox );
#define DigSigAPXObjectFromLogo (*((DSAPXObjectFromLogoSELPROTO)(gDigSigHFT[DSAPXObjectFromLogoSEL])))


/** A linked list of text items to be merged together */
typedef struct _t_DSAPTextEntryRec {
	/** next entry in list, or NULL if last entry */
	struct _t_DSAPTextEntryRec *next;	
	/** ratio of height to overall height of bbox (0x00010000 is 100%) */
	ASFixed heightRatio;			
	/** text to render in this object - caller owns memory */
	ASText text;				
} DSAPTextEntryRec, *DSAPTextEntry;

/** Create a new XObj that consists of vertically stacked blocks of text
	where each block is auto sized to fit its own bbox.
*/
typedef ACCBPROTO1 CosObj (ACCBPROTO2 *DSAPCreateCompositeTextXObjSELPROTO) ( const CosDoc cosDoc, 
							const DSAPTextEntry inText,
							const ASFixedRect* const bbox,	/* already compensated for rotation and position within annot*/
							const CosObj sigField, const CosObj sigAnnot );
#define DigSigAPCreateCompositeTextXObj (*((DSAPCreateCompositeTextXObjSELPROTO)(gDigSigHFT[DSAPCreateCompositeTextXObjSEL])))

/*
** New routines for Acrobat 6.0
*/

/**
	Recalculates the number of signature fields and redraws 
	the signature panel after a specified signature is deleted. 
	

	This version, added in Acrobat 6.0, is more efficient than 
	DigSigDeletedSig, but requires that you specify the signature 
	field that is being deleted. 
	@param pdDoc The document that contained the signature 
	field. 
	@param cosField The signature field that was deleted.
	@see DigSigAddedSigEx 
	@see DigSigDeletedSig 	
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DigSigDeletedSigExSELPROTO)
	(PDDoc pdDoc, CosObj cosField);
#define DigSigDeletedSigEx (*((DigSigDeletedSigExSELPROTO)(gDigSigHFT[DigSigDeletedSigExSEL])))

/**
	Recalculates the number of signature fields and redraws 
	the signature panel after a specified signature field is 
	added. 

	This version, added in Acrobat 6.0, is more efficient than 
	DigSigAddedSig, but requires that you specify the signature 
	field that is being added. 
	@param pdDoc The document that contains the signature 
	field. 
	@param cosField The signature field that was added.
	@see DigSigAddedSig 
	@see DigSigDeletedSigEx 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DigSigAddedSigExSELPROTO)
	(PDDoc pdDoc, CosObj cosField);
#define DigSigAddedSigEx (*((DigSigAddedSigExSELPROTO)(gDigSigHFT[DigSigAddedSigExSEL])))

/**
	Unregisters a signing method plug-in. The caller should 
	deallocate the DigSigHandler object after making this call. 
	
	@param digSigHandler A structure specifying the name of 
	the filter and the functions to call to create and validate 
	signatures.
	@see DigSigRegisterFilter 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DigSigUnregisterFilterSELPROTO)
	( DigSigHandler digSigHandler );
#define DigSigUnregisterFilter (*((DigSigUnregisterFilterSELPROTO)(gDigSigHFT[DigSigUnregisterFilterSEL])))


/************************************************************************************
 * New routines for Acrobat 6.0
 ***********************************************************************************/

/** Error constants for signature reference dictionary procedures. */
typedef enum {
	kDSSigRefErrNone=0,
	/**	Missing required plug-in or software module.
		Software module is named by errText in
		DSSigRefDictErrParams.
	*/
	kDSSigRefErrMissingPlugin,
	/** New unsupported version of signature.  */
	kDSSigRefErrNewVersion,
	/** Old unsupported version of signature. */
	kDSSigRefErrOldVersion,
	/** */
	kDSSigRefErrEnumSize
} DSSigRefErrCode;

/** A structure that contains information about exceptions that occurred in signature
	reference dictionary procedures. 
*/
typedef struct _t_DSSigRefDictErrParamsRec {
	/** The size of this structure. */
	ASSize_t size;				/* Size of this struct */
	/** The exception code. */
	DSSigRefErrCode errCode;	/* Exception code. */
	/** Text associated with the error code. If errCode is
		kDSSigRefErrMissingPlugin, the name of a software module.
		Must point to an initialized ASText object.
	*/
	ASText errText;				/* Name of software module, if errCode == kDSSigRefErrMissingPlugin. Must point to initialized ASText object. */
} DSSigRefDictErrParamsRec, *DSSigRefDictErrParams;

/** Parameters used by methods that create a signature reference dictionary. See Section 8.7,
	"Digital Signatures," in the PDF Reference for more information on signature reference
	dictionaries. 
*/
typedef struct _t_DSSigRefDictParamsRec {
	/** Size of the data structure. Must be set to
		sizeof(DSSigRefDictParamsRec).
	*/
	ASSize_t size;				/* Size of this struct */
	/** The document containing the object to be signed. */
	CosDoc cosDoc;				/* CosDoc in which to-be-signed object resides */
	/** The root object to be signed. */
	CosObj rootObj;				/* Root object that is being signed */
	/** The signature dictionary that this reference dictionary will be part of. */
    CosObj sigDict;             /* References will be part of this sigDict */
    /** The transform method name. Possible values are DocMDP, FieldMDP, UR and Identity.*/
	ASAtom transformMethod;		/* Transform name */
	/** The transform parameters, which are specific to each transform method. */
	CosObj transformParams;		/* Parameters to transform */
	/** true if the dictionary should be indirect, false if it should be direct. */
	ASBool bIndirect;			/* Set true if refDict should be indirect */
} DSSigRefDictParamsRec, *DSSigRefDictParams;

/**
	Begins the process of creating a new signature reference 
	dictionary containing an object digest. The caller fills 
	in the values of the refParams structure. 

	In this method, no persistent changes are made to the document; 
	therefore, it is safe to cancel the creation of the reference 
	dictionary without any cleanup. To save the file, DigSigCommitSigRefDict 
	should be called next. 
	@param refParams A structure containing information about 
	the signature reference dictionary; must be filled in by 
	the caller. 
	@param errParams (Filled by the method) A structure containing 
	information about exceptions that occurred.
	@return kDSTrue on success; kDSFalse on parameter errors. 
	kDSException means an exception occurred; information can 
	be found in errParams. 
	@see DigSigClearSigRefDict 
	@see DigSigCommitSigRefDict 
	@see DigSigFinishSigRefDict 
	@see DigSigVerifySigRefDict 
*/
typedef ACCBPROTO1 DSRetCode (ACCBPROTO2 *DigSigNewSigRefDictSELPROTO)
	( DSSigRefDictParams refParams, DSSigRefDictErrParams errParams );
#define DigSigNewSigRefDict (*((DigSigNewSigRefDictSELPROTO)(gDigSigHFT[DigSigNewSigRefDictSEL])))

/**
	Adds a signature reference dictionary to the document and 
	saves the document. 

	This method uses the information that was provided in the 
	DSSigRefDictErrParams parameter block during the call toDigSigNewSigRefDict.transformMethod 
	and sigDict should match the values provided at that time. 
	

	After this operation, some of the values in the reference 
	dictionary are still dummy values, including DigestValue 
	and DigestLocation (see Section 8.7, "Digital Signatures" 
	in the PDF Reference for more information). DigSigFinishSigRefDict 
	should be called next to fill in these values. 

	If signing is cancelled after this operation, DigSigClearSigRefDict 
	should be called to remove all references to the reference 
	dictionary from the document. 
	@param transformMethod The transform method. 
	@param sigDict The signature dictionary that contains 
	the signature reference dictionary. 
	@param pOutRefDict (Filled by the method) A pointer to the 
	committed signature reference dictionary.
	@return kDSTrue on success; kDSFalse on parameter error; kDSException 
	for all other errors. 
	@see DigSigClearSigRefDict 
	@see DigSigFinishSigRefDict 
	@see DigSigNewSigRefDict 
	@see DigSigVerifySigRefDict 
*/
typedef ACCBPROTO1 DSRetCode (ACCBPROTO2 *DigSigCommitSigRefDictSELPROTO)
    ( ASAtom transformMethod, CosObj sigDict, CosObj *pOutRefDict );
#define DigSigCommitSigRefDict (*((DigSigCommitSigRefDictSELPROTO)(gDigSigHFT[DigSigCommitSigRefDictSEL])))

/**
	Computes the digest value and stores it in the reference 
	dictionary. It should be called after DigSigCommitSigRefDict. 
	
	@param transformMethod The transform method used to calculate 
	the digest. It must match originally specified in DigSigNewSigRefDict 
	
	@param sigDict The signature dictionary that contains 
	the signature reference dictionary. 
	@param refDict The reference dictionary returned from 
	the call to DigSigCommitSigRefDict 
	@param errParams A structure containing information about 
	an exception. 
	@return kDSTrue on success; kDSFalse on parameter error. kDSException 
	means an exception, and the information about it can be 
	found in errParams. 
	@see DigSigClearSigRefDict 
	@see DigSigCommitSigRefDict 
	@see DigSigNewSigRefDict 
	@see DigSigVerifySigRefDict 
*/
typedef ACCBPROTO1 DSRetCode (ACCBPROTO2 *DigSigFinishSigRefDictSELPROTO)
    ( ASAtom transformMethod, CosObj sigDict, CosObj refDict, DSSigRefDictErrParams errParams );
#define DigSigFinishSigRefDict (*((DigSigFinishSigRefDictSELPROTO)(gDigSigHFT[DigSigFinishSigRefDictSEL])))

/**
	Verifies an object signature stored in a signature reference 
	dictionary. It involves computing the object digest using 
	the transform method and comparing it to the digest value 
	stored in the reference dictionary. 
	@param transformMethod The transform method used to calculate 
	the signature. It must match the one stored in refDict 
	@param sigDict The signature dictionary that contains 
	the signature reference dictionary. 
	@param refDict The signature reference dictionary that 
	was returned from a call to DigSigCommitSigRefDict 
	@param errParams A structure containing information about 
	an exception.
	@return kDSTrue on success; kDSFalse on parameter error. kDSException 
	means an exception, and the information about it can be 
	found in errParams. 
	@see DigSigClearSigRefDict 
	@see DigSigCommitSigRefDict 
	@see DigSigFinishSigRefDict 
	@see DigSigNewSigRefDict 
*/
typedef ACCBPROTO1 DSRetCode (ACCBPROTO2 *DigSigVerifySigRefDictSELPROTO)
    ( ASAtom transformMethod, CosObj sigDict, CosObj refDict, DSSigRefDictErrParams errParams );
#define DigSigVerifySigRefDict (*((DigSigVerifySigRefDictSELPROTO)(gDigSigHFT[DigSigVerifySigRefDictSEL])))

/**
	Clears the signature reference dictionary referred to by 
	refDict and removes any reference to it from the document. 
	
	@param transformMethod The transform method. It should 
	match the one contained in sigDict. 
	@param sigDict The signature dictionary containing the 
	signature reference dictionary. 
	@param refDict The signature reference dictionary.
	@return Positive on success. 
	@see DigSigCommitSigRefDict 
	@see DigSigFinishSigRefDict 
	@see DigSigNewSigRefDict 
	@see DigSigVerifySigRefDict 
*/
typedef ACCBPROTO1 DSRetCode (ACCBPROTO2 *DigSigClearSigRefDictSELPROTO)
    ( ASAtom transformMethod, CosObj sigDict, CosObj refDict );
#define DigSigClearSigRefDict (*((DigSigClearSigRefDictSELPROTO)(gDigSigHFT[DigSigClearSigRefDictSEL])))


/*************************************************************************
 * DigSigAPCreateLayeredStreamEx
 * Creates a stream that is composed of four or five layers:
 *	layer n0 - background, inherited from form annot background
 *	layer n1 - middle, question mark or equivalent (none if CosNull)
 *	layer n2 - main, text and appearance (must be provided)
 *	layer n3 - top, blank or X (none if CosNull)
 *	layer nN - optional very top layer, used for text showing validity state (none if CosNull)
 ************************************************************************/
/**
	Parameter structure for DigSigAPCreateLayeredStreamEx, that 
	creates a stream that is composed of either four or five 
	layers, as specified by layerMatrices: 
	- layer 0: background, inherited from form annot background 
	- layer 1: middle, question mark or equivalent (uses the 
	default if the corresponding XObject is CosNull) 
	- layer 2: main, text and appearance (must be provided) 
	- layer 3: top, blank or X (uses the default if the layer1 
	XObject is CosNull) 
	- layer N: optional very top layer, used for text showing 
	validity state (none if CosNull) 
*/
typedef struct _t_DSAPCreateLayeredStreamExParams
{	
	/** The document in which to include the standard XObject. */
	CosDoc cosDoc;	
	/** A pointer to an array of XObjects for the appearance layers.*/
	CosObj* XObjects;
	/** A pointer to an array of positions. */
	ASFixedMatrixP layerMatrices;
	/** The size of the XObjects array. */
	ASInt32 numXObjects;
	/** The layer number for the optional validity state layer. */
	ASInt16 layerNNum;
	/** The border from AFPDWidgetGetBorder. */
	AFPDWidgetBorder border; 
	/** The color of the border. */
	PDColorValue cBorder;
	/** color of the background. */
	PDColorValue cBackGnd;
	/** The width of the new stream object. */
	ASFixed width;
	/** The height of the new stream object. */
	ASFixed height;
	/** The rotational parameter from AFPDWidgerGetRotation. */
	PDRotate pdr; 
	/** An array of flag values corresponding 
		to the layers specified by XObject. A value of true means 
		that the corresponding layer is displayed. 
	*/
	ASBool* layerFlags;
} DSAPCreateLayeredStreamExParamsRec, *DSAPCreateLayeredStreamExParams;

/** 
	Creates a signature-appearance layered stream, using a parameters structure.
	NOTE: Supersedes DigSigAPCreateLayeredStream in Acrobat 6.0.
	@param params The parameter structure containing the signature appearance layer
	information.
	@return A new CosStream composed of different layers.
	@see DigSigAPXObjectFromLogo
	@see DigSigAPXObjectFromXObjList
*/
typedef ACCBPROTO1 CosObj (ACCBPROTO2 *DSAPCreateLayeredStreamExSELPROTO) (const DSAPCreateLayeredStreamExParams params);
#define DigSigAPCreateLayeredStreamEx (*((DSAPCreateLayeredStreamExSELPROTO)(gDigSigHFT[DSAPCreateLayeredStreamExSEL])))

/**
	Tests whether a particular signature field in a document 
	is signed. 
	@param pdDoc The document that contains the signature field. 
	@param sigField The signature field that is tested. 
	@return true if the signature field is signed, false otherwise. 
	
	@see DigSigIsDocSigned 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *DigSigIsSigSignedSELPROTO)
	(PDDoc pdDoc, CosObj sigField);
#define DigSigIsSigSigned (*((DigSigIsSigSignedSELPROTO)(gDigSigHFT[DigSigIsSigSignedSEL])))

/*****************************************************************************
 DigSig Notification Server 
*****************************************************************************/
/** A type of notification to register for. 
	@see DigSigRegisterObserver
*/
typedef enum { 
	/** Signature field is added. */
    DSSigAdded = 0,/* Signature field is added */
    /** Signature field is deleted. */
    DSSigDeleted,  /* Signature field got deleted */
    /** A signature is requested. (That is, the notification procedure is
		invoked before the signature is applied.)
	*/
	DSWillSign,
	/** A signature is created. (That is, the notification procedure is
		invoked when signing succeeds.)
	*/
	DSDidSign,
	/** A signature request fails. (That is, the notification procedure is
		invoked when signing fails.)
	*/
    DSFailSign,
    /** A signature verification is requested. */
	DSWillVerify,
	/** A signature verification succeeds. */
	DSDidVerify,
	/** A signature verification request fails. */
    DSFailVerify,
    /** The clearing of a signature is requested. */
	DSWillClear,
	/** The clearing of a signature succeeds. */
	DSDidClear,
	/** The clearing of a signature fails. */
    DSFailClear,
    /** */
	DigSigNumNotifications	/* Always last */
} DSNotificationType;


/** A structure passed to the callback when a digital signature event occurs for which an
	interest has been registered. The structure contains information about the event.
	
	These are the arguments in DSNtfyParams_t which are applicable:
	
	DSSigAdded    : size, notificationID, pdDoc, cosDoc, sigField
	DSSigDeleted  : size, notificationID, pdDoc, cosDoc, sigField
	DSWillSign    : size, notificationID, pdDoc, cosDoc, sigField
	DSDidSign     : size, notificationID, pdDoc, cosDoc, sigField, sigDict
	DSFailSign    : size, notificationID, pdDoc, cosDoc, sigField
	DSWillVerify  : size, notificationID, pdDoc, cosDoc, sigField
	DSDidVerify   : size, notificationID, pdDoc, cosDoc, sigField, sigDict
	DSFailVerify  : size, notificationID, pdDoc, cosDoc, sigField
	DSWillClear   : size, notificationID, pdDoc, cosDoc, sigField, sigDict
	DSDidClear    : size, notificationID, pdDoc, cosDoc, sigField
	DSFailClear   : size, notificationID, pdDoc, cosDoc, sigField
	
*/
typedef struct _t_DSNtfyParamsRec {
	/** Size of the data structure. Must be set to
		sizeof(DSNotifyParamsRec).
	*/
    ASSize_t           size;
    /** The event that occurred. */
    DSNotificationType notificationID;
    /** The PDF document for which the event occurred. */
	PDDoc 	           pdDoc;          
	/** The Cos document containing the object for which the event
		occurred.
	*/
    CosDoc             cosDoc;
    /** The signature field for which the event occurred. */
    CosObj             sigField;
    /** The signature dictionary (used only for the events DSDidSign
		and DSDidClear).
	*/
    CosObj             sigDict;
} DSNtfyParamsRec, *DSNtfyParams;

/**
	Callback for the Notification Server. Called when a digital 
	signature event occurs for which you have registered an 
	interest. This procedure is called for these events: 

	- Adding or deleting a signature field.
	- Requesting a new signature, and the success of the request. 
	- Requesting that an existing signature be cleared, and 
	the success of the request. 

	For failure events, the server calls the DSNotificationFailureProc. 
	
	@param info Pointer to a DSNotifyParams structure containing 
	information about the event. 
	@param clientData Pointer to user-supplied data to pass 
	to proc each time it is called.
	@see DSNotificationFailureProc 
	@see DigSigRegisterObserver 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DSNotificationProc)(void *info, void *clientData);

/**
	Callback for the Notification Server. Called when a digital 
	signature event occurs for which you have registered an 
	interest. This procedure is called for these events: 

	- The failure of a request for a new signature. 
	- The failure of a request for an existing signatureto be 
	cleared. 

	For field change, request, and success events, the server 
	calls the DSNotificationProc. 
	@param error The error code for the error that occured. 
	
	@param info Pointer to a DSNotifyParams structure containing 
	information about the event. 
	@param clientData Pointer to user-supplied data to pass 
	to proc each time it is called.
	@see DSNotificationProc 
	@see DigSigRegisterObserver 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DSNotificationFailureProc)(ASInt32 error, void *info, void *clientData);

/**
	Registers callbacks with the notification server, to be 
	called for specific digital signature events. Events are: 
	
	- Adding or deleting a signature field. 
	- Requesting a new signature, and the success or failure 
	of the request. 
	- Requesting that an existing signature be cleared, and 
	the success or failure of the request.

	@param notificationID The type of event for which to register 
	a callback. 
	@param notifyProc The procedure to call when a field change, 
	request, or success event occurs. 
	@param notifyFailure The procedure to call when a failure 
	event occurs. 
	@param clientData A pointer to client-supplied data to 
	pass to the callback procedure.
	@return The observer identifier, which must be provided to DigSigUnregisterObserver. 
	
	@see DigSigUnregisterObserver 
*/
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *DigSigRegisterObserverSELPROTO)
		(DSNotificationType notificationID, DSNotificationProc notifyProc,
		DSNotificationFailureProc notifyFailure, void *clientData);
#define DigSigRegisterObserver (*((DigSigRegisterObserverSELPROTO)(gDigSigHFT[DigSigRegisterObserverSEL])))

/**
	Unregisters an event interest from the notification server. 
	
	@param notificationID The event for which an interest 
	has been registered, as specified in DigSigRegisterObserver. 
	
	@param observerID The observer identifier as returned 
	by DigSigRegisterObserver.
	@see DigSigRegisterObserver 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DigSigUnregisterObserverSELPROTO)
		(DSNotificationType notificationID, ASInt32 observerID);
#define DigSigUnregisterObserver (*((DigSigUnregisterObserverSELPROTO)(gDigSigHFT[DigSigUnregisterObserverSEL])))

/**
	Gets the author signature for a document as a Cos object. 
	
	@param doc The document for which the author signature 
	is obtained.
	@return The CosObj containing the author signature. 
	@see DigSigGetDocMDPSetting 
*/
typedef ACCBPROTO1 CosObj (ACCBPROTO2 *DigSigGetDocAuthorSignatureSELPROTO)
		(PDDoc doc);
#define DigSigGetDocAuthorSignature (*((DigSigGetDocAuthorSignatureSELPROTO)(gDigSigHFT[DigSigGetDocAuthorSignatureSEL])))

/** Parameters for page comparison. 
	@see DigSigComparePagesEx
*/
typedef struct _t_DSComparePagesParamsRec {
	/** Size of the data structure. Must be set to
		sizeof(DSComparePagesParamsRec).
	*/
	ASSize_t	size;
	/** The sensitivity level for the comparison. The DPI determines
		the resolution at which each page is rendered before
		comparing on a pixel-by-pixel basis. Values are:
		0: 72 DPI
		1: 36 DPI
		2: 18 DPI
	*/
	ASInt32 sensitivityLevel;
	
} DSComparePagesParamsRec, *DSComparePagesParams;

/**
	Compares the pages of two documents, producing a third document 
	of the differences. 
	@param docA First document being compared. 
	@param docB Second document being compared. 
	@param params A structure containing the sensitivity level 
	for the comparison.
	@return true if the pages of the documents are identical, false 
	otherwise. 
	@see DigSigComparePages 
	@see DigSigCompareWords 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *DigSigComparePagesExSELPROTO)
	(PDDoc docA, PDDoc docB, DSComparePagesParams params);
#define DigSigComparePagesEx (*((DigSigComparePagesExSELPROTO)(gDigSigHFT[DigSigComparePagesExSEL])))

/** Parameters for page comparison.
	@see DigSigCompareWordsEx
*/
typedef struct _t_DSCompareWordsParamsRec {
	/** Size of the data structure. Must be set to
		sizeof(DSCompareWordsParamsRec).
	*/
	ASSize_t	size;
	/** If true, the font name, size, and color of each character in a word are
		considered when matching against other words. If false, these
		attributes are ignored.
	*/
	ASBool useFont;				/* Turn font comparison on or off. */
} DSCompareWordsParamsRec, *DSCompareWordsParams;


/**
	Compares the words of two documents, producing a third document 
	of the differences. 

	NOTE: Supercedes DigSigCompareWords, DigSigCompareWordsAndFontsRecent, 
	and DigSigCompareWordsRecent in Acrobat 6.0. 
	@param docA First document being compared. 
	@param docB Second document being compared. 
	@param params A structure containing the comparison parameters.
	@return true if the words of the documents are identical, false 
	otherwise. 
	@see DigSigComparePagesEx 
	@see DigSigCompareWords 
	@see DigSigCompareWordsAndFontsRecent 
	@see DigSigCompareWordsRecent 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *DigSigCompareWordsExSELPROTO)
	(PDDoc docA, PDDoc docB, DSCompareWordsParams params);
#define DigSigCompareWordsEx (*((DigSigCompareWordsExSELPROTO)(gDigSigHFT[DigSigCompareWordsExSEL])))

/**
	Verifies a digital signature. 
	@param pdDoc The document. 
	@param sigField The signature field to verify. 
	@param bUI When true, can bring up the user-interface 
	dialogs if needed.
	@return The validity state that results from verification. 
	@see DigSigClearSig 
*/
typedef ACCBPROTO1 DSValidState (ACCBPROTO2 *DigSigVerifySigSELPROTO)
	(PDDoc pdDoc, CosObj sigField, ASBool bUI);
#define DigSigVerifySig (*((DigSigVerifySigSELPROTO)(gDigSigHFT[DigSigVerifySigSEL])))

/**
	Clears a signature field in a document. This removes the 
	signature, so that the document is unsigned. 
	@param pdDoc The document. 
	@param sigField The signature field to clear.
	@see DigSigVerifySig 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DigSigClearSigSELPROTO)
	(PDDoc pdDoc, CosObj sigField);
#define DigSigClearSig (*((DigSigClearSigSELPROTO)(gDigSigHFT[DigSigClearSigSEL])))

/**
	Gets the MDP setting for the author signature for the specified 
	document. 
	@param pdDoc The document for which the MDP setting is obtained.
	@return The MDP setting of the document's author signature, or kDSMDPNone 
	if there is no author signature. 
	@see DigSigGetDocAuthorSignature 
*/
typedef ACCBPROTO1 DSMDPType (ACCBPROTO2 *DigSigGetDocMDPSettingSELPROTO)
	(PDDoc pdDoc);
#define DigSigGetDocMDPSetting (*((DigSigGetDocMDPSettingSELPROTO)(gDigSigHFT[DigSigGetDocMDPSettingSEL])))

/** 
	Gets the Reader Extensions signature for a document as a Cos object.
	@param pdDoc The document for which the Reader Extensions signature is obtained.
	@return The CosObj containing the Reader Extensions signature.
	@see DigSigGetDocAuthorSignature
*/
typedef ACCBPROTO1 CosObj (ACCBPROTO2 *DigSigGetUbiquitySigSELPROTO)
	(PDDoc pdDoc);
#define DigSigGetUbiquitySig (*((DigSigGetUbiquitySigSELPROTO)(gDigSigHFT[DigSigGetUbiquitySigSEL])))

/************************************************************************************
 * DigSigHFT globals
 ***********************************************************************************/

extern HFT gDigSigHFT;

#ifdef __cplusplus
}
#endif

#if PRAGMA_STRUCT_ALIGN
	#pragma options align=reset
#endif

#endif /* _H_DigSigHFT */

/* End of DigSigHFT.h */
