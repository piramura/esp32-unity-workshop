using System;
using System.IO.Ports;
using UnityEngine;
using UnityEngine.Events;

/// <summary>
/// ESP32とのSerial通信を担当するコンポーネント。
/// ポートの開閉、1行読み書き、受信イベントの通知を行う。
/// LED制御や色変更などのゲームロジックはここに書かない。
/// </summary>
public class Esp32SerialController : MonoBehaviour
{
    [Header("Serial設定")]
    // Inspectorでポート名を設定する（例: macOS=/dev/cu.usbmodem*, Windows=COM3）
    public string portName = "/dev/cu.usbmodem1101";
    public int baudRate = 115200;

    [Header("受信イベント")]
    // 1行受信するたびに呼ばれる。引数は受信した文字列（Trim済み）
    public UnityEvent<string> onLineReceived;

    SerialPort _port;

    void Start()
    {
        // Playボタンを押したときにSerialポートを開く
        try
        {
            _port = new SerialPort(portName, baudRate)
            {
                NewLine = "\n",
                ReadTimeout = 10,   // 短いタイムアウトで毎フレームノンブロッキングに読む
            };
            _port.Open();
            Debug.Log($"[Esp32SerialController] ポートを開きました: {portName}");
        }
        catch (Exception e)
        {
            Debug.LogError($"[Esp32SerialController] ポートを開けませんでした: {portName}\n{e.Message}");
        }
    }

    void Update()
    {
        if (_port == null || !_port.IsOpen) return;

        // 毎フレーム1行読む。読めなければスキップする
        try
        {
            string line = _port.ReadLine();
            onLineReceived?.Invoke(line.Trim());
        }
        catch (TimeoutException)
        {
            // タイムアウトは正常。受信待ちの間は毎フレームここに来る
        }
        catch (Exception e)
        {
            Debug.LogWarning($"[Esp32SerialController] 読み取りエラー: {e.Message}");
        }
    }

    /// <summary>
    /// ESP32へ1行送信する（末尾に改行が付く）。
    /// </summary>
    public void WriteLine(string line)
    {
        if (_port == null || !_port.IsOpen)
        {
            Debug.LogWarning("[Esp32SerialController] ポートが開いていないため送信できません。");
            return;
        }

        try
        {
            _port.WriteLine(line);
        }
        catch (Exception e)
        {
            Debug.LogWarning($"[Esp32SerialController] 送信エラー: {e.Message}");
        }
    }

    void OnDestroy()
    {
        // Playを止めたときにSerialポートを閉じて解放する
        if (_port != null)
        {
            if (_port.IsOpen) _port.Close();
            _port.Dispose();
            _port = null;
        }
    }
}
