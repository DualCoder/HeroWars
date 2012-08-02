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



#ifndef _TCPSERVER_H
#define _TCPSERVER_H

#include "ISocket.h"

class WinSocket : public ISocket
{
public:
	WinSocket(DatabaseType dbType);
	~WinSocket();
	bool initialize(string port);
	void run();
	bool SendPacket(SOCKET client, uint8* data, int length);
	bool SendFile(SOCKET client, char* fileName);
	void shutdown();
	Ptr<IDatabase> getDatabase();
	Ptr<PacketHandler> getPacketHandler();

private:
	SOCKET _server;
	Ptr<IDatabase> _database;
	Ptr<PacketHandler> _packetHandler;
};


#endif  //_TCPSERVER_H
