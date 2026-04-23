#pragma once

#include <stdexcept>

namespace ARL {

class initialization_error : public std::runtime_error {
public:
	initialization_error(const char* const errorMessage) :
	  std::runtime_error(errorMessage) {}
};

}