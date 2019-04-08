/*! \mainpage SCA300-D01 library Documentation
 *
 * \section intro_sec Introduction
 *
 * This library provides the minimal support to communicate with Murata SCA3300-D01 3 axis MEMS accelerometer.
 *
 * Datasheet available here: DataSheet: https://www.murata.com/~/media/webrenewal/products/sensor/pdf/datasheet/datasheet_sca3300-d01.ashx
 *
 * \section install_sec SCA3300-D01
 *
 *  Please take a look inside the top-level README.md
 *
 * \subsection running Running the program
 *  Please take a look (again) inside the README.md
 *
 * \section copyright Copyright and License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 */


/*============================================================================*/
/*                                  INCLUDES                                  */
/*============================================================================*/
/* ******** Includes/System ************************************************* */
#include <cstring> // memset...
#include <iomanip> // setprecision...

/* *********Includes/functions prototypes *********************************** */
#include "sca3300.h"
#include "sca3300def.h"
#include "sca3300-tools.h"

#include "macrologger.h"

/*============================================================================*/
/*                                DEFINITIONS                                 */
/*============================================================================*/
/* ******** Definitions/Consts ********************************************** */
/* Error and status maps */
const std::map<std::string, std::pair<int, bool>> \
                        sca3300d01::ErrorTable::SCA3300_ERRORMAP = ErrorTable::create_map();

const std::map<std::string, int> \
                        sca3300d01::StatusTable::SCA3300_STATUSMAP = StatusTable::create_map();

/* ******** Definitions/Functions ******************************************* */
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

#define LOG_LEVEL INFO_LEVEL

/*============================================================================*/
/*                                NAMESPACES                                  */
/*============================================================================*/
using namespace std;
using namespace sca3300d01;


/**
 * @brief   Default constructor.
 * @detail  No param needed
 */
sca3300::sca3300(){
    this->mode        = SPI_MODE_0;
    this->bitsPerWord = 8;
    this->speed       = SCA3300_MAX_SPI_FREQ_HZ;
    this->spifd       = -1;

    this->OpenSpiBus(std::string("/dev/spidev0.0"));
    this->InitChip();
}


/**
 * @brief   Overloaded constructor.
 * @detail  No param needed
 *
 *
 * @param[in]   devspi  { SPI Device }
 * @param[in]   spiMode { SPI Mode }
 * @param[in]   spiSpeed    { SPI Max Speed }
 * @param[in]   spibitsPerWord  { SPI Bits per word }
 */
sca3300::sca3300(std::string devspi, unsigned char spiMode, unsigned int spiSpeed, unsigned char spibitsPerWord){
    this->mode        = spiMode ;
    this->bitsPerWord = spibitsPerWord;
    this->speed       = spiSpeed;
    this->spifd       = -1;

    this->OpenSpiBus(devspi);
    this->InitChip();
}


/**
 * @brief    Default destructor of sca3300.
 */
sca3300::~sca3300(){
    this->CloseSpiBus();
}


/**
 *
 * @brief      { Function is called by the constructor.
 * It is responsible for opening the spidev device "devspi" and then setting up the spidev interface.
 * Private member variables are used to configure spidev. }
 *
 *@note         { They must be set appropriately by constructor before calling this function.}
 *
 * @param[in] SPI Device
 *
 */
