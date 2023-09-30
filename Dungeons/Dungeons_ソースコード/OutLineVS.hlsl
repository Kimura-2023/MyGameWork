// 入力
struct VS_IN
{
	float3 pos : POSITION0;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 weight : WEIGHT0;
	uint4 index : INDEX0;
};

// 出力
struct VS_OUT
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 wPos : TEXCOORD1;
};

// WVP変換行列
cbuffer WVP : register(b0)
{
	float4x4 world;
	float4x4 view;
	float4x4 proj;
};

// カメラ座標
cbuffer Camera:register(b1)
{
	float4 camPos;
}

// 骨の定数バッファ
cbuffer BoneMatrix : register(b3)
{
	float4x4 bone[64];
};

VS_OUT main(VS_IN vin)
{
	VS_OUT vout;
	vout.pos = float4(vin.pos, 1.0f);

	// ---ワールド座標を取得---
	float4 objectPos;
	objectPos.x = world[0][3];
	objectPos.y = world[1][3];
	objectPos.z = world[2][3];
	objectPos.w = world[3][3];

	float4 objectPosInCamera = mul(objectPos, view);
	float dist = length(objectPosInCamera);						// カメラとの距離を求める

	vout.pos.xyz += normalize(vin.normal) * 0.002f * dist;		// カメラからの距離によって法線方向に拡大

	// ---アニメーション計算
	float4x4 anime = bone[vin.index.x] * vin.weight.x +
		bone[vin.index.y] * vin.weight.y +
		bone[vin.index.z] * vin.weight.z +
		bone[vin.index.w] * vin.weight.w;

	vout.pos = mul(vout.pos, anime);

	// ---各変換行列反映---
	vout.pos = mul(vout.pos, world);
	vout.wPos = vout.pos;
	vout.pos = mul(vout.pos, view);
	vout.pos = mul(vout.pos, proj);

	vout.uv = vin.uv;
	vout.normal = mul(vin.normal, (float3x3)world);

	return vout;
}