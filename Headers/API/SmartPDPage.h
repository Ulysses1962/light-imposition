/*********************************************************************************
	
	ADOBE SYSTEMS INCORPORATED
	Copyright (C) 1994-2004 Adobe Systems Incorporated
	All rights reserved.

	NOTICE: Adobe permits you to use, modify, and distribute this file
	in accordance with the terms of the Adobe license agreement
	accompanying it. If you have received this file from a source other
	than Adobe, then your use, modification, or distribution of it
	requires the prior written permission of Adobe.

 ---------------------------------------------------------------------
	
	SmartPDPage.h
	
	- This class contains a thin wrapper for PDPage objects that
		ensures the reference count is decremented when the object
		is destroyed.

************************************************************************************/

#ifndef _SMARTPDPAGE_H
#define _SMARTPDPAGE_H

#if defined (__cplusplus)

/* Include common macros */
#include "PDClassDefs.h"

/* Set ENABLE_SMARTPDPAGETESTS to 1 to enable unit tests for the class  */
#define ENABLE_SMARTPDPAGETESTS 0

/**
   This class ensures that PDPage objects are released even in case 
   of exceptions and RAISE. There are many ways to acquire a Page that 
   we need to be careful about. Those APIs are enumerated below.
   PDDocAcquirePage, PDPageAcquireFromCosPage, PDCosAcquirePage
   PDBeadAcquirePage. Note that PDPage objects retrieved using 
   AVPageViewGetPage should not be released.

   This class is modelled based on a SmartPointer class. It does not
   have a release() API as there was a clash in the meaning of the API
   in the auto_ptr domain and the PD API domain.
   
   This class is designed to be a Final class and hence does not
   have a virtual destructor. Classes that need this functionality
   should encapsulate it using containment.
*/

#if CPP_EXCEPTIONS
#define ERRFILE "SmartPDPage.h"
#endif

