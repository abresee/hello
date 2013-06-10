#include "global.h"
namespace Config {

    //offset(samples) / sample_rate (samples/second) * 1e9 ns/second = time
    //offset(samples)   |             (seconds) | 1e9 (ns)
    //                  | sample_rate (samples) | 1 (sec)
    GstClockTime offset_to_ns(offset_t offset, offset_t sample_rate) {
        return GstClockTime(offset*1e9/static_cast<double>(sample_rate));
    }
    //position(beats) / tempo(beats/sec) * 1e9 ns/second = time
    //position(beats)   |     (sec)     | 1e9 (ns)
    //                  | tempo(beats)  | 1 (sec)
    GstClockTime position_to_ns(position_t position, position_t tempo) {
        return boost::rational_cast<GstClockTime>((position/tempo)*1e9);
    }

    //offset(samples) / (sample rate (samples/second)) * tempo(beat/second) = beats
    //offset(samples)   |             (seconds) | tempo (beats)
    //                  | sample rate (samples) |       (seconds)
    position_t offset_to_position(offset_t offset, offset_t sample_rate, position_t tempo) {
        return position_t(offset*tempo/static_cast<double>(sample_rate));
    }
    
    //time(ns) / (tempo (beats/sec) * 1e9 ns/sec) = beats
    //time(ns)  | tempo (beats) | 1   (sec)
    //          |       (sec)   | 1e9 (ns)
    position_t ns_to_position(GstClockTime time, position_t tempo) {
        return position_t(time*tempo/1e9); 
    }

    //time(ns) * (1 sec / 1e9 ns) * sample rate (sample/sec) = offset (sample)
    //time(ns)  | 1   (sec) | sample rate (sample)  
    //          | 1e9 (ns)  |             (sec)     
    offset_t ns_to_offset(GstClockTime time, offset_t sample_rate) {
        return offset_t(time*sample_rate/1e9);
    }

    //position (beats) * (1/tempo) (sec/beat) sample rate (sample / second)
    //position (beats)  |       (sec)   | sample rate (samples)
    //                  | tempo (beats) |             (second)
    offset_t position_to_offset(position_t position, position_t tempo, offset_t sample_rate) {
        return boost::rational_cast<offset_t>(position/(tempo)*sample_rate);
    }
}
