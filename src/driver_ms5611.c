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
 * @file      driver_ms5611.c
 * @brief     driver ms5611 source file
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

#include "driver_ms5611.h"

/**
 * @brief chip register definition
 */
#define CHIP_NAME                 "TE MS5611"        /**< chip name */
#define MANUFACTURER_NAME         "TE"               /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        1.8f               /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        3.6f               /**< chip max supply voltage */
#define MAX_CURRENT               1.4f               /**< chip max current */
#define TEMPERATURE_MIN           -40.0f             /**< chip min operating temperature */
#define TEMPERATURE_MAX           85.0f              /**< chip max operating temperature */
#define DRIVER_VERSION            1000               /**< driver version */

/**
 * @brief coeff index definition
 */
#define MS5611_PRESSURE_SENSITIVITY_INDEX                      1        /**< pressure sensitivity index */
#define MS5611_PRESSURE_OFFSET_INDEX                           2        /**< pressure offset index */
#define MS5611_TEMP_COEFF_OF_PRESSURE_SENSITIVITY_INDEX        3        /**< temp coeff of pressure sensitivity index */
#define MS5611_TEMP_COEFF_OF_PRESSURE_OFFSET_INDEX             4        /**< temp coeff of pressure offset index */
#define MS5611_REFERENCE_TEMPERATURE_INDEX                     5        /**< reference temperature index */
#define MS5611_TEMP_COEFF_OF_TEMPERATURE_INDEX                 6        /**< temp coeff of temperature index */

/**
 * @brief chip command definition
 */
#define MS5611_COMMAND_RESET                      0x1E        /**< reset command */
#define MS5611_COMMAND_PRESSURE                   0x40        /**< d1 convert command */
#define MS5611_COMMAND_TEMPERATURE                0x50        /**< d2 convert command */
#define MS5611_COMMAND_ADC_READ                   0x00        /**< adc read command */
#define MS5611_COMMAND_PROM_READ_ADDRESS_0        0xA0        /**< prom read address 0 command */

/**
 * @brief      iic or spi interface read bytes
 * @param[in]  *handle points to an ms5611 handle structure
 * @param[in]  reg is the iic register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_ms5611_iic_spi_read(ms5611_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle->iic_spi == MS5611_INTERFACE_IIC)                         /* iic interface */
    {
        if (handle->iic_read(handle->iic_addr, reg, buf, len) != 0)      /* read data */
        {
            return 1;                                                    /* return error */
        }

        return 0;                                                        /* success return 0 */
    }
    else                                                                 /* spi interface */
    {
        if (handle->spi_read(reg, buf, len) != 0)                        /* read data */
        {
            return 1;                                                    /* return error */
        }

        return 0;                                                        /* success return 0 */
    }
}

/**
 * @brief     iic or spi interface write bytes
 * @param[in] *handle points to an ms5611 handle structure
 * @param[in] reg is the iic register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_ms5611_iic_spi_write(ms5611_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle->iic_spi == MS5611_INTERFACE_IIC)                          /* iic interface */
    {
        if (handle->iic_write(handle->iic_addr, reg, buf, len) != 0)      /* write data */
        {
            return 1;                                                     /* return error */
        }

        return 0;                                                         /* success return 0 */
    }
    else                                                                  /* spi interface */
    {
        if (handle->spi_write(reg, buf, len) != 0)                        /* write data */
        {
            return 1;                                                     /* return error */
        }

        return 0;                                                         /* success return 0 */
    }
}

/**
 * @brief     crc check
 * @param[in] *n_prom points to a prom buffer
 * @param[in] crc is the checked crc
 * @return    status code
 *            - 0 success
 *            - 1 failed
 * @note      none
 */
