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
 * @file      driver_ms5611_basic.c
 * @brief     driver ms5611 basic source file
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

#include "driver_ms5611_basic.h"

static ms5611_handle_t gs_handle;        /**< ms5611 handle */

/**
 * @brief     basic example init
 * @param[in] interface is the chip interface
 * @param[in] addr_pin is the iic device address
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t ms5611_basic_init(ms5611_interface_t interface, ms5611_address_t addr_pin)
{
    uint8_t res;
    
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
    
    /* set the interface */
    res = ms5611_set_interface(&gs_handle, interface);
    if (res != 0)
    {
        ms5611_interface_debug_print("ms5611: set interface failed.\n");
       
        return 1;
    }
    
    /* set addr pin */
    res = ms5611_set_addr_pin(&gs_handle, addr_pin);
    if (res != 0)
    {
        ms5611_interface_debug_print("ms5611: set addr pin failed.\n");
       
        return 1;
    }
    
    /* ms5611 initialization */
    res = ms5611_init(&gs_handle);
    if (res != 0)
    {
        ms5611_interface_debug_print("ms5611: init failed.\n");
       
        return 1;
    }
    
    /* set default temperature osr */
    res = ms5611_set_temperature_osr(&gs_handle, MS5611_BASIC_DEFAULT_TEMPERATURE_OSR);
    if (res != 0)
    {
        ms5611_interface_debug_print("ms5611: set temperature osr failed.\n");
        (void)ms5611_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default pressure osr */
    res = ms5611_set_pressure_osr(&gs_handle, MS5611_BASIC_DEFAULT_PRESSURE_OSR);
    if (res != 0)
    {
        ms5611_interface_debug_print("ms5611: set pressure osr failed.\n");
        (void)ms5611_deinit(&gs_handle);
        
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic example read
 * @param[out] *temperature_c points to a converted temperature buffer
 * @param[out] *pressure_mbar points to a converted pressure buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t ms5611_basic_read(float *temperature_c, float *pressure_mbar)
{
    uint8_t res;
    uint32_t temperature_raw;
    uint32_t pressure_raw;
    
    res = ms5611_read_temperature_pressure(&gs_handle, &temperature_raw, temperature_c, 
                                           &pressure_raw, pressure_mbar);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t ms5611_basic_deinit(void)
{
    if (ms5611_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}
