#!/usr/bin/python
from gi.repository import Gtk
import player
class MyWindow(Gtk.Window):
    def __init__(self):
        Gtk.Window.__init__(self, title="Hello World")

        self.box = Gtk.Box(spacing=6)
        self.add(self.box)

        self.button1 = Gtk.Button(label="clip 1")
        self.button1.connect("clicked", self.on_button1_clicked)
        self.box.pack_start(self.button1, True, True, 0)

        self.button2 = Gtk.Button(label="clip 2")
        self.button2.connect("clicked", self.on_button2_clicked)
        self.box.pack_start(self.button2, True, True, 0)

        self.master_slider = Gtk.Scale()
        self.master_slider.set_range(0.0, 2.0)
        self.master_slider.set_properties(orientation=1, inverted=True)
        self.master_slider.connect("value-changed", self.on_slider_changed)
        self.box.pack_start(self.master_slider, True, True, 0)

        self.track_1 = Gtk.Scale()
        self.track_1.set_range(0.0, 2.0)
        self.track_1.set_properties(orientation=1, inverted=True)
        self.track_1.connect("value-changed", self.on_slider_changed)
        self.box.pack_start(self.track_1, True, True, 0)

        self.track_2 = Gtk.Scale()
        self.track_2.set_range(0.0, 2.0)
        self.track_2.set_properties(orientation=1, inverted=True)
        self.track_2.connect("value-changed", self.on_slider_changed)
        self.box.pack_start(self.track_2, True, True, 0)

        self.player = player.PyPlayer()
    def run_main(self):
        self.player.run()
    def on_button1_clicked(self, widget):
        self.player.play_sample("test.ogg")
    def on_button2_clicked(self, widget):
        self.player.play_sample("test2.ogg")
    def close_app(self, _,__):
        self.player.destruct()
        Gtk.main_quit()
    def on_slider_changed(self, slider):
        value = slider.get_value()
        if (slider == self.master_slider):
            self.player.set_volume(value)
            print "set master volume"
        if (slider == self.track_1):
            self.player.set_volume_track1(value)
            print "set track1 volume"
        if (slider == self.track_2):
            self.player.set_volume_track2(value)
            print "set track2 volume"

        
        

win = MyWindow()
win.set_default_size(500, 200);

print dir(win.master_slider.props) #use this to see available properties on a widget
win.connect("delete-event", win.close_app)
win.show_all()
Gtk.main()
#win.run_main()
