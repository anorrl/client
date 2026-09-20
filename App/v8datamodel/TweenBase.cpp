#include "stdafx.h"
#include "V8DataModel/TweenBase.h"
#include "V8DataModel/TweenService.h"
#include "V8DataModel/GuiObject.h"
//  print(game:GetService("TweenService"):Create(workspace.BasePlate, TweenInfo.new(1, Enum.EasingStyle.Linear, Enum.EasingDirection.InOut),
//  {Reflectance = 12}):Play())

namespace ARL
{
	REFLECTION_BEGIN();
	static Reflection::BoundFuncDesc<TweenBase, void()> playFunc(&TweenBase::play, "Play", Security::None);
	static Reflection::BoundFuncDesc<TweenBase, void()> pauseFunc(&TweenBase::pause, "Pause", Security::None);
	static Reflection::BoundFuncDesc<TweenBase, void()> cancelFunc(&TweenBase::cancel, "Cancel", Security::None);

	static Reflection::EnumPropDescriptor<TweenBase, PlayBackState> prop_playBackState(
		"PlaybackState", category_Behavior, &TweenBase::getPlaybackState, NULL, Reflection::PropertyDescriptor::STANDARD_NO_REPLICATE);

	static Reflection::EventDesc<TweenBase, void(PlayBackState)> event_Completed(
		&TweenBase::completedSignal, "Completed", "playBackState", Security::None);
	REFLECTION_END();

	const char* const sTweenBase = "TweenBase";

	namespace Reflection
	{
		template<>
		EnumDesc<PlayBackState>::EnumDesc()
			: EnumDescriptor("PlayBackState")
		{
			addPair(PLAYBACK_STATE_BEGIN, "Begin");
			addPair(PLAYBACK_STATE_CANCELLED, "Cancelled");
			addPair(PLAYBACK_STATE_COMPLETED, "Completed");
			addPair(PLAYBACK_STATE_DELAYED, "Delayed");
			addPair(PLAYBACK_STATE_PAUSED, "Paused");
			addPair(PLAYBACK_STATE_PLAYING, "Playing");
		}

		template<>
		PlayBackState& Variant::convert<PlayBackState>(void)
		{
			return genericConvert<PlayBackState>();
		}

	} // namespace Reflection

	  // ARL:
#ifdef __clang__
	template<>
#endif
	bool ARL::StringConverter<PlayBackState>::convertToValue(const std::string& text, PlayBackState& value)
	{
		return Reflection::EnumDesc<PlayBackState>::singleton().convertToValue(text.c_str(), value);
	}

	TweenBase::TweenBase(shared_ptr<Instance> instance, TweenInfo twnInfo, shared_ptr<const Reflection::ValueArray> propTable)
		: instance(instance)
		, twnInfo(twnInfo)
		, propTable(propTable)
		, state(PLAYBACK_STATE_BEGIN)
		, elapsedTime(0)
		, isLive(false)
	{
		setName(sTweenBase);
	}

	void TweenBase::cancel()
	{
		setIsPlaying(false);
		setElapsedTime(getTweenInfo().getTime() + 100);
		setPlaybackState(PLAYBACK_STATE_CANCELLED);
	}

	void TweenBase::play()
	{
		setIsPlaying(true);
		setPlaybackState(PLAYBACK_STATE_PLAYING);
		// twnInfo.setTime(twnInfo.getPauseTime());
		TweenService* tweenService = ServiceProvider::find<TweenService>(this);
		if (!tweenService)
		{
			throw std::runtime_error("Can only tween objects in the workspace");
		}

		tweenService->addTweeningInstance(weak_from(this->instance.get()));
	}

	void TweenBase::pause()
	{
		setIsPlaying(false);
		twnInfo.setTime(twnInfo.getTime() - getElapsedTime());
		setPlaybackState(PLAYBACK_STATE_PAUSED);
	}

	void TweenBase::setElapsedTime(float value)
	{
		if (elapsedTime != value)
		{
			elapsedTime = value;
		}
	}
	void TweenBase::setPlaybackState(PlayBackState value)
	{
		if (state != value)
		{
			state = value;
			raisePropertyChanged(prop_playBackState);
			completedSignal(value);
		}
	}
	void TweenBase::setIsPlaying(bool value)
	{
		if (isLive != value)
		{
			isLive = value;
		}
	}
} // namespace ARL