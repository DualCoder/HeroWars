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



#ifndef _MYSQL_H
#define _MYSQL_H

#include "IDatabase.h"
#include "mysql/mysql_connection.h"

#include "mysql/cppconn/driver.h"
#include "mysql/cppconn/exception.h"
#include "mysql/cppconn/resultset.h"
#include "mysql/cppconn/statement.h"

class MySQL : public IDatabase
{
public:
	MySQL();
	~MySQL();
	bool Connect(string username, string password, string database, string host, int port = 3306);
	void Disconnect();
	void* Query(string query);
	bool FetchArray(void* queryResult, map<string,string>* result);
	void* FetchRow(void* queryResult);
	const char* getLastError();
	const char* escapeString(const char* input);

private:
	void setErrorMessage(const char* error);
	sql::Driver *_driver;  
	sql::Connection *_connection;
	sql::Statement *_statement;
	

	const char* _lastError;
};

#endif  //_MYSQL_H
