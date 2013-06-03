#ifndef CONFIG_H
#define CONFIG_H 
#include <memory>
#include <cstdint>
#include <limits>
#include <vector>
#include <boost/iterator/zip_iterator.hpp>
#include <boost/functional/hash.hpp>
#include <glib.h>
typedef int16_t Sample;
typedef int64_t offset_t;

namespace std {
    template<typename... T>
    struct hash<tuple<T...>>{
        size_t operator()(tuple<T...> const& arg) const noexcept
        {
            return boost::hash_value(arg);
        }
    };
}

    template<typename SeqA,typename SeqB> 
    class Zipper {
        SeqA& seqa_;
        SeqB& seqb_;
        Zipper(const SeqA& seqa, const SeqB& seqb):
            seqa_(seqa),seqb_(seqb) {
        }

        Zipper(SeqA& seqa, SeqB& seqb): 
            seqa_(seqa),
            seqb_(seqb) { 
            }
        
        auto begin() const {
            return boost::make_zip_iterator(
                std::make_tuple(seqa_.begin(),seqb_.begin()));
        }      
        auto end() const {
            return boost::make_zip_iterator(
                std::make_tuple(seqa_.end(),seqb_.end()));
        }
        auto begin() {
            return boost::make_zip_iterator(
                std::make_tuple(seqa_.begin(),seqb_.begin()));
        }
        auto end() {
            return boost::make_zip_iterator(
                std::make_tuple(seqa_.end(),seqb_.end()));
        }
    };

namespace Config {
    typedef int64_t GstClockTime;
    const Sample max_intensity = std::numeric_limits<Sample>::max();
    /// player's current sample rate
    const int sample_rate = 44100;
    /// reference frequency of the whole system, that is, frequency of pitch_class = 0, octave = 0
    const double freq_reference = 220;
    /// function to convert from offsets to nanoseconds
    GstClockTime offset_to_ns(offset_t offset);
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
