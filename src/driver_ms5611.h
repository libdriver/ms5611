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
 * @file      driver_ms5611.h
 * @brief     driver ms5611 header file
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

#ifndef DRIVER_MS5611_H
#define DRIVER_MS5611_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup ms5611_driver ms5611 driver function
 * @brief    ms5611 driver modules
 * @{
 */

/**
 * @addtogroup ms5611_base_driver
 * @{
 */

/**
 * @brief ms5611 interface enumeration definition
 */
typedef enum
{
    MS5611_INTERFACE_IIC = 0x00,        /**< iic interface function */
    MS5611_INTERFACE_SPI = 0x01,        /**< spi interface function */
} ms5611_interface_t;

/**
 * @brief ms5611 address enumeration definition
 */
typedef enum
{
    MS5611_ADDRESS_CSB_0 = 0xEE,        /**< csb connected to the GND */
    MS5611_ADDRESS_CSB_1 = 0xEC,        /**< csb connected to the VCC */
} ms5611_address_t;

/**
 * @brief ms5611 bool enumeration definition
 */
typedef enum
{
    MS5611_BOOL_FALSE = 0x00,        /**< false */
    MS5611_BOOL_TRUE  = 0x01,        /**< true */
} ms5611_bool_t;

/**
 * @brief ms5611 osr enumeration definition
 */
typedef enum
{
    MS5611_OSR_256  = 0x00,        /**< 256 */
    MS5611_OSR_512  = 0x02,        /**< 512 */
    MS5611_OSR_1024 = 0x04,        /**< 1024 */
    MS5611_OSR_2048 = 0x06,        /**< 2048 */
    MS5611_OSR_4096 = 0x08,        /**< 4096 */
} ms5611_osr_t;

/**
 * @brief ms5611 handle structure definition
 */
typedef struct ms5611_handle_s
{
    uint8_t iic_spi;                                                                    /**< iic spi interface type */
    uint8_t iic_addr;                                                                   /**< iic address */
    uint8_t (*iic_init)(void);                                                          /**< point to an iic_init function address */
    uint8_t (*iic_deinit)(void);                                                        /**< point to an iic_deinit function address */
    uint8_t (*iic_read)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);         /**< point to an iic_read function address */
    uint8_t (*iic_write)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);        /**< point to an iic_write function address */
    uint8_t (*spi_init)(void);                                                          /**< point to a spi_init function address */
    uint8_t (*spi_deinit)(void);                                                        /**< point to a spi_deinit function address */
    uint8_t (*spi_read)(uint8_t reg, uint8_t *buf, uint16_t len);                       /**< point to a spi_read function address */
    uint8_t (*spi_write)(uint8_t reg, uint8_t *buf, uint16_t len);                      /**< point to a spi_write function address */
    void (*delay_ms)(uint32_t ms);                                                      /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                                    /**< point to a debug_print function address */
    uint8_t inited;                                                                     /**< inited flag */
    uint16_t coeff[8];                                                                  /**< eeprom coeff */
    uint8_t temperature_osr;                                                            /**< temperature osr */
    uint8_t pressure_osr;                                                               /**< pressure osr */
} ms5611_handle_t;

/**
 * @brief ms5611 information structure definition
 */
typedef struct ms5611_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} ms5611_info_t;

/**
 * @}
 */

/**
 * @defgroup ms5611_link_driver ms5611 link driver function
 * @brief    ms5611 link driver modules
 * @ingroup  ms5611_driver
 * @{
 */

/**
 * @brief     initialize ms5611_handle_t structure
 * @param[in] HANDLE points to an ms5611 handle structure
 * @param[in] STRUCTURE is ms5611_handle_t
 * @note      none
 */
#define DRIVER_MS5611_LINK_INIT(HANDLE, STRUCTURE)         memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link iic_init function
 * @param[in] HANDLE points to an ms5611 handle structure
 * @param[in] FUC points to an iic_init function address
 * @note      none
 */
#define DRIVER_MS5611_LINK_IIC_INIT(HANDLE, FUC)          (HANDLE)->iic_init = FUC

/**
 * @brief     link iic_deinit function
 * @param[in] HANDLE points to an ms5611 handle structure
 * @param[in] FUC points to an iic_deinit function address
 * @note      none
 */
#define DRIVER_MS5611_LINK_IIC_DEINIT(HANDLE, FUC)        (HANDLE)->iic_deinit = FUC

/**
 * @brief     link iic_read function
 * @param[in] HANDLE points to an ms5611 handle structure
 * @param[in] FUC points to an iic_read function address
 * @note      none
 */
#define DRIVER_MS5611_LINK_IIC_READ(HANDLE, FUC)          (HANDLE)->iic_read = FUC

/**
 * @brief     link iic_write function
 * @param[in] HANDLE points to an ms5611 handle structure
 * @param[in] FUC points to an iic_write function address
 * @note      none
 */
#define DRIVER_MS5611_LINK_IIC_WRITE(HANDLE, FUC)         (HANDLE)->iic_write = FUC

/**
 * @brief     link spi_init function
 * @param[in] HANDLE points to an ms5611 handle structure
 * @param[in] FUC points to a spi_init function address
 * @note      none
 */
#define DRIVER_MS5611_LINK_SPI_INIT(HANDLE, FUC)          (HANDLE)->spi_init = FUC

/**
 * @brief     link spi_deinit function
 * @param[in] HANDLE points to an ms5611 handle structure
 * @param[in] FUC points to a spi_deinit function address
 * @note      none
 */
#define DRIVER_MS5611_LINK_SPI_DEINIT(HANDLE, FUC)        (HANDLE)->spi_deinit = FUC

