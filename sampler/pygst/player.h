extern "C" {
    #include <stdio.h>
    #include <gst/gst.h>
}

class Player{
public:
    GMainLoop* loop;
    GstBus* bus;
    guint bus_watch_id;
    GstElement* pipeline;
    GstElement* source;
    GstElement* decoder;
    GstElement* demuxer;
    GstElement* conv;
    GstElement* sink;

    Player();
    void _d_Player();
    void print_gst_version();
    void play(char* track_name);
};
