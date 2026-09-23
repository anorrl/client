#pragma once

#include "v8world/Poly.h"
#include "v8world/GeometryPool.h"
#include "v8world/ParallelRampMesh.h"
#include "v8world/BlockMesh.h"

namespace ARL {

	class ParallelRampPoly : public Poly {
	public:
		typedef GeometryPool<Vector3, POLY::ParallelRampMesh, Vector3Comparer> ParallelRampMeshPool;

		/*override*/ Matrix3 getMoment(float mass) const;
		/*override*/ Vector3 getCofmOffset() const;
		/*override*/ bool isGeometryOrthogonal( void ) const { return false; }
		/*override*/ bool setUpBulletCollisionData(void) { return false; }

	private:
		ParallelRampMeshPool::Token aParallelRampMesh;

	protected:
		// Geometry Overrides
		/*override*/ virtual GeometryType getGeometryType() const	{return GEOMETRY_PARALLELRAMP;}
		
		// Poly Overrides
		/*override*/ void buildMesh();
	};

} // namespace
