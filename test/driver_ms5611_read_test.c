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
 * @file      driver_ms5611_read_test.c
 * @brief     driver ms5611 read test source file
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

static ms5611_handle_t gs_handle;        /**< ms5611 handle */

/**
 * @brief     read test
 * @param[in] interface is the chip interface
 * @param[in] addr_pin is the iic device address
 * @param[in] times is the test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t ms5611_read_test(ms5611_interface_t interface, ms5611_address_t addr_pin, uint32_t times)
{
    uint8_t res;
    uint32_t i;
    ms5611_info_t info;
    
    /* link interface function */
    DRIVER_MS5611_LINK_INIT(&gs_handle, ms5611_handle_t);
    DRIVER_MS5611_LINK_IIC_INIT(&gs_handle, ms5611_interface_iic_init);
    DRIVER_MS5611_LINK_IIC_DEINIT(&gs_handle, ms5611_interface_iic_deinit);
    DRIVER_MS5611_LINK_IIC_READ(&gs_handle, ms5611_interface_iic_read);
    DRIVER_MS5611_LINK_IIC_WRITE(&gs_handle, ms5611_interface_iic_write);
    DRIVER_MS5611_LINK_SPI_INIT(&gs_handle, ms5611_interface_spi_init);
    DRIVER_MS5611_LINK_SPI_DEINIT(&gs_handle, ms5611_interface_spi_deinit);
    DRIVER_MS5611_LINK_SPI_READ(&gs_handle, ms5611_interface_spi_read);
    DRIVER_MS5611_LINK_SPI_WRITE(&gs_handle, ms5611_interface_spi_write);
    DRIVER_MS5611_LINK_DELAY_MS(&gs_handle, ms5611_interface_delay_ms);
    DRIVER_MS5611_LINK_DEBUG_PRINT(&gs_handle, ms5611_interface_debug_print);
    
    /* get information */
    res = ms5611_info(&info);
    if (res != 0)
    {
        ms5611_interface_debug_print("ms5611: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip info */
        ms5611_interface_debug_print("ms5611: chip is %s.\n", info.chip_name);
        ms5611_interface_debug_print("ms5611: manufacturer is %s.\n", info.manufacturer_name);
        ms5611_interface_debug_print("ms5611: interface is %s.\n", info.interface);
        ms5611_interface_debug_print("ms5611: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        ms5611_interface_debug_print("ms5611: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        ms5611_interface_debug_print("ms5611: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        ms5611_interface_debug_print("ms5611: max current is %0.2fmA.\n", info.max_current_ma);
        ms5611_interface_debug_print("ms5611: max temperature is %0.1fC.\n", info.temperature_max);
        ms5611_interface_debug_print("ms5611: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* set the interface */
    res = ms5611_set_interface(&gs_handle, interface);
    if (res != 0)
    {
        ms5611_interface_debug_print("ms5611: set interface failed.\n");
       
        return 1;
    }
    
    /* set the address pin */
    res = ms5611_set_addr_pin(&gs_handle, addr_pin);
    if (res != 0)
    {
        ms5611_interface_debug_print("ms5611: set addr pin failed.\n");
       
        return 1;
    }
    
    /* start read test */
    ms5611_interface_debug_print("ms5611: start read test.\n");
    
    /* init ms5611 */
    res = ms5611_init(&gs_handle);
    if (res != 0)
    {
        ms5611_interface_debug_print("ms5611: init failed.\n");
       
        return 1;
    }
    
    /* set temperature osr 4096 */
    res = ms5611_set_temperature_osr(&gs_handle, MS5611_OSR_4096);
    if (res != 0)
    {
        ms5611_interface_debug_print("ms5611: set temperature osr failed.\n");
        (void)ms5611_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set pressure osr 4096 */
    res = ms5611_set_pressure_osr(&gs_handle, MS5611_OSR_4096);
    if (res != 0)
    {
        ms5611_interface_debug_print("ms5611: set pressure osr failed.\n");
        (void)ms5611_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    ms5611_interface_debug_print("ms5611: set temperature osr 4096.\n");
    ms5611_interface_debug_print("ms5611: set pressure osr 4096.\n");
    
    for (i = 0; i < times; i++)
    {
        uint32_t temperature_raw;
        float temperature_c; 
        uint32_t pressure_raw;
        float pressure_mbar;
        
        res = ms5611_read_temperature_pressure(&gs_handle, &temperature_raw, &temperature_c, 
                                               &pressure_raw, &pressure_mbar);
        if (res != 0)
        {
            ms5611_interface_debug_print("ms5611: read temperature pressure failed.\n");
            (void)ms5611_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ms5611_interface_debug_print("ms5611: temperature is %0.2fC.\n", temperature_c);
        ms5611_interface_debug_print("ms5611: pressure is %0.2fmbar.\n", pressure_mbar);
        
        /* delay 1000ms */
        ms5611_interface_delay_ms(1000);
    }
    
    /* set temperature osr 256 */
    res = ms5611_set_temperature_osr(&gs_handle, MS5611_OSR_256);
    if (res != 0)
    {
        ms5611_interface_debug_print("ms5611: set temperature osr failed.\n");
        (void)ms5611_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set pressure osr 256 */
    res = ms5611_set_pressure_osr(&gs_handle, MS5611_OSR_256);
    if (res != 0)
    {
        ms5611_interface_debug_print("ms5611: set pressure osr failed.\n");
        (void)ms5611_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    ms5611_interface_debug_print("ms5611: set temperature osr 256.\n");
    ms5611_interface_debug_print("ms5611: set pressure osr 256.\n");
    
    for (i = 0; i < times; i++)
    {
        uint32_t temperature_raw;
        float temperature_c; 
        uint32_t pressure_raw;
        float pressure_mbar;
        
        res = ms5611_read_temperature_pressure(&gs_handle, &temperature_raw, &temperature_c, 
                                               &pressure_raw, &pressure_mbar);
        if (res != 0)
        {
            ms5611_interface_debug_print("ms5611: read temperature pressure failed.\n");
            (void)ms5611_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ms5611_interface_debug_print("ms5611: temperature is %0.2fC.\n", temperature_c);
        ms5611_interface_debug_print("ms5611: pressure is %0.2fmbar.\n", pressure_mbar);
        
        /* delay 1000ms */
        ms5611_interface_delay_ms(1000);
    }
    
    /* finish read test */
    ms5611_interface_debug_print("ms5611: finish read test.\n");
    (void)ms5611_deinit(&gs_handle);
    
    return 0;
}
