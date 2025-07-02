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

VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    output.position = float4(input.position, 1.0f);
    output.uv = input.uv;
    return output;
}

Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 PS(VertexOutput input) : SV_TARGET
{
    return tex.Sample(samp, input.uv);
}
