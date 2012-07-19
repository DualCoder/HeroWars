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
#ifndef _CRYPT_H
#define _CRYPT_H

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE 1
#endif

#include "crypt/md5.h"

class Crypt
{
public:
	static char* encryptAES(char* input, char* password);
	static char* decryptAES(char* input, char* password);
	static string MD5(string input);
};

#endif  //_CRYPT_H
