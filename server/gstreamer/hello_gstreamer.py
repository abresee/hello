#!/usr/bin/env python

from __future__ import print_function
import gi
gi.require_version('Gst','1.0')
from gi.repository import GObject, Gst, Gtk
from gi.repository import GdkX11, GstVideo
gi.require_version('Gst','1.0')
GObject.threads_init()
Gst.init(None)

class Player(object):
    def __init__(self):

        self.pipeline=Gst.Pipeline()

        self.bus = self.pipeline.get_bus()
        self.bus.add_signal_watch()
        self.bus.connect('message::eos',self.on_eos)
        self.bus.connect('message::error',self.on_error)

        self.src=Gst.ElementFactory.make('appsrc','source')
        self.conv=Gst.ElementFactory.make('audioconvert','conv')
        self.audiosink = Gst.ElementFactory.make('autoaudiosink','audio-output')
        self.capsstring="audio/x-raw, format={0}, rate={1}, channels=1, signed=true, layout=interleaved".format("S16LE",44100)
        self.src.set_property("caps",Gst.Caps.from_string(self.capsstring))
        self.src.link(self.conv)
        self.conv.link(self.audiosink)

        self.src.connect("need-data",self.need_data)
        self.src.connect("enough-data",self.enough_data)


        
    def run(self):
        self.pipeline.set_state(Gst.State.PLAYING)
        Gtk.main()

    def quit(self,window):
        self.pipeline.set_state(Gst.State.NULL)
        Gtk.main_quit()

    def on_eos(self,bus,msg):
        self.quit()

    def on_error(self,bus,msg):
        print('on_error():',msg.parse_error())

    def push_data(self):
        pass

    def need_data(self):
        pass
    
    def enough_data(self):
        pass

p = Player()
