#pragma once

#include "PacketIds.h"
#include "Streaming.h"
#include "NetworkSettings.h"
#include "Item.h"
#include "network/api.h"
#include "PluginInterface2.h"
#include "v8tree/Instance.h"
#include "util/RunStateOwner.h"
#include "util/Region2.h"
#include "v8datamodel/DataModelJob.h"
#include "queue"
#include "Rand.h"

class RakPeerInterface;
class PacketLogger;
class RakPeer;

namespace ARL { 

namespace Network {

	class ConcurrentRakPeer;
	class Replicator;
	class PacketReceiveJob;

	// Client and Server descend from this class
	extern const char* const sPeer;
	class Peer 
		: public Reflection::Described<Peer, sPeer, Instance >
		, public RakNet::PluginInterface2
	{
	private:
		typedef Reflection::Described<Peer, sPeer, Instance > Super;
		shared_ptr<PacketReceiveJob> receiveJob;
		static unsigned char aesKey[ 16 ];
		RakNet::RakNetRandom rnr;
	public:
		RunningAverageDutyCycle<> rakDutyCycle;
		boost::shared_ptr<ConcurrentRakPeer> rakPeer;
		void setOutgoingKBPSLimit(int limit);

		void encryptDataPart(RakNet::BitStream& bitStream);
		static void decryptDataPart(RakNet::BitStream& bitStream);

	protected:
		
		int protocolVersion;	// default network protocol, shared between client and server
		
		Peer();
		~Peer();
		// Called after rakPeer is created
		virtual void onCreateRakPeer();
		bool askAddChild(const Instance* instance) const;
		/*override*/ void onServiceProvider(ServiceProvider* oldProvider, ServiceProvider* newProvider);
	};


} }

