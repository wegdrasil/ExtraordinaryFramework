cbuffer cbPerObject 
{
	float4x4 WorldViewProj;
};

void VS(float3 iPos : POSITION, float3 iNormal : NORMAL, float2 iTex : TEXCOORD, out float4 oPosH : SV_POSITION)
{
	oPosH = mul(float4(iPos, 1.0f), WorldViewProj);
}