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
	float cPos : TEXCOORD3;
};

cbuffer Matrix : register(b0)
{
	float4x4 world;
	float4x4 view;
	float4x4 proj;
}

cbuffer SunCamera : register(b2)
{
	float4x4 sunView;
	float4x4 sunproj;
}

VS_OUT main(VS_IN vin)
{
	VS_OUT vout;
	vout.pos = float4(vin.pos, 1.0f);

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

	vout.normal = mul(vin.normal, (float3x3)world);

	float4 objectPos;
	objectPos.x = world[0][3];
	objectPos.y = world[1][3];
	objectPos.z = world[2][3];
	objectPos.w = world[3][3];

	float4 objectPosInCamera = mul(objectPos,view);
	vout.cPos = length(objectPosInCamera);

	return vout;
}