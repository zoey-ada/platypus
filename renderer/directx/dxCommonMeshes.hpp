#pragma once

#include <memory>

#include "../commonMesh.hpp"

class DirectXRenderer;
class MeshResource;

std::shared_ptr<MeshResource> createCommonMesh(const CommonMesh mesh_type,
	const std::shared_ptr<DirectXRenderer>& renderer);

std::shared_ptr<MeshResource> createRectangle_uFlipped(
	const std::shared_ptr<DirectXRenderer>& renderer);

std::shared_ptr<MeshResource> createRectangle(const std::shared_ptr<DirectXRenderer>& renderer);
