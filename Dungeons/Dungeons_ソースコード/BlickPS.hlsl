struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 wPos : TEXCOORD1;
	float3 tangentL : TEXCOORD2;
};

cbuffer Light : register(b0)
{
	float4 lightDir;
};

cbuffer Camera : register(b1)
{
	float4 camPos;
}

SamplerState samp : register(s0);
Texture2D tex : register(t0);
Texture2D normalMap : register(t1);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = tex.Sample(samp,pin.uv);

	// 法線マップから代わりの法線を取り出す
	float3 N = normalMap.Sample(samp, pin.uv);
	N = N * 2.0f - 1.0f;		// 0～1の色データを、-1～1の値に補正
	N = normalize(N);

	float3 L = normalize(-pin.tangentL);

	float diffuse = dot(N, L);
	float ambient = 0.5f;

	float3 R = reflect(-L, N);

	float3 V = normalize(camPos.xyz - pin.wPos.xyz);
	float specular = max(0, dot(R, V));
	specular = pow(specular, 10.0f);

	color.rgb *= diffuse * 0.6f + ambient + specular;

	return color;
}