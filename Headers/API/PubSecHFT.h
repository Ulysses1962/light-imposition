/*************************************************************************
 * PubSecHFT.h
 *
 * Copyright (c) 2000-2003 Adobe Systems Inc. All Rights Reserved.
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
 * Public key security interface for Acrobat public-key security
 * handlers.  Handlers can register as PubSec handlers to provide
 * crypto services for private key signing, for signature validation,
 * as a crypto source for decrypting using private keys, and as a
 * directory source.
 *
 * Handlers can also call back into the PubSecHFT for various
 * services, including a signature appearance handler and a trusted
 * address book.
 *
 * Update History: (most recent first)
 *  30-Mar-2003 -- Acrobat 6.0 SDK cleanup
 *  13-Dec-2002 -- Acrobat 6.0 beta cleanup
 *  18-Oct-2002 -- Added Confidentiality notice
 *  11-Aug-2002 -- Added FDF data exchange import/export calls
 *	02-Aug-2002 -- Added new notification API for AAB, removed old API
 *  10-Mar-2002 -- Added routines to support CosDoc and generic signature API
 *  11-Jan-2001 -- Wrote hftbuild perl script to autogenerate portions of this file
 *  09-Jan-2001 -- Created for Acrobat 6.0
 ************************************************************************/

#ifndef PUBSECHFT_H
#define PUBSECHFT_H
   
#include "CosExpT.h"
#include "PDExpT.h"
#include "ASExpT.h"				/* Required for ASText */
#include "DigSigHFT.h"
#include "DirectoryHFT.h"

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

/************************************************************************************
 * PubSecHFT - Enumerations and structure declarations
 ***********************************************************************************/

/** the maximum size of the binary signature blob in bytes
 Note: this traslates to SIG_MAX_SIZE * 2 + 2 in the file as it is hex encoded 
 currently (* 2) and the delimiters at the two end (+2)
 */
#define PS_SIG_MAX_SIZE   65535 

/** 
	A PubSec engine object. There can be multiple engine objects per handler.
	@see PSCloseEncryptedDocs
	@see PSCountEncryptedDocs
*/
typedef void* PubSecEngine;

/** 
	A signature appearance file object to be used by handlers to access the built-in APIcon
	appearance handler.
	@see DSAPFileAcquire
*/
typedef struct CAPIconFile *DSAPFile;

/** Operations for which a PubSec session can be acquired. */
typedef enum { 
	/**  None */
	kPSOpTypeNone=0,
	/** Sign a PDDoc to create a PDDoc signature. */
	kPSOpTypePDDocSign,				
	/** Validate a PDDoc signature. */
	kPSOpTypePDDocSigValidate,		
	/** Sign a cosDoc. */
	kPSOpTypeCosDocSign,			
	/** Validate cosDoc signature. */
	kPSOpTypeCosDocSigValidate,		
	/** Sign bytes. */
	kPSOpTypeDataSign,			 
	/** Validate bytes. */
	kPSOpTypeDataSigValidate,		
	/** Read and export My Contact information. */
	kPSOpTypeMyContactExport,		
	/** Import contacts into address book. */
	kPSOpTypeContactImport,			
	/** Modify credential access policy (used by EScript). */
	kPSOpTypePolicyModify,			
	/** Read an appearance file (used by EScript). */
	kPSOpTypeAPRead,				
	/** Prepare to encrypt documents. */
	kPSOpTypePDCryptDocCreate,		
	/** Prepare to open encrypted documents. */
	kPSOpTypePDCryptDocOpen,		
	/** Authenticate or login user (used by EScript). */
	kPSOpTypeLogin,					
	/** De-authenticate or logout user (used by EScript). */
	kPSOpTypeLogout,				
	/** Enumerate certificates available for signing and encryption. */
	kPSOpTypeCertEnum,				
	/** Enumerate directories and get directory information. */
	kPSOpTypeDirEnum,				
	/** Connect to and use directory. */
	kPSOpTypeDirGet,
    /* Enumerate credential stores */
    kPSOpTypeStoreEnum
} PSSessionOpType;

/** A signature type to use for generating a signature appearance.
	@see PSSigCreateAPNXObjProc
 */
typedef enum { 
	/** Signing a prexisting signature field. */
	kPSAPSigPreExists=0,		
	/** Creating a signature on the fly on a page. */
	kPSAPSigTentative,			
	/** Creating a preview of a signature. */
	kPSAPSigPreview				
} PSAPSigType;


/************************************************************************************
 * Enumerations and structures used for signing and validation
 ***********************************************************************************/

/** A buffer of data to be hashed when signing or verifying signatures. The buffer contains the
	bytes to be signed (by ByteRange) when signing a PDF file.
	@see PSDataBufferDigest
	@see PSDataBufferEnum
*/
typedef struct _s_PSDataBufferRec PSDataBufferRec, *PSDataBuffer;

/** Types of PDDoc signatures. */
typedef enum {
	/** */
    kPSSigTypeUnknown=0,
	/** Ordinary signature */
	kPSSigTypeUser,			
	/** Ordinary signature, plus an MDP object signature */
	kPSSigTypeAuthor,		
	/** Ubiquity (Reader rights-enabling) object signature */
	kPSSigTypeUbiquity,		
	/** */
    kPSSigTypeEnumSize
} PSSigType;

/** The type of document being signed.
	@see PSSigPDDocParams
	@see PSSigSigPropParams
	@see PSSigValidateDialogParams
*/
typedef enum {
	/** */
	kPSSigDocTypeNone=0,
	/** A PDF document. */
	kPSSigDocTypePDDoc,			
	/** A FDF file. */
	kPSSigDocTypeCosDoc,		
	/** Transactional data, e.g. XFA */
	kPSSigDocTypeTransData,		
	/** */
	kPSSigDocTypeEnumSize
} PSSigDocType;

/** A constant value that indicates the file type and environment in which data is being signed,
	used to set up buttons and text in the signing dialog.
	@see PSSigSigPropParams
*/
typedef enum {
	/** Uninitialized */
	kPSFileNone=0,			
	/** Normal PDDoc signing. Enable 'Save' and 'SaveAs' buttons */
	kPSFileAVDoc,			
	/** External browser window. Enable 'Sign' button */
	kPSFileBrowserDoc,		
	/** External non-browser window. Enable 'Sign' button */
	kPSFileExternalDoc,		
	/** Temp file. Enable 'SaveAs' button */
	kPSFileTempDoc,			
	/** Signing a FDF Data Exchange file. Enable 'Sign' button */
	kPSFileFDFData,			
	/** Signing a CosDoc. Enable 'Sign' button */
	kPSFileCosDoc,			
	/** Signing transactional data */
	kPSFileTransData,		
	/** */
	kPSFileEnumSize
} PSSigFileType;

/** Constants that specify the method to use for creating a signature. For details, see the PDF
	Reference.
	@see PSSigGetSigValueParams
	@see PSSigGetSigValueProc
*/
typedef enum {
	/** None */
	kPSSigMethodNone=0,
	/** PKCS#1, adbe.x509.rsa.sha1 */
	kPSSigMethodPKCS1,					
	/** adbe.pkcs7.detached */
	kPSSigMethodPKCS7Detached,			
	/** adbe.pkcs7.sha1 */
	kPSSigMethodPKCS7SHA1,				
	/** */
	kPSSigMethodEnumSize
} PSSigMethod;

/** Dialog information, returned from GetSigProperties() and other
   calls to tell PubSec what flow of subsequent dialogs or actions to
   execute. Not all values are legal for all procs. */
typedef enum {
	/** No further dialogs required. */
	kPSSigDialogNone=0,					
	/** PubSec's sign dialog should be executed. */
	kPSSigDialogSign,					
	/** PubSec's sign dialog should be executed, and ask for password. */
	kPSSigDialogSignWithPassword,		
	/** PubSec's sign dialog should be executed, then SigAuthenticate() should be called. */
	kPSSigDialogSignThenAuthenticate,	
	/** Size of PSSigDialogStatus enum. */
	kPSSigDialogStatusEnumSize
} PSSigDialogStatus;

// Enum of whether signature validation is supported
/** Constant values indicating how a handler supports validation for a particular signature. */
typedef enum {
	/** The handler does not support validationof this signature. */
	kPSSigValSupportFalse,
	/** The handler supports validationof this signature. */
	kPSSigValSupportTrue,
	/** A handler software update is needed to support validation of this signature. */
	kPSSigValSupportUpdate,	
	/** The signature was created with a pre-release or invalid release of the software. Warns user. */
	kPSSigValSupportPreRelease,
	/** Size of PSSigValSupport enum */
	kPSSigValSupportEnumSize
} PSSigValSupport;

/** A structure containing parameters and return values for
	PSSigGetSigPropertiesProc.
	@see SigGetSigPropertiesProc 
*/
typedef struct _t_PSSigSigPropParamsRec {
	/** The size of the structure. */
	ASSize_t size;
	/** Constant. The pdDoc being signed. If not signing a pdDoc ,
		this is only used for window parenting and can be NULL.
	*/
	PDDoc pdDoc;					
	/** Constant. The file type and signing environment, which the
		handler uses to put up appropriate save buttons.
	*/
	PSSigFileType fileOptions;		
	/** Constant. When true , the call is being made from the UI,
		when false it is being made from a script or batch file.
	*/
	ASBool bUI;						
	/** Title to use for the signing dialog, if bUI is true. Usually
		NULL, which allows the handler to use the default.
	*/
	ASText dialogTitle;				
	/** Constant. Parameters passed in by EScript converted to an
		ASCab, or NULL if the call is not from EScript.
	*/
	ASCab sigParamsCab;				
	/** If not providing your own sign dialog, set this value to
		DSSignSave.
		If providing your own sign dialog, return the result of your
		sign dialog. Setting the value to DSignCancel cancels the
		signing operation.
	*/
	DSSaveType outSaveOptions;			
	/** Return status information that tells PubSec what dialogs to
		put up and how to authenticate the handler.
	    Legal return values are: kPSSigDialogNone (handler provides its own sign dialog),
		kPSSigDialogSign (PubSec provides sign dialog), 
		kPSSigDialogSignWithPassword (PubSec provides sign dialog with password), and
		kPSSigDialogSignThenAuthenticate (PubSec provides sign dialog, then afterwards calls handler 
		to allow handler to authenticate user).*/
	PSSigDialogStatus outDialogStatus;	
	/** Required. Return an ASCab containing an ordered certificate chain, signing certificate first (at key 0). */
	ASCab outCertListCab;				
	/** Required. Populate this existing ASCab with signature
		properties that will be written to the signature dictionary
		when the signature is committed.
	*/
	ASCab outNewSigPropCab;
	/** Optional. Populate this existing ASCab with return build
		properties that will be written to the signature dictionary
		when the signature is committed.
	*/
	ASCab outNewSigBuildCab;
	/** Constant. The type of document being signed. */
	PSSigDocType docType;			
	/** A structure containing signature parameters appropriate to
		the type of document. Depending on sigType, a structure
		of type PSSigPDDocParams, PSSigCosDocParams, or
		PSSigDataBufferParams.
	*/
	void* docParams;
	/** const. Specification for SignatureValue - see PDF Reference */
	PSSigMethod sigMethod;			
	/** const. Method used to create digest */
	DSDigestMethod digestMethod;	

    /**
        Additions for Acrobat 7.
    */
    /** The type of the signature being created
    */
    PSSigType inSigType;

} PSSigSigPropParamsRec, *PSSigSigPropParams;

/** A structure containing parameters and return values for PSSigGetSigValueProc. */
typedef struct _t_PSSigGetSigValueParamsRec {
	/** The size of the structure. */
	ASSize_t size;
	/** Constant. The method used to create the signature value. */
	PSSigMethod sigMethod;			
	/** Constant. The method used to create the digest. */
	DSDigestMethod digestMethod;	
	/** Constant. A digest value to be signed or NULL. */
	const ASUns8* digestValue;		
	/** The handle to use for fetching bytes to digest. */
	PSDataBuffer dataBuffer;		
	/** Constant. If true, get a signature value,
		otherwise return the size of the signature
		value.
	*/
	ASBool bGetSigValue;			
	/** Return the signature value data. Caller will call
		ASfree to destroy the structure.
	*/
	ASUns8* outSigValueData;			
	/** Return size of SignatureValue */
	ASUns32 outSigValueSize;			
	/** Return an ASCab containing signature
		properties that should be displayed instead of
		signature dictionary entries.
	*/
	ASCab outSigPropCab;		
} PSSigGetSigValueParamsRec, *PSSigGetSigValueParams;

/** A structure containing signature parameters for PSSigValidateSupportedProc. 
	@see PSSigValidateSupportedProc */
