using UnityEngine;

/// <summary>
/// 第2回用の最小サンプル。
/// ESP32から届いた文字列をConsoleに表示するだけの雛形。
/// </summary>
public class Lesson02SerialTemplate : MonoBehaviour
{
    /// <summary>
    /// Esp32SerialController の onLineReceived に接続するメソッド。
    /// </summary>
    public void HandleSerialLine(string line)
    {
        Debug.Log($"ESP32から受信: {line}");

        if (line == "button=1")
        {
            Debug.Log("ボタンが押されました");
        }
        else if (line == "button=0")
        {
            Debug.Log("ボタンが離されました");
        }
    }
}
