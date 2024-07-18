// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_MESH
#define BAAL_MESH

#include <vector>

#include <Mjolnir.h>

namespace Baal
{
	struct Vertex 
	{
		Vector3f pos;
		Vector3f norm;
		Vector2f texCoords;
	};

	struct SubMesh
	{
		std::vector<Vertex> vertices;
		std::vector<int> indices;
	};

	// Mesh is made up of multiple Sub Meshes / Shapes
	// SubMeshes can be used to assign different materiels, animations, textures, etc.
	class Mesh
	{
		std::vector<SubMesh> subMeshes;
	public:
		explicit Mesh(const char* parentDirectory, const char* meshFileName);
		Mesh(const Mesh&) = delete;
		Mesh(Mesh&&) = delete;

		~Mesh();

		Mesh& operator=(const Mesh&) = delete;
		Mesh& operator = (Mesh&&) = delete;
	};
}

#endif // !BAAL_MESH