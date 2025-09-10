using QuantumCore.Data;
using QuantumCore.Events.QuantumEvents;
using QuantumCore.Events.QuantumEvents.Mouse;
using QuantumCore.Objects.Camera;
using QuantumCore.Input.Keyboard;
using QuantumCore.Time;

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
        MetaData.Transform.Rotate(MetaData.Transform.Right, CameraSensitivity * mouseMove.Rel.Y);
        MetaData.Transform.Rotate(Transform.WorldUp, -CameraSensitivity * mouseMove.Rel.X);
    }

    public override void Update(ClockMeta clockMeta)
    {
        base.Update(clockMeta);

        var moveSpeed = CameraSpeed * (float)clockMeta.DeltaTime;

        // x-y
        if (Keyboard.GetKey(Key.W))
            MetaData.Transform.Translate(MetaData.Transform.Forward * moveSpeed);
        if (Keyboard.GetKey(Key.S))
            MetaData.Transform.Translate(-MetaData.Transform.Forward * moveSpeed);
        if (Keyboard.GetKey(Key.D))
            MetaData.Transform.Translate(-MetaData.Transform.Right * moveSpeed);
        if (Keyboard.GetKey(Key.A))
            MetaData.Transform.Translate(MetaData.Transform.Right * moveSpeed);

        // z
        if (Keyboard.GetKey(Key.R))
            MetaData.Transform.Translate(MetaData.Transform.Up * moveSpeed);
        if (Keyboard.GetKey(Key.F))
            MetaData.Transform.Translate(-MetaData.Transform.Up * moveSpeed);

        var rotationSpeed = moveSpeed / 20;

        // yaw-pitch
        if (Keyboard.GetKey(Key.Up))
            MetaData.Transform.Rotate(MetaData.Transform.Right, -rotationSpeed);
        if (Keyboard.GetKey(Key.Down))
            MetaData.Transform.Rotate(MetaData.Transform.Right, rotationSpeed);
        if (Keyboard.GetKey(Key.Right))
            MetaData.Transform.Rotate(QuantumCore.Data.Transform.WorldUp, -rotationSpeed);
        if (Keyboard.GetKey(Key.Left))
            MetaData.Transform.Rotate(QuantumCore.Data.Transform.WorldUp, rotationSpeed);
    }
}