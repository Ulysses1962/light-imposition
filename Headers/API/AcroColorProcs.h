/*
** AcroColorProcs.h
**
** (c) Copyright 2002,2004, Adobe Systems, Inc.  All Rights Reserved.
** Exposure to ACE functions
** 
*/

#if !EXTERNAL_ACROCOLORPROCS_USER  /* External user of this header file, e.g. PISDK */
#if CAN_EDIT && !READER /* Restore X Macros -- was CAN_EDIT */
#define XNPROC(returnType, name, params) NPROC(returnType, name, params)
#define XPROC(returnType, name, params) PROC(returnType, name, params)
#define XSPROC(returnType, name, params, stubProc) SPROC(returnType, name, params, stubProc)
#else
#define XNPROC(returnType, name, params) NOPROC(name)
#define XPROC(returnType, name, params) NOPROC(name)
#define XSPROC(returnType, name, params, stubProc) NOPROC(name)
#endif /* CAN_EDIT && !READER */

#endif /* EXTERNAL_AcroColorPROCS_USER */

/**
	Gets the number of Color Management System/Color Management 
	Module (CMS/CMM) choices available for the AcroColor engine 
	(ACE). 
	@param count (Filled by the method) A pointer to the count 
	value.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACEngineInfo 
	@see ACSetEngine 
*/
XNPROC(AC_Error, ACEngineCount, (ASUns32 *count))

/**
	Gets information for a CMS/CMM in the AcroColor engine (ACE) 
	by index. The CMS and CMM identifiers specify an engine 
	to the AcroColor engine (ACE). Engine names should only 
	be used as the text for popup menus. It is better to store 
	the identifiers in settings files (rather than names), because 
	they are independent of localization. 
	@param index The 0-based index of the CMS/CMM. The highest 
	legal value is (AC_EngineCount)-1. 
	@param name (Filled by the method) Optional. If non-NULL, 
	returns the name of the CMS/CMM. 
	@param cmsID (Filled by the method) Returns the CMS identifier. 
	
	@param cmmID (Filled by the method) Returns the CMM identifier.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACEngineCount 
	@see ACSetEngine 

*/
XNPROC(AC_Error, ACEngineInfo, (ASUns32 index, AC_String *name, ASUns32 *cmsID, ASUns32 *cmmID))

/**
	Sets the AcroColor engine (ACE) for the system, changing 
	the global default CMS/CMM choice. 

	This method rebuilds all existing transforms using the new 
	engine. 

	If the user aborts the process, or if the ACE runs out of 
	resources during the rebuilding process, an error code is 
	returned and some existing transforms may still use the 
	previous engine choice. Everything will still work, since 
	multiple engines can be used at once. Call the method again 
	to restart the transform rebuilding process. 
	@param cmsID The Color Management System (CMS) identifier 
	for the new engine default, as returned by ACEngineInfo. 
	
	@param cmmID The Color Management Module (CMM) identifier 
	for the new engine default, as returned by ACEngineInfo.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACEngineCount 
	@see ACEngineInfo 
*/
XNPROC(AC_Error, ACSetEngine, (ASUns32 cmsID, ASUns32 cmmID))


/**
	Creates a list of device color profiles of a given type. 
	

	Builds a list of those profiles from the database that meet 
	the criterion of the specified selector. If the profile 
	database has never been built, it will be automatically 
	built, without a progress callback. Clients should call 
	ACUnReferenceProfileList when done with the profile list. 
	
	@param list (Filled by the method) A pointer to the new 
	profile list object. 
	@param selector The code for the type of device profile 
	to include in the list.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACProfileListCount 
	@see ACProfileListItemCode 
	@see ACProfileListItemDescription 
	@see ACUnReferenceProfileList 

*/
XNPROC(AC_Error, ACMakeProfileList,(AC_ProfileList *list, AC_SelectorCode selector ))

/**
	Gets the number of profiles in a device color profile list. 
	
	@param list The profile list. 
	@param count (Filled by the method) A pointer to the number 
	of profiles in the list.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACMakeProfileList 
	@see ACProfileListItemCode 
	@see ACProfileListItemDescription 

*/
XNPROC(AC_Error, ACProfileListCount, (AC_ProfileList list, ASUns32 *count) )

