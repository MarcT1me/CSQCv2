struct VertexInput
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

cbuffer TransformCB : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 projection;
}

VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    float4x4 wvp = mul(mul(world, view), projection);
    output.position = mul(float4(input.position, 1.0), wvp);
    output.uv = input.uv;
    return output;
}

Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 PS(VertexOutput input) : SV_TARGET
{
    return tex.Sample(samp, input.uv);
}