static uint8_t a_ms5611_crc_check(uint16_t *n_prom, uint8_t crc)
{
    uint8_t cnt;
    uint8_t n_bit;
    uint16_t n_rem;
    uint16_t crc_read;

    n_rem = 0x00;                                                    /* init 0 */
    crc_read = n_prom[7];                                            /* get crc */
    n_prom[7] = (0xFF00U & (n_prom[7]));                             /* init prom */
    for (cnt = 0; cnt < 16; cnt++)                                   /* loop all */
    {
        if ((cnt % 2) == 1)                                          /* check bit */
        {
            n_rem ^= (uint16_t)((n_prom[cnt >> 1]) & 0x00FF);        /* run part1 */
        }
        else
        {
            n_rem ^= (uint16_t)(n_prom[cnt >> 1] >> 8);              /* run part1 */
        }
        for (n_bit = 8; n_bit > 0; n_bit--)                          /* loop all */
        {
            if ((n_rem & 0x8000U) != 0)                              /* check bit */
            {
                n_rem = (n_rem << 1) ^ 0x3000;                       /* run part2 */
            }
            else
            {
                n_rem = (n_rem << 1);                                /* run part2 */
            }
        }
    }
    n_rem = (0x000F & (n_rem >> 12));                                /* get rem */
    n_prom[7] = crc_read;                                            /* set crc read */
    n_rem ^= 0x00;                                                   /* xor */
    if (n_rem == crc)                                                /* check crc */
    {
        return 0;                                                    /* success return 0 */
    }
    else
    {
        return 1;                                                    /* return error */
    }
}

/**
 * @brief     prom read
 * @param[in] *handle points to an ms5611 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 prom read failed
 * @note      none
 */
static uint8_t a_ms5611_prom_read(ms5611_handle_t *handle)
{
    uint8_t res;
    uint8_t i ;

    for (i = 0; i < 8; i++)                                                                            /* loop all */
    {
        uint8_t buf[2];

        res = a_ms5611_iic_spi_read(handle, MS5611_COMMAND_PROM_READ_ADDRESS_0 + i * 2, buf, 2);       /* read prom */
        if (res != 0)                                                                                  /* check result */
        {
            return 1;                                                                                  /* return error */
        }
        handle->coeff[i] = (uint16_t)((uint16_t)buf[0]) << 8 | (uint16_t)buf[1];                       /* set coeff */
    }
    if (a_ms5611_crc_check(handle->coeff, (uint8_t)(handle->coeff[7] & 0x0F)) != 0)                    /* check crc */
    {
        return 1;                                                                                      /* return error */
    }

    return 0;                                                                                          /* success return 0 */
}

/**
 * @brief     set the chip interface
 * @param[in] *handle points to an ms5611 handle structure
 * @param[in] interface is the chip interface
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t ms5611_set_interface(ms5611_handle_t *handle, ms5611_interface_t interface)
{
    if (handle == NULL)                          /* check handle */
    {
        return 2;                                /* return error */
    }

    handle->iic_spi = (uint8_t)interface;        /* set interface */

    return 0;                                    /* success return 0 */
}

/**
 * @brief      get the chip interface
 * @param[in]  *handle points to an ms5611 handle structure
 * @param[out] *interface points to a chip interface buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ms5611_get_interface(ms5611_handle_t *handle, ms5611_interface_t *interface)
{
    if (handle == NULL)                                        /* check handle */
    {
        return 2;                                              /* return error */
    }

    *interface = (ms5611_interface_t)(handle->iic_spi);        /* get interface */

    return 0;                                                  /* success return 0 */
}

/**
 * @brief     set the iic address pin
 * @param[in] *handle points to an ms5611 handle structure
 * @param[in] addr_pin is the address pin
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t ms5611_set_addr_pin(ms5611_handle_t *handle, ms5611_address_t addr_pin)
{
    if (handle == NULL)                        /* check handle */
    {
        return 2;                              /* return error */
    }

    handle->iic_addr = (uint8_t)addr_pin;      /* set pin */

    return 0;                                  /* success return 0 */
}

/**
 * @brief      get the iic address pin
 * @param[in]  *handle points to an ms5611 handle structure
 * @param[out] *addr_pin points to an address pin buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ms5611_get_addr_pin(ms5611_handle_t *handle, ms5611_address_t *addr_pin)
{
    if (handle == NULL)                                      /* check handle */
    {
        return 2;                                            /* return error */
    }

    *addr_pin = (ms5611_address_t)(handle->iic_addr);        /* get pin */

    return 0;                                                /* success return 0 */
}

