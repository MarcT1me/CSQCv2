using Engine.Events.QuantumEvents;
using Engine.Events.QuantumEvents.Mouse;
using Engine.Objects.Camera;
using Engine.Input.Keyboard;
using Engine.Time;

namespace AppLib.Game;

public class GameCamera(CameraData cameraData) : Camera<CameraData>(cameraData)
{
    public float CameraSpeed = 0.025f;
    public float CameraSensitivity = 0.00125f;

    public override void HandleEvent(QuantumEvent e)
    {
        base.HandleEvent(e);

        if (e is not MouseMoveEvent mouseMove || !TestApp.Instance.MainWindow.MetaData.WinData.Fullscreen) return;

        // yaw-pitch
        Transform.Rotate(Transform.Right, CameraSensitivity * mouseMove.Rel.Y);
        Transform.Rotate(Engine.Data.Transform.WorldUp, -CameraSensitivity * mouseMove.Rel.X);
    }

    public override void Update(ClockMeta clockMeta)
    {
        base.Update(clockMeta);

        var moveSpeed = CameraSpeed * (float)clockMeta.DeltaTime;

        // x-y
        if (Keyboard.GetKey(Key.W))
            Transform.Translate(Transform.Forward * moveSpeed);
        if (Keyboard.GetKey(Key.S))
            Transform.Translate(-Transform.Forward * moveSpeed);
        if (Keyboard.GetKey(Key.D))
            Transform.Translate(-Transform.Right * moveSpeed);
        if (Keyboard.GetKey(Key.A))
            Transform.Translate(Transform.Right * moveSpeed);

        // z
        if (Keyboard.GetKey(Key.R))
            Transform.Translate(Transform.Up * moveSpeed);
        if (Keyboard.GetKey(Key.F))
            Transform.Translate(-Transform.Up * moveSpeed);

        var rotationSpeed = moveSpeed / 20;

        // yaw-pitch
        if (Keyboard.GetKey(Key.Up))
            Transform.Rotate(Transform.Right, -rotationSpeed);
        if (Keyboard.GetKey(Key.Down))
            Transform.Rotate(Transform.Right, rotationSpeed);
        if (Keyboard.GetKey(Key.Right))
            Transform.Rotate(Engine.Data.Transform.WorldUp, -rotationSpeed);
        if (Keyboard.GetKey(Key.Left))
            Transform.Rotate(Engine.Data.Transform.WorldUp, rotationSpeed);
    }
}