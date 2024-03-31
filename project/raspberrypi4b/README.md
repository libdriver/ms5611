### 1. Board

#### 1.1 Board Info

Board Name: Raspberry Pi 4B.

IIC Pin: SCL/SDA GPIO3/GPIO2.

SPI Pin: SCLK/MOSI/MISO/CS GPIO11/GPIO10/GPIO9/GPIO8.

### 2. Install

#### 2.1 Dependencies

Install the necessary dependencies.

```shell
sudo apt-get install libgpiod-dev pkg-config cmake -y
```

#### 2.2 Makefile

Build the project.

```shell
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

#### 2.3 CMake

Build the project.

```shell
mkdir build && cd build 
cmake .. 
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

Test the project and this is optional.

```shell
make test
```

Find the compiled library in CMake. 

```cmake
find_package(ms5611 REQUIRED)
```

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
./ms5611 -i

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
./ms5611 -p

ms5611: SPI interface SCK connected to GPIO11(BCM).
ms5611: SPI interface MISO connected to GPIO9(BCM).
ms5611: SPI interface MOSI connected to GPIO10(BCM).
ms5611: SPI interface CS connected to GPIO8(BCM).
ms5611: IIC interface SCL connected to GPIO3(BCM).
ms5611: IIC interface SDA connected to GPIO2(BCM).
```

```shell
./ms5611 -t read --interface=spi --times=3

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
ms5611: temperature is 31.11C.
ms5611: pressure is 1001.87mbar.
ms5611: temperature is 31.13C.
ms5611: pressure is 1001.91mbar.
ms5611: temperature is 31.11C.
ms5611: pressure is 1001.87mbar.
ms5611: set temperature osr 256.
ms5611: set pressure osr 256.
ms5611: temperature is 31.11C.
ms5611: pressure is 1001.87mbar.
ms5611: temperature is 31.11C.
ms5611: pressure is 1001.87mbar.
ms5611: temperature is 31.11C.
ms5611: pressure is 1001.87mbar.
ms5611: finish read test.
```

```shell
./ms5611 -e read --interface=spi --times=3

ms5611: 1/3.
ms5611: temperature is 30.74C.
ms5611: pressure is 1001.86mbar.
ms5611: 2/3.
ms5611: temperature is 30.73C.
ms5611: pressure is 1001.81mbar.
ms5611: 3/3.
ms5611: temperature is 30.71C.
ms5611: pressure is 1001.88mbar.
```

```shell
./ms5611 -h

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
