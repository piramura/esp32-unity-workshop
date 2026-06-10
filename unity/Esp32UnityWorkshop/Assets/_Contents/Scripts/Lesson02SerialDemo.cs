using UnityEngine;

/// <summary>
/// 第2回デモ：ESP32からのボタン入力を受け取り、オブジェクトの色を変える。
/// 状態に応じてESP32へLED制御命令を返す。
/// Serial通信の処理はEsp32SerialControllerに任せる。
/// </summary>
public class Lesson02SerialDemo : MonoBehaviour
{
    [Header("接続")]
    public Esp32SerialController serialController;

    [Header("色の変化対象")]
    public Renderer targetRenderer;

    [Header("色の設定")]
    public Color normalColor = Color.white;
    public Color pressedColor = Color.red;

    // 現在のボタン状態（重複処理を避けるために保持する）
    bool _isPressed = false;

    void Start()
    {
        // 初期色を設定する
        ApplyColor(normalColor);
    }

    /// <summary>
    /// Esp32SerialController の onLineReceived に接続するメソッド。
    /// ESP32から届いた1行の文字列を解釈する。
    /// </summary>
    public void HandleSerialLine(string line)
    {
        if (line == "button=1")
        {
            // ボタンが押された
            if (!_isPressed)
            {
                _isPressed = true;
                ApplyColor(pressedColor);
                serialController?.WriteLine("led=1");
            }
        }
        else if (line == "button=0")
        {
            // ボタンが離された
            if (_isPressed)
            {
                _isPressed = false;
                ApplyColor(normalColor);
                serialController?.WriteLine("led=0");
            }
        }
    }

    // targetRendererの色を変える。未設定でも落ちない
    void ApplyColor(Color color)
    {
        if (targetRenderer == null) return;
        targetRenderer.material.color = color;
    }
}
