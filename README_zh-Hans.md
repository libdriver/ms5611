[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MS5611

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ms5611/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

MS5611是瑞士MEAS的新一代高分辨率高度计传感器，具有SPI和I2C总线接口。该气压传感器针对海拔分辨率为10厘米的高度计和变差计进行了优化。传感器模块包括一个高线性压力传感器和一个超低功率24位Δ∑ADC，具有内部工厂校准系数。它提供精确的数字24位压力和温度值以及不同的操作模式，允许用户优化转换速度和电流消耗。高分辨率温度输出允许在没有任何额外传感器的情况下实现高度计/温度计功能。MS5611几乎可以连接到任何微控制器，通信协议很简单，不需要对设备中的内部寄存器进行编程。仅5.0毫米x 3.0毫米的小尺寸和仅1.0毫米的高度允许集成在移动设备中。这一代新的传感器模块基于领先的MEMS技术和瑞士MEAS在高度计模块的大批量制造方面的经验和诀窍，这些技术已经广泛使用了十多年。所采用的传感原理使压力和温度信号拥有非常低的滞后性和高的稳定性。

LibDriver MS5611是LibDriver推出的MS5611全功能驱动，该驱动提供压强和温度读取等功能并且它符合MISRA标准。

### 目录

  - [说明](#说明)
  - [安装](#安装)
  - [使用](#使用)
    - [example basic](#example-basic)
  - [文档](#文档)
  - [贡献](#贡献)
  - [版权](#版权)
  - [联系我们](#联系我们)

### 说明

/src目录包含了LibDriver MS5611的源文件。

/interface目录包含了LibDriver MS5611与平台无关的IIC、SPI总线模板。

/test目录包含了LibDriver MS5611驱动测试程序，该程序可以简单的测试芯片必要功能。

/example目录包含了LibDriver MS5611编程范例。

/doc目录包含了LibDriver MS5611离线文档。

/datasheet目录包含了MS5611数据手册。

/project目录包含了常用Linux与单片机开发板的工程样例。所有工程均采用shell脚本作为调试方法，详细内容可参考每个工程里面的README.md。

/misra目录包含了LibDriver MISRA代码扫描结果。

### 安装

参考/interface目录下与平台无关的IIC、SPI总线模板，完成指定平台的IIC、SPI总线驱动。

将/src目录，您使用平台的接口驱动和您开发的驱动加入工程，如果您想要使用默认的范例驱动，可以将/example目录加入您的工程。

### 使用

您可以参考/example目录下的编程范例完成适合您的驱动，如果您想要使用默认的编程范例，以下是它们的使用方法。

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

### 文档

在线文档: [https://www.libdriver.com/docs/ms5611/index.html](https://www.libdriver.com/docs/ms5611/index.html)。

离线文档: /doc/html/index.html。

### 贡献

请参考CONTRIBUTING.md。

### 版权

版权 (c) 2015 - 现在 LibDriver 版权所有

MIT 许可证（MIT）

特此免费授予任何获得本软件副本和相关文档文件（下称“软件”）的人不受限制地处置该软件的权利，包括不受限制地使用、复制、修改、合并、发布、分发、转授许可和/或出售该软件副本，以及再授权被配发了本软件的人如上的权利，须在下列条件下：

上述版权声明和本许可声明应包含在该软件的所有副本或实质成分中。

本软件是“如此”提供的，没有任何形式的明示或暗示的保证，包括但不限于对适销性、特定用途的适用性和不侵权的保证。在任何情况下，作者或版权持有人都不对任何索赔、损害或其他责任负责，无论这些追责来自合同、侵权或其它行为中，还是产生于、源于或有关于本软件以及本软件的使用或其它处置。

### 联系我们

请联系lishifenging@outlook.com。