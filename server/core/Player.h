#ifndef PLAYER_H
#define PLAYER_H 
#include <vector>
#include <limits>
#include <cmath>
#include <string>
#include <valarray>
#include <boost/shared_ptr.hpp>
#include <stdint.h>
#include <gst/gst.h>
#include <gst/app/gstappsrc.h>

class Instrument;

/// @brief master class to handle audio generation and playback
class Player {
public:
    /// @brief type of each individual sample
    typedef int16_t sample_t;
    typedef boost::shared_ptr<Instrument> spInstrument;
    /// @brief type of each packet
    typedef std::valarray<sample_t> packet_t;
    typedef boost::shared_ptr<packet_t> spPacket;

    Player();
    void add_instrument(spInstrument);
    void add_instrument(Instrument*);

    void play();
    void quit();
    ~Player();

private:

    /// @brief cstring for gst representing the data format (e.g. S16LE --> Signed 16 bit Little Endian
    static const char * format;

    /// @brief player's current sample rate
    static const int sample_rate = 44100;

    /// @brief amount of bytes in a sample 
    static const int word_size=sizeof(sample_t);

    /// @brief amount of samples written to buffers in each go
    static const int packet_size= 512;

    /// @brief size of each buffer created
    static const int buffer_length = packet_size*word_size;

    /// @brief amount of channels in our audio stream
    static const int channels = 1;

    /// TODO: get rid of this stuff
    static const int seconds = 10;
    static const int signal_length = sample_rate*seconds;

    /// @brief gst object representing the whole pipeline
    GstElement * pipeline;
    /// @brief gst object representing the interface between our code and the pipeline
    GstElement * appsrc;
    /// @brief gst object that converts audio data between appsrc and the sink
    GstElement * conv;
    /// @brief gst object representing the sink (stream, speakers, whatever)
    GstElement * audiosink;
    /// @brief glib object representing the mainloop used by gstreamer
    GMainLoop * loop;

    /// @brief container for the player object's instruments
    std::vector<spInstrument> instruments;
    /// @brief private typedef to more easily iterate over the instruments
    typedef std::vector<spInstrument>::iterator itInstruments;

    /// TODO: get rid of offset
    int offset=0;
    guint sourceid=0;

    /// @brief Helper function to build gst elements to centralize boilerplate error checking.
    void static build_gst_element(GstElement*&,const char*,const char*);
    /// @brief Helper function to initialize gstreamer
    void static initialize_gst();

    /// @brief wrapper for member function b/c gst can't handle member functions
    static gboolean push_data_g(gpointer);
    /// @brief wrapper for member function b/c gst can't handle member functions
    static void need_data_g(GstElement*,guint,gpointer);
    /// @brief wrapper for member function b/c gst can't handle member functions
    static void enough_data_g(GstElement*,gpointer);

    /// @brief callback that actually inserts data into appsrc
    gboolean push_data();
    /// @brief callback that handles appsrc's need-data signal 
    void need_data();
    /// @brief callback that handles appsrc's enough-data signal 
    void enough_data();


    ///Static constant representing the largest representable sample value
    static const sample_t max_volume;

};

#endif /* PLAYER_H */
