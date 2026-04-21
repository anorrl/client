#include "stdafx.h"
#include "TextureRef.h"

#include "GfxCore/Texture.h"

namespace ARL
{
namespace Graphics
{

shared_ptr<Texture> TextureRef::emptyTexture;
ImageInfo TextureRef::emptyInfo;

TextureRef TextureRef::future(const shared_ptr<Texture>& fallback)
{
    Data* data = new Data();

    data->status = Status_Waiting;
	data->texture = fallback;

    return TextureRef(data);
}

TextureRef::TextureRef(Data* data)
	: data(data)
{
}

TextureRef::TextureRef(const shared_ptr<Texture>& texture, Status status)
    : data(new Data())
{
    if (texture)
	{
        data->status = status;
        data->texture = texture;
		data->info = ImageInfo(texture->getWidth(), texture->getHeight(), true);
	}
	else
	{
        data->status = Status_Null;
	}
}

TextureRef TextureRef::clone() const
{
    return data ? TextureRef(new Data(*data)) : TextureRef();
}

void TextureRef::updateAllRefs(const shared_ptr<Texture>& texture)
{
    ARLASSERT(data);

    data->texture = texture;
	data->info = texture ? ImageInfo(texture->getWidth(), texture->getHeight(), true) : ImageInfo();
}

void TextureRef::updateAllRefsToLoaded(const shared_ptr<Texture>& texture)
{
    ARLASSERT(texture);
    ARLASSERT(data);
	ARLASSERT(data->status == Status_Waiting);

    data->status = Status_Loaded;
    data->texture = texture;
    data->info = ImageInfo(texture->getWidth(), texture->getHeight(), true);
}

void TextureRef::updateAllRefsToLoaded(const shared_ptr<Texture>& texture, const ImageInfo& info)
{
    ARLASSERT(texture);
    ARLASSERT(data);
	ARLASSERT(data->status == Status_Waiting);

    data->status = Status_Loaded;
    data->texture = texture;
    data->info = info;
}

void TextureRef::updateAllRefsToFailed()
{
    ARLASSERT(data);
	ARLASSERT(data->status == Status_Waiting);

    data->status = Status_Failed;
}

void TextureRef::updateAllRefsToWaiting()
{
    ARLASSERT(data);

    data->status = Status_Waiting;
}

}
}
