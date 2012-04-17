#include "PacketManager.h"

#include <iterator>

PacketManager::PacketManager(ENetHost *server, shared_ptr<BlowFish> blowfish)
{
	_server = server;
	_blowfish = blowfish;
	_packetHandlers.clear();		
}

PacketManager::~PacketManager()
{
	_packetHandlers.clear();
}

void PacketManager::registerIPacketHandler(IPacketHandler* handler)
{
	_packetHandlers.push_back(handler);
}

void PacketManager::unregisterIPacketHandler(IPacketHandler* handler)
{
	auto it = _packetHandlers.begin();
	for(; it != _packetHandlers.end(); ++it)
	{	
		if(*it._Ptr == handler) break;
	}
	_packetHandlers.erase(it);
}

void PacketManager::printPacket(uint8 *buf, uint32 len)
{
	PDEBUG_LOG(Log::getMainInstance(),"   ");
	for(uint32 i = 0; i < len; i++)
	{
		PDEBUG_LOG(Log::getMainInstance(),"%02X ", static_cast<uint8>(buf[i]));
		if((i+1)%16 == 0)
			PDEBUG_LOG(Log::getMainInstance(),"\n   ");
	}
	PDEBUG_LOG(Log::getMainInstance(),"\n");
}

void PacketManager::printLine(uint8 *buf, uint32 len)
{
	for(uint32 i = 0; i < len; i++)
		PDEBUG_LOG(Log::getMainInstance(),"%02X ", static_cast<uint8>(buf[i]));
	PDEBUG_LOG(Log::getMainInstance(),"\n");
}

bool PacketManager::sendPacket(ENetPeer *peer, uint8 *data, uint32 length, uint8 channelNo, uint32 flag)
{
	PDEBUG_LOG_LINE(Log::getMainInstance()," Sending packet:\n");
	if(length < 300)
		printPacket(data, length);

	if(length >= 8)
		_blowfish->Encrypt(data, length-(length%8)); //Encrypt everything minus the last bytes that overflow the 8 byte boundary

	ENetPacket *packet = enet_packet_create(data, length, flag);
	if(enet_peer_send(peer, channelNo, packet) < 0)
	{
		PDEBUG_LOG_LINE(Log::getMainInstance(),"Warning fail, send!");
		return false;
	}
	return true;
}

bool PacketManager::broadcastPacket(uint8 *data, uint32 length, uint8 channelNo, uint32 flag)
{
	PDEBUG_LOG_LINE(Log::getMainInstance()," Broadcast packet:\n");
	printPacket(data, length);

	if(length >= 8)
		_blowfish->Encrypt(data, length-(length%8)); //Encrypt everything minus the last bytes that overflow the 8 byte boundary

	ENetPacket *packet = enet_packet_create(data, length, flag);

	enet_host_broadcast(_server, channelNo, packet);
	return true;
}

bool PacketManager::handlePacket(ENetPeer *peer, ENetPacket *packet, uint8 channelID)
{
	if(packet->dataLength >= 8)
	{
		if(peerInfo(peer)->keyChecked)
			_blowfish->Decrypt(packet->data, packet->dataLength-(packet->dataLength%8)); //Encrypt everything minus the last bytes that overflow the 8 byte boundary
	}

	bool handled = false;

	for(int i = 0; i < _packetHandlers.size(); i++)
	{
		handled = handled || _packetHandlers[i]->processPacket(peer,packet,channelID);
	}

	if(!handled)
	{
		PacketHeader* header = (PacketHeader*)packet->data;
		PDEBUG_LOG_LINE(Log::getMainInstance(),"Unknown packet: CMD %X(%i) CHANNEL %X(%i)\n", header->cmd, header->cmd,channelID,channelID);
	}

	return handled;	
}

shared_ptr<BlowFish> PacketManager::getBlowFish() const
{
	return _blowfish;
}

