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

 AFTTS_Sel.h

 - Selectors for all AcroTTS HFT functions.

*********************************************************************/

#ifndef _H_AFTTS_SEL
#define _H_AFTTS_SEL

/** Notification callback for TTS routines.
	@see AFTTSSetNotify
*/
typedef ACCB1 void (ACCB2 AFTTSNotifyProc)();

enum
{	AcroTTSFirst_SEL  = 0,
	AFTTSEnd_SEL,
	AFTTSQueueTextData_SEL,
	AFTTSTalk_SEL,
	AFTTSQSound_SEL, 
	AFTTSQTone_SEL, 
	AFTTSQSilence_SEL, 
	AFTTSResume_SEL, 
	AFTTSPause_SEL, 
	AFTTSStop_SEL,
	AFTTSReset_SEL, 
	AFTTSGetVoiceName_SEL, 
	AFTTSIsAvailable_SEL, 
	AFTTSGetSpeaker_SEL, 
	AFTTSGetPunctuations_SEL, 
	AFTTSGetSpeechRate_SEL, 
	AFTTSGetCharacterScale_SEL, 
	AFTTSGetVolume_SEL, 
	AFTTSGetPitch_SEL, 
	AFTTSGetNumberOfVoices_SEL, 
	AFTTSSetSpeaker_SEL, 
	AFTTSSetPunctuations_SEL, 
	AFTTSSetSpeechRate_SEL, 
	AFTTSSetCharacterScale_SEL, 
	AFTTSSetVolume_SEL,
	AFTTSSetPitch_SEL,
	AFTTSSetNotify_SEL,
	AcroTTSLast_SEL  };

#define AcroTTSNum_SEL  AcroTTSLast_SEL - 1
#define AcroTTSHFT_NAME "TTS"
#define AcroTTSHFT_LATEST_VERSION (0x00010012)

