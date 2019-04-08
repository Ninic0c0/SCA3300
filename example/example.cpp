/**
 * @author Nicolas SALMIN
 * @date 4 Apr 2019
 * @file example.cpp
 * @brief Simple Hello World to use SCA3300 library
 *
 */

/*============================================================================*/
/*                                  INCLUDES                                  */
/*============================================================================*/
/* ******** Includes/System ************************************************* */
#include <iostream>

/* ******** Includes/Consts ************************************************* */

/* ******** Includes/Types ************************************************** */

/* ******** Includes/Globals ************************************************ */

/* *********Includes/functions prototypes *********************************** */
/* external */

/* local */
#include "sca3300.h"

/*============================================================================*/
/*                                DEFINITIONS                                 */
/*============================================================================*/
/* ******** Definitions/Consts ********************************************** */

/* ******** Definitions/Types *********************************************** */

/* ******** Definitions/Variables ******************************************* */

/* ******** Definitions/Functions ******************************************* */

/*============================================================================*/
/*                                NAMESPACES                                  */
/*============================================================================*/
using namespace std;
using namespace sca3300d01;

int main(int argc, char** argv)
{
    /* Create sca3300 Object */
    sca3300 sca3300Chip("/dev/spidev0.0", SPI_MODE_0, 2000000, 8);

    bool ret = true;

    float temp  = 0.0;
    float Acc_X = 0.0;
    float Acc_Y = 0.0;
    float Acc_Z = 0.0;

    /* Call temperature and acceleration */
    for (int i = 0; i < 10; ++i)
    {
        /* Ask for acceleration */
        ret &= sca3300Chip.GetAccel( ACCEL_X, Acc_X );
        ret &= sca3300Chip.GetAccel( ACCEL_Y, Acc_Y );
        ret &= sca3300Chip.GetAccel( ACCEL_Z, Acc_Z );

        /* Ask for temperature */
        ret &= sca3300Chip.GetTemperature( temp );

        /* Print accelerations */
        printf (" Temperature: %f\n",temp);
        printf (" Accel_X: %f\n",Acc_X);
        printf (" Accel_Y: %f\n",Acc_Y);
        printf (" Accel_Z: %f\n",Acc_Z);

        sleep(1);
    }

    return ret;
}