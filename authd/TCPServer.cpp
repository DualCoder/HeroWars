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
#include "stdafx.h"
#include "TCPServer.h"
#include "Log.h"
#include "Settings.h"


#if _WIN32
DWORD WINAPI ClientHandler(LPVOID lpParam )
#else
void ClientHandler(void *lpParam )
#endif
{
	ThreadData* threadData = reinterpret_cast<ThreadData*>(lpParam);
	TCPServer* server = threadData->server;
	Client client = threadData->client;

	char buff[1024];
	int msgSize;

	while(true)
	{
		msgSize=recv(client.socket,buff,1024,0);
		if(msgSize==SOCKET_ERROR || msgSize == 0 )
		{
			server->_packetHandler->removeUser(client.sessionHash);
			Logging->writeDebug("Lost connection\n");
			break;
		}
		buff[msgSize]=0;

		if(!server->_packetHandler->HandlePacket(&client,buff))
		{
			Logging->writeError("connection closed\n");
			break;
		}
	}
	return 0;
}

TCPServer::TCPServer(DatabaseType dbType)
{
	switch(dbType)
	{
	case DT_MYSQL:
		_database = newPtr<MySQL>();
		break;
	case DT_POSTGRESQL:
		_database = newPtr<PostgreSQL>();
		break;
	}
	_packetHandler = newPtr<PacketHandler>(this);
}
TCPServer::~TCPServer()
{
	shutdown();
}
bool TCPServer::initialize(string port)
{
	hostent* localHost;
	char* localIP;
	sockaddr_in addr;
	WSADATA wsaData;

	// Initialize Winsock
	HRESULT iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		Logging->writeError("WSAStartup failed: %d\n", iResult);
		return false;
	}

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(strtoul(port.c_str(),NULL,0));

	_server = socket(AF_INET,SOCK_STREAM,0);
	if(_server==INVALID_SOCKET)
	{
		Logging->writeError("Error while creating socket: INVALID_SOCKET\n");
		return false;
	}
	if(bind(_server,(sockaddr*)&addr,sizeof(addr))!=0)
	{
		Logging->writeError("Failed to bind to localhost on port %s\n",port.c_str());
		return false;
	}
	Logging->writeDebug("Successfully connected on port %s\n",port.c_str());

	//Connecting to sql
	Logging->writeDebug("Connecting to SQL Server...\n");
	if(!_database->Connect(Settings::getValue("dbuser"),Settings::getValue("dbpass"),Settings::getValue("dbdatabase"),Settings::getValue("dbhost"),atoi(Settings::getValue("dbport").c_str())))
	{
		Logging->writeError("unable to connect to sql server: %s\n",_database->getLastError());
		return false;
	}
	Logging->writeDebug("Successfully connected to Database %s on host %s\n",Settings::getValue("dbdatabase").c_str(),Settings::getValue("dbhost").c_str());
	
	return true;

}
void TCPServer::run()
{
	if(listen(_server,10)!=0)
	{
		return;
	}
	Logging->writeDebug("Main routine started. Waiting for clients...\n");
	SOCKET client;
	sockaddr_in from;
	int fromlen=sizeof(from);
	while(true)
	{
		client=accept(_server, (struct sockaddr*)&from,&fromlen);
		Logging->writeDebug("Client connected!\n");
		
		Client c;
		c.socket = client;
		ThreadData threadData(c,this);		
#if _WIN32
		HANDLE thread = CreateThread( NULL, 0, ClientHandler, &threadData, 0, NULL);  
#else
		pthread_t thread;
		pthread_create (&thread, NULL, ClientHandler, &threadData);
#endif
	}
}


bool TCPServer::SendPacket(SOCKET client, uint8* data, int length)
{
	int result = send(client,reinterpret_cast<const char*>(data),length,0);
	return result != SOCKET_ERROR;
}

bool TCPServer::SendFile(SOCKET client, char* fileName)
{
	return true;
}

Ptr<IDatabase> TCPServer::getDatabase()
{
	return _database;
}

void TCPServer::shutdown()
{
	if(_database)
		_database->Disconnect();
}