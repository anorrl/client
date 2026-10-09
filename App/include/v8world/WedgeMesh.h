#pragma once

/*
	Utility class - holds Wedge Meshes of same size for use by Geometry Pool.
*/

#include "util/Memory.h"
#include "v8world/Mesh.h"


namespace ARL {

	namespace POLY {

		class WedgeMesh : public Allocator<WedgeMesh>
		{
		private:
			Mesh mesh;

		public:
			WedgeMesh(const Vector3& size) 
			{
				mesh.makeWedge(size);
			}
			const Mesh* getMesh() const {return &mesh;}
		};

	} // namespace POLY
} // namespace ARL
