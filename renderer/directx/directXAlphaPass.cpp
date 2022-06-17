#include "directXAlphaPass.hpp"

#include "directXRenderer.hpp"

DirectXAlphaPass::DirectXAlphaPass(const std::shared_ptr<DirectXRenderer>& renderer)
{
	this->_renderer = renderer;

	this->_renderer->context()->OMGetBlendState(&this->_previous_blend_state,
		this->_previous_blend_factor, &this->_previous_sample_mask);

	D3D11_BLEND_DESC blend_state {};
	blend_state.AlphaToCoverageEnable = FALSE;
	blend_state.IndependentBlendEnable = FALSE;
	blend_state.RenderTarget[0].BlendEnable = true;
	blend_state.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	blend_state.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
	blend_state.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	blend_state.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
	blend_state.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
	blend_state.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	blend_state.RenderTarget[0].RenderTargetWriteMask =
		D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

	renderer->device()->CreateBlendState(&blend_state, &this->_current_blend_state);
	renderer->context()->OMSetBlendState(this->_current_blend_state, 0, 0xFFFFFFFF);
}

DirectXAlphaPass::~DirectXAlphaPass()
{
	this->_renderer->context()->OMSetBlendState(this->_previous_blend_state,
		this->_previous_blend_factor, this->_previous_sample_mask);

	if (this->_current_blend_state != nullptr)
		this->_current_blend_state->Release();
	this->_current_blend_state = nullptr;

	if (this->_previous_blend_state != nullptr)
		this->_previous_blend_state->Release();
	this->_previous_blend_state = nullptr;
}
