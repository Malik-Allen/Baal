// MIT License, Copyright (c) 2024 Malik Allen

#include "Mesh.h"

#include "../src/utility/DebugLog.h"
#include "../src/core/vulkan/resource/Allocator.h"
#include "../src/core/vulkan/resource/Buffer.h"
#include "../src/core/vulkan/devices/LogicalDevice.h"
#include "../src/core/vulkan/descriptors/DescriptorSet.h"
#include "../src/core/vulkan/descriptors/DescriptorPool.h"
#include "../src/core/vulkan/descriptors/DescriptorSetLayout.h"

#include <string>
#include <tiny_obj_loader.h>

namespace Baal
{
	namespace VK
	{
		Mesh::Mesh(const char* parentDirectory, const char* meshFileName)
		{
			std::string meshFilePath = std::string(parentDirectory) + std::string(meshFileName);

			tinyobj::attrib_t attrib;
			std::vector<tinyobj::shape_t> shapes;
			std::vector<tinyobj::material_t> materials;

			std::string warn;
			std::string err;

			DEBUG_LOG(LOG::INFO, "Loading Mesh: {}...", meshFileName);

			const bool bSuccess = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, meshFilePath.c_str(), parentDirectory);
			if (!bSuccess)
			{
				DEBUG_LOG(LOG::ERRORLOG, "Failed to load Mesh: {}, Error: {}", meshFileName, err);
			}
			else
			{
				for (const auto& shape : shapes)
				{
					std::vector<Vertex> vertices;
					std::vector<int> indices;
					for (const auto& index : shape.mesh.indices)
					{
						Vertex v;
						v.pos.x = attrib.vertices[3 * index.vertex_index + 0];
						v.pos.y = attrib.vertices[3 * index.vertex_index + 1];
						v.pos.z = attrib.vertices[3 * index.vertex_index + 2];

						if (!attrib.normals.empty())
						{
							v.norm.x = attrib.normals[3 * index.normal_index + 0];
							v.norm.y = attrib.normals[3 * index.normal_index + 1];
							v.norm.z = attrib.normals[3 * index.normal_index + 2];
						}

						if (!attrib.texcoords.empty())
						{
							// The OBJ format assumes a coordinate system where a vertical coordinate of 0 means the bottom of the image.
							// Most textures that will be used will be a top to bottom orientation where 0 means the top of the image
							// So we flip the y-position, of the incoming text coords
							v.texCoords.x = attrib.texcoords[2 * index.texcoord_index + 0];
							v.texCoords.y = 1.0f - attrib.texcoords[2 * index.texcoord_index + 1];
						}						

						if (!attrib.colors.empty())
						{
							v.color.x = attrib.colors[3 * index.vertex_index + 0];
							v.color.y = attrib.colors[3 * index.vertex_index + 1];
							v.color.z = attrib.colors[3 * index.vertex_index + 2];
						}

						vertices.push_back(v);

						indices.push_back(indices.size());
					}

					subMeshes.push_back(SubMesh(vertices, indices));
				}

				if (!warn.empty())
				{
					DEBUG_LOG(LOG::WARNING, "Succesfully loaded Mesh: {}, Warning: {}", meshFileName, warn);
				}
				else
				{
					DEBUG_LOG(LOG::INFO, "Succesfully loaded Mesh: {}", meshFileName);
				}
			}
		}

		Mesh::~Mesh()
		{
			subMeshes.clear();
		}


		SubMeshInstance::SubMeshInstance(const uint32_t _id, const uint32_t _parentId)
		{
			id = _id;
			parentId = _parentId;
			vertexBuffer = nullptr;
			indexBuffer = nullptr;
			indexCount = 0;
		}

		SubMeshInstance::~SubMeshInstance()
		{
			vertexBuffer.reset();
			indexBuffer.reset();
		}

		MeshInstance::MeshInstance(LogicalDevice& device, Mesh& resource, const uint32_t _id)
		{
			id = _id;
			for (size_t i = 0; i < resource.subMeshes.size(); ++i)
			{
				std::shared_ptr<SubMeshInstance> subMesh = std::make_shared<SubMeshInstance>(static_cast<uint32_t>(i), id);

				// Staging vertex data from CPU to GPU memory
				const VkDeviceSize vertexBufferSize = sizeof(resource.subMeshes[i].vertices[0]) * resource.subMeshes[i].vertices.size();
				Buffer vertexStagingBuffer = Buffer::CreateStagingBuffer(device.GetAllocator(), vertexBufferSize, resource.subMeshes[i].vertices.data());
				subMesh->vertexBuffer = std::make_unique<Buffer>(device.GetAllocator(), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBufferSize, nullptr);
				device.CopyBuffer(vertexStagingBuffer, *subMesh->vertexBuffer.get(), vertexBufferSize);

				// Staging index data from CPU to GPU memory
				const VkDeviceSize indexBufferSize = sizeof(resource.subMeshes[i].indices[0]) * resource.subMeshes[i].indices.size();
				Buffer indexStagingBuffer = Buffer::CreateStagingBuffer(device.GetAllocator(), indexBufferSize, resource.subMeshes[i].indices.data());
				subMesh->indexBuffer = std::make_unique<Buffer>(device.GetAllocator(), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBufferSize, nullptr);
				device.CopyBuffer(indexStagingBuffer, *subMesh->indexBuffer.get(), indexBufferSize);

				subMesh->indexCount = static_cast<const uint32_t>(resource.subMeshes[i].indices.size());

				subMeshes.push_back(subMesh);					
			}
		}

		MeshInstance::~MeshInstance()
		{
			subMeshes.clear();
		}
	}
}