// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_VK_MESHHANDLER_H
#define BAAL_VK_MESHHANDLER_H

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

		class MeshHandler
		{
			std::unordered_map<std::string, std::shared_ptr<Mesh>> loadedMeshMap;
			std::vector<std::shared_ptr<MeshInstance>> meshInstances;
			std::vector<std::shared_ptr<SubMeshInstance>> subMeshInstances;

		public:
			MeshHandler();
			MeshHandler(const MeshHandler&) = delete;
			MeshHandler(MeshHandler&&) = delete;

			~MeshHandler();

			MeshHandler& operator=(const MeshHandler&) = delete;
			MeshHandler& operator = (MeshHandler&&) = delete;

			std::shared_ptr<Mesh> LoadMeshResource(const char* parentDirectory, const char* meshFileName);
			std::shared_ptr<MeshInstance> CreateMeshInstance(Allocator& allocator, Mesh& resource);

			std::vector<std::shared_ptr<MeshInstance>>& GetMeshInstances() { return meshInstances; }
			std::vector<std::shared_ptr<SubMeshInstance>>& GetSubMeshInstances() { return subMeshInstances; }
		};
	}
}

#endif // !BAAL_VK_MESHHANDLER_H