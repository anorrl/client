#include "GuidRegistryService.h"

const char* const ARL::Network::sGuidRegistryService = "GuidRegistryService";

ARL::Network::GuidRegistryService::GuidRegistryService(void)
:registry(Registry::create())
{
}

ARL::Network::GuidRegistryService::~GuidRegistryService(void)
{
}
