#pragma once
#include "v8tree/instance.h"
#include "v8tree/service.h"

namespace G3D
{
	class BinaryOutput;
};

namespace ARL
{
	class ContentProvider;
    class ViewBase;
}

extern const char* const sThumbnailGenerator;
class ThumbnailGenerator 
	: public ARL::DescribedCreatable<ThumbnailGenerator, ARL::Instance, sThumbnailGenerator>
	, public ARL::Service
{
public:
	int graphicsMode;
	static volatile long totalCount;
	
	ThumbnailGenerator(void);
	~ThumbnailGenerator(void);
	shared_ptr<const ARL::Reflection::Tuple> click(std::string fileType, int cx, int cy, bool hideSky,bool crop);
	shared_ptr<const ARL::Reflection::Tuple> clickTexture(std::string textureId, std::string fileType, int cx, int cy);

	void renderThumb(ARL::ViewBase* view, void* windowHandle, std::string fileType, int cx, int cy, bool hideSky, bool crop, std::string* strOutput);
    void exportScene(ARL::ViewBase* view, std::string* outStr);

private:
	void configureCaches();
};
