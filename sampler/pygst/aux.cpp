#include "player.h"

static gboolean bus_call(GstBus* bus, GstMessage* msg, gpointer data){
    GMainLoop *loop = (GMainLoop *) data;
    switch (GST_MESSAGE_TYPE (msg)) {
        case GST_MESSAGE_EOS:
             g_print ("End of stream\n");
             g_main_loop_quit (loop);
             break;
        case GST_MESSAGE_ERROR: {
             gchar  *debug;
             GError *error;
             gst_message_parse_error (msg, &error, &debug);
             g_free (debug);
             g_printerr ("Error: %s\n", error->message);
             g_error_free (error);
             g_main_loop_quit (loop);
             break;
        }
        default:
             break;
    }
    return TRUE;
}

static void on_pad_added (GstElement *element, GstPad *pad, gpointer data){
  GstPad *sinkpad;
  GstElement *decoder = (GstElement *) data;
  g_print ("Dynamic pad created, linking demuxer/decoder\n");
  sinkpad = gst_element_get_static_pad (decoder, "sink");
  gst_pad_link (pad, sinkpad);
  gst_object_unref (sinkpad);
}

Player::Player(){
    gst_init(NULL, NULL);
    loop = g_main_loop_new(NULL, FALSE);

    pipeline = gst_pipeline_new("audio-player");
    source   = gst_element_factory_make("filesrc", "file-source");
    demuxer  = gst_element_factory_make ("oggdemux",      "ogg-demuxer");
    decoder  = gst_element_factory_make ("vorbisdec",     "vorbis-decoder");
    conv     = gst_element_factory_make ("audioconvert",  "converter");
    sink     = gst_element_factory_make ("autoaudiosink", "audio-output");

    if (!pipeline || !source || !demuxer || !decoder || !conv || !sink) {
       g_printerr ("One element could not be created. Exiting.\n");
    }

    bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
    bus_watch_id = gst_bus_add_watch (bus, bus_call, loop);
    gst_object_unref (bus);

    gst_bin_add_many(GST_BIN(pipeline), source, demuxer, decoder, conv, sink, NULL);
    gst_element_link(source, demuxer);
    gst_element_link_many (decoder, conv, sink, NULL);
    g_signal_connect (demuxer, "pad-added", G_CALLBACK (on_pad_added), decoder); 
}

void Player::_d_Player(){
    g_print("destructor\n");
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(GST_OBJECT(pipeline));
    g_source_remove(bus_watch_id);
    g_main_loop_unref(loop);
}

void Player::print_gst_version() {
    const gchar *nano_str;
    guint major, minor, micro, nano;
    gst_version (&major, &minor, &micro, &nano);
    if (nano == 1)
        nano_str = "(CVS)";
   else if (nano == 2)
        nano_str = "(Prerelease)";
    else
        nano_str = "";
    printf ("This program is linked against GStreamer %d.%d.%d %s\n",
          major, minor, micro, nano_str);
    return;
}

void Player::play(char* track_name){
    GstState state;
    gst_element_get_state(pipeline, &state, NULL, GST_CLOCK_TIME_NONE);
    if (state == GST_STATE_PLAYING){
        g_print("pausing...");
        gst_element_set_state(pipeline, GST_STATE_PAUSED);
    }else{
        g_print("playing...");
        g_object_set(G_OBJECT(source), "location", track_name, NULL);
        gst_element_set_state(pipeline, GST_STATE_PLAYING);
    }

    g_main_loop_run(loop);
}
    
