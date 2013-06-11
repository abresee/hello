#include <iterator>
#include <iostream>
#include <memory>
#include <boost/assert.hpp>
#include "Player.h"
#include "Time.h"
#include "BadFlowException.h"

const char * Player::format_ = "S16LE";

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
    return this_->seek_data(Offset(destination));
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
    instruments_.push_back(instrument_h);
}

void Player::play() {   
    Beat stream_end(0);
    for(auto instrument_h : instruments_) { 
        stream_end = std::max(stream_end,instrument_h->stream_end());
    }
    end_offset_ = stream_end.to_offset(Config::tempo, Config::sample_rate);
    
    g_object_set(G_OBJECT(appsrc_),"size",end_offset_.value()*Config::word_size,nullptr);
    gst_element_set_state (pipeline_, GST_STATE_PLAYING);
    g_main_loop_run (loop_);
}

void Player::eos() {
    GstFlowReturn r = gst_app_src_end_of_stream(GST_APP_SRC(appsrc_));
    if (r!=GST_FLOW_OK) {
        throw BadFlowException("bad flow on end of stream");
    }
}

void Player::quit() {
    gst_element_set_state(pipeline_, GST_STATE_NULL);
    g_main_loop_quit(loop_);
}

Player::Player() : pipeline_(), appsrc_(), conv_(), audiosink_(), loop_(), instruments_(), current_offset_(), end_offset_(), push_id_(), bus_watch_id_(), last_hint_() {
    if(!gst_is_initialized()) {
        util::initialize_gst();
    }
    pipeline_ = gst_pipeline_new ("pipeline");
    if(pipeline_==nullptr) {
        std::exit(EXIT_FAILURE);
    };

    util::build_gst_element(appsrc_,"appsrc","source");
    util::build_gst_element(conv_,"audioconvert","conv");

    GstBus * bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline_));
    bus_watch_id_ = gst_bus_add_watch (bus, util::wrap_bus_callback, this);
    gst_object_unref (bus);
    

    GstCaps * caps = gst_caps_new_simple(
        "audio/x-raw",
        "format", G_TYPE_STRING, format_,
        "rate", G_TYPE_INT, Config::sample_rate,
        "channels",G_TYPE_INT, Config::channels,
        "signed", G_TYPE_BOOLEAN, TRUE,
        "layout", G_TYPE_STRING, "interleaved",
        nullptr);

    g_object_set(G_OBJECT(appsrc_),"caps",caps,nullptr);
    g_object_set(G_OBJECT(appsrc_),"is-live",true,nullptr);
    g_object_set(G_OBJECT(appsrc_),"min-latency",0,nullptr);
    g_object_set(G_OBJECT(appsrc_),"emit-signals",false,nullptr);
    g_object_set(G_OBJECT(appsrc_),"format",GST_FORMAT_TIME,nullptr);

    //the gstreamer main loop is the main event loop for audio generation
    loop_ = g_main_loop_new (nullptr, FALSE);

    gst_bin_add_many (GST_BIN (pipeline_), appsrc_, conv_, nullptr);
    gst_element_link (appsrc_, conv_);

    GstAppSrcCallbacks callbacks = {util::wrap_need_data, util::wrap_enough_data, util::wrap_seek_data};
    gst_app_src_set_callbacks(GST_APP_SRC(appsrc_), &callbacks, this, nullptr);
}

gboolean Player::push_data() {
    if(current_offset_ >= end_offset_) {
        eos();
        return false;
    }
    
    Offset packet_size = ((current_offset_+last_hint_) < end_offset_) ? last_hint_ : end_offset_ - current_offset_ ; 
    Packet data(packet_size.value());

    for (InstrumentHandle instrument_h : instruments_) {
        data+=instrument_h->get_samples(current_offset_,current_offset_+packet_size);
    }

    GstBuffer * buffer = gst_buffer_new_allocate(
        nullptr, data.size()*Config::word_size, nullptr);

    GST_BUFFER_PTS(buffer) = current_offset_.to_time(Config::sample_rate).value();
    GST_BUFFER_DURATION(buffer) = packet_size.to_time(Config::sample_rate).value();
    GST_BUFFER_OFFSET(buffer) = current_offset_.value();
    GST_BUFFER_OFFSET_END(buffer) = (current_offset_+packet_size).value();

    auto size = data.size() * Config::word_size;
    auto rsize = gst_buffer_fill(buffer, 0, static_cast<void *>(data.data()), size);
    BOOST_ASSERT(size==rsize);

    auto ret = gst_app_src_push_buffer(GST_APP_SRC(appsrc_), buffer); 
    if ( ret != GST_FLOW_OK) {
        throw BadFlowException("bad flow while pushing buffer");
    }
    current_offset_+=data.size();
    return true;
}

void Player::need_data(guint length) {
    last_hint_=length;
    if(push_id_ == 0) {
        push_id_=g_idle_add((GSourceFunc) util::wrap_push_data, this);
    } 
}

void Player::enough_data() {
    if(push_id_ != 0) {
        g_source_remove(push_id_);
        push_id_=0;
    }
}

gboolean Player::seek_data(Offset destination) {
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

Player::~Player() {
    gst_element_set_state (pipeline_, GST_STATE_NULL);
    gst_object_unref (GST_OBJECT (pipeline_));
    g_main_loop_unref (loop_);
}
