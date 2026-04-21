#pragma once

#include "Util/SystemAddress.h"
#include "util/G3DCore.h"
#include "RakNetTypes.h"

#ifdef _WIN32
#if defined(_NOOPT) || defined(_DEBUG) || defined(ARL_TEST_BUILD)
#define NETWORK_PROFILER
#define NETWORK_DEBUG
#endif
#endif

namespace ARL {

namespace SpatialRegion {
	class Id;
}

namespace Network {

	const ARL::SystemAddress RakNetToRbxAddress(const RakNet::SystemAddress& raknetAddress);
	std::string RakNetAddressToString(const RakNet::SystemAddress& raknetAddress, bool writePort = true, char portDelineator='|');

}}	// namespace

