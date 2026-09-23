#pragma once

#include "signal.h"
#include "v8tree/Instance.h"
#include "util/TweenInfo.h"
#include "reflection/Reflection.h"

namespace ARL
{
	enum PlayBackState
	{
		PLAYBACK_STATE_BEGIN = 0,
		PLAYBACK_STATE_DELAYED = 1,
		PLAYBACK_STATE_PLAYING = 2,
		PLAYBACK_STATE_PAUSED = 3,
		PLAYBACK_STATE_COMPLETED = 4,
		PLAYBACK_STATE_CANCELLED = 5,
	};
	extern const char* const sTweenBase;
	class TweenBase : public DescribedNonCreatable<TweenBase, Instance, sTweenBase, Reflection::ClassDescriptor::RUNTIME_LOCAL>
	{
		// Internal
		shared_ptr<const Reflection::ValueArray> propTable;
		shared_ptr<Instance> instance;
		TweenInfo twnInfo;
		float elapsedTime;
		bool isLive;

		// For reflection
		PlayBackState state;

	public:
		TweenBase(shared_ptr<Instance> instance, TweenInfo twnInfo, shared_ptr<const Reflection::ValueArray> propTable);

		void cancel();
		void play();
		void pause();

		arl::signal<void(PlayBackState)> completedSignal;

		float getElapsedTime() const
		{
			return elapsedTime;
		}
		void setElapsedTime(float value);

		shared_ptr<const Reflection::ValueArray> getPropTable() const
		{
			return propTable;
		}

		PlayBackState getPlaybackState() const
		{
			return state;
		}
		void setPlaybackState(PlayBackState value);

		TweenInfo getTweenInfo() const
		{
			return twnInfo;
		}
		shared_ptr<Instance> getInstance() const
		{
			return instance;
		}

		bool isDone() const
		{
			return elapsedTime >= twnInfo.getTime();
		}

		bool isPlaying() const
		{
			return isLive;
		}
		void setIsPlaying(bool value);
	};
} // namespace ARL