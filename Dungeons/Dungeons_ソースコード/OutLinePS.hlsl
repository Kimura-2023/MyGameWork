struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 wPos : TEXCOORD1;
	float cPos : TEXCOORD3;
};

cbuffer Line : register(b2)
{
	float4 work;
};

SamplerState samp : register(s0);
Texture2D modelTex : register(t0);

// ---ディザパターン---
static const int pattern[4][4] = {
	{ 0,32, 8,40},
	{48,16,56,24},
	{12,44, 4,36},
	{60,28,52,20},
};

float4 main(PS_IN pin) : SV_TARGET
{
	//float4 color = modelTex.Sample(samp,pin.uv);
	/*color.r += 0.2f;
	color.rgb *= 0.3f;*/

	float4 color = work;
	return color;
}