#include <iterator>
#include <iostream>
#include <memory>
#include "Player.h"
#include "WaveGenerator.h"

const char * Player::format = "S16LE";
using std::for_each;
using std::cout;
using std::endl;

void Player::util::initialize_gst()
{
    GError *err;
    if(!gst_init_check(nullptr,nullptr,&err))
    {
        std::exit(err->code);
    }
}

void Player::util::build_gst_element(GstElement * &element, const char * kind, const char * name)
{
    element = gst_element_factory_make(kind,name);
    if(element==nullptr)
    {
        std::exit(EXIT_FAILURE);
    }
}

void Player::util::wrap_need_data(GstAppSrc * element, guint length, gpointer instance)
{
    Player * this_ = static_cast<Player *>(instance);
    this_->need_data(length);
}

void Player::util::wrap_enough_data(GstAppSrc * src, gpointer instance)
{
    Player * this_ = static_cast<Player *>(instance);
    this_->enough_data();
}

gboolean Player::util::wrap_seek_data(GstAppSrc * element, guint64 destination, gpointer instance)
{
    Player * this_ = static_cast<Player *>(instance);
    return this_->seek_data(destination);
}

gboolean Player::util::wrap_push_data(gpointer instance)
{
    Player * this_ = static_cast<Player *>(instance);
    return this_->push_data();
}

gboolean Player::util::wrap_bus_callback(GstBus * bus, GstMessage * message, gpointer data)
{
    Player * this_ = static_cast<Player *>(data);
    return this_->bus_callback(bus,message);
}

void Player::add_instrument(InstrumentHandle instrument)
{
    instruments.push_back(instrument);
}

void Player::add_instrument(Instrument * instrument)
{
    add_instrument(InstrumentHandle(instrument));
}

void Player::play()
{   
    guint64 stream_end=0;
    for(auto instrument : instruments)
    { 
        stream_end = (stream_end > instrument->stream_end()) ? 
            stream_end : 
            instrument->stream_end();
    }
    offset_end = stream_end;
    gst_element_set_state (pipeline, GST_STATE_PLAYING);
    g_main_loop_run (loop);
}

void Player::eos()
{
    GstFlowReturn r = gst_app_src_end_of_stream(GST_APP_SRC(appsrc));
    if (r!=GST_FLOW_OK)
    {
        cout<<"shit's fucked"<<endl;
    }
}

void Player::quit()
{
    gst_element_set_state(pipeline, GST_STATE_NULL);
    g_main_loop_quit(loop);
}

Player::Player(const char * sinktype) : pipeline(), appsrc(), conv(), audiosink(), loop(), instruments(), offset(), offset_end(), sourceid(), last_hint()
{
    if(!gst_is_initialized())
    {
        util::initialize_gst();
    }
    pipeline = gst_pipeline_new ("pipeline");
    if(pipeline==nullptr)
    {
        std::exit(EXIT_FAILURE);
    };

    util::build_gst_element(appsrc,"appsrc","source");
    util::build_gst_element(conv,"audioconvert","conv");
    util::build_gst_element(audiosink,sinktype,"output");

    GstBus * bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
    bus_watch_id = gst_bus_add_watch (bus, util::wrap_bus_callback, this);
    gst_object_unref (bus);
    

    GstCaps * caps = gst_caps_new_simple (
        "audio/x-raw",
        "format", G_TYPE_STRING, format,
        "rate", G_TYPE_INT, Config::sample_rate,
        "channels",G_TYPE_INT, Config::channels,
        "signed", G_TYPE_BOOLEAN, TRUE,
        "layout", G_TYPE_STRING, "interleaved",
        NULL);

    g_object_set (G_OBJECT (appsrc), "caps",
        caps,
        NULL);

    //the gstreamer main loop is the main event loop for audio generation
    loop = g_main_loop_new (NULL, FALSE);

    gst_bin_add_many (GST_BIN (pipeline), appsrc, conv, audiosink, NULL);
    gst_element_link_many (appsrc, conv, audiosink, NULL);

    GstAppSrcCallbacks callbacks = {util::wrap_need_data, util::wrap_enough_data, util::wrap_seek_data};
    gst_app_src_set_callbacks(GST_APP_SRC(appsrc), &callbacks, this, nullptr);
}


gboolean Player::push_data()
{
    if(offset >= offset_end)
    {
        eos();
        return false;
    }
    auto packet_size = ((offset+last_hint) < offset_end) ? last_hint : offset_end - offset ;
    std::unique_ptr<Packet> datahandle(new Packet(packet_size));
    for (InstrumentHandle i : instruments)
    {
        i->get_samples(*datahandle,offset);
    }
    
    GstBuffer * buffer = gst_buffer_new_allocate(NULL, datahandle->size()*Config::word_size, NULL);
    auto size = datahandle->size() * Config::word_size;
    gst_buffer_fill(buffer, 0, static_cast<void *>(datahandle->data()), size);
    auto ret = gst_app_src_push_buffer(GST_APP_SRC(appsrc), buffer); 
    if ( ret != GST_FLOW_OK)
    {
        cout<<"flow no good!"<<endl;
        return false;
    }
    offset+=packet_size;
    return true;
}

void Player::need_data(guint length)
{
    last_hint=length;
    if(sourceid==0)
    {
        sourceid=g_idle_add((GSourceFunc) util::wrap_push_data, this);
    } 
}

void Player::enough_data()
{
    if(sourceid!=0)
    {
        g_source_remove(sourceid);
        sourceid=0;
    }
}

gboolean Player::seek_data(guint64 destination)
{
    //TODO: IMPLEMENT THIS
    return true;
}

gboolean Player::bus_callback(GstBus * bus, GstMessage * message)
{
    cout<<"Got "<<
        GST_MESSAGE_TYPE_NAME(message)
        <<" message"<<endl;
    switch(GST_MESSAGE_TYPE(message)) {
        case GST_MESSAGE_ERROR:
            GError *err;
            gchar *debug;
            gst_message_parse_error(message,&err,&debug);            
            g_print("Error: %s\n",err->message);
            g_error_free(err); 
            g_free(debug);
            break;
        case GST_MESSAGE_EOS:
            eos_callback();
            break;
        default:
            break;
    }
    return true;
}

void Player::eos_callback()
{
    quit();
}


Player::~Player()
{
    gst_element_set_state (pipeline, GST_STATE_NULL);
    gst_object_unref (GST_OBJECT (pipeline));
    g_main_loop_unref (loop);
}
