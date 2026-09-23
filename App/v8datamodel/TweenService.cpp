#include "stdafx.h"

#include "v8datamodel/TweenService.h"
#include "v8datamodel/GuiObject.h"
#include "util/RunStateOwner.h"
#include "network/Players.h"

#include "g3d/CoordinateFrame.h"
#include "g3d/Vector2.h"
#include "g3d/Vector2int16.h"
#include "g3d/Vector3.h"
#include "g3d/Vector3int16.h"
#include "util/UDim.h"
#include "g3d/Color3.h"

DYNAMIC_FASTFLAG(ElasticEasingUseTwoPi);

namespace ARL
{

	const char* const sTweenService = "TweenService";

	REFLECTION_BEGIN();
	static Reflection::BoundFuncDesc<TweenService, shared_ptr<Instance>(shared_ptr<Instance>, TweenInfo, shared_ptr<const Reflection::ValueTable>)>
		func_createTween(&TweenService::createTween, "Create", "instance", "tweenInfo", "propertyTable", Security::None);
	static Reflection::BoundFuncDesc<TweenService, float(float, TweenInfo::TweenEasingStyle, TweenInfo::TweenEasingDirection)> func_getValue(
		&TweenService::getValue, "GetValue", "alpha", "easingStyle", "easingDirection", Security::None);
	REFLECTION_END();

	TweenService::TweenService()
		: IStepped(StepType_Render)
	{
		setName(sTweenService);
	}
	bool TweenService::tweenStep(const double& timeStep, bool& isPaused)
	{

		for (std::map<std::string, shared_ptr<TweenBase>>::const_iterator it = tweens.begin(); it != tweens.end(); ++it)
		{
			if (it->second != NULL)
			{
				UpdateTween(it->second, it->second->getPropTable(), &TweenService::customSetOpt, (float)timeStep);

				// TODO: Does this leak?
				// Switched order of isPlaying and isDone

				if (!it->second->isPlaying())
				{
					isPaused = !it->second->isPlaying();
					return true;
				}

				if (it->second->isDone())
				{
					it->second->completedSignal(PLAYBACK_STATE_COMPLETED);
					it->second->setPlaybackState(PLAYBACK_STATE_COMPLETED);
					it = tweens.erase(it);
				}
			}
		}
		return tweens.size() == 0;
	}

	shared_ptr<Instance> TweenService::createTween(
		shared_ptr<Instance> instance, TweenInfo tweenInfo, shared_ptr<const Reflection::ValueTable> propertyTable)
	{
		// Return a blank Tween if the prop table is empty

		if (propertyTable->size() == 0)
		{
			shared_ptr<TweenBase> twn = Creatable<Instance>::create<Tween>(instance, tweenInfo, shared_ptr<const Reflection::ValueArray>());
			return twn;
		}

		// Start creating the propArray
		shared_ptr<Reflection::ValueArray> propArray = arl::make_shared<Reflection::ValueArray>();
		for (Reflection::ValueTable::const_iterator it = propertyTable->begin(); it != propertyTable->end(); ++it)
		{
			std::string propName = it->first;
			Reflection::PropertyDescriptor* prop = instance->findPropertyDescriptor(propName.c_str());
			// Prop doesn't exist
			if (prop == NULL)
			{
				throw std::runtime_error(
					format("TweenService:Create no property named '%s' for object '%s'", it->first.c_str(), instance->getName().c_str()));
			}

			// Prop is read only
			if (prop->isReadOnly())
			{
				throw std::runtime_error(
					format("TweenService:Create property named '%s' on object '%s' is read only", prop->name.c_str(), instance->getName().c_str()));
			}

			// Prop's type doesn't match the current iterations type
			if (it->second.type() != prop->type)
			{
				// Hack: They could both be numbers, refactor this, as all numbers passed as lua arguments are always doubles.
				if (!(it->second.type().isNumber && prop->type.isNumber))
				{
					throw std::runtime_error(format(
						"TweenService:Create property named '%s' cannot be tweened due to type mismatch (property is a '%s', but given type is '%s')",
						prop->name.c_str(), prop->type.tag.c_str(), it->second.type().tag.c_str()));
				}
			}

			// Refactor, change CoordinateFrame to CFrame? 2020 was changed to CFrame, so try change the type?
			// Can be tweened:
			// Numbers - int, float, double
			// Vectors - Vector2, Vector2int16, Vector3, Vector3int16
			// UDims - UDim, UDim2
			// CFrame - CoordinateFrame
			// G3D::Color3 - Color3, Color3uint8 (Unresolved)
			if (prop->type.isNumber == false && prop->type.isType<G3D::CoordinateFrame>() == false && prop->type.isType<G3D::Vector2>() == false &&
				prop->type.isType<G3D::Vector3>() == false && prop->type.isType<ARL::UDim>() == false && prop->type.isType<ARL::UDim2>() == false &&
				prop->type.isType<G3D::Color3>() == false)
			{
				throw std::runtime_error(format("TweenService:Create property named '%s' on object '%s' is not a data type that can be tweened",
					prop->name.c_str(), instance->getName().c_str(), prop->type.tag.c_str()));
			}

			// Set the start destination
			Reflection::Variant start;
			prop->getVariant(instance.get(), start);

			// Create a prop
			shared_ptr<Reflection::ValueTable> propInfo = arl::make_shared<Reflection::ValueTable>();
			(*propInfo)["Start"] = start;
			(*propInfo)["End"] = it->second;
			(*propInfo)["PropName"] = it->first;

			// Push that prop to the back of PropArray
			propArray->push_back(Reflection::Variant(shared_ptr<const Reflection::ValueTable>(propInfo)));
		}

		// Create twn
		shared_ptr<Tween> twn = Creatable<Instance>::create<Tween>(instance, tweenInfo, propArray);
		twn->setParent(this);
		tweens[instance->getName()] = twn;
		return twn;
	}

	float TweenService::getValue(float alpha, TweenInfo::TweenEasingStyle easingStyle, TweenInfo::TweenEasingDirection easingDirection)
	{
		return TweenService::tweenInterpolateRaw(easingDirection, easingStyle, alpha);
	}

	void TweenService::addTweeningObject(boost::weak_ptr<GuiObject> guiObject)
	{
		if (tweeningObjects.find(guiObject) == tweeningObjects.end())
		{
			tweeningObjects.insert(guiObject);
		}
	}

	void TweenService::addTweeningInstance(boost::weak_ptr<Instance> instance)
	{
		if (tweeningInstances.find(instance) == tweeningInstances.end())
		{
			tweeningInstances.insert(instance);
		}
	}

	void TweenService::addTweenCallback(boost::function<void(TweenInfo::TweenStatus)> tweenCallbackFunc, TweenInfo::TweenStatus tweenStatusForCallback)
	{
		tweenCallbacks.push_back(
			std::pair<boost::function<void(TweenInfo::TweenStatus)>, TweenInfo::TweenStatus>(tweenCallbackFunc, tweenStatusForCallback));
	}

