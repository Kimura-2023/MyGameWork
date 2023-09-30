// ---デバッグ線用シェーダー---
// (渡された座標をWVP変換した座標と色をそのまま渡す)
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

	// ワールド行列をかける
	vout.pos = mul(vout.pos, World);

	// ビュー行列をかける
	vout.pos = mul(vout.pos, View);

	// プロジェクション行列をかける
	vout.pos = mul(vout.pos, Proj);

	// 色を格納
	vout.color = vin.color;

	return vout;
}