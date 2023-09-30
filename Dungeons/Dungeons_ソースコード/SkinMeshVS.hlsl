// CPUから貰ってくる値(モデルの座標)
struct VS_IN
{
	float3 pos : POSITION0;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 weight : WEIGHT0;
	uint4 index : INDEX0;
};

// ピクセルシェーダーに渡す値
struct VS_OUT
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 wPos : TEXCOORD1;
	float4 sunPos : TEXCOORD2;				// 太陽から見た位置
};

// 定数バッファ
cbuffer Matrix : register(b0)
{
	float4x4 world;
	float4x4 view;
	float4x4 proj;
};

// 太陽から見た視点
cbuffer SunCamera : register(b2)
{
	float4x4 sunView;
	float4x4 sunproj;
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
	vout.normal = vin.normal;
	 //動かしたい座標はいろんな骨の影響を受けている
	 //骨ごとの行列に割合を求めて、最終的に全部足す
	float4x4 anime = bone[vin.index.x] * vin.weight.x +
		bone[vin.index.y] * vin.weight.y +
		bone[vin.index.z] * vin.weight.z +
		bone[vin.index.w] * vin.weight.w;
	vout.pos = mul(vout.pos, anime);
	vout.normal = mul(vout.normal, (float3x3)anime);

	vout.pos = mul(vout.pos, world);
	vout.wPos = vout.pos;
	vout.pos = mul(vout.pos, view);
	vout.pos = mul(vout.pos, proj);

	// メインカメラとは別に、太陽用のカメラで頂点を計算
	float4 sunPos = vout.wPos;
	sunPos = mul(sunPos, sunView);
	sunPos = mul(sunPos, sunproj);

	vout.sunPos = sunPos;

	vout.uv = vin.uv;
	vout.normal = mul(vout.normal, (float3x3)world);

	return vout;
}