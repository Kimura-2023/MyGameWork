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

// 骨の定数バッファ
cbuffer BoneMatrix : register(b3)
{
	float4x4 bone[64];
};

VS_OUT main(VS_IN vin)
{
	VS_OUT vout;
	vout.pos = float4(vin.pos, 1.0f);
	// ---アニメーション計算---
	float4x4 anime = bone[vin.index.x] * vin.weight.x +
		bone[vin.index.y] * vin.weight.y +
		bone[vin.index.z] * vin.weight.z +
		bone[vin.index.w] * vin.weight.w;
	vout.pos = mul(vout.pos, anime);

	// ローカル座標 > ワールド座標 > ビュー座標 > プロジェクション座標 と変換をかける
	vout.pos = mul(vout.pos, world);			// ピクセルシェーダー側でワールド座標でカメラの座標を計算したいのでWVP変換する前に値を取得したい
	vout.pos = mul(vout.pos, view);
	vout.pos = mul(vout.pos, proj);

	// SV_POSITIONとは別にデータを渡す
	vout.screenPos = vout.pos;

	return vout;
}