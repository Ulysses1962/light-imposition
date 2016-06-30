/*************************************************************************
 * DirectoryHFT.h
 *
 * Copyright (c) 2000-2002 Adobe Systems Inc. All Rights Reserved.
 *
 * NOTICE: All information contained herein is, and remains the
 * property of Adobe Systems Incorporated and its suppliers, if any.
 * The intellectual and technical concepts contained herein are
 * proprietary to Adobe Systems Incorporated and its suppliers and may
 * be covered by U.S. and Foreign Patents, patents in process, and are
 * protected by trade secret or copyright law.  Dissemination of this
 * information or reproduction of this material is strictly forbidden
 * unless prior written permission is obtained from Adobe Systems Inc.
 *
 * Description: Interface to Acrobat Directory Services
 *
 * 06-20-2002 sheretov -- Created
 ************************************************************************/

#ifndef DIR_SERVICES_HFT_H
#define DIR_SERVICES_HFT_H

#include "CoreExpT.h"

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
 * Error handling
 ***********************************************************************************/

/** @name Error handling */
 /*@{*/
/** */
typedef ASInt32 SecRetCode;
/** */
#define kSecTrue       1
/** */
#define kSecFalse      0
/** */
#define kSecOk         1
/** */
#define kSecError	  -1
/*@}*/

/** If a function returns -1 or less, you can call a GetLastError appropriate to 
   the context and retrieve information about the last error.  That call may 
   return NULL which basically means "Unknown Error", Note that in the presence 
   of threads, the error information may become inaccurate. */
typedef struct _t_SecErrorInfo
{
	/** size of this structure */
	ASSize_t size;       
	/** Description of error */
	ASText   text;       
	/** Adobe use only. Must be set to NULL by external developers */
	void*    reserved;   
} 
SecErrorInfoRec, *SecErrorInfo;

/** */
typedef struct _t_DirConnection* DirConnection;
/************************************************************************************
 * Rules for out-parameters
 ***********************************************************************************/

/* Consider the following function prototypes:

   void foo(ASBool* outResult);
   void bar(void** outResult);  
   void baz(ASCab outResult);

   - if(outResult==0) when the call is made, the caller is not interested in this 
     out parameter. The callee should not fail just because outResult is 0.

   - if(outResult!=0 && *outResult==0) when the call is made, the callee is 
     expected to allocate the result and the caller is responsible for its 
     deallocation.  One exception to this rule is when the callee wishes to 
     return an "empty value" (such as an empty ASText or ASCab), it will leave 
     *outResult==0.

   - if(outResult!=0 && *outResult!=0) when the call is made, the callee will 
     deallocate the *outResult if needed and replace it with a new value.

   - Raw memory and primitive structures (the ones that don't have any members that
     require deallocation) shall be deallocated with ASFree/miFree.  ASCabs ASTexts 
     and other known types shall be deallocated with their special routines 
     (ASTextDestroy(), ASCabDestroy(), etc.)

   - If a call fails, *outResult remains unchanged.
*/

/************************************************************************************
 * Directory Information format
 ***********************************************************************************/

/** A directory information structure contains configuration settings used to 
   establish a connection to a directory. Common top-level properties are 
   defined below. Note that the prefix 'DirStdEntry_' is reserved for standard 
   entries and should not be used for entries specific to a particular directory.
   Optionally this could contain other configuration information specific to 
   the directory. 
*/
typedef ASCab DirectoryInfo;

/* Name of this directory for display purposes.
   An example of this would be "Adobe Employees"

   TEXT, REQUIRED. */
#define PROP_DirectoryInfo_Name "dirStdEntryName"

/* ID. Unique atom that identifies the directory
   An example of this would be Adobe.PPKMS.LDAP.dir0
   ATOM, REQUIRED. */
#define PROP_DirectoryInfo_ID "dirStdEntryID"

/* Language independent name of the directory handler to be used 
   when connecting to this directory. This is required when there
   are multiple direcrtory handlers within a DSP. An example of this
   would be Adobe.PPKMS.ADSI

   ATOM, OPTIONAL. */
#define PROP_DirectoryInfo_DirHandlerID "dirStdEntryPrefDirHandlerID"

/* Language independent name of the type of directory this represents. 
   This is required when you want to import/export entries. An example
   of this would be LDAP, ADSI.

   ATOM, OPTIONAL. */
#define PROP_DirectoryInfo_DirType "dirStdEntryDirType"

/* An integer that represent the version of the directory.

   Integer, OPTIONAL. */
#define PROP_DirectoryInfo_Version "dirStdEntryVersion"

/************************************************************************************
 * Directory Attributes
 ***********************************************************************************/

