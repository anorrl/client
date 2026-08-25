#pragma once

#include <vector>

namespace ARL {

#pragma pack( push, 1)
	// nb: keep backward/forward compatibility by only appending to these structs. 
	// stride information will keep this working.
	struct FileMeshHeaderLOD
	{
		/** size of header */
		unsigned short cbSize;
		/** size of vertices */
		unsigned char cbVerticesStride;
		/** size of faces */
		unsigned char cbFaceStride;
		unsigned short cbLODStride; // Mesh 3.00
		// ---dword boundary-----
		unsigned short num_LODOffsets; // Mesh 3.00
		unsigned int num_vertices;
		unsigned int num_faces;
	};

	struct FileMeshHeaderV4
	{
		unsigned short cbSize;
		unsigned short lodType;

		unsigned int num_vertices;
		unsigned int num_faces;

		unsigned short num_LODOffsets;
		unsigned short num_joints;

		unsigned int num_jointNameChars;
		unsigned short num_skinningSubsets;

		char num_highQualityLODs;
		char padding;

		unsigned int facsDataFormat; // Mesh 5.00
		unsigned int num_facsData; // Mesh 5.00
	};

	struct FileMeshVertexNormalTexture3d
	{
		float vx, vy, vz;
		float nx, ny, nz;
		float tu, tv, tw;
		char r, g, b, a; // color
	};

	struct FileMeshFace
	{
		unsigned int a;
		unsigned int b;
		unsigned int c;
	};

	enum FileMeshLodType // Mesh 4.00
	{
		None = 0,
		Unknown = 1,
		RbxSimplifier = 2,
		ZeuxMeshOptimizer = 3
	};

	struct FileMeshEnvelope // Mesh 4.00
	{
		char bones[4];
		char weights[4];
	};

	struct FileMeshBone // Mesh 4.00
	{
		unsigned int boneNameIndex;

		unsigned short parentIndex;
		unsigned short lodParentIndex;

		float culling;

		float r00, r01, r02;
		float r10, r11, r12;
		float r20, r21, r22;

		float x, y, z;
	};

	struct FileMeshSubset
	{
		unsigned int facesBegin;
		unsigned int facesLength;

		unsigned int vertsBegin;
		unsigned int vertsLength;

		unsigned int numBoneIndices;
		unsigned short boneIndices[26];
	};

	struct FileMeshFacsData // Mesh 5.00
	{
		unsigned int sizeof_faceBoneNamesBuffer;
		unsigned int sizeof_faceControlNamesBuffer;
		unsigned int sizeof_quantizedTransformsBuffer;

		unsigned int unknown_offset;
		unsigned int sizeof_twoPoseCorrectivesBuffer;
		unsigned int sizeof_threePoseCorrectivesBuffer;

		std::vector<char> faceBoneNamesBuffer;
		std::vector<char> faceControlNamesBuffer;
		std::vector<char> quantizedTransformsBuffer;

		std::vector<char> twoPoseCorrectivesBuffer;
		std::vector<char> threePoseCorrectivesBuffer;
	};

	struct FileMeshQuantizedMatrix // Mesh 5.00
	{
		char sizeof_ushort;
		char padding;

		unsigned int rows, cols;
		float min, max;

		std::vector<std::vector<short>> matrix;
	};

	struct FileMeshQuantizedTransforms // Mesh 5.00
	{
		FileMeshQuantizedMatrix px;
		FileMeshQuantizedMatrix py;
		FileMeshQuantizedMatrix pz;

		FileMeshQuantizedMatrix rx;
		FileMeshQuantizedMatrix ry;
		FileMeshQuantizedMatrix rz;
	};

	struct FileMeshTwoPoseCorrective
	{
		unsigned short controlIndex0;
		unsigned short controlIndex1;
	};

	struct FileMeshThreePoseCorrective
	{
		unsigned short controlIndex0;
		unsigned short controlIndex1;
		unsigned short controlIndex2;
	};

#pragma pack( pop )

}

