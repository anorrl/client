#include "stdafx.h"
#include "util/Action.h"
#include "ARLAssert.h"
#include "reflection/EnumConverter.h"


namespace ARL 
{
	namespace Reflection
	{
		template<>
		EnumDesc<Action::ActionType>::EnumDesc()
			:EnumDescriptor("ActionType")
		{
			addPair(Action::NO_ACTION,"Nothing");
			addPair(Action::PAUSE_ACTION,"Pause");
			addPair(Action::LOSE_ACTION,"Lose");
			addPair(Action::DRAW_ACTION,"Draw");
			addPair(Action::WIN_ACTION,"Win");
		}
	} // namespace Reflection
}	// namespace ARL
