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
#include "AuthHandler.h"
#include "User.h"
#include "IDatabase.h"

AuthHandler::AuthHandler(Ptr<IDatabase> database)
{
	_database = database;
}
bool AuthHandler::authentificate(char* username, char* password,uint32 &sessionKey)
{
	//Session key is generated like this:
	// sessionKey = hash(username + password + time)
	string query("SELECT * FROM users WHERE username='");
	query.append(username);
	query.append("';");
	void* result = _database->Query(query);
	if(!result) return false;

	map<string, string> data;
	if(!_database->FetchArray(result,&data))
		return false;
	if(strcmp(data["password"].c_str(),password) != 0)
		return false;
	
	//Generate session key
	char buffer[1024];
	std::hash<std::string> hash;

	time_t rawtime;
	time ( &rawtime );
	char* time = ctime(&rawtime);
	sprintf(buffer,"%s%s%s",username,password,time);
	
	uint32 key = hash(buffer);
	sessionKey = key;

	return true;
}

bool AuthHandler::userConnected(uint32 sessionHash)
{
	for(int i=0;i<_users.size();i++)
	{
		if(_users[i]->sessionHash == sessionHash)
			return true;
	}
	return false;
}

bool AuthHandler::getUser(uint32 sessionHash, User** user)
{
	for(int i=0;i<_users.size();i++)
	{
		if(_users[i]->sessionHash == sessionHash)
		{
			user = &_users[i];
			return true;
		}
	}
	return false;
}

void AuthHandler::removeUser(uint32 sessionHash)
{
	for(int i=0;i<_users.size();i++)
	{
		if(_users[i]->sessionHash == sessionHash)
		{
			_users.erase(_users.begin()+i);
			return;
		}
	}
}

