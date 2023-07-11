#include "meshLoader.hpp"

#include <assimp/cimport.h>
#include <assimp/scene.h>

#include <renderer/graphics.hpp>
#include <renderer/iRenderer.hpp>
#include <utilities/logging/iLoggingSystem.hpp>

#include "../resourceCache.hpp"
#include "../resources/meshResource.hpp"

namespace platypus
{

MeshLoader::MeshLoader(std::shared_ptr<IResourceCache> cache, std::shared_ptr<IRenderer> renderer,
	std::shared_ptr<ILoggingSystem> logging)
	: _cache(std::move(cache)), _renderer(std::move(renderer)), _logging(std::move(logging))
{}

std::shared_ptr<Resource> MeshLoader::load(const char* resource_id, const char* store_id,
	std::byte* resource_data, const uint64_t data_size)
{
	if (_cache == nullptr || _renderer == nullptr || resource_data == nullptr ||
		strlen(resource_id) == 0)
	{
		this->_logging->warning("cache", "received invalid data for " + std::string(resource_id));
		return nullptr;
	}

	auto assimp_scene = std::shared_ptr<const aiScene>(
		aiImportFileFromMemory(reinterpret_cast<const char*>(resource_data),
			static_cast<uint32_t>(data_size), 0, resource_id));
	if (assimp_scene == nullptr)
	{
		this->_logging->warning("cache", "unable to load scene from " + std::string(resource_id));
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
	if (vertex_buffer == nullptr)
	{
		this->_logging->warning("cache",
			"unable to load vertex buffer from " + std::string(resource_id));

		return nullptr;
	}

	auto index_buffer = this->_renderer->createIndexBuffer(indices.data(), indices.size());
	if (index_buffer == nullptr)
	{
		this->_logging->warning("cache",
			"unable to load index buffer from " + std::string(resource_id));

		return nullptr;
	}

	MeshData mesh_data {};
	mesh_data.resource_id = resource_id;
	mesh_data.store_id = store_id;
	mesh_data.primative = graphics::PrimitiveType::TriangleList;
	mesh_data.index_buffer = index_buffer;
	mesh_data.index_count = indices.size();
	mesh_data.vertices = std::move(vertices);
	mesh_data.vertex_buffer = vertex_buffer;

	return std::make_shared<MeshResource>(&mesh_data);
}

uint8_t* MeshLoader::allocate(unsigned int size)
{
	return this->_cache->allocate(size);
}

};
