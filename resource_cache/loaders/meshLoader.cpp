#include "meshLoader.hpp"

#include <assimp/cimport.h>
#include <assimp/scene.h>

#include <renderer/graphics.hpp>
#include <renderer/iRenderer.hpp>

#include "../resourceCache.hpp"
#include "../resources/meshResource.hpp"

MeshLoader::MeshLoader(std::shared_ptr<ResourceCache> cache,
	const std::shared_ptr<IRenderer>& renderer)
	: _cache(std::move(cache)), _renderer(std::move(renderer))
{}

std::shared_ptr<Resource> MeshLoader::load(const char* resource_id, const char* store_id,
	std::byte* resource_data, const uint64_t data_size)
{
	if (strlen(resource_id) == 0 || _cache == nullptr || _renderer == nullptr ||
		resource_data == nullptr)
	{
		return nullptr;
	}

	auto assimp_scene = std::shared_ptr<const aiScene>(
		aiImportFileFromMemory(reinterpret_cast<const char*>(resource_data),
			static_cast<uint32_t>(data_size), 0, resource_id));
	if (assimp_scene == nullptr)
	{
		// log
		return nullptr;
	}

	// TODO: handle loading multiple meshes from the same file
	auto mesh = assimp_scene->mMeshes[assimp_scene->mRootNode->mChildren[0]->mMeshes[0]];

	std::vector<graphics::Vertex> vertices(mesh->mNumVertices);
	for (auto i = 0u; i < mesh->mNumVertices; ++i)
	{
		graphics::Vertex v;
		v.coord.x = mesh->mVertices[i].x;
		v.coord.y = mesh->mVertices[i].y;
		v.coord.z = mesh->mVertices[i].z;

		if (mesh->HasNormals())
		{
			v.normal.x = mesh->mNormals[i].x;
			v.normal.y = mesh->mNormals[i].y;
			v.normal.z = mesh->mNormals[i].z;
		}

		if (mesh->HasTextureCoords(0))
		{
			v.textCoord.x = mesh->mTextureCoords[0][i].x;
			v.textCoord.y = mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(v);
	}

	std::vector<uint32_t> indices;
	for (auto i = 0u; i < mesh->mNumFaces; ++i)
	{
		auto face = mesh->mFaces[i];

		for (auto j = 0u; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// handle materials

	auto vertex_buffer = this->_renderer->createVertexBuffer(vertices.data(), vertices.size());
	auto index_buffer = this->_renderer->createIndexBuffer(indices.data(), indices.size());

	PtMeshResourceData mesh_data {};
	mesh_data.resource_id = resource_id;
	mesh_data.store_id = store_id;
	mesh_data.primative = PtPrimitiveType::TriangleList;
	mesh_data.vertex_buffer = vertex_buffer;
	mesh_data.index_buffer = index_buffer;
	mesh_data.index_count = indices.size();

	return std::make_shared<MeshResource>(&mesh_data);
}

uint8_t* MeshLoader::allocate(unsigned int size)
{
	return this->_cache->allocate(size);
}
