// ����
struct VS_IN
{
	float3 pos : POSITION0;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 weight : WEIGHT0;
	uint4 index : INDEX0;
};

// �o��
struct VS_OUT
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 wPos : TEXCOORD1;
};

// WVP�ϊ��s��
cbuffer WVP : register(b0)
{
	float4x4 world;
	float4x4 view;
	float4x4 proj;
};

// �J�������W
cbuffer Camera:register(b1)
{
	float4 camPos;
}

// ���̒萔�o�b�t�@
cbuffer BoneMatrix : register(b3)
{
	float4x4 bone[64];
};

VS_OUT main(VS_IN vin)
{
	VS_OUT vout;
	vout.pos = float4(vin.pos, 1.0f);

	// ---���[���h���W���擾---
	float4 objectPos;
	objectPos.x = world[0][3];
	objectPos.y = world[1][3];
	objectPos.z = world[2][3];
	objectPos.w = world[3][3];

	float4 objectPosInCamera = mul(objectPos, view);
	float dist = length(objectPosInCamera);						// �J�����Ƃ̋��������߂�

	vout.pos.xyz += normalize(vin.normal) * 0.002f * dist;		// �J��������̋����ɂ���Ė@�������Ɋg��

	// ---�A�j���[�V�����v�Z
	float4x4 anime = bone[vin.index.x] * vin.weight.x +
		bone[vin.index.y] * vin.weight.y +
		bone[vin.index.z] * vin.weight.z +
		bone[vin.index.w] * vin.weight.w;

	vout.pos = mul(vout.pos, anime);

	// ---�e�ϊ��s�񔽉f---
	vout.pos = mul(vout.pos, world);
	vout.wPos = vout.pos;
	vout.pos = mul(vout.pos, view);
	vout.pos = mul(vout.pos, proj);

	vout.uv = vin.uv;
	vout.normal = mul(vin.normal, (float3x3)world);

	return vout;
}