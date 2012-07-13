#ifndef _NETWORK_LISTENER_H
#define _NETWORK_LISTENER_H

#include <enet/enet.h>
#include <intlib/base64.h>
#include <intlib/blowfish.h>
#include <memory>

#include "common.h"
#include "PacketManager.h"

#define PEER_MTU 996
class NetworkListener
{
	public:
		NetworkListener();
		~NetworkListener();

		bool initialize(ENetAddress *address, const char *baseKey);
		void processPacket();
		shared_ptr<PacketManager> getPacketManager();

	private:
		bool _isAlive;
		shared_ptr<PacketManager> _handler;
		ENetHost* _server;
		shared_ptr<BlowFish> _blowfish;
};

#endif

