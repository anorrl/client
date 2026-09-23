#pragma once

#include "v8world/Poly.h"
#include "v8world/GeometryPool.h"
#include "v8world/PrismMesh.h"
#include "v8world/BlockMesh.h"

namespace ARL {

	class PrismPoly : public Poly {
	private:
		typedef GeometryPool<Vector3_2Ints, POLY::PrismMesh, Vector3_2IntsComparer> PrismMeshPool;
		PrismMeshPool::Token prismMesh;

		int numSides;
		int numSlices;

		void setNumSides( int num );
		void setNumSlices( int num );
		/*override*/ bool isGeometryOrthogonal( void ) const { return false; }

	protected:
		// Geometry Overrides
		/*override*/ virtual GeometryType getGeometryType() const	{return GEOMETRY_PRISM;}
		/*override*/ void setGeometryParameter(const std::string& parameter, int value);
		/*override*/ int getGeometryParameter(const std::string& parameter) const;

		/*override*/ Matrix3 getMoment(float mass) const;
		/*override*/ Vector3 getCofmOffset() const;
		/*override*/ CoordinateFrame getSurfaceCoordInBody( const size_t surfaceId ) const;
		/*override*/ size_t getFaceFromLegacyNormalId( const NormalId nId ) const;
	
		// Poly Overrides
		/*override*/ void buildMesh();

	public:
		PrismPoly() : numSides(0), numSlices(0)
		{}

		/*override*/ bool setUpBulletCollisionData(void) { return false; }

	};

} // namespace
