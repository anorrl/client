#include "stdafx.h"

#include "util/SystemAddress.h"

namespace ARL { 

bool SystemAddress::operator==( const ARL::SystemAddress& right ) const
{
	return binaryAddress == right.binaryAddress && port == right.port;
}

bool SystemAddress::operator!=( const ARL::SystemAddress& right ) const
{
	return binaryAddress != right.binaryAddress || port != right.port;
}

bool SystemAddress::operator>( const ARL::SystemAddress& right ) const
{
	return ( ( binaryAddress > right.binaryAddress ) || ( ( binaryAddress == right.binaryAddress ) && ( port > right.port ) ) );
}

bool SystemAddress::operator<( const ARL::SystemAddress& right ) const
{
	return ( ( binaryAddress < right.binaryAddress ) || ( ( binaryAddress == right.binaryAddress ) && ( port < right.port ) ) );
}

}	// namespace

