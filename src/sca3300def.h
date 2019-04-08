/**
 * \file  sca3300def.h
 *
 * \brief SCA3300 definition file
 *
 * \details This file contains all informations extracted from the sca3300-d01 datasheet.
 *
 * \author Nicolas SALMIN
 *
 * \version 0.1
 *
 * \date 2019/03/29 10:00:00
 *
 * Contact: nicolas.salmin@gmail.com
 *
 */

#ifndef SCA3300DEF_API_HPP_
#define SCA3300DEF_API_HPP_

#include <map>

namespace sca3300d01
{

#define SCA3300_DEV_NAME "sca3300"

#define SCA3300_MAX_SPI_FREQ_HZ  8000000
#define SCA3300_CHIP_ID           0x0051

#define TEMP_SIGNAL_SENSITIVITY    18.9
#define TEMP_ABSOLUTE_ZERO       -273.15

/**
 * @brief      SCA3300 Error Explanation
 *
 * @note datasheet p.18
 */
struct ErrorTable{
    static std::map<std::string, std::pair<int, bool> > create_map()
    {
            std::map<std::string, std::pair<int, bool> > m;
            /* Description                                                           Bit |       reset? */
            m.insert(std::make_pair("Digital block error type 1",                   std::make_pair(9, 1)));
            m.insert(std::make_pair("Digital block error type 2",                   std::make_pair(8, 1)));
            m.insert(std::make_pair("ASIC clock error",                             std::make_pair(7, 1)));
            m.insert(std::make_pair("Signal saturated in signal path",              std::make_pair(6, 0)));
            m.insert(std::make_pair("Signal saturated in temperature compensation", std::make_pair(5, 0)));
            m.insert(std::make_pair("Voltage level failure",                        std::make_pair(4, 0)));
            m.insert(std::make_pair("Memory error",                                 std::make_pair(3, 0)));
            m.insert(std::make_pair("Digital block error type 3",                   std::make_pair(2, 1)));
            m.insert(std::make_pair("Operation mode has changed",                   std::make_pair(1, 1)));
            m.insert(std::make_pair("Component internal connection error",          std::make_pair(0, 0)));
            return m;
    }
    static const std::map<std::string, std::pair<int, bool>> SCA3300_ERRORMAP;
};

/**
 * @brief      SCA3300 Status Explanation
 *
 * @note datasheet p.18
 */
struct StatusTable{
    static std::map<std::string, int> create_map()
        {
          std::map<std::string, int> m;
          m.insert(std::make_pair("Error",                     3));
          m.insert(std::make_pair("Unknown error",             2));
          m.insert(std::make_pair("Normal operation, no flag", 1));
          m.insert(std::make_pair("Startup in progress",       0));
          return m;
        }
    static const std::map<std::string, int> SCA3300_STATUSMAP;
};

/* SCA3300 Sensivity definitions */
#define SENSITIVITY_MODE_1          2700 // +/-6g   1350 LSB/g
#define SENSITIVITY_MODE_2          1350 // +/-2g   2700 LSB/g
#define SENSITIVITY_MODE_3_4        5400 // +/-1.5g 5400 LSB/g

/* SCA3300 SPI frame field masks */
#define OPCODE_FIELD_MASK     0xFC000000
#define RS_FIELD_MASK         0x03000000
#define DATA_FIELD_MASK       0x00FFFF00
#define CRC_FIELD_MASK        0x000000FF

/* SCA3300 return status */
#define ST_START_UP                 0x00
#define ST_NORMAL_OP                0x01
#define ST_ERROR                    0x11

/* Status Explanation */
#define SCA3300_ERR_DIGI1_BIT          9
#define SCA3300_ERR_DIGI2_BIT          8
#define SCA3300_ERR_CLOCK_BIT          7
#define SCA3300_ERR_STAT_BIT           6
#define SCA3300_ERR_TEMP_BIT           5
#define SCA3300_ERR_PWR_BIT            4
#define SCA3300_ERR_MEM_BIT            3
#define SCA3300_ERR_DIGI3_BIT          2
#define SCA3300_ERR_MODE_CHANGE_BIT    1
#define SCA3300_ERR_PIN_CONTINUITY_BIT 0

/* SCA3300 SPI requests */
#define REQ_READ_ACC_X        0x040000F7
#define REQ_READ_ACC_Y        0x080000FD
#define REQ_READ_ACC_Z        0x0C0000FB
#define REQ_READ_STO          0x100000E9
#define REQ_READ_TEMP         0x140000EF
#define REQ_READ_STATUS       0x180000E5
#define REQ_WRITE_SW_RESET    0xB4002098
#define REQ_WRITE_MODE1       0xB400001F
#define REQ_WRITE_MODE2       0xB4000102
#define REQ_WRITE_MODE3       0xB4000225
#define REQ_WRITE_MODE4       0xB4000338
#define REQ_READ_WHOAMI       0x40000091

}

#endif //SCA3300DEF_API_HPP_
