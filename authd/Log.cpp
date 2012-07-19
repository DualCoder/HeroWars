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
#include "Log.h"

Log* Log::_instance;

Log::Log()
{
	_stream = &cout;
	_logLevel = LT_HIGH;
	_instance = NULL;
}

Log::~Log()
{
	if(_stream != &cout) delete _stream;
}

void Log::setStream(ostream* stream)
{
	if(_stream != &cout) delete _stream;
	_stream = stream;
}

void Log::setOutputFile(const char* path)
{
	ofstream *filestream = new ofstream();
	filestream->open(path);
	setStream(filestream);
}

void Log::writeDate()
{
	GetSystemTime(&_time);	
	_snprintf_s(_buffer,LOG_BUFFER_SIZE,"[%02d/%02d/%02d %02d:%02d:%02d:%03d]",_time.wDay,
		_time.wMonth,
		_time.wYear,
		_time.wHour,
		_time.wMinute,
		_time.wSecond,
		_time.wMilliseconds);
	operator<<(*_stream,_buffer);
}

Log* Log::getInstance()
{
	if(_instance == NULL)
	{
		_instance = new Log();
	}
	return _instance;
}

void Log::writeLine(const char* buffer, ...)
{
	va_list al;
	va_start(al,buffer);
	va_start(al,buffer);
	vsprintf_s(_buffer,LOG_BUFFER_SIZE,buffer,al);
	operator<<(*_stream,_buffer);
	va_end(al);
}

void Log::writeDebug(const char* buffer, ...)
{
	if(_logLevel != LT_HIGH)
		return;

	va_list al;
	writeDate();
	va_start(al,buffer);
	operator<<(*_stream,"[DEBUG] ");
	vsprintf_s(_buffer,LOG_BUFFER_SIZE,buffer,al);
	operator<<(*_stream,_buffer);
	va_end(al);
}

void Log::writeError(const char* buffer, ...)
{
	if(_logLevel == LT_NONE)
		return;
	
	va_list al;
	writeDate();
	va_start(al,buffer);
	operator<<(*_stream,"[ERROR] ");
	vsprintf_s(_buffer,LOG_BUFFER_SIZE,buffer,al);
	operator<<(*_stream,_buffer);
	va_end(al);
}

void Log::clear()
{
	_stream->clear();
}

void Log::setLogLevel(LogType level)
{
	_logLevel = level;
}