[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MS5611

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ms5611/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

MS5611是瑞士MEAS的新一代高解析度高度計感測器，具有SPI和I2C匯流排介面。 該氣壓感測器針對海拔分辯率為10釐米的高度計和變差計進行了優化。 感測器模塊包括一個高線性壓力感測器和一個超低功率24比特 Δ∑ ADC，具有內部工廠校準係數。 它提供精確的數位24比特壓力和溫度值以及不同的操作模式，允許用戶優化轉換速度和電流消耗。 高解析度溫度輸出允許在沒有任何額外感測器的情况下實現高度計/溫度計功能。 MS5611幾乎可以連接到任何微控制器，通信協議很簡單，不需要對設備中的內部寄存器進行程式設計。 僅5.0毫米x 3.0毫米的小尺寸和僅1.0毫米的高度允許集成在移動設備中。 這一代新的感測器模塊基於領先的MEMS科技和瑞士MEAS在高度計模塊的大批量製造方面的經驗和訣竅，這些科技已經廣泛使用了十多年。 所採用的傳感原理使壓力和溫度訊號擁有非常低的滯後性和高的穩定性。

LibDriver MS5611是LibDriver推出的MS5611全功能驅動，該驅動提供壓強和溫度讀取等功能並且它符合MISRA標準。

### 目錄

  - [說明](#說明)
  - [安裝](#安裝)
  - [使用](#使用)
    - [example basic](#example-basic)
  - [文檔](#文檔)
  - [貢獻](#貢獻)
  - [版權](#版權)
  - [聯繫我們](#聯繫我們)

### 說明

/src目錄包含了LibDriver MS5611的源文件。

/interface目錄包含了LibDriver MS5611與平台無關的IIC、SPI總線模板。

/test目錄包含了LibDriver MS5611驅動測試程序，該程序可以簡單的測試芯片必要功能。

/example目錄包含了LibDriver MS5611編程範例。

/doc目錄包含了LibDriver MS5611離線文檔。

/datasheet目錄包含了MS5611數據手冊。

/project目錄包含了常用Linux與單片機開發板的工程樣例。所有工程均採用shell腳本作為調試方法，詳細內容可參考每個工程裡面的README.md。

/misra目錄包含了LibDriver MISRA程式碼掃描結果。

### 安裝

參考/interface目錄下與平台無關的IIC、SPI總線模板，完成指定平台的IIC、SPI總線驅動。

將/src目錄，您使用平臺的介面驅動和您開發的驅動加入工程，如果您想要使用默認的範例驅動，可以將/example目錄加入您的工程。

### 使用

您可以參考/example目錄下的程式設計範例完成適合您的驅動，如果您想要使用默認的程式設計範例，以下是它們的使用方法。

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

### 文檔

在線文檔: [https://www.libdriver.com/docs/ms5611/index.html](https://www.libdriver.com/docs/ms5611/index.html)。

離線文檔: /doc/html/index.html。

### 貢獻

請參攷CONTRIBUTING.md。

### 版權

版權 (c) 2015 - 現在 LibDriver 版權所有

MIT 許可證（MIT）

特此免費授予任何獲得本軟件副本和相關文檔文件（下稱“軟件”）的人不受限制地處置該軟件的權利，包括不受限制地使用、複製、修改、合併、發布、分發、轉授許可和/或出售該軟件副本，以及再授權被配發了本軟件的人如上的權利，須在下列條件下：

上述版權聲明和本許可聲明應包含在該軟件的所有副本或實質成分中。

本軟件是“如此”提供的，沒有任何形式的明示或暗示的保證，包括但不限於對適銷性、特定用途的適用性和不侵權的保證。在任何情況下，作者或版權持有人都不對任何索賠、損害或其他責任負責，無論這些追責來自合同、侵權或其它行為中，還是產生於、源於或有關於本軟件以及本軟件的使用或其它處置。

### 聯繫我們

請聯繫lishifenging@outlook.com。
