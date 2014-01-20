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

setup(ext_modules=[Extension("cygst", ["player.pyx", "player2.cpp"], language="c++",include_dirs=cflags, libraries=libs,)],
      cmdclass = {'build_ext': build_ext})

#python setup.py build_ext -i

#python
# >>>import cygst
# >>>x = cygst.PyPlayer()
# >>>x.print_version()
# This Program is linked against GStreamer 1.2.0
# >>>x.play("test.ogg")
