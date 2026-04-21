/* Copyright 2003-2005 ROBLOX Corporation, All Rights Reserved */
#include "stdafx.h"

#include "util/standardout.h"
#include "rbx/Log.h"
#include "RbxFormat.h"

using namespace ARL;

bool StandardOut::allowPrintWarnings = true;

shared_ptr<StandardOut> StandardOut::singleton()
{
	static shared_ptr<StandardOut> standardOut(new StandardOut());
	return standardOut;
}

void StandardOut::print_exception(const boost::function0<void>& f, MessageType type, bool rethrow)
{
	try
	{
		f();
	}
	catch (const ARL::base_exception& e)
	{
		singleton()->print(type, e);
		if (rethrow)
			throw;
	}
}

void StandardOut::printf(MessageType type, const char* format, ...)
{
	va_list arg_list;
	va_start(arg_list, format);
	std::string message = ARL::vformat(format, arg_list);
	va_end(arg_list);
	print(type, message);
}



void StandardOut::print(MessageType type, const std::exception& e)
{
	print(type, e.what());
}



void StandardOut::print(MessageType type, const char* message)
{
	print(type, std::string(message));
}



void StandardOut::print(MessageType type, const std::string& message)
{
	if (ARL::Log::current())
	{
		switch (type)
		{
		case MESSAGE_ERROR:
			ARL::Log::current()->writeEntry(Log::Error, message.c_str());
			break;
		case MESSAGE_WARNING:
			ARL::Log::current()->writeEntry(Log::Warning, message.c_str());
			break;
		case MESSAGE_INFO:
			ARL::Log::current()->writeEntry(Log::Information, message.c_str());
			break;
		case MESSAGE_OUTPUT:
			// No more than 256 characters please!
			if (message.size() < 256)
				ARL::Log::current()->writeEntry(Log::Information, message.c_str());
			else
				ARL::Log::current()->writeEntry(Log::Information,message.substr(0, 256).c_str());
			break;
		case MESSAGE_SENSITIVE:
			ARL::Log::current()->writeEntry(Log::Warning, message.c_str());
			break;
        default:
            ARL::Log::current()->writeEntry(Log::Error,"Standard Message Out set with incorrect Message Type");
            break;
                
		}
	}

	if(!StandardOut::allowPrintWarnings && (MESSAGE_WARNING == type))
	{
		// Do nothing.
	}
	else if(!messageOut.empty())
	{
		StandardOutMessage msg(type, message.c_str());
		boost::mutex::scoped_lock lock(sync);
		messageOut(boost::cref(msg));
	}
}
