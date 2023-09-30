// ---インスタンシング描画用シェーダー---
// 
//------------------------------
#define MAX_WORLD_MATRIX	100

struct VS_IN
{
	float3 pos : POSITION0;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	uint id : SV_InstanceID;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
};

cbuffer Matrix : register(b0)
{
	float4x4 world;
	float4x4 view;
	float4x4 proj;
}

cbuffer Global_Instance : register(b2)
{
	float4x4 World[MAX_WORLD_MATRIX];
}

VS_OUT main(VS_IN vin)
{
	VS_OUT vout;

	vout.pos = float4(vin.pos, 1.0f);
	uint n = vin.id % MAX_WORLD_MATRIX;

	float4x4 mWorld = World[n];

	vout.pos = mul(vout.pos, mWorld);
	vout.pos = mul(vout.pos, view);
	vout.pos = mul(vout.pos, proj);

	vout.uv = vin.uv;

	vout.normal = mul(vin.normal, (float3x3)world);

	return vout;
}