/* These are attribute names used in the "standard" connection functions 
   (DirSetStandardOutputOptions(), DirStandardSearch(), etc.)  These attribute names
   are used both as keys/property names (in representation of the search results) and
   as values (in representation of output settings). The attributes are stored as 
   ASText so as to enable easy conversion from JavaScript.
*/
#define ATTR_DirFirstName    "firstName"
#define ATTR_DirLastName     "lastName"
#define ATTR_DirFullName     "fullName"
#define ATTR_DirEmail        "email"
#define ATTR_DirCertificates "certificates"
#define ATTR_DirPrefEncryptionCert "defaultEncryptCert"

/************************************************************************************
 * Connection Interface.
 ***********************************************************************************/

/** Group names are represented as text in an ASCab.  Example:
   { ("0", "friends"), ("1", "family"), ... } 
*/
typedef ASCab DirGroupList;
/**  Retrieves the list of groups that this connection supports.

	@return kSecOk is successful, else kSecError
*/
typedef ACCBPROTO1 SecRetCode (ACCBPROTO2 *DirConnection_getGroups)
	( DirConnection inConnection, DirGroupList outGroupList );

/*-----------------------------------------------------------------------------------
   The output options of a connection determine which directory attributes will be
   returned when a search is performed.  There are two calls for setting the output
   options, and they differ in the way the attributes are interpreted.  The default
   behavior for a new connection if output options have not been explicitly set, is
   as if DirSetStandardOutputOptions call with all "standard" attributes has been 
   made.
*/

/** A directory attribute collection is used to set output options of a directory 
   connection.  The collection is represented in an ASCab as:
      { ("0", "nameOfAttribute1"), ("1", "nameOfAttribute2"), ... } 
*/
typedef ASCab DirAttributes;

/** Special case:  ATTR_Certificates attribute is intended to encompass all 
    certificate attributes a directory might have.  If ATTR_Certificates is requested
    it may have to be translated into several attributes depending on the directory.
    @param inConnection is the connection for which the output options are set.
    @param inRequestedAttrs contains only "standard" attribute names defined as ATTR_ 
    constants above.  These names most likely will need to be "translated" to ones 
    understood by specific directory types.  For example, in case of a generic LDAP 
    directory, ATTR_LastName may be interpreted as "sn".
	@param outUnsupportedAttrs is used to pass back the the names of attributes (from 
    inRequestedAttrs) that are not supported by the connection.
	@return either { kOK, kSecError }
*/
typedef ACCBPROTO1 SecRetCode (ACCBPROTO2 *DirConnection_setStandardOutputOptions) 
	( DirConnection inConnection, 
	  DirAttributes inRequestedAttrs, DirAttributes outUnsupportedAttrs);

/** TBD
	@param inConnection is the connection for which the output options are set.
	@param inRequestedAttrs contains attribute names that are not "translated".  Any 
    attribute names can be used as long as they are supported by the target directory.
	@param outUnsupportedAttrs is used to pass back the the names of attributes (from 
    inRequestedAttrs) that are not supported by the connection.
	@return either { kSecOk, kSecError }
*/
typedef ACCBPROTO1 SecRetCode (ACCBPROTO2 *DirConnection_setCustomOutputOptions) 
	( DirConnection inConnection, 
	  DirAttributes inRequestedAttrs, DirAttributes outUnsupportedAttrs);

/*-----------------------------------------------------------------------------------
   Perform a search using a connection.  There are three different search functions,
   and although all three are optional, at least one should be implemented for a 
   connection to be usable.
*/

/** Directory search criteria are represented as a set of key/value pairs where the 
   keys are attribute names and values are search strings applied to those 
   attributes.  Logical "AND" is implied when multiple search criteria are present.
   Unrecognized search criteria are ignored.
   
   Example: { ("firstName", "Jonh"), ("lastName", "S*") } searches for people with 
            first name "John" whose last name starts with letter 'S'.

   Special Case: For "standard" search, the value of ATTR_Certificates search 
   criterion shall be a list of certificates.  In order for a directory entry to 
   match the criterion, it must have all those certificates.
*/
typedef ASCab DirSearchCriteria;

/** Search and enumeration results are represented as a two-dimensional ASCab where 
   each "row" is itself an ASCab that contains attributes defined by SetOutputOptions 
   calls.

   Example: { ("0", { ("fullName", "John Doe"), ("email", "johndoe@yahoo.com") } ),
              ("1", { ("fullName", "Jane Doe"), ("email", "janedoe@hotmail.com") } ) }

   Special Case: If standard output options are used and ATTR_Certificates is 
   included, the ATTR_Certificates value in the "row" ASCabs will be another ASCab,
   which stores all certificates associated with that directory entry.  
   
   Example: { ("0", { ("fullName", John Doe"), 
                      ("certificates", { ("0", encryptionCertBinaryValue),
                                         ("1", signingCertBinaryValue) 
                                       }
                    }
            }
*/
typedef ASCab DirResults;

