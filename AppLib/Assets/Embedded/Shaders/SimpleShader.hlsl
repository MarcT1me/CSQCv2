struct VertexInput
{
    float3 position : POSITION;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

VertexOutput VS(VertexInput input)
{
    VertexOutput output;

    output.position = float4(
        input.position,
        1.0f
    );

    output.uv = float2(
        (input.position.x + 1.0f) * 0.5f,
        1.0f - (input.position.y + 1.0f) * 0.5f
    );

    return output;
}

float4 PS(VertexOutput input) : SV_TARGET
{
    return float4(input.uv.x, 0.0f, input.uv.y, input.uv.x * input.uv.y);
}
