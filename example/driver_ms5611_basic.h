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
 * @file      driver_ms5611_basic.h
 * @brief     driver ms5611 basic header file
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

#ifndef DRIVER_MS5611_BASIC_H
#define DRIVER_MS5611_BASIC_H

#include "driver_ms5611_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup ms5611_example_driver ms5611 example driver function
 * @brief    ms5611 example driver modules
 * @ingroup  ms5611_driver
 * @{
 */

/**
 * @brief ms5611 basic example default definition
 */
#define MS5611_BASIC_DEFAULT_TEMPERATURE_OSR        MS5611_OSR_4096          /**< 4096 */
#define MS5611_BASIC_DEFAULT_PRESSURE_OSR           MS5611_OSR_4096          /**< 4096 */

/**
 * @brief     basic example init
 * @param[in] interface is the chip interface
 * @param[in] addr_pin is the iic device address
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t ms5611_basic_init(ms5611_interface_t interface, ms5611_address_t addr_pin);

/**
 * @brief      basic example read
 * @param[out] *temperature_c points to a converted temperature buffer
 * @param[out] *pressure_mbar points to a converted pressure buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t ms5611_basic_read(float *temperature_c, float *pressure_mbar);

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t ms5611_basic_deinit(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
