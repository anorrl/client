/* Copyright 2003-2005 ROBLOX Corporation, All Rights Reserved */

#pragma once

#include "v8datamodel/JointInstance.h"
#include "v8datamodel/Workspace.h"
#include "humanoid/Humanoid.h"
#include "v8world/Primitive.h"
#include "v8world/Assembly.h"
#include "arl/rbxTime.h"

DYNAMIC_FASTINT(ActionStationDebounceTime)

namespace ARL {


template<class Base>
class ActionStation : public Base
{
	private:			
		typedef Base Super;
	protected:
		Time sleepTime;
		Time debounceTime;

		bool sleepTimeUp() const {
			return (Time::now<Time::Fast>() - sleepTime).seconds() > 3.0;
		}

		bool debounceTimeUp() const {
				return (Time::now<Time::Fast>() - debounceTime).seconds() > DFInt::ActionStationDebounceTime;
		}

		// Instance
		// TODO: Ultra mega super hack - setName is setting the internal Primitive::sizeMultiplier
		void setName(const std::string& value) {
			Super::setName(value);
			this->getPartPrimitive()->setSizeMultiplier(Primitive::SEAT_SIZE);
		}

	public:
		ActionStation() : sleepTime(Time::now<Time::Fast>() - Time::Interval(4.0)), debounceTime(Time::now<Time::Fast>())
		{
			ARLASSERT(this->sleepTimeUp());
			ARLASSERT(this->getPartPrimitive());
			this->getPartPrimitive()->setSizeMultiplier(Primitive::SEAT_SIZE);
		}

		virtual ~ActionStation() {}
};


} // namespace