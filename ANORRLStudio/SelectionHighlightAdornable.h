#pragma once

#include "GfxBase/IAdornable.h"
#include "GfxBase/Part.h"
#include "V8DataModel/DataModel.h"

#include <boost/shared_ptr.hpp>

namespace ARL
{
	class Workspace;
}

class SelectionHighlightAdornable : public ARL::IAdornable
{
	static bool getSelectionDimensions(ARL::Workspace* workspace, shared_ptr<ARL::Instance> instance,
		ARL::Part* out, float* lineThickness, bool* checkChildren);
	static void render(shared_ptr<ARL::Instance> instance, ARL::Part part, float lineThickness, ARL::Adorn* adorn);

protected:
	bool shouldRender3dAdorn() const override { return true; }

public:
	template<class T>
	static void renderSelection(ARL::DataModel* dm, T& collection, ARL::Adorn* adorn,
		boost::function<void(shared_ptr<ARL::Instance>, ARL::Part, float, ARL::Adorn* adorn)> renderer)
	{
		using namespace ARL;
		for (auto& instance : collection)
		{
			Part size;
			float lineSize;
			bool checkChildren;
			if (getSelectionDimensions(dm->getWorkspace(), instance, &size, &lineSize, &checkChildren))
			{
				renderer(instance, size, lineSize, adorn);
			}
			if (checkChildren && instance->getChildren2())
			{
				renderSelection(dm, *instance->getChildren2(), adorn, renderer);
			}
		}
	}
	void render3dAdorn(ARL::Adorn* adorn) override;
};