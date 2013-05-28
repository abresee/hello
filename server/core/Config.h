#ifndef CONFIG_H
#define CONFIG_H 
#include <boost/shared_ptr.hpp>
#include <vector>
class Instrument;
typedef int16_t Sample;
typedef boost::shared_ptr<Instrument> InstrumentHandle;
typedef std::vector<Sample> Packet;
typedef boost::shared_ptr<Packet> PacketHandle;

namespace Config 
{
    const Sample max_volume = std::numeric_limits<Sample>::max();
    const int sample_rate = 44100;
    const double freq_reference = 220;

    /// player's current sample rate

    /// amount of bytes in a sample 
    static const int word_size=sizeof(Sample);

    /// amount of samples written to buffers in each go
    static const int packet_size= 512;

    /// size of each buffer created
    static const int buffer_length = packet_size*word_size;

    /// amount of channels in our audio stream
    static const int channels = 1;

    /// TODO: get rid of this stuff
    static const int seconds = 10;
    static const int signal_length = sample_rate*seconds;
}

#endif /* CONFIG_H */
