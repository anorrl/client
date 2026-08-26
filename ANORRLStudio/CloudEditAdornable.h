#pragma once

#include "GfxBase/IAdornable.h"
#include "GfxBase/Part.h"
#include "V8Tree/Instance.h"
#include "V8DataModel/DataModel.h"

#include <boost/shared_ptr.hpp>

namespace ARL
{
	class Camera;
}

class PlayersDataManager;

class CloudEditAdornable : public ARL::IAdornable
{
	shared_ptr<ARL::DataModel>     m_pDataModel;
	shared_ptr<PlayersDataManager> m_pPlayersDataManager;
		
	static void renderSelection(ARL::Color3 playerColor,
		shared_ptr<ARL::Instance> instance, ARL::Part part, float lineThickness, ARL::Adorn* adorn);
	static void renderCamera(ARL::Color3 playerColor, shared_ptr<ARL::Network::Player> player,
		const ARL::Camera* localCamera, ARL::Adorn* adorn);
	
protected:
	bool shouldRender3dAdorn() const override { return true; }

public:
	void attach(shared_ptr<ARL::DataModel> dataModel, shared_ptr<PlayersDataManager> playersDataManager); // requires DataModel write lock is held
	void render3dAdorn(ARL::Adorn* adorn) override;
};
