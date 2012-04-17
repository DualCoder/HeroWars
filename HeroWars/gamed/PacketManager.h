
#ifndef _PACKET_HANDLER_H
#define _PACKET_HANDLER_H
#include <memory>
#include <vector>

#include <enet/enet.h>

#include "common.h"
#include "Log.h"
#include "ChatBox.h"
#include "IPacketHandler.h"

#include <intlib/base64.h>
#include <intlib/blowfish.h>


#include "Packets.h"
#include "Client.h"

#define RELIABLE ENET_PACKET_FLAG_RELIABLE
#define UNRELIABLE 0

class PacketManager
{
	public:
		PacketManager(ENetHost *server, shared_ptr<BlowFish> blowfish);
		~PacketManager();
		
		void registerIPacketHandler(IPacketHandler* handler);
		void unregisterIPacketHandler(IPacketHandler* handler);
		bool handlePacket(ENetPeer *peer, ENetPacket *packet, uint8 channelID);
		void printPacket(uint8 *buf, uint32 len);
		void printLine(uint8 *buf, uint32 len);
		bool sendPacket(ENetPeer *peer, uint8 *data, uint32 length, uint8 channelNo, uint32 flag = RELIABLE);
		bool broadcastPacket(uint8 *data, uint32 length, uint8 channelNo, uint32 flag = RELIABLE);	
		shared_ptr<BlowFish> getBlowFish() const;

	private:
		ENetHost *_server;
		shared_ptr<BlowFish> _blowfish;
		std::vector<IPacketHandler* > _packetHandlers;
};
#endif