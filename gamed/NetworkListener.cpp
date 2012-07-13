#include "NetworkListener.h"
#include "Log.h"

NetworkListener::NetworkListener()
{

}

NetworkListener::~NetworkListener()
{
	_isAlive = false;
			
	enet_host_destroy(_server);
}

bool NetworkListener::initialize(ENetAddress *address, const char *baseKey)
{
	if (enet_initialize () != 0)
		return false;
	atexit(enet_deinitialize);

	_server = enet_host_create(address, 32, 0, 0);
	if(_server == NULL)
		return false;

	std::string key = base64_decode(baseKey);
	if(key.length() <= 0)
		return false;
	_blowfish = shared_ptr<BlowFish>(new BlowFish((uint8*)key.c_str(), 16));
	_handler = shared_ptr<PacketManager>(new PacketManager(_server, _blowfish));
	
	return _isAlive = true;
}

void NetworkListener::processPacket()
{
	ENetEvent event;

	while(enet_host_service(_server, & event, 10) > 0 && _isAlive) // > 0 we dispatch all received packets 
	{
		switch (event.type)
		{
			case ENET_EVENT_TYPE_CONNECT:
				Log::getMainInstance()->writeLine("A new client connected connected\n");

				/* Set some defaults */
				event.peer->mtu = PEER_MTU;

				event.peer->data = new ClientInfo();
				peerInfo(event.peer)->setName("HeroWars");
				peerInfo(event.peer)->setType("Teemo");

			break;

		case ENET_EVENT_TYPE_RECEIVE:
			if(!_handler->handlePacket(event.peer, event.packet,event.channelID))
			{
				//enet_peer_disconnect(event.peer, 0);
			}

			/* Clean up the packet now that we're done using it. */
			enet_packet_destroy (event.packet);
		break;

		case ENET_EVENT_TYPE_DISCONNECT:
			Log::getMainInstance()->writeLine("Peer disconnected.\n");

			/* Cleanup */
			delete event.peer->data;
		break;
		}
	}
}

shared_ptr<PacketManager> NetworkListener::getPacketManager()
{
	return _handler;
}