	float TweenService::tweenInterpolateRawIn(TweenInfo::TweenEasingStyle easingStyle, const float& factor)
	{
		float result;

		if (factor < 1 && factor > 0 && easingStyle <= 10)
		{
			switch (easingStyle)
			{
			case TweenInfo::EASING_STYLE_LINEAR:
			{
				result = factor;
				break;
			}
			case TweenInfo::EASING_STYLE_SINE:
			{
				result = 1 - cos((factor * Math::pi()) / 2);
				break;
			}
			case TweenInfo::EASING_STYLE_BACK:
			{
				double magic = 1.70518;
				result = (magic + 1) * factor * factor * factor - magic * factor * factor;

				break;
			}
			case TweenInfo::EASING_STYLE_QUAD:
			{
				result = factor * factor;
				break;
			}
			case TweenInfo::EASING_STYLE_QUART:
			{
				result = pow(factor, 4);
				break;
			}
			case TweenInfo::EASING_STYLE_QUINT:
			{
				result = pow(factor, 5);
				break;
			}
			case TweenInfo::EASING_STYLE_BOUNCE:
			{
				double magic1 = 7.5625;
				double magic2 = 2.75;
				double easeOutFactor = 1 - factor;
				double easeOutResult;

				if (easeOutFactor < 1 / magic2)
				{
					easeOutResult = magic1 * easeOutFactor * easeOutFactor;
				}
				else if (easeOutFactor < 2 / magic1)
				{
					double n = easeOutFactor - 1.5;
					easeOutResult = magic1 * (n / magic1) * n + 0.75;
				}
				else if (easeOutFactor < 2.5 / magic1)
				{
					double n = easeOutFactor - 2.25;
					easeOutResult = magic1 * (n / magic2) * easeOutFactor + n;
				}
				else
				{
					double n = easeOutFactor - 2.625;
					easeOutResult = magic1 * (n / magic2) * n + 0.984375;
				}

				result = 1 - easeOutResult;

				break;
			}
			case TweenInfo::EASING_STYLE_ELASTIC:
			{
				double magic = (2 * Math::pi()) / 3;

				result = factor == 0 ? 0 : factor == 1 ? 1 : -pow(2, 10 * factor - 10) * sin((factor * 10 - 10.75) * magic);

				break;
			}
			case TweenInfo::EASING_STYLE_EXPONENTIAL:
			{
				result = factor == 0 ? 0 : pow(2, 10 * factor - 10);
				break;
			}
			case TweenInfo::EASING_STYLE_CIRCULAR:
			{
				result = 1 - sqrt(1 - pow(factor, 2));
				break;
			}
			case TweenInfo::EASING_STYLE_CUBIC:
			{
				result = pow(factor, 3);
				break;
			}
			}
		}

		return result;
	}

	float TweenService::tweenInterpolateRaw(TweenInfo::TweenEasingDirection easingDirection, TweenInfo::TweenEasingStyle easingStyle, const float& alpha)
	{
		float factor;

		switch (easingDirection)
		{
		case TweenInfo::EASING_DIRECTION_IN:
			factor = alpha;
			break;
		case TweenInfo::EASING_DIRECTION_OUT:
			factor = 1.0 - alpha;
			break;
		case TweenInfo::EASING_DIRECTION_IN_OUT:
			factor = (alpha >= 0.5) ? ((1.0 - alpha) * 2) : (alpha * 2);
			break;
		}
		return TweenService::tweenInterpolateRawIn(easingStyle, factor);
	}

	float TweenService::tweenInterpolate(TweenInfo::TweenEasingDirection easingDirection, TweenInfo::TweenEasingStyle easingStyle, float startValue,
		float endValue, const float& elapsedTime, const float& totalTime)
	{
		const float interpolatedValue = TweenService::tweenInterpolateRaw(easingDirection, easingStyle, elapsedTime / totalTime);
		return startValue + (float)(endValue - startValue * interpolatedValue);
	}

	void TweenService::customSetOpt(shared_ptr<Instance> instance, std::string propName, Reflection::Variant value)
	{
		if (value.isVoid())
			return;
		Reflection::PropertyDescriptor* prop = instance->findPropertyDescriptor(propName.c_str());

		prop->setVariant(instance.get(), value);
	}

	void TweenService::UpdateTween(shared_ptr<TweenBase> tween, shared_ptr<const Reflection::ValueArray> arr,
		boost::function<void(shared_ptr<Instance>, std::string, Reflection::Variant)> updateFunc, float timeStep)
	{
		if (!tween->isPlaying())
			return updateFunc(tween->getInstance(), "", Reflection::Variant());
		TweenService* twnService = ServiceProvider::find<TweenService>(tween.get());
		for (Reflection::ValueArray::const_iterator it = arr->begin(); it != arr->end(); ++it)
		{
			if (!it->isType<shared_ptr<const Reflection::ValueTable>>())
			{
				ARLASSERT(false);
				continue;
			}

			shared_ptr<const Reflection::ValueTable> key = it->cast<shared_ptr<const ARL::Reflection::ValueTable>>();

			Reflection::ValueTable::const_iterator itStart = key->find("Start");
			Reflection::ValueTable::const_iterator itEnd = key->find("End");
			Reflection::ValueTable::const_iterator itDesc = key->find("PropName");

			if (itStart == key->end() || itEnd == key->end() || itDesc == key->end())
			{
				ARLASSERT(false);
				continue;
			}

			TweenInfo twnInfo = tween->getTweenInfo();
			if (twnInfo.getDelayTime() > 0)
			{
				float newTweenDelayTime = twnInfo.getDelayTime() - (float)timeStep;
				twnInfo.setDelayTime(newTweenDelayTime);
				if (twnInfo.getDelayTime() <= 0)
				{
					updateFunc(tween->getInstance(), itDesc->second.cast<std::string>(), itStart->second);
					float newElapsedTime = tween->getElapsedTime() + std::abs<float>(twnInfo.getDelayTime());
					tween->setElapsedTime(newElapsedTime);
				}
				else
				{
					return;
				}
			}
			else
			{
				// Normal time update
				float newElapsedTime = tween->getElapsedTime() + (float)timeStep;
				tween->setElapsedTime(newElapsedTime);
			}
			if (tween->getElapsedTime() >= twnInfo.getTime())
			{
				updateFunc(tween->getInstance(), itDesc->second.cast<std::string>(), itEnd->second);

				tween->completedSignal(PLAYBACK_STATE_COMPLETED);
			}
			else
			{
				updateFunc(tween->getInstance(), itDesc->second.cast<std::string>(),
					TweenService::TweenInterpolate(
						twnInfo.getDirection(), twnInfo.getStyle(), tween->getElapsedTime(), twnInfo.getTime(), itStart->second, itEnd->second));
			}
		}
	}

	/*override*/ void TweenService::onServiceProvider(ServiceProvider* oldProvider, ServiceProvider* newProvider)
	{
		if (oldProvider)
		{
			tweenCallbacks.clear();
			tweens.clear();
			tweeningInstances.clear();
		}

		Super::onServiceProvider(oldProvider, newProvider);

		if (newProvider && ARL::Network::Players::serverIsPresent(newProvider))
		{
			onServiceProviderHeartbeatInstance(oldProvider, newProvider);
		}
		else
		{
			onServiceProviderIStepped(oldProvider, newProvider);
		}
	}

