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
#include "PacketHandler.h"
#include "ISocket.h"
#include "Log.h"
#include "Crypt.h"

void stringtolower(char* input)
{
	int i = 0;
	while (input[i])
	{
		input[i] = tolower(input[i]);
		i++;
	}
}

PacketHandler::PacketHandler(ISocket* tcpServer)
{
	_tcpServer = tcpServer;
	_authHandler = newPtr<AuthHandler>(_tcpServer->getDatabase());
}

bool PacketHandler::HandlePacket(Client* client, char* packet)
{
	switch(packet[0])
	{
	case PKT_HandshakeReq:
		return handleHandshake(client, packet);
		break;
	case PKT_AuthReq:
		return handleAuth(client,packet);
		break;
	case PKT_Logout:
		return handleDisconnect(client, packet);
		break;
	}
	return false;
}
bool PacketHandler::handleHandshake(Client* client, char* packet)
{
	HandshakeReq* handshakeReq = reinterpret_cast<HandshakeReq*>(packet);
	HandshakeRes response;
	string s = Crypt::MD5(SERVER_VERSION);
	stringtolower(handshakeReq->version);
	if(strcmp(handshakeReq->version,s.c_str()) != 0)
	{
		Logging->writeDebug("Handshake failed. Probably wrong client version.\n");
		response.handshakeResult = HSK_WrongVersion;
	}
	else
	{
		Logging->writeDebug("Handshake successful\n");
		response.handshakeResult = HSK_OK;
	}

	return _tcpServer->SendPacket(client->socket,reinterpret_cast<uint8*>(&response),sizeof(HandshakeRes));
}
bool PacketHandler::handleAuth(Client* client, char* packet)
{
	uint32 sessionHash = 0;
	AuthReq* authRequest = reinterpret_cast<AuthReq*>(packet);
	AuthRes authResponse;
	
	stringtolower(authRequest->username);
	stringtolower(authRequest->password);
	bool authSuccess = _authHandler->authentificate(authRequest->username,authRequest->password,sessionHash);
	authResponse.result = authSuccess ? AUR_OK : AUR_AuthFailed;
	authResponse.header.sessionHash = sessionHash;
	_tcpServer->SendPacket(client->socket,reinterpret_cast<uint8*>(&authResponse),sizeof(authResponse));

	if(authSuccess)
	{ 
		Logging->writeDebug("User authentificated. SessionHash: %u\n",sessionHash);
		client->sessionHash = sessionHash;
	}
	else
		Logging->writeDebug("User authentificated failed\n");

	return true;
}

bool PacketHandler::handleDisconnect(Client* client, char* packet)
{
	PacketLogout* logoutPacket = reinterpret_cast<PacketLogout*>(packet);
	_authHandler->removeUser(logoutPacket->header.sessionHash);
	client->sessionHash = 0;
	Logging->writeDebug("User logged out\n");
	return true;
}

void PacketHandler::removeUser(uint32 sessionHash)
{
	if(sessionHash == NULL) return;
	_authHandler->removeUser(sessionHash);
}