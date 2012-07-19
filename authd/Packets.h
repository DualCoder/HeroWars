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
#ifndef _PACKETS_H
#define _PACKETS_H


enum PacketType : uint8
{
	PKT_HandshakeReq		=	0x01,
	PKT_HandshakeRes		=	0x02,
	PKT_AuthReq				=	0x03,
	PKT_AuthRes				=	0x04,

	PKT_Logout				=	0xFF
};

enum AuthResult : uint8
{
	AUR_OK					=	0x00,
	AUR_AuthFailed			=	0xFF,
};

enum HandshakeResult : uint8
{
	HSK_OK					=	0x00,
	HSK_WrongVersion		=	0x01,
	HSK_Unknown				=	0xFF,
};


//Packet header
typedef struct _PacketHeader
{
	_PacketHeader()
	{
		sessionHash = 0x00000000;
	}
	PacketType packetType;
	uint32 sessionHash;	
} PacketHeader;


//Handshake packets
typedef struct _HandshakeReq
{
	PacketHeader header;
	char version[33];	//md5 length (32 characters) + null terminating char (\0)
} HandshakeReq;

typedef struct _HandshakeRes
{
	_HandshakeRes()
	{
		header.packetType = PKT_HandshakeRes;
	}
	PacketHeader header;
	HandshakeResult handshakeResult;	// 0x00 when ok, 0xFF when failed
} HandshakeRes;


//Authentification packets
typedef struct _AuthReq
{
	PacketHeader header;
	char username[33]; //md5 length (32 characters) + null terminating char (\0)
	char password[33]; //md5 length (32 characters) + null terminating char (\0)
} AuthReq;

typedef struct _AuthRes
{
	_AuthRes()
	{
		header.packetType = PKT_AuthRes;
	}
	PacketHeader header;
	AuthResult result;
} AuthRes;


//Logout packet
typedef struct _Logout
{
	_Logout()
	{
		header.packetType = PKT_Logout;
	}
	PacketHeader header;
} PacketLogout;

#endif