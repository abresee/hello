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
    static const char * format;
    class util {
    public:
        /// @brief Helper function to initialize gstreamer
        static void initialize_gst();
        /// @brief Helper function to build gst elements to centralize boilerplate error checking.
        static void build_gst_element(GstElement * &element, const char * kind, const char * name);
        /// @brief wrapper for member function b/c gst can't handle member functions
        static void wrap_need_data(GstAppSrc * element, guint length, gpointer instance);
        /// @brief wrapper for member function b/c gst can't handle member functions
        static void wrap_enough_data(GstAppSrc * element, gpointer instance);
        /// @brief wrapper for member function b/c gst can't handle member functions
        static gboolean wrap_seek_data(GstAppSrc * element,guint64 destination,gpointer instance);
        /// @brief wrapper for member function b/c gst can't handle member functions
        static gboolean wrap_push_data(gpointer instance);
    };
public: 
    /// @brief add an instrument by a InstrumentHandle pointing to it
    void add_instrument(InstrumentHandle instrument);
    /// @brief add an instrument by normal pointer
    void add_instrument(Instrument* instrument);

    /// @brief start playback
    void play();
    /// @brief stop playback
    void quit();

    /// @brief dtor
    virtual ~Player();

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
    
    /// @brief container for the player object's instruments
    std::vector<InstrumentHandle> instruments;
    guint64 offset;
    guint64 offset_end;
    guint sourceid;
    guint last_hint;

    /// @brief callback that actually inserts data into appsrc
    gboolean push_data();
    /// @brief callback that handles appsrc's need-data signal 
    void need_data(guint);
    /// @brief callback that handles appsrc's enough-data signal 
    void enough_data();
    /// @brief callback that handles appsrc's seek-data signal 
    gboolean seek_data(guint64);

};

class LocalPlayer : boost::noncopyable, public Player
{
public:
    LocalPlayer() : Player("autoaudiosink") {}
};
#endif /* PLAYER_H */
