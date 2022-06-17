#pragma once

#include <memory>

#include <d3d11.h>

#include "../iRendererState.hpp"

class DirectXRenderer;

class DirectXAlphaPass: public IRendererState
{
public:
	DirectXAlphaPass(const std::shared_ptr<DirectXRenderer>& renderer);
	virtual ~DirectXAlphaPass();

	[[nodiscard]] inline const char* description() const override { return "DirectX alpha pass"; }

private:
	ID3D11BlendState* _current_blend_state {nullptr};
	ID3D11BlendState* _previous_blend_state {nullptr};
	float _previous_blend_factor[4];
	unsigned int _previous_sample_mask;

	std::shared_ptr<DirectXRenderer> _renderer;
};
