using Engine.Data;
using Engine.Objects.Camera;
using Engine.Events.QuantumEvents;
using Engine.Events.QuantumEvents.Mouse;
using Engine.Input.Keyboard;

namespace AppLib.Game;

public class GameCamera(CameraData cameraData) : Camera(cameraData)
{
    public float CameraSpeed = 0.5f;
    public float CameraSensitivity = 0.5f;

    public override void HandleEvent(QuantumEvent e)
    {
        base.HandleEvent(e);

        if (e is not MouseMoveEvent mouseMove) return;

        var speed = CameraSensitivity * (float)TestApp.Instance.Clock.MetaData.DeltaTime;

        MetaData.Yaw += mouseMove.Pos.X * speed;
        MetaData.Pitch += mouseMove.Pos.Y * speed;
        NeedsUpdate = true;
    }

    public override void Update()
    {
        base.Update();

        var speed = CameraSpeed * (float)TestApp.Instance.Clock.MetaData.DeltaTime;
        var updated = false;

        if (Keyboard.GetKey(Key.W))
        {
            MetaData.Transform.Position += Forward * speed;
            updated = true;
        }

        if (Keyboard.GetKey(Key.S))
        {
            MetaData.Transform.Position -= Forward * speed;
            updated = true;
        }

        if (Keyboard.GetKey(Key.A))
        {
            MetaData.Transform.Position -= Right * speed;
            updated = true;
        }

        if (Keyboard.GetKey(Key.D))
        {
            MetaData.Transform.Position += Right * speed;
            updated = true;
        }

        if (Keyboard.GetKey(Key.Space))
        {
            MetaData.Transform.Position += Transform.UpVector * speed;
            updated = true;
        }

        if (Keyboard.GetKey(Key.Shift))
        {
            MetaData.Transform.Position -= Transform.UpVector * speed;
            updated = true;
        }

        if (!updated) return;

        NeedsUpdate = true;
    }
}
