struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 wPos : TEXCOORD1;
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
Texture2D modelTex : register(t0);
Texture2D rampTex : register(t1);	// ランプテクスチャ

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = modelTex.Sample(samp,pin.uv);

	float3 N = normalize(pin.normal);
	float3 L = normalize(-lightDir.xyz);

	// 拡散光(diffuse) 物体の表面に直接あたる光、物体の色を表す
	float diffuse = dot(N, L);

	// 内積の値を基にランプテクスチャの色を引っ張り出す
	// ※内積の結果は-1～1なので0～1に変換する

	diffuse = diffuse * 0.5f + 0.5f;
	float shadow = rampTex.Sample(samp, float2(diffuse, 0.0f));

	float3 R = reflect(-L, N);

	// ピクセルからカメラに向かうベクトル(視線ベクトル)
	float3 V = normalize(camPos.xyz - pin.wPos.xyz);

	float specular = max(0, dot(R, V));
	specular = pow(specular, 30.0f);

	color.rgb *= shadow + specular;
	return color;
}