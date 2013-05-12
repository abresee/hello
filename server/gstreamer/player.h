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
typedef boost::shared_ptr<Instrument> spInstrument;

typedef int16_t sample_t;
class Player {
    class static_init{
        static_init(int*,char***);
    };
    static static_init _0;

    GstElement * pipeline;
    GstElement * appsrc;
    GstElement * conv;
    GstElement * audiosink;
    GMainLoop * loop;

    std::vector<spInstrument> instruments;
    typedef std::vector<boost::shared_ptr<Instrument>>::iterator instruments_iter;
    const int amplitude = std::numeric_limits<int16_t>::max();

    int offset=0;
    guint sourceid=0;

    void static build_gst_element(GstElement*&,const char*,const char*);
    void static initialize_gst();

    static gboolean push_data_g(gpointer);
    static void need_data_g(GstElement*,guint,gpointer);
    static void enough_data_g(GstElement*,gpointer);

    gboolean push_data();
    void need_data();
    void enough_data();


public:
    static const int sample_rate = 44100;
    static const char * format;
    static const int word_size= sizeof(sample_t);
    static const int packet_size= 512;
    static const int buffer_length = packet_size*word_size;
    static const int channels = 1;
    static const int frequency = 440;
    static const int seconds = 10;
    static const int signal_length = sample_rate*seconds;
    static const int max_volume;

    Player();
    void add_instrument(spInstrument);
    void add_instrument(Instrument*);

    void play();
    void quit();
    ~Player();
};

typedef std::valarray<sample_t> packet_t;

class Instrument
{
public:
    virtual boost::shared_ptr<packet_t> get_samples(int sample_count)=0; 
    virtual ~Instrument(){}
};

class SinGenerator : public Instrument
{
    int amplitude;
    double omega;
    int total_samples;

public:
    SinGenerator(int amplitude_, double omega_) : amplitude(amplitude_), omega(omega_) {}
    SinGenerator() : amplitude(0), omega(0.0) {}
    virtual boost::shared_ptr<packet_t> get_samples(int sample_count); 
    void set_amplitude(int amplitude_){amplitude=amplitude_;}
    void set_omega(double omega_){omega=omega_;}
    void set_frequency(double frequency){omega=2*M_PI*frequency;}
};

#endif /* PLAYER_H */
