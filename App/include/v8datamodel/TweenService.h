/* Copyright 2003-2009 ROBLOX Corporation, All Rights Reserved */

#pragma once

#include "v8tree/Instance.h"
#include "v8tree/Service.h"
#include "util/HeartbeatInstance.h"
#include "util/SteppedInstance.h"
#include "v8datamodel/GuiObject.h"
#include "util/TweenInfo.h"
#include "v8datamodel/TweenBase.h"

namespace ARL
{

	extern const char* const sTweenService;
	class TweenService
		: public DescribedCreatable<TweenService, Instance, sTweenService, Reflection::ClassDescriptor::INTERNAL>
		, public Service
		, public HeartbeatInstance
		, public IStepped
	{
	private:
		typedef DescribedCreatable<TweenService, Instance, sTweenService, Reflection::ClassDescriptor::INTERNAL> Super;

	public:
		TweenService();

		shared_ptr<Instance> createTween(shared_ptr<Instance> instance, TweenInfo tweenInfo, shared_ptr<const Reflection::ValueTable> propertyTable);
		float getValue(float alpha, TweenInfo::TweenEasingStyle easingStyle, TweenInfo::TweenEasingDirection easingDirection);
		void addTweeningObject(boost::weak_ptr<GuiObject> guiObject);
		void addTweeningInstance(boost::weak_ptr<Instance> instance);
		void addTweenCallback(boost::function<void(TweenInfo::TweenStatus)> tweenCallbackFunc, TweenInfo::TweenStatus tweenStatusForCallback);
		// float update(double);
		// float updateGenericTweens(double);
		// void removeExistingTween(shared_ptr<ARL::Tween>);
		// void removeTweenFromUpdate(shared_ptr<ARL::Tween>);
		// void addTweenForUpdate(shared_ptr<ARL::Tween>);
		// static bool validateTweenProperty(shared_ptr<ARL::Instance> instance, shared_ptr<const Reflection::ValueTable> propertyTable);
		static float tweenInterpolateRawIn(TweenInfo::TweenEasingStyle easingStyle, const float& factor);
		static float tweenInterpolateRaw(TweenInfo::TweenEasingDirection easingDirection, TweenInfo::TweenEasingStyle easingStyle, const float& alpha);
		static float tweenInterpolate(TweenInfo::TweenEasingDirection easingDirection, TweenInfo::TweenEasingStyle easingStyle, float startValue,
			float endValue, const float& elapsedTime, const float& totalTime);


		bool tweenStep(const double& timeStep, bool& isPaused);

		static void customSetOpt(shared_ptr<Instance> instance, std::string prop, Reflection::Variant value);

		std::map<std::string, shared_ptr<TweenBase>> tweens;

	protected:
		typedef std::set<boost::weak_ptr<GuiObject>> TweeningObjectsList;

		TweeningObjectsList tweeningObjects;

		/*override*/ void onHeartbeat(const Heartbeat& heartbeatEvent);
		/*override*/ void onServiceProvider(ServiceProvider* oldProvider, ServiceProvider* newProvider);

	private:
		// static G3D::CoordinateFrame interpolateCFrame(const G3D::CoordinateFrame& start, const G3D::CoordinateFrame& end, float& factor, const
		// TweenInfo::TweenEasingStyle& style, float totalTime, float elapsedTime);
		static void UpdateTween(shared_ptr<ARL::TweenBase> tween, shared_ptr<const Reflection::ValueArray> arr,
			boost::function<void(shared_ptr<Instance>, std::string, Reflection::Variant)> updateFunc, float timeStep);

		typedef std::vector<std::pair<boost::function<void(TweenInfo::TweenStatus)>, TweenInfo::TweenStatus>> TweenCallbacks;
		typedef std::set<boost::weak_ptr<Instance>> TweeningInstanceList;

		TweeningInstanceList tweeningInstances;
		static Reflection::Variant TweenInterpolate(TweenInfo::TweenEasingDirection direction, TweenInfo::TweenEasingStyle style, float elapsedTime,
			float totalTime, const Reflection::Variant& startValue, const Reflection::Variant& endValue);

		TweenCallbacks tweenCallbacks;

		void update(const double step);

		void updateInstance(const double step);


		// IStepped
		virtual void onStepped(const Stepped& event);
	};
} // namespace ARL