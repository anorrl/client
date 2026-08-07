#include "stdafx.h"

#include "V8DataModel/TextService.h"

FASTFLAGVARIABLE(TypesettersReleaseResources, true);

namespace ARL
{
const char* const sTextService = "TextService";

namespace Reflection {
template<>
EnumDesc<TextService::FontSize>::EnumDesc()
:EnumDescriptor("FontSize")
{
	addPair(TextService::SIZE_8 , "Size8");
	addPair(TextService::SIZE_9 , "Size9");
	addPair(TextService::SIZE_10, "Size10");
	addPair(TextService::SIZE_11, "Size11");
	addPair(TextService::SIZE_12, "Size12");
	addPair(TextService::SIZE_14, "Size14");
	addPair(TextService::SIZE_18, "Size18");
	addPair(TextService::SIZE_24, "Size24");
	addPair(TextService::SIZE_36, "Size36");
	addPair(TextService::SIZE_48, "Size48");
	addPair(TextService::SIZE_28, "Size28");
	addPair(TextService::SIZE_32, "Size32");
	addPair(TextService::SIZE_42, "Size42");
	addPair(TextService::SIZE_60, "Size60");
	addPair(TextService::SIZE_96, "Size96");
}


template<>
EnumDesc<TextService::Font>::EnumDesc()
:EnumDescriptor("Font")
{
	addPair(TextService::FONT_LEGACY, "Legacy");
	addPair(TextService::FONT_ARIAL, "Arial");
	addPair(TextService::FONT_ARIALBOLD, "ArialBold");
	addPair(TextService::FONT_SOURCESANS, "SourceSans");
	addPair(TextService::FONT_SOURCESANSBOLD, "SourceSansBold");
	addPair(TextService::FONT_SOURCESANSLIGHT, "SourceSansLight");
	addPair(TextService::FONT_SOURCESANSITALIC, "SourceSansItalic");
	addPair(TextService::FONT_ANTIQUE, "Antique");
	addPair(TextService::FONT_ARCADE, "Arcade");
	addPair(TextService::FONT_BODONI, "Bodoni");
	addPair(TextService::FONT_CARTOON, "Cartoon");
	addPair(TextService::FONT_CODE, "Code");
	addPair(TextService::FONT_FANTASY, "Fantasy");
	addPair(TextService::FONT_GARAMOND, "Garamond");
	addPair(TextService::FONT_HIGHWAY, "Highway");
	addPair(TextService::FONT_SCIFI, "SciFi");
	addPair(TextService::FONT_FINGERPAINT, "Fingerpaint");
	addPair(TextService::FONT_COMICSANS, "ComicSans");
	addPair(TextService::FONT_COMICSANSBOLD, "ComicSansBold");
	addPair(TextService::FONT_SILKSCREEN, "SilkScreen");
	addPair(TextService::FONT_PROXIMANOVA, "ProximaNova");
	addPair(TextService::FONT_NOTOSANS, "NotoSans");
	addPair(TextService::FONT_NOTOSANSBOLD, "NotoSansBold");
	addPair(TextService::FONT_VCR, "VCR");
	addPair(TextService::FONT_DASHHORIZON, "DashHorizon");
	addPair(TextService::FONT_COURIERNEW, "CourierNew");
	addPair(TextService::FONT_ZENITH, "Zenith");
	addPair(TextService::FONT_KOSMOS, "Kosmos");
}

template<>
TextService::Font& Variant::convert<TextService::Font>(void)
{
	return genericConvert<TextService::Font>();
}

template<>
EnumDesc<TextService::XAlignment>::EnumDesc()
:EnumDescriptor("TextXAlignment")
{
	addPair(TextService::XALIGNMENT_LEFT,   "Left");
	addPair(TextService::XALIGNMENT_CENTER, "Center");
	addPair(TextService::XALIGNMENT_RIGHT,  "Right");
}

template<>
EnumDesc<TextService::YAlignment>::EnumDesc()
:EnumDescriptor("TextYAlignment")
{
	addPair(TextService::YALIGNMENT_TOP,	"Top");
	addPair(TextService::YALIGNMENT_CENTER,"Center");
	addPair(TextService::YALIGNMENT_BOTTOM,"Bottom");
}
}//namespace Reflection

template<>
bool StringConverter<TextService::Font>::convertToValue(const std::string& text, TextService::Font& value)
{
	return Reflection::EnumDesc<TextService::Font>::singleton().convertToValue(text.c_str(),value);
}

static Reflection::BoundFuncDesc<TextService, Vector2(std::string, int, TextService::Font, Vector2)> func_getTextSize(&TextService::getTextSize, "GetTextSize", "string", "fontSize", "font", "frameSize", Security::ANORRLScript);

TextService::Font TextService::FromTextFont(Text::Font font)
{
	switch(font)
	{
	case Text::FONT_LEGACY:		return FONT_LEGACY;
	case Text::FONT_ARIAL:		return FONT_ARIAL;
	case Text::FONT_ARIALBOLD:	return FONT_ARIALBOLD;
	case Text::FONT_SOURCESANS:		return FONT_SOURCESANS;
	case Text::FONT_SOURCESANSBOLD:	return FONT_SOURCESANSBOLD;
	case Text::FONT_SOURCESANSLIGHT:	return FONT_SOURCESANSLIGHT;
	case Text::FONT_SOURCESANSITALIC:	return FONT_SOURCESANSITALIC;
	case Text::FONT_ANTIQUE:	return FONT_ANTIQUE;
	case Text::FONT_ARCADE:	return FONT_ARCADE;
	case Text::FONT_BODONI:	return FONT_BODONI;
	case Text::FONT_CARTOON:	return FONT_CARTOON;
	case Text::FONT_CODE:	return FONT_CODE;
	case Text::FONT_FANTASY:	return FONT_FANTASY;
	case Text::FONT_GARAMOND:	return FONT_GARAMOND;
	case Text::FONT_HIGHWAY:	return FONT_HIGHWAY;
	case Text::FONT_SCIFI:	return FONT_SCIFI;
	case Text::FONT_FINGERPAINT:	return FONT_FINGERPAINT;

	case Text::FONT_COMICSANS:	return FONT_COMICSANS;
	case Text::FONT_COMICSANSBOLD:	return FONT_COMICSANSBOLD;
	case Text::FONT_SILKSCREEN:	return FONT_SILKSCREEN;
	case Text::FONT_PROXIMANOVA:	return FONT_PROXIMANOVA; 
	case Text::FONT_NOTOSANS:	return FONT_NOTOSANS;
	case Text::FONT_NOTOSANSBOLD:	return FONT_NOTOSANSBOLD;

	case Text::FONT_VCR:	return FONT_VCR;
	case Text::FONT_DASHHORIZON:	return FONT_DASHHORIZON;
	case Text::FONT_PUNKTYPE:	return FONT_PUNKTYPE;

	case Text::FONT_COURIERNEW:	return FONT_COURIERNEW;
	case Text::FONT_ZENITH: return FONT_ZENITH;
	case Text::FONT_KOSMOS: return FONT_KOSMOS;

	default:
		ARLASSERT(0);
		return FONT_LEGACY;
	}
}
Text::Font TextService::ToTextFont(Font font)
{
	switch(font)
	{
	case FONT_LEGACY:		return Text::FONT_LEGACY;
	case FONT_ARIAL:		return Text::FONT_ARIAL;
	case FONT_ARIALBOLD:	return Text::FONT_ARIALBOLD;
	case FONT_SOURCESANS:		return Text::FONT_SOURCESANS;
	case FONT_SOURCESANSBOLD:	return Text::FONT_SOURCESANSBOLD;
	case FONT_SOURCESANSLIGHT:	return Text::FONT_SOURCESANSLIGHT;
	case FONT_SOURCESANSITALIC:	return Text::FONT_SOURCESANSITALIC;
	case FONT_ANTIQUE:	return Text::FONT_ANTIQUE;
	case FONT_ARCADE:	return Text::FONT_ARCADE;
	case FONT_BODONI:	return Text::FONT_BODONI;
	case FONT_CARTOON:	return Text::FONT_CARTOON;
	case FONT_CODE:		return Text::FONT_CODE;
	case FONT_FANTASY:	return Text::FONT_FANTASY;
	case FONT_GARAMOND:	return Text::FONT_GARAMOND;
	case FONT_HIGHWAY:	return Text::FONT_HIGHWAY;
	case FONT_SCIFI:	return Text::FONT_SCIFI;
	case FONT_FINGERPAINT:	return Text::FONT_FINGERPAINT;
	case FONT_COMICSANS:	return Text::FONT_COMICSANS;
	case FONT_COMICSANSBOLD:	return Text::FONT_COMICSANSBOLD;
	case FONT_SILKSCREEN:	return Text::FONT_SILKSCREEN;
	case FONT_PROXIMANOVA:	return Text::FONT_PROXIMANOVA; 
	case FONT_NOTOSANS:	return Text::FONT_NOTOSANS;
	case FONT_NOTOSANSBOLD:	return Text::FONT_NOTOSANSBOLD;
	case FONT_VCR:	return Text::FONT_VCR;
	case FONT_DASHHORIZON:	return Text::FONT_DASHHORIZON;
	case FONT_PUNKTYPE:	return Text::FONT_PUNKTYPE;
	case FONT_COURIERNEW:	return Text::FONT_COURIERNEW;
	case FONT_ZENITH:	return Text::FONT_ZENITH;
	case FONT_KOSMOS:	return Text::FONT_KOSMOS;

	default:
		ARLASSERT(0);
		return Text::FONT_LEGACY;
	}
}

Text::XAlign TextService::ToTextXAlign(XAlignment xalign)
{

	switch(xalign){
		case TextService::XALIGNMENT_LEFT:
			return Text::XALIGN_LEFT;		
		case TextService::XALIGNMENT_RIGHT:
			return Text::XALIGN_RIGHT;	
		case TextService::XALIGNMENT_CENTER:
			return Text::XALIGN_CENTER;	
		default:
			ARLASSERT(0);
			return Text::XALIGN_LEFT;
	}
}
Text::YAlign TextService::ToTextYAlign(YAlignment yalign)
{
	switch(yalign){
		case TextService::YALIGNMENT_TOP:
			return Text::YALIGN_TOP;
		case TextService::YALIGNMENT_CENTER:
			return Text::YALIGN_CENTER;	
		case TextService::YALIGNMENT_BOTTOM:
			return Text::YALIGN_BOTTOM;	
		default:
			ARLASSERT(0);
			return Text::YALIGN_TOP;
	}
}

TextService::TextService()
	:Super()
{
	this->setName(sTextService);

	clearTypesetters();
}

void TextService::clearTypesetters()
{
    if (FFlag::TypesettersReleaseResources)
    {
        if (m_typesetters.get())
        {
            for (size_t i = 0; i < FONT_LAST; ++i)
            {
                m_typesetters[i].get()->releaseResources();
            }
        }
        else
        {
            m_typesetters.reset(new shared_ptr<Typesetter>[FONT_LAST]);
        }
    }
    else
    {
        m_typesetters.reset(new shared_ptr<Typesetter>[FONT_LAST]);
    }
}
void TextService::registerTypesetter(Font font, shared_ptr<ARL::Typesetter> typesetter)
{
	ARLASSERT(font < FONT_LAST);
	m_typesetters[font] = typesetter;
}

Typesetter* TextService::getTypesetter(Font font)
{
	ARLASSERT(font < FONT_LAST);
	return m_typesetters[font].get();
}

Vector2 TextService::getTextSize(std::string text, int fontSize, Font font, Vector2 frameSize)
{
	if (font >= FONT_LAST || font < FONT_LEGACY)
	{
		return Vector2::zero();
	}

	if (Typesetter* typesetter = getTypesetter(font))
	{
		return typesetter->measure(text, (float) fontSize, frameSize);
	}

	return Vector2::zero();
}

}
