using System;
using System.IO.Ports;
using System.Text;
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

    // ReadExisting() で届いたデータを改行まで溜めるバッファ
    readonly StringBuilder _buffer = new StringBuilder();

    void OnEnable()
    {
        // GameObjectが有効になったときにSerialポートを開く
        try
        {
            _port = new SerialPort(portName, baudRate)
            {
                NewLine = "\n",
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

        // 受信バッファにある分をまとめて読む（ブロックしない）
        string chunk;
        try
        {
            chunk = _port.ReadExisting();
        }
        catch (Exception e)
        {
            Debug.LogWarning($"[Esp32SerialController] 読み取りエラー: {e.Message}");
            return;
        }

        if (string.IsNullOrEmpty(chunk)) return;

        _buffer.Append(chunk);

        // バッファ内の完成した行を順番に取り出す
        while (true)
        {
            string buf = _buffer.ToString();
            int newlineIndex = buf.IndexOf('\n');
            if (newlineIndex < 0) break;

            string line = buf.Substring(0, newlineIndex).Trim();
            _buffer.Remove(0, newlineIndex + 1);

            if (line.Length > 0)
            {
                onLineReceived?.Invoke(line);
            }
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

    void OnDisable()
    {
        // GameObjectが無効になったとき（Play停止を含む）にSerialポートを閉じて解放する
        if (_port != null)
        {
            if (_port.IsOpen) _port.Close();
            _port.Dispose();
            _port = null;
        }

        _buffer.Clear();
    }
}