typedef struct _t_PSSigValidateSupportParamsRec {
	/** The size of the structure. */
	ASSize_t size;
	/** Constant. The value of the /Filter attribute for the signature. */
	ASAtom filter;			
	/** Constant. The value of the /SubFilter attribute for the signature. */
	ASAtom subFilter;	
	/** Constant. The value of the /V attribute for the signature. */
	ASInt32 version;			
	/** Constant. The value of the /R attribute for the signature. */
	ASInt32 revision;			
	/** Constant. The handler’s build properties dictionary, as an ASCab. */
	ASCab buildCab;			
} PSSigValidateSupportParamsRec, *PSSigValidateSupportParams;

/** A structure containing parameters and return values for PSSigValidateProc.
	@see PSSigValidateProc */
typedef struct _t_PSSigValidateParamsRecRec {
	/** The size of the structure. */
	ASSize_t size;
	/** Constant. The method used to create the
		signature value. */
	PSSigMethod sigMethod;			
	/** Constant. The method used to create the
		digest. */
	DSDigestMethod digestMethod;	
	/** Constant. A digest value to be signed or NULL. */
	ASUns8* digestValue;			
	/** Constant. A signature value to be verified. */
	ASUns8* sigValueData;			
	/** Constant. The size of the signature value. */
	ASUns32 sigValueSize;			
	/** In and out.
			For a PKCS#1 signature, PubSec fills in the
		certificate chain.
			For a PKCS#7 signature, PubSec returns an
		empty ASCab.
		For both signature types, the handler must
		provide the certificate chain that was
		validated.
	*/
	ASCab certListCab;
    /** Informs the handler whether revocation
		checks are required. Handlers should always
		do revocation checks, but return failure only
		when this value is true.
     */
    ASBool reqRevokeChecks;
    /** The maximum life time (in minutes) of the
		cached information that is used for revocation
		checking. This is relevant for some types of
		revocation checking (such as CRL-based) and
		not for others (such as OCSP).
	*/
    ASInt32 maxRevokeInfoCacheLifetime;
	/** Returns an ASCab containing the validity of
		the signature. Must not be NULL.
		The handler must set the values for
		PROP_SigVal_Id and
		PROP_SigVal_TrustFlags, and can
		optionally set PROP_SigVal_IdPriv and/or
		PROP_SigVal_IDPrivTextValidity. The
		handler should also set the value of
		PROP_SigVal_Digest if it is not already
		set. All other values should be left unmodified
		since they will not be preserved by PubSec. 
	*/
	ASCab sigValCab;				
	/** Returns signature properties to display instead
		of signature dictionary entries.
	*/
	ASCab outSigPropCab;				
	/** The handle to use for fetching bytes to digest. */
	PSDataBuffer dataBuffer;			

    /**
        Additions for Acrobat 7.
    */
    /** PubSec provides the time that it thinks signing occured.
    */
    ASTimeRec *inSignTime;
    /** The type of the signature being validated
    */
    PSSigType inSigType;
} PSSigValidateParamsRec, *PSSigValidateParams;

/** A structure containing parameters and return values for PSSigValidateDialogProc.
	@see PSSigValidateDialogProc
	@see PSSigPropDialogProc 
*/
typedef struct _t_PSSigValidateDialogParamsRec {
	/** The size of the structure. */
	ASSize_t size;
	/** In and out.
		For a PKCS#1 signature, PubSec fills in the
	certificate chain.
		For a PKCS#7 signature, PubSec returns an
	empty ASCab.
	For both signature types, the handler must
	provide the certificate chain that was
	validated.
 */
	ASCab certListCab;    
	/** Informs the handler whether revocation
		checks are required. Handlers should always
		do revocation checks, but return failure only
		when this value is true.
	*/
    ASBool reqRevokeChecks;
    /** The maximum life time (in minutes) of the
		cached information that is used for revocation
		checking. This is relevant for some types of
		revocation checking (such as CRL-based) and
		not for others (such as OCSP).
    */
    ASInt32 maxRevokeInfoCacheLifetime;
	/** An ASCab containing the validity of the
		signature. Never NULL.
		The handler can validate the signature and
		update this object if desired.
	 */
	ASCab sigValCab;
	/** Indicates whether being called when a modal
		parent dialog is open. If true, rollback and
		verify buttons should be disabled because
		i) rollback is not possible while modal parent is
		open; (ii) verify status may not get propagated
		properly to various caching locations. 
	*/
	ASBool bModalParent;
	/** Returns a value that tells PubSec whether to
		rollback the signature or show a properties
		dialog for the signature.
	*/
	DSPropertyType dsPropType;
	/** Constant. The type of document being signed. */
	PSSigDocType docType;
	/** A structure containing signature parameters
		appropriate to the type of document.
		Depending on sigType, a structure of type
		PSSigPDDocParams or
		PSSigCosDocParams.
	*/
	void* docParams;
	ASCab sigPropCab;
	ASCab sigDictCab;
} PSSigValidateDialogParamsRec, *PSSigValidateDialogParams;

/** Validation parameters for validating a document signature or specific signature field.
	@see PSSigValidatePDDocSigField
	@see PSSigGetSigPropertiesProc
	@see PSSigCreateAPNXObjProc
	@see PSSigSigPropParams
*/
typedef struct _t_PSSigPDDocParamsRec {
	/** The size of the structure. */
	ASSize_t size;
	/** Constant. Always kPSSigDocTypePDDoc. */
	PSSigDocType sigType;			
	/** Constant. The document being signed. */
	PDDoc pdDoc;					
	/** Constant. The field being signed. */
	CosObj sigField;				
	/** Constant. The annotation associated with this field. */
	CosObj sigAnnot;				
	/** Constant. Width of the annotion if the signature is visible. */
	ASFixed annotWidth;				
	/** Constant. Height of the annotion if the signature is visible. */
	ASFixed annotHeight;			
	/** Returns handler-specific appearance information for
		PSSigGetSigPropertiesProc. */
	ASCab sigAPCab;					
} PSSigPDDocParamsRec, *PSSigPDDocParams;

/** Signature parameters for a cosDoc. Used internally.
	@see PSSigGetSigPropertiesProc
	@see PSSigSigPropParams */
typedef struct _t_PSSigCosDocParamsRec {
	/** The size of the structure. */
	ASSize_t size;
	/** Constant. Always kPSSigDocTypeCosDoc. */
	PSSigDocType sigType;			
	/** Constant. Null if no parent. */
	PDDoc pdDoc;					
	/** Constant. The document being signed. */
	CosDoc cosDoc;					
	/* Constant. The signature dictionary. */
/*	CosObj sigDict;	*/				
} PSSigCosDocParamsRec, *PSSigCosDocParams;

/** Signature parameters for signing transitional data, such as XFA.
	@see PSSigGetSigPropertiesProc
	@see PSSigSigPropParams
*/
typedef struct _t_PSSigDataBufferParamsRec {
	/** The size of the structure. */
	ASSize_t size;
	/** Constant. Always kPSSigDocTypeTransData. */
	PSSigDocType sigType;			
	/** Constant. NULL if no parent. */
	PDDoc pdDoc;					
	/** Constant. The document being signed. */
	CosDoc cosDoc;					
	/* const */
/*	CosObj sigDict;	*/			
} PSSigDataBufferParamsRec, *PSSigDataBufferParams;

/** Parameters that are used when opening a CMS envelope */
typedef struct _t_PSOpenCMSEnvelopeParamsRec {
	/** The size of this structure. */
	ASSize_t size;

	/** Constant. The ASN1 encoded PKCS#7 Enveloped
		Data to open. */
	const ASUns8* cmsEnvelopeData;
	/** Size in bytes of the data pointed to by
		cmsEnvelopeData. */
	ASUns32 cmsEnvelopeSize;

	/** Returns the envelope contents data. PubSec
		owns, allocates and frees the memory. */
	ASUns8* outEnvelopeContentsData;
	/** Size in bytes of the data pointed to by
		cmsEnvelopeContentsData. */
	ASUns32 outEnvelopeContentsSize;

	/** Optional. Returns the session key recovered
		when opening the envelope. If a session key is
		returned by the handler, PubSec uses it to open
		other recipient groups and enable the user to
		edit them.
*/
	ASUns8* outSessionKeyData;
	/** Size in bytes of the data pointed to by
		outSessionKeyData. */
	ASUns32 outSessionKeySize;

	/** Optional. The certificate corresponding to the
		private key used to open the envelope. When
		provided, PubSec displayes the name of the
		opener when the user edits the recipient list.
	 */
	ASCab outCertificates;

} PSOpenCMSEnvelopeParamsRec, *PSOpenCMSEnvelopeParams;

typedef struct _t_PSSigSignReportParamsRec
{
	/** The size of this structure. */
	ASSize_t size;
	/** Sig properties of the signature we would like to report on.	*/
	ASCab sigPropCab;
} PSSigSignReportParamsRec, *PSSigSignReportParams;



/*************************************************************************
 * Validity Cab public properties and property values.  This Cab is
 * populated by PubSec and its handlers when validating signatures. It
 * contains the validity state of the signature, and is cached by
 * PubSec so that validity information is immediately available to
 * javascript calls and UI. Handlers can populate the ASCab with
 * additional custom values as required. PubSec takes care of most of these values.
 * The PubSec handler must set PROP_SigVal_Id and may set PROP_SigVal_IdPriv.
 * The PubSec handler must set PROP_SigVal_Digest with the result of the
 * digest comparison: this occurs when comparing the signed digest 
 * to the passed-in digest, or when calculating a byte range digest. 
 * The PubSec handler also must set PROP_SigVal_TrustFlags, though
 * these values may be 'tested' by PubSec.
 * PubSec will update the ASCab itself, just after signing, 
 * to set the state to 'just signed' (kDSSigValJustSigned).
 ************************************************************************/

/* Public Validity Cab entries.  Engines can add their own private
   entries, but must do so using their own namespace
   (e.g. "Acme:Identity") to prevent future name clashes. */
#define	PROP_SigVal_DS		"ds"		/* value is ASInt32(DSValidState), state returned through DigSigHFT */
#define	PROP_SigVal_Digest	"digest"	/* value is ASInt32(DSSigValState), handler digest result, indicates if signature digest is valid */
#define	PROP_SigVal_Doc		"doc"		/* value is ASInt32(DSSigValState), byte range digest result */
#define	PROP_SigVal_Obj		"obj"		/* value is ASInt32(DSSigValState), object signature results, used by author sigs and ordinary sigs when MDP+ is used */
#define	PROP_SigVal_Id		"id"		/* value is ASInt32(DSValidState), Indicates whether identity is valid */
#define PROP_SigVal_IdPriv	"idPriv"	/* value is handler-specific ASInt32. Indicates identity validity. */
#define PROP_SigVal_IdPrivText	"idPrivText"	/* Language-independent, machine readible version of  PROP_SigVal_IdPriv (eg. 'kIdUnknown') */
#define	PROP_SigVal_AuthSig	"authSig"	/* value is ASInt32(DSSigValState),  indicates the status of the author sig for this document */
#define	PROP_SigVal_Mod		"mod"		/* value is ASBool, true if doc has been modified after this sig was applied */
#define	PROP_SigVal_Author	"auth"		/* value is ASBool, true if this signature is an author signature */
#define	PROP_SigVal_Ubiquity "ubiquity"	/* value is ASBool, true if this signature is an ubiquity signature. This if for Adobe's internal user only*/
#define	PROP_SigVal_Data    "data"	    /* value is ASBool, true if this signature is an data signature */
#define	PROP_SigVal_FDF     "fdf"	    /* value is ASBool, true if this signature is an FDF signature */
#define PROP_SigVal_TrustFlags "trustFlags" /* value is ASInt32(PSSigTrust), stores the actions that the signer is trusted for */

/** Types of text requested by PSSigValGetTextProc for a signature-validity ASCab
	object. The strings are generated by the handler, usually on the fly, when requested. A
	handler can cache the values in the sigVal ASCab (Acrobat plug-ins do not do this).
	PubSec copies values for DSTop, DSDetail, DSDetails and DSTooltip to the
	signature-propeties ASCab.
		For optional entries, returning NULL causes PubSec to generate the values.
		Depending on the context, one of the *IdValidity entries is required to specify the
	signer identity validity.

*/
typedef enum {
	/** Never called, should return NULL. */
	kDSSigValTextNull,			
	/* Optional. Get text to show in AVPanel,
		added to end of signature summary line. */
	kDSSigValTextDSTop,			
	/** Optional. Get text to show in AVPanel,
		shown in signature detail title line. */
	kDSSigValTextDSDetail,		
	/** Optional. Get text to show in tooltip when
		mouse is over signature annotation. */
	kDSSigValTextDSTooltip,		
	/** Optional. Get text to show in EScript
		SignatureInfo object. */
	kDSSigValTextSigInfo,		
	/** Optional. Get text to show in validate alert
		dialog. */
	kDSSigValTextVal,			
	/** Optional. Get text to show in signature
		properties dialog. */
	kDSSigValTextProp,			
	/** Optional. Get text to show in appearance, if
		signature appearance uses layer n4 (not
		recommended). In this case, you must also
		pass an index. */
	kDSSigValTextAP,			
	/* Get text to describe validity of the signer
		certificate (used in properties and validity
		dialogs). */
	kDSSigValTextPropIdValidity,
	/** Get text to describe validity of the signer
		certificate (used in AVPanel). */
	kDSSigValTextDSAVIdValidity,
	/** Get text to describe validity of the signer
		certificate (appended to tooltip, called only
		when describing why certificate is invalid). */
	kDSSigValTextDSTTIdValidity,
	/** */
	kDSSigValTextEnumSize
} DSSigValText;

