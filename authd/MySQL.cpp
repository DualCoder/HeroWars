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
#include "MySQL.h"
#include "Log.h"

MySQL::MySQL()
{
	_connection = 0;
	_driver = 0;
	_lastError = 0;
	_statement = 0;
}
MySQL::~MySQL()
{
	//Because of the strange design of Mysql connector we have to handle deletion of objects by ourselves
	if(_connection)
	{
		if(!_connection->isClosed())
			_connection->close();
		delete _connection;
		_connection = 0;
	}
	if(_lastError)
	{
		delete[] _lastError;
		_lastError = 0;
	}
	if(_statement)
	{
		delete _statement;
		_statement = 0;
	}
}
bool MySQL::Connect(string username, string password, string database, string host, int port)
{
	//I know try/catch in C++ is worser than killing people but its how mysql api is designed
	try
	{
		_driver = get_driver_instance();
		_connection = _driver->connect(host,username,password);
		_connection->setSchema(database);
	}
	catch(sql::SQLException &e)
	{
		if(_connection && !_connection->isClosed())
			_connection->close();

		setErrorMessage(e.what());
		return false;
	}

	return true;
}

void MySQL::Disconnect()
{
	if(_connection && !_connection->isClosed())
		_connection->close();
}

void* MySQL::Query(string query)
{
	try
	{
		if(_connection == NULL) return NULL;
		_statement = _connection->createStatement();
		return _statement->executeQuery(query.c_str());
	}
	catch(sql::SQLException &e)
	{
		setErrorMessage(e.what());
		Logging->writeError("MySQL Error: %s",_lastError);
		return NULL;
	}
}

bool MySQL::FetchArray(void* queryResult, map<string,string>* result)
{
	//map<char*,void*> result;	
	string columnName;
	string columnValue;
	sql::ResultSet *resultSet = reinterpret_cast<sql::ResultSet*>(queryResult);
	try
	{

		if(!resultSet->next()) return false;
		
		char buffer[1024];
		sql::ResultSetMetaData* metaData = resultSet->getMetaData();
		for(uint32 i=0;i<metaData->getColumnCount();i++)
		{
			columnName = metaData->getColumnName(i+1);
			switch(metaData->getColumnType(i+1))
			{
			case sql::DataType::CHAR:
			case sql::DataType::VARCHAR:
			case sql::DataType::LONGVARCHAR:
			case sql::DataType::TIME:
			case sql::DataType::TIMESTAMP:
				columnValue = resultSet->getString(i+1);
				break;
			case sql::DataType::INTEGER:
			case sql::DataType::BIGINT:
			case sql::DataType::SMALLINT:
			case sql::DataType::NUMERIC:
				_itoa(resultSet->getInt(i+1),buffer,10);
				columnValue = buffer;
				break;
			case sql::DataType::DECIMAL:
			case sql::DataType::DOUBLE:
			case sql::DataType::REAL:

				sprintf(buffer,"%f",resultSet->getDouble(i+1));
				columnValue = buffer;
				break;
			}
			result->insert(pair<string,string>(columnName,columnValue));
		}

	}
	catch(sql::SQLException &e)
	{
		setErrorMessage(e.what());
		Logging->writeError("MySQL Error: %s",_lastError);
		return false;
	}
	return true;
}

void* MySQL::FetchRow(void* queryResult)
{
	//TODO
	return NULL;
}

const char* MySQL::getLastError()
{
	return _lastError;
}

const char* MySQL::escapeString(const char* input)
{	
	try
	{
		sql::mysql::MySQL_Connection* mysql_conn = dynamic_cast<sql::mysql::MySQL_Connection*>(_connection); 
		return mysql_conn->escapeString(input).c_str();
	}
	catch(sql::SQLException &e)
	{
		setErrorMessage(e.what());
		Logging->writeError("MySQL Error: %s",_lastError);
		return false;
	}
}

void MySQL::setErrorMessage(const char* error)
{
	const size_t len = strlen(error);
	char * tmp_error = new char[len + 1];
	strncpy(tmp_error, error, len);
	tmp_error[len] = '\0'; // I'm paranoid, maybe someone has changed something in _lastError :-)
	if(_lastError) delete[] _lastError;
	_lastError = tmp_error;
}