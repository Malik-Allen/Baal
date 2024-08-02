// MIT License, Copyright (c) 2024 Malik Allen

#include "MeshHandler.h"

#include "../src/core/3d/Mesh.h"
#include "../src/core/vulkan/resource/Allocator.h"
#include "../src/utility/DebugLog.h"
#include <cassert>
#include <algorithm>

namespace Baal
{
	namespace VK
	{
		MeshHandler::MeshHandler()
		{
		}

		MeshHandler::~MeshHandler()
		{
			subMeshInstances.clear();
			meshInstances.clear();
			loadedMeshMap.clear();
		}

		std::shared_ptr<Mesh> MeshHandler::LoadMeshResource(const char* parentDirectory, const char* meshFileName)
		{
			const std::string path = std::string(parentDirectory);
			const std::string fileName = std::string(meshFileName);
			const std::string completeFilePath = path + fileName;

			DEBUG_LOG(LOG::INFO, "Looking up existing Mesh Resource for: {}", completeFilePath);

			std::shared_ptr<Mesh> mesh = loadedMeshMap[fileName];

			if (mesh == nullptr)
			{
				DEBUG_LOG(LOG::INFO, "Could not find existing Mesh Resource for {}... Attempting to create new Mesh Resource!", fileName);
				mesh = std::make_shared<Mesh>(parentDirectory, meshFileName);
				assert(mesh != nullptr);
				loadedMeshMap[fileName] = mesh;
				return mesh;
			}

			DEBUG_LOG(LOG::INFO, "Found existing Mesh Resource for {}", fileName);
			return mesh;
		}

		std::shared_ptr<MeshInstance> MeshHandler::CreateMeshInstance(Allocator& allocator, Mesh& resource)
		{
			const uint32_t id = static_cast<uint32_t>(meshInstances.size());
			std::shared_ptr<MeshInstance> instance = std::make_shared<MeshInstance>(allocator, resource, id);
			assert(instance != nullptr);
			meshInstances.push_back(instance);
			subMeshInstances.insert(subMeshInstances.end(), instance->subMeshes.begin(), instance->subMeshes.end());
			return instance;
		}


	}
}