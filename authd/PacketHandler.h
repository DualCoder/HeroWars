/*
HeroWars server for League of Legends 
Copyright (C) 2012  Intline9

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

title: authd Server
author: C0dR

*/



#ifndef _PACKETHANDLER_H
#define _PACKETHANDLER_H

#include "AuthHandler.h"
#include "Packets.h"

//dummy class
class TCPServer;

class PacketHandler
{
public:
	PacketHandler(TCPServer *tcpServer);
	bool HandlePacket(Client* client, char* packet);
	void removeUser(uint32 sessionHash);
private:
	bool handleHandshake(Client* client, char* packet);
	bool handleAuth(Client* client, char* packet);
	bool handleDisconnect(Client* client, char* packet);
	TCPServer *_tcpServer;
	Ptr<AuthHandler> _authHandler;
};

#endif  //_PACKETHANDLER_H
