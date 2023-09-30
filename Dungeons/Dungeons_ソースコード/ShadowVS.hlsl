struct VS_IN
{
	float3 pos : POSITION0;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
};
struct VS_OUT
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 wPos : TEXCOORD1;
	float4 sunPos : TEXCOORD2;				// 太陽から見た位置
};

// ↓ゲームのメインカメラに表示するための行列
cbuffer WVP : register(b0)
{
	float4x4 world;
	float4x4 view;
	float4x4 proj;
};

// 太陽の位置にあるカメラに表示するための行列
cbuffer SunCamera:register(b2)
{
	float4x4 sunView;
	float4x4 sunproj;
}


VS_OUT main(VS_IN vin)
{
	VS_OUT vout;
	vout.pos = float4(vin.pos, 1.0f);

	// ローカル座標 > ワールド座標 > ビュー座標 > プロジェクション座標 と変換をかける
	vout.pos = mul(vout.pos, world);			// ピクセルシェーダー側でワールド座標でカメラの座標を計算したいのでWVP変換する前に値を取得したい
	vout.wPos = vout.pos;						// ピクセルシェーダーにワールド座標を渡すため、値をコピーする。
	vout.pos = mul(vout.pos, view);
	vout.pos = mul(vout.pos, proj);

	// メインカメラとは別に、太陽用のカメラで頂点を計算
	float4 sunPos = vout.wPos;
	sunPos = mul(sunPos, sunView);
	sunPos = mul(sunPos, sunproj);

	vout.sunPos = sunPos;

	vout.normal = vin.normal;
	vout.uv = vin.uv;
	return vout;
}