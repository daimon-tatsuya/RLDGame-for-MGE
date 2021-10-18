
#include "Lambert.hlsli"

Texture2D diffuseMap : register(t0);
SamplerState diffuseMapSamplerState : register(s0);

float4 main(VS_OUT pin) : SV_TARGET
{
    //Šm”F—p
    //return pin.color;

	return diffuseMap.Sample(diffuseMapSamplerState, pin.texcoord) * pin.color;
}