/** Bit flag constants that specify a level of trust for a certificate. 
	@see AABGetTrustedCerts
	@see PSImportDataParams
*/
/*@{*/
typedef ASInt32 PSSigTrust;
/** Certificate is untrusted */
#define kPSSigTrustUntrusted			0x0000	
/** Trusted for signing (creating recipient signatures). 
    Note: Starting Acrobat 7, this flag can no longer be (un)set by user, this 
    flag is alwyas set internally */
#define kPSSigTrustSigning              0x0001  
/** Trusted for authoring documents
	(creating author signatures). */
#define kPSSigTrustAuthenticDocuments	0x0002	
/** Trusted for authoring documents with
	dyamic (multimedia) content. */
#define kPSSigTrustDynamicContent		0x0004	
/** Trusted for feature-enabling signatures
	(Adobe internal use only). */
#define kPSSigTrustUbiquity				0x0008	
/** Trusted for authoring documents with fullaccess
	JavaScript. */
#define kPSSigTrustJavaScript		    0x0010	
/** Trusted for identity - must be present to use in trust calculations. */
#define kPSSigTrustIdentity				0x0020	
/** Trusted as an anchor - No checks are done for certs above this cert */
#define kPSSigTrustAnchor				0x0040	
/** Trusted for everything. */
#define kPSSigTrustAll                  0xFFFF  
/*@}*/

/*************************************************************************
 * Import and Export data
 ************************************************************************/

/** The format of data to be imported using PSImportDataProc.
	The operation is only executed if the corresponding property (PROP_ImportContact or
	PROP_ImportDirSettings) is set, which indicates that the handler supports this
	format. If PROP_ImportContact is set, both of the corresponding formats must be
	supported. */
typedef enum {
	/** None. No import operation is performed. */
	kPSImportDataNone=0,
	/** An ASCab containing a list ("0", "1", ...) of
		contact cabs. Contact cabs have the entries
		PROP_ContactCab_*. */	
	kPSImportDataContactsCab,			
	/** An ASCab containing a list ("0", "1", ...)
		containing a single contact cab. This Contact
		cab will have only the
		PROP_ContactCab_Certs entry set.
		This is sent only when trusting the signer of a
		signature. */	
	kPSImportDataContactsCabFromSig,
	/** An ASCab containing a list ("0", "1", ...) of dir
		setting cabs. DirSetting cabs have the entries
		PROP_DirSettingCab_*. */	
	kPSImportDataDirsCab,		
	/** Size of PSImportDataType */
	kPSImportDataEnumSize
} PSImportDataType;

/** Parameters for importing data from a file into PubSec; used in PSImportDataProc.
	@see PSImportDataProc */
typedef struct _t_PSImportDataParamsRec {
	/** The size of the structure. */
	ASSize_t size;
	/** The format of the data to be imported. */
	PSImportDataType dataType;	
	/** 	If dataType is kPSImportDataSigDict, the signature
		dictionary.
			If dataType is kPSImportDataFDFImportArray, the import
		array.
			If dataType is kPSImportDataFDFDirArray, the directory
		settings array.
	*/
	ASCab dataCab;
	/** A flag that indicates whether this data comes from a trusted source. */
	PSSigTrust sigTrust;			
} PSImportDataParamsRec, *PSImportDataParams;

/** The format of data to be exported using PSExportDataProc or
	PSExportDataExchange.
	@see PSExportDataProc
	@see PSExportDataExchange
*/
typedef enum { 
	/** None. No export operation is performed. */
	kPSExportDataNone=0,			
	/** Export certificates for self.
		This operation is performed only if the
		PROP_PSENG_ExportContact property is
		set, indicating that the handler supports this
		format. 
	*/
	kPSExportDataMyContact,			
	/* Export a list of entries from an address book.
		This operation is performed only if the
		PROP_PSENG_ExportContact property is
		set, indicating that the handler supports this
		format. 
	*/
	kPSExportDataContacts,			
	/** Export a list of directory settings.
		This operation is performed only if the
		PROP_PSENG_ExportDirSettings
		property is set, indicating that the handler
		supports this format.
	*/
	kPSExportDataDirSettings,		
	/** Enum size*/
	kPSExportDataEnumSize
} PSExportDataType;

/** File output options for PSExportDataExchangeParams.
	@see PSExportDataProc
	@see PSExportDataExchange
*/
typedef enum { 
	/** None, prompt for output file type. Recommended. */
	kPSExportDestNone=0,			
	/** Save as unspecified file type, possibly prompting for the file type. */
	kPSExportDestFile,				
	/** Save as CMS file (.p7c) */
	kPSExportDestCMSFile,			
	/** Save as Certificate file (.cer) */
	kPSExportDestCertFile,			
	/** Save as FDF file. */
	kPSExportDestFDFFile,			
	/** Email FDF file. */
	kPSExportDestEMailFDF,			
	/** Send single cert to URL. */
	kPSExportDestCertURL,			
	/** */
	kPSExportDestEnumSize
} PSExportDestType;

 
/*************************************************************************
 * Engine Public Properties
 * The properties listed here are public and common to all engines.
 * An engine is free to define its own additional properties.
 * Many of these property names are also used by EScript or as ASCab keys, 
 * which is why (char*) is used for the keys.
 ************************************************************************/

/* Common engine ASText properties */
#define PROP_PSENG_Exception			"exception"				/* Exception string if return code is kDSException */
#define	PROP_PSENG_ProfilePath			"loginPath"				/* Used by JavaScript to get path to DigitalID file */
#define	PROP_PSENG_CN					"loginName"				/* Used by JavaScript to get active signing credential */
#define	PROP_PSENG_HandlerUIName		"uiName"				/* Language dependent name */
#define PROP_PSENG_Text_HandlerVersion	"version"				/* Engine version as text*/
#define	PROP_PSENG_Appearances			"appearances"
#define PROP_PSENG_Directories			"directories"			/* list of directories for this pubsec handler */
#define PROP_PSENG_DirectoryHandlers	"directoryHandlers"		/* list of directory handlers for this pubsec handler */
#define PROP_PSENG_BuildDate			"buildDate"				/* Build date/time for handler */

/* Common engine ASBool properties */
#define	PROP_PSENG_PDSignVisible		"signVisible"			/* If true then can create signatures with visible on-page appearances */
#define	PROP_PSENG_PDSignInvisible		"signInvisible"			/* If true then can create signatures that do not have a signature appearance */
#define	PROP_PSENG_PDSignAuthor			"signAuthor"			/* If true then can create author sigs, that include MDP */
#define	PROP_PSENG_PDSignValidate		"signValidate"			/* If true then can validate signatures */
#define	PROP_PSENG_CosSign				"signFDF"				/* If true then can sign CosDocs */
#define	PROP_PSENG_CosValidate			"validateFDF"			/* If true then can validate CosDoc signatures */
#define	PROP_PSENG_PDEncrypt			"docEncrypt"
#define	PROP_PSENG_PDDecrypt			"docDecrypt"
#define	PROP_PSENG_IsLoggedOn			"isLoggedIn"		/* Do we have a persitent contect? Does not indicate if authentication required. */
#define PROP_PSENG_PDSignCustomAP		"signCustomAP"		/* set to true if you want to do your own signature appearances */
#define	PROP_PSENG_UserInterface		"userInterface"		/* Can operate with UI (not just an escript engine) */
#define PROP_PSENG_ImportContact		"importContact"		/* Must be true if PSImportDataProc and kPSImportDataSigDict or kPSImportDataFDFImportArray are supported */
#define PROP_PSENG_ImportDirSettings	"importDirSettings"	/* Must be true if PSImportDataProc and kPSImportDataFDFDirArray are supported */
#define PROP_PSENG_ExportContact		"exportContact"		/* Must be true if PSExportDataProc and kPSExportDataMyContact are supported */
#define PROP_PSENG_ExportDirSettings	"exportDirSettings"	/* Must be true if PSExportDataProc and kPSExportDataDirSettings are supported */

/** Return true if the handler is capable of signing and verifying PKCS#1 signatures.
    Then handler is then required to support adbe.x509.rsa_sha1 signatures. */
#define PROP_PSENG_SignFormatPKCS1					"signPKCS1"
/** Return true if the handler is capable of signing and verifying PKCS#7 signatures.
    The handler is then required to support both adbe.pkcs7.sha1 and adbe.pkcs7.detached formats */
#define PROP_PSENG_SignFormatPKCS7					"signPKCS7"
/** Return true if the handler must do its own digesting when signing and verifying adbe.x509.rsa_sha1 
    signatures. In other words, if the handler cannot directly sign the digest. */
#define PROP_PSENG_SignFormatPKCS1Digest			"signPKCS1Digest"
/** Return true if the handler must do its own digesting when signing and verifying adbe.pkcs7.detached 
    signatures. In other words, if the handler cannot directly sign the digest. */
#define PROP_PSENG_SignFormatPKCS7DetachedDigest	"signPKCS7DetachedDigest"

/** Return an ASInt32 indicating the number of appearances the handler has.
    This is used for javaScript. */
#define	PROP_PSENG_ASInt32_AppearanceNum	"appearances"
/** Return an ASInt32 indicating the number of directories the handler has.
    This is used for javaScript. */
#define	PROP_PSENG_ASInt32_DirectoryNum		"directories"
/** Return the version of this plug-in. The handler should be careful about setting this value
    in relation to the build dictionary that is created. If a handler ever needs to revoke
	signatures that are created with a version of software that has errors, and the PreRelease
	flag in the build dictionary is not set, then the handler will probably need to use the
	value of this number. */
#define	PROP_PSENG_ASInt32_HandlerVersion	"version"

/** Common engine ASAtom properties */
/** The name by which this handler is to be registered with DigSigHFT, and the alias of the handler
    (primarily used for back compatibility, where the handler has changed it's name over time) 
*/
#define	PROP_PSENG_ASAtom_DigSigHandlerName		"DigSigHandlerName"
#define	PROP_PSENG_ASAtom_DigSigHandlerAlias	"DigSigHandlerAlias"

/** The name by which this handler is to be registered with PubSecHFT (usually same name as DigSig) 
    and the alias of the handler 
*/
#define	PROP_PSENG_ASAtom_PubSecHandlerName		"PubSecHandlerName"
#define	PROP_PSENG_ASAtom_PubSecHandlerAlias	"PubSecHandlerAlias"

/** The preferred signing format to use, specifing the value of the SubFilter. Is overridden by SeedValue and SigInfo. */
#define PROP_PSENG_ASAtom_DefaultSubFilter		"SubFilter"

/*************************************************************************
 * PSPerformOperation parameters
 * Currently called only from EScript.
 * The PubSec plug-in interprets relevant calls and dispatches
 * then to handlers for the operation to be performed.
 ************************************************************************/

/** The type of operation to be performed by PSPerformOperationProc. The PubSec
	plug-in interprets relevant calls and dispatches them to handlers for the operation to be
	performed. 
	@see PSPerformOperationProc
*/
typedef enum {
	/** None */
	kPSOpPerformNone=0,
	/** Silent scripted operation. Access/select file/store containing digital IDs. */
	kPSOpPerformESLogin,			
	/** Silent scripted operation. Deaccess file/store containing digital IDs. */
	kPSOpPerformESLogout,			
	/** Silent scripted operation. Create a new self-sign digital ID. */
	kPSOpPerformESNewUser,		
	/** Silent scripted operation. Deprecated. */
	kPSOpPerformESSetValidateMethod,
	/** Silent scripted operation. Set passwort timeout policy. */
	kPSOpPerformESPasswordTimeout,	
	/** Brings up a UI to display a list of
		certificates contained in an ASCab.
		Returns true if implemented by the
		handler.
	 */
	kPSOpPerformDisplayCertList,	
	/** Returns a list of certificates in an ASCab. */
	kPSOpPerformGetCerts,	
	/** Returns a list of credential stores */
	kPSOpPerformGetStores,
	kPSOpPerformEnumSize
} PSPerformOpType;

/*************************************************************************
 * APPreviewRec - parameter to DSAPFileEditNthEntry
 ************************************************************************/

/** Data with which to build a signature preview in the 
	edit dialog for a signature appearance file entry. 
*/
typedef struct _t_APPreviewRec {
	/** The logo string. */
	const char* logo;
	/** The bounding box for the logo string. */
	ASFixedRect* logoBBox;
	/** The entry name. */
	ASText fName;
	/** The distinguished name. */
	ASText fDN;
	/** The reason for signing. */
	ASText fReason;
	/** The location of signing. */
	ASText fLocation;
	/** The width that determines the aspect ratio of the preview image. */
	ASFixed fWidth;				
	/** The height that determines the aspect ratio of the preview image. */
	ASFixed fHeight;			
	/** true if this is an author signature, false otherwise. */
	bool fbAuthSig;				
} APPreviewRec, *APPreview;

