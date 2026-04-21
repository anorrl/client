#pragma once

namespace ARL
{
	class IProgressIndicator
	{
	public:
		// returns true if cancel requested.
		virtual bool setProgess(float percent) { return step(); }; // optional
		virtual bool step() = 0;
	};
}