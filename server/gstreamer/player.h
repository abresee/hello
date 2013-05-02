#ifndef PLAYER_H
#define PLAYER_H 
#include <iostream>
#include <limits>
#include <cmath>
#include <stdint.h>
#include <gst/gst.h>
#include <gst/app/gstappsrc.h>

class Player;
gboolean cb_push_data(gpointer);
void cb_need_data(GstElement*, guint,gpointer);
void cb_enough_data(GstElement*,gpointer);

class Player {
    GstElement * pipeline;
    GstElement * appsrc;
    GstElement * conv;
    GstElement * audiosink;
    GMainLoop * loop;
    static const int sample_rate = 44100;
    static const char * format;
    static const int width = 16;
    static const int depth = 16;
    static const int word_size = 2;
    static const int packet_count = 512;
    static const int buffer_length = packet_count*word_size;
    static const int channels = 1;
    static const int frequency = 440;
    static const int seconds = 10;
    static const int signal_length = sample_rate*seconds;
    const int amplitude = std::numeric_limits<int16_t>::max();
    int16_t data[signal_length];
    int offset=0;
    guint sourceid=0;
    void initdata();
    friend gboolean cb_push_data(gpointer);
    friend void cb_need_data(GstElement*, guint, gpointer);
    friend void cb_enough_data(GstElement*, gpointer);

public:
    Player(int *, char ***);
    Player(): Player(NULL,NULL){}
    void play();
    void quit();
    ~Player();
};
gboolean cb_push_data(gpointer);
void cb_need_data(GstElement*, guint, gpointer);
void cb_enough_data(GstElement*,gpointer);
#endif /* PLAYER_H */
