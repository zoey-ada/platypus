#include "directXMeshLoader.hpp"

#include <assimp/cimport.h>
#include <assimp/scene.h>

#include <d3d11.h>

#include <exceptions/creationException.hpp>
#include <renderer/directx/directXRenderer.hpp>
#include <renderer/iRenderer.hpp>
#include <utilities/safeDelete.hpp>

#include "../resourceCache.hpp"
#include "../resources/meshResource.hpp"
#include "../resources/resource.hpp"
#include "../stores/iResourceStore.hpp"

DirectXMeshLoader::DirectXMeshLoader(std::shared_ptr<ResourceCache> cache,
	const std::shared_ptr<IRenderer>& renderer)
	: _cache(std::move(cache))
{
	this->_renderer = std::dynamic_pointer_cast<DirectXRenderer>(renderer);
}

std::shared_ptr<Resource> DirectXMeshLoader::load(const std::shared_ptr<IResourceStore>& store,
	const std::string& filename)
{
	if (_cache == nullptr || store == nullptr || filename.empty() || _renderer == nullptr)
		return nullptr;

	auto size = store->getResourceSize(filename);
	auto* buffer = new (std::nothrow) uint8_t[size];

	if (buffer == nullptr)
	{
		// log res cache full...
		return nullptr;
	}

	if (!store->getResource(filename, buffer))
	{
		// log error
		return nullptr;
	}

	auto assimp_scene = std::shared_ptr<const aiScene>(aiImportFileFromMemory(
		reinterpret_cast<const char*>(buffer), static_cast<uint32_t>(size), 0, filename.c_str()));
	if (assimp_scene == nullptr)
	{
		// log
		return nullptr;
	}

	// TODO: handle loading multiple meshes from the same file
	auto mesh = assimp_scene->mMeshes[assimp_scene->mRootNode->mChildren[0]->mMeshes[0]];

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	for (auto i = 0u; i < mesh->mNumVertices; ++i)
	{
		Vertex v;
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

	for (auto i = 0u; i < mesh->mNumFaces; ++i)
	{
		auto face = mesh->mFaces[i];

		for (auto j = 0u; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// handle materials

	D3D11_BUFFER_DESC vertex_buffer_desc {};
	vertex_buffer_desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	vertex_buffer_desc.ByteWidth = static_cast<UINT>(sizeof(Vertex) * vertices.size());
	vertex_buffer_desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertex_data {};
	auto drawable_verts = drawable(vertices);
	vertex_data.pSysMem = &drawable_verts[0];

	auto vertex_buffer =
		toSharedPtr(this->_renderer->create()->newBuffer(vertex_buffer_desc, vertex_data));

	D3D11_BUFFER_DESC index_buffer_desc {};
	index_buffer_desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	index_buffer_desc.ByteWidth = static_cast<UINT>(sizeof(uint32_t) * indices.size());
	index_buffer_desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA index_data {};
	index_data.pSysMem = &indices[0];

	auto index_buffer =
		toSharedPtr(this->_renderer->create()->newBuffer(index_buffer_desc, index_data));

	return std::make_shared<MeshResource>(filename, buffer, size, store, this->_cache,
		vertex_buffer, vertices, vertices.size(), index_buffer, indices, indices.size());
}

uint8_t* DirectXMeshLoader::allocate(unsigned int size)
{
	return this->_cache->allocate(size);
}
