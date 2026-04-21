/* Copyright 2003-2006 ROBLOX Corporation, All Rights Reserved */

#pragma once

#include "rbx/Debug.h"

namespace ARL {

	class KernelIndex {
	protected:
		int		kernelIndex;

	public:
		bool indexInKernel() const {
			return (kernelIndex != -1);
		}

		KernelIndex() : kernelIndex(-1)
		{}

		~KernelIndex() {
			ARLASSERT(!indexInKernel());
		}
	};

} // namespace