void sca3300::OpenSpiBus(std::string devspi)
{
    int iStatus = -1;

    this->spifd = open(devspi.c_str(), O_RDWR);

    if(this->spifd < 0){
        LOG_ERROR("could not open SPI device");
        exit(1);
    }

    iStatus = ioctl (this->spifd, SPI_IOC_WR_MODE, &(this->mode));
    if(iStatus < 0){
        LOG_ERROR("Could not set SPIMode (WR)...ioctl fail");
        exit(1);
    }

    iStatus = ioctl (this->spifd, SPI_IOC_RD_MODE, &(this->mode));
    if(iStatus < 0) {
      LOG_ERROR("Could not set SPIMode (RD)...ioctl fail");
      exit(1);
    }

    iStatus = ioctl (this->spifd, SPI_IOC_WR_BITS_PER_WORD, &(this->bitsPerWord));
    if(iStatus < 0) {
      LOG_ERROR("Could not set SPI bitsPerWord (WR)...ioctl fail");
      exit(1);
    }

    iStatus = ioctl (this->spifd, SPI_IOC_RD_BITS_PER_WORD, &(this->bitsPerWord));
    if(iStatus < 0) {
      LOG_ERROR("Could not set SPI bitsPerWord(RD)...ioctl fail");
      exit(1);
    }

    iStatus = ioctl (this->spifd, SPI_IOC_WR_MAX_SPEED_HZ, &(this->speed));
    if(iStatus < 0) {
      LOG_ERROR("Could not set SPI speed (WR)...ioctl fail");
      exit(1);
    }

    iStatus = ioctl (this->spifd, SPI_IOC_RD_MAX_SPEED_HZ, &(this->speed));
    if(iStatus < 0) {
      LOG_ERROR("Could not set SPI speed (RD)...ioctl fail");
      exit(1);
    }
}


/**
 * @brief      CloseSpiBus(): Responsible for closing the spidev interface.
 *
 * @note     Called in destructor.
 */
int sca3300::CloseSpiBus()
{
    int iStatus = -1;
    iStatus = close(this->spifd);

    if(iStatus < 0)
    {
        LOG_ERROR("Could not close SPI device");
        exit(1);
    }

    return iStatus;
}


/**
 * @brief      Check Return Code Status of SPI trame
 *
 * @param[in]  aRsCode  A Return Status code
 *
 * @return     True if the RS code is valid, else false
 */
bool sca3300::CheckRS( const uint16_t aRsCode )
{
    if (ST_START_UP == aRsCode || ST_NORMAL_OP == aRsCode )
        return true;
    else
        return false;
}


/**
 * @brief      This function sends data to the spidev device.
 *
 * @param[in]  aRequest  A request
 *
 * @return     Filled sca3300Frame structure
 */
sca3300Frame sca3300::SendRequest( const uint32_t aRequest ){

    int ret;
    const int REQUEST_SIZE_BYTES = 4; // u8*4 = 32bits

    uint8_t arryBytes[REQUEST_SIZE_BYTES];
    uint8_t rx[REQUEST_SIZE_BYTES];
    uint32_t response = 0x00;

    sca3300Frame cframe;

    // LSB first datasheet p.9/21
    arryBytes[3] = (unsigned char) ((aRequest) & 0xFF);
    arryBytes[2] = (unsigned char) ((aRequest >>  8) & 0xFF);
    arryBytes[1] = (unsigned char) ((aRequest >> 16) & 0xFF);
    arryBytes[0] = (unsigned char) ((aRequest >> 24));

    struct spi_ioc_transfer tr;
    memset(&tr, 0, sizeof (tr));
    tr.tx_buf = (unsigned long)arryBytes,
    tr.rx_buf = (unsigned long)rx,
    tr.len = ARRAY_SIZE(arryBytes),
    tr.delay_usecs = 0,
    tr.speed_hz = this->speed,
    tr.bits_per_word = this->bitsPerWord,
    tr.cs_change = 0;

    ret = ioctl(this->spifd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1)
        LOG_ERROR("can't send spi message");

    response = ((rx[3] & 0xFF) | ((rx[2] << 8) & 0xFF00) | ((rx[1] << 16) & 0xFF0000) | (rx[0] << 24));

    /* Check trame validity = CRC + Return Status */
    cframe.st_ReturnStatus = ( response & RS_FIELD_MASK ) >> 24 ;
    cframe.st_IsValid = sca3300d01::CheckCRCTrame(rx, sizeof(rx) ) && CheckRS( cframe.st_ReturnStatus );
    cframe.st_Data = ( response & DATA_FIELD_MASK ) >> 8;
    cframe.st_Crc = response & CRC_FIELD_MASK;

    LOG_DEBUG("Frame : 0x%08x\n", response);
    LOG_DEBUG("response validity: %d\n", cframe.st_IsValid);
    LOG_DEBUG("response Status:  0x%02x\n", cframe.st_ReturnStatus);
    LOG_DEBUG("response Data:    0x%04x\n", cframe.st_Data);
    LOG_DEBUG("response CRC:     0x%02x\n", cframe.st_Crc);

    usleep(1000);

    return cframe;
}


