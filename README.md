[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MS5611

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ms5611/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

The MS5611 is a new generation of high resolution altimeter sensors from MEAS Switzerland with SPI and I2C bus interface. This barometric pressure sensor is optimized for altimeters and variometers with an altitude resolution of 10 cm.The sensor module includes a high linearity pressure sensor and an ultra-low power 24 bit ΔΣ ADC with internal factory calibrated coefficients. It provides a precise digital 24 Bit pressure and temperature value and different operation modes that allow the user to optimize for conversion speed and current consumption. A high resolution temperature output allows the implementation of an altimeter/thermometer function without any additional sensor.The MS5611 can be interfaced to virtually any micro controller. The communication protocol is simple, without the need of programming internal registers in the device. Small dimensions of only 5.0 mm x 3.0 mm and a height of only 1.0 mm allow for integration in mobile devices. This new sensor module generation is based on leading MEMS technology and latest benefits from MEAS Switzerland proven experience and know how in high volume manufacturing of altimeter modules, which have been widely used for over a decade. The sensing principle employed leads to very low hysteresis and high stability of both pressure and temperature signal.

LibDriver MS5611 is a full function driver of MS5611 launched by LibDriver.It provides pressure and temperature reading and other functions. LibDriver is MISRA compliant.

### Table of Contents

  - [Instruction](#Instruction)
  - [Install](#Install)
  - [Usage](#Usage)
    - [example basic](#example-basic)
  - [Document](#Document)
  - [Contributing](#Contributing)
  - [License](#License)
  - [Contact Us](#Contact-Us)

### Instruction

/src includes LibDriver MS5611 source files.

/interface includes LibDriver MS5611 IIC, SPI platform independent template.

/test includes LibDriver MS5611 driver test code and this code can test the chip necessary function simply.

/example includes LibDriver MS5611 sample code.

/doc includes LibDriver MS5611 offline document.

/datasheet includes MS5611 datasheet.

/project includes the common Linux and MCU development board sample code. All projects use the shell script to debug the driver and the detail instruction can be found in each project's README.md.

/misra includes the LibDriver MISRA code scanning results.

### Install

Reference /interface IIC, SPI platform independent template and finish your platform IIC, SPI driver.

Add the /src directory, the interface driver for your platform, and your own drivers to your project, if you want to use the default example drivers, add the /example directory to your project.

### Usage

You can refer to the examples in the /example directory to complete your own driver. If you want to use the default programming examples, here's how to use them.

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

### Document

Online documents: [https://www.libdriver.com/docs/ms5611/index.html](https://www.libdriver.com/docs/ms5611/index.html).

Offline documents: /doc/html/index.html.

### Contributing

Please refer to CONTRIBUTING.md.

### License

Copyright (c) 2015 - present LibDriver All rights reserved



The MIT License (MIT) 



Permission is hereby granted, free of charge, to any person obtaining a copy

of this software and associated documentation files (the "Software"), to deal

in the Software without restriction, including without limitation the rights

to use, copy, modify, merge, publish, distribute, sublicense, and/or sell

copies of the Software, and to permit persons to whom the Software is

furnished to do so, subject to the following conditions: 



The above copyright notice and this permission notice shall be included in all

copies or substantial portions of the Software. 



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE

AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

SOFTWARE. 

### Contact Us

Please send an e-mail to lishifenging@outlook.com.