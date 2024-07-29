// MIT License, Copyright (c) 2024 Malik Allen

#include "Mesh.h"

#include "../src/utility/DebugLog.h"
#include "../src/core/vulkan/devices/LogicalDevice.h"
#include "../src/core/vulkan/resource/Buffer.h"

#include <string>

#include <tiny_obj_loader.h>

namespace Baal
{
	namespace VK
	{
		Mesh::Mesh(LogicalDevice& device, const char* parentDirectory, const char* meshFileName)
		{
			std::string meshFilePath = std::string(parentDirectory) + std::string(meshFileName);

			tinyobj::attrib_t attrib;
			std::vector<tinyobj::shape_t> shapes;
			std::vector<tinyobj::material_t> materials;

			std::string warn;
			std::string err;

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

						v.norm.x = attrib.normals[3 * index.normal_index + 0];
						v.norm.y = attrib.normals[3 * index.normal_index + 1];
						v.norm.z = attrib.normals[3 * index.normal_index + 2];

						v.texCoords.x = attrib.texcoords[2 * index.texcoord_index + 0];
						v.texCoords.y = attrib.texcoords[2 * index.texcoord_index + 1];

						v.color.x = attrib.colors[3 * index.vertex_index + 0];
						v.color.y = attrib.colors[3 * index.vertex_index + 1];
						v.color.z = attrib.colors[3 * index.vertex_index + 2];

						vertices.push_back(v);

						indices.push_back(indices.size());
					}

					subMeshes.push_back(std::make_unique<SubMesh>(
						vertices, 
						indices, 
						std::make_unique<Buffer>(device, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, sizeof(vertices[0]) *vertices.size(), vertices.data()),
						std::make_unique<Buffer>(device, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, sizeof(indices[0]) * indices.size(), indices.data())
					));
				}

				if (!warn.empty())
				{
					DEBUG_LOG(LOG::WARNING, "Loaded Mesh: {}, Warning: {}", meshFileName, warn);
				}
				else
				{
					DEBUG_LOG(LOG::INFO, "Loaded Mesh: {}", meshFileName);
				}
			}
		}

		Mesh::~Mesh()
		{
			subMeshes.clear();
		}
	}
}