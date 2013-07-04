#include "VorbisPlayer.h"
VorbisPlayer::VorbisPlayer(const std::string output_name) {
    util::build_gst_element(vorbisencoder_,"vorbisenc", "encoder");
    util::build_gst_element(oggmuxer_,"oggmux", "muxer");
    util::build_gst_element(audiosink_, "filesink", "sink");

    g_object_set(G_OBJECT(audiosink_),"location",output_name.c_str(),nullptr);
    gst_bin_add_many(GST_BIN(pipeline_),vorbisencoder_,oggmuxer_,audiosink_,nullptr);
    gst_element_link_many(conv_,vorbisencoder_,oggmuxer_,audiosink_,nullptr);
}
