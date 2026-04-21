/* Copyright 2003-2005 ROBLOX Corporation, All Rights Reserved */
#include "stdafx.h"

#include "Util/HeartbeatInstance.h"
#include "Util/RunStateOwner.h"

namespace ARL {

void HeartbeatInstance::onServiceProviderHeartbeatInstance(ServiceProvider* oldProvider, ServiceProvider* newProvider)
{
	ARLASSERT((oldProvider == NULL) != (newProvider == NULL));

	heartbeatConnection.disconnect();

	if (newProvider) {
		RunService* runService = ServiceProvider::create<RunService>(newProvider);
		ARLASSERT(runService);
		heartbeatConnection = runService->heartbeatSignal.connect(boost::bind(&HeartbeatInstance::onHeartbeat, this, _1));
	}
}

} // namespace