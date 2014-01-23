from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
from subprocess import check_output
from re import search

lib = "gstreamer-1.0"

def fsh(arg):
    r=[]
    for i in str(check_output(arg)).split():
        m=search("-[Il]([\w\d/.-]+)",i)
        if m:
            r.append(m.group(1))
    return r

cflags = fsh(["pkg-config",lib,"--cflags"])
libs = fsh(["pkg-config",lib,"--libs"])
libs.append("asound")
libs.append("rtmidi")
libs.append("pthread")

setup(ext_modules=[Extension("cygst", ["player.pyx", "aux.cpp"], language="c++",include_dirs=cflags, libraries=libs,extra_compile_args=["-Wl,--no-as-needed")],
      cmdclass = {'build_ext': build_ext})

#python setup.py build_ext -i

#python
# >>>import cygst
# >>>x = cygst.PyPlayer()
# >>>x.print_version()
# This Program is linked against GStreamer 1.2.0
# >>>x.play("test.ogg")

#MIDI DIRECTIONS
'''
Launch the application from note/sampler/gui with the following command:

python hello_world.py

From here, the GUI should immediately be displayed, and the MIDI engine will be initialized:
    Reading MIDI input ... press <enter> to quit.

This application is set up right now such that if you have your MIDI device connected and recognized 
via the ALSA protocol, it should automatically be detected by Note.

Currently the application is set up to play the audio sample "test.ogg" on a note_on MIDI signal
from MIDI channel no. 1.
'''
