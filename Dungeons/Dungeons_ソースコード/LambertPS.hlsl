struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
};

cbuffer Light : register(b0)
{
	float4 lightDir;
}

Texture2D<float4> tex0 : register(t0);

sampler g_sampler : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 Texcolor = tex0.Sample(g_sampler,pin.uv);

	float3 L = normalize(-lightDir.xyz);
	float3 N = normalize(pin.normal);

	float diffuse = dot(L, N);
	float ambient = 0.6f;

	Texcolor.xyz *= (diffuse * 0.4f + ambient);
	return Texcolor;


}