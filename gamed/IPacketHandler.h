#ifndef _INTERFACE_PACKET_HANDLER_H
#define _INTERFACE_PACKET_HANDLER_H
#include <enet\enet.h>
#define HANDLE_ARGS ENetPeer *peer, ENetPacket *packet,  unsigned int channelID
/*
*	Interface that represent classes that can handle received packets.
*/
class IPacketHandler
{
public:
	/*
	* Return true if the packet has been sucessfully dispatched and handled, otherwise false.
	*/
	virtual bool processPacket(HANDLE_ARGS) = 0;
};
#endif