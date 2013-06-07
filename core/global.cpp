#include "global.h"
namespace Config {
    GstClockTime offset_to_ns(offset_t offset)
    {
        return offset*1000000000/sample_rate;
    }
}
