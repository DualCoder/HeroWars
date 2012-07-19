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

#include "IDatabase.h"
#include "PacketHandler.h"
#include "MySQL.h"
#include "PostgreSQL.h"
#if _WIN32
#include <Windows.h>
#else
#include <pthread.h>
#endif

class TCPServer
{
public:
	TCPServer(DatabaseType dbType);
	~TCPServer();
	bool initialize(string port);
	void run();
	bool SendPacket(SOCKET client, uint8* data, int length);
	bool SendFile(SOCKET client, char* fileName);
	void shutdown();
	Ptr<IDatabase> getDatabase();

	Ptr<PacketHandler> _packetHandler;
private:
	SOCKET _server;
	Ptr<IDatabase> _database;
};

typedef struct _threadData
{
	_threadData(Client pClient, TCPServer* pServer)
	{
		client = pClient;
		server = pServer;
	}
	Client client;
	TCPServer* server;
}ThreadData;


#endif  //_TCPSERVER_H
