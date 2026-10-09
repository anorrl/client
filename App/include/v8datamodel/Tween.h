#pragma once
#include "v8tree/Instance.h"
#include "util/TweenInfo.h"
#include "v8datamodel/TweenBase.h"
#include "util/UDim.h"

namespace ARL
{
	extern const char* const sTween;
	class Tween : public DescribedNonCreatable<Tween, TweenBase, sTween, Reflection::ClassDescriptor::RUNTIME_LOCAL>
	{
	private:
		TweenInfo twnInfo;
		shared_ptr<Instance> instance;

	public:
		Tween(shared_ptr<Instance> instance, TweenInfo twnInfo, shared_ptr<const Reflection::ValueArray> propTable);

		Instance* getInstance() const
		{
			return instance.get();
		}
		TweenInfo getTweenInfo() const
		{
			return twnInfo;
		}
	};

}; // Namespace ARL