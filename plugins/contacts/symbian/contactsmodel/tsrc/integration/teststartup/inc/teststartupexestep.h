/*
* Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
* Contact: http://www.qt-project.org/legal
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*
*/




#ifndef __TESTSTARTUPEXESTEP_H__
#define __TESTSTARTUPEXESTEP_H__

#include "teststartupserver.h"

class CTestStartUpExeStep : public CTestStep
    {
public:        
    CTestStartUpExeStep(CTestStartUpServer& aTestServerPtr);
    ~CTestStartUpExeStep();
	TVerdict doTestStepPreambleL();
    TVerdict doTestStepPostambleL();
	TVerdict doTestStepL();
	
private:
    TPtrC               iStartupAction;
    TInt                iExitReasonFromIni;
    TPtrC               iCntsStarupExeName;  
    TPtrC               iServerMode;
    TExitType           iExitTypeFromIni;
    CTestStartUpServer* iTestServerPtr;      
    };
    
#endif