/*************************************************************************
 * Parameters to EScript and cabs
 ************************************************************************/

/* EScript ESRDN object public properties */
#define	PROP_ESRDN_CN				"cn"
#define	PROP_ESRDN_O				"o"
#define	PROP_ESRDN_OU				"ou"
#define	PROP_ESRDN_C				"c"
#define PROP_ESRDN_E				"e"

/* EScript ESCPS object public properties */
#define	PROP_ESCPS_OID				"oid"
#define	PROP_ESCPS_URL				"url"
#define	PROP_ESCPS_NOTICE			"notice"
   
/* EScript Security object public method parameters (also passed to PSPerformOperation) */
#define PARAM_ESSecurity_Password			"cPassword"		/* This is a string */
#define PARAM_ESSecurity_DIPath				"cDIPath"		/* This is an ASPathName */
#define PARAM_ESSecurity_PFX                "cPFX"          /* Instead of a Path to a PFX, the entire PFX can be put here */
#define PARAM_ESSecurity_RDN				"oRDN"			/* If true then acquiring with new user fields */
#define PARAM_ESSecurity_CPS				"oCPS"			/* Generic object containing CPS info to be 
                                                               added to the Self signed certificate */
#define PARAM_ESSecurity_Timeout			"iTimeout"		/* An ASInt32 */
#define PARAM_ESSecurity_DigestSHA1			"cDigestSHA1"	/* SHA-1 digest of certificate */
#define PARAM_ESSecurity_Message			"cMsg"			/* Message to display in login dialog */
#define PARAM_ESSecurity_Method				"cMethod"		/* An ASInt32 */
#define PARAM_ESSecurity_Params				"oParams"		/* */
#define PARAM_ESSecurity_Select				"oSelect"		/* Select Digital ID */
#define PARAM_ESSecurity_Type				"cType"			/* Type of data, operation, etc */
#define PARAM_ESSecurity_Cert				"oCert"			/* Certificate Object */
#define PARAM_ESSecurity_Object				"oObject"		/* Object of unspecified type */
#define PARAM_ESSecurity_EndUserSignCert	"oEndUserSignCert"	/* Cert that is selected to use when signing */
#define PARAM_ESSecurity_EndUserCryptCert	"oEndUserCryptCert"	/* Cert that is selected to use when encrypting */
#define PARAM_ESSecurity_Certs				"certs"			/* Returned array of certificates */
#define PARAM_ESSecurity_Stores				"stores"		/* Stores where the above credentials are stored */
#define PARAM_ESSecurity_URI				"cURI"			/* String - URI for APS connection */
#define PARAM_ESSecurity_UserId				"cUserId"		/* String - User id for APS authentication */
#define PARAM_ESSecurity_Domain				"cDomain"		/* String - Domain name for APS authentication */

#define PROP_Op_Certificates				"certificates"		/* r */
#define PROP_Op_ContactInfo					"contactInfo"
   
/** Signature properties that are written to the signature dictionary. 
	These property names exactly match the corresponding sig dictionary names.
	These are used in PSSigSigPropParamsRec.outNewSigPropCab
	Handlers can specify custom sig properties. These should be named using 
	format ACME_Prop_MyProperty, where ACME is appreviated company name. */
#define PROP_SigProp_Name				"Name"
#define PROP_SigProp_Reason				"Reason"
#define PROP_SigProp_Location			"Location"
#define PROP_SigProp_ContactInfo		"ContactInfo"
#define PROP_SigProp_Date				"M"
#define PROP_SigProp_Filter				"Filter"		/* Handler can override value put in /Filter */
#define PROP_SigProp_AuthType			"Prop_AuthType"
#define PROP_SigProp_AuthTime			"Prop_AuthTime"
#define PROP_SigProp_MDP			    "Prop_MDP"

/** Signature properties that are written to the signature dictionary. 
	These property names exactly match the corresponding build dictionary names.
	These are used in PSSigSigPropParamsRec.outNewSigBuildCab
	Handlers can specify custom sig properties. These should be named using 
	format ACME_MyProperty, where ACME is appreviated company name. */
/** Signature dictionary formatting version number, for specific/private use by the handler. 
	The handler may use this to determine whether the handler supports verification of this signature.
	Set to 0 if unused. */ 
#define PROP_SigBuild_V					"V"
/** Indicates if signature was created by pre-release software. The default value is false. */
#define PROP_SigBuild_PreRelease		"PreRelease"

/* Common engine APCab properties */
#define PROP_APCab_Type					"type"
#define PROP_APCab_Name					"text"
#define PROP_APCab_Index				"index"
   
/* Value (char*) of PROP_APCab_Type when using DSAPIcon APHandler */
#define VALUE_APCab_TypeDSAPIcon		"apIcon"
   
/* Preference settings? */
#define PROP_Reasons			"reasons"			// this is an array when in a cab
#define PROP_More				"more"
#define PROP_Left				"left"
#define PROP_Top				"top"
#define PROP_APIndex			"apIndex"

/************************************************************************************
 * Directory Handler - Declarations
 ***********************************************************************************/

/** Directory Service Provider Object. For Acrobat 6.0 this is a
  PubSecEngine object that acts as a DSP. In future releases this may
  become a stand-along structure. */
typedef void* DirDSP;

 /** A directory information list ASCab contains a vector of nested DirectoryInfo ASCabs,
	one for every directory contained within the engine. Each directory ASCab contains a UI
	name as an ASText and a unique id as an ASAtom.
	@see PSGetDirList
 */
typedef ASCab DirectoryList;

/** A directory handler information ASCab contains a vector of nested ASCabs, one for every
	directory handler contained within the engine. Each directory handler ASCab contains a UI
	name and a unique id; for example, "Adobe.PPKMS.LDAP". 
	@see PSGetDirHandlerInfo 
*/
typedef ASCab DirHandlerInfo;

/* DirHandlerInfo object properties */
/** Required. UI name for the handler. */
#define PROP_DirHandlerInfo_Name	"name"		/* ASText, required. UI friendly name for the handler */
/** Required. Unique identifier for the
	directory handler. The handler is
	rejected if the ASAtom conflicts with
	one already registered.
*/
#define PROP_DirHandlerInfo_ID		 "id"		/* Unique ASAtom identifying the directory handler. Example is Adobe.PPKMS.LDAP. Required */

/**  An ASCab object containing authentication details such as the user name and password. It
	could be used to override the default authentication entries, or to avoid repeated
	authentications in case the context could be cached. The actual contents depend on the
	directory handlers. The currently supported directory handlers (AAB and LDAP) do not
	support this feature.
	@see PSOpenConnection
*/
typedef ASCab DirAuthenticationContext;


/*************************************************************************
 * PubSecHFT Procedure Declarations
 ************************************************************************/

/** Create a new public key security engine for this handler.
   This engine would not be associated with any UI. 
   @see PSDestroyEngineProc
*/ 
typedef ACCBPROTO1 PubSecEngine (ACCBPROTO2 *PSNewEngineProc) ( );

/** Destroys a public key security engine for this handler, freeing the memory.
	@param engine The engine to be destroyed.
	@see PSNewEngineProc
*/ 
typedef ACCBPROTO1 void (ACCBPROTO2 *PSDestroyEngineProc) 
	( PubSecEngine engine );

/** Gets an ASBool property of an engine. (For a list of public properties of a
	PubSecEngine, see PubSecHFT.h.)
	@param engine The engine for which the property value is obtained.
	@param szPropertyName The name of the ASBool property whose value is obtained.
	@param defaultValue The value to return if the property value is not set.
	@return The boolean property value, or the specified default value if the property value is not set.
	@see PSGetAtomPropertyProc
	@see PSGetInt32PropertyProc
	@see PSGetTextPropertyProc
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PSGetBoolPropertyProc)
	( PubSecEngine engine, const char* szPropertyName, const ASBool defaultValue );

/** Gets an ASAtom property of an engine. (For a list of public properties of a
	PubSecEngine, see PubSecHFT.h.)
	@param engine The engine for which the property value is obtained.
	@param szPropertyName The name of the ASAtom property whose value is obtained.
	@return The ASAtom property value, or ASAtomNull if the property value is not set.
	@see PSGetBoolPropertyProc
	@see PSGetInt32PropertyProc
	@see PSGetTextPropertyProc
*/
typedef ACCBPROTO1 ASAtom (ACCBPROTO2 *PSGetAtomPropertyProc)
	( PubSecEngine engine, const char* szPropertyName );

/** Gets an ASInt32 property of an engine. (For a list of public properties of a
	PubSecEngine, see PubSecHFT.h.)
	@param engine The engine for which the property value is obtained.
	@param szPropertyName The name of the ASInt32 property whose value is obtained.
	@param defaultValue The value to return if the property value is not set.
	@return The ASInt32 property value, or the specified default value if the property value is not set.
	@see PSGetAtomPropertyProc
	@see PSGetBoolPropertyProc
	@see PSGetTextPropertyProc
*/
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *PSGetInt32PropertyProc)
	( PubSecEngine engine, const char* szPropertyName, const ASInt32 defaultValue );

/** Gets an ASText property of an engine. (For a list of public properties of a
	PubSecEngine, see PubSecHFT.h.)
	For the PROP_PSENG_Exception property, index is a DSRetCode value. If a handler
	receives a a call to get PROP_PSENG_Exception, it must reset its exception status so
	that subsequent calls to get an exception string return NULL.
	NULL return values are legal for all properties.
	@param engine The engine for which the property value is obtained.
	@param szPropertyName The name of the ASText property whose value is obtained.
	@param index The index of the string to obtain if the property contains a list of values. If it does not, index is ignored.
	@return The ASText property value, or NULL if the property value is not set.
	@see PSGetAtomPropertyProc
	@see PSGetBoolPropertyProc
	@see PSGetInt32PropertyProc
*/
typedef ACCBPROTO1 ASText (ACCBPROTO2 *PSGetTextPropertyProc)
	( PubSecEngine engine, const char* szPropertyName, const ASInt32 index );


/** Called to acquire resources that will be needed to begin a PubSec session. For example, it
	might need to log on before performing a desired operation. If resources are already
	available, the handler may not need to do anything.
	When the function completes successfully, PubSec calls PSSessionReleaseProc with
	the same operation type. Multiple calls can be made to acquire the same or different
	operation types, before a previously acquired resource is released. Handlers should keep a
	reference count if required for a particular resource.
	Despite the name, this function does not acquire a session, just the resources that will be
	needed by the session. For example, to begin a signing session, PubSec calls
	PSSessionAcquire with kPSOpTypePDDocSign. The PubSec handler should select
	the resources and credentials that are to be used for signing. If the call is successfull,
	PubSec will proceed with the PSSigGetSigPropertiesProc and PSSigGetSigValueProc calls.
	@param engine The engine for which the information is obtained.
	@param pdDoc The PDF document for which the session is acquired.
	@param opType The operation that is the reason for acquiring the session.
	@param opText A human-readable version of the reason for acquiring the session. If
	not supplied, opType is used to build text.
	@param cabParams An ASCab object containing information about the acquisition,
	owned by PubSec. Currently not used, passed as NULL.
	@param bUIAllowed When true, the call can invoke the user-interface dialogs if needed.
	@return Positive on success.
	@see PSSessionReadyProc
	@see PSSessionReleaseProc
*/
typedef ACCBPROTO1 DSRetCode (ACCBPROTO2 *PSSessionAcquireProc)
	( PubSecEngine engine, PDDoc pdDoc, PSSessionOpType opType, ASText opText, ASCab cabParams, ASBool bUIAllowed );

/** Releases any resources that were required for the specified operation, such as file handles.
	It is up to a handler to decide what resources to release. A handler can, for example, leave a
	user logged on, even after a session is released.
	This call can fail. For example, you might want the call to fail if the operation is
	kPSOpTypeEScriptLogin and encrypted documents are open.
	@param engine The engine for which the session was acquired.
	@param opType The operation for which the session was needed.
	@return kDSTrue if the session is successfully released, otherwise kDSFalse.
	@see PSSessionAcquireProc
	@see PSSessionReadyProc
*/
typedef ACCBPROTO1 DSRetCode (ACCBPROTO2 *PSSessionReleaseProc)
	( PubSecEngine engine, PSSessionOpType opType );

/** Returns kDSTrue if the resources and information needed to perform the specified
	operation have been acquired.
	@param engine The engine for which the information is obtained.
	@param opType The operation for which the session is needed.
	@return kDSTrue if the session is ready, otherwise kDSFalse.
	@see PSSessionAcquireProc
	@see PSSessionReleaseProc
 */