/**
 * @brief     set temperature osr
 * @param[in] *handle points to an ms5611 handle structure
 * @param[in] osr is the set osr
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t ms5611_set_temperature_osr(ms5611_handle_t *handle, ms5611_osr_t osr)
{
    if (handle == NULL)                            /* check handle */
    {
        return 2;                                  /* return error */
    }

    handle->temperature_osr = (uint8_t)osr;        /* set temperature osr */

    return 0;                                      /* success return 0 */
}

/**
 * @brief      get temperature osr
 * @param[in]  *handle points to an ms5611 handle structure
 * @param[out] *osr points to an osr buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ms5611_get_temperature_osr(ms5611_handle_t *handle, ms5611_osr_t *osr)
{
    if (handle == NULL)                                    /* check handle */
    {
        return 2;                                          /* return error */
    }

    *osr = (ms5611_osr_t)(handle->temperature_osr);        /* get temperature osr */

    return 0;                                              /* success return 0 */
}

/**
 * @brief     set pressure osr
 * @param[in] *handle points to an ms5611 handle structure
 * @param[in] osr is the set osr
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t ms5611_set_pressure_osr(ms5611_handle_t *handle, ms5611_osr_t osr)
{
    if (handle == NULL)                         /* check handle */
    {
        return 2;                               /* return error */
    }

    handle->pressure_osr = (uint8_t)osr;        /* set pressure osr */

    return 0;                                   /* success return 0 */
}

