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
#ifndef _MAIN_H
#define _MAIN_H

//Disable deprecation
#define _CRT_SECURE_NO_DEPRECATE 
#define _SCL_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

#define Ptr shared_ptr
#define newPtr make_shared
#define Logging Log::getInstance()

#include <memory>
#include <string>
#include <algorithm>
#include <cctype>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#if _WIN32
#include <winsock2.h>
#endif
#include <time.h>
#include "functions.h"

using namespace std;

#define SERVER_VERSION "0.0.1"

#pragma comment(lib, "Ws2_32.lib")

typedef char int8;
typedef short int16;
typedef int int32;
typedef long long int64; 
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64; 

typedef struct _client
{
	_client()
	{
		sessionHash = 0;
		socket = 0;
	}
	SOCKET socket;
	uint32 sessionHash;
} Client;

#endif  //_MAIN_H
