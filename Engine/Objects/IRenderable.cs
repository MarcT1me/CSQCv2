namespace Engine.Objects;

public interface IRenderable
{
    public void PreRender();
    public void Render();
    public void PostRender();
}