/* Copyright 2003-2005 ROBLOX Corporation, All Rights Reserved */

#pragma once

#include "V8Tree/Instance.h"
#include "V8Tree/Service.h"
#include "V8DataModel/InputObject.h"

namespace ARL {

	class VirtualHardwareDevice;
	class DataModel;

	extern const char* const sVirtualUser;
	class VirtualUser 
		: public DescribedCreatable<VirtualUser, Instance, sVirtualUser, Reflection::ClassDescriptor::INTERNAL_LOCAL>
		, public Service
	{
	private:
		boost::scoped_ptr<VirtualHardwareDevice> virtualHardwareDevice;	
		typedef DescribedCreatable<VirtualUser, Instance, sVirtualUser, Reflection::ClassDescriptor::INTERNAL_LOCAL> Super;

		std::stringstream recording;	// the script code when recording
		rbx::signals::scoped_connection recordingConnection;

		ARL::Time lastEventTime;

	public:
		VirtualUser();
		// The following functions are used to automate user input (used by test scripts, for example)
		void pressKey(std::string key);
		void setKeyDown(std::string key);
		void setKeyUp(std::string key);
		void clickButton1(ARL::Vector2 position, ARL::CoordinateFrame camera);
		void clickButton2(ARL::Vector2 position, ARL::CoordinateFrame camera);
		void button1Down(ARL::Vector2 position, ARL::CoordinateFrame camera);
		void button2Down(ARL::Vector2 position, ARL::CoordinateFrame camera);
		void button1Up(ARL::Vector2 position, ARL::CoordinateFrame camera);
		void button2Up(ARL::Vector2 position, ARL::CoordinateFrame camera);
		void moveMouse(ARL::Vector2 position, ARL::CoordinateFrame camera);
		void captureInputDevice();

		void startRecording();
		std::string stopRecording();

	protected:
		/*override*/ void onServiceProvider(ServiceProvider* oldProvider, ServiceProvider* newProvider);
	private:
		void onInputObject(const shared_ptr<InputObject>& event);
		void sendMouseEvent(InputObject::UserInputType eventType, InputObject::UserInputState eventState, ARL::Vector2 position, ARL::CoordinateFrame camera);
		KeyCode convert(const std::string& key);
		void writeWait();
		void writeKey(const char* func, const shared_ptr<InputObject>& event);
		void writeMouse(const char* func, const shared_ptr<InputObject>& event);
		DataModel* getDataModel();
	};

}	// namespace ARL
