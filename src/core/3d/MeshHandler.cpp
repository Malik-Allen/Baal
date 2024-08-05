// MIT License, Copyright (c) 2024 Malik Allen

#include "MeshHandler.h"

#include "../src/core/3d/Mesh.h"
#include "../src/core/vulkan/devices/LogicalDevice.h"
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
			garbage.clear();
			subMeshInstances.clear();
			meshInstances.clear();
			loadedMeshMap.clear();
		}

		std::weak_ptr<Mesh> MeshHandler::LoadMeshResource(const char* parentDirectory, const char* meshFileName)
		{
			const std::string path = std::string(parentDirectory);
			const std::string fileName = std::string(meshFileName);
			const std::string completeFilePath = path + fileName;

			DEBUG_LOG(LOG::INFO, "Looking up existing Mesh Resource for: {}", completeFilePath);

			std::shared_ptr<Mesh> mesh = loadedMeshMap[completeFilePath];

			if (mesh == nullptr)
			{
				DEBUG_LOG(LOG::INFO, "Could not find existing Mesh Resource for {}... Attempting to create new Mesh Resource!", fileName);
				mesh = std::make_shared<Mesh>(parentDirectory, meshFileName);
				assert(mesh != nullptr);
				loadedMeshMap[completeFilePath] = mesh;
				return mesh;
			}

			DEBUG_LOG(LOG::INFO, "Found existing Mesh Resource for {}", fileName);
			return mesh;
		}

		std::weak_ptr<MeshInstance> MeshHandler::CreateMeshInstance(LogicalDevice& device, Mesh& resource)
		{
			const uint32_t id = static_cast<uint32_t>(meshInstances.size());
			std::shared_ptr<MeshInstance> instance = std::make_shared<MeshInstance>(device, resource, id);
			assert(instance != nullptr);
			meshInstances.push_back(instance);
			subMeshInstances.insert(subMeshInstances.end(), instance->subMeshes.begin(), instance->subMeshes.end());
			return instance;
		}

		void MeshHandler::DestroyMeshInstance(std::weak_ptr<MeshInstance> mesh)
		{
			if (mesh.expired())
			{
				return; // Exit! A non-valid mesh instance cannot be destroyed!
			}

			std::shared_ptr<MeshInstance> meshToDestroy = mesh.lock();

			const uint32_t lastIndex = meshInstances.size() - 1;
			std::shared_ptr<MeshInstance> lastMesh = meshInstances[lastIndex];

			lastMesh->id = meshToDestroy->id;	// Take the index for the mesh that is being removed from the vector
			for (size_t i = 0; i < lastMesh->subMeshes.size(); ++i)
			{
				lastMesh->subMeshes[i]->parentId = lastMesh->id;
			}
			
			meshInstances[lastMesh->id] = lastMesh;	// Move the last mesh into its new index.

			// Clean up
			meshInstances[lastIndex].reset();
			meshInstances.pop_back();

			// Destroy the target mesh
			garbage.push_back(meshToDestroy);
		}

		void MeshHandler::CollectSubMeshesToRender()
		{
			subMeshInstances.clear();

			for (size_t i = 0; i < meshInstances.size(); ++i)
			{
				for (size_t n = 0; n < meshInstances[i]->subMeshes.size(); ++n)
				{
					subMeshInstances.push_back(meshInstances[i]->subMeshes[n]);
				}
			}
		}

		bool MeshHandler::IsGarbageFull() const
		{
			return garbage.size() > 0;
		}

		void MeshHandler::TryEmptyGarbage()
		{
			if (IsGarbageFull())
			{
				garbage.clear();
			}
		}
	}
}