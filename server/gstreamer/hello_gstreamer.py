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

sample_rate = 44100
amplitude = 32767
frequency = 440
seconds = 10
signal_length=sample_rate*seconds
word_size = 2
byte_length = signal_length * word_size
omega = 2 * pi * frequency / sample_rate
packet_count = 512
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

        self.capsstring="audio/x-raw, format={0}, rate={1}, channels=1, signed=true, layout=interleaved".format("S16LE",sample_rate)
        self.src.set_property("caps",Gst.Caps.from_string(self.capsstring))

        self.pipeline.add(self.src)
        self.pipeline.add(self.conv)
        self.pipeline.add(self.audiosink)

        self.src.link(self.conv)
        self.conv.link(self.audiosink)

        self.src.connect("need-data",self.need_data)
        self.src.connect("enough-data",self.enough_data)

        self.add = False
        self.playing=False
        self.byte_count = 0 
         
        sample_list = [trunc(amplitude * sin(omega*i)) for i in range(signal_length)]

        data_list = [i.to_bytes(2,'little',signed=True) for i in sample_list]

        self.data = bytes().join(data_list)

    def run(self):
        self.playing=True
        self.pipeline.set_state(Gst.State.PLAYING)

    def quit(self):
        self.playing=False
        self.pipeline.set_state(Gst.State.NULL)

    def on_eos(self,bus,msg):
        self.quit()

    def on_error(self,bus,msg):
        print('on_error():',msg.parse_error())

    def push_data(self):
        
        buff=Gst.Buffer.new_allocate(None,buffer_length , None)
        if self.byte_count > byte_length:
            self.quit()
        buff.fill(0,self.data[self.byte_count:self.byte_count+buffer_length],buffer_length)
        self.byte_count += buffer_length

        self.src.emit("push_buffer",buff)

    def need_data(self,src,byte_count):
        print("need data!")
        self.add=True

    def enough_data(self,src):
        print("that's enough!")
        self.add=False

p = Player()
try:
    p.run()
    while p.playing:
        if p.add:
            p.push_data()
        

except KeyboardInterrupt:
    p.quit()


