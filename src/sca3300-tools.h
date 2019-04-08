#ifndef SCA3300_TOOLS_H_
#define SCA3300_TOOLS_H_

#include <stdint.h>

namespace sca3300d01
{
    bool CheckCRCTrame( uint8_t *ptr, const uint8_t octets );
    float ProcessAccel( const uint16_t aAccel, const int aSensivity );
    float ConvertTemperature( const uint16_t aRawTemp );
}

#endif //SCA3300_TOOLS_H_
