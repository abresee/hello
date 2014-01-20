extern "C" {
    #include <stdio.h>
    #include <gst/gst.h>
}
#include <string>

class Player{
public:
    GMainLoop* loop;
    GstBus* bus;
    int count;
    guint bus_watch_id;
    GstElement* pipeline;
    GstElement* source;
    GstElement* decoder;
    GstElement* demuxer;
    GstElement* conv;
    GstElement* sink;
    GstElement* adder;

    Player();
    void _d_Player();
    void print_gst_version();
    void play(char* track_name);
    void play_sample(char* sample_name);
};
