// Простая структура вершины
struct VertexInput {
    float3 position : POSITION;
    float4 color : COLOR;
};

// Выход вершинного шейдера
struct VertexOutput {
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

// Вершинный шейдер
VertexOutput VS(VertexInput input) {
    VertexOutput output;
    output.position = float4(input.position, 1.0f);
    output.color = input.color;
    return output;
}

// Пиксельный шейдер
float4 PS(VertexOutput input) : SV_TARGET {
    return input.color;
}