/* Copyright 2003-2005 ROBLOX Corporation, All Rights Reserved */
#include "stdafx.h"

#include "Util/SteppedInstance.h"
#include "FastLog.h"

namespace ARL {

void IStepped::onServiceProviderIStepped(ServiceProvider* oldProvider, ServiceProvider* newProvider)
{
	ARLASSERT((oldProvider == NULL) != (newProvider == NULL));

	steppedConnection.disconnect();

	if (newProvider) {
		RunService* runService = ServiceProvider::create<RunService>(newProvider);
		ARLASSERT(runService);
		FASTLOG1(FLog::ISteppedLifetime, "Subscribed to IStepped", this);

        switch (stepType)
        {
		case StepType_Default:
			steppedConnection = runService->steppedSignal.connect(boost::bind(&IStepped::onStepped, this, _1));
            break;

        case StepType_HighPriority:
			steppedConnection = runService->highPrioritySteppedSignal.connect(boost::bind(&IStepped::onStepped, this, _1));
            break;

        case StepType_Render:
			steppedConnection = runService->renderSteppedSignal.connect(boost::bind(&IStepped::onStepped, this, _1));
            break;

        default:
            ARLASSERT(false);
            break;
        }
	}
}
} // namespace