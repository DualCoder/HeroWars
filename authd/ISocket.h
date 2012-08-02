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

#ifndef _ISOCKET_H
#define _ISOCKET_H

#include "PacketHandler.h"
#include "MySQL.h"
#include "PostgreSQL.h"
#include "Log.h"

#if _WIN32
#include <Windows.h>
#else
#include <pthread.h>
#endif

class ISocket
{
public:
	virtual bool initialize(string port) = 0;
	virtual void run() = 0;
	virtual bool SendPacket(SOCKET client, uint8* data, int length) = 0;
	virtual bool SendFile(SOCKET client, char* fileName) = 0;
	virtual void shutdown() = 0;
	virtual Ptr<IDatabase> getDatabase() = 0;
	virtual Ptr<PacketHandler> getPacketHandler() = 0;
};

typedef struct _threadData
{
	_threadData(Client pClient, ISocket* pServer)
	{
		client = pClient;
		server = pServer;
	}
	Client client;
	ISocket* server;
}ThreadData;




#endif