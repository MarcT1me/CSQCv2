namespace Engine.Objects.Light;

using Actor;

public class Light<T>(T nodeData)
    : Actor<T>(nodeData), IRenderable
    where T : LightData
{
    private LightUniforms _uniforms;
    public bool NeedsUpdate = true;

    public void PreRender()
    {
        if (NeedsUpdate)
        {
            // 1. Обновляем данные света
            UpdateLightData();

            // 2. Загружаем данные в буфер
            // LightBufferManager.UploadLightData(this, ref _uniforms);

            NeedsUpdate = false;
        }
    }

    /// <summary>
    /// Преобразует данные источника света в читаемый для shader формат
    /// </summary>
    private void UpdateLightData()
    {
        _uniforms = new LightUniforms
        {
            Type = MetaData.LightType,

            Position = MetaData.Transform.Position,
            Rotation = MetaData.Transform.Rotation,
            Size = MetaData.Transform.Size,

            Color = MetaData.Color.ToArgb(),
            Intensity = MetaData.Intensity,
            CastShadows = MetaData.CastShadows
        };

        if (MetaData is PointLightData point)
        {
            _uniforms.Attenuation = point.Attenuation;
            _uniforms.ShadowDistance = point.ShadowDistance;
            _uniforms.MaxDistance = point.MaxDistance;
        }

        if (MetaData is not SpotLightData spot) return;
        _uniforms.InnerAngle = spot.InnerAngle;
        _uniforms.OuterAngle = spot.OuterAngle;
        _uniforms.FalloffExponent = spot.FalloffExponent;
    }

    public void Render()
    {
        // не требуется
    }

    public void PostRender()
    {
        // не требуется
    }
}