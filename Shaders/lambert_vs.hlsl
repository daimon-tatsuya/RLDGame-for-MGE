
#include "Lambert.hlsli"

VS_OUT main(
	float4 position     : POSITION,
	float3 normal       : NORMAL,
	float2 texcoord     : TEXCOORD,
	float4 boneWeights : WEIGHTS,
	uint4  boneIndices : BONES
	)
{
	float3 p = { 0, 0, 0 };
	float3 n = { 0, 0, 0 };
	for (int i = 0; i < 4; i++)
	{
		p += (boneWeights[i] * mul(position, bone_transforms[boneIndices[i]])).xyz;
		n += (boneWeights[i] * mul(float4(normal.xyz, 0), bone_transforms[boneIndices[i]])).xyz;
	}

	VS_OUT vout;
	vout.position = mul(float4(p, 1.0f), view_projection);

	float3 N = normalize(n);
	float3 L = normalize(light_direction.xyz);
	float d = dot(L, N);
	vout.color.xyz = material_color.xyz *max(0, d);
	vout.color.w = material_color.w;
	vout.texcoord = texcoord;

	return vout;
}
