#pragma once

#include "v8datamodel/GuiObject.h"
#include "v8datamodel/GuiMixin.h"

namespace ARL
{
	extern const char* const sImageLabel;

	class ImageLabel 
		: public DescribedCreatable<ImageLabel, GuiLabel, sImageLabel>
		, public GuiImageMixin
	{	
	public:
		ImageLabel();

		DECLARE_GUI_IMAGE_MIXIN(ImageLabel);

	private:
		////////////////////////////////////////////////////////////////////////////////////
		// 
		// IAdornable
		/*override*/ void render2d(Adorn* adorn);
		/*override*/ void renderBackground2d(Adorn* adorn);
	};
}
