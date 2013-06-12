#ifndef PLAYER_H
#define PLAYER_H 
#include <vector>
#include <functional>
#include <cmath>
#include <boost/utility.hpp>
#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include "WaveSynth.h"
#include "global.h"

/// @brief master class to handle audio generation and playback
class Player : public boost::noncopyable {
    static const char * format_;

public: 
    void add_instrument(InstrumentHandle instrument);
    void play();
    void eos();
    void quit();

    Offset sample_rate() const;
    double freq_reference() const;

    virtual ~Player();

protected:
    class util {
    public:
        static void initialize_gst();
        static void build_gst_element(GstElement * &element, const char * kind, const char * name);
        static void wrap_need_data(GstAppSrc * element, guint length, gpointer instance);
        static void wrap_enough_data(GstAppSrc * element, gpointer instance);
        static gboolean wrap_seek_data(GstAppSrc * element, guint64 destination, gpointer instance);
        static gboolean wrap_push_data(gpointer instance);
        static gboolean wrap_bus_callback (GstBus *bus, GstMessage *message, gpointer instance);
    };
    Player();
    explicit Player(const Offset& sample_rate_init);
    explicit Player(const double freq_reference_init);
    Player(const Offset& sample_rate_init, const double freq_reference_init);

    gboolean push_data();
    void need_data(guint hint);
    void enough_data();
    gboolean seek_data(Offset);
    gboolean bus_callback(GstBus * bus, GstMessage * message);

    void eos_callback();


    const Offset sample_rate_;
    const double freq_reference_;

    GstElement * pipeline_;
    GstElement * appsrc_;
    GstElement * conv_;
    GstElement * audiosink_;
    GMainLoop * loop_;

    std::vector<InstrumentHandle> instruments_;
    Offset current_offset_;
    Offset end_offset_;
    guint push_id_;
    guint bus_watch_id_;
    guint last_hint_;

};

typedef std::shared_ptr<Player> PlayerHandle;
#endif /* PLAYER_H */