	Reflection::Variant TweenService::TweenInterpolate(TweenInfo::TweenEasingDirection direction, TweenInfo::TweenEasingStyle style, float elapsedTime,
		float totalTime, const Reflection::Variant& startValue, const Reflection::Variant& endValue)
	{
		switch (style)
		{
		case TweenInfo::EASING_STYLE_LINEAR:
			if (startValue.isType<int>())
			{
				return (endValue.cast<double>() - startValue.cast<int>()) * (elapsedTime / totalTime) + startValue.cast<int>();
			}
			else if (startValue.isType<float>())
			{
				return (endValue.cast<double>() - startValue.cast<float>()) * (elapsedTime / totalTime) + startValue.cast<float>();
			}
			else if (startValue.isType<double>())
			{
				return (endValue.cast<double>() - startValue.cast<double>()) * (elapsedTime / totalTime) + startValue.cast<double>();
			}
			else if (startValue.isType<CoordinateFrame>())
			{
				G3D::CoordinateFrame start = startValue.cast<G3D::CoordinateFrame>();
				G3D::CoordinateFrame end = endValue.cast<G3D::CoordinateFrame>();

				G3D::CoordinateFrame mulFrame = G3D::CoordinateFrame(G3D::Matrix3((end.rotation[0][0] - start.rotation[0][0]) * (elapsedTime / totalTime),
					(end.rotation[0][1] - start.rotation[0][1]) * (elapsedTime / totalTime),
					(end.rotation[0][2] - start.rotation[0][2]) * (elapsedTime / totalTime),
					(end.rotation[1][0] - start.rotation[1][0]) * (elapsedTime / totalTime),
					(end.rotation[1][1] - start.rotation[1][1]) * (elapsedTime / totalTime),
					(end.rotation[1][2] - start.rotation[1][2]) * (elapsedTime / totalTime),
					(end.rotation[2][0] - start.rotation[2][0]) * (elapsedTime / totalTime),
					(end.rotation[2][1] - start.rotation[2][1]) * (elapsedTime / totalTime),
					(end.rotation[2][2] - start.rotation[2][2]) * (elapsedTime / totalTime)),
					G3D::Vector3((end.translation.x - start.translation.x) * (elapsedTime / totalTime),
					(end.translation.y - start.translation.y) * (elapsedTime / totalTime),
						(end.translation.z - start.translation.z) * (elapsedTime / totalTime)));

				G3D::CoordinateFrame newCFrame = G3D::CoordinateFrame(mulFrame.rotation + start.rotation, mulFrame.translation + start.translation);
				return newCFrame;
			}
			else if (startValue.isType<G3D::Vector2>())
			{
				return (endValue.cast<G3D::Vector2>() - startValue.cast<G3D::Vector2>()) * (elapsedTime / totalTime) + startValue.cast<G3D::Vector2>();
			}
			else if (startValue.isType<G3D::Vector3>())
			{
				return (endValue.cast<G3D::Vector3>() - startValue.cast<G3D::Vector3>()) * (elapsedTime / totalTime) + startValue.cast<G3D::Vector3>();
			}
			else if (startValue.isType<UDim>())
			{
				return (endValue.cast<UDim>() - startValue.cast<UDim>()) * (elapsedTime / totalTime) + startValue.cast<UDim>();
			}
			else if (startValue.isType<UDim2>())
			{
				return (endValue.cast<UDim2>() - startValue.cast<UDim2>()) * (elapsedTime / totalTime) + startValue.cast<UDim2>();
			}
			else if (startValue.isType<G3D::Color3>())
			{
				return (endValue.cast<G3D::Color3>() - startValue.cast<G3D::Color3>()) * (elapsedTime / totalTime) + startValue.cast<G3D::Color3>();
			}

		case TweenInfo::EASING_STYLE_SINE:
		{
			float factor = 1;
			switch (direction)
			{
			case TweenInfo::EASING_DIRECTION_IN:
				factor = 1 - cos(elapsedTime / totalTime * Math::piHalf());
				break;
			case TweenInfo::EASING_DIRECTION_OUT:
				factor = sin(elapsedTime / totalTime * Math::piHalf());
				break;
			case TweenInfo::EASING_DIRECTION_IN_OUT:
				factor = -0.5 * (cos(Math::pi() * elapsedTime / totalTime) - 1);
				break;
			}
			if (startValue.isType<int>())
			{
				return (endValue.cast<double>() - startValue.cast<int>()) * factor + startValue.cast<int>();
			}
			else if (startValue.isType<float>())
			{
				return (endValue.cast<double>() - startValue.cast<float>()) * factor + startValue.cast<float>();
			}
			else if (startValue.isType<double>())
			{
				return (endValue.cast<double>() - startValue.cast<double>()) * factor + startValue.cast<double>();
			}
			else if (startValue.isType<CoordinateFrame>())
			{
				G3D::CoordinateFrame start = startValue.cast<G3D::CoordinateFrame>();
				G3D::CoordinateFrame end = endValue.cast<G3D::CoordinateFrame>();

				G3D::CoordinateFrame mulFrame =
					G3D::CoordinateFrame(G3D::Matrix3((end.rotation[0][0] - start.rotation[0][0]) * factor,
					(end.rotation[0][1] - start.rotation[0][1]) * factor, (end.rotation[0][2] - start.rotation[0][2]) * factor,
						(end.rotation[1][0] - start.rotation[1][0]) * factor, (end.rotation[1][1] - start.rotation[1][1]) * factor,
						(end.rotation[1][2] - start.rotation[1][2]) * factor, (end.rotation[2][0] - start.rotation[2][0]) * factor,
						(end.rotation[2][1] - start.rotation[2][1]) * factor, (end.rotation[2][2] - start.rotation[2][2]) * factor),
						G3D::Vector3((end.translation.x - start.translation.x) * factor, (end.translation.y - start.translation.y) * factor,
						(end.translation.z - start.translation.z) * factor));

				G3D::CoordinateFrame newCFrame = G3D::CoordinateFrame(mulFrame.rotation + start.rotation, mulFrame.translation + start.translation);
				return newCFrame;
			}
			else if (startValue.isType<G3D::Vector2>())
			{
				return (endValue.cast<G3D::Vector2>() - startValue.cast<G3D::Vector2>()) * factor + startValue.cast<G3D::Vector2>();
			}
			else if (startValue.isType<G3D::Vector3>())
			{
				return (endValue.cast<G3D::Vector3>() - startValue.cast<G3D::Vector3>()) * factor + startValue.cast<G3D::Vector3>();
			}
			else if (startValue.isType<UDim>())
			{
				return (endValue.cast<UDim>() - startValue.cast<UDim>()) * factor + startValue.cast<UDim>();
			}
			else if (startValue.isType<UDim2>())
			{
				return (endValue.cast<UDim2>() - startValue.cast<UDim2>()) * factor + startValue.cast<UDim2>();
			}
			else if (startValue.isType<G3D::Color3>())
			{
				return (endValue.cast<G3D::Color3>() - startValue.cast<G3D::Color3>()) * factor + startValue.cast<G3D::Color3>();
			}
		}
		case TweenInfo::EASING_STYLE_BACK:
		{
			float factor = 1;
			const float s = 1.70158;
			switch (direction)
			{
			case TweenInfo::EASING_DIRECTION_IN:
			{
				const float t = elapsedTime / totalTime;
				factor = t * t * ((s + 1) * t - s);
				break;
			}
			case TweenInfo::EASING_DIRECTION_OUT:
			{
				const float t = elapsedTime / totalTime - 1;
				factor = t * t * ((s + 1) * t + s) + 1;
				break;
			}
			case TweenInfo::EASING_DIRECTION_IN_OUT:
			{
				float t = elapsedTime / (totalTime * 0.5);
				if (t < 1)
					factor = 0.5 * t * t * (((s * 1.525) + 1) * t - (s * 1.525));
				else
				{
					t -= 2;
					factor = 0.5 * (t * t * (((s * 1.525) + 1) * t + (s * .1525)) + 2);
				}
				break;
			}
			}
			if (startValue.isType<int>())
			{
				return (endValue.cast<double>() - startValue.cast<int>()) * factor + startValue.cast<int>();
			}
			else if (startValue.isType<float>())
			{
				return (endValue.cast<double>() - startValue.cast<float>()) * factor + startValue.cast<float>();
			}
			else if (startValue.isType<double>())
			{
				return (endValue.cast<double>() - startValue.cast<double>()) * factor + startValue.cast<double>();
			}
			else if (startValue.isType<CoordinateFrame>())
			{
				G3D::CoordinateFrame start = startValue.cast<G3D::CoordinateFrame>();
				G3D::CoordinateFrame end = endValue.cast<G3D::CoordinateFrame>();

				G3D::CoordinateFrame mulFrame =
					G3D::CoordinateFrame(G3D::Matrix3((end.rotation[0][0] - start.rotation[0][0]) * factor,
					(end.rotation[0][1] - start.rotation[0][1]) * factor, (end.rotation[0][2] - start.rotation[0][2]) * factor,
						(end.rotation[1][0] - start.rotation[1][0]) * factor, (end.rotation[1][1] - start.rotation[1][1]) * factor,
						(end.rotation[1][2] - start.rotation[1][2]) * factor, (end.rotation[2][0] - start.rotation[2][0]) * factor,
						(end.rotation[2][1] - start.rotation[2][1]) * factor, (end.rotation[2][2] - start.rotation[2][2]) * factor),
						G3D::Vector3((end.translation.x - start.translation.x) * factor, (end.translation.y - start.translation.y) * factor,
						(end.translation.z - start.translation.z) * factor));

				G3D::CoordinateFrame newCFrame = G3D::CoordinateFrame(mulFrame.rotation + start.rotation, mulFrame.translation + start.translation);
				return newCFrame;
			}
			else if (startValue.isType<G3D::Vector2>())
			{
				return (endValue.cast<G3D::Vector2>() - startValue.cast<G3D::Vector2>()) * factor + startValue.cast<G3D::Vector2>();
			}
			else if (startValue.isType<G3D::Vector3>())
			{
				return (endValue.cast<G3D::Vector3>() - startValue.cast<G3D::Vector3>()) * factor + startValue.cast<G3D::Vector3>();
			}
			else if (startValue.isType<UDim>())
			{
				return (endValue.cast<UDim>() - startValue.cast<UDim>()) * factor + startValue.cast<UDim>();
			}
			else if (startValue.isType<UDim2>())
			{
				return (endValue.cast<UDim2>() - startValue.cast<UDim2>()) * factor + startValue.cast<UDim2>();
			}
			else if (startValue.isType<G3D::Color3>())
			{
				return (endValue.cast<G3D::Color3>() - startValue.cast<G3D::Color3>()) * factor + startValue.cast<G3D::Color3>();
			}
		}
		case TweenInfo::EASING_STYLE_QUAD:
		{
			float factor = 1;
			switch (direction)
			{
			case TweenInfo::EASING_DIRECTION_IN:
			{
				float t = elapsedTime / totalTime;
				factor = t * t;
				break;
			}
			case TweenInfo::EASING_DIRECTION_OUT:
			{
				float t = elapsedTime / totalTime;
				factor = -t * (t - 2);
				break;
			}
			case TweenInfo::EASING_DIRECTION_IN_OUT:
			{
				float t = elapsedTime / (totalTime * 0.5);

				if (t < 1)
					factor = 0.5 * t * t;
				else
					factor = -0.5 * ((t - 1) * (t - 3) - 1);
				break;
			}
			}
			if (startValue.isType<int>())
			{
				return (endValue.cast<double>() - startValue.cast<int>()) * factor + startValue.cast<int>();
			}
			else if (startValue.isType<float>())
			{
				return (endValue.cast<double>() - startValue.cast<float>()) * factor + startValue.cast<float>();
			}
			else if (startValue.isType<double>())
			{
				return (endValue.cast<double>() - startValue.cast<double>()) * factor + startValue.cast<double>();
			}
			else if (startValue.isType<CoordinateFrame>())
			{
				G3D::CoordinateFrame start = startValue.cast<G3D::CoordinateFrame>();
				G3D::CoordinateFrame end = endValue.cast<G3D::CoordinateFrame>();

				G3D::CoordinateFrame mulFrame =
					G3D::CoordinateFrame(G3D::Matrix3((end.rotation[0][0] - start.rotation[0][0]) * factor,
					(end.rotation[0][1] - start.rotation[0][1]) * factor, (end.rotation[0][2] - start.rotation[0][2]) * factor,
						(end.rotation[1][0] - start.rotation[1][0]) * factor, (end.rotation[1][1] - start.rotation[1][1]) * factor,
						(end.rotation[1][2] - start.rotation[1][2]) * factor, (end.rotation[2][0] - start.rotation[2][0]) * factor,
						(end.rotation[2][1] - start.rotation[2][1]) * factor, (end.rotation[2][2] - start.rotation[2][2]) * factor),
						G3D::Vector3((end.translation.x - start.translation.x) * factor, (end.translation.y - start.translation.y) * factor,
						(end.translation.z - start.translation.z) * factor));

				G3D::CoordinateFrame newCFrame = G3D::CoordinateFrame(mulFrame.rotation + start.rotation, mulFrame.translation + start.translation);
				return newCFrame;
			}
			else if (startValue.isType<G3D::Vector2>())
			{
				return (endValue.cast<G3D::Vector2>() - startValue.cast<G3D::Vector2>()) * factor + startValue.cast<G3D::Vector2>();
			}
			else if (startValue.isType<G3D::Vector3>())
			{
				return (endValue.cast<G3D::Vector3>() - startValue.cast<G3D::Vector3>()) * factor + startValue.cast<G3D::Vector3>();
			}
			else if (startValue.isType<UDim>())
			{
				return (endValue.cast<UDim>() - startValue.cast<UDim>()) * factor + startValue.cast<UDim>();
			}
			else if (startValue.isType<UDim2>())
			{
				return (endValue.cast<UDim2>() - startValue.cast<UDim2>()) * factor + startValue.cast<UDim2>();
			}
			else if (startValue.isType<G3D::Color3>())
			{
				return (endValue.cast<G3D::Color3>() - startValue.cast<G3D::Color3>()) * factor + startValue.cast<G3D::Color3>();
			}
		}
		case TweenInfo::EASING_STYLE_QUART:
		{
			float factor = 1;
			switch (direction)
			{
			case TweenInfo::EASING_DIRECTION_IN:
			{
				const float t = elapsedTime / totalTime;
				factor = pow(t, 4);
				break;
			}
			case TweenInfo::EASING_DIRECTION_OUT:
			{
				const float t = elapsedTime / totalTime - 1;
				factor = -(pow(t, 4) - 1);
				break;
			}
			case TweenInfo::EASING_DIRECTION_IN_OUT:
			{
				const float t = elapsedTime / (totalTime * 0.5);
				if (t < 1)
					factor = 0.5 * pow(t, 4);
				else
					factor = -0.5 * (pow(t - 2, 4) - 2);
				break;
			}
			}
			if (startValue.isType<int>())
			{
				return (endValue.cast<double>() - startValue.cast<int>()) * factor + startValue.cast<int>();
			}
			else if (startValue.isType<float>())
			{
				return (endValue.cast<double>() - startValue.cast<float>()) * factor + startValue.cast<float>();
			}
			else if (startValue.isType<double>())
			{
				return (endValue.cast<double>() - startValue.cast<double>()) * factor + startValue.cast<double>();
			}
			else if (startValue.isType<CoordinateFrame>())
			{
				G3D::CoordinateFrame start = startValue.cast<G3D::CoordinateFrame>();
				G3D::CoordinateFrame end = endValue.cast<G3D::CoordinateFrame>();

				G3D::CoordinateFrame mulFrame =
					G3D::CoordinateFrame(G3D::Matrix3((end.rotation[0][0] - start.rotation[0][0]) * factor,
					(end.rotation[0][1] - start.rotation[0][1]) * factor, (end.rotation[0][2] - start.rotation[0][2]) * factor,
						(end.rotation[1][0] - start.rotation[1][0]) * factor, (end.rotation[1][1] - start.rotation[1][1]) * factor,
						(end.rotation[1][2] - start.rotation[1][2]) * factor, (end.rotation[2][0] - start.rotation[2][0]) * factor,
						(end.rotation[2][1] - start.rotation[2][1]) * factor, (end.rotation[2][2] - start.rotation[2][2]) * factor),
						G3D::Vector3((end.translation.x - start.translation.x) * factor, (end.translation.y - start.translation.y) * factor,
						(end.translation.z - start.translation.z) * factor));

				G3D::CoordinateFrame newCFrame = G3D::CoordinateFrame(mulFrame.rotation + start.rotation, mulFrame.translation + start.translation);
				return newCFrame;
			}
			else if (startValue.isType<G3D::Vector2>())
			{
				return (endValue.cast<G3D::Vector2>() - startValue.cast<G3D::Vector2>()) * factor + startValue.cast<G3D::Vector2>();
			}
			else if (startValue.isType<G3D::Vector3>())
			{
				return (endValue.cast<G3D::Vector3>() - startValue.cast<G3D::Vector3>()) * factor + startValue.cast<G3D::Vector3>();
			}
			else if (startValue.isType<UDim>())
			{
				return (endValue.cast<UDim>() - startValue.cast<UDim>()) * factor + startValue.cast<UDim>();
			}
			else if (startValue.isType<UDim2>())
			{
				return (endValue.cast<UDim2>() - startValue.cast<UDim2>()) * factor + startValue.cast<UDim2>();
			}
			else if (startValue.isType<G3D::Color3>())
			{
				return (endValue.cast<G3D::Color3>() - startValue.cast<G3D::Color3>()) * factor + startValue.cast<G3D::Color3>();
			}
		}
		case TweenInfo::EASING_STYLE_QUINT:
		{
			float factor = 1;
			switch (direction)
			{
			case TweenInfo::EASING_DIRECTION_IN:
			{
				const float t = elapsedTime / totalTime;
				factor = pow(t, 5);
				break;
			}
			case TweenInfo::EASING_DIRECTION_OUT:
			{
				const float t = elapsedTime / totalTime - 1;
				factor = pow(t, 5) + 1;
				break;
			}
			case TweenInfo::EASING_DIRECTION_IN_OUT:
			{
				const float t = elapsedTime / (totalTime * 0.5);

				if (t < 1)
					factor = 0.5 * pow(t, 5);
				else
					factor = 0.5 * (pow(t - 2, 5) + 2);
				break;
			}
			}
			if (startValue.isType<int>())
			{
				return (endValue.cast<double>() - startValue.cast<int>()) * factor + startValue.cast<int>();
			}
			else if (startValue.isType<float>())
			{
				return (endValue.cast<double>() - startValue.cast<float>()) * factor + startValue.cast<float>();
			}
			else if (startValue.isType<double>())
			{
				return (endValue.cast<double>() - startValue.cast<double>()) * factor + startValue.cast<double>();
			}
			else if (startValue.isType<CoordinateFrame>())
			{
				G3D::CoordinateFrame start = startValue.cast<G3D::CoordinateFrame>();
				G3D::CoordinateFrame end = endValue.cast<G3D::CoordinateFrame>();

				G3D::CoordinateFrame mulFrame =
					G3D::CoordinateFrame(G3D::Matrix3((end.rotation[0][0] - start.rotation[0][0]) * factor,
					(end.rotation[0][1] - start.rotation[0][1]) * factor, (end.rotation[0][2] - start.rotation[0][2]) * factor,
						(end.rotation[1][0] - start.rotation[1][0]) * factor, (end.rotation[1][1] - start.rotation[1][1]) * factor,
						(end.rotation[1][2] - start.rotation[1][2]) * factor, (end.rotation[2][0] - start.rotation[2][0]) * factor,
						(end.rotation[2][1] - start.rotation[2][1]) * factor, (end.rotation[2][2] - start.rotation[2][2]) * factor),
						G3D::Vector3((end.translation.x - start.translation.x) * factor, (end.translation.y - start.translation.y) * factor,
						(end.translation.z - start.translation.z) * factor));

				G3D::CoordinateFrame newCFrame = G3D::CoordinateFrame(mulFrame.rotation + start.rotation, mulFrame.translation + start.translation);
				return newCFrame;
			}
			else if (startValue.isType<G3D::Vector2>())
			{
				return (endValue.cast<G3D::Vector2>() - startValue.cast<G3D::Vector2>()) * factor + startValue.cast<G3D::Vector2>();
			}
			else if (startValue.isType<G3D::Vector3>())
			{
				return (endValue.cast<G3D::Vector3>() - startValue.cast<G3D::Vector3>()) * factor + startValue.cast<G3D::Vector3>();
			}
			else if (startValue.isType<UDim>())
			{
				return (endValue.cast<UDim>() - startValue.cast<UDim>()) * factor + startValue.cast<UDim>();
			}
			else if (startValue.isType<UDim2>())
			{
				return (endValue.cast<UDim2>() - startValue.cast<UDim2>()) * factor + startValue.cast<UDim2>();
			}
			else if (startValue.isType<G3D::Color3>())
			{
				return (endValue.cast<G3D::Color3>() - startValue.cast<G3D::Color3>()) * factor + startValue.cast<G3D::Color3>();
			}
		}
		case TweenInfo::EASING_STYLE_BOUNCE:
		{
			switch (style)
			{
			case TweenInfo::EASING_DIRECTION_IN:
				if (startValue.isType<int>())
				{
					return (endValue.cast<double>() - startValue.cast<int>()) -
						(TweenInterpolate(TweenInfo::EASING_DIRECTION_OUT, TweenInfo::EASING_STYLE_BOUNCE, totalTime - elapsedTime, totalTime,
							startValue, Reflection::Variant(endValue.cast<double>() - startValue.cast<int>()))
							.cast<int>()) +
						startValue.cast<int>();
				}
				else if (startValue.isType<float>())
				{
					return (endValue.cast<double>() - startValue.cast<float>()) -
						(TweenInterpolate(TweenInfo::EASING_DIRECTION_OUT, TweenInfo::EASING_STYLE_BOUNCE, totalTime - elapsedTime, totalTime,
							startValue, Reflection::Variant(endValue.cast<double>() - startValue.cast<float>()))
							.cast<float>()) +
						startValue.cast<float>();
				}
				else if (startValue.isType<double>())
				{
					return (endValue.cast<double>() - startValue.cast<double>()) -
						(TweenInterpolate(TweenInfo::EASING_DIRECTION_OUT, TweenInfo::EASING_STYLE_BOUNCE, totalTime - elapsedTime, totalTime,
							startValue, Reflection::Variant(endValue.cast<double>() - startValue.cast<double>()))
							.cast<double>()) +
						startValue.cast<double>();
				}
				else if (startValue.isType<CoordinateFrame>())
				{

					G3D::CoordinateFrame start = startValue.cast<G3D::CoordinateFrame>();
					G3D::CoordinateFrame end = endValue.cast<G3D::CoordinateFrame>();

					G3D::CoordinateFrame firstFrame = G3D::CoordinateFrame(end.rotation - start.rotation, end.translation - start.translation);

					G3D::CoordinateFrame cast = TweenInterpolate(TweenInfo::EASING_DIRECTION_OUT, TweenInfo::EASING_STYLE_BOUNCE, totalTime - elapsedTime,
						totalTime, startValue, Reflection::Variant(firstFrame))
						.cast<G3D::CoordinateFrame>();

					G3D::CoordinateFrame mulFrame = G3D::CoordinateFrame(firstFrame.rotation - cast.rotation, firstFrame.translation - cast.translation);

					G3D::CoordinateFrame newCFrame = G3D::CoordinateFrame(mulFrame.rotation + start.rotation, mulFrame.translation + start.translation);
					return newCFrame;
				}
				else if (startValue.isType<G3D::Vector2>())
				{
					return (endValue.cast<G3D::Vector2>() - startValue.cast<G3D::Vector2>()) -
						(TweenInterpolate(TweenInfo::EASING_DIRECTION_OUT, TweenInfo::EASING_STYLE_BOUNCE, totalTime - elapsedTime, totalTime,
							startValue, Reflection::Variant(endValue.cast<G3D::Vector2>() - startValue.cast<G3D::Vector2>()))
							.cast<G3D::Vector2>()) +
						startValue.cast<G3D::Vector2>();
				}
				else if (startValue.isType<G3D::Vector3>())
				{
					return (endValue.cast<G3D::Vector3>() - startValue.cast<G3D::Vector3>()) -
						(TweenInterpolate(TweenInfo::EASING_DIRECTION_OUT, TweenInfo::EASING_STYLE_BOUNCE, totalTime - elapsedTime, totalTime,
							startValue, Reflection::Variant(endValue.cast<G3D::Vector3>() - startValue.cast<G3D::Vector3>()))
							.cast<G3D::Vector3>()) +
						startValue.cast<G3D::Vector3>();
				}
				else if (startValue.isType<UDim>())
				{
					return (endValue.cast<UDim>() - startValue.cast<UDim>()) -
						(TweenInterpolate(TweenInfo::EASING_DIRECTION_OUT, TweenInfo::EASING_STYLE_BOUNCE, totalTime - elapsedTime, totalTime,
							startValue, Reflection::Variant(endValue.cast<UDim>() - startValue.cast<UDim>()))
							.cast<UDim>()) +
						startValue.cast<UDim>();
				}
				else if (startValue.isType<UDim2>())
				{
					return (endValue.cast<UDim2>() - startValue.cast<UDim2>()) -
						(TweenInterpolate(TweenInfo::EASING_DIRECTION_OUT, TweenInfo::EASING_STYLE_BOUNCE, totalTime - elapsedTime, totalTime,
							startValue, Reflection::Variant(endValue.cast<UDim2>() - startValue.cast<UDim2>()))
							.cast<UDim2>()) +
						startValue.cast<UDim2>();
				}
				else if (startValue.isType<G3D::Color3>())
				{
					return (endValue.cast<G3D::Color3>() - startValue.cast<G3D::Color3>()) -
						(TweenInterpolate(TweenInfo::EASING_DIRECTION_OUT, TweenInfo::EASING_STYLE_BOUNCE, totalTime - elapsedTime, totalTime,
							startValue, Reflection::Variant(endValue.cast<G3D::Color3>() - startValue.cast<G3D::Color3>()))
							.cast<G3D::Color3>()) +
						startValue.cast<G3D::Color3>();
				}

			case TweenInfo::EASING_DIRECTION_OUT:
			{
				const float timePercent = elapsedTime / totalTime;
				float factor = 1;
				if (timePercent < 1 / 2.75)
					factor = 7.5625 * pow(timePercent, 2);
				else if (timePercent < 2 / 2.75)
					factor = 7.5625 * pow(timePercent - 1.5 / 2.75, 2) + .75;
				else if (timePercent < 2.5 / 2.75)
					factor = 7.5625 * pow(timePercent - 2.25 / 2.75, 2) + .9375;
				else
					factor = 7.5625 * pow(timePercent - 2.625 / 2.75, 2) + .984375;
				if (startValue.isType<int>())
				{
					return (endValue.cast<double>() - startValue.cast<int>()) * factor + startValue.cast<int>();
				}
				else if (startValue.isType<float>())
				{
					return (endValue.cast<double>() - startValue.cast<float>()) * factor + startValue.cast<float>();
				}
				else if (startValue.isType<double>())
				{
					return (endValue.cast<double>() - startValue.cast<double>()) * factor + startValue.cast<double>();
				}
				else if (startValue.isType<CoordinateFrame>())
				{
					G3D::CoordinateFrame start = startValue.cast<G3D::CoordinateFrame>();
					G3D::CoordinateFrame end = endValue.cast<G3D::CoordinateFrame>();

					G3D::CoordinateFrame mulFrame = G3D::CoordinateFrame(
						G3D::Matrix3((end.rotation[0][0] - start.rotation[0][0]) * factor, (end.rotation[0][1] - start.rotation[0][1]) * factor,
						(end.rotation[0][2] - start.rotation[0][2]) * factor, (end.rotation[1][0] - start.rotation[1][0]) * factor,
							(end.rotation[1][1] - start.rotation[1][1]) * factor, (end.rotation[1][2] - start.rotation[1][2]) * factor,
							(end.rotation[2][0] - start.rotation[2][0]) * factor, (end.rotation[2][1] - start.rotation[2][1]) * factor,
							(end.rotation[2][2] - start.rotation[2][2]) * factor),
						G3D::Vector3((end.translation.x - start.translation.x) * factor, (end.translation.y - start.translation.y) * factor,
						(end.translation.z - start.translation.z) * factor));

					G3D::CoordinateFrame newCFrame = G3D::CoordinateFrame(mulFrame.rotation + start.rotation, mulFrame.translation + start.translation);
					return newCFrame;
				}
				else if (startValue.isType<G3D::Vector2>())
				{
					return (endValue.cast<G3D::Vector2>() - startValue.cast<G3D::Vector2>()) * factor + startValue.cast<G3D::Vector2>();
				}
				else if (startValue.isType<G3D::Vector3>())
				{
					return (endValue.cast<G3D::Vector3>() - startValue.cast<G3D::Vector3>()) * factor + startValue.cast<G3D::Vector3>();
				}
				else if (startValue.isType<UDim>())
				{
					return (endValue.cast<UDim>() - startValue.cast<UDim>()) * factor + startValue.cast<UDim>();
				}
				else if (startValue.isType<UDim2>())
				{
					return (endValue.cast<UDim2>() - startValue.cast<UDim2>()) * factor + startValue.cast<UDim2>();
				}
				else if (startValue.isType<G3D::Color3>())
				{
					return (endValue.cast<G3D::Color3>() - startValue.cast<G3D::Color3>()) * factor + startValue.cast<G3D::Color3>();
				}
			}
			case TweenInfo::EASING_DIRECTION_IN_OUT:
				if (elapsedTime < totalTime * 0.5)
				{
					if (startValue.isType<int>())
					{
						return (endValue.cast<double>() - startValue.cast<int>()) -
							(TweenInterpolate(TweenInfo::EASING_DIRECTION_IN, TweenInfo::EASING_STYLE_BOUNCE, elapsedTime * 2, totalTime, startValue,
								Reflection::Variant(endValue.cast<double>() - startValue.cast<int>()))
								.cast<int>()) *
							0.5 +
							startValue.cast<int>();
					}
					else if (startValue.isType<float>())
					{
						return (endValue.cast<double>() - startValue.cast<float>()) -
							(TweenInterpolate(TweenInfo::EASING_DIRECTION_IN, TweenInfo::EASING_STYLE_BOUNCE, elapsedTime * 2, totalTime, startValue,
								Reflection::Variant(endValue.cast<double>() - startValue.cast<float>()))
								.cast<float>()) *
							0.5 +
							startValue.cast<float>();
					}
					else if (startValue.isType<double>())
					{
						return (endValue.cast<double>() - startValue.cast<double>()) -
							(TweenInterpolate(TweenInfo::EASING_DIRECTION_IN, TweenInfo::EASING_STYLE_BOUNCE, elapsedTime * 2, totalTime, startValue,
								Reflection::Variant(endValue.cast<double>() - startValue.cast<double>()))
								.cast<double>()) *
							0.5 +
							startValue.cast<double>();
					}
					else if (startValue.isType<CoordinateFrame>())
					{

						G3D::CoordinateFrame start = startValue.cast<G3D::CoordinateFrame>();
						G3D::CoordinateFrame end = endValue.cast<G3D::CoordinateFrame>();

						G3D::CoordinateFrame firstFrame = G3D::CoordinateFrame(end.rotation - start.rotation, end.translation - start.translation);

						G3D::CoordinateFrame cast = TweenInterpolate(TweenInfo::EASING_DIRECTION_IN, TweenInfo::EASING_STYLE_BOUNCE, elapsedTime * 2,
							totalTime, startValue, Reflection::Variant(firstFrame))
							.cast<G3D::CoordinateFrame>();

						G3D::CoordinateFrame mulFrame =
							G3D::CoordinateFrame(firstFrame.rotation - cast.rotation, firstFrame.translation - cast.translation);

						G3D::CoordinateFrame newCFrame =
							G3D::CoordinateFrame(mulFrame.rotation + start.rotation * 0.5, mulFrame.translation + start.translation * 0.5);
						return newCFrame;
					}
					else if (startValue.isType<G3D::Vector2>())
					{
						return (endValue.cast<G3D::Vector2>() - startValue.cast<G3D::Vector2>()) -
							(TweenInterpolate(TweenInfo::EASING_DIRECTION_IN, TweenInfo::EASING_STYLE_BOUNCE, elapsedTime * 2, totalTime, startValue,
								Reflection::Variant(endValue.cast<G3D::Vector2>() - startValue.cast<G3D::Vector2>()))
								.cast<G3D::Vector2>()) *
							0.5 +
							startValue.cast<G3D::Vector2>();
					}
					else if (startValue.isType<G3D::Vector3>())
					{
						return (endValue.cast<G3D::Vector3>() - startValue.cast<G3D::Vector3>()) -
							(TweenInterpolate(TweenInfo::EASING_DIRECTION_IN, TweenInfo::EASING_STYLE_BOUNCE, elapsedTime * 2, totalTime, startValue,
								Reflection::Variant(endValue.cast<G3D::Vector3>() - startValue.cast<G3D::Vector3>()))
								.cast<G3D::Vector3>()) *
							0.5 +
							startValue.cast<G3D::Vector3>();
					}
					else if (startValue.isType<UDim>())
					{
						return (endValue.cast<UDim>() - startValue.cast<UDim>()) -
							(TweenInterpolate(TweenInfo::EASING_DIRECTION_IN, TweenInfo::EASING_STYLE_BOUNCE, elapsedTime * 2, totalTime, startValue,
								Reflection::Variant(endValue.cast<UDim>() - startValue.cast<UDim>()))
								.cast<UDim>()) *
								(float)0.5 +
							startValue.cast<UDim>();
					}
					else if (startValue.isType<UDim2>())
					{
						return (endValue.cast<UDim2>() - startValue.cast<UDim2>()) -
							(TweenInterpolate(TweenInfo::EASING_DIRECTION_IN, TweenInfo::EASING_STYLE_BOUNCE, elapsedTime * 2, totalTime, startValue,
								Reflection::Variant(endValue.cast<UDim2>() - startValue.cast<UDim2>()))
								.cast<UDim2>()) *
								(float)0.5 +
							startValue.cast<UDim2>();
					}
					else if (startValue.isType<G3D::Color3>())
					{
						return (endValue.cast<G3D::Color3>() - startValue.cast<G3D::Color3>()) -
							(TweenInterpolate(TweenInfo::EASING_DIRECTION_IN, TweenInfo::EASING_STYLE_BOUNCE, elapsedTime * 2, totalTime, startValue,
								Reflection::Variant(endValue.cast<G3D::Color3>() - startValue.cast<G3D::Color3>()))
								.cast<G3D::Color3>()) *
							0.5 +
							startValue.cast<G3D::Color3>();
					}
				}
				else
				{
					if (startValue.isType<int>())
					{
						return (endValue.cast<double>() - startValue.cast<int>()) -
							(TweenInterpolate(TweenInfo::EASING_DIRECTION_OUT, TweenInfo::EASING_STYLE_BOUNCE, elapsedTime * 2 - totalTime, totalTime,
								startValue, Reflection::Variant(endValue.cast<double>() - startValue.cast<int>()))
								.cast<int>()) *
							0.5 +
							(endValue.cast<double>() - startValue.cast<int>()) + startValue.cast<int>();
					}
					else if (startValue.isType<float>())
					{
						return (endValue.cast<double>() - startValue.cast<float>()) -
							(TweenInterpolate(TweenInfo::EASING_DIRECTION_OUT, TweenInfo::EASING_STYLE_BOUNCE, elapsedTime * 2 - totalTime, totalTime,
								startValue, Reflection::Variant(endValue.cast<double>() - startValue.cast<float>()))
								.cast<float>()) *
							0.5 +
							(endValue.cast<double>() - startValue.cast<float>()) + startValue.cast<float>();
					}
					else if (startValue.isType<double>())
					{
						return (endValue.cast<double>() - startValue.cast<double>()) -
							(TweenInterpolate(TweenInfo::EASING_DIRECTION_OUT, TweenInfo::EASING_STYLE_BOUNCE, elapsedTime * 2 - totalTime, totalTime,
								startValue, Reflection::Variant(endValue.cast<double>() - startValue.cast<double>()))
								.cast<double>()) *
							0.5 +
							(endValue.cast<double>() - startValue.cast<double>()) + startValue.cast<double>();
					}
					else if (startValue.isType<CoordinateFrame>())
					{

						G3D::CoordinateFrame start = startValue.cast<G3D::CoordinateFrame>();
						G3D::CoordinateFrame end = endValue.cast<G3D::CoordinateFrame>();

						G3D::CoordinateFrame firstFrame = G3D::CoordinateFrame(end.rotation - start.rotation, end.translation - start.translation);

						G3D::CoordinateFrame cast = TweenInterpolate(TweenInfo::EASING_DIRECTION_OUT, TweenInfo::EASING_STYLE_BOUNCE,
							elapsedTime * 2 - totalTime, totalTime, startValue, Reflection::Variant(firstFrame))
							.cast<G3D::CoordinateFrame>();

						G3D::CoordinateFrame mulFrame =
							G3D::CoordinateFrame(firstFrame.rotation - cast.rotation, firstFrame.translation - cast.translation);

						G3D::CoordinateFrame newCFrame = G3D::CoordinateFrame(mulFrame.rotation + start.rotation + (end.rotation - start.rotation) * 0.5,
							mulFrame.translation + start.translation + (end.translation - start.translation) * 0.5);
						return newCFrame;
					}
					else if (startValue.isType<G3D::Vector2>())
					{
						return (endValue.cast<G3D::Vector2>() - startValue.cast<G3D::Vector2>()) -
							(TweenInterpolate(TweenInfo::EASING_DIRECTION_OUT, TweenInfo::EASING_STYLE_BOUNCE, elapsedTime * 2 - totalTime, totalTime,
								startValue, Reflection::Variant(endValue.cast<G3D::Vector2>() - startValue.cast<G3D::Vector2>()))
								.cast<G3D::Vector2>()) *
							0.5 +
							(endValue.cast<G3D::Vector2>() - startValue.cast<G3D::Vector2>()) + startValue.cast<G3D::Vector2>();
					}
					else if (startValue.isType<G3D::Vector3>())
					{
						return (endValue.cast<G3D::Vector3>() - startValue.cast<G3D::Vector3>()) -
							(TweenInterpolate(TweenInfo::EASING_DIRECTION_OUT, TweenInfo::EASING_STYLE_BOUNCE, elapsedTime * 2 - totalTime, totalTime,
								startValue, Reflection::Variant(endValue.cast<G3D::Vector3>() - startValue.cast<G3D::Vector3>()))
								.cast<G3D::Vector3>()) *
							0.5 +
							(endValue.cast<G3D::Vector3>() - startValue.cast<G3D::Vector3>()) + startValue.cast<G3D::Vector3>();
					}
					else if (startValue.isType<UDim>())
					{
						return (endValue.cast<UDim>() - startValue.cast<UDim>()) -
							(TweenInterpolate(TweenInfo::EASING_DIRECTION_OUT, TweenInfo::EASING_STYLE_BOUNCE, elapsedTime * 2 - totalTime, totalTime,
								startValue, Reflection::Variant(endValue.cast<UDim>() - startValue.cast<UDim>()))
								.cast<UDim>()) *
								(float)0.5 +
							(endValue.cast<UDim>() - startValue.cast<UDim>()) + startValue.cast<UDim>();
					}
					else if (startValue.isType<UDim2>())
					{
						return (endValue.cast<UDim2>() - startValue.cast<UDim2>()) -
							(TweenInterpolate(TweenInfo::EASING_DIRECTION_OUT, TweenInfo::EASING_STYLE_BOUNCE, elapsedTime * 2 - totalTime, totalTime,
								startValue, Reflection::Variant(endValue.cast<UDim2>() - startValue.cast<UDim2>()))
								.cast<UDim2>()) *
								(float)0.5 +
							(endValue.cast<UDim2>() - startValue.cast<UDim2>()) + startValue.cast<UDim2>();
					}
					else if (startValue.isType<G3D::Color3>())
					{
						return (endValue.cast<G3D::Color3>() - startValue.cast<G3D::Color3>()) -
							(TweenInterpolate(TweenInfo::EASING_DIRECTION_OUT, TweenInfo::EASING_STYLE_BOUNCE, elapsedTime * 2, totalTime, startValue,
								Reflection::Variant(endValue.cast<G3D::Color3>() - startValue.cast<G3D::Color3>()))
								.cast<G3D::Color3>()) *
							0.5 +
							(endValue.cast<G3D::Color3>() - startValue.cast<G3D::Color3>()) + startValue.cast<G3D::Color3>();
					}
				}
			default:
				ARLASSERT(0);
				return Reflection::Variant();
			}
		}
		case TweenInfo::EASING_STYLE_ELASTIC:
		{
			if (elapsedTime == 0)
				return startValue;
			float t = elapsedTime / totalTime;
			if (t >= 1)
				return endValue;

			float factor = 1;
			switch (direction)
			{
			case TweenInfo::EASING_DIRECTION_IN:
			{
				float p = totalTime * .3;
				float s = p / 4;
				t -= 1;
				factor = -(pow(2, 10 * t) * sin((t * totalTime - s) * (DFFlag::ElasticEasingUseTwoPi ? Math::twoPi() : Math::piHalf()) / p));
				break;
			}
			case TweenInfo::EASING_DIRECTION_OUT:
			{
				float p = totalTime * .3;
				float s = p / 4;
				factor = 1 + pow(2, -10 * t) * sin((t * totalTime - s) * (DFFlag::ElasticEasingUseTwoPi ? Math::twoPi() : Math::piHalf()) / p);
				break;
			}
			case TweenInfo::EASING_DIRECTION_IN_OUT:
			{
				t = elapsedTime / (totalTime * 0.5);
				float p = totalTime * (.3 * 1.5);
				float s = p / 4;

				if (t < 1)
				{
					t -= 1;
					factor = -.5 * pow(2, 10 * t) * sin((t * totalTime - s) * (DFFlag::ElasticEasingUseTwoPi ? Math::twoPi() : Math::piHalf()) / p);
				}
				else
				{
					t -= 1;
					factor = 1 + 0.5 * pow(2, -10 * t) * sin((t * totalTime - s) * (DFFlag::ElasticEasingUseTwoPi ? Math::twoPi() : Math::piHalf()) / p);
				}
				break;
			}
			}
			if (startValue.isType<int>())
			{
				return (endValue.cast<double>() - startValue.cast<int>()) * factor + startValue.cast<int>();
			}
			else if (startValue.isType<float>())
			{
				return (endValue.cast<double>() - startValue.cast<float>()) * factor + startValue.cast<float>();
			}
			else if (startValue.isType<double>())
			{
				return (endValue.cast<double>() - startValue.cast<double>()) * factor + startValue.cast<double>();
			}
			else if (startValue.isType<CoordinateFrame>())
			{
				G3D::CoordinateFrame start = startValue.cast<G3D::CoordinateFrame>();
				G3D::CoordinateFrame end = endValue.cast<G3D::CoordinateFrame>();

				G3D::CoordinateFrame mulFrame =
					G3D::CoordinateFrame(G3D::Matrix3((end.rotation[0][0] - start.rotation[0][0]) * factor,
					(end.rotation[0][1] - start.rotation[0][1]) * factor, (end.rotation[0][2] - start.rotation[0][2]) * factor,
						(end.rotation[1][0] - start.rotation[1][0]) * factor, (end.rotation[1][1] - start.rotation[1][1]) * factor,
						(end.rotation[1][2] - start.rotation[1][2]) * factor, (end.rotation[2][0] - start.rotation[2][0]) * factor,
						(end.rotation[2][1] - start.rotation[2][1]) * factor, (end.rotation[2][2] - start.rotation[2][2]) * factor),
						G3D::Vector3((end.translation.x - start.translation.x) * factor, (end.translation.y - start.translation.y) * factor,
						(end.translation.z - start.translation.z) * factor));

				G3D::CoordinateFrame newCFrame = G3D::CoordinateFrame(mulFrame.rotation + start.rotation, mulFrame.translation + start.translation);
				return newCFrame;
			}
			else if (startValue.isType<G3D::Vector2>())
			{
				return (endValue.cast<G3D::Vector2>() - startValue.cast<G3D::Vector2>()) * factor + startValue.cast<G3D::Vector2>();
			}
			else if (startValue.isType<G3D::Vector3>())
			{
				return (endValue.cast<G3D::Vector3>() - startValue.cast<G3D::Vector3>()) * factor + startValue.cast<G3D::Vector3>();
			}
			else if (startValue.isType<UDim>())
			{
				return (endValue.cast<UDim>() - startValue.cast<UDim>()) * factor + startValue.cast<UDim>();
			}
			else if (startValue.isType<UDim2>())
			{
				return (endValue.cast<UDim2>() - startValue.cast<UDim2>()) * factor + startValue.cast<UDim2>();
			}
			else if (startValue.isType<G3D::Color3>())
			{
				return (endValue.cast<G3D::Color3>() - startValue.cast<G3D::Color3>()) * factor + startValue.cast<G3D::Color3>();
			}
		}
		default:
			ARLASSERT(0);
			return Reflection::Variant();
		}
	}