/** TBD
	@param inConnection is the connection that is used to perform the search.
	@param inSearchCriteria are search criteria to be used.  Only "standard" attribute
    names defined as ATTR_ constants above are present.  These names most likely 
    will need to be "translated" to ones understood by specific directory types.  
    For example, in case of a generic LDAP directory, ATTR_LastName may be 
    interpreted as "sn".
	@param inGroup specifies the group, which should be searched.  NULL means that all 
    groups should be searched.
	@param outResults points to the results of the search.
	@return either { kSecOk, kSecError }
*/
typedef ACCBPROTO1 SecRetCode (ACCBPROTO2 *DirConnection_standardSearch)
	( DirConnection inConnection, 
	  DirSearchCriteria inSearchCriteria, 
	  ASText inGroup, 
	  DirResults outResults );

/* TBD
	@param inConnection is the connection that is used to perform the search.
	@param inSearchCriteria are search criteria to be used.  Attribute names are not 
    "translated".  Any attribute names can be used as long as they are supported 
    by the target directory.
	@param inGroup specifies the group, which should be searched.  NULL means that all 
    groups should be searched.
	@param outResults points to the results of the search.
	@return either { kSecOk, kSecError }
*/
typedef ACCBPROTO1 SecRetCode (ACCBPROTO2 *DirConnection_customSearch)
	( DirConnection inConnection, 
	  DirSearchCriteria inSearchCriteria, 
	  ASText inGroup, 
	  DirResults outResults );

/** Pops up custom UI that allows the user to set search criteria and execute the 
	search.
	@param inConnection is the connection that is used to perform the search.
	@param inGroup specifies the group, which should be searched.  NULL means that all 
	groups should be searched.
	@param outResults points to the results of the search.
	@return either { kSecOk, kSecError }
*/
typedef ACCBPROTO1 SecRetCode (ACCBPROTO2 *DirConnection_customUISearch)
	( DirConnection inConnection, 
	  ASText inGroup, 
	  DirResults outResults );

/** Retrieve all entries in the specified groups.  If that would take too long, return
	kDirDirectoryTooLargeToList.  This may used in the UI to immediately display 
	contents of a small directory or group, so responsiveness is important.
	@param inConnection is the connection that use to list directory entries
	@param inGroup specifies the group, which should be listed.  NULL means that all 
	groups should be listed.
	@param outResults points to the results of the enumeration.
	@return either { kSecOk, kDirDirectoryTooLargeToList, kSecError }
*/

#define kDirDirectoryTooLargeToList 2

/** TBD */
typedef ACCBPROTO1 SecRetCode (ACCBPROTO2 *DirConnection_list)
	( DirConnection inConnection, 
	  ASText inGroup, 
	  DirResults outResults );

/** Close the specified directory connection.
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *DirConnection_close) 
	( DirConnection inConnection );

/** Retrieve information about the directory associated with the connection.
	@param inConnection is a directory connection object.
	@param outDirInfo is a Cab file containing the Directory Info
	@return either { kSecOk, kSecError }
*/
typedef ACCBPROTO1 SecRetCode (ACCBPROTO2 *DirConnection_getDirInfo) 
	( DirConnection inConnection, DirectoryInfo outDirInfo );

/** Retrieve information about the last error that ocurred in the specified 
	connection.
*/
typedef ACCBPROTO1 SecErrorInfo (ACCBPROTO2 *DirConnection_getLastError) 
	( DirConnection inConnection );

/** Directory Connection object structure 
*/
typedef struct _t_DirConnection 
{
	/** size of this data structure */
	ASSize_t size; 
	/** Data to be passed in. */
	void* clientData;

	/** */
	DirConnection_close                      close;
	/** */
	DirConnection_getLastError               getLastError;
	/** */
	DirConnection_getDirInfo                 getDirInfo;
	/** */
	DirConnection_getGroups                  getGroups;

	/** */
	DirConnection_setStandardOutputOptions   setStandardOutputOptions;
	/** */
	DirConnection_setCustomOutputOptions     setCustomOutputOptions;

	/* Although all four of these methods are optional, at least one must be present
	   for a connection to be usable */
	/** */
	DirConnection_standardSearch             standardSearch;
	/** */
	DirConnection_customSearch               customSearch;
	/** */
	DirConnection_customUISearch             customUISearch;
	/** */
	DirConnection_list                       list;

}
DirConnectionRec, *DirConnectionHandler;

#ifdef __cplusplus
}
#endif

#if PRAGMA_STRUCT_ALIGN
	#pragma options align=reset
#endif

#endif /* DIR_SERVICES_HFT_H */
