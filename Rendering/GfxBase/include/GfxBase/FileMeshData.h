#pragma once

#include "MeshFileStructs.h"

#include "util/Object.h"
#include "util/G3DCore.h"

#include <vector>

namespace ARL
{
	struct FileMeshData
	{
		std::vector<FileMeshVertexNormalTexture3d> vnts;
		std::vector<FileMeshEnvelope> envelopes; // Mesh 4.00
		std::vector<FileMeshFace> faces;
		AABox aabb;
		std::vector<unsigned int> LODOffsets; // Mesh 3.00
		std::vector<FileMeshBone> bones; // Mesh 4.00
		std::vector<char> boneNames; // Mesh 4.00
		std::vector<FileMeshSubset> subsets; // Mesh 4.00
		std::vector<char> facsData; // Mesh 5.00
	};

	shared_ptr<FileMeshData> ReadFileMesh(const std::string& data);

	// writes the newest version always.
	// remember: set ostream to binary!
	void WriteFileMesh(std::ostream& f, const FileMeshData& mesh);
}