RAISEAWARECLASS(CSmartPDPage)
class CSmartPDPage
#ifdef BASECLASS
: public BASECLASS
#endif
{
	RAISEAWARE_WITH_COPY_CONSTRUCTOR(CSmartPDPage)
public:

	/**
		Constructor
		This is the default constructor. It is used along with
		AssignAndTakeOwnership or the AcquirePage method.
	*/
	CSmartPDPage(): m_pdPage(NULL)
	{
		CTOR;	/* Macro for creating a RaiseAwareConstructor */
	}

	/**
		Constructor
		Constructor for the most common PDPage creation API. Modules using this
		class should never call the PDDocAcquirePage directly.
		@param pdDoc			The PDDoc containing the page that is requested. This
								cannot be NULL.
		@param nPageNum			The page number of the page that is requested. This cannot
								be negative and should be in the page range of the document.
	*/
	CSmartPDPage(PDDoc pdDoc, ASInt32 nPageNum): m_pdPage(NULL)
	{
		CTOR;	/* Macro for creating a RaiseAwareConstructor */
		AssignPage(pdDoc, nPageNum);
	}

	/**
		Constructor
		Constructor that takes a PDPage pointer as parameter. It increases the
		reference count of the object before taking ownership of the PDPage 
		pointer passed in. It is to be used in cases where the PDPage pointer
		is acquired by APIs that do not increase the reference count like
		AVPageViewGetPage
		@param pdPage	The Page object. This should not be NULL
		@Example		CSmartPDPage cPDPage(AVPageViewGetPage(avPageView));
	*/
	explicit CSmartPDPage( PDPage pdPage): m_pdPage(NULL)
	{
		CTOR;	/* Macro for creating a RaiseAwareConstructor */
		AssignPage(pdPage);
	}

	/**
		Copy Constructor
		It increases the reference count of the contained PDPage object in case
		it is not NULL
		@param cAutoPage	The CSmartPDPage object.
	*/
	CSmartPDPage (const CSmartPDPage &cAutoPage): m_pdPage(NULL)
	{
		CTOR;	/* Macro for creating a RaiseAwareConstructor */
		/* Increase the refcount for the page. */
		if (cAutoPage.m_pdPage != NULL)
			AssignPage(cAutoPage.m_pdPage);
	}

	/**
		Destructor
		If the PDPage object contained within is not empty then it 
		releases the PDPage object. This method is not a virtual method
		as per design as the class is supposed to be a Final class.
	*/
	~CSmartPDPage ()
	{
		DTOR;	/* Macro for creating a RaiseAwareDestructor */
		if (m_pdPage != NULL)
		{
			/* Destructor should not Raise and hence is different from Reset */
			PDPage tmpPage = m_pdPage;
			m_pdPage = NULL;
			DURING
				PDPageRelease(tmpPage);
			HANDLER
			END_HANDLER
		}
	}

	/**
		AcquirePage
		The PDPage object contained within is released. It then acquires a new PDPage
		pointer using the PDDocAcquirePage API. Modules using this class should	never call 
		the PDDocAcquirePage directly.
		@param pdDoc			The PDDoc containing the page that is requested. This
								cannot be NULL.
		@param nPageNum			The page number of the page that is requested. This cannot
								be negative and should be in the page range of the document.
		@Exception				The object will be set to empty state.
	*/
	void AcquirePage(PDDoc pdDoc, ASInt32 nPageNum)
	{
		/* Release the current PDPage object. */
		Reset();
		/* Acquire a new page */
		AssignPage(pdDoc, nPageNum);
	}

	/**
		AssignAndTakeOwnership
		The old PDPage pointer contained within is released. It then takes ownership 
		of the PDPage pointer passed in as parameter. The method does not increase the
		reference count of the passed in PDPage object. This method is used in scenarios 
		where the Smart pointer needs to be constructed at a point earlier to the point
		where the page object is available. It is also used with APIs like 
		PDPageAcquireFromCosPage that have already increased the reference count for
		the PDPage object but are not encapsulated by the class methods.
		@param pdPage			The Page object. It cannot be NULL.
		@Exception				The object will be set to empty state.
	*/
	void AssignAndTakeOwnership(PDPage pdPage)
	{
		ACROASSERT(pdPage != NULL);
		/* Release the current PDPage object. */
		Reset();
		/* Take ownership of the new PDPage object. */
		m_pdPage = pdPage;
	}

	/**
		PDPage operator
		Operator to access the PDPage pointer contained within. This gives access to 
		the raw pointer within and hence should not be released. The lifetime of the
		PDPage pointer returned is bound to the CSmartPDPage object that returned it.
		@Design Note			Using a explicit get method was discussed but was dropped 
								because of the convenience factor of using PDPage operator.
		@return the PDPage contained in this object. This may be NULL.
		@Exception				Does not raise exception.
	*/
	inline operator PDPage ( void ) const
	{
		return m_pdPage;
	}

	/**
		Assignment operator
		This copies the PDPage pointer contained within the CSmartPDPage object passed 
		in as parameter. It increases the reference count in case the PDPage pointer
		contained within the passed in object is not NULL.
		@param hRhs				The Page object.
		@return a reference to this object
		@Exception				The object will be set to empty state.
	*/
	CSmartPDPage& operator=( const CSmartPDPage& hRhs )
	{
		if( this != &hRhs )
		{
			/* Release the current PDPage object. */
			Reset();
			/* Assign and increase the refcount for the page. */
			if (hRhs.m_pdPage != NULL)
				AssignPage(hRhs.m_pdPage);
		}
		return *this;
	}

	/**
		GiveOwnershipToCaller
		This method releases ownership of the internal PDPage object. The caller
		will be responsible for releasing the PDPage object. The PDPage object 
		contained within will be empty once this API is called and hence should not 
		be used.
		@return the PDPage that was contained in this object. This can be NULL
		@Exception				Does not raise exception.
	*/
	PDPage GiveOwnershipToCaller()
	{
		PDPage pdRetVal = m_pdPage;
		m_pdPage = NULL;
		return pdRetVal;
	}

	/**
		Reset
		Method to release the PDPage object contained within. In case the object
		is empty it does not do anything. The PDPage object contained within will 
		no longer be valid once this API is called and hence should not be used.
		@Exception				Object will be in empty state.
	*/
	void Reset()
	{
		if (m_pdPage != NULL)
		{
			// The method 'PDPageRelease' may raise and hence we need to reset the value before this
			PDPage tmpPage = m_pdPage;
			m_pdPage = NULL;
			ASTRY
			{
				PDPageRelease(tmpPage);
			}
			END_ASTRY
		}
	}
/* Run the following tests when changing the class */
#if (ENABLE_SMARTPDPAGETESTS)
	static void UnitTestSmartPDPage(PDPage pdPageOwnedByCaller, PDDoc pdDoc, ASInt32 nPageNum)
	{
		//////////////////////////////////////
		// Test Code to test CSmartPDPage
		// ASRaise test
		DURING
		{
			CSmartPDPage pdPage(pdPageOwnedByCaller);
			ASRaise(0);
		}
		HANDLER
		END_HANDLER
		// Method tests
		{
			// Constructor
			CSmartPDPage pdPage(pdDoc, nPageNum);
			// PDPage operator
			CSmartPDPage pdCopy = pdPage;
			// Copy constructor
			CSmartPDPage pdCopy2;
			CSmartPDPage pdCopy3(pdCopy2);
			CSmartPDPage pdCopy4(pdCopy);
			// Release
			PDPageRelease(pdCopy.GiveOwnershipToCaller());
			//Assignment operator
			pdPage = pdCopy4;
			pdPage = pdCopy;
			//Assign and take ownership
			pdPage.AssignAndTakeOwnership(pdCopy4.GiveOwnershipToCaller());
			// Acquire page
			pdPage.AcquirePage(pdDoc, nPageNum);
			// Reset
			pdPage.Reset();
			// Destructor
			{
				CSmartPDPage pdPageDesTest, nullPDPageTest;
				pdPageDesTest.AcquirePage(pdDoc, nPageNum);
			}
		}
	}
#endif /* ENABLE_SMARTPDPAGETESTS */

private:

#if (NEEDS_RAISEAWARE)
	/* The volatile keyword is needed in a RAISE scenario */
	PDPage volatile m_pdPage;
#else
	PDPage m_pdPage;
#endif

	/**
		PDPage Assignment operator
		There is no clear way to handle this operator. It could be used as
		AssignAndTakeOwnership (reference count unchanged) or AcquirePage
		where the reference count is increased by 1. Hence we have made this
		private so that the unambiguous APIs are explicitly used.
		@param hRhs				The Page object.
	*/
	CSmartPDPage& operator=( const PDPage& hRhs );

	/**
		AcquirePage
		Wrapper for the PDPageAcquirePage API. This API assumes that there is no
		PDPage object contained within.
		@param pdPage			The Page object. This should not be NULL
		@Exception				The object will be in empty state.
	*/
	void AssignPage(PDPage pdPage)
	{
		ACROASSERT((m_pdPage == NULL) && (pdPage != NULL));
		/* Assign and increase the refcount for the page. */
		ASTRY 
		{
			PDPageAcquirePage(pdPage);
			m_pdPage = pdPage;
		}
		END_ASTRY;
	}
	/**
		AcquirePage
		Wrapper for the PDDocAcquirePage API. This API assumes that there is no
		PDPage object contained within.
		@param pdDoc			The PDDoc containing the page that is requested. This
								cannot be NULL.
		@param nPageNum			The page number of the page that is requested. This cannot
								be negative and should be in the page range of the document.
		@Exception				The object will be in empty state.
	*/
	void AssignPage(PDDoc pdDoc, ASInt32 nPageNum)
	{
		ACROASSERT((m_pdPage == NULL) && (pdDoc != NULL) && (nPageNum >= 0));
		/* Acquire the page. */
		ASTRY 
		{
			m_pdPage = PDDocAcquirePage(pdDoc, nPageNum);
		}
		END_ASTRY;
	}
};

#if CPP_EXCEPTIONS
#undef ERRFILE
#endif

#endif /* __cplusplus */

#endif /* _SMARTPDPAGE_H */