/**
	Returns the description string of a specified profile in 
	a list. The returned description string always contains 
	both ASCII and Unicode data, even if the profile itself 
	only contains an ASCII version. You can store only the Unicode 
	data in settings files if you wish; the ACProfileFromDescription 
	method finds the correct profile when passed the Unicode-only 
	string. 
	@param list The profile list. 
	@param index The index for the profile in the list. 
	@param description (Filled by the method) A pointer to 
	the profile descriptive string.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACMakeProfileList 
	@see ACMakeString 
	@see ACProfileFromDescription 
	@see ACProfileListCount 
	@see ACProfileListItemCode 

*/
XNPROC(AC_Error, ACProfileListItemDescription, (AC_ProfileList list, ASUns32 index, AC_String *description ))

/**
	Gets the profile code of a specified profile in a profile 
	list. If the specified profile does not have a code, returns 
	AC_Profile_Null. 

	While this routine is not absolutely required, since the 
	description string is always a unique reference, profile 
	codes have the advantage that they are localization-independent. 
	
	@param list The profile list. 
	@param index The index for the profile in the list. 
	@param code (Filled by the method) A pointer to the profile 
	code.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACMakeProfileList 
	@see ACProfileListCount 
	@see ACProfileListItemDescription 
*/
XNPROC(AC_Error, ACProfileListItemCode,(AC_ProfileList list, ASUns32 index, AC_ProfileCode *code ))

/**
	Decrements the reference count of a device color profile 
	list object. If this causes the object's reference count 
	to reach zero, the method deletes it. 
	@param list The profile list object.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACMakeProfileList 
*/
XNPROC(AC_Error, ACUnReferenceProfileList, (AC_ProfileList list))


/**
	Creates a list of preset AcroColor engine (ACE) settings, 
	of the specified type. Clients should call ACUnReferencePresetList 
	when done with the preset list. 

	A preset list is a list of predefined color settings that 
	specifies the source and destination working color profiles 
	to be used for color conversion. 
	@param list (Filled by the method) A pointer to the new 
	preset list object. 
	@param type The settings type, AC_SettingsType_Color or 
	AC_SettingsType_Proof.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACPresetListCount 
	@see ACPresetListItemFile 
	@see ACUnReferencePresetList 
*/
XNPROC(AC_Error, ACMakePresetList, (AC_PresetList *list, AC_SettingsType type))

/**
	Gets the number of predefined color settings, as listed 
	under Edit->Preferences->Color Management-> Settings in 
	the Acrobat UI. 
	@param list The preset list object. 
	@param count (Filled by the method)A pointer to the number 
	of settings in the list.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACMakePresetList 
	@see ACPresetListItemFile 
	@see ACUnReferencePresetList 
*/
XNPROC(AC_Error, ACPresetListCount, (AC_PresetList list, ASUns32 *count))

/**
	Gets the file specification for a preset settings item in 
	a preset list. 
	@param list The preset list object. 
	@param index The item index in the list. 
	@param file (Filled by the method) A pointer to the file 
	specification for the item.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACLoadSettings 
	@see ACMakePresetList 
	@see ACPresetFileToName 
	@see ACPresetListCount 
	@see ACUnReferencePresetList 

*/
XNPROC(AC_Error, ACPresetListItemFile, (AC_PresetList list, ASUns32 index, AC_FileSpec* file))

/**
	Translates a pre-set settings file specification to a name 
	ready to be displayed in menus (with directory paths and 
	file extensions removed). The client should call ACUnReferenceString 
	when done with the name. 
	<ul>
	<li> If the file contains an internal name tag, the returned 
	string is created from the internal name. 
	<li> If the file does not contain an internal name tag, the 
	returned string is built from the file name. In this case, 
	the method assumes that the file name is in the local script 
	code, and the ASCII data of the returned string is also 
	in the local script code. 
	</ul>
	@param file A pointer to the preset file specification. 
	@param name (Filled by the method) A pointer to the display 
	name string.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACPresetListItemFile 
	@see ACUnReferenceString 

*/
XNPROC(AC_Error, ACPresetFileToName, (const AC_FileSpec* file, AC_String* name))

/**
	Decrements the reference count of a preset list object. 
	If this causes the object's reference count to reach zero, 
	the method deletes it. 
	@param list The preset list object.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACMakePresetList 
*/
XNPROC(AC_Error, ACUnReferencePresetList, (AC_PresetList list)) 


