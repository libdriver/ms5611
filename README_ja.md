[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MS5611

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ms5611/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

MS5611 は、SPI および I2C バス インターフェイスを備えたスイス MEAS の新世代の高解像度高度計センサーです。 この気圧センサーは、高度分解能 10 cm の高度計およびバリオメーター用に最適化されています。センサー モジュールには、高線形性圧力センサーと、工場で内部校正された係数を備えた超低消費電力 24 ビット ΔΣ ADC が含まれています。 正確なデジタル 24 ビット圧力および温度値と、ユーザーが変換速度と消費電流を最適化できるさまざまな動作モードを提供します。 高分解能の温度出力により、追加のセンサーなしで高度計/温度計機能を実装できます。MS5611 は、事実上あらゆるマイクロ コントローラーに接続できます。 通信プロトコルはシンプルで、デバイスの内部レジスタをプログラミングする必要はありません。 わずか 5.0 mm x 3.0 mm、高さわずか 1.0 mm という小さな寸法により、モバイル デバイスへの統合が可能になります。 この新しい世代のセンサー モジュールは、最先端の MEMS テクノロジーと、10 年以上にわたって広く使用されてきた高度計モジュールの大量生産における MEAS Switzerland の実証済みの経験とノウハウからの最新のメリットに基づいています。 採用された検出原理により、ヒステリシスが非常に低くなり、圧力信号と温度信号の両方の安定性が高くなります。

LibDriver MS5611 は、LibDriver によって起動された MS5611 のフル機能ドライバーです。圧力と温度の読み取りおよびその他の機能を提供します。 LibDriver は MISRA に準拠しています。

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic](#example-basic)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver MS5611のソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver MS5611用のプラットフォームに依存しないIIC, SPIバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver MS5611ドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver MS5611プログラミング例が含まれています。

/ docディレクトリには、LibDriver MS5611オフラインドキュメントが含まれています。

/ datasheetディレクトリには、MS5611データシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

/ misraはLibDriver misraコードスキャン結果を含む。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないIIC, SPIバステンプレートを参照して、指定したプラットフォームのIIC, SPIバスドライバを完成させます。

/src ディレクトリ、プラットフォームのインターフェイス ドライバー、および独自のドライバーをプロジェクトに追加します。デフォルトのサンプル ドライバーを使用する場合は、/example ディレクトリをプロジェクトに追加します。

### 使用

/example ディレクトリ内のサンプルを参照して、独自のドライバーを完成させることができます。 デフォルトのプログラミング例を使用したい場合の使用方法は次のとおりです。

#### example basic

```C
#include "driver_ms5611_basic.h"

uint8_t res;
uint32_t i;

/* basic init */
res = ms5611_basic_init(MS5611_INTERFACE_IIC, MS5611_ADDRESS_CSB_0);
if (res != 0)
{
    return 1;
}

...
    
/* loop */
for (i = 0; i < 3; i++)
{
    float temperature_c;
    float pressure_mbar;

    /* read data */
    res = ms5611_basic_read(&temperature_c, &pressure_mbar);
    if (res != 0)
    {
        (void)ms5611_basic_deinit();

        return 1;
    }

    /* output */
    ms5611_interface_debug_print("ms5611: %d/%d.\n", i + 1, 3);
    ms5611_interface_debug_print("ms5611: temperature is %0.2fC.\n", temperature_c);
    ms5611_interface_debug_print("ms5611: pressure is %0.2fmbar.\n", pressure_mbar);

    /* delay 1000ms */
    ms5611_interface_delay_ms(1000);
    
    ...
}

...
    
/* basic deinit */
(void)ms5611_basic_deinit();

return 0;
```

### ドキュメント

オンラインドキュメント: [https://www.libdriver.com/docs/ms5611/index.html](https://www.libdriver.com/docs/ms5611/index.html)。

オフラインドキュメント: /doc/html/index.html。

### 貢献

CONTRIBUTING.mdを参照してください。

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com。