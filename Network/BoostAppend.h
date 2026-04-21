#pragma once

#include "rbx/boost.hpp"


namespace ARL {
	class PartInstance;
}


namespace boost {
	std::size_t hash_value(const shared_ptr<ARL::PartInstance>& b);
}