/**
	Gets the current color profile for a given key from the 
	AcroColor engine (ACE) settings object. 
	<ul>
	<li> If the settings file contains a profile entry with the 
	specified key, returns that profile. 
	<li> If the settings file contains a special NULL entry with 
	the key, a NULL profile is returned. 
	<li> If the settings file contains a string with this key rather 
	than an embedded profile, returns an installed profile whose 
	description matches the string, if found. 
	<li> Otherwise, returns AC_Error_MissingKey. 
	</ul>
	The method does not check for known keys or legal key values. 
	It is up to the client to write only legal key values, and 
	to verify key values when reading. 
	@param settings The settings object from which the profile 
	is obtained. 
	@param key The value key constant. 
	@param profile (Filled by the method) A pointer to the 
	current color profile value of the given key.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACGetSettingsString 
	@see ACGetSettingsUnsigned32 
	@see ACLoadSettings 
	@see ACMakeSettings 
	@see ACUnReferenceProfile 
	@see ACUnReferenceSettings 
*/
XNPROC(AC_Error, ACGetSettingsProfile, (AC_Settings settings, AC_SettingsKey key, AC_Profile *profile))

/**
	Creates an AcroColor engine (ACE) settings object of a given 
	type, with no entries. When done with all operations, call 
	ACUnReferenceSettings to free the settings object. 
	@param settings (Filled by the method) A pointer to the 
	new settings object. 
	@param type The settings type, AC_SettingsType_Color or 
	AC_SettingsType_Proof.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACGetSettingsProfile 
	@see ACGetSettingsString 
	@see ACGetSettingsUnsigned32 
	@see ACLoadSettings 
	@see ACUnReferenceSettings 
*/
XNPROC(AC_Error, ACMakeSettings, (AC_Settings *settings, AC_SettingsType type))

/**
	Loads the AcroColor engine (ACE) settings from a file. 

	Reads the settings entries from the specified file and stores 
	them in the settings object, including entries with unknown 
	keys or data formats. As a general rule, the client should 
	keep the settings object around so these unknown keys are 
	preserved when the settings are saved out. The only time 
	the client should start with a fresh settings object is 
	when performing another settings load. 
	@param settings (Filled by the method) The settings object. 
	
	@param file A pointer to the file specification for the 
	file containing the settings.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACGetSettingsProfile 
	@see ACGetSettingsString 
	@see ACGetSettingsUnsigned32 
	@see ACMakeSettings 
	@see ACPresetListItemFile 
	@see ACUnReferenceSettings 
*/
XNPROC(AC_Error, ACLoadSettings, (AC_Settings settings, AC_FileSpec *file))

/**
	Gets the current string value for a given key from the AcroColor 
	engine (ACE) settings object. 
	<ul>
	<li> if the settings file contains a string entry with the 
	specified key, returns the entry. 
	<li> If the settings file contains a special NULL entry with 
	the key, returns a NULL string 
	<li> Otherwise, returns AC_Error_MissingKey. 
	</ul>
	@param settings The settings object from which the string 
	is obtained. 
	@param key The value key constant. 
	@param string (Filled by the method) A pointer to the 
	current string value of the given key.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACGetSettingsProfile 
	@see ACGetSettingsUnsigned32 
	@see ACLoadSettings 
	@see ACMakeSettings 
	@see ACUnReferenceSettings 
	@see ACUnReferenceString 

*/
XNPROC(AC_Error, ACGetSettingsString, (AC_Settings settings, AC_SettingsKey key, AC_String *string))

/**
	Gets the current numeric value for a given key from the 
	AcroColor engine (ACE) settings object. 
	<ul>
	<li> if the settings file contains an unsigned 32-bit numeric 
	entry with the specified key, returns the entry. 
	<li> Otherwise, returns AC_Error_MissingKey. 
	</ul>
	@param settings The settings object from which the value 
	is obtained. 
	@param key The value key constant. 
	@param value (Filled by the method) A pointer to the current 
	numeric value of the given key. 
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACGetSettingsProfile 
	@see ACGetSettingsString 
	@see ACLoadSettings 
	@see ACMakeSettings 
	@see ACUnReferenceSettings 
*/
XNPROC(AC_Error, ACGetSettingsUnsigned32, (AC_Settings settings, AC_SettingsKey key, ASUns32 *value))

/**
	Decrements the reference count of an AcroColor engine settings 
	object. If this causes the object's reference count to reach 
	zero, the method deletes it. 
	@param settings The settings object.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACMakeSettings 

*/
XNPROC(AC_Error, ACUnReferenceSettings, (AC_Settings settings))


