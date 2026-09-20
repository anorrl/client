#pragma once
#include <boost/functional/hash.hpp>

namespace ARL
{

	class TweenInfo
	{
	public:
		// Moved from GuiObject.h
		enum TweenEasingDirection
		{
			EASING_DIRECTION_IN,
			EASING_DIRECTION_OUT,
			EASING_DIRECTION_IN_OUT
		};

		enum TweenEasingStyle
		{
			EASING_STYLE_LINEAR,
			EASING_STYLE_SINE,
			EASING_STYLE_BACK,
			EASING_STYLE_QUAD,
			EASING_STYLE_QUART,
			EASING_STYLE_QUINT,
			EASING_STYLE_BOUNCE,
			EASING_STYLE_ELASTIC,
			EASING_STYLE_EXPONENTIAL,
			EASING_STYLE_CIRCULAR,
			EASING_STYLE_CUBIC,
		};
		enum TweenStatus
		{
			TWEEN_CANCELED,
			TWEEN_COMPLETED,
		};

	private:
		TweenEasingStyle style;
		TweenEasingDirection direction;
		int repeatCount;
		bool reverses;
		float time;
		float delayTime;
		float pauseTime;

	public:
		TweenInfo(TweenEasingStyle easingStyle_ = EASING_STYLE_QUAD, TweenEasingDirection easingDirection_ = EASING_DIRECTION_IN, int repeatCount_ = 0,
			bool reverses_ = false, float time_ = 1.0, float delayTime_ = 0.0, float pauseTime_ = 0.0)
			: style(easingStyle_)
			, direction(easingDirection_)
			, repeatCount(repeatCount_)
			, reverses(reverses_)
			, time(time_)
			, delayTime(delayTime_)
			, pauseTime(pauseTime_)
		{
		}

		size_t hashCode() const;

		float getTime() const
		{
			return time;
		}

		float getPauseTime() const
		{
			return pauseTime;
		}


		TweenEasingStyle getStyle() const
		{
			return style;
		}

		TweenEasingDirection getDirection() const
		{
			return direction;
		}

		int getRepeatCount() const
		{
			return repeatCount;
		}

		bool getReverses() const
		{
			return reverses;
		}

		float getDelayTime() const
		{
			return delayTime;
		}

		void setTime(float value)
		{
			if (time != value)
			{
				time = value;
			}
		}

		/*void setPauseTime(float value)
		{
		if (pauseTime != value)
		{
		pauseTime = value;
		}
		}*/

		void setStyle(TweenEasingStyle value)
		{
			if (style != value)
			{
				style = value;
			}
		}

		void setDirection(TweenEasingDirection value)
		{
			if (direction != value)
			{
				direction = value;
			}
		}

		void setRepeatCount(int value)
		{
			if (repeatCount != value)
			{
				repeatCount = value;
			}
		}

		void setReverses(bool value)
		{
			if (reverses != value)
			{
				reverses = value;
			}
		}

		void setDelayTime(float value)
		{
			if (delayTime != value)
			{
				delayTime = value;
			}
		}

		// Operators

		bool operator==(const TweenInfo& other) const
		{
			return (time == other.time) && (style == other.style) && (direction == other.direction) && (repeatCount == other.repeatCount) &&
				(reverses == other.reverses) && (delayTime == other.delayTime);
		}

		bool operator!=(const TweenInfo& other) const
		{
			return !(*this == other);
		}
	};

	size_t hash_value(const TweenInfo& properties);

}; // Namespace ARL