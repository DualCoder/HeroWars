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



#ifndef _POSTGRESQL_H
#define _POSTGRESQL_H

#include "IDatabase.h"

class PostgreSQL : public IDatabase
{
public:
	bool Connect(string username, string password, string database, string host, int port = 3306);
	void Disconnect();
	void* Query(string query);
	bool FetchArray(void* queryResult, map<string,string>* result);
	void* FetchRow(void* queryResult);
	const char* getLastError();
	const char* escapeString(const char* input);
};

#endif  //_POSTGRESQL_H
