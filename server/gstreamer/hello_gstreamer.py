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


class Player(object):
    '''instantiations of this class generate aribtrary data for the sound device'''

    #statics
    @classmethod
    def Byte_convert(cls,sample_list):
        return [i.to_bytes(2,'little',signed=True) for i in sample_list]

    Sample_rate = 44100
    Amplitude = 32767
    Frequency = 440
    Seconds = 5
    Signal_length=Sample_rate*Seconds
    Word_size = 2
    Byte_length = Signal_length * Word_size
    Omega = 2 * pi * Frequency / Sample_rate
    Packet_count = 10*512
    Buffer_length = Word_size * Packet_count 
    __Init=False

    @classmethod
    def __Static_init(cls):
        cls.Sample_list = [trunc(cls.Amplitude * sin(cls.Omega*i)) for i in range(cls.Signal_length)]
        cls.Data_list = cls.Byte_convert(cls.Sample_list)
        cls.__Init=True

    #instance 
    def __init__(self):
        if not self.__Init:
            self.__Static_init()

        self.pipeline=Gst.Pipeline()

        self.bus = self.pipeline.get_bus()
        self.bus.add_signal_watch()
        self.bus.connect('message::eos',self.on_eos)
        self.bus.connect('message::error',self.on_error)

        self.src=Gst.ElementFactory.make('appsrc','source')
        self.conv=Gst.ElementFactory.make('audioconvert','conv')
        self.audiosink = Gst.ElementFactory.make('autoaudiosink','audio-output')

        self.capsstring="audio/x-raw, format={0}, rate={1}, channels=1, signed=true, layout=interleaved".format("S16LE",self.Sample_rate)
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
         

        self.data = bytes().join(self.Data_list)
        self.protect=False
        self.written=[]


    def run(self):
        self.playing=True
        self.pipeline.set_state(Gst.State.PLAYING)

    def quit(self):
        self.playing=False
        self.protect=False
        self.pipeline.set_state(Gst.State.NULL)

    def on_eos(self,bus,msg):
        self.quit()

    def on_error(self,bus,msg):
        print('on_error():',msg.parse_error())

    def push_data(self):
        while self.protect: pass
        self.protect=True
        
        buff=Gst.Buffer.new_allocate(None,self.Buffer_length , None)
        if self.byte_count > self.Byte_length:
            self.src.emit("end-of-stream")
            return
        to_write=self.data[self.byte_count:self.byte_count+self.Buffer_length]
        buff.fill(0,to_write,self.Buffer_length)
        self.byte_count += self.Buffer_length-4

        self.src.emit("push_buffer",buff)
        self.written.append(to_write)
        self.protect=False

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
            while p.protect: pass
            p.push_data()
        

except KeyboardInterrupt:
    p.quit()


