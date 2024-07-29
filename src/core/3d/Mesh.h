// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_VK_MESH_H
#define BAAL_VK_MESH_H

#include <vector>
#include <memory>
#include <Mjolnir.h>

namespace Baal
{
	namespace VK
	{
		class Buffer;
		class LogicalDevice;

		struct Vertex
		{
			Vector3f pos;
			Vector3f norm;
			Vector2f texCoords;
			Vector3f color;
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
			std::vector<std::unique_ptr<SubMesh>> subMeshes;
			std::vector<std::unique_ptr<Buffer>> vertexBuffers;
			std::vector<std::unique_ptr<Buffer>> indexBuffers;

		public:
			explicit Mesh(const char* parentDirectory, const char* meshFileName);
			Mesh(const Mesh&) = delete;
			Mesh(Mesh&&) = delete;

			~Mesh();

			Mesh& operator=(const Mesh&) = delete;
			Mesh& operator = (Mesh&&) = delete;

			void CreateBuffers(LogicalDevice& device);
		};
	}	
}

#endif // !BAAL_VK_MESH_H