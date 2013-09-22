cbuffer cbPerObject
{
	float4x4 WorldViewProj;
};

void main(in float3 iPos : POSITION,
		  in float3 iNormal : NORMAL,
		  in float2 iTex : TEXCOORD,
		  out float4 oPosH : SV_POSITION,
		  out float2 oTex : TEXCOORD
		  )
{
	oTex = iTex;
	oPosH = mul(float4(iPos, 1.0f), WorldViewProj);
	
}