#include <iterator>
#include <iostream>
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
    for_each(instruments.begin(), instruments.end(), [&stream_end](InstrumentHandle i){
        stream_end = (stream_end > i->stream_end()) ? stream_end : i->stream_end();});
    offset_end = stream_end;
    gst_element_set_state (pipeline, GST_STATE_PLAYING);
    g_main_loop_run (loop);
}

void Player::quit()
{
    gst_app_src_end_of_stream(GST_APP_SRC(appsrc));
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
    mutex.lock();
    cout<<"Player::push_data"<<endl;
    auto packet_size = ((offset+last_hint) < offset_end) ? last_hint : offset_end - offset ;
    Packet data(packet_size);

    for (InstrumentHandle i : instruments)
    {
        i->get_samples(data,offset);
    }

    GstBuffer * buffer = gst_buffer_new();
    GstMemory * memory = gst_allocator_alloc(NULL, data.size(), NULL);

    GstMapInfo i;
    if(!gst_memory_map(memory, &i, GST_MAP_WRITE))
    {
        cout<<"shit's fucked up and bullshit"<<endl;
        std::exit(EXIT_FAILURE);
    }

    std::copy(data.begin(), data.end(), (Sample *)i.data);
    gst_memory_unmap(memory, &i);
    gst_buffer_insert_memory(buffer, -1, memory);
    memory = nullptr;
    auto ret = gst_app_src_push_buffer(GST_APP_SRC(appsrc), buffer); 
    if ( ret != GST_FLOW_OK)
    {
        cout<<"buffer fill returned error code "<<ret<<endl;
        return false;
    }
    cout<<"end of Player::push_data"<<endl;
    mutex.unlock();
    return true;
}

void Player::need_data(guint length)
{
    mutex.lock();
    cout<<"Player::need_data"<<endl;
    last_hint=length;
    if(sourceid==0)
    {
        sourceid=g_idle_add((GSourceFunc) util::wrap_push_data, this);
        cout<<"added source "<<sourceid<<endl;
    } 
    cout<<"end of Player::need_data"<<endl;
    mutex.unlock();
}

void Player::enough_data()
{
    mutex.lock();
    cout<<"enough_data!"<<endl;
    if(sourceid!=0)
    {
        cout<<"removing source "<<sourceid;
        g_source_remove(sourceid);
        sourceid=0;
        cout<<" aaaand done!"<<endl;
    }
    mutex.unlock();
}

gboolean Player::seek_data(guint64 destination)
{
    mutex.lock();
    //TODO: IMPLEMENT THIS
    cout<<"seek_data!"<<endl;
    return true;
    mutex.unlock();
}


Player::~Player()
{
    gst_element_set_state (pipeline, GST_STATE_NULL);
    gst_object_unref (GST_OBJECT (pipeline));
    g_main_loop_unref (loop);
}
