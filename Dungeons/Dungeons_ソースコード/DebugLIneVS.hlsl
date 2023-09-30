// ---�f�o�b�O���p�V�F�[�_�[---
// (�n���ꂽ���W��WVP�ϊ��������W�ƐF�����̂܂ܓn��)
//------------------------------
struct VS_IN
{
	float3 pos : POSITION0;
	float4 color : COLOR0;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
};

cbuffer Matrix : register(b0)
{
	float4x4 World;
	float4x4 View;
	float4x4 Proj;
}

VS_OUT main(VS_IN vin)
{
	VS_OUT vout;
	
	vout.pos = float4(vin.pos, 1.0f);

	// ���[���h�s���������
	vout.pos = mul(vout.pos, World);

	// �r���[�s���������
	vout.pos = mul(vout.pos, View);

	// �v���W�F�N�V�����s���������
	vout.pos = mul(vout.pos, Proj);

	// �F���i�[
	vout.color = vin.color;

	return vout;
}