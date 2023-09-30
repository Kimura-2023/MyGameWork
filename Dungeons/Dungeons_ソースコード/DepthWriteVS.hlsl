struct VS_IN
{
	float3 pos : POSITION0;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 weight : WEIGHT0;
	uint4 index : INDEX0;
};
struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 screenPos : TEXCOORD0;
};
cbuffer WVP : register(b0)
{
	float4x4 world;
	float4x4 view;
	float4x4 proj;
};

// ���̒萔�o�b�t�@
cbuffer BoneMatrix : register(b3)
{
	float4x4 bone[64];
};

VS_OUT main(VS_IN vin)
{
	VS_OUT vout;
	vout.pos = float4(vin.pos, 1.0f);
	// ---�A�j���[�V�����v�Z---
	float4x4 anime = bone[vin.index.x] * vin.weight.x +
		bone[vin.index.y] * vin.weight.y +
		bone[vin.index.z] * vin.weight.z +
		bone[vin.index.w] * vin.weight.w;
	vout.pos = mul(vout.pos, anime);

	// ���[�J�����W > ���[���h���W > �r���[���W > �v���W�F�N�V�������W �ƕϊ���������
	vout.pos = mul(vout.pos, world);			// �s�N�Z���V�F�[�_�[���Ń��[���h���W�ŃJ�����̍��W���v�Z�������̂�WVP�ϊ�����O�ɒl���擾������
	vout.pos = mul(vout.pos, view);
	vout.pos = mul(vout.pos, proj);

	// SV_POSITION�Ƃ͕ʂɃf�[�^��n��
	vout.screenPos = vout.pos;

	return vout;
}