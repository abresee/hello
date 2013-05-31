#ifndef CONFIG_H
#define CONFIG_H 
#include <memory>
#include <cstdint>
#include <limits>
#include <vector>
#include <glib.h>
class Instrument;
typedef int16_t Sample;
typedef std::shared_ptr<Instrument> InstrumentHandle;
typedef std::vector<Sample> Packet;
typedef std::shared_ptr<Packet> PacketHandle;

namespace Config 
{
    typedef guint64 GstClockTime;
    const Sample max_intensity = std::numeric_limits<Sample>::max();
    /// player's current sample rate
    const int sample_rate = 44100;
    const double freq_reference = 220;
    GstClockTime offset_to_nano(guint64 offset);
    /// amount of bytes in a sample 
    static const int word_size=sizeof(Sample);

    /// amount of samples written to buffers in each go
    static const int packet_size= 512;

    /// size of each buffer created
    static const int buffer_length = packet_size*word_size;

    /// amount of channels in our audio stream
    static const int channels = 1;
    /// how many pitch classes are there in an octave?
    static const int pc_count = 12;
}

#endif /* CONFIG_H */
