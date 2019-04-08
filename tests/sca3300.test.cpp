#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <map>

// Let Catch provide main():
#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <sca3300.h>
#include <sca3300-tools.h>

using namespace std;
using namespace sca3300d01;

/**
 *
 * Test Temperature Data Conversion
 *
 */
TEST_CASE( "Test Temperature Data Conversion" )
{
    const std::map<uint16_t, float> TEMP_MAP
    { /* Temperatures tested in lab. */
        { 0x15C5, 21.72 },
        { 0x15A2, 19.87 },
        { 0x153C, 14.47 },
        { 0x14CD,  8.60 },
        { 0x1474,  3.89 },
        { 0x142B,  0.02 },
        { 0x13EF, -3.15 },
        { 0x13BF, -5.69 },
    };

    for (auto const&temp : TEMP_MAP)
    {
        SECTION( std::string( "Temperature Tested: " + std::to_string( temp.second )))
        {
            REQUIRE( ConvertTemperature( temp.first ) == temp.second );
            SUCCEED( "Temperatures " + std::to_string( temp.second ) + " OK" );
        }
    }
}

/**
 *
 * Cyclic Redundancy Check
 *
 */
TEST_CASE( "Cyclic Redundancy Check" )
{
    struct Trame {
        uint8_t nOP      : 8;   // [31:24] (8 bits)
        uint8_t nhiDATA  : 8;   // MSB Data  (8 bits)
        uint8_t nloDATA  : 8;   // LSB Data  (8 bits)
        uint8_t nCRC     : 8;   // [7:0]   (8 bits)
    };

    const std::array<Trame, 16> VALIDATED_TRAMES = {
    /* Read ACC_X */            Trame{0x04, 0x00, 0x00, 0xF7}, // okay
    /* Read ACC_Y */            Trame{0x08, 0x00, 0x00, 0xFD}, // okay
    /* Read ACC_Z */            Trame{0x0C, 0x00, 0x00, 0xFB}, // okay
    /* Read temp */             Trame{0x14, 0x00, 0x00, 0xEF}, // okay
    /* Change Mode 1 */         Trame{0xB4, 0x00, 0x00, 0x1F}, // okay
    /* Change Mode 2 */         Trame{0xB4, 0x00, 0x01, 0x02}, // okay
    /* Change Mode 3 */         Trame{0xB4, 0x00, 0x02, 0x25}, // okay
    /* Change Mode 4 */         Trame{0xB4, 0x00, 0x03, 0x38}, // okay
    /* R. WHOAMI*/              Trame{0x40, 0x00, 0x00, 0x91}, // okay
    /* reponse STO*/            Trame{0x11, 0x00, 0x01, 0x7B}, // okay from SCL3300 datasheet.
    /* reponse whoami*/         Trame{0x11, 0x00, 0x01, 0x7B}, // okay from SCL3300 datasheet.
    /* acc_x example*/          Trame{0x05, 0x00, 0xDC, 0x1C}, // okay from SCL3300 datasheet.
    /* request angle example */ Trame{0xB0, 0x00, 0x1F, 0x6F}, // okay from SCL3300 datasheet.
    /* angle data  example */   Trame{0x25, 0x0F, 0x88, 0x25}, // okay from SCL3300 datasheet.
    /* angle data  example */   Trame{0x15, 0x16, 0x1E, 0x0A}, // okay from SCL3300 datasheet.
    /* Whoami reg */            Trame{0x15, 0x15, 0xFD, 0x4B},
    };

    for(auto const& matrame : VALIDATED_TRAMES)
    {

        uint8_t dummytab[4] = { matrame.nOP, matrame.nhiDATA, matrame.nloDATA, matrame.nCRC };

        SECTION( std::string("Cyclic Redundancy Check: " + std::to_string( matrame.nOP )))
        {
            REQUIRE( CheckCRCTrame( dummytab, sizeof( dummytab )) == true );
            SUCCEED( "CRC: " + std::to_string( matrame.nCRC ) + " OK" );
        }
    }
}

/**
 *
 * Test Acceleration Data Conversion
 *
 */
TEST_CASE( "Test Acceleration Data Conversion" )
{
    const std::map<uint16_t, float> ACCEL_MAP
    { /* Acceleration tested in lab. */
        { 0xfb00, 11.8992 },
        { 0x1594, 1.0229  },
        { 0x0469, 0.2090  },
        { 0x0852, 0.3944  },
        { 0xff26, 12.0959 },
        { 0x0941, 0.4387  },
        { 0xfdc7, 12.0309 },
        { 0x0ab7, 0.5079  },
        { 0xfdc7, 12.0309 },
        { 0x0ab7, 0.5079  },
        { 0x1595, 1.0231  },
        { 0xfdd6, 12.0337 },
        { 0x0a84, 0.4985  },
    };

    /* Tolerance floating point values comparison */
    const float epsilon = 0.0001f;

    for (auto const&accel : ACCEL_MAP)
    {
        SECTION( std::string( "Acceleration Tested: " + std::to_string( accel.second )))
        {
            REQUIRE( fabs(ProcessAccel( accel.first, 5400 ) - accel.second) < epsilon );
            SUCCEED( "Aceleration: " + std::to_string( accel.second ) + " OK" );
        }
    }
}
