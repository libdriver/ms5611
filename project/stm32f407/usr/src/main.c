/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      main.c
 * @brief     main source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2024-03-31
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2024/03/31  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_ms5611_read_test.h"
#include "driver_ms5611_basic.h"
#include "shell.h"
#include "clock.h"
#include "delay.h"
#include "uart.h"
#include "mutex.h"
#include "getopt.h"
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];                        /**< uart buffer */
volatile uint16_t g_len;                   /**< uart buffer length */

/**
 * @brief     ms5611 full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t ms5611(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"csb", required_argument, NULL, 1},
        {"interface", required_argument, NULL, 2},
        {"times", required_argument, NULL, 3},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    uint32_t times = 3;
    ms5611_address_t addr = MS5611_ADDRESS_CSB_0;
    ms5611_interface_t interface = MS5611_INTERFACE_IIC;
    
    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }
    
    /* init 0 */
    optind = 0;
    
    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);
        
        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");
                
                break;
            }
            
            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");
                
                break;
            }
            
            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");
                
                break;
            }
            
            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);
                
                break;
            }
            
            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);
                
                break;
            }
            
            /* csb */
            case 1 :
            {
                /* set the addr pin */
                if (strcmp("0", optarg) == 0)
                {
                    addr = MS5611_ADDRESS_CSB_0;
                }
                else if (strcmp("1", optarg) == 0)
                {
                    addr = MS5611_ADDRESS_CSB_1;
                }
                else
                {
                    return 5;
                }
                
                break;
            }
            
            /* interface */
            case 2 :
            {
                /* set the interface */
                if (strcmp("iic", optarg) == 0)
                {
                    interface = MS5611_INTERFACE_IIC;
                }
                else if (strcmp("spi", optarg) == 0)
                {
                    interface = MS5611_INTERFACE_SPI;
                }
                else
                {
                    return 5;
                }
                
                break;
            }
            
            /* running times */
            case 3 :
            {
                /* set the times */
                times = atol(optarg);
                
                break;
            } 
            
            /* the end */
            case -1 :
            {
                break;
            }
            
            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_read", type) == 0)
    {
        uint8_t res;
        
        /* read test */
        res = ms5611_read_test(interface, addr, times);
        if (res != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("e_read", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        
        /* basic init */
        res = ms5611_basic_init(interface, addr);
        if (res != 0)
        {
            return 1;
        }
        
        /* loop */
        for (i = 0; i < times; i++)
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
            ms5611_interface_debug_print("ms5611: %d/%d.\n", i + 1, times);
            ms5611_interface_debug_print("ms5611: temperature is %0.2fC.\n", temperature_c);
            ms5611_interface_debug_print("ms5611: pressure is %0.2fmbar.\n", pressure_mbar);
            
            /* delay 1000ms */
            ms5611_interface_delay_ms(1000);
        }
        
        /* basic deinit */
        (void)ms5611_basic_deinit();
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        ms5611_interface_debug_print("Usage:\n");
        ms5611_interface_debug_print("  ms5611 (-i | --information)\n");
        ms5611_interface_debug_print("  ms5611 (-h | --help)\n");
        ms5611_interface_debug_print("  ms5611 (-p | --port)\n");
        ms5611_interface_debug_print("  ms5611 (-t read | --test=read) [--csb=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]\n");
        ms5611_interface_debug_print("  ms5611 (-e read | --example=read) [--csb=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]\n");
        ms5611_interface_debug_print("\n");
        ms5611_interface_debug_print("Options:\n");
        ms5611_interface_debug_print("      --csb=<0 | 1>                  Set the chip iic address.([default: 0])\n");
        ms5611_interface_debug_print("  -e read, --example=read            Run the driver example.\n");
        ms5611_interface_debug_print("  -h, --help                         Show the help.\n");
        ms5611_interface_debug_print("  -i, --information                  Show the chip information.\n");
        ms5611_interface_debug_print("      --interface=<iic | spi>        Set the chip interface.([default: iic])\n");
        ms5611_interface_debug_print("  -p, --port                         Display the pin connections of the current board.\n");
        ms5611_interface_debug_print("  -t read, --test=read               Run the driver test.\n");
        ms5611_interface_debug_print("      --times=<num>                  Set the running times.([default: 3])\n");

        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        ms5611_info_t info;
        
        /* print ms5611 info */
        ms5611_info(&info);
        ms5611_interface_debug_print("ms5611: chip is %s.\n", info.chip_name);
        ms5611_interface_debug_print("ms5611: manufacturer is %s.\n", info.manufacturer_name);
        ms5611_interface_debug_print("ms5611: interface is %s.\n", info.interface);
        ms5611_interface_debug_print("ms5611: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        ms5611_interface_debug_print("ms5611: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        ms5611_interface_debug_print("ms5611: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        ms5611_interface_debug_print("ms5611: max current is %0.2fmA.\n", info.max_current_ma);
        ms5611_interface_debug_print("ms5611: max temperature is %0.1fC.\n", info.temperature_max);
        ms5611_interface_debug_print("ms5611: min temperature is %0.1fC.\n", info.temperature_min);
        
        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        ms5611_interface_debug_print("ms5611: SPI interface SCK connected to GPIOA PIN5.\n");
        ms5611_interface_debug_print("ms5611: SPI interface MISO connected to GPIOA PIN6.\n");
        ms5611_interface_debug_print("ms5611: SPI interface MOSI connected to GPIOA PIN7.\n");
        ms5611_interface_debug_print("ms5611: SPI interface CS connected to GPIOA PIN4.\n");
        ms5611_interface_debug_print("ms5611: IIC interface SCL connected to GPIOB PIN8.\n");
        ms5611_interface_debug_print("ms5611: IIC interface SDA connected to GPIOB PIN9.\n");
        
        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief main function
 * @note  none
 */
int main(void)
{
    uint8_t res;
    
    /* stm32f407 clock init and hal init */
    clock_init();
    
    /* delay init */
    delay_init();
    
    /* uart init */
    uart_init(115200);
    
    /* shell init && register ms5611 function */
    shell_init();
    shell_register("ms5611", ms5611);
    uart_print("ms5611: welcome to libdriver ms5611.\n");
    
    while (1)
    {
        /* read uart */
        g_len = uart_read(g_buf, 256);
        if (g_len != 0)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart_print("ms5611: run failed.\n");
            }
            else if (res == 2)
            {
                uart_print("ms5611: unknown command.\n");
            }
            else if (res == 3)
            {
                uart_print("ms5611: length is too long.\n");
            }
            else if (res == 4)
            {
                uart_print("ms5611: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart_print("ms5611: param is invalid.\n");
            }
            else
            {
                uart_print("ms5611: unknown status code.\n");
            }
            uart_flush();
        }
        delay_ms(100);
    }
}
