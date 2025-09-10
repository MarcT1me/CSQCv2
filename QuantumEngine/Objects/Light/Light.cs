using QuantumCore.Graphic.Window;

namespace QuantumCore.Objects.Light;

using Actor;

public class Light<TData>(TData nodeData)
    : Actor<TData>(nodeData), IWindowRenderable
    where TData : LightData
{
    private LightUniforms _uniforms;

    public void PreRender(WindowData winMeta)
    {
        if (!MetaData.NeedsUpdate) return;
        
        // 1. Обновляем данные света
        UpdateLightData();

        // 2. Загружаем данные в буфер
        // LightBufferManager.UploadLightData(this, ref _uniforms);

        MetaData.NeedsUpdate = false;
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
            Rotation = MetaData.Transform.EulerAngles,
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

    public void Render(WindowData winMeta)
    {
    }

    public void PostRender(WindowData winMeta)
    {
    }
}