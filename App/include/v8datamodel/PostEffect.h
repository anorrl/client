#pragma once

#include "v8tree/Instance.h"

namespace ARL {

	extern const char* const sPostEffect;
	class PostEffect : public DescribedNonCreatable<PostEffect, Instance, sPostEffect>
	{
	private:
		bool enabled;
	public:
		PostEffect();
		bool isEnabled() const { return enabled; }
		void setIsEnabled(bool value);
	};

	extern const char* const sBlurEffect;
	class BlurEffect : public DescribedCreatable<BlurEffect, PostEffect, sBlurEffect>
	{
	private:
		float size;
	public:
		BlurEffect();

		float getSize() const { return size; }
		void setSize(float value);
	};

	extern const char* const sColorCorrectionEffect;
	class ColorCorrectionEffect : public DescribedCreatable<ColorCorrectionEffect, PostEffect, sColorCorrectionEffect>
	{
	private:
		float brightness;
		float contrast;
		float saturation;
		G3D::Color3 tintColor;
	public:
		ColorCorrectionEffect();

		float getBrightness() const { return brightness; }
		float getContrast() const { return contrast; }
		float getSaturation() const { return saturation; }
		G3D::Color3 getTintColor() const { return tintColor; }

		void setBrightness(float value);
		void setContrast(float value);
		void setSaturation(float value);
		void setTintColor(G3D::Color3 value);
	};
}