/**
	Gets the description of a device profile. The returned description 
	string contains both ASCII and Unicode data, even if the 
	profile itself only contains ASCII data. 
	@param profile The device color profile. 
	@param description (Filled by the method) A pointer to 
	the descriptive string.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACMakeString 
	@see ACProfileColorSpace 
	@see ACProfileData 
	@see ACProfileListItemDescription 
	@see ACProfileFromDescription 
*/
XNPROC(AC_Error, ACProfileDescription, (AC_Profile profile, AC_String *description))

/**
	Finds a profile in the database given its description string. 
	The client should call ACUnReferenceProfile when done with 
	the profile. 
	<ul>
	<li> If the description string contains Unicode text, the Unicode 
	text is used to find the profile. 
	<li> If the description string contains only ASCII text, the 
	method tries to find a match. However, the AcroColor engine 
	requires only Unicode descriptions to be unique, so this 
	might return the wrong profile is in some rare cases. Use 
	ASCII-only description strings only when Unicode description 
	string are unavailable. 
	</ul>
	@param profile (Filled by the method) A pointer to the 
	device color profile object. 
	@param description The descriptive string.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACGetSettingsProfile 
	@see ACGetWorkingSpaceProfile 
	@see ACMakeBufferProfile 
	@see ACMakeCalGray 
	@see ACMakeCalLab 
	@see ACMakeCalRGB 
	@see ACMakeString 
	@see ACMonitorProfile 
	@see ACProfileFromCode 
	@see ACProfileListItemDescription 
	@see ACUnReferenceProfile 
*/
XNPROC(AC_Error, ACProfileFromDescription, (AC_Profile *profile, AC_String description))

/**
	Creates a device profile from a device profile type code. 
	The client should call ACUnReferenceProfile when done with 
	the profile. 
	@param profile (Filled by the method)A pointer to the 
	device color profile object. 
	@param code The profile type code.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACGetSettingsProfile 
	@see ACGetWorkingSpaceProfile 
	@see ACMakeBufferProfile 
	@see ACMakeCalGray 
	@see ACMakeCalLab 
	@see ACMakeCalRGB 
	@see ACMonitorProfile 
	@see ACProfileFromDescription 
	@see ACUnReferenceProfile 

*/
XNPROC(AC_Error, ACProfileFromCode, (AC_Profile *profile, AC_ProfileCode code))

/**
	Gets a device color profile for a specific monitor device. 
	The returned profile may be either RGB or grayscale. If 
	no profile is specified by the system, returns a default 
	platform profile (sRGB on Windows, Apple RGB on Macintosh). 
	The client should call ACUnReferenceProfile when done with 
	the returned profile. 
	@param profile (Filled by the method) A pointer to the 
	profile object. 
	@param monitorID A pointer to the platform-specific monitor 
	device identifier. On the Macintosh, this is the monitor's 
	AVID. On Windows, this is a NULL-terminated ASCII string 
	containing the monitor's device name (for example, "Display"). 
	
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACGetSettingsProfile 
	@see ACGetWorkingSpaceProfile 
	@see ACMakeBufferProfile 
	@see ACMakeCalGray 
	@see ACMakeCalLab 
	@see ACMakeCalRGB 
	@see ACProfileFromCode 
	@see ACProfileFromDescription 
	@see ACUnReferenceProfile 
*/
XNPROC(AC_Error, ACMonitorProfile, (AC_Profile *profile, void *monitorID))

/**
	Creates a device color profile object from a data buffer 
	containing the raw ICC profile data. Copies the data, so 
	the client can dispose of the source data. The client should 
	call ACUnReferenceProfile when done with the profile. 
	@param profile (Filled by the method) The device profile. 
	
	@param data The buffer containing the device profile data. 
	
	@param dataSize The size in bytes of the data buffer.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACGetSettingsProfile 
	@see ACGetWorkingSpaceProfile 
	@see ACMakeCalGray 
	@see ACMakeCalLab 
	@see ACMakeCalRGB 
	@see ACMonitorProfile 
	@see ACProfileFromCode 
	@see ACProfileFromDescription 
	@see ACUnReferenceProfile 

*/
XNPROC(AC_Error, ACMakeBufferProfile, (AC_Profile *profile, void *data, ASUns32 dataSize))


