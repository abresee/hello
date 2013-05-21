#include <iterator>
#include <iostream>
#include "Player.h"
#include "WaveGenerator.h"

const char * Player::format = "S16LE";

void Player::build_gst_element(GstElement* &element, const char * kind, const char * name)
{
    element = gst_element_factory_make(kind,name);
    if(element==nullptr)
    {
        std::exit(EXIT_FAILURE);
    }
}

void Player::initialize_gst()
{
    GError *err;
    if(!gst_init_check(nullptr,nullptr,&err))
    {
        std::exit(err->code);
    }
}

Player::Player(const char * sinktype)
{
    if(!gst_is_initialized())
    {
        initialize_gst();
    }

    if(pipeline = gst_pipeline_new ("pipeline"),pipeline==nullptr)
    {
        std::exit(EXIT_FAILURE);
    };

    build_gst_element(pipeline,"pipeline","pipe");
    build_gst_element(appsrc,"appsrc","source");
    build_gst_element(conv,"audioconvert","conv");
    build_gst_element(audiosink,sinktype,"output");

    caps = gst_caps_new_simple (
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

    //need_data_g and enough_data_g are wrappers for member functions in Player
    GstAppSrcCallbacks callbacks = {need_data_g, enough_data_g, seek_data_g};
    gst_app_src_set_callbacks(GST_APP_SRC(appsrc), &callbacks, this, nullptr);
}

void Player::add_instrument(Instrument * instrument)
{
    add_instrument(InstrumentHandle(instrument));
}

void Player::add_instrument(InstrumentHandle instrument)
{
    instruments.push_back(instrument);
}

void Player::play()
{   
    std::cout<<"playing!"<<std::endl; 
    gst_element_set_state (pipeline, GST_STATE_PLAYING);
    g_main_loop_run (loop);
}

void Player::quit()
{
    gst_app_src_end_of_stream(GST_APP_SRC(appsrc));
    gst_element_set_state(pipeline, GST_STATE_NULL);
    g_main_loop_quit(loop);
}

gboolean Player::push_data()
{
    player_mutex.lock();
    std::cout<<"push_data! mutex is locked"<<std::endl;
    Packet data(Config::packet_size);
    auto offset_ = this->offset;
    std::cout<<"getting shit from instruments..."<<std::endl;
    std::for_each(instruments.begin(), instruments.end(),
        [&data,offset_](InstrumentHandle i)
    {
        std::cout<<"getting shit from an instrument..."<<std::endl;
        PacketHandle p=i->get_samples(offset_,offset_+Config::packet_size);
        std::cout<<"got a packet of length "<<p->size()<<std::endl;
        std::transform(data.begin(),data.end(),p->begin(),p->end(),std::plus<Sample>());
        std::cout<<"transformed!"<<std::endl;

    });
    std::cout<<"makin a new buffer!"<<std::endl;
    GstBuffer * buffer = gst_buffer_new();
    std::cout<<"buffer address: "<<buffer<<std::endl;
    GstMemory * memory = gst_allocator_alloc(NULL, Config::buffer_length, NULL);
    std::cout<<"memory address: "<<memory<<std::endl;
    GstMapInfo i;
    bool map_return = gst_memory_map(memory, &i, GST_MAP_WRITE);
    if(!map_return)
    {
        std::cout<<"shit's fucked up and bullshit"<<std::endl;
        std::exit(EXIT_FAILURE);
    }
    std::copy(data.begin(),data.end(),(Sample *)i.data);
    gst_buffer_insert_memory(buffer,-1,memory);
    memory=nullptr;
    auto ret =gst_app_src_push_buffer(GST_APP_SRC(appsrc),buffer); 
    if ( ret != GST_FLOW_OK)
    {
        std::cout<<"buffer fill returned error code "<<ret<<std::endl;
        return false;
    }
    std::cout<<"buffer filled, supposedly! unlocking mutex..."<<std::endl;
    player_mutex.unlock();
    return true;
}

void Player::need_data(int length)
{
    if(sourceid==0)
    {
        player_mutex.lock();
        std::cout<<"need_data! length is "<<length<<", mutex is locked"<<std::endl;
        sourceid=g_idle_add((GSourceFunc) push_data_g, this);
        std::cout<<"sourceid is "<<sourceid<<", unlocking mutex..."<<std::endl;
        player_mutex.unlock();
    } 
}

void Player::enough_data()
{
    if(sourceid!=0)
    {
        player_mutex.lock();
        std::cout<<"enough_data! mutex is locked. sourceid is: "<<sourceid<<std::endl;
        g_source_remove(sourceid);
        sourceid=0;
        std::cout<<"unlocking mutex..."<<std::endl;
        player_mutex.unlock();
    }
}

gboolean Player::seek_data(guint64 destination)
{
    //TODO: IMPLEMENT THIS
    return true;
}

gboolean Player::push_data_g(gpointer instance)
{
    Player * this_ = static_cast<Player *>(instance);
    return this_->push_data();
}

gboolean Player::seek_data_g(GstAppSrc* element, guint64 destination, gpointer instance)
{
    Player * this_ = static_cast<Player *>(instance);
    return this_->seek_data(destination);
}

void Player::need_data_g(GstAppSrc* element, guint length, gpointer instance)
{
    Player * this_ = static_cast<Player *>(instance);
    this_->need_data(length);
}

void Player::enough_data_g(GstAppSrc* src, gpointer instance)
{
    Player * this_ = static_cast<Player *>(instance);
    this_->enough_data();
}

Player::~Player()
{
    gst_element_set_state (pipeline, GST_STATE_NULL);
    gst_object_unref (GST_OBJECT (pipeline));
    g_main_loop_unref (loop);
}
