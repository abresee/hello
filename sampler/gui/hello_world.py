#!/usr/bin/python
from gi.repository import Gtk
import player

class MyWindow(Gtk.Window):
    def __init__(self):
        Gtk.Window.__init__(self, title="Hello World")

        self.button = Gtk.Button(label="Click Here")
        self.button.connect("clicked", self.on_button_clicked)
        self.add(self.button)
        self.player = player.PyPlayer()
    def on_button_clicked(self, widget):
        self.player.play("test2.ogg")

win = MyWindow()
win.connect("delete-event", Gtk.main_quit)
win.show_all()
Gtk.main()
