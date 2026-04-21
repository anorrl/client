/* Copyright 2003-2005 ROBLOX Corporation, All Rights Reserved */
#include "stdafx.h"

#include "V8World/IPipelined.h"
#include "V8Kernel/Kernel.h"

namespace ARL {


Kernel* IPipelined::getKernel() const				
{
	ARLASSERT(this->inKernel());
	IStage* answer = getStage(IStage::KERNEL_STAGE);
	return rbx_static_cast<Kernel*>(answer);
}

void IPipelined::putInKernel(Kernel* kernel)
{
	putInStage(kernel);
}

void IPipelined::removeFromKernel()
{
	ARLASSERT(currentStage);
	ARLASSERT(currentStage->getStageType() == IStage::KERNEL_STAGE);
	removeFromStage(currentStage);
}



IStage* IPipelined::getStage(IStage::StageType stageType) const
{
	ARLASSERT(currentStage);
	IStage* tryStage = currentStage;
	do {
		if (tryStage->getStageType() == stageType) {
			return tryStage;
		}
		tryStage = (tryStage->getStageType() > stageType)
						?	tryStage->getUpstream()
						:	tryStage->getDownstream();
	}
	while (1);
}

void IPipelined::putInPipeline(IStage* stage)
{
	ARLASSERT(stage);
	ARLASSERT(!currentStage);
	currentStage = stage;
}

void IPipelined::removeFromPipeline(IStage* stage)
{
	ARLASSERT(stage);
	ARLASSERT(currentStage);
	ARLASSERT(currentStage == stage);
	currentStage = NULL;
}


void IPipelined::putInStage(IStage* stage)
{
	ARLASSERT(stage);
	ARLASSERT(currentStage);
	ARLASSERT(stage->getUpstream() == currentStage);
	ARLASSERT(currentStage->getDownstream() == stage);
	currentStage = stage;
}

void IPipelined::removeFromStage(IStage* stage)
{
	ARLASSERT(currentStage);
	ARLASSERT(stage);
	ARLASSERT(stage == currentStage);
	ARLASSERT(stage->getUpstream());
	currentStage = currentStage->getUpstream();
}

} // namespace