#pragma once

/*
	Utility class - holds RightAngleRamp Meshes of same size for use by Geometry Pool.
*/

#include "util/Memory.h"
#include "v8world/Mesh.h"


namespace ARL {

	namespace POLY {

		class RightAngleRampMesh : public Allocator<RightAngleRampMesh>
		{
		private:
			Mesh mesh;
			Vector3 LocalCofM;

		public:
			RightAngleRampMesh(const Vector3& size) 
			{
				mesh.makeRightAngleRamp(size, LocalCofM);
			}
			const Mesh* getMesh() const {return &mesh;}
			const Vector3& GetLocalCofMFromMesh() const { return LocalCofM; }
		};

	} // namespace POLY
} // namespace ARL