typedef ACCBPROTO1 DSRetCode (ACCBPROTO2 *PSSessionReadyProc)
	( PubSecEngine engine, PSSessionOpType opType );

/** Performs the specified operation using parameters contained in cab. This interface is used
	by EScript.
	@param engine The engine for which the operation is performed.
	@param type The type of operation to perform.
	@param cab An ASCab containing parameters for the requested operation, or NULL to test whether the operation is supported.
	@param bUI When true, the call can invoke the user-interface dialogs if needed.
	@return If cab is NULL, returns kDSTrue if the operation is supported, or kDSFalse if it is not.
	If cab contains data, returns kDSOk if the operation is successful, an exception code if not.
*/
typedef ACCBPROTO1 DSRetCode (ACCBPROTO2 *PSPerformOperationProc)
	( PubSecEngine engine, PSPerformOpType type, const ASCab cab, ASBool bUI );


/** Called when creating a new signature. For PDDoc signatures, this call replaces SigNew
and SigCommit calls. It returns the values that PubSec writes into the signature dictionary
in the parameters structure.
Before making this call, the handler should use PSSessionAcquireProc to choose the
credential to be used for signing.
The handler can use the UI during this call to allow authentication and to bring up the
signing dialog.
	@param engine The engine for which signature properties are retrieved.
	@param params A signature properties parameters structure.
	@return kDSOk if successful, otherwise an exception code.
	@see PSGetLogoProc
	@see PSSessionAcquireProc
	@see PSSigValGetAPLabelProc
	@see PSSigGetSigValueProc
*/
typedef ACCBPROTO1 DSRetCode (ACCBPROTO2 *PSSigGetSigPropertiesProc)
	( PubSecEngine engine, PSSigSigPropParams params );

/**
	Called to authenticate the signer. The caller can determine 
	whether authentication is required by calling PSGetBoolPropertyProc 
	with the property PROP_PSENG_IsAuthenticated. 

	This procedure is called only if specified by the PSSigSigPropParams 
	passed to PSSigGetSigPropertiesProc. If your handler does 
	not need this call, it should be defined to always return 
	kDSTrue. 
	@param engine The engine for which signature authentication 
	is performed. 
	@param pdDoc A PDF document for window parenting. NULL 
	if there is no PDDoc. 
	@param inESParams Optional. An ASCab containing authentication 
	parameters. If not supplied or if authentication fails, 
	and if bInUIAllowed is true, brings up the authentication 
	UI to obtain parameters. 
	@param bInUIAllowed When true, the call can invoke the 
	authentication UI. 
	@return kDSTrue if authentication succeeds, kDSFalse if authentication 
	fails. 
	@see PSGetBoolPropertyProc 
	@see PSSigGetSigValueProc 
	@see PSSigValidateProc 
*/
typedef ACCBPROTO1 DSRetCode (ACCBPROTO2 *PSSigAuthenticateProc)
	( PubSecEngine engine, const PDDoc pdDoc, ASCab inESParams, ASBool bInUIAllowed );

/**
	Called to sign the digest and return the signature value. 
	The memory for the signature value becomes the property 
	of the caller. 

	When called for a PKCS #7 signature, there is a "size-only" 
	option: if the value of pOutSigValueData in the inOutParams 
	structure is NULL, returns the expected size of the signature 
	value. 

	For most values of digestMethod in the inOutParams structure, 
	the digest is passed into the handler, and the handler needs 
	to sign it. However if a handler specifies that it must 
	do its own digest ( for example, for the legacy EntrustFile 
	toolkit) then a NULL digest is passed in. The handler can 
	specify this using PROP_PSENG_SignFormatPKCS7Digest. If 
	and only if this property is true, the digest is not passed 
	to the handler. 

	This procedure should not show any user interface and not 
	allow a user to cancel the signing operation. 
	@param engine The engine for which a signature value is 
	created. 
	@param inOutParams (Modified by the method) A structure containing 
	signature-value parameters and return values.
	@return kDSOk if successful, otherwise an exception code. 
	@see PSSigAuthenticateProc 
	@see PSSigGetSigPropertiesProc 
	@see PSSigValidateProc 
*/
typedef ACCBPROTO1 DSRetCode (ACCBPROTO2 *PSSigGetSigValueProc)
	( PubSecEngine engine, PSSigGetSigValueParams inOutParams );

/**
	Called to determine whether the handler can validate a signature. 
	
	@param engine The engine for which the signature is validated. 
	
	@param params A parameters structure containing information 
	about the signature to be validated.
	@return A signature-validation support value. 
	@see PSSigAuthenticateProc 
	@see PSSigGetSigValueProc 
	@see PSSigValidateDialogProc 
	@see PSSigValidateProc 
*/
typedef ACCBPROTO1 PSSigValSupport (ACCBPROTO2 *PSSigValidateSupportedProc)
	( PubSecEngine engine, PSSigValidateSupportParams params );

/**
	Called to validate a signature. 
	@param engine The engine for which the signature is validated. 
	
	@param params (Modified by the method) A signature validation 
	parameters and return values structure. The result of validation 
	is stored in the sigValCab field.
	@return kDSOk if successful, otherwise an exception code. 
	@see PSSigAuthenticateProc 
	@see PSSigGetSigValueProc 
	@see PSSigValidateDialogProc 
	@see PSSigValidateSupportedProc 
*/
typedef ACCBPROTO1 DSRetCode (ACCBPROTO2 *PSSigValidateProc)
	( PubSecEngine engine, PSSigValidateParams params );

/**
	Called to show a dialog that shows validation status, after 
	the signature is validated. 

	If this procedure is NULL (which is recommended), PubSec 
	opens its own validation dialog. 
	@param engine The engine for which the validation status 
	is shown. 
	@param valParams A validation dialog parameters structure. 
	The validation state resulting from the previous validation 
	operation is passed in the sigValCab field. The handler 
	can update the dsPropType field if the properties dialog 
	should be opened next.
	@return kDSOk if successful, otherwise an exception code. 
	@see PSSigAuthenticateProc 
	@see PSSigGetSigValueProc 
	@see PSSigPropDialogProc 
	@see PSSigValidateProc 
	@see PSSigValidateSupportedProc 
*/
typedef ACCBPROTO1 DSRetCode (ACCBPROTO2 *PSSigValidateDialogProc)
	( PubSecEngine engine, PSSigValidateDialogParams valParams );

/**
	Called to show a dialog that shows signature properties. 
	

	If this procedure is NULL (which is recommended), PubSec 
	opens its own properties dialog. The default properties 
	dialog includes buttons that allow the user to import and 
	to display certificates: 

	- The import button calls PSImportDataProc. 
	- If the handler implements the kPSOpPerformDisplayCertList 
	operation (see PSPerformOperationProc) the show-certificate 
	button can open the handler-provided display dialog. Otherwise, 
	the button opens the default certificate-display dialog. 
	
	@param engine The engine for which the validation status 
	is shown. 
	@param valParams (Modified by the method) A properties-dialog 
	parameters and return values structure. The handler can 
	set the dsPropType to kSDPropViewVersion to request rollback 
	to this signature when the dialog is closed.
	@return kDSOk if successful, otherwise an exception code. 
	@see PSSigValidateDialogProc 
*/
typedef ACCBPROTO1 DSRetCode (ACCBPROTO2 *PSSigPropDialogProc)
	( PubSecEngine engine, PSSigValidateDialogParams valParams );

/**
	Called to get the text result of validation. For most values 
	of textType, the handler should return NULL, causing PubSec 
	to use the default text strings ("Valid," "Invalid," or 
	"Unknown," depending on the validity state). Handlers will 
	normally need to provide their own ID validity strings. 
	
	@param valCab The ASCab containing the validation result. 
	(See PSSigValidateParams' sigValCab.) 
	@param textType The type of result text to obtain, for 
	a specific context. 
	@param index If the type is kDSSigValTextAP, the corresponding 
	index value. Otherwise ignored.
	@return The text result of validation as a new ASText object. 
	@see PSSigValidateProc 
	@see PSSigValidateDialogProc 
*/
typedef ACCBPROTO1 ASText (ACCBPROTO2 *PSSigValGetTextProc)
	( ASCab valCab, const DSSigValText textType, const ASInt32 index );

/**
	Creates the signature appearance that is put into the /AP 
	dictionary /N entry. 

	It is recommended that you set this procedure to NULL, w 
	hich allows PubSec to use the default APHandler to generate 
	the XObj for the appearance. 
	@param engine The engine for which an appearance is created. 
	
	@param pOutXObj (Filled by the method) The XObject to 
	use for the signature appearance. 
	@param docParams A validation parameters structure containing 
	information that is needed to create the signature appearance, 
	such as the PDDoc and annotation. 
	@param sigType The signature type.
	@return kDSOk if successful, otherwise an exception code. 
	@see PSGetLogoProc 
	@see PSSigValGetAPLabelProc 
*/
typedef ACCBPROTO1 DSRetCode (ACCBPROTO2 *PSSigCreateAPNXObjProc)
	( PubSecEngine engine, CosObj *pOutXObj, PSSigPDDocParams docParams, PSAPSigType sigType );

/**
	Gets custom artwork from the handler, to be used as a label 
	for a particular validity state. The artwork for standard 
	labels does not need to be specifically retrieved. If you 
	are not using dynamic signature appearances or not using 
	custom artwork, the handler need only handle the null-label 
	case. 
	@param label A validity state, as returned by PSSigValGetAPLabelProc, 
	for which to use this logo. If ASAtomNull, the logo is used 
	as an invariable watermark of the signature appearance. 
	
	@param pcLogo (Filled by the method) A string of the uncompressed 
	graphics stream for the logo artwork. 
	@param pRect (Filled by the method) The precise bounding 
	box that the artwork occupies.
	@see PSSigCreateAPNXObjProc 
	@see PSSigValGetAPLabelProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PSGetLogoProc)
	( ASAtom label, const char* *pcLogo, const ASFixedRect* *pRect );

/**
	Called to get a name to use for a particular layer of signature 
	appearance. This is used when the signature appearance uses 
	the n1 and n3 layers for dynamic signature appearances. 
	Beginning in Acrobat 6.0, Acrobat is discouraging the use 
	of dynamic signature appearances and is instead showing 
	the signature validity as an icon that is rendered at run 
	time. 

	Provide this prededure for handlers that need to be backward 
	compatible with earlier implementations that use custom 
	artwork. The procedure can be set to NULL when not using 
	dynamic signature appearances. 

	If the value DSAPValid, DSAPDoubleValid, or DSAPInvalid 
	is returned, Acrobat uses standard labels. Otherwise Acrobat 
	calls PSGetLogoProc to return logo artwork to use for the 
	XObject. A return value of ASAtomNull causes Acrobat to 
	use a blank XObject for the specified layer. 
	@param valCab An ASCab containing the signature validation 
	result for which the label is obtained. 
	@param layerNum The layer for which a label is obtained.
	@return The label of a signature appearance layer as an ASAtom. 
	
	@see PSGetLogoProc 
	@see PSSigCreateAPNXObjProc 
*/
typedef ACCBPROTO1 ASAtom (ACCBPROTO2 *PSSigValGetAPLabelProc)
	( ASCab valCab, const ASInt32 layerNum );

/**
	This function sends data of a particular type to a handler 
	to import into its own data store. This is call is executed 
	in response to data received, for example, through an FDF 
	file or through the signature dictionary. 

	Acrobat calls PSGetBoolPropertyProc to see if the relevant 
	data type is supported in the handler implementation. See 
	PSImportDataType. 
	@param engine The engine for which the data is exported. 
	
	@param params A structure that contains the data to be 
	imported. 
	@param bInUIAllowed When true, the call can invoke the 
	user-interface dialogs if needed.
	@return Positive on success. 
	@see PSExportDataProc 
*/
typedef ACCBPROTO1 DSRetCode (ACCBPROTO2 *PSImportDataProc)
	( PubSecEngine engine, PSImportDataParams params, ASBool bInUIAllowed );

/**
	This function gets data of the specified type from a handler, 
	to be exported to an FDF or CMS file and possibly sent as 
	an email attachment. The function is called when exporting 
	to FDF, for example, in response to an FDF Data Exchange 
	certificate request. 

	Acrobat calls PSGetBoolPropertyProc to see if the relevant 
	data type is supported in the handler implementation. See 
	PSExportDataType. 
	@param engine The engine for which the data is exported. 
	
	@param dataType The type of data to be exported. 
	@param outCab The ASCab containing the data to be exported. 
	
	@param bInUIAllowed When true, the call can invoke the 
	UI to export the data. 
	@return Positive on success. 
	@see PSImportDataProc 
*/
typedef ACCBPROTO1 DSRetCode (ACCBPROTO2 *PSExportDataProc)
	( PubSecEngine engine, PSExportDataType dataType, ASCab outCab, ASBool bInUIAllowed );