/**
 * @brief      { This function send the SCA3300 init sequence }
 *
 * @return     { true if the device is Successfully configured }
 * @return     { 0  Otherwise}
 */
bool sca3300::InitChip( void )
{/* Sensor Power Up sequence details in datasheet p.15 */

    // Wait 10 ms
    usleep(10000);

    //Set Mode
    this->SendRequest ( REQ_WRITE_MODE3 );
    if ( true == ChangeMode ( OPMODE3 ) )
        LOG_INFO("[OK] Change Mode done.\n");

    //Wait 5 ms
    usleep(5000);

    // Read status register twice to clear the error flags from power up
    this->SendRequest ( REQ_READ_STATUS );
    this->SendRequest ( REQ_READ_STATUS );

    // Read Status register again to get the correct
    //   status data from the off-frame protocol
    this->SendRequest ( REQ_READ_STATUS );
    this->SendRequest ( REQ_READ_STATUS );

    //Wait 5 ms
    usleep(5000);

    bool ret = this->CheckChipId();

    // First read temp once to get into desired
    //  measurement cycle in off-frame protocol
    this->SendRequest( REQ_READ_TEMP );

    return ret;
}


/**
 * @brief      Gets the temperature.
 *
 * @param      temp  The temperature
 *
 * @return     1 if the data temperature returns seems correct.
 */
bool sca3300::GetTemperature( float &temp )
{
    bool ret = false;

    sca3300Frame dummy = this->SendRequest ( REQ_READ_TEMP );
    dummy = this->SendRequest ( REQ_READ_TEMP );
    dummy = this->SendRequest ( REQ_READ_TEMP );

    if ( true == dummy.st_IsValid && ST_ERROR != dummy.st_ReturnStatus)
    {/* Temperature equation in datasheet p.4 */
        temp = ConvertTemperature( dummy.st_Data );
        ret  = true;
        LOG_DEBUG("Temperature: %lf°C", temp);
    }
    else
    {
        temp = 0.0;
        ret  = false;
        LOG_ERROR("Error during temperature acquisition.");
    }

    return ret;
}


/**
 * @brief      This function check the SCA3300 WHOAMI register validity.
 *
 * @return     1 if it's a real SCA3300
 */
bool sca3300::CheckChipId( void )
{
    sca3300Frame dummy;

    // Read again to get the correct status data
    this->SendRequest ( REQ_READ_WHOAMI );
    dummy = this->SendRequest ( REQ_READ_WHOAMI );

    if ( true == dummy.st_IsValid && SCA3300_CHIP_ID == dummy.st_Data )
    {
        LOG_INFO("SCA3300 Getting probed successfully.");
        /* to avoid first bad measurement */
        this->SendRequest ( REQ_READ_TEMP );
        return true;
    }
    else
    {
        LOG_ERROR("Failed to probe device!");
        return false;
    }
}

/**
 * @brief      Change measurement mode
 *
 * @param[in]  aMode  A mode
 *
 * @return     1 if mode is set, 0 for default mode
 */
bool sca3300::ChangeMode( const operationMode aMode)
{/* Return false in defautl mode */
    bool ret = true;

    /* Change mode */
    this->opMode = aMode;

    /* Update sensivity to get accel */
    switch(aMode)
    {
    case OPMODE1:
        this->sensivity = SENSITIVITY_MODE_1;
        break;

    case OPMODE2:
        this->sensivity = SENSITIVITY_MODE_2;
        break;

    case OPMODE3:
    case OPMODE4:
        this->sensivity = SENSITIVITY_MODE_3_4;
        break;

    default:
        LOG_INFO("Set default sensivity mode.");
        this->sensivity = SENSITIVITY_MODE_1;
        this->opMode = OPMODE1;
        ret = false;
        break;
   }

   return ret;
}


