/**
 * @author Nicolas SALMIN
 * @date 4 Apr 2019
 * @file sca3300-tools.cpp
 * @brief Tools for SCA3300 device
 *
 */

/*============================================================================*/
/*                                  INCLUDES                                  */
/*============================================================================*/
/* ******** Includes/System ************************************************* */
#include <math.h>

/* *********Includes/functions prototypes *********************************** */
#include "sca3300def.h"
#include "sca3300-tools.h"

/*============================================================================*/
/*                                NAMESPACES                                  */
/*============================================================================*/
using namespace sca3300d01;


/**
 * @brief      Check if the CRC of SPI trame is valid
 *
 * @note       Calculate CRC for 24 MSB's of the 32 bit dword
 *
 * @param      ptr     The pointer
 * @param[in]  octets  The octets
 *
 * @return     Return true if CRC is valid , false otherwise
 */
bool sca3300d01::CheckCRCTrame(uint8_t *ptr, const uint8_t octets)
{
    uint8_t crc  = 0xFF;
    uint8_t data = 0x00;

    // (8 LSB's are the CRC field and are not included in CRC calculation)
    for (uint8_t i = 0; i < octets -1; i++)
    {
        data = *ptr++;

        for (uint8_t mask = 0x80; mask != 0; mask >>= 1)
        {
            uint8_t poly = 0, adder = 0;

            /* case for new bit = 1 */
            if (data & mask)
            {/* define the polynomial */
                poly = 0x1C;
                adder++;
            }

            if (crc & 0x80)
                poly ^= 0x1D;

            crc <<= 1;
            crc += adder;
            crc ^= poly;
        }
    }

    if (uint8_t(~crc) == *ptr)
        return true;
    else
        return false;
}


/**
 * @brief      Convert data from SPI trame to acceleration
 *
 * @param[in]  aAccel  A data acceleration
 *
 * @return     Acceleration converted (g.)
 */
float sca3300d01::ProcessAccel( const uint16_t aAccel, const int aSensivity )
{
    float value = (float)aAccel / aSensivity;

    return value;
}

/**
 * @brief      Converts raw data from SPI trame in degrees Celcuis (°C)
 *
 * @param[in]  aRawTemp  A raw data from SPI trame
 *
 * @return     Temperature in degrees Celcuis
 */
float sca3300d01::ConvertTemperature( const uint16_t aRawTemp )
{
    float ftemp = TEMP_ABSOLUTE_ZERO + ( aRawTemp / TEMP_SIGNAL_SENSITIVITY );

    ftemp = roundf( ftemp * 100 ) / 100;

    return ftemp;
}
