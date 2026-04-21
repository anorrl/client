 /* Copyright 2003-2005 ROBLOX Corporation, All Rights Reserved */
#include "stdafx.h"

#include "V8World/MultiJoint.h"
#include "V8World/Primitive.h"
#include "V8World/Tolerance.h"
#include "V8Kernel/Kernel.h"
#include "V8Kernel/Connector.h"
#include "V8Kernel/Point.h"

namespace ARL {

void MultiJoint::init(int numBreaking)
{
	numBreakingConnectors = numBreaking;
	numConnector = 0;
	for (int j = 0; j < 4; j++) {
		point[j*2] = NULL;
		point[j*2+1] = NULL;
		connector[j] = NULL;
	}
}

MultiJoint::MultiJoint(int numBreaking)
{
	init(numBreaking);
}

MultiJoint::MultiJoint(
	Primitive* p0,
	Primitive* p1,
	const CoordinateFrame& jointCoord0,
	const CoordinateFrame& jointCoord1,
	int numBreaking)
	:	Joint(p0, p1, jointCoord0, jointCoord1)
{
	init(numBreaking);
}

MultiJoint::~MultiJoint() 
{
	ARLASSERT(connector[0] == NULL);
	ARLASSERT(numConnector == 0);
}

void MultiJoint::putInKernel(Kernel* _kernel)
{
	Super::putInKernel(_kernel);
}


float MultiJoint::getJointK()
{
	return G3D::min( getPrimitive(0)->getJointK(), getPrimitive(1)->getJointK() );
}

void MultiJoint::addToMultiJoint(Point* point0, Point* point1, Connector* _connector)
{
	ARLASSERT(numConnector < 4);
	
	point[numConnector*2] = point0;
	point[numConnector*2+1] = point1;
	connector[numConnector] = _connector;
	getKernel()->insertConnector(_connector);

	numConnector++;

	ARLASSERT_IF_VALIDATING(validateMultiJoint());
}

Point* MultiJoint::getPoint(int id)
{
	ARLASSERT(id < 8);
	ARLASSERT(point[id] != NULL);
	ARLASSERT(point[id]->getBody() == getPrimitive(id % 2)->getBody());
	return point[id];
}

Connector* MultiJoint::getConnector(int id)
{
	ARLASSERT(id < 4);
	ARLASSERT(connector[id] != NULL);
	return connector[id];
}

#ifdef __ARL_VALIDATE_ASSERT
bool MultiJoint::pointsAligned() const
{
	ARLASSERT(numBreakingConnectors <= numConnector);

		// THIS ASSERT WILL BLOW in the situation where "unaligned" joints
		// are present on save or sleep.  For now, just watching to see how often 
		// this occurs
		// If too often - will need to store joint information or implement wheel/holes
		//
	for (int i = 0; i < numBreakingConnectors; ++i) {
		if (Tolerance::pointsUnaligned(
							point[i*2]->getWorldPos(), 
							point[i*2+1]->getWorldPos())) {
			return false;
		}
	}
	return true;
}
#endif


bool MultiJoint::validateMultiJoint()
{
#ifdef _DEBUG
	for (int i = 0; i < numConnector; ++i) 
	for (int j = 0; j < 2; ++j)
	{{
		Point *p = point[i*2 + j];
		ARLASSERT(p->getBody() == this->getPrimitive(j)->getBody());
		
		Connector* c = connector[i];
		ARLASSERT(c->getBody((Connector::BodyIndex)j) == this->getPrimitive(j)->getBody());
	}}
#endif
	return true;
}


void MultiJoint::removeFromKernel()
{
	ARLASSERT(this->inKernel());

// TODO - unsuppress this - indicates something that will NOT rejoin!
//	ARLASSERT_IF_VALIDATING(pointsAligned());

	ARLASSERT_IF_VALIDATING(validateMultiJoint());

	for (int i = 0; i < numConnector; i++) {
		ARLASSERT(connector[i]);

		getKernel()->removeConnector(connector[i]);
		delete connector[i];
		getKernel()->deletePoint(point[i*2]);
		getKernel()->deletePoint(point[i*2+1]);

		point[i*2] = NULL;
		point[i*2+1] = NULL;
		connector[i] = NULL;
	}

	numConnector = 0;

	Super::removeFromKernel();

	ARLASSERT(!this->inKernel());
}

bool MultiJoint::isBroken() const 
{
///	int max = std::min(numConnector, numBreakingConnectors);
	ARLASSERT(numBreakingConnectors <= numConnector);
	ARLASSERT(this->inKernel());

//	ToDo:  Saving joints, otherwise a save in this configuration could cause broken joints.
//	ARLASSERT_IF_VALIDATING(pointsAligned());
	
	for (int i = 0; i < numBreakingConnectors; ++i) {
		ARLASSERT(connector[i]);
		if (connector[i]->getBroken()) {
			return true;
		}
	}
	return false;
}


} // namespace

// Randomized Locations for hackflags
namespace ARL 
{ 
    namespace Security
    {
        unsigned int hackFlag10 = 0;
    };
};
