#!/usr/bin/env python

from __future__ import print_function
from math import sin, pi, trunc
import gi
gi.require_version('Gst','1.0')
from gi.repository import GLib, GObject, Gst, Gtk
from gi.repository import GdkX11, GstVideo
gi.require_version('Gst','1.0')
GObject.threads_init()
Gst.init(None)

sample_rate=44100
amplitude=32767
frequency = 440
omega = 2 * pi * frequency / sample_rate
packet_count = 512
word_size = 2
buffer_length = word_size * packet_count 
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

        self.pipeline.add(self.src)
        self.pipeline.add(self.conv)
        self.pipeline.add(self.audiosink)

        self.src.link(self.conv)
        self.conv.link(self.audiosink)

        self.src.connect("need-data",self.need_data)
        self.src.connect("enough-data",self.enough_data)
    

        self.sourceid=None

        self.sample_count=0

    def run(self):
        self.pipeline.set_state(Gst.State.PLAYING)

    def quit(self):
        self.pipeline.set_state(Gst.State.NULL)

    def on_eos(self,bus,msg):
        self.quit()

    def on_error(self,bus,msg):
        print('on_error():',msg.parse_error())

    def need_data(self,src,avail):
        data=bytearray().join(reversed([trunc(amplitude * sin(omega*i)).to_bytes(2,'little',signed=True) 
                        for i in range(self.sample_count,
                            self.sample_count+trunc(avail/2))]))
        self.last_data=data
        self.sample_count+=avail
        buff=Gst.Buffer.new_allocate(None, avail, None)
        buff.fill(0,data,avail)
        src.emit("push_buffer",buff)


    def enough_data(self):
        print("that's enough!")
        if self.sourceid is not None:
            GObject.source_remove(self.sourceid)
            self.sourceid=None

p = Player()
p.run()
