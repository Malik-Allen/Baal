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
		class DescriptorSet;
		class LogicalDevice;
		class DescriptorPool;
		class DescriptorSetLayout;

		struct Vertex
		{
			Vector3f pos;
			Vector3f norm;
			Vector2f texCoords;
			Vector3f color;
		};

		struct Material
		{
			alignas(16) Vector4f ambient;
			Vector3f diffuse;
			alignas(16) Vector3f specular;
			float shininess = 3.0f;
		};

		struct SubMesh
		{
			std::vector<Vertex> vertices;
			std::vector<int> indices;
			Material material;
		};

		struct VertexPushConstants
		{
			Matrix4f model;
		};

		struct FragmentPushConstants
		{
			Material material;
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

		class SubMeshInstance
		{
			friend class MeshHandler;
			friend class MeshInstance;

			uint32_t id;
			uint32_t parentId;
			std::unique_ptr<Buffer> vertexBuffer;
			std::unique_ptr<Buffer> indexBuffer;
			uint32_t indexCount;
			Material material;

		public:
			explicit SubMeshInstance(const uint32_t _id, const uint32_t _parentId);
			SubMeshInstance(const SubMeshInstance&) = delete;
			SubMeshInstance(SubMeshInstance&&) noexcept = delete;

			~SubMeshInstance();

			SubMeshInstance& operator=(const SubMeshInstance&) = delete;
			SubMeshInstance& operator = (SubMeshInstance&&) = delete;

			Buffer& GetVertexBuffer() { return *vertexBuffer.get(); }
			Buffer& GetIndexBuffer() { return *indexBuffer.get(); }
			uint32_t GetIndexCount() const { return indexCount; }
			uint32_t GetId() const { return id; }
			uint32_t GetParentId() const { return parentId; }
			Material& GetMaterial() { return material; }
		};

		class MeshInstance
		{
			friend class MeshHandler;

			uint32_t id;
			std::vector<std::shared_ptr<SubMeshInstance>> subMeshes;

		public:
			explicit MeshInstance(LogicalDevice& device, Mesh& resource, const uint32_t _id);
			MeshInstance(const MeshInstance&) = delete;
			MeshInstance(MeshInstance&&) noexcept = delete;

			~MeshInstance();

			MeshInstance& operator=(const MeshInstance&) = delete;
			MeshInstance& operator = (MeshInstance&&) = delete;

			Matrix4f model;
		};
	}	
}

#endif // !BAAL_VK_MESH_H