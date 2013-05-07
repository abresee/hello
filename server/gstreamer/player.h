#ifndef PLAYER_H
#define PLAYER_H 
#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include <string>
#include <valarray>
#include <boost/shared_ptr.hpp>
#include <stdint.h>
#include <gst/gst.h>
#include <gst/app/gstappsrc.h>

class Player;
class Instrument;
typedef boost::shared_ptr<Instrument> spInstrument;
typedef int16_t sample_t;

gboolean cb_push_data(gpointer);
void cb_need_data(GstElement*, guint,gpointer);
void cb_enough_data(GstElement*,gpointer);

class Player {
    GstElement * pipeline;
    GstElement * appsrc;
    GstElement * conv;
    GstElement * audiosink;
    GMainLoop * loop;

    std::vector<spInstrument> instruments;

    const int amplitude = std::numeric_limits<int16_t>::max();

    int offset=0;
    guint sourceid=0;
    friend gboolean cb_push_data(gpointer);
    friend void cb_need_data(GstElement*, guint, gpointer);
    friend void cb_enough_data(GstElement*, gpointer);

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
    Player(int *, char ***);
    Player(): Player(NULL,NULL){}
    void play();
    void quit();
    ~Player();
};

typedef std::valarray<sample_t> packet_t;

class Instrument
{
public:
    virtual boost::shared_ptr<packet_t> get_samples(int sample_count)=0; 
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

gboolean cb_push_data(gpointer);
void cb_need_data(GstElement*, guint, gpointer);
void cb_enough_data(GstElement*,gpointer);

#endif /* PLAYER_H */
