### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

UART Pin: TX/RX PA9/PA10.

IIC Pin: SCL/SDA PB8/PB9.

SPI Pin: SCK/MISO/MOSI/CS  PA5/PA6/PA7/PA4.

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LibDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. MS5611

#### 3.1 Command Instruction

1. Show ms5611 chip and driver information.

   ```shell
   ms5611 (-i | --information)
   ```

2. Show ms5611 help.

   ```shell
   ms5611 (-h | --help)
   ```

3. Show ms5611 pin connections of the current board.

   ```shell
   ms5611 (-p | --port)
   ```

5. Run ms5611 read test, num means the test times. 

   ```shell
   ms5611 (-t read | --test=read) [--csb=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]
   ```

5. Run ms5611 read function, num means the read times. 

   ```shell
   ms5611 (-e read | --example=read) [--csb=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]
   ```

#### 3.2 Command Example

```shell
ms5611 -i

ms5611: chip is TE MS5611.
ms5611: manufacturer is TE.
ms5611: interface is IIC SPI.
ms5611: driver version is 1.0.
ms5611: min supply voltage is 1.8V.
ms5611: max supply voltage is 3.6V.
ms5611: max current is 1.40mA.
ms5611: max temperature is 85.0C.
ms5611: min temperature is -40.0C.
```

```shell
ms5611 -p

ms5611: SPI interface SCK connected to GPIOA PIN5.
ms5611: SPI interface MISO connected to GPIOA PIN6.
ms5611: SPI interface MOSI connected to GPIOA PIN7.
ms5611: SPI interface CS connected to GPIOA PIN4.
ms5611: IIC interface SCL connected to GPIOB PIN8.
ms5611: IIC interface SDA connected to GPIOB PIN9.
```

```shell
ms5611 -t read --csb=0 --interface=iic --times=3

ms5611: chip is TE MS5611.
ms5611: manufacturer is TE.
ms5611: interface is IIC SPI.
ms5611: driver version is 1.0.
ms5611: min supply voltage is 1.8V.
ms5611: max supply voltage is 3.6V.
ms5611: max current is 1.40mA.
ms5611: max temperature is 85.0C.
ms5611: min temperature is -40.0C.
ms5611: start read test.
ms5611: set temperature osr 4096.
ms5611: set pressure osr 4096.
ms5611: temperature is 27.50C.
ms5611: pressure is 1001.87mbar.
ms5611: temperature is 27.52C.
ms5611: pressure is 1001.91mbar.
ms5611: temperature is 27.55C.
ms5611: pressure is 1001.89mbar.
ms5611: set temperature osr 256.
ms5611: set pressure osr 256.
ms5611: temperature is 27.57C.
ms5611: pressure is 1001.93mbar.
ms5611: temperature is 27.59C.
ms5611: pressure is 1001.97mbar.
ms5611: temperature is 27.60C.
ms5611: pressure is 1001.91mbar.
ms5611: finish read test.
```

```shell
ms5611 -e read --csb=0 --interface=iic --times=3

ms5611: 1/3.
ms5611: temperature is 27.97C.
ms5611: pressure is 1001.82mbar.
ms5611: 2/3.
ms5611: temperature is 27.97C.
ms5611: pressure is 1001.93mbar.
ms5611: 3/3.
ms5611: temperature is 28.01C.
ms5611: pressure is 1001.91mbar.
```

```shell
ms5611 -h

Usage:
  ms5611 (-i | --information)
  ms5611 (-h | --help)
  ms5611 (-p | --port)
  ms5611 (-t read | --test=read) [--csb=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]
  ms5611 (-e read | --example=read) [--csb=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]

Options:
      --csb=<0 | 1>                  Set the chip iic address.([default: 0])
  -e read, --example=read            Run the driver example.
  -h, --help                         Show the help.
  -i, --information                  Show the chip information.
      --interface=<iic | spi>        Set the chip interface.([default: iic])
  -p, --port                         Display the pin connections of the current board.
  -t read, --test=read               Run the driver test.
      --times=<num>                  Set the running times.([default: 3])
```