/**
	This procedure is required. Called to open the provided 
	PKCS#7 cryptographic message service (CMS) enveloped data 
	object and return the data contained in it. 
	@param engine The engine for which the data is exported. 
	@param inCMSEnvelope buffer that contains the enveloped data object
	@param inCMSEnvelopeSize size of the buffer pointed to by inCMSEnvelope
	@param pOutData buffer containing the decrypted data if successful
	@param pOutDataSize size of the buffer pointed to by pOutData
	@param bInUIAllowed When true, the call can invoke the 
	UI for anything required to open the envelope. For instance, 
	a login may be needed to access the user's private key.
	@return kDSTrue if the envelope was opened, kDSFalse if the envelope 
	could not be opened, or an error code in case of error. 
	
*/
typedef ACCBPROTO1 DSRetCode (ACCBPROTO2 *PSOpenCMSEnvelopeProc)
	( PubSecEngine engine, 
	  ASUns8* inCMSEnvelope, ASUns32 inCMSEnvelopeSize, 
	  ASUns8** pOutData, ASUns32* pOutDataSize, 
	  ASBool bInUIAllowed );

/** PSOpenCMSEnvelopeExProc
   Open the provided PKCS#7 (CMS) Enveloped Data object and return the
   data contained in it.
   @param engine The engine for which the data is exported.
   @param params contains the envelope data and fields for the handler to 
     return opened envelope data contents and optionally the session key
     and opening certificate.
   @param bInUIAllowed indicates whether the handler can pop-up any UI to open the envelope. For
     instance, a log-in maybe required to access the user's private key.
   @return
   - kDSTrue means the envelope was opened
   - kDSFalse means the envelope could not be opened */
typedef ACCBPROTO1 DSRetCode (ACCBPROTO2 *PSOpenCMSEnvelopeExProc)
	( PubSecEngine engine, 
	  PSOpenCMSEnvelopeParams params, 
	  ASBool bInUIAllowed );

/**
	Gets information about directory handlers in an engine that 
	is acting as a directory service provider. 
	@param engine The engine for which the information is 
	obtained. 
	@param outCertList (Filled by the method) An ASCab 
	containing an array of ASCab directory handlers.
	@param bInUIAllowed When true, the call can invoke the 
	UI to display the directory handlers.
	@return Positive on success. 
	@see PSGetDirInfo 
	@see PSGetDirList 
	@see PSSetDirInfo 
*/
typedef ACCBPROTO1 DSRetCode (ACCBPROTO2 *PSGetImplicitRecipientsProc)
	( PubSecEngine engine, ASCab outCertList, ASBool bInUIAllowed );


/**
	Gets information about directory handlers in an engine that 
	is acting as a directory service provider. 
	@param engine The engine for which the information is 
	obtained. 
	@param outDirHandlerInfo (Filled by the method) A structure 
	containing an array of ASCab directory handlers.
	@return Positive on success. 
	@see PSGetDirInfo 
	@see PSGetDirList 
	@see PSOpenConnection 
	@see PSSetDirInfo 
*/
typedef ACCBPROTO1 DSRetCode (ACCBPROTO2 *PSGetDirHandlerInfo) 
	( PubSecEngine engine, DirHandlerInfo outDirHandlerInfo );

/**
	Gets a list of directories in an engine that is acting as 
	a directory service provider. 
	@param engine The engine for which the information is 
	obtained. 
	@param outDirList (Filled by the method) An ASCab containing 
	an array of DirectoryInfo ASCab objects.
	@return Positive on success. 
	@see PSGetDirHandlerInfo 
	@see PSGetDirInfo 
	@see PSOpenConnection 
	@see PSSetDirInfo 
*/
typedef ACCBPROTO1 DSRetCode (ACCBPROTO2 *PSGetDirList) 
	( PubSecEngine engine, DirectoryList outDirList );

/**
	Gets information about directories in an engine that is 
	acting as a directory service provider. 
	@param engine The engine for which the information is 
	obtained. 
	@param inDirID The unique identifier associated with the 
	directory. See DirHandlerInfo. 
	@param outDirInfo (Filled by the method) An ASCab containing 
	information about the directory.
	@return Positive on success. 
	@see PSGetDirHandlerInfo 
	@see PSGetDirList 
	@see PSOpenConnection 
	@see PSSetDirInfo 
*/
typedef ACCBPROTO1 DSRetCode (ACCBPROTO2 *PSGetDirInfo) 
	( PubSecEngine engine, ASAtom inDirID, DirectoryInfo outDirInfo );

/**
	Sets information about a directory in an engine that is 
	acting as a directory service provider. 
	@param engine The engine for which the information is 
	obtained. 
	@param inDirInfo An ASCab containing information about 
	the directory.
	- If the PROP_DirectoryInfo_ID value matches 
	one of the existing directories, that directory's information 
	is overwritten.
	- If it does not match any existing directory, 
	a new directory is created.
	@return kDSOk if successful, otherwise an exception code. 
	@see PSGetDirHandlerInfo 
	@see PSGetDirInfo 
	@see PSGetDirList 
	@see PSOpenConnection 
*/
typedef ACCBPROTO1 DSRetCode (ACCBPROTO2 *PSSetDirInfo) 
	( PubSecEngine engine, DirectoryInfo inDirInfo );

/**
	Opens a connection to a specified directory to use for authentication. 
	
	@param engine The engine for which the connection is opened. 
	
	@param inDirID The identifier for the directory to which 
	the connection is opened. 
	@param inAuthCtx Not currently supported. Pass as NULL. 
	
	@param inUI Whether the authentication UI should be shown. 
	
	@param pOutConnection (Filled by the method) A pointer 
	to the new connection object.
	@return kDSTrue if the connection is opened, or kDSFalse if it is 
	not. 
	@see PSGetDirHandlerInfo 
	@see PSGetDirInfo 
	@see PSGetDirList 
	@see PSSetDirInfo 
*/
typedef ACCBPROTO1 DSRetCode (ACCBPROTO2 *PSOpenConnection) 
	( PubSecEngine engine, ASAtom inDirID, DirAuthenticationContext inAuthCtx, ASBool inUI, 
	  DirConnection* pOutConnection );

/** Ask the handler to report a summary on a signature that has just been signed by this handler. 	*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PSSigSignReportProc) 
	( PubSecEngine engine, PSSigSignReportParams);


/** PubSecHandlerRec Definition
	See PSRegisterHandler for registration of PubSecHandlers
	@see PSRegisterHandler
*/
typedef struct _t_PubSecHandlerRec {
	/** */
	ASSize_t size;
	/** Used internally. */
	_t_PubSecHandlerRec *next;
	/** Used internally. */
	DigSigHandler dsHandler;
	/** Used internally. */
	PDCryptHandler cryptHandler;
	/** Engine to use for UI-based execution. */
	PubSecEngine engine;

	/* Engine constructor/destructor */
	/** */
	PSNewEngineProc					newEngine;
	/** */
	PSDestroyEngineProc				destroyEngine;

	/* Properties */
	/** */
	PSGetBoolPropertyProc			getBoolProperty;
	/** */
	PSGetAtomPropertyProc			getAtomProperty;
	/** */
	PSGetInt32PropertyProc			getInt32Property;
	/** */
	PSGetTextPropertyProc			getTextProperty;

	/* Engine acquire/release/performOperation */
	/** */
	PSSessionAcquireProc			sessionAcquire;
	/** */
	PSSessionReleaseProc			sessionRelease;
	/** */
	PSSessionReadyProc				sessionReady;
	/** */
	PSPerformOperationProc			performOperation;

	/* Signing methods */
	/** */
	PSSigGetSigPropertiesProc		sigGetSigProperties;
	/** */
	PSSigAuthenticateProc			sigAuthenticate;
	/** */
	PSSigGetSigValueProc			sigGetSigValue;
	
	/* Validation methods */
	/** */
	PSSigValidateSupportedProc		sigValidateSupported;
	/** */
	PSSigValidateProc				sigValidate;
	/** */
	PSSigValidateDialogProc			sigValidateDialog;
	/** */
	PSSigPropDialogProc				sigPropDialog;
	/** */
    PSSigValGetTextProc				sigValGetText;

	/* Signature appearance methods */
	/** */
	PSGetLogoProc					getLogo;
	/** */
    PSSigValGetAPLabelProc			sigValGetAPLabel;
	/** */
	PSSigCreateAPNXObjProc			sigCreateAPNXObj;

	/* Contact Exchange */
	/** */
	PSImportDataProc				importData;
	/** */
	PSExportDataProc				exportData;

	/* Encryption methods */
	/** */
	PSOpenCMSEnvelopeProc			cryptOpenCMSEnvelope;
	/** */
	PSGetImplicitRecipientsProc		cryptGetImplicitRecipients;

	/* Directory Service Provider methods */
	/** */
	PSGetDirHandlerInfo				 dirGetDirHandlerInfo;
	/** */
	PSGetDirList					 dirGetDirList; 
	/** */
	PSGetDirInfo					 dirGetDirInfo;
	/** */
	PSSetDirInfo					 dirSetDirInfo;
	/** */
	PSOpenConnection				 dirOpenConnection;

	/* Additional methods */
	/** */
	PSOpenCMSEnvelopeExProc			cryptOpenCMSEnvelopeEx;

	PSSigSignReportProc				sigSignReport;
} PubSecHandlerRec, *PubSecHandler;

/************************************************************************************
 * PubSecHFT - HFT index table
 ***********************************************************************************/

/* HFTBeginEnum - do not alter/remove this line! */

enum
{
	PubSecHFTDUMMYBLANKSELECTOR,

	PSRegisterHandlerSEL,
	PSUnregisterHandlerSEL,
	PSCountEncryptedDocsSEL,
	PSCloseEncryptedDocsSEL,
	PSSigValidatePDDocSigFieldSEL,
	PSDataBufferEnumSEL,
	PSDataBufferDigestSEL,
	DSAPFileAcquireSEL,
	DSAPFileReleaseSEL,
	DSAPFileSaveSEL,
	DSAPFileGetCountSEL,
	DSAPFileCanDeleteNthEntrySEL,
	DSAPFileGetNewNthNameSEL,
	DSAPFileRemoveNthEntrySEL,
	DSAPFileEditNthEntrySEL,
	DSAPFileCopyNthEntrySEL,
	AABIsCertPresentSEL,
	AABGetCertTrustSEL,
	AABFindCertsByNameSEL,
	AABGetTrustedCertsSEL,
	AABGetCertChainSEL,
	PSExportDataExchangeSEL,
	PSImportDataExchangeSEL,
	PSCertIssuedUnderTestCPSEL,

	PubSecHFTNUMSELECTORSPlusOne
};

#define PubSecHFT_NUMSELECTORS (PubSecHFTNUMSELECTORSPlusOne - 1)

/* HFTEndEnum - do not alter/remove this line! */

/************************************************************************************
 * PubSecHFT - Declarations of structs used by prototypes
 ***********************************************************************************/

/* Top level entries in PSExportDataExchangeParamsRec.dataCab. */
#define PROP_ExportDataCab_CN					"cn"			/* Text, used to identify originator of FDF */
#define PROP_ExportDataCab_EMail				"EMail"			/* Text, request reply-email-address */
#define PROP_ExportDataCab_URL					"URL"			/* Text, request reply-URL */
#define PROP_ExportDataCab_Filter				"Filter"		/* Text, language independent name of the (pubsec) handler that generated this file */
#define PROP_ExportDataCab_Contacts				"Contacts"		/* A cab array of Contact cabs */
#define PROP_ExportDataCab_DirSettings			"DirSettings"	/* A cab array of DirSetting cabs */

/* Entries in Contact Cab */
#define PROP_ContactCab_CN						"cn"			/* Text, common name */
#define PROP_ContactCab_EMail					"EMail"			/* Text, email address */
#define PROP_ContactCab_O						"o"				/* Text, corporation */
#define PROP_ContactCab_ContactInfo				"ContactInfo"	/* Text, 'contact info', eg phone number, used to verify origin of cert */
#define PROP_ContactCab_CMS						"CMS"			/* Binary CMS object, in lieu of certificates */
#define PROP_ContactCab_Certs					"Certs"			/* A cab array of binary certificates, in lieu of CMS */
#define PROP_ContactCab_Trust					"Trust"			/* A cab array of trust flags associate with this contacts certificates */
#define PROP_ContactCab_Policy					"Policy"		/* A cab array of policy cabs associated with this contacts certificates */
#define PROP_ContactCab_Group					"Group"			/* Text, the name of a group, if this contact belongs to a group */
#define PROP_ContactCab_Contact					"Contact"		/* Text, the name of the contact, if this is an exported contact */

/* Entries in DirSetting Cab */
#define PROP_DirSettingCab_Type					"Type"			/* Text, always 'DirSetting' */
#define PROP_DirSettingCab_Attr					"Attr"			/* Cab containing DirectoryInfo entries as specified in the DirectoryHFT */

/* Entries in certificate's Policy cab */
#define PROP_CertPolicyCab_OID					"PolicyOID"		/* A cab array of Text policy OIDs in dotted notation */
#define PROP_CertPolicyCab_UFName				"PolicyUFName"	/* Text, a human-readable description for the OID */

