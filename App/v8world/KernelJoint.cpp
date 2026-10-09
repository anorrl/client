 /* Copyright 2003-2005 ROBLOX Corporation, All Rights Reserved */
#include "stdafx.h"

#include "v8world/KernelJoint.h"
#include "v8world/Primitive.h"
#include "v8kernel/Kernel.h"
#include "v8kernel/Constants.h"
#include "v8kernel/Connector.h"
#include "v8kernel/Body.h"

namespace ARL {


void KernelJoint::putInKernel(Kernel* _kernel)
{
	Super::putInKernel(_kernel);

	_kernel->insertConnector(this);
}


void KernelJoint::removeFromKernel()
{
	getKernel()->removeConnector(this);

	Super::removeFromKernel();
}




} // namespace


// Randomized Locations for hackflags
namespace ARL 
{ 
    namespace Security
    {
        unsigned int hackFlag8 = 0;
    };
};
