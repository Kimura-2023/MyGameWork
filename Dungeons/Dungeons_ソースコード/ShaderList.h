#pragma once
#include "Shader.h"

enum VS_KIND		// ���_�V�F�[�_�[���X�g
{
	VS_LINE,
	VS_ASSIMP,
	VS_SKINMESH,
	VS_INSTANCING,
	VS_OUTLINE,
	VS_OBJECT,
	VS_DEPTHWRITE,
	VS_DEPTHSHADOW,
	VS_MAX,
};

enum PS_KIND		// �s�N�Z���V�F�[�_�\���X�g
{
	PS_LINE,
	PS_ASSIMP,
	PS_LAMBERT,
	PS_OUTLINE,
	PS_TOON,
	PS_TEXTURE,
	PS_PHONG,
	PS_DEPTHWRITE,
	PS_DEPTHSHADOW,
	PS_MASKOBJECT,
	PS_BEHIND,
	PS_MAX,
};

void InitShaderList();
void UninitShaderList();
VertexShader* GetVS(VS_KIND);
PixelShader* GetPS(PS_KIND);