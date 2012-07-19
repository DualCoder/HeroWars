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
#ifndef _LOG_H
#define _LOG_H

#include <Windows.h>

#define LOG_BUFFER_SIZE 1024
#define DO_LOG

#ifdef DO_LOG
#define PDEBUG_LOG_LINE(pLogHandler,buffer,...) pLogHandler->debugLine(buffer,__VA_ARGS__)
#define VDEBUG_LOG_LINE(LogHandler,buffer,...) LogHandler.debugLine(buffer,__VA_ARGS__)
#define PDEBUG_LOG(pLogHandler,buffer,...) pLogHandler->debug(buffer,__VA_ARGS__)
#define VDEBUG_LOG(LogHandler,buffer,...) LogHandler.debug(buffer,__VA_ARGS__)
#else
#define PDEBUG_LOG_LINE(pLogHandler,buffer,...) 
#define VDEBUG_LOG_LINE(LogHandler,buffer,...)
#define PDEBUG_LOG(pLogHandler,buffer,...)
#define VDEBUG_LOG(LogHandler,buffer,...)
#endif

class Log
{
public:
	enum LogType 
	{
		LT_HIGH,
		LT_LOW,
		LT_NONE,
	};
	Log();
	~Log();
	static Log* getInstance();
	
	void setStream(ostream* stream);
	void setOutputFile(const char* path);
	void writeDate();
	void writeLine(const char* buffer, ...);
	void writeDebug(const char* buffer, ...);
	void writeError(const char* buffer, ...);
	void clear();
	void setLogLevel(LogType level);


private:
	static Log* _instance;
	LogType _logLevel;
	ostream* _stream;
	SYSTEMTIME _time;
	char _buffer[LOG_BUFFER_SIZE]; 
};

#endif  //_LOG_H
