#include "Config.h"
namespace Config {
    GstClockTime offset_to_nano(guint64 offset)
    {
        return offset*1000000000/sample_rate;
    }
}
