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

win = MyWindow()
win.connect("delete-event", win.close_app)
win.show_all()
Gtk.main()
#win.run_main()
