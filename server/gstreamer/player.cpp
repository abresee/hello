#include "player.h"

using std::cout;
using std::endl;

void Player::initdata() {
    for(int i=0; i < signal_length/4; ++i)
    {
        data[i]=amplitude*sin(frequency*M_PI*2*i/sample_rate);
    }

    for(int i=signal_length/4; i < signal_length/2; ++i)
    {
        data[i]=amplitude*sin(1.5*frequency*M_PI*2*i/sample_rate);
    }

    for(int i=signal_length/2; i < signal_length; ++i)
    {
        data[i]=amplitude*sin(2*frequency*M_PI*2*i/sample_rate);
    }
}

Player::Player(int * argc, char *** argv)
{
    initdata();
    gst_init(argc,argv);
    pipeline = gst_pipeline_new ("pipeline");

    appsrc = gst_element_factory_make ("appsrc", "source");
    conv = gst_element_factory_make ("audioconvert", "conv");
    audiosink = gst_element_factory_make ("autoaudiosink", "audio-output");

    g_object_set (G_OBJECT (appsrc), "caps",
            gst_caps_new_simple (
                "audio/x-raw",
                "format", G_TYPE_STRING, format,
                "rate", G_TYPE_INT, sample_rate,
                "channels",G_TYPE_INT, channels,
                "signed", G_TYPE_BOOLEAN, TRUE,
                "layout", G_TYPE_STRING, "interleaved",
                NULL),
            NULL);

    loop = g_main_loop_new (NULL, FALSE);
    gst_bin_add_many (GST_BIN (pipeline), appsrc, conv, audiosink, NULL);
    gst_element_link_many (appsrc, conv, audiosink, NULL);
    g_signal_connect (appsrc, "need-data", G_CALLBACK (cb_need_data),this);
    g_signal_connect (appsrc, "enough-data", G_CALLBACK (cb_enough_data),this);
}

void Player::play()
{
    gst_element_set_state (pipeline, GST_STATE_PLAYING);
    g_main_loop_run (loop);
}
void Player::quit()
{
    gst_element_set_state(pipeline, GST_STATE_NULL);
    g_main_loop_quit(loop);
}

Player::~Player()
{
    gst_element_set_state (pipeline, GST_STATE_NULL);
    gst_object_unref (GST_OBJECT (pipeline));
    g_main_loop_unref (loop);
}

const char * Player::format = "S16LE";
gboolean cb_push_data(gpointer instance)
{
    Player * this_ = static_cast<Player *>(instance);

    GstBuffer * buffer = gst_buffer_new_allocate (NULL, Player::buffer_length, NULL);
    gst_buffer_fill(buffer,0,this_->data+this_->offset, Player::buffer_length);

    this_->offset+=Player::buffer_length/Player::word_size;
    if(this_->offset > Player::signal_length)
    {
        this_->quit();
        return false;
    }
    GstFlowReturn ret;
    g_signal_emit_by_name (this_->appsrc, "push-buffer", buffer, &ret);

    if (ret != GST_FLOW_OK) {
      /* something wrong, stop pushing */
      g_main_loop_quit (this_->loop);
      return false;
    }
    return true;
}

void cb_need_data(GstElement* element, guint length, gpointer instance)
{
    Player * this_ = static_cast<Player *>(instance);
    if(this_->sourceid==0)
    {
        this_->sourceid=g_idle_add((GSourceFunc) cb_push_data, instance);
    } 
}

void cb_enough_data(GstElement* src, gpointer instance)
{
    Player * this_ = static_cast<Player *>(instance);
    if(this_->sourceid!=0)
    {
        g_source_remove(this_->sourceid);
        this_->sourceid=0;
    }
}