/** Parameters for importing data from a file into PubSec. */
typedef struct _t_PSImportDataExchangeParamsRec {
	/** The size of the structure. */
	ASSize_t size;
	/** The handler to use to accept the import data, if it accepts the specified type. */
	PubSecHandler psHandler;			
	/** The engine to use to accept the import data. If not supplied, the method uses the default (UI) engine. */
	PubSecEngine psEngine;				
	/** The type of data to be imported. If data of this type is not found in the FDF, the operation fails. . */
	PSExportDataType dataType;			
	/** The file system used for filePath. */
	ASFileSys fileSys;					
	/** The path of the file to be opened for import. */
	ASPathName filePath;				
} PSImportDataExchangeParamsRec, *PSImportDataExchangeParams;

/** Parameters for exporting data from PubSec to a file.
	 The PSExportDataExchangeParamsRec.dataCab format is as follows: A
   top level cab will contain lists of PROP_ExportDataCab_Contacts,
   PROP_ExportDataCab_DirSettings or whatever other data is to be
   included.  This top level cab can include entries for an EMail
   address (used as the return email address) and cn. It is
   recommended that the cn entry be set to the common name of the
   user; this will be used only in creating the filename for the FDF
   (or PKCS#7) file. If either of cn or EMail address are not provided
   then they will be fetched from the AVIdentity preference
   settings. Contents of the individual list entries are added as
   CosDicts to the FDF file with almost know filtering. The filtering
   is to add a /Type attribute and to turn /Certs entries into /CMS
   entries (meaning that a contact can contain Certs entry that is a
   list of certificates). Refer to the FDF Data Exchange spec for
   details on attributes.  Each contact can include a list of certs
   and an email address. Contacts that contain lists of certs will
   have these lists turned into a CMS object because the FDF format
   supports only CMS. Contacts can directly provide the CMS object if
   they choose.  Each directory entry contains directory-specific
   information that can be converted to a CosDict before storing in
   the FDF file.  File entries give cDIPaths to files that are to be
   embedded.  PubSecHandler and PubSecEngine are used to specify the
   handler to use to sign the data exchange file (if it is signed at
   all).

   Example cab for exporting MyContact:
	dataCab : Cab
		"Contacts" : Cab
			"0" : Cab
				"EMail" : "jsmith@smithcorp.com"
				"Certs" : Cab
					"0" : binary end entity cert
		"cn" : "John Smith"
		"EMail" : "jsmith@smithcorp.com"

   Example cab for exporting dir settings:
	dataCab : Cab
		"DirSettings" : Cab
			"0" : Cab
				"Port" : 369
	@see PSExportDataExchange 
*/
typedef struct _t_PSExportDataExchangeParamsRec {
	/** The size of the structure. */
	ASSize_t size;
	/** If exporting FDF, the handler to use to sign the FDF. If not supplied,
		the user is prompted for a handler 
	*/
	PubSecHandler psHandler;			
	/** If exporting FDF, the engine to use to sign the FDF. If not supplied,
		the method uses the default (UI) engine.
	*/
	PubSecEngine psEngine;				
	/** Type of data to be exported. One of:
		kPSExportDataNone: Not specified.
		kPSExportDataMyContact: Export the user's own contact
		information (certificates for encryption and/or signing).
		kPSExportDataContacts: Export a list of contacts from an
		address book or directory that are to be shared.
		kPSExportDataDirSettings : Export list of directory
		settings that can be used to help someone else configure their directory.
	*/
	PSExportDataType dataType;			
	/** Required. Data to export, or empty ASCab if no data to export. Format defined below. */
	ASCab dataCab;						
	/** The type of data to be requested. If supplied, the method adds a
		request to the FDF for this data type and forces the export format to
		be FDF. Same possible values as dataType.
	*/
	PSExportDataType requestType;		
	/** The destination type that specifies how data should be delivered.
		One of:
		kPSExportDestNone: Not specified, prompt the user
		(recommended)
		kPSExportDestFile: Save as unspecified file type, possibly
		prompting for the file type.
		kPSExportDestCMSFile: Save as CMS file.
		kPSExportDestFDFFile: Save as FDF file.
		kPSExportDestEMailFDF: Save a email FDF file.
		kPSExportDestCertURL: Send single certificate to URL. 
	*/
	PSExportDestType destType;			
} PSExportDataExchangeParamsRec, *PSExportDataExchangeParams;

/*
   */

/************************************************************************************
 * PubSecHFT - Prototype declarations
 ***********************************************************************************/

/* HFTBeginProto - do not alter/remove this line! */


/**
	Registers a handler with the PubSec HFT. The caller retains 
	ownership of the PubSecHandlerRec. 
	@param owner The handler's plug-in ExtensionID, assigned 
	at initialization. 
	@param psHandler The handler structure containing the 
	handler methods to register.
	@return true if successful, false otherwise. 
	@see PSUnregisterHandler 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PSRegisterHandlerSELPROTO)
	( ExtensionID owner, PubSecHandler psHandler  );
#define PSRegisterHandler (*((PSRegisterHandlerSELPROTO)(gPubSecHFT[PSRegisterHandlerSEL])))

/**
	Unregisters a handler from the PubSec HFT. This does not 
	destroy the handler; the caller owns the PubSecHandlerRec. 
	
	@param psHandler The handler to unregister.
	@return true if successful, false otherwise. 
	@see PSRegisterHandler 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PSUnregisterHandlerSELPROTO)
	( PubSecHandler psHandler  );
#define PSUnregisterHandler (*((PSUnregisterHandlerSELPROTO)(gPubSecHFT[PSUnregisterHandlerSEL])))

/*************************************************************************
 * Doc Cache methods. PubSec keeps a list of all encrypted documents
 * that are open. For security reasons handlers will want these
 * documents to all be closed when a handler releases access to
 * critical resources, for example when logging out.
 ************************************************************************/

/**
	Returns the number of encrypted documents associated with 
	a PubSec engine. Returns separate values for documents that 
	need to be saved, and for those that do not need to be saved 
	and can be safely closed. 
	@param engine The engine for which the encrypted docs 
	are counted. 
	@param outNeedSave (Filled by the method) A pointer to 
	the number of encrypted documents associated with the engine 
	that need to be saved. 
	@param outCanClose (Filled by the method) A pointer to 
	the number of encrypted documents associated with the engine 
	that do not need to be saved and can be safely closed.
	@see PSCloseEncryptedDocs 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PSCountEncryptedDocsSELPROTO)
	( PubSecEngine engine, ASUns32 *outNeedSave, ASUns32 *outCanClose );
#define PSCountEncryptedDocs (*((PSCountEncryptedDocsSELPROTO)(gPubSecHFT[PSCountEncryptedDocsSEL])))

/**
	Closes all encrypted documents associated with a PubSec 
	engine, regardless of whether they need to be saved or not. 
	Use PSCountEncryptedDocs to determine if there are any documents 
	that will need to be opened or saved. 

	PubSec keeps a list of all open encrypted documents. For 
	security reasons, handlers will want all of these documents 
	to be closed when it releases access to critical resources, 
	for example when logging out. Use this method (rather than 
	closing the documents directly) so that PubSec can maintain 
	its cache correctly. 
	@param engine The engine for which the encrypted docs 
	are closed.
	@return true if successful, false otherwise. 
	@see PSCountEncryptedDocs 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PSCloseEncryptedDocsSELPROTO)
	( PubSecEngine engine );
#define PSCloseEncryptedDocs (*((PSCloseEncryptedDocsSELPROTO)(gPubSecHFT[PSCloseEncryptedDocsSEL])))

/*************************************************************************
 * Signature operations
 ************************************************************************/

/**
	Validates a specified signature field in a PDDoc. For example, 
	you might call this from the validate button of a signature 
	properties dialog, or if any information used during validation 
	is changed. Does not bring up any UI. 

	A return value of true indicates that the validation operation 
	was successfully peformed, but does not provide any information 
	about the result of the validation (that is, the signature's 
	validity value). The method does not return validity information, 
	but simply updates the DigSig and PubSec validation caches. 
	

	NOTE: This method cannot validate a signature whose cache 
	has not been updated or is NULL. In this case, use the method 
	DigSigVerifySig. 
	@param docParams The validation parameters.
	@return true if the validation was successfully performed, false 
	otherwise. 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PSSigValidatePDDocSigFieldSELPROTO)
	( PSSigPDDocParams docParams  );
#define PSSigValidatePDDocSigField (*((PSSigValidatePDDocSigFieldSELPROTO)(gPubSecHFT[PSSigValidatePDDocSigFieldSEL])))

/**
	Gets bytes of data to digest when signing or verifying, 
	in chunks of a specified size. Continues getting data chunks 
	until all of the data in the data buffer has been returned. 
	

	A handler will use this call when computing its own data 
	digest, to get the next blob of bytes to digest. The dataBuffer 
	object keeps track of the bytes that have been returned, 
	of how many bytes remain to be returned, and of the byte 
	ranges of data to be provided. 

	When signing or verifying a PDDoc, the data buffer object 
	is a PDDoc handle and the bytes returned will be those defined 
	by /ByteRange in the signature object dictionary. See the 
	PDF Reference for details. 

	The PSDataBufferDigest uses this method when computing the 
	digest for the data. 
	@param dataBuffer The buffer containing the data. 
	@param maxSize The maximum number of bytes to return in 
	the return buffer. 
	@param pReturnBuffer (Filled by the method) A pointer 
	to the buffer containing the current bytes to be processed. 
	If NULL, an error occurred and you should abort the enumeration. 
	
	@param pReturnSize (Filled by the method) A pointer to 
	the size in bytes of the return buffer. When 0, do not process 
	the return buffer, but continue enumerating until the method 
	returns false. Always less than maxSize.
	@return true as long as there is more data to process, false when 
	the end of the buffer is reached. 
	@see PSDataBufferDigest 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PSDataBufferEnumSELPROTO)
	( PSDataBuffer dataBuffer, ASInt32 maxSize, ASUns8 **pReturnBuffer, ASInt32 *pReturnSize  );
#define PSDataBufferEnum (*((PSDataBufferEnumSELPROTO)(gPubSecHFT[PSDataBufferEnumSEL])))

/**
	Computes the digest for a set of data. A handler will use 
	this call to make PubSec compute the digest for a data buffer 
	when signing or verifying signatures. This method calls 
	PSDataBufferEnum to get the bytes and computes an MD5 or 
	SHA-1 digest. 
	@param dataBuffer The buffer containing the data. 
	@param digestValue (Filled by the method) A pointer to 
	the digest value. The buffer must large enough for the requested 
	digest method:
	- For an MD5 digest, it must be at least 
	16 bytes.
	- For an SHA-1 digest, it must be at least 20 
	bytes. 
	@param digestMethod The method to use to compute the digest.
	@return true if successful, false otherwise. 
	@see PSDataBufferEnum 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PSDataBufferDigestSELPROTO)
	( PSDataBuffer dataBuffer, ASUns8* digestValue, DSDigestMethod digestMethod  );
#define PSDataBufferDigest (*((PSDataBufferDigestSELPROTO)(gPubSecHFT[PSDataBufferDigestSEL])))


/**
	Acquires the DSAP file and opens it, if it has not already 
	been acquired. 

	PubSec calls this method to access a file, so a handler 
	does not need to acquire a DSAP file unless it needs to 
	access it for other reasons. 
	@param bResolveProblems When true, if there are problems 
	trying to open the file, PubSec opens a UI that gives a 
	user the option to delete the corrupted file. 
	@param bCreate When true, if the file does not exist it 
	is created. Normally true.
	@return true if the file was acquired and opened, false otherwise. 
	
	@see DSAPFileRelease 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *DSAPFileAcquireSELPROTO)
	( const ASBool bResolveProblems, const ASBool bCreate  );
#define DSAPFileAcquire (*((DSAPFileAcquireSELPROTO)(gPubSecHFT[DSAPFileAcquireSEL])))

/**
	Closes the digital signature appearance (DSAP) file. 
	@see DSAPFileAcquire 
	@see DSAPFileSave
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DSAPFileReleaseSELPROTO)
	(  );
#define DSAPFileRelease (*((DSAPFileReleaseSELPROTO)(gPubSecHFT[DSAPFileReleaseSEL])))

/**
	Saves the DSAP file if it is dirty, leaving it open. 
	@see DSAPFileRelease
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DSAPFileSaveSELPROTO)
	(  );
#define DSAPFileSave (*((DSAPFileSaveSELPROTO)(gPubSecHFT[DSAPFileSaveSEL])))

/**
	Gets the number of configured signature appearance entries 
	in the DSAP file, 
	@return The number of configured AP entries. 
	@see DSAPFileCanDeleteNthEntry
*/
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *DSAPFileGetCountSELPROTO)
	(  );
#define DSAPFileGetCount (*((DSAPFileGetCountSELPROTO)(gPubSecHFT[DSAPFileGetCountSEL])))