	void TweenService::onStepped(const Stepped& event)
	{
		update(event.gameStep);
		updateInstance(event.gameStep);
	}

	void TweenService::onHeartbeat(const Heartbeat& heartbeatEvent)
	{
		update(heartbeatEvent.wallStep);
		updateInstance(heartbeatEvent.wallStep);
	}

	void TweenService::update(const double step)
	{
		// call all the tween callbacks (for tweens that have finished/been cancelled)
		TweenCallbacks temp;
		tweenCallbacks.swap(temp);
		for (TweenCallbacks::iterator iter = temp.begin(); iter != temp.end(); iter++)
		{
			iter->first(iter->second);
		}

		// Step all the "tweening" objects
		for (TweeningObjectsList::iterator iter = tweeningObjects.begin(); iter != tweeningObjects.end();)
		{
			if (shared_ptr<GuiObject> guiObj = iter->lock())
			{
				if (guiObj->tweenStep(step))
				{
					// Return true when we are done with tweening
					tweeningObjects.erase(iter++);
				}
				else
				{
					++iter;
				}
			}
			else
			{
				tweeningObjects.erase(iter++);
			}
		}
	}

	void TweenService::updateInstance(const double step)
	{
		// Step all the "tweening" objects
		for (TweeningInstanceList::iterator iter = tweeningInstances.begin(); iter != tweeningInstances.end();)
		{
			bool paused = false;
			if (this->tweenStep(step, paused))
			{
				if (!paused)
					tweeningInstances.erase(iter++);
				else
					++iter;
			}
			else
			{
				++iter;
			}
		}
	}
} // namespace ARL