/**
	Creates a device color profile object from a calibrated 
	RGB color space, with the specified rendering intent and 
	descriptive string. The client should call ACUnReferenceProfile 
	when done with the profile. 
	@param profile (Filled by the method) A pointer to the 
	new device color profile. 
	@param spec The calibrated RGB color space specification. 
	
	@param intent The rendering intent. 
	@param description The description of the new profile. 
	If non-NULL, must contain ASCII data, and may contain a 
	Unicode data also. If NULL, a hard-coded default description 
	is used.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACGetSettingsProfile 
	@see ACGetWorkingSpaceProfile 
	@see ACMakeBufferProfile 
	@see ACMakeCalGray 
	@see ACMakeCalLab 
	@see ACMonitorProfile 
	@see ACProfileFromCode 
	@see ACProfileFromDescription 
	@see ACUnReferenceProfile 
*/
XNPROC(AC_Error, ACMakeCalRGB, (AC_Profile *profile, ACCalRGB *spec, AC_RenderIntent intent, AC_String description))

/**
	Creates a device color profile object from a calibrated 
	grayscale color space, with the specified rendering intent 
	and descriptive string. The client should call ACUnReferenceProfile 
	when done with the profile. 
	@param profile (Filled by the method) A pointer to the 
	new device color profile. 
	@param spec A pointer to the calibrated grayscale color 
	space specification. 
	@param intent The rendering intent. 
	@param description The description of the new profile. 
	If non-NULL, must contain ASCII data, and may contain a 
	Unicode data also. If NULL, a hard-coded default description 
	is used. 
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACGetSettingsProfile 
	@see ACGetWorkingSpaceProfile 
	@see ACMakeBufferProfile 
	@see ACMakeCalLab 
	@see ACMakeCalRGB 
	@see ACMonitorProfile 
	@see ACProfileFromCode 
	@see ACProfileFromDescription 
	@see ACUnReferenceProfile 
*/
XNPROC(AC_Error, ACMakeCalGray, (AC_Profile *profile, ACCalGray *spec, AC_RenderIntent intent, AC_String description))

/**
	Creates a device color profile object from a calibrated 
	LAB color space, with the specified rendering intent and 
	descriptive string. The client should call ACUnReferenceProfile 
	when done with the profile. 
	@param profile (Filled by the method) A pointer to the 
	new device color profile. 
	@param spec The calibrated LAB color space specification. 
	
	@param intent The rendering intent. 
	@param description The description of the new profile. 
	If non-NULL, must contain ASCII data, and may contain a 
	Unicode data also. If NULL, a hard-coded default description 
	is used. 
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACGetSettingsProfile 
	@see ACGetWorkingSpaceProfile 
	@see ACMakeBufferProfile 
	@see ACMakeCalGray 
	@see ACMakeCalRGB 
	@see ACMonitorProfile 
	@see ACProfileFromCode 
	@see ACProfileFromDescription 
	@see ACUnReferenceProfile 

*/
XNPROC(AC_Error, ACMakeCalLab, (AC_Profile *profile, ACCalLab *spec, AC_RenderIntent intent, AC_String description))

/**
	Gets the color space for a device profile. 
	@param profile The device color profile. 
	@param space (Filled by the method) A pointer to the color 
	space.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACProfileData 
	@see ACProfileDescription 
*/
XNPROC(AC_Error, ACProfileColorSpace, (AC_Profile profile, AC_ColorSpace *space))

/**
	Gets the size, in bytes, of the raw ICC profile data in 
	a device profile. 
	@param profile The device color profile object. 
	@param size (Filled by the method) A pointer to the profile 
	data size.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACProfileData 
*/
XNPROC(AC_Error, ACProfileSize, (AC_Profile profile, ASUns32 *size))

/**
	Gets the data for a device profile. Copies the raw ICC profile 
	data into a supplied buffer. 
	@param profile The device color profile. 
	@param data (Filled by the method) A pointer to the color 
	profile data.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACProfileColorSpace 
	@see ACProfileDescription 
	@see ACProfileSize 

*/
XNPROC(AC_Error, ACProfileData, (AC_Profile profile, void *data))