/**
	Tests whether a signature appearance entry at a specified 
	index in the DSAP file can be edited or is read-only. 
	@param index The position of the entry to test. The first 
	entry is at index 0. A negative value gets the default entry.
	@return true if the entry is editable, false otherwise. 
	@see DSAPFileGetCount 
	@see DSAPFileRemoveNthEntry 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *DSAPFileCanDeleteNthEntrySELPROTO)
	( const ASInt32 index  );
#define DSAPFileCanDeleteNthEntry (*((DSAPFileCanDeleteNthEntrySELPROTO)(gPubSecHFT[DSAPFileCanDeleteNthEntrySEL])))

/**
	Gets a copy of the name of the specified signature appearance 
	entry in the DSAP file. Use this when building a list of 
	signatures for a user to choose from or edit. 
	@param index The position of the entry whose name to obtain. 
	The first entry is at index 0. A negative value gets the 
	default entry.
	@return A copy of the name as an ASText object. 
	@see DSAPFileGetCount 
	Closes the digital signature appearance (DSAP) file. 
	@see DSAPFileAcquire 
	@see DSAPFileSave
*/
typedef ACCBPROTO1 ASText (ACCBPROTO2 *DSAPFileGetNewNthNameSELPROTO)
	( const ASInt32 index  );
#define DSAPFileGetNewNthName (*((DSAPFileGetNewNthNameSELPROTO)(gPubSecHFT[DSAPFileGetNewNthNameSEL])))

/**
	Deletes the specified signature appearance entry from the 
	DSAP file. 
	@param index The position of the entry to remove. The 
	first entry is at index 0. A negative value gets the default 
	entry.
	@return true if successful, false otherwise. 
	@see DSAPFileCanDeleteNthEntry 
	@see DSAPFileGetCount 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *DSAPFileRemoveNthEntrySELPROTO)
	( const ASInt32 index );
#define DSAPFileRemoveNthEntry (*((DSAPFileRemoveNthEntrySELPROTO)(gPubSecHFT[DSAPFileRemoveNthEntrySEL])))

/**
	Opens the UI that allows the user to edit the specified 
	signature appearance entry of the DSAP file. 
	@param previewData Data with which to create a signature 
	preview in the edit dialog. 
	@param index The position of the entry to edit. The first 
	entry is at index 0. A negative value gets the default entry. 
	An index larger than the current number of entries creates 
	a new entry.
	@return true if successful (the changes to the entry were made and 
	saved), false otherwise. 
	@see DSAPFileCanDeleteNthEntry 
	@see DSAPFileCopyNthEntry 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *DSAPFileEditNthEntrySELPROTO)
	( const APPreview previewData, const ASInt32 index );
#define DSAPFileEditNthEntry (*((DSAPFileEditNthEntrySELPROTO)(gPubSecHFT[DSAPFileEditNthEntrySEL])))

/**
	Creates a copy of the specified entry in the default DSAP 
	file and appends the copy to the end of the list of signature 
	appearances in the file. 

	When you copy a default appearance entry, the copy is not 
	considered a default appearance entry. 
	@param index The position of the entry to copy. The first 
	entry is at index 0. A negative value gets the default entry.
	@return true if the copy was successful and the appearance file 
	was successfully edited and saved, false otherwise. 
	@see DSAPFileCanDeleteNthEntry 
	@see DSAPFileEditNthEntry 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *DSAPFileCopyNthEntrySELPROTO)
	( const ASInt32 index  );
#define DSAPFileCopyNthEntry (*((DSAPFileCopyNthEntrySELPROTO)(gPubSecHFT[DSAPFileCopyNthEntrySEL])))

/************************************************************************************
 * PubSec Acrobat Address Book (AAB) API
 ***********************************************************************************/

/**
	Finds the specified certificate in the Acrobat Address Book. 
	Use this method to distinguish a certificate that is not 
	found by AABGetCertTrust from one whose trust level is reported 
	as untrusted. 
	@param x509 The certificate identifier, as defined in 
	X.509 (RFC 3280). 
	@param size The size of the certificate pointed to by 
	x509.
	@return true if the certificate is found, false otherwise. 
	@see AABGetCertTrust 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AABIsCertPresentSELPROTO)
	( const ASUns8* x509, ASInt32 size  );
#define AABIsCertPresent (*((AABIsCertPresentSELPROTO)(gPubSecHFT[AABIsCertPresentSEL])))

/**
	Finds the specified certificate in the Acrobat Address Book 
	and returns the trust level. 
	@param inX509Cert The certificate identifier, as defined 
	in X.509 (RFC 3280). This is a generic 8-bit pointer to 
	the certificate data. 
	@param inX509CertSize The size in bytes of the X.509 certificate 
	pointed to by inX509Cert. 
	@param inCertChain An ASCab containing the certificate 
	chain for the certificate, with the trust level for each 
	certificate. It starts with inX509Cert's issuer at index 
	0 and continues in the issuing order. Can be NULL if the 
	chain is not available. 
	@param inHelperCerts An ASCab containing an unordered 
	sequence of certificates that can be used to build the certificate 
	chain. If inCertChain is NULL and inX509Cert is not self-signed, 
	PubSec attempts to build a chain of certificates using a 
	default mechanism. A certificate ASCab contains an entry 
	for each certificate,with a 0-based index followed by the 
	X509 certificate as ASN1-encoded binary data; for example: 
	{ ("0", cert1), ("1", cert2), ... }
	@return The trust value for the specified certificate, if found. 
	If no certificate is found, returns kPSSigTrustUntrusted. 
	To distinguish a certificate that is not found from one 
	whose trust level is reported as untrusted, use AABIsCertPresent. 
	
	@see AABGetTrustedCerts 
	@see AABIsCertPresent 
*/
typedef ACCBPROTO1 PSSigTrust (ACCBPROTO2 *AABGetCertTrustSELPROTO)
	( const ASUns8* inX509Cert, ASInt32 inX509CertSize, ASCab inCertChain, ASCab inHelperCerts  );
#define AABGetCertTrust (*((AABGetCertTrustSELPROTO)(gPubSecHFT[AABGetCertTrustSEL])))

/**
	Does a lookup in the Acrobat Address Book by certificate 
	subject name. Returns all certificates that match the name 
	along with trust information associated with them. 

	The returned ASCab contains: 

	- An entry for each certificate, with a 0-based index followed 
	by the X509 certificate as ASN1-encoded binary data 
	- An entry with a key Tn containing the associated trust 
	value of each certificate, where n corresponds to the certificate's 
	index key. 

	If a trust key is missing, the value should be assumed to 
	be untrusted. For example: 

	{ ("0", cert1), ("1", cert2), ("T1", kPSSigTrustAuthenticDocuments) 
	} 

	In this case, cert1 is untrusted, cert2 is trusted for authentic 
	documents. 
	@param inCertNameData The subject name of the certificates 
	to find. Specify a BER-encoded value of ASN.1 type Name 
	defined in X.509 (RFC 3280). 
	@param inCertNameSize The size of the certificate subject 
	name data. 
	@param outResults (Filled by the method) An ASCab containing 
	any certificates found by the lookup and their trust information.
	@return true if successful, false otherwise. 
	@see AABGetCertTrust 
	@see AABGetTrustedCerts 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AABFindCertsByNameSELPROTO)
	( const ASUns8* inCertNameData, ASInt32 inCertNameSize, ASCab outResults  );
#define AABFindCertsByName (*((AABFindCertsByNameSELPROTO)(gPubSecHFT[AABFindCertsByNameSEL])))

/**
	Finds the certificates with a specified level of trust in 
	the Acrobat Address Book. 
	@param inTrust The level of trust for which to find certificates. 
	A logical OR of PSSigTrust bit flags. 
	@param outResults (Filled by the method) An ASCab containing 
	the trusted certificates found in the AAB. A certificate 
	ASCab contains an entry for each certificate,with a 0- based 
	index followed by the X509 certificate as ASN1-encoded binary 
	data; for example: { ("0", cert1), ("1", cert2), ... }
	@return true if successful, false otherwise. 
	@see AABGetCertTrust 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *AABGetTrustedCertsSELPROTO)
	( PSSigTrust inTrust, ASCab outResults  );
#define AABGetTrustedCerts (*((AABGetTrustedCertsSELPROTO)(gPubSecHFT[AABGetTrustedCertsSEL])))

/**
	Finds the certificate chain for the specified certificate 
	in the Acrobat Address Book. 

	A certificate ASCab contains an entry for each certificate,with 
	a 0-based index followed by the X509 certificate as ASN1-encoded 
	binary data; for example: 

	{ ("0", cert1), ("1", cert2), ... } 
	@param inX509Cert The certificate identifier, as defined 
	in X.509 (RFC 3280). This is a generic 8-bit pointer to 
	the certificate data. 
	@param inX509CertSize The size in bytes of the X.509 certificate 
	pointed to by inX509Cert. 
	@param inTrustedCerts An ASCab containing the user's trusted 
	certificates. 
	@param inUntrustedCerts An ASCab containing additional 
	certificates needed to build the certificate chain. 
	@param outChain (Filled by the method) An ASCab containing 
	the certificate chain. The specified certificate itself 
	is at index 0, followed by the chain certificates in issuing 
	order.
	@return true if successful, false otherwise. 
	@see AABGetTrustedCerts 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AABGetCertChainSELPROTO)
	( const ASUns8* inX509Cert, ASInt32 inX509CertSize, ASCab inTrustedCerts, ASCab inUntrustedCerts, ASCab outChain  );
#define AABGetCertChain (*((AABGetCertChainSELPROTO)(gPubSecHFT[AABGetCertChainSEL])))

/************************************************************************************
 * Import/Export Facilities
 * Use to import/export certificates, requests for certificates, etc to FDF
 * files or other file types. Includes support to export to a file or
 * to email.  Uses wizard-like user interface
 ***********************************************************************************/

/**
	Exports certificates, requests for certificates, and so 
	on, to FDF files or other file types, using a UI wizard 
	interface. Includes support to export to a file or to email. 
	

	The specified type of data is exported to a file and optionally 
	emailed to a destination that is chosen using the wizard. 

	- If the data is saved to a file and is the user's own contact 
	information, it can be a PKCS#7 file (.p7c). 
	- If the data contains just one certificate and is saved 
	to a file, it can be a raw certificate file (.cer). 
	- Otherwise, it is always an FDF file. 

	Does not raise or throw. Displays an alert if unsuccessful. 
	
	@param params A structure containing the export parameters.
	@return true if successful, false otherwise. 
	@see PSImportDataExchange 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PSExportDataExchangeSELPROTO)
	( PSExportDataExchangeParams params  );
#define PSExportDataExchange (*((PSExportDataExchangeSELPROTO)(gPubSecHFT[PSExportDataExchangeSEL])))

/**
	Imports FDF data from a file, using a UI wizard interface. 
	The parameters structure specifies the type and location 
	of the data. This call is used, for example, by the Directory 
	configuration dialog to import directory settings from an 
	FDF file. 

	When you use this call (rather than opening the FDF file 
	directly) the PubSec FDF handling code is used, which provided 
	support for FDF signature verification. PubSec opens the 
	FDF file, then calls the handler's PSImportDataProc, using 
	the handler and engine specified in the parameters structure. 
	

	The operation fails if the data is not of the specified 
	type. Does not raise or throw. Displays an alert if unsuccessful. 
	
	@param params A structure containing the import parameters.
	@return true if successful, false otherwise. 
	@see PSExportDataExchange 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PSImportDataExchangeSELPROTO)
	( PSImportDataExchangeParams params  );
#define PSImportDataExchange (*((PSImportDataExchangeSELPROTO)(gPubSecHFT[PSImportDataExchangeSEL])))

/**
	Tests whether any certificate in a chain has been issued 
	under the Adobe Test Certificate Policy. 

	If this function returns true, PubSec handlers are recommended 
	to provide feedback to the user regarding the test nature 
	of the certificate, which may render it untrustworthy. 
	@param inCertChain The certificate chain to test, as an 
	ASCab array. The first certificate is the end entity, and 
	certificates should follow in the issuing order. For example: 
	{ ("0", cert1), ("1", cert2), ... } where certn is an X509 
	certificate as ASN1-encoded binary data.
	@return true if any certificate in the chain was issued under the 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PSCertIssuedUnderTestCPSELPROTO)
	( ASCab inCertChain  );
#define PSCertIssuedUnderTestCP (*((PSCertIssuedUnderTestCPSELPROTO)(gPubSecHFT[PSCertIssuedUnderTestCPSEL])))

/* HFTEndProto - do not alter/remove this line! */

/************************************************************************************
 * PubSecHFT globals
 ***********************************************************************************/

extern HFT gPubSecHFT;

#ifdef __cplusplus
}
#endif

#if PRAGMA_STRUCT_ALIGN
	#pragma options align=reset
#endif

#endif	// PUBSECHFT_H
 

