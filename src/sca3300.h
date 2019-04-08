/**
 * \class sca3300
 *
 * \brief Simple class to use SCA3300-D01.
 *
 * This class could be use to manage SCA3300 device.
 *
 * \author Nicolas SALMIN
 *
 * \version 0.1
 *
 * \date 2019/03/13 10:57:00
 *
 * Contact: nicolas.salmin@gmail.
 *
 */

#ifndef SCA3300LIB_API_H_
#define SCA3300LIB_API_H_

#include <iostream>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h> // O_RDWR...
#include <sys/ioctl.h> // SPI_IOC_WR_MODE ...
#include <linux/spi/spidev.h>

#include "sca3300def.h"

/**
 * @brief      SPI frame structure
 */
struct sca3300Frame
{
  uint16_t st_Data = 0;        /**< Data */
  uint16_t st_Crc  = 0;        /**< Cyclic Redundancy Check */
  uint8_t st_ReturnStatus = 0; /**< Return Status Code*/
  bool st_IsValid  = false;    /**< Trame is valid? */
};

/**
 * @brief      Measurement Mode
 */
enum operationMode
{
  ERR = 0,
  OPMODE1, /*!<   3g full-scale. 88 Hz 1st order low pass filter (default) */
  OPMODE2, /*!<   6g full-scale. 88 Hz 1st order low pass filter */
  OPMODE3, /*!< 1.5g full-scale. 88 Hz 1st order low pass filter */
  OPMODE4, /*!< 1.5g full-scale. 10 Hz 1st order low pass filter */
};

/**
 * @brief      Acceleration axis
 */
enum accelAxe
{
  ACCEL_X = 0,
  ACCEL_Y,
  ACCEL_Z,
};

namespace sca3300d01
{
  class sca3300
  {
      public:
          // Constructors and Destructor
          sca3300();
          sca3300(std::string devspi, unsigned char spiMode, \
                                      unsigned int  spiSpeed,\
                                      unsigned char spiBitsPerWord);
          ~sca3300();

          // Basics operations
          bool CheckChipId( void );
          bool GetStatus ( void );
          bool ChangeMode( const operationMode aMode);

          // Data processing
          bool GetAccel( const accelAxe aAxe, float &aAccel );
          bool GetTemperature( float &temp );
          bool ReadAndProcessData( const int aLoop );
          sca3300Frame SendRequest( const uint32_t aRequest );

      private:
          // SPI configuration
          unsigned char mode;
          unsigned char bitsPerWord;
          unsigned int speed;
          int spifd;

          void OpenSpiBus( const std::string devspi );
          int CloseSpiBus( void );

          // Basic device configuration
          operationMode opMode;
          int sensivity;

          bool InitChip( void );
          bool CheckRS( const uint16_t aRsCode );

  }; // end of Class

} //namespace sca3300d01

#endif //SCA3300LIB_API_H_
