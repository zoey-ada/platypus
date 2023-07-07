#pragma once

#include <memory>
#include <string>

#include "../commonMesh.hpp"

class DirectXRenderer;
class MeshResource;

std::shared_ptr<MeshResource> createCommonMesh(const CommonMesh mesh_type,
	const std::shared_ptr<const DirectXRenderer>& renderer);

std::shared_ptr<MeshResource> createCommonMesh(const CommonMesh mesh_type,
	const std::shared_ptr<const DirectXRenderer>& renderer, const std::string& resource_id);

std::shared_ptr<MeshResource> createRectangle_uFlipped(
	const std::shared_ptr<const DirectXRenderer>& renderer,
	const std::string& resource_id = "rectangle_1x1_u_flipped");

std::shared_ptr<MeshResource> createRectangle(const std::shared_ptr<const DirectXRenderer>& renderer,
	const std::string& resource_id = "rectangle_1x1");