/**
 * @brief     link spi_read function
 * @param[in] HANDLE points to an ms5611 handle structure
 * @param[in] FUC points to a spi_read function address
 * @note      none
 */
#define DRIVER_MS5611_LINK_SPI_READ(HANDLE, FUC)          (HANDLE)->spi_read = FUC

/**
 * @brief     link spi_write function
 * @param[in] HANDLE points to an ms5611 handle structure
 * @param[in] FUC points to a spi_write function address
 * @note      none
 */
#define DRIVER_MS5611_LINK_SPI_WRITE(HANDLE, FUC)         (HANDLE)->spi_write = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE points to an ms5611 handle structure
 * @param[in] FUC points to a delay_ms function address
 * @note      none
 */
#define DRIVER_MS5611_LINK_DELAY_MS(HANDLE, FUC)          (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE points to an ms5611 handle structure
 * @param[in] FUC points to a debug_print function address
 * @note      none
 */
#define DRIVER_MS5611_LINK_DEBUG_PRINT(HANDLE, FUC)       (HANDLE)->debug_print = FUC

/**
 * @}
 */

/**
 * @defgroup ms5611_base_driver ms5611 base driver function
 * @brief    ms5611 base driver modules
 * @ingroup  ms5611_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info points to an ms5611 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ms5611_info(ms5611_info_t *info);

/**
 * @brief     set the chip interface
 * @param[in] *handle points to an ms5611 handle structure
 * @param[in] interface is the chip interface
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t ms5611_set_interface(ms5611_handle_t *handle, ms5611_interface_t interface);

/**
 * @brief      get the chip interface
 * @param[in]  *handle points to an ms5611 handle structure
 * @param[out] *interface points to a chip interface buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ms5611_get_interface(ms5611_handle_t *handle, ms5611_interface_t *interface);

/**
 * @brief     set the iic address pin
 * @param[in] *handle points to an ms5611 handle structure
 * @param[in] addr_pin is the address pin
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t ms5611_set_addr_pin(ms5611_handle_t *handle, ms5611_address_t addr_pin);

/**
 * @brief      get the iic address pin
 * @param[in]  *handle points to an ms5611 handle structure
 * @param[out] *addr_pin points to an address pin buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ms5611_get_addr_pin(ms5611_handle_t *handle, ms5611_address_t *addr_pin);

/**
 * @brief     set temperature osr
 * @param[in] *handle points to an ms5611 handle structure
 * @param[in] osr is the set osr
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t ms5611_set_temperature_osr(ms5611_handle_t *handle, ms5611_osr_t osr);

/**
 * @brief      get temperature osr
 * @param[in]  *handle points to an ms5611 handle structure
 * @param[out] *osr points to an osr buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ms5611_get_temperature_osr(ms5611_handle_t *handle, ms5611_osr_t *osr);

/**
 * @brief     set pressure osr
 * @param[in] *handle points to an ms5611 handle structure
 * @param[in] osr is the set osr
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t ms5611_set_pressure_osr(ms5611_handle_t *handle, ms5611_osr_t osr);

/**
 * @brief      get pressure osr
 * @param[in]  *handle points to an ms5611 handle structure
 * @param[out] *osr points to an osr buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ms5611_get_pressure_osr(ms5611_handle_t *handle, ms5611_osr_t *osr);

/**
 * @brief     initialize the chip
 * @param[in] *handle points to an ms5611 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 spi or iic initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 reset failed
 *            - 5 prom read failed
 * @note      none
 */
uint8_t ms5611_init(ms5611_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle points to an ms5611 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic or spi deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 reset failed
 * @note      none
 */
uint8_t ms5611_deinit(ms5611_handle_t *handle);

/**
 * @brief      read the temperature and pressure
 * @param[in]  *handle points to an ms5611 handle structure
 * @param[out] *temperature_raw points to a raw temperature buffer
 * @param[out] *temperature_c points to a converted temperature buffer
 * @param[out] *pressure_raw points to a raw pressure buffer
 * @param[out] *pressure_mbar points to a converted pressure buffer
 * @return     status code
 *             - 0 success
 *             - 1 read temperature pressure failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ms5611_read_temperature_pressure(ms5611_handle_t *handle, uint32_t *temperature_raw, float *temperature_c, 
                                         uint32_t *pressure_raw, float *pressure_mbar);

/**
 * @brief      read the temperature
 * @param[in]  *handle points to an ms5611 handle structure
 * @param[out] *temperature_raw points to a raw temperature buffer
 * @param[out] *temperature_c points to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 read temperature failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ms5611_read_temperature(ms5611_handle_t *handle, uint32_t *temperature_raw, float *temperature_c);

/**
 * @brief      read the pressure
 * @param[in]  *handle points to an ms5611 handle structure
 * @param[out] *pressure_raw points to a raw pressure buffer
 * @param[out] *pressure_mbar points to a converted pressure buffer
 * @return     status code
 *             - 0 success
 *             - 1 read pressure failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ms5611_read_pressure(ms5611_handle_t *handle, uint32_t *pressure_raw, float *pressure_mbar);

/**
 * @brief     reset the chip
 * @param[in] *handle points to an ms5611 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 reset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ms5611_reset(ms5611_handle_t *handle);

/**
 * @}
 */

/**
 * @defgroup ms5611_extern_driver ms5611 extern driver function
 * @brief    ms5611 extern driver modules
 * @ingroup  ms5611_driver
 * @{
 */

/**
 * @brief     set the chip register
 * @param[in] *handle points to an ms5611 handle structure
 * @param[in] reg is the register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ms5611_set_reg(ms5611_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief      get the chip register
 * @param[in]  *handle points to an ms5611 handle structure
 * @param[in]  reg is the register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ms5611_get_reg(ms5611_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
