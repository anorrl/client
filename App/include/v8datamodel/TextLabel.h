#pragma once

#include "v8datamodel/GuiObject.h"
#include "v8datamodel/GuiText.h"

namespace ARL {

extern const char* const sTextLabel;

class TextLabel 
	: public DescribedCreatable<TextLabel, GuiLabel, sTextLabel>
	, public GuiTextMixin
{
	typedef DescribedCreatable<TextLabel, GuiLabel, sTextLabel> Super;

	////////////////////////////////////////////////////////////////////////////////////
	// 
	// IAdornable
	/*override*/ void render2d(Adorn* adorn);
	/*override*/ void render2dContext(Adorn* adorn, const Instance* context);

public:
	TextLabel();

	DECLARE_GUI_TEXT_MIXIN();

};

}  // namespace ARL