/** Finalizes a speech section shuting down the speech engines used by the TTS object.
AFTTSIsAvailable should return \<false\> after that. Its use by a plug-in should not be 
needed anytime, as AcroForms already handles TTS termination. Initialization of TTS 
is also handled automatically. By calling any function below, AFTTSIsAvailable should 
then return true, in case a SAPI engine is installed on the system. */
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AFTTSEnd_SELPROTO)();
/**
	Puts text into the queue to be performed by AFTTSTalk. 
	@param textdata The text that will be put into the queue. 
	
	@param UseDefaultSpeaker Whether to use the default speaker.
	@return true if the speech engine is available, false otherwise. 
	
	@see AFTTSQSilence 
	@see AFTTSQSound 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AFTTSQueueTextData_SELPROTO)(const char* textdata, ASBool UseDefaultSpeaker);
/**
	Sends whatever is in the queue to be spoken by the SAPI 
	TTS engine. If the text output had been paused, resumes 
	playback of the queued text. 
	@return true if the speech engine is available, false otherwise. 
	
	@see AFTTSQueueTextData
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AFTTSTalk_SELPROTO)();
/**
	Puts a sound into the queue. The sound can then be performed 
	by AFTTSTalk. 
	@param soundName The sound name. Names are: ActionCopy, 
	ActionCut, ActionDelete, ActionPaste, DocActive, DocClose, 
	DocOpen, DocPrint, DocSave, KeyEnd, KeyHome, and PageTurn. 
	This list can be augmented by adding sound files to the 
	SoundCues folder in Acrobat's installation, in 22kHz 16-bit 
	PCM .wav format.
	@return true if the speech engine is available, false otherwise. 
	@see AFTTSQSilence 
	@see AFTTSQueueTextData 
	Puts text into the queue to be performed by AFTTSTalk. 
	@return true if the speech engine is available, false otherwise. 
	@see AFTTSQSilence 
	@see AFTTSQSound 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AFTTSQSound_SELPROTO)(const char* soundName);
/** Not implemented in 4.05. */
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AFTTSQTone_SELPROTO)(ASUns32 frequency, ASUns32 duration);
/**
	Queues a period of silence into the text. 
	@param duration The amount of silence in milliseconds.
	@return true if the speech engine is available, false otherwise. 
	
	@see AFTTSQSound 
	@see AFTTSQueueTextData 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AFTTSQSilence_SELPROTO)(ASUns32 duration);
/**
	Resumes playback of text on a paused TTS object. 
	@return true if the speech engine is available, false otherwise. 
	
	@see AFTTSPause
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AFTTSResume_SELPROTO)();
/**
	Immediately pauses TTS output on a TTS object. Playback 
	of the remaining queued text can be resumed via AFTTSResume. 
	
	@return true if the speech engine is available, false otherwise. 
	@see AFTTSResume
	Queues a period of silence into the text. 
	@return true if the speech engine is available, false otherwise. 
	@see AFTTSQSound 
	@see AFTTSQueueTextData 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AFTTSPause_SELPROTO)();
/**
	Stops playback of currently queued text, and flushes the 
	queue. Playback of queued text cannot be resumed. 
	@return true if the speech engine is available, false otherwise. 
	
	@see AFTTSReset
	Sends whatever is in the queue to be spoken by the SAPI 
	TTS engine. If the text output had been paused, resumes 
	playback of the queued text. 
	@return true if the speech engine is available, false otherwise. 
	
	@see AFTTSQueueTextData
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AFTTSStop_SELPROTO)();
/**
	Stops playback of the currently queued text, and flushes 
	the queue. Resets all the properties of the TTS object to 
	their default values. 

	NOTE: Text playback cannot be resumed via AFTTSResume. 
	@return true if the speech engine is available, false otherwise. 
	
	@see AFTTSStop
	Resumes playback of text on a paused TTS object. 
	@return true if the speech engine is available, false otherwise. 
	
	@see AFTTSPause
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AFTTSReset_SELPROTO)();
/**
	Gets the voice name of any of the available speakers in 
	the installed TTS engine. 
	@param index The index of the speaker.
	@return The name of the voice. 
	@see AFTTSSetSpeaker 
*/
typedef ACCBPROTO1 char* (ACCBPROTO2 *AFTTSGetVoiceName_SELPROTO)(ASInt32 index);
/**
	Determines whether the TTS object is available and the Text-to-Speech 
	engine can be used. 

	NOTE: Calling any method in this API causes the Forms plug-in 
	to initialize TTS automatically, if an SAPI engine is installed 
	on the system. 
	@return true if the Text-to-Speech engine can be used, false otherwise. 
	
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AFTTSIsAvailable_SELPROTO)();
/**
	Gets the name of the current speaker. 
	@return The name of the current speaker. 
	@see AFTTSSetSpeaker
*/
typedef ACCBPROTO1 char* (ACCBPROTO2 *AFTTSGetSpeaker_SELPROTO)();
/** Not implemented in 4.05. */
typedef ACCBPROTO1 char* (ACCBPROTO2 *AFTTSGetPunctuations_SELPROTO)();
/**
	Gets the speed at which text is being spoken by the TTS 
	engine. 
	@return The speed, in number of words per minute, at which text 
	is being spoken. 
	@see AFTTSSetSpeechRate
	Gets the voice name of any of the available speakers in 
	the installed TTS engine. 
	@return The name of the voice. 
	@see AFTTSSetSpeaker 
*/
typedef ACCBPROTO1 ASUns32 (ACCBPROTO2 *AFTTSGetSpeechRate_SELPROTO)();
/** Not implemented in 4.05. */
typedef ACCBPROTO1 ASUns32 (ACCBPROTO2 *AFTTSGetCharacterScale_SELPROTO)();
/**
	Gets the volume for the speaker. 
	@return The volume. Valid values are from 0 (mute) to 10 (loudest). 
	The default is 5. 
	@see AFTTSSetVolume
	Determines whether the TTS object is available and the Text-to-Speech 
	engine can be used. 

	NOTE: Calling any method in this API causes the Forms plug-in 
	to initialize TTS automatically, if an SAPI engine is installed 
	on the system. 
	@return true if the Text-to-Speech engine can be used, false otherwise. 
	
*/
typedef ACCBPROTO1 ASUns32 (ACCBPROTO2 *AFTTSGetVolume_SELPROTO)();
/**
	Gets the baseline pitch for the voice of a speaker. 
	@return The baseline pitch. The valid range is from 0 to 10, with 
	5 being the default for the speaker. 
	@see AFTTSSetPitch
	Gets the name of the current speaker. 
	@return The name of the current speaker. 
	@see AFTTSSetSpeaker
*/
typedef ACCBPROTO1 ASUns32 (ACCBPROTO2 *AFTTSGetPitch_SELPROTO)();
/**
	Gets the number of different speakers available to the current 
	TTS engine. 
	@return The number of speakers available. 
	@see AFTTSGetVoiceName
*/
typedef ACCBPROTO1 ASUns32 (ACCBPROTO2 *AFTTSGetNumberOfVoices_SELPROTO)();
/**
	Sets the current voice. Valid values are any of those enumerated 
	via AFTTSGetVoiceName and AFTTSGetNumberOfVoices. 
	@param voiceName The speaker name.
	@return true if the speech engine is available, false otherwise. 
	
	@see AFTTSGetSpeaker 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AFTTSSetSpeaker_SELPROTO)(const char* voiceName);
/** Not implemented in 4.05. */
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AFTTSSetPunctuations_SELPROTO)(const char* punctuations);
/**
	Sets the speed at which text is being spoken by the TTS 
	engine. 
	@param speed The speed in number of words per minute.
	@return true if the speech engine is available, false otherwise. 
	@see AFTTSGetSpeechRate 
	Sets the speech volume. 
	@return true if the speech engine is available, false otherwise. 
	@see AFTTSGetVolume 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AFTTSSetSpeechRate_SELPROTO)(ASUns32 speed);
/** Not implemented in 4.05. */
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AFTTSSetCharacterScale_SELPROTO)(ASUns32 chrScale);
/**
	Sets the speech volume. 
	@param volume The volume. Valid values are from 0 (mute) 
	to 10 (loudest).
	@return true if the speech engine is available, false otherwise. 
	
	@see AFTTSGetVolume 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AFTTSSetVolume_SELPROTO)(ASUns32 volume);
/**
	Sets the baseline pitch for the voice of a speaker. 
	@param pitch The baseline pitch. The valid range is from 
	0 to 10, with 5 being the default for the speaker.
	@return true if the speech engine is available, false otherwise. 
	
	@see AFTTSGetPitch 
	Sets the current voice. Valid values are any of those enumerated 
	via AFTTSGetVoiceName and AFTTSGetNumberOfVoices. 
	@return true if the speech engine is available, false otherwise. 
	
	@see AFTTSGetSpeaker 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *AFTTSSetPitch_SELPROTO)(ASUns32 pitch);
/** Sets the function to be called when TTS finished speaking. NULL disables
   callbacks. */
typedef ACCBPROTO1 void (ACCBPROTO2 *AFTTSSetNotify_SELPROTO)(AFTTSNotifyProc proc);

#endif /* _H_AFTTS_SEL  } */
