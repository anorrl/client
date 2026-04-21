/* Copyright 2003-2005 ROBLOX Corporation, All Rights Reserved */

#pragma once

#include "Util/SystemAddress.h"
#include "Util/Color.h"

namespace ARL {
	namespace Network {

	class NetworkOwner 
	{
	public:
		static const ARL::SystemAddress Server() {
			static ARL::SystemAddress s(1, 0);
			return s;
		}

		// created on server, have not be properly assigned via network owner job
		static const ARL::SystemAddress ServerUnassigned() {
			static ARL::SystemAddress s(1, 1);
			return s;
		}

		// default
		static const ARL::SystemAddress Unassigned() {
			static ARL::SystemAddress s;
			ARLASSERT(s == SystemAddress());
			ARLASSERT(s != NetworkOwner::Server());
			ARLASSERT(s != NetworkOwner::ServerUnassigned());
			ARLASSERT(s != NetworkOwner::AssignedOther());
			return s;
		}

		// generic value used on client indicating assigned to other clients or server (i.e. not self)
		static const ARL::SystemAddress AssignedOther() {
			static ARL::SystemAddress s(0, 1);
			return s;
		}

		static bool isClient(const ARL::SystemAddress& address) {
			return (	(address != Server())
					&&	(address != Unassigned())	
					&&  (address != ServerUnassigned()));
		}

		static bool isServer(const ARL::SystemAddress& address) {
			return address == Server() || address == ServerUnassigned();
		}
		
		static Color3 colorFromAddress(const ARL::SystemAddress& systemAddress) {
			if (systemAddress == Server()) {
				return Color3::white();
			}
			else if (systemAddress == Unassigned() || systemAddress == ServerUnassigned()) {
				return Color3::black();
			}
			else if (systemAddress == AssignedOther())
				return Color3::gray();
			else {
				unsigned int address = systemAddress.getAddress();
				unsigned int port = systemAddress.getPort();
				address += port;
				return ARL::Color::colorFromInt(address);
			}
		}

	};

}}	// namespace