#include "stdafx.h"

#include "v8datamodel/team.h"
#include "V8DataModel/Workspace.h"
#include "V8DataModel/Teams.h"
#include "Humanoid/Humanoid.h"
#include "Network/Players.h"


namespace ARL {

	const char* const sTeam = "Team";

	REFLECTION_BEGIN();
	static const Reflection::PropDescriptor<Team, int> prop_Score("Score", category_Data, &Team::getScore, &Team::setScore, Reflection::PropertyDescriptor::Attributes::deprecated());
	static const Reflection::PropDescriptor<Team, BrickColor> prop_Color("TeamColor", category_Data, &Team::getTeamColor, &Team::setTeamColor);
	static const Reflection::PropDescriptor<Team, bool> prop_AutoAssignable("AutoAssignable", category_Data, &Team::getAutoAssignable, &Team::setAutoAssignable);
	Reflection::BoundProp<bool> Team::prop_AutoColorCharacters("AutoColorCharacters", category_Data, &Team::autoColorCharacters, Reflection::PropertyDescriptor::Attributes::deprecated());
	static Reflection::BoundFuncDesc<Team, shared_ptr<const Instances>()> func_GetPlayers(&Team::getPlayers, "GetPlayers", Security::None);
	static Reflection::EventDesc<Team, void(shared_ptr<Instance>)> event_PlayerAdded(&Team::playerAddedSignal, "PlayerAdded", "player");
	static Reflection::EventDesc<Team, void(shared_ptr<Instance>)> event_PlayerRemoving(&Team::playerRemovedSignal, "PlayerRemoved", "player");
	REFLECTION_END();

	Team::Team() :
		autoAssignable(true),
		score(0),
		autoColorCharacters(true)
	{
		setName(sTeam);
		color = BrickColor::brickWhite();
	}

	Team::~Team()
	{
	}

	bool Team::askSetParent(const Instance* parent) const
	{
		return Instance::fastDynamicCast<Teams>(parent) != NULL;
	}

	int Team::getScore() const
	{
		return score;
	}

	void Team::setScore(int score)
	{
		if (this->score != score) {
			this->score = score;
			raisePropertyChanged(prop_Score);
		}
	}

	BrickColor Team::getTeamColor() const
	{
		return color;
	}

	void Team::setTeamColor(BrickColor color)
	{
		if (this->color != color) {
			this->color = color;
			raisePropertyChanged(prop_Color);
		}
	}

	bool Team::getAutoAssignable() const { return autoAssignable; }
	void Team::setAutoAssignable(bool autoAssign) {
		if (this->autoAssignable != autoAssign)
		{
			this->autoAssignable = autoAssign;
			raisePropertyChanged(prop_AutoAssignable);
		}
	}

	void Team::playerAdded(Network::Player* player)
	{
		playerAddedSignal(shared_from(player));
	}

	void Team::playerRemoved(Network::Player* player)
	{
		playerRemovedSignal(shared_from(player));
	}

	shared_ptr<const Instances> Team::getPlayers()
	{
		Instance* parent = getParent();
		if (parent && !Instance::fastDynamicCast<Teams>(parent))
			throw std::runtime_error("Team:GetPlayers() failed because team must be a child of the Teams service");

		shared_ptr<Instances> result(new Instances());
		Network::Players* players = ServiceProvider::find<Network::Players>(this);

		for (unsigned int n = 0; n < players->numChildren(); n++)
		{
			Network::Player* p = Instance::fastDynamicCast<Network::Player>(players->getChild(n));
			if (p == NULL) continue; // only care about player objects
			if (p->getNeutral() == false && p->getTeamColor() == color) result->push_back(shared_from(p));

		}
		return result;
	}



} // namespace
