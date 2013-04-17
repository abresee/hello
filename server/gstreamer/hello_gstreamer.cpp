#include <iostream>
#include <limits>
#include <cmath>
#include <stdint.h>
#include <gst/gst.h>
#include <gst/app/gstappsrc.h>

const int sample_rate = 10000;
const int frequency = 440;
const char * format = "S16LE";
const int width = 16;
const int depth = 16;
const int seconds = 10;
const int signal_length = sample_rate*seconds;
const int buffer_length = 1024;
const int channels = 1;

int16_t data[signal_length];

static GMainLoop *loop;

using std::cout;
using std::endl;

static void
cb_need_data (GstElement *appsrc,
	      guint       unused_size,
	      gpointer    user_data)
{
    GstBuffer *buffer;
    GstFlowReturn ret;
    static int offset=0;

    buffer = gst_buffer_new_allocate (NULL, buffer_length, NULL);
    gst_buffer_fill(buffer,0,data+offset,buffer_length);

    offset+=buffer_length;
    offset%=signal_length;

    g_signal_emit_by_name (appsrc, "push-buffer", buffer, &ret);

    if (ret != GST_FLOW_OK) {
      /* something wrong, stop pushing */
      g_main_loop_quit (loop);
    }
}

gint main (gint argc, gchar *argv[])
{
    GstElement *pipeline, *appsrc, *conv, *audiosink;

    cout<<"data init."<<endl;
    for(int i=0; i < signal_length/2; ++i)
    {
        data[i]=std::numeric_limits<int16_t>::max()*sin(frequency*M_PI*2*i/sample_rate);
    }

    for(int i=signal_length/2; i < signal_length; ++i)
    {
        data[i]=std::numeric_limits<int16_t>::max()*sin(1.5*frequency*M_PI*2*i/sample_rate);
    }


    /* init GStreamer */
    cout<<"gstreamer init."<<endl;
    gst_init (&argc, &argv);
    loop = g_main_loop_new (NULL, FALSE);
    

    /* setup pipeline */
    cout<<"pipeline init."<<endl;
    pipeline = gst_pipeline_new ("pipeline");
    appsrc = gst_element_factory_make ("appsrc", "source");
    conv = gst_element_factory_make ("audioconvert", "conv");
    audiosink = gst_element_factory_make ("autoaudiosink", "audio-output");


    /* setup */
    cout<<"caps init."<<endl;
    g_object_set (G_OBJECT (appsrc), "caps",
            gst_caps_new_simple (
                "audio/x-raw",
                "format", G_TYPE_STRING, format,
                "rate", G_TYPE_INT, sample_rate,
                "channels",G_TYPE_INT, channels,
                "signed", G_TYPE_BOOLEAN, TRUE,
                "layout", G_TYPE_STRING, "interleaved",
                "channel-mask", GST_TYPE_BITMASK, 1,
                NULL),
            NULL);
    cout<<"bin init."<<endl;
    gst_bin_add_many (GST_BIN (pipeline), appsrc, conv, audiosink, NULL);
    cout<<"linking."<<endl;
    gst_element_link_many (appsrc, conv, audiosink, NULL);
    /* setup appsrc */
    g_signal_connect (appsrc, "need-data", G_CALLBACK (cb_need_data), NULL);

    /* play */
    gst_element_set_state (pipeline, GST_STATE_PLAYING);
    g_main_loop_run (loop);

    /* clean up */
    gst_element_set_state (pipeline, GST_STATE_NULL);
    gst_object_unref (GST_OBJECT (pipeline));
    g_main_loop_unref (loop);

    return 0;
}
