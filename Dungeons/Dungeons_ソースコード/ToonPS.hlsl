struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 wPos : TEXCOORD1;
	float4 sunPos : TEXCOORD2;
};

cbuffer Light : register(b0)
{
	float4 lightDir;
};

SamplerState samp : register(s0);
Texture2D modelTex : register(t0);
Texture2D rampTex : register(t1);		// �����v�e�N�X�`��
Texture2D sunViewTex : register(t2);	// �����e

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = modelTex.Sample(samp,pin.uv);

	float3 N = normalize(pin.normal);
	float3 L = normalize(-lightDir.xyz);

	float diffuse = dot(N, L);

	// ���ς̒l����Ƀ����v�e�N�X�`���̐F����������o��
	// �����ς̌��ʂ�-1�`1�Ȃ̂�0�`1�ɕϊ�����
	diffuse = diffuse * 0.5f + 0.5f;
	float rampshadow = rampTex.Sample(samp, float2(diffuse,0.0f));

	float2 sunUV = pin.sunPos.xy / pin.sunPos.w;

	// ��ʂ̍��W-1�`1���e�N�X�`���̍��W0�`1�ɕϊ�
	sunUV = sunUV * 0.5f + 0.5f;

	// ��ʂ�Y���W�͉������Ɍ������đ����邪�A�e�N�X�`����Y���W�͏ォ�牺�Ɍ������đ�����̂Ŕ��]
	sunUV.y = 1.0f - sunUV.y;

	// ���s�����������񂾃e�N�X�`������A�������擾
	float sunDepth = sunViewTex.Sample(samp, sunUV).r;
	sunDepth += 0.00025f;		// �����͓K�X����
	// ���C���J�����ŕ\�����悤�Ƃ��Ă���s�N�Z���̑��z���猩���������擾
	float pixelDepth = pin.sunPos.z / pin.sunPos.w;

	float shadow = 1.0f;

	// ���݂̃s�N�Z���̉��s�����e�N�X�`���ɏ������܂�Ă��鉜�s�����������ꍇ�A�e�Ɣ��肷��B(�f�v�X�V���h�E)
	if (sunDepth < pixelDepth)
	{
		shadow = 0.4f;	// �e�̋���
	}

	color.rgb = color.rgb * rampshadow * shadow + 0.05f;

	return color;
}