/**
	Decrements the reference count of a device color profile 
	object. If this causes the object's reference count to reach 
	zero, the method deletes it. 
	@param profile The profile object.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACGetSettingsProfile 
	@see ACGetWorkingSpaceProfile 
	@see ACMakeBufferProfile 
	@see ACMakeCalGray 
	@see ACMakeCalLab 
	@see ACMakeCalRGB 
	@see ACMonitorProfile 
	@see ACProfileFromCode 
	@see ACProfileFromDescription 
	@see ACProfileFromDescription 

*/
XNPROC(AC_Error, ACUnReferenceProfile, (AC_Profile profile))


/**
	Creates a color transformation object. 

	The client can dispose of the source and destination profiles 
	as soon as the transform is created. If the source profile 
	is a device link or abstract profile, then the destination 
	profile must be NULL; otherwise it must be non-NULL. 
	@param transform (Filled by the method) A pointer to the 
	new color transformation object. 
	@param srcProfile The source profile from which to transform 
	color data. 
	@param dstProfile The destination profile to which to 
	tranfrom color data. 
	@param intent The rendering intent for colors outside 
	the gamut of the destination profile.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACApplyTransform 
	@see ACUnReferenceTransform 

*/
XNPROC(AC_Error, ACMakeColorTransform, (AC_Transform *transform, AC_Profile srcProfile, AC_Profile dstProfile, AC_RenderIntent intent /* options */))

/**
	Applies a color conversion or gamut test transformation. 
	Processes the number of colors specified by count, using 
	the memory formats for the source and destination data specified 
	in srcPacking and dstPacking. The source data and destination 
	data can point to the same block of memory if the source 
	and destination packing formats use the same number of bits 
	per color. 
	@param transform The color conversion or tranformation 
	to apply. 
	@param srcData The source data to tranform. 
	@param dstData The destination for the transformed data. 
	
	@param count The number of colors to transform. 
	@param srcPacking The packing type used in the source 
	data. 
	@param dstPacking The packing type to use in the destination 
	data.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACMakeColorTransform 

*/
XNPROC(AC_Error, ACApplyTransform, (AC_Transform transform, const void* srcData, void*	dstData, ASUns32 count, AC_PackingCode srcPacking, AC_PackingCode	dstPacking /* options */))

/**
	Decrements the reference count of a color transformation 
	object. If this causes the object's reference count to reach 
	zero, the method deletes it. 
	@param transform The tranform object.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACMakeColorTransform 
*/
XNPROC(AC_Error, ACUnReferenceTransform, (AC_Transform transform))


/**
	Creates an AcroColor string from a NULL-terminated ASCII 
	string or a NULL-terminated Unicode string, or both. If 
	both ASCII and Unicode data are specified, the AC_String 
	object keeps track of both in parallel, returning the ASCII 
	data when asked for ASCII, and the Unicode data when asked 
	for Unicode. 

	These dual-encoded strings are useful as description strings 
	for ICC profiles, which can store both ASCII and Unicode 
	data in their description tags. The ICC profile standard 
	requires that ASCII version of the description string be 
	limited to 7-bit ASCII characters. The AcroColor engine 
	requires only the Unicode descriptions to be unique among 
	profile descriptions. 
	@param string (Filled by the method) A pointer to the 
	new string object. 
	@param ascii The ASCII data. Should be limited to 7-bit 
	ASCII characters for use in profile descriptions. 
	@param unicode The Unicode data. All Unicode characters 
	are two-byte characters, in native byte order, including 
	the trailing NULL.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACProfileFromDescription 
	@see ACProfileListItemDescription 
	@see ACStringASCII 
	@see ACStringLocalized 
	@see ACStringUnicode 
	@see ACUnReferenceString 

*/
XNPROC(AC_Error, ACMakeString, (AC_String* string, const char* ascii,  const ASUns16* unicode))

/**
	Copies the ASCII version of a string into a supplied buffer. 
	Either the buffer or the count can be NULL. 

	The ICC profile standard requires that ASCII version of 
	the profile description string be limited to 7-bit ASCII 
	characters. 

	Depending on the API, OS, file contents, and so on, the 
	method can return strings in the local script code (8-bit 
	single byte or 8-bit encoded multi-byte). To be safe, clients 
	should always assume that the ASCII data is in the local 
	script code (of which the 7-bit ASCII characters are a subset). 
	
	@param string The AcroColor string containing ASCII data. 
	If the string does not contain an ASCII version. the method 
	returns AC_Error_NoASCII. 
	@param buffer (Filled by the method) A buffer to contain 
	a copy of the ASCII data. 
	@param count (Filled by the method) A pointer to the size 
	of buffer in bytes, including the trailing NULL character. 
	
	@param maxCount The maximum size of buffer in bytes. If 
	the length of the string is longer than maxCount, the method 
	copies a truncated string to the buffer and returns AC_Error_StringOverflow.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACMakeString 
	@see ACStringLocalized 
	@see ACStringUnicode 
	@see ACUnReferenceString 

*/
XNPROC(AC_Error, ACStringASCII, (AC_String string, char* buffer, ASUns32* count, ASUns32 maxCount))

