// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_VK_MESH_MANAGER_H
#define BAAL_VK_MESH_MANAGER_H

#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

namespace Baal
{
	namespace VK
	{
		class Mesh;
		class MeshInstance;
		struct SubMeshInstance;
		class Allocator;

		class MeshManager
		{
			std::unordered_map<std::string, std::shared_ptr<Mesh>> loadedMeshMap;
			std::vector<std::shared_ptr<MeshInstance>> meshInstances;
			std::vector<std::shared_ptr<SubMeshInstance>> subMeshInstances;

		public:
			MeshManager();
			MeshManager(const MeshManager&) = delete;
			MeshManager(MeshManager&&) = delete;

			~MeshManager();

			MeshManager& operator=(const MeshManager&) = delete;
			MeshManager& operator = (MeshManager&&) = delete;

			std::shared_ptr<Mesh> LoadMeshResource(const char* parentDirectory, const char* meshFileName);
			std::shared_ptr<MeshInstance> CreateMeshInstance(Allocator& allocator, Mesh& resource);

			std::vector<std::shared_ptr<MeshInstance>>& GetMeshInstances() { return meshInstances; }
			std::vector<std::shared_ptr<SubMeshInstance>>& GetSubMeshInstances() { return subMeshInstances; }
		};
	}
}

#endif // !BAAL_VK_MESH_MANAGER_H