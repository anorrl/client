#pragma once


namespace RBX {

/* to-do: add v3 and v4 */
/* what the fuck is LOD? (jk) */
#pragma pack( push, 1)
	// nb: keep backward/forward compatibility by only appending to these structs. 
	// stride information will keep this working.
	struct FileMeshHeader
	{
		/** size of header */
		unsigned short cbSize;
		/** size of vertices */
		unsigned char cbVerticesStride;
		/** size of faces */
		unsigned char cbFaceStride;
		// ---dword boundary-----
		unsigned int num_vertices;
		unsigned int num_faces;
	};

	struct FileMeshVertexNormalTexture3d
	{
		float vx,vy,vz;
		float nx,ny,nz;
		float tu,tv,tw;
	};

	struct FileMeshFace
	{
		unsigned int a;
		unsigned int b;
		unsigned int c;
	};

#pragma pack( pop )

}

