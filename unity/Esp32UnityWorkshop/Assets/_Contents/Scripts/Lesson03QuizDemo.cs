using UnityEngine;
using TMPro;

/// <summary>
/// 第3回デモ：ESP-NOW経由で届くプレイヤーのボタン入力を解析し、
/// 最初にボタンを押したプレイヤーを勝者として表示する早押しクイズ用スクリプト。
/// Serial通信の処理は Esp32SerialController に任せる。
/// </summary>
public class Lesson03QuizDemo : MonoBehaviour
{
    [Header("接続")]
    public Esp32SerialController serialController;

    [Header("表示テキスト")]
    public TextMeshProUGUI winnerText;

    [Header("メッセージ設定")]
    public string waitingMessage = "Waiting...";
    public string winnerMessagePrefix = "Winner: Player ";

    // 勝者が決まったかどうかを記録するフラグ
    bool _decided = false;

    void Start()
    {
        // 起動時は待機中メッセージを表示する
        ShowMessage(waitingMessage);
    }

    /// <summary>
    /// Esp32SerialController の onLineReceived に接続するメソッド。
    /// "player=N,button=1" 形式の行を解析して勝者を決定する。
    /// </summary>
    public void HandleSerialLine(string line)
    {
        // 既に勝者が決まっていたら新しい入力は無視する
        if (_decided) return;

        // カンマで分割して "player=N" と "button=1" の2つを取り出す
        string[] parts = line.Split(',');
        if (parts.Length < 2) return;

        string playerPart = parts[0].Trim();  // "player=N"
        string buttonPart = parts[1].Trim();  // "button=1"

        // ボタンが押された（button=1）でなければ無視する
        if (buttonPart != "button=1") return;

        // "player=" のプレフィックスを確認してプレイヤー番号を取り出す
        if (!playerPart.StartsWith("player=")) return;
        string playerNumber = playerPart.Substring("player=".Length);

        // 最初の勝者として記録し、テキストに表示する
        _decided = true;
        ShowMessage(winnerMessagePrefix + playerNumber);
        serialController?.WriteLine("winner=" + playerNumber);
    }

    /// <summary>
    /// クイズをリセットして待機状態に戻す。
    /// Reset ボタンの OnClick に接続する。
    /// </summary>
    public void ResetQuiz()
    {
        _decided = false;
        ShowMessage(waitingMessage);
        serialController?.WriteLine("clear_leds");
    }

    // winnerText にメッセージを表示する。未設定でも落ちない
    void ShowMessage(string message)
    {
        if (winnerText == null) return;
        winnerText.text = message;
    }
}
