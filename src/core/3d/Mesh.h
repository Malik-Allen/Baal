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
		class Allocator;

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
			friend class MeshInstance;
			std::vector<SubMesh> subMeshes;
		public:
			explicit Mesh(const char* parentDirectory, const char* meshFileName);
			Mesh(const Mesh&) = delete;
			Mesh(Mesh&&) = delete;

			~Mesh();

			Mesh& operator=(const Mesh&) = delete;
			Mesh& operator = (Mesh&&) = delete;
		};

		struct SubMeshInstance
		{
			uint32_t id;
			uint32_t parentId;
			std::unique_ptr<Buffer> vertexBuffer;
			std::unique_ptr<Buffer> indexBuffer;
			uint32_t indexCount;
		};
		
		struct MeshMatrices
		{
			Matrix4f model;
		};

		class MeshInstance
		{
			friend class MeshManager;
			uint32_t id;
			std::vector<std::shared_ptr<SubMeshInstance>> subMeshes;

			std::unique_ptr<Buffer> uniformBuffer;
		public:
			MeshMatrices matrices;

			explicit MeshInstance(Allocator& allocator, Mesh& resource, const uint32_t _id);
			MeshInstance(const MeshInstance&) = delete;
			MeshInstance(MeshInstance&&) noexcept = delete;

			~MeshInstance();

			MeshInstance& operator=(const MeshInstance&) = delete;
			MeshInstance& operator = (MeshInstance&&) = delete;

			void Update();

			Buffer& GetUniformBuffer() { return *uniformBuffer.get(); }
		};
	}	
}

#endif // !BAAL_VK_MESH_H