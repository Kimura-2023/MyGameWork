struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
};

SamplerState samp : register(s0);
Texture2D modelTex : register(t0);

float4 main(PS_IN pin) : SV_TARGET
{
	// �e�N�X�`���̐F�����̂܂ܕ\��
	float4 color = modelTex.Sample(samp,pin.uv);

	return color;
}