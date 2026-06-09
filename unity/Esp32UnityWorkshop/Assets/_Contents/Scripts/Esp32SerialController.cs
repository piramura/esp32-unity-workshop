using System;
using System.IO;
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
    public string portName = "/dev/cu.usbmodem101";
    public int baudRate = 115200;
    public float warmupDelay = 1.0f;
    public float reconnectInterval = 1.0f;

    [Header("受信イベント")]
    // 1行受信するたびに呼ばれる。引数は受信した文字列（Trim済み）
    public UnityEvent<string> onLineReceived;

    SerialPort _port;
    float _readStartTime;
    float _nextReconnectTime;
    string _lastOpenedPortName = "";

    // ReadExisting() で届いたデータを改行まで溜めるバッファ
    readonly StringBuilder _buffer = new StringBuilder();

    void OnEnable()
    {
        // GameObjectが有効になったときにSerialポートを開く
        TryOpenPort();
    }

    void TryOpenPort()
    {
        if (_port != null && _port.IsOpen) return;

        string resolvedPortName = ResolvePortName();
        if (string.IsNullOrEmpty(resolvedPortName))
        {
            Debug.LogWarning("[Esp32SerialController] Serialポートが見つかりません。ESP32を接続してportNameを確認してください。");
            _nextReconnectTime = Time.unscaledTime + reconnectInterval;
            return;
        }

        try
        {
            _port = new SerialPort(resolvedPortName, baudRate)
            {
                NewLine = "\n",
                ReadTimeout = 100,
                WriteTimeout = 100,
                DtrEnable = false,
                RtsEnable = false,
                Handshake = Handshake.None,
            };
            _port.Open();
            _port.DiscardInBuffer();
            _readStartTime = Time.unscaledTime + warmupDelay;
            _lastOpenedPortName = resolvedPortName;
            Debug.Log($"[Esp32SerialController] 接続しました: {resolvedPortName}");
            Debug.Log($"[Esp32SerialController] {warmupDelay:0.0}秒待ってから読み取りを開始します");
        }
        catch (Exception e)
        {
            Debug.LogError($"[Esp32SerialController] ポートを開けませんでした: {resolvedPortName}\n{e.Message}\n候補: {GetPortListText()}");
            ClosePort();
            _nextReconnectTime = Time.unscaledTime + reconnectInterval;
        }
    }

    void Update()
    {
        if (_port == null || !_port.IsOpen)
        {
            if (Time.unscaledTime >= _nextReconnectTime)
            {
                TryOpenPort();
            }
            return;
        }

        if (Time.unscaledTime < _readStartTime) return;

        // 受信バッファにある分をまとめて読む（ブロックしない）
        string chunk;
        try
        {
            chunk = _port.ReadExisting();
        }
        catch (TimeoutException)
        {
            return;
        }
        catch (Exception e)
        {
            Debug.LogWarning($"[Esp32SerialController] 読み取りエラー: {e.Message}");
            ClosePort();
            _nextReconnectTime = Time.unscaledTime + reconnectInterval;
            Debug.Log($"[Esp32SerialController] {reconnectInterval:0.0}秒後に再接続します");
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
            ClosePort();
            _nextReconnectTime = Time.unscaledTime + reconnectInterval;
            Debug.Log($"[Esp32SerialController] {reconnectInterval:0.0}秒後に再接続します");
        }
    }

    void OnDisable()
    {
        // GameObjectが無効になったとき（Play停止を含む）にSerialポートを閉じて解放する
        ClosePort();
        _buffer.Clear();
    }

    void ClosePort()
    {
        if (_port != null)
        {
            try
            {
                if (_port.IsOpen)
                {
                    _port.Close();
                }
                _port.Dispose();
            }
            catch (Exception e)
            {
                Debug.LogWarning($"[Esp32SerialController] ポートの解放中にエラー: {e.Message}");
            }
            Debug.Log($"[Esp32SerialController] 切断しました: {_lastOpenedPortName}");
            _port = null;
            _lastOpenedPortName = "";
        }
    }

    string ResolvePortName()
    {
        string requestedPortName = (portName ?? "").Trim();

        if (IsPortAvailable(requestedPortName))
        {
            return requestedPortName;
        }

        // macOSではUnityから開くSerialポートは /dev/cu.* を使う。
        if (requestedPortName.StartsWith("/dev/tty.", StringComparison.Ordinal))
        {
            string cuPortName = "/dev/cu." + requestedPortName.Substring("/dev/tty.".Length);
            if (IsPortAvailable(cuPortName))
            {
                portName = cuPortName;
                return cuPortName;
            }
        }

        string[] candidates = GetPortCandidates();
        if (candidates.Length == 0)
        {
            return "";
        }

        portName = candidates[0];
        return portName;
    }

    bool IsPortAvailable(string candidate)
    {
        if (string.IsNullOrEmpty(candidate)) return false;
        if (candidate.StartsWith("/dev/", StringComparison.Ordinal)) return File.Exists(candidate);

        foreach (string availablePortName in GetSystemPortNames())
        {
            if (availablePortName == candidate) return true;
        }

        return false;
    }

    string[] GetPortCandidates()
    {
        if (Directory.Exists("/dev"))
        {
            string[] patterns = { "cu.usbmodem*", "cu.usbserial*" };
            foreach (string pattern in patterns)
            {
                string[] ports = Directory.GetFiles("/dev", pattern);
                Array.Sort(ports);
                if (ports.Length > 0) return ports;
            }
        }

        string[] serialPorts = GetSystemPortNames();
        Array.Sort(serialPorts);
        return serialPorts;
    }

    string[] GetSystemPortNames()
    {
        try
        {
            return SerialPort.GetPortNames();
        }
        catch (Exception e)
        {
            Debug.LogWarning($"[Esp32SerialController] Serialポート一覧を取得できませんでした: {e.Message}");
            return Array.Empty<string>();
        }
    }

    string GetPortListText()
    {
        string[] ports = GetPortCandidates();
        if (ports.Length == 0) return "なし";
        return string.Join(", ", ports);
    }
}
