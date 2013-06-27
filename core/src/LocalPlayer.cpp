#include "LocalPlayer.h"
LocalPlayer::LocalPlayer() {
    util::build_gst_element(audiosink_,"autoaudiosink","output");
    gst_bin_add(GST_BIN(pipeline_),audiosink_);
    gst_element_link(conv_, audiosink_);
}