/**
	Copies the localized Unicode version of a string into a 
	supplied buffer. Either the buffer or the count can be NULL. 
	

	The settings file format, and ICC profiles later than version 
	2, can contain text in multiple languages or countries. 
	When the AcroColor engine (ACE) create strings from these 
	files or profiles, it uses the current language and country 
	codes to create strings with a third fork--a localized Unicode 
	version. These localized versions are intended for user 
	display only, and should not be stored in preferences files 
	or action scripts, since they vary from country to country 
	and are not portable. 
	@param string The AcroColor string containing localized 
	Unicode data. If the string does not contain a localized 
	Unicode version. the method returns AC_Error_NoLocalized. 
	
	@param buffer (Filled by the method) A buffer to contain 
	a copy of the localized Unicode data. 
	@param count (Filled by the method) A pointer to the size 
	of buffer in bytes, including the trailing NULL character. 
	
	@param maxCount The maximum size of buffer in bytes. If 
	the length of the string is longer than maxCount, the method 
	copies a truncated string to the buffer and returns AC_Error_StringOverflow.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACMakeString 
	@see ACStringASCII 
	@see ACStringUnicode 
	@see ACUnReferenceString 
*/
XNPROC(AC_Error, ACStringLocalized, (AC_String string, ASUns16* buffer,ASUns32* count, ASUns32 maxCount))
	  

/**
	Copies the Unicode version of a string into a supplied buffer. 
	Either the buffer or the count can be NULL. 
	@param string The AcroColor string containing localized 
	Unicode data. If the string does not contain a Unicode version. 
	the method returns AC_Error_NoUnicode. 
	@param buffer (Filled by the method) A buffer to contain 
	a copy of the Unicode data. 
	@param count (Filled by the method) A pointer to the size 
	of buffer in bytes, including the trailing NULL character. 
	
	@param maxCount The maximum size of buffer in bytes. If 
	the length of the string is longer than maxCount, the method 
	copies a truncated string to the buffer and returns AC_Error_StringOverflow.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACMakeString 
	@see ACStringASCII 
	@see ACStringLocalized 
	@see ACUnReferenceString 

*/
XNPROC(AC_Error, ACStringUnicode, (AC_String string, ASUns16* buffer, ASUns32* count, ASUns32 maxCount))

/**
	Decrements the reference count of a string object. If this 
	causes the object's reference count to reach zero, the method 
	deletes it. 
	@param string The string object.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACMakeString 
*/
XNPROC(AC_Error, ACUnReferenceString, (AC_String string))


/**
	Gets a working color profile in a specified color space. 
	
	@param space The type of color space of the profile to 
	obtain. 
	@param workingProfile (Filled by the method) A pointer 
	to the working profile. When done with this object, dereference 
	it using ACUnReferenceProfile.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACProfilesMatch 
	@see ACUnReferenceProfile 
*/
XNPROC(AC_Error, ACGetWorkingSpaceProfile, (ACWorkingSpace space, AC_Profile *workingProfile))

/**
	Compares the working device profile with the document device 
	profile to determine if they are the same. This comparison 
	ignores rendering intents, and is "fuzzy," allowing very 
	close, but not exactly the same, profiles to match. Equivalent 
	profiles always match, but some non-equivalent profiles 
	may also match. 
	@param workingProfile The working device color profile. 
	
	@param documentProfile The document's device color profile. 
	
	@param match (Filled by the method) A pointer to the result: 
	true if the profiles match, false otherwise.
	@return 0 if successful, an non-zero error code otherwise. 
	@see ACGetSettingsProfile 
	@see ACGetWorkingSpaceProfile 
	@see ACUnReferenceProfile 

*/
XNPROC(AC_Error, ACProfilesMatch, (AC_Profile workingProfile, AC_Profile documentProfile, ASBool *match))

#undef XSPROC
#undef XNPROC
#undef XPROC
