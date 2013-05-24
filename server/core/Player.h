#ifndef PLAYER_H
#define PLAYER_H 
#include <vector>
#include <mutex>
#include <functional>
#include <cmath>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>
#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include "Config.h"

/// @brief master class to handle audio generation and playback
class Player : public boost::noncopyable {
public: 
    /// @brief add an instrument by a InstrumentHandle pointing to it
    void add_instrument(InstrumentHandle);
    /// @brief add an instrument by normal pointer
    void add_instrument(Instrument*);

    /// @brief start playback
    void play();
    /// @brief stop playback
    void quit();

    /// @brief dtor
    ~Player();


protected:
    Player(const char *);

    /// @brief gst object representing the whole pipeline
    GstElement * pipeline;
    /// @brief gst object representing the interface between our code and the pipeline
    GstElement * appsrc;
    /// @brief gst object that converts audio data between appsrc and the sink
    GstElement * conv;
    /// @brief gst object representing the sink (stream, speakers, whatever)
    GstElement * audiosink;
    /// @brief glib object representing the mainloop used by gstreamer
    GMainLoop * loop;
    
    std::mutex player_mutex;

    /// @brief container for the player object's instruments
    std::vector<InstrumentHandle> instruments;

    int offset=0;
    /// @brief gst internal id for the push_data idle handler
    guint sourceid=0;

    /// @brief Helper function to build gst elements to centralize boilerplate error checking.
    void static build_gst_element(GstElement*&,const char*,const char*);
    /// @brief Helper function to initialize gstreamer
    void static initialize_gst();

    /// @brief wrapper for member function b/c gst can't handle member functions
    static gboolean push_data_g(gpointer);
    /// @brief wrapper for member function b/c gst can't handle member functions
    static void need_data_g(GstAppSrc*,guint,gpointer);
    /// @brief wrapper for member function b/c gst can't handle member functions
    static void enough_data_g(GstAppSrc*,gpointer);

    static gboolean seek_data_g(GstAppSrc*,guint64,gpointer);

    /// @brief callback that actually inserts data into appsrc
    gboolean push_data();
    /// @brief callback that handles appsrc's need-data signal 
    void need_data(int);
    /// @brief callback that handles appsrc's enough-data signal 
    void enough_data();

    gboolean seek_data(guint64);

    ///Static constant representing the largest representable sample value
    static const Sample max_volume;
    static const char * format;
};

class LocalPlayer : boost::noncopyable, public Player
{
public:
    LocalPlayer() : Player("autoaudiosink") {}
};
#endif /* PLAYER_H */