/**
 * @brief      Read given acceleration from the device
 *
 * @param[in]  aAxe    X,Y,Z axis request
 * @param      aAccel  A acceleration
 *
 * @return     1 sucess | 0 otherwise.
 */
bool sca3300::GetAccel( const accelAxe aAxe, float &aAccel )
{
    bool     ret = true;
    uint32_t req = 0x00;

    switch(aAxe)
    {
    case ACCEL_X: req = REQ_READ_ACC_X; break;
    case ACCEL_Y: req = REQ_READ_ACC_Y; break;
    case ACCEL_Z: req = REQ_READ_ACC_Z; break;

    default:
        LOG_ERROR("Invalid axe asked. Please Check.");
        ret = false;
        break;
    }

    if ( ret )
    {
        sca3300Frame dummy = this->SendRequest ( req );

        if ( true == dummy.st_IsValid )
        {
            aAccel = ProcessAccel( dummy.st_Data, this->sensivity );
            LOG_INFO("Accel[%d]: %fg\n", aAxe, aAccel);
            ret = true;
        }
        else
            LOG_ERROR("Invalid response.");
    }

    return ret;
}

/**
 * @brief      Reads and process data.
 *
 * @param[in]  aLoop  A loop
 *
 * @return     1 if success 0 otherwise
 */
bool sca3300::ReadAndProcessData( int aLoop )
{
    // Read temperature and accelerations.
    // Note: interleaved reading due to off-frame protocol
    sca3300Frame Response_Temp;

    float Curr_T = 0.0;

    float Sum_T = 0.0;
    float Sum_X = 0.0;
    float Sum_Y = 0.0;
    float Sum_Z = 0.0;

    float Acc_curr = 0.0;

    for (int i = 0; i < aLoop; ++i)
    {/* Ask sensor and check validity */

        Curr_T = 0.0;

        // Handle temperature data
        if ( true == GetTemperature(Curr_T) )
            Sum_T += Curr_T;

        if ( true == GetAccel ( ACCEL_X, Acc_curr ) )
            Sum_X += Acc_curr;

        if ( true == GetAccel ( ACCEL_Y, Acc_curr ) )
            Sum_Y += Acc_curr;

        if ( true == GetAccel ( ACCEL_Z, Acc_curr ) )
            Sum_Z += Acc_curr;

        sleep(1); // TODO check minimal time needed between each cmd
    }

    LOG_DEBUG("Processing Done.");
    LOG_DEBUG("Acc_X: %f", Sum_X/aLoop);
    LOG_DEBUG("Acc_Y: %f", Sum_Y/aLoop);
    LOG_DEBUG("Acc_Z: %f", Sum_Z/aLoop);
    LOG_DEBUG("Temp : %f", Sum_T/aLoop);

    // TODO return ret &= ret
    return true;
}


int GetError ( uint16_t aData )
{
    unsigned   i = 1;
    unsigned pos = 1;

    while (!(i & aData))
    {
        i = i << 1;
        ++pos;
    }

    return pos;
}

/**
 * @brief      Gets the device status register.
 *
 * @return     1 as success and 0 if trouble appeared.
 */
bool sca3300::GetStatus ( void )
{
    int err = 0;

    sca3300Frame dummy = this->SendRequest ( REQ_READ_STATUS );

    if ( ST_ERROR == dummy.st_ReturnStatus )
    {
        err = GetError( dummy.st_Data );

        for (auto& t : ErrorTable::SCA3300_ERRORMAP)
        {
            if ( err == t.second.first)
            {
                LOG_ERROR("[ERRO] Error Id: %d", t.second.first  );
                LOG_ERROR("[ERRO] Message : %s", t.first.c_str() );
                LOG_ERROR("[ERRO] Reboot? : %d", t.second.second );
            }
        }
        return false;
    }
    else
    {
        LOG_INFO("Status register is correct. All right!");
        return true;
    }
}
