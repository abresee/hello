#include "player.h"

static gboolean bus_call(GstBus* bus, GstMessage* msg, gpointer data){
    Player* my_player = (Player*) data;
    switch (GST_MESSAGE_TYPE (msg)) {
        case GST_MESSAGE_EOS:{
             g_print ("End of stream\n");
             my_player->pipeline = NULL;
             my_player->pipeline = gst_pipeline_new("audio-player");
             my_player->sink     = gst_element_factory_make ("autoaudiosink", "audio-output");
             my_player->adder    = gst_element_factory_make ("adder", "mixer");
             gst_bin_add_many(GST_BIN(my_player->pipeline), my_player->adder, my_player->sink, NULL);
             gst_element_link(my_player->adder, my_player->sink);
             break;}
        case GST_MESSAGE_ERROR: {
             gchar  *debug;
             GError *error;
             gst_message_parse_error (msg, &error, &debug);
             g_free (debug);
             g_printerr ("Error: %s\n", error->message);
             g_error_free (error);
             g_main_loop_quit (my_player->loop);
             break;
        }
        default:
             break;
    }
    return TRUE;
}

static void on_pad_added (GstElement *element, GstPad *pad, gpointer data){
  printf("on pad added\n");
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

    gpointer my_player;
    my_player = this;

    count = 0;
    pipeline = gst_pipeline_new("audio-player");
    source   = gst_element_factory_make("filesrc", "file-source");
    sink     = gst_element_factory_make("autoaudiosink", "sink");
    adder    = gst_element_factory_make("adder", "mixer");

    if (!sink) printf("sink could not be created\n");
    if (!adder) printf("adder could not be created\n");
    if (!pipeline) printf("pipeline could not be created\n");

    bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
    bus_watch_id = gst_bus_add_watch (bus, bus_call, my_player);
    gst_object_unref (bus);

    gst_bin_add_many(GST_BIN(pipeline), adder, sink, NULL);
    gst_element_link(adder, sink);
}

void Player::_d_Player(){
    g_print("destructor\n");
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(GST_OBJECT(pipeline));
    g_source_remove(bus_watch_id);
    g_main_loop_quit (loop);
    g_main_loop_unref(loop);
}

void Player::play_sample(char* sample_name){
    std::string _count = std::to_string(count);

    GstElement* _source   = gst_element_factory_make("filesrc", (std::string("file-source")+_count).c_str());
    GstElement* _demuxer  = gst_element_factory_make ("oggdemux",(std::string("ogg-demuxer")+_count).c_str());
    GstElement* _decoder  = gst_element_factory_make ("vorbisdec",(std::string("vorbis-decoder")+_count).c_str());
    GstElement* _conv     = gst_element_factory_make ("audioconvert",(std::string("converter")+_count).c_str());

    if (!_source || !_demuxer || !_decoder || !_conv){
        printf("One element couldn't be created.");
    }

    gst_bin_add_many(GST_BIN(pipeline), _source, _demuxer, _decoder, _conv, NULL);
    g_signal_connect (_demuxer, "pad-added", G_CALLBACK (on_pad_added), _decoder); 

    g_object_set(G_OBJECT(_source), "location", sample_name, NULL);
    printf("linking source and demux %d\n", gst_element_link(_source, _demuxer));
    printf("linking decoder and conv %d\n", gst_element_link(_decoder, _conv));
    printf("linking conv and adder   %d\n", gst_element_link(_conv, adder));

    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    count++;
    g_main_loop_run(loop);
    printf("main loop terminated");
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
        gst_element_set_state(pipeline, GST_STATE_READY);
        g_object_set(G_OBJECT(source), "location", track_name, NULL);
        gst_element_set_state(pipeline, GST_STATE_PLAYING);
    }

}
