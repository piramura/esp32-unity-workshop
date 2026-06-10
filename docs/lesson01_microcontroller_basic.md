# 第1回：マイコンのみ

# ゴール
マイコン、センサー、アクチュエータといったマイコン関連の基本事項を知ってもらい、実際にSeeed Studio XIAO ESP32C6を用いてセンサーを読み取れることを確認し、LEDをチカチカさせます。

# この回の流れ
1. GitHubからレポジトリを持ってくる
1. VScodeの拡張機能のPlatformIO・SerialMonitorを入れる
1. PlatformIOでプロジェクトを開く
1. マイコンのはんだ付け
1. LEDとタクトスイッチの回路作り
1. PlatformIOプロジェクトをコンパイルできるようにする
1. マイコンとシリアル通信
1. マイコンでLEDチカチカ
1. マイコンでスイッチ読んでLEDを光らせる
1. 時間あったらセンサー・アクチュエータ紹介

# GitHubからレポジトリを持ってくる
GitHubからレポジトリを持ってきます。
https://github.com/piramura/esp32-unity-workshop 

# VScodeの拡張機能のPlatformIO・SerialMonitorを入れる
VScodeを開いて拡張機能を入れましょう。
1. VScodeの拡張機能を入れます。拡張機能からPlatformIOと調べて、アリのようなアイコンのものを入れます。入れるところまでは[このサイト](https://qiita.com/nextfp/items/f54b216212f08280d4e0)が参考になりそうです。
1. また、SerialMonitorと調べてMicroSoftのSerialMonitorを入れます。

ここまで来たら、一旦VScodeを閉じます。
1. 何も開いていない新しいVScodeのウィンドウを出します。
1. ここで、拡張機能として左側にアリマークが増えるので、押します。
1. PlatformIOのPIO Homeのタブが出てくると思う(出てこなければOpen)のでOpenボタンを押します。
1. ここで、GitHubから持ってきた中の以下のフォルダを選択してOpenを押します。```esp32-unity-workshop\firmware\lesson01_microcontroller_basic```
こっから**めちゃくちゃ**時間がかかるので違う作業に移ります。

# マイコンのはんだ付け
その場で教えるので特に書かないでおきます。

# LEDとタクトスイッチの回路作り
プログラムを書き込む前に回路を作りましょう。
ブレッドボードに部品を挿していきます。
部品一覧
- はんだ付けしたマイコン
- LED
- タクトスイッチ
- 約10kΩ抵抗
- 200Ω以上の抵抗なんでも
- ジャンパワイヤー

回路図は以下のようになります。
![回路図](https://github.com/piramura/esp32-unity-workshop/blob/origin/lesson1/images/lesson01/kairozu.png "サンプル")