/**
 * @brief      get pressure osr
 * @param[in]  *handle points to an ms5611 handle structure
 * @param[out] *osr points to an osr buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ms5611_get_pressure_osr(ms5611_handle_t *handle, ms5611_osr_t *osr)
{
    if (handle == NULL)                                 /* check handle */
    {
        return 2;                                       /* return error */
    }

    *osr = (ms5611_osr_t)(handle->pressure_osr);        /* get pressure osr */

    return 0;                                           /* success return 0 */
}

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
uint8_t ms5611_reset(ms5611_handle_t *handle)
{
    uint8_t res;

    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }

    res = a_ms5611_iic_spi_write(handle, MS5611_COMMAND_RESET, NULL, 0);        /* write config */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("ms5611: reset failed.\n");                         /* reset failed */

        return 1;                                                               /* return error */
    }
    handle->delay_ms(5);                                                        /* delay 5ms */

    return 0;                                                                   /* success return 0 */
}

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
                                         uint32_t *pressure_raw, float *pressure_mbar)
{
    uint8_t res;
    int32_t dt;
    int32_t temp;
    int64_t off;
    int64_t sens;
    int64_t p;
    int64_t t2;
    int64_t off2;
    int64_t sens2;
    uint8_t buf[3];

    if (handle == NULL)                                                                                  /* check handle */
    {
        return 2;                                                                                        /* return error */
    }
    if (handle->inited != 1)                                                                             /* check handle initialization */
    {
        return 3;                                                                                        /* return error */
    }

    res = a_ms5611_iic_spi_write(handle, MS5611_COMMAND_TEMPERATURE, NULL, 0);                           /* write config */
    if (res != 0)                                                                                        /* check result */
    {
        handle->debug_print("ms5611: start temperature failed.\n");                                      /* start temperature failed */

        return 1;                                                                                        /* return error */
    }
    if (handle->temperature_osr == MS5611_OSR_256)                                                       /* osr 256 */
    {
        handle->delay_ms(1);                                                                             /* delay 1ms */
    }
    else if (handle->temperature_osr == MS5611_OSR_512)                                                  /* osr 512 */
    {
        handle->delay_ms(2);                                                                             /* delay 2ms */
    }
    else if (handle->temperature_osr == MS5611_OSR_1024)                                                 /* osr 1024 */
    {
        handle->delay_ms(3);                                                                             /* delay 3ms */
    }
    else if (handle->temperature_osr == MS5611_OSR_2048)                                                 /* osr 2048 */
    {
        handle->delay_ms(5);                                                                             /* delay 5ms */
    }
    else                                                                                                 /* osr 4096 */
    {
        handle->delay_ms(10);                                                                            /* delay 10ms */
    }
    res = a_ms5611_iic_spi_read(handle, MS5611_COMMAND_ADC_READ, buf, 3);                                /* read adc */
    if (res != 0)                                                                                        /* check result */
    {
        handle->debug_print("ms5611: read adc failed.\n");                                               /* read adc failed */

        return 1;                                                                                        /* return error */
    }
    *temperature_raw = ((uint32_t)(buf[0]) << 16) | ((uint32_t)(buf[1]) << 8) |
                       ((uint32_t)(buf[2]) << 0);                                                        /* set temperature raw */

    res = a_ms5611_iic_spi_write(handle, MS5611_COMMAND_PRESSURE, NULL, 0);                              /* write config */
    if (res != 0)                                                                                        /* check result */
    {
        handle->debug_print("ms5611: start pressure failed.\n");                                         /* start pressure failed */

        return 1;                                                                                        /* return error */
    }
    if (handle->temperature_osr == MS5611_OSR_256)                                                       /* osr 256 */
    {
        handle->delay_ms(1);                                                                             /* delay 1ms */
    }
    else if (handle->temperature_osr == MS5611_OSR_512)                                                  /* osr 512 */
    {
        handle->delay_ms(2);                                                                             /* delay 2ms */
    }
    else if (handle->temperature_osr == MS5611_OSR_1024)                                                 /* osr 1024 */
    {
        handle->delay_ms(3);                                                                             /* delay 3ms */
    }
    else if (handle->temperature_osr == MS5611_OSR_2048)                                                 /* osr 2048 */
    {
        handle->delay_ms(5);                                                                             /* delay 5ms */
    }
    else                                                                                                 /* osr 4096 */
    {
        handle->delay_ms(10);                                                                            /* delay 10ms */
    }
    res = a_ms5611_iic_spi_read(handle, MS5611_COMMAND_ADC_READ, buf, 3);                                /* read adc */
    if (res != 0)                                                                                        /* check result */
    {
        handle->debug_print("ms5611: read adc failed.\n");                                               /* read adc failed */

        return 1;                                                                                        /* return error */
    }
    *pressure_raw = ((uint32_t)(buf[0]) << 16) | ((uint32_t)(buf[1]) << 8) |
                    ((uint32_t)(buf[2]) << 0);                                                           /* set pressure raw */

    dt = (int32_t)(*temperature_raw) -
         ((int32_t)handle->coeff[MS5611_REFERENCE_TEMPERATURE_INDEX] << 8);                              /* get dt */
    temp = 2000 + ((int64_t)dt *
          (int64_t)handle->coeff[MS5611_TEMP_COEFF_OF_TEMPERATURE_INDEX] >> 23);                         /* set temp */
    if (temp < 2000)                                                                                     /* check temp */
    {
        t2 = (3 * ((int64_t)dt * (int64_t)dt)) >> 33;                                                    /* set t2 */
        off2 = 61 * ((int64_t)temp - 2000) * ((int64_t)temp - 2000) / 16;                                /* set off2 */
        sens2 = 29 * ((int64_t)temp - 2000) * ((int64_t)temp - 2000) / 16;                               /* set sens2 */
        if (temp < -1500)                                                                                /* if < -1500 */
        {
            off2 += 17 * ((int64_t)temp + 1500) * ((int64_t)temp + 1500);                                /* set off2 */
            sens2 += 9 * ((int64_t)temp + 1500) * ((int64_t)temp + 1500);                                /* set sens2 */
        }
    }
    else
    {
        t2 = (5 * ((int64_t)dt * (int64_t)dt)) >> 38;                                                    /* set t2 */
        off2 = 0;                                                                                        /* init off2 0 */
        sens2 = 0;                                                                                       /* init sens2 0 */
    }
    off = ((int64_t)(handle->coeff[MS5611_PRESSURE_OFFSET_INDEX]) << 16) +
          (((int64_t)(handle->coeff[MS5611_TEMP_COEFF_OF_PRESSURE_OFFSET_INDEX]) * dt) >> 7);            /* set off */
    off -= off2;                                                                                         /* get off */
    sens = ((int64_t)handle->coeff[MS5611_PRESSURE_SENSITIVITY_INDEX] << 15) +
           (((int64_t)handle->coeff[MS5611_TEMP_COEFF_OF_PRESSURE_SENSITIVITY_INDEX] * dt) >> 8);        /* set sens */
    sens -= sens2;                                                                                       /* get new sens */
    p = ((((*pressure_raw) * sens) >> 21) - off) >> 15;                                                  /* get p */
    *temperature_c = ((float)temp - (float)t2) / 100.0f;                                                 /* get real temperature */
    *pressure_mbar = (float)p / 100.0f;                                                                  /* get real pressure */

    return 0;                                                                                            /* success return 0 */
}

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
uint8_t ms5611_read_temperature(ms5611_handle_t *handle, uint32_t *temperature_raw, float *temperature_c)
{
    uint8_t res;
    int32_t dt;
    int32_t temp;
    int64_t t2;
    uint8_t buf[3];

    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }

    res = a_ms5611_iic_spi_write(handle, MS5611_COMMAND_TEMPERATURE, NULL, 0);          /* write config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("ms5611: start temperature failed.\n");                     /* start temperature failed */

        return 1;                                                                       /* return error */
    }
    if (handle->temperature_osr == MS5611_OSR_256)                                      /* osr 256 */
    {
        handle->delay_ms(1);                                                            /* delay 1ms */
    }
    else if (handle->temperature_osr == MS5611_OSR_512)                                 /* osr 512 */
    {
        handle->delay_ms(2);                                                            /* delay 2ms */
    }
    else if (handle->temperature_osr == MS5611_OSR_1024)                                /* osr 1024 */
    {
        handle->delay_ms(3);                                                            /* delay 3ms */
    }
    else if (handle->temperature_osr == MS5611_OSR_2048)                                /* osr 2048 */
    {
        handle->delay_ms(5);                                                            /* delay 5ms */
    }
    else                                                                                /* osr 4096 */
    {
        handle->delay_ms(10);                                                           /* delay 10ms */
    }
    res = a_ms5611_iic_spi_read(handle, MS5611_COMMAND_ADC_READ, buf, 3);               /* read adc */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("ms5611: read adc failed.\n");                              /* read adc failed */

        return 1;                                                                       /* return error */
    }
    *temperature_raw = ((uint32_t)(buf[0]) << 16) | ((uint32_t)(buf[1]) << 8) |
                       ((uint32_t)(buf[2]) << 0);                                       /* set temperature raw */

    dt = (int32_t)(*temperature_raw) -
         ((int32_t)handle->coeff[MS5611_REFERENCE_TEMPERATURE_INDEX] << 8);             /* get dt */
    temp = 2000 + ((int64_t)dt *
          (int64_t)handle->coeff[MS5611_TEMP_COEFF_OF_TEMPERATURE_INDEX] >> 23);        /* set temp */
    if (temp < 2000)                                                                    /* check temp */
    {
        t2 = (3 * ((int64_t)dt * (int64_t)dt)) >> 33;                                   /* set t2 */
    }
    else
    {
        t2 = (5 * ((int64_t)dt * (int64_t)dt)) >> 38;                                   /* set t2 */
    }
    *temperature_c = ((float)temp - (float)t2) / 100.0f;                                /* get real temperature */

    return 0;                                                                           /* success return 0 */
}

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
uint8_t ms5611_read_pressure(ms5611_handle_t *handle, uint32_t *pressure_raw, float *pressure_mbar)
{
    uint8_t res;
    int32_t dt;
    int32_t temp;
    uint32_t temperature_raw;
    int64_t off;
    int64_t sens;
    int64_t p;
    int64_t off2;
    int64_t sens2;
    uint8_t buf[3];

    if (handle == NULL)                                                                                  /* check handle */
    {
        return 2;                                                                                        /* return error */
    }
    if (handle->inited != 1)                                                                             /* check handle initialization */
    {
        return 3;                                                                                        /* return error */
    }

    res = a_ms5611_iic_spi_write(handle, MS5611_COMMAND_TEMPERATURE, NULL, 0);                           /* write config */
    if (res != 0)                                                                                        /* check result */
    {
        handle->debug_print("ms5611: start temperature failed.\n");                                      /* start temperature failed */

        return 1;                                                                                        /* return error */
    }
    if (handle->temperature_osr == MS5611_OSR_256)                                                       /* osr 256 */
    {
        handle->delay_ms(1);                                                                             /* delay 1ms */
    }
    else if (handle->temperature_osr == MS5611_OSR_512)                                                  /* osr 512 */
    {
        handle->delay_ms(2);                                                                             /* delay 2ms */
    }
    else if (handle->temperature_osr == MS5611_OSR_1024)                                                 /* osr 1024 */
    {
        handle->delay_ms(3);                                                                             /* delay 3ms */
    }
    else if (handle->temperature_osr == MS5611_OSR_2048)                                                 /* osr 2048 */
    {
        handle->delay_ms(5);                                                                             /* delay 5ms */
    }
    else                                                                                                 /* osr 4096 */
    {
        handle->delay_ms(10);                                                                            /* delay 10ms */
    }
    res = a_ms5611_iic_spi_read(handle, MS5611_COMMAND_ADC_READ, buf, 3);                                /* read adc */
    if (res != 0)                                                                                        /* check result */
    {
        handle->debug_print("ms5611: read adc failed.\n");                                               /* read adc failed */

        return 1;                                                                                        /* return error */
    }
    temperature_raw = ((uint32_t)(buf[0]) << 16) | ((uint32_t)(buf[1]) << 8) |
                      ((uint32_t)(buf[2]) << 0);                                                         /* set temperature raw */

    res = a_ms5611_iic_spi_write(handle, MS5611_COMMAND_PRESSURE, NULL, 0);                              /* write config */
    if (res != 0)                                                                                        /* check result */
    {
        handle->debug_print("ms5611: start pressure failed.\n");                                         /* start pressure failed */

        return 1;                                                                                        /* return error */
    }
    if (handle->temperature_osr == MS5611_OSR_256)                                                       /* osr 256 */
    {
        handle->delay_ms(1);                                                                             /* delay 1ms */
    }
    else if (handle->temperature_osr == MS5611_OSR_512)                                                  /* osr 512 */
    {
        handle->delay_ms(2);                                                                             /* delay 2ms */
    }
    else if (handle->temperature_osr == MS5611_OSR_1024)                                                 /* osr 1024 */
    {
        handle->delay_ms(3);                                                                             /* delay 3ms */
    }
    else if (handle->temperature_osr == MS5611_OSR_2048)                                                 /* osr 2048 */
    {
        handle->delay_ms(5);                                                                             /* delay 5ms */
    }
    else                                                                                                 /* osr 4096 */
    {
        handle->delay_ms(10);                                                                            /* delay 10ms */
    }
    res = a_ms5611_iic_spi_read(handle, MS5611_COMMAND_ADC_READ, buf, 3);                                /* read adc */
    if (res != 0)                                                                                        /* check result */
    {
        handle->debug_print("ms5611: read adc failed.\n");                                               /* read adc failed */

        return 1;                                                                                        /* return error */
    }
    *pressure_raw = ((uint32_t)(buf[0]) << 16) | ((uint32_t)(buf[1]) << 8) |
                    ((uint32_t)(buf[2]) << 0);                                                           /* set pressure raw */

    dt = (int32_t)(temperature_raw) -
         ((int32_t)handle->coeff[MS5611_REFERENCE_TEMPERATURE_INDEX] << 8);                              /* get dt */
    temp = 2000 + ((int64_t)dt *
          (int64_t)handle->coeff[MS5611_TEMP_COEFF_OF_TEMPERATURE_INDEX] >> 23);                         /* set temp */
    if (temp < 2000)                                                                                     /* check temp */
    {
        off2 = 61 * ((int64_t)temp - 2000) * ((int64_t)temp - 2000) / 16;                                /* set off2 */
        sens2 = 29 * ((int64_t)temp - 2000) * ((int64_t)temp - 2000) / 16;                               /* set sens2 */
        if (temp < -1500)                                                                                /* if < -1500 */
        {
            off2 += 17 * ((int64_t)temp + 1500) * ((int64_t)temp + 1500);                                /* set off2 */
            sens2 += 9 * ((int64_t)temp + 1500) * ((int64_t)temp + 1500);                                /* set sens2 */
        }
    }
    else
    {
        off2 = 0;                                                                                        /* init off2 0 */
        sens2 = 0;                                                                                       /* init sens2 0 */
    }
    off = ((int64_t)(handle->coeff[MS5611_PRESSURE_OFFSET_INDEX]) << 16) +
          (((int64_t)(handle->coeff[MS5611_TEMP_COEFF_OF_PRESSURE_OFFSET_INDEX]) * dt) >> 7);            /* set off */
    off -= off2;                                                                                         /* get off */
    sens = ((int64_t)handle->coeff[MS5611_PRESSURE_SENSITIVITY_INDEX] << 15) +
           (((int64_t)handle->coeff[MS5611_TEMP_COEFF_OF_PRESSURE_SENSITIVITY_INDEX] * dt) >> 8);        /* set sens */
    sens -= sens2;                                                                                       /* get new sens */
    p = ((((*pressure_raw) * sens) >> 21) - off) >> 15;                                                  /* get p */
    *pressure_mbar = (float)p / 100.0f;                                                                  /* get real pressure */

    return 0;                                                                                            /* success return 0 */
}

