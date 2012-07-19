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
#include "PostgreSQL.h"

bool PostgreSQL::Connect(string username, string password, string database, string host, int port)
{
	//TODO
	return true;
}

void PostgreSQL::Disconnect()
{
	//TODO
}

void* PostgreSQL::Query(string query)
{
	//TODO
	return NULL;
}

bool PostgreSQL::FetchArray(void* queryResult, map<string,string>* result)
{
	//TODO
	return true;
}

void* PostgreSQL::FetchRow(void* queryResult)
{
	//TODO
	return NULL;
}

const char* PostgreSQL::getLastError()
{
	//TODO
	return NULL;
}

const char* PostgreSQL::escapeString(const char* input)
{
	//TODO
	return NULL;
}