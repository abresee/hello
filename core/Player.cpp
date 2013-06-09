#include <iterator>
#include <iostream>
#include <memory>
#include <boost/assert.hpp>
#include "Player.h"

const char * Player::format = "S16LE";

void Player::util::initialize_gst() {
    GError *err;
    if(!gst_init_check(nullptr,nullptr,&err)) {
        std::exit(err->code);
    }
}

void Player::util::build_gst_element(GstElement * &element, const char * kind, const char * name) {
    element = gst_element_factory_make(kind,name);
    if(element==nullptr) {
        std::exit(EXIT_FAILURE);
    }
}

void Player::util::wrap_need_data(GstAppSrc * element, guint length, gpointer instance) {
    Player * this_ = static_cast<Player *>(instance);
    this_->need_data(length);
}

void Player::util::wrap_enough_data(GstAppSrc * src, gpointer instance) {
    Player * this_ = static_cast<Player *>(instance);
    this_->enough_data();
}

gboolean Player::util::wrap_seek_data(GstAppSrc * element, guint64 destination, gpointer instance) {
    Player * this_ = static_cast<Player *>(instance);
    return this_->seek_data(destination);
}

gboolean Player::util::wrap_push_data(gpointer instance) {
    Player * this_ = static_cast<Player *>(instance);
    return this_->push_data();
}

gboolean Player::util::wrap_bus_callback(GstBus * bus, GstMessage * message, gpointer instance) {
    Player * this_ = static_cast<Player *>(instance);
    return this_->bus_callback(bus,message);
}

void Player::add_instrument(InstrumentHandle instrument_h) {
    instruments.push_back(instrument_h);
}

void Player::play() {   
    offset_t stream_end=0;
    for(auto instrument_h : instruments) { 
        stream_end = std::max(stream_end,instrument_h->stream_end());
    }
    offset_end = stream_end;
    g_object_set(G_OBJECT(appsrc),"size",offset_end*Config::word_size,nullptr);
    gst_element_set_state (pipeline, GST_STATE_PLAYING);
    g_main_loop_run (loop);
}

void Player::eos() {
    GstFlowReturn r = gst_app_src_end_of_stream(GST_APP_SRC(appsrc));
    if (r!=GST_FLOW_OK) {
        throw BadFlowException("bad flow on end of stream");
    }
}

void Player::quit() {
    gst_element_set_state(pipeline, GST_STATE_NULL);
    g_main_loop_quit(loop);
}

Player::Player(const char * sinktype) : pipeline(), appsrc(), conv(), audiosink(), loop(), instruments(), offset(), offset_end(), sourceid(), last_hint() {
    if(!gst_is_initialized()) {
        util::initialize_gst();
    }
    pipeline = gst_pipeline_new ("pipeline");
    if(pipeline==nullptr) {
        std::exit(EXIT_FAILURE);
    };

    util::build_gst_element(appsrc,"appsrc","source");
    util::build_gst_element(conv,"audioconvert","conv");

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
        nullptr);

    g_object_set( G_OBJECT(appsrc), "caps", caps, nullptr);
    g_object_set( G_OBJECT(appsrc),"is-live",true,nullptr);
    g_object_set( G_OBJECT(appsrc),"min-latency",0,nullptr);
    g_object_set( G_OBJECT(appsrc),"emit-signals",false,nullptr);
    g_object_set( G_OBJECT(appsrc),"format",GST_FORMAT_TIME,nullptr);

    //the gstreamer main loop is the main event loop for audio generation
    loop = g_main_loop_new (nullptr, FALSE);

    gst_bin_add_many (GST_BIN (pipeline), appsrc, conv, nullptr);
    gst_element_link (appsrc, conv);

    GstAppSrcCallbacks callbacks = {util::wrap_need_data, util::wrap_enough_data, util::wrap_seek_data};
    gst_app_src_set_callbacks(GST_APP_SRC(appsrc), &callbacks, this, nullptr);
}

