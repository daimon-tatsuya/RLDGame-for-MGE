#pragma once

#include "Engine/Systems/Math.h"

//ToDo ����,h���s�����Ƃ�����


// �����_�[�R���e�L�X�g
struct RenderContext
{
	DirectX::XMFLOAT4X4		view;
	DirectX::XMFLOAT4X4		projection;
	DirectX::XMFLOAT4			lightDirection;
};