/**
 * @brief     close the chip
 * @param[in] *handle points to an ms5611 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 close failed
 * @note      none
 */
static uint8_t a_ms5611_close(ms5611_handle_t *handle)
{
    if (handle->iic_spi == MS5611_INTERFACE_IIC)                        /* iic interface */
    {
        if (handle->iic_deinit() != 0)                                  /* iic deinit */
        {
            handle->debug_print("ms5611: iic deinit failed.\n");        /* iic deinit failed */

            return 1;                                                   /* return error */
        }
        else
        {
            return 0;                                                   /* success return 0 */
        }
    }
    else                                                                /* spi interface */
    {
        if (handle->spi_deinit() != 0)                                  /* spi deinit */
        {
            handle->debug_print("ms5611: spi deinit failed.\n");        /* spi deinit failed */

            return 1;                                                   /* return error */
        }
        else
        {
            return 0;                                                   /* success return 0 */
        }
    }
}

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
uint8_t ms5611_init(ms5611_handle_t *handle)
{
    uint8_t res;

    if (handle == NULL)                                               /* check handle */
    {
        return 2;                                                     /* return error */
    }
    if (handle->debug_print == NULL)                                  /* check debug_print */
    {
        return 3;                                                     /* return error */
    }
    if (handle->iic_init == NULL)                                     /* check iic_init */
    {
        handle->debug_print("ms5611: iic_init is null.\n");           /* iic_init is null */

        return 3;                                                     /* return error */
    }
    if (handle->iic_deinit == NULL)                                   /* check iic_deinit */
    {
        handle->debug_print("ms5611: iic_deinit is null.\n");         /* iic_deinit is null */

        return 3;                                                     /* return error */
    }
    if (handle->iic_read == NULL)                                     /* check iic_read */
    {
        handle->debug_print("ms5611: iic_read is null.\n");           /* iic_read is null */

        return 3;                                                     /* return error */
    }
    if (handle->iic_write == NULL)                                    /* check iic_write */
    {
        handle->debug_print("ms5611: iic_write is null.\n");          /* iic_write is null */

        return 3;                                                     /* return error */
    }
    if (handle->spi_init == NULL)                                     /* check spi_init */
    {
        handle->debug_print("ms5611: spi_init is null.\n");           /* spi_init is null */

        return 3;                                                     /* return error */
    }
    if (handle->spi_deinit == NULL)                                   /* check spi_deinit */
    {
        handle->debug_print("ms5611: spi_deinit is null.\n");         /* spi_deinit is null */

        return 3;                                                     /* return error */
    }
    if (handle->spi_read == NULL)                                     /* check spi_read */
    {
        handle->debug_print("ms5611: spi_read is null.\n");           /* spi_read is null */

        return 3;                                                     /* return error */
    }
    if (handle->spi_write == NULL)                                    /* check spi_write */
    {
        handle->debug_print("ms5611: spi_write is null.\n");          /* spi_write is null */

        return 3;                                                     /* return error */
    }
    if (handle->delay_ms == NULL)                                     /* check delay_ms */
    {
        handle->debug_print("ms5611: delay_ms is null.\n");           /* delay_ms is null */

        return 3;                                                     /* return error */
    }

    if (handle->iic_spi == MS5611_INTERFACE_IIC)                      /* iic interface */
    {
        if (handle->iic_init() != 0)                                  /* initialize iic bus */
        {
            handle->debug_print("ms5611: iic init failed.\n");        /* iic init failed */

            return 1;                                                 /* return error */
        }
    }
    else                                                              /* spi interface */
    {
        if (handle->spi_init() != 0)                                  /* initialize spi bus */
        {
            handle->debug_print("ms5611: spi init failed.\n");        /* spi init failed */

            return 1;                                                 /* return error */
        }
    }
    res = a_ms5611_iic_spi_write(handle, MS5611_COMMAND_RESET,
                                 NULL, 0);                            /* write config */
    if (res != 0)                                                     /* check result */
    {
        handle->debug_print("ms5611: reset failed.\n");               /* reset failed */

        return 4;                                                     /* return error */
    }
    handle->delay_ms(5);                                              /* delay 5ms */
    res = a_ms5611_prom_read(handle);                                 /* prom read */
    if (res != 0)                                                     /* check result */
    {
        handle->debug_print("ms5611: prom read failed.\n");           /* prom read failed */

        return 5;                                                     /* return error */
    }
    handle->inited = 1;                                               /* flag finish initialization */

    return 0;                                                         /* success return 0 */
}

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
uint8_t ms5611_deinit(ms5611_handle_t *handle)
{
    uint8_t res;

    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }

    res = a_ms5611_iic_spi_write(handle, MS5611_COMMAND_RESET, NULL, 0);        /* write config */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("ms5611: reset failed.\n");                         /* reset failed */

        return 4;                                                               /* return error */
    }
    handle->delay_ms(5);                                                        /* delay 5ms */
    res = a_ms5611_close(handle);                                               /* close */
    if (res != 0)                                                               /* check result */
    {
        return 1;                                                               /* return error */
    }
    else
    {
        handle->inited = 0;                                                     /* flag close */

        return 0;                                                               /* success return 0 */
    }
}

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
uint8_t ms5611_set_reg(ms5611_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                          /* check handle */
    {
        return 2;                                                /* return error */
    }
    if (handle->inited != 1)                                     /* check handle initialization */
    {
        return 3;                                                /* return error */
    }

    return a_ms5611_iic_spi_write(handle, reg, buf, len);       /* write data */
}

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
uint8_t ms5611_get_reg(ms5611_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                         /* check handle */
    {
        return 2;                                               /* return error */
    }
    if (handle->inited != 1)                                    /* check handle initialization */
    {
        return 3;                                               /* return error */
    }

    return a_ms5611_iic_spi_read(handle, reg, buf, len);       /* read data */
}

/**
 * @brief      get chip's information
 * @param[out] *info points to an ms5611 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ms5611_info(ms5611_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }

    memset(info, 0, sizeof(ms5611_info_t));                         /* initialize ms5611 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "IIC SPI", 8);                         /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */

    return 0;                                                       /* success return 0 */
}