void Player::add_instrument(InstrumentHandle instrument_h) {
    instruments.push_back(instrument_h);
}

gboolean Player::push_data() {
    if(offset >= offset_end) {
        eos();
        return false;
    }
    
    const Packet::size_type packet_size = ((offset+last_hint) < offset_end) ? last_hint : offset_end - offset ; 
    Packet data(packet_size);

    for (InstrumentHandle instrument_h : instruments) {
        data+=instrument_h->get_samples(offset,offset+packet_size);
    }

    GstBuffer * buffer = gst_buffer_new_allocate(
        nullptr, data.size()*Config::word_size, nullptr);

    GST_BUFFER_PTS(buffer) = Config::offset_to_ns(offset);
    GST_BUFFER_DURATION(buffer) = Config::offset_to_ns(packet_size);
    GST_BUFFER_OFFSET(buffer) = offset;
    GST_BUFFER_OFFSET_END(buffer) = offset+packet_size;
    auto size = data.size() * Config::word_size;
    auto rsize = gst_buffer_fill(buffer, 0, static_cast<void *>(data.data()), size);
    BOOST_ASSERT(size==rsize);
    auto ret = gst_app_src_push_buffer(GST_APP_SRC(appsrc), buffer); 
    if ( ret != GST_FLOW_OK) {
        throw BadFlowException("bad flow while pushing buffer");
    }
    offset+=data.size();
    return true;
}

void Player::need_data(guint length) {
    last_hint=length;
    if(sourceid==0) {
        sourceid=g_idle_add((GSourceFunc) util::wrap_push_data, this);
    } 
}

void Player::enough_data() {
    if(sourceid!=0) {
        g_source_remove(sourceid);
        sourceid=0;
    }
}

gboolean Player::seek_data(offset_t destination) {
    //TODO: IMPLEMENT THIS
    return true;
}

gboolean Player::bus_callback(GstBus * bus, GstMessage * message) {
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
            quit();
            break;

        default:
            break;
    }
    return true;
}

void Player::play() {   
    offset_t stream_end=0;
    for(auto instrument_h : instruments) { 
        stream_end = std::max(stream_end,instrument_h->stream_end());
    }
    offset_end = stream_end;
    g_object_set(G_OBJECT(appsrc),"size",offset_end*Config::word_size,nullptr);
    gst_element_set_state (pipeline, GST_STATE_PLAYING);
    g_main_loop_run (loop);
}

void Player::eos() {
    GstFlowReturn r = gst_app_src_end_of_stream(GST_APP_SRC(appsrc));
    if (r!=GST_FLOW_OK)
    {
        std::cout<<"shit's fucked"<<std::endl;
    }
}

void Player::quit() {
    gst_element_set_state(pipeline, GST_STATE_NULL);
    g_main_loop_quit(loop);
}

Player::~Player() {
    gst_element_set_state (pipeline, GST_STATE_NULL);
    gst_object_unref (GST_OBJECT (pipeline));
    g_main_loop_unref (loop);
}

LocalPlayer::LocalPlayer() {

    util::build_gst_element(audiosink,"autoaudiosink","output");
    gst_bin_add(GST_BIN(pipeline),audiosink);
    gst_element_link(conv, audiosink);
}

StreamPlayer::StreamPlayer() {
    util::build_gst_element(vorbisencoder,"vorbisenc", "encoder");
    util::build_gst_element(oggmuxer,"oggmux", "muxer");
    util::build_gst_element(audiosink, "filesink", "sink");

    g_object_set(G_OBJECT(audiosink),"location","out.ogg",nullptr);
    gst_bin_add_many(GST_BIN(pipeline),vorbisencoder,oggmuxer,audiosink,nullptr);
    gst_element_link_many(conv,vorbisencoder,oggmuxer,audiosink,nullptr);
}
    
BadFlowException::BadFlowException(const char * cstr):
    message(cstr) { 
}

BadFlowException::BadFlowException(const std::string& str):
    message(str) {
}

const char * BadFlowException::what() {
    return message.c_str();
}

