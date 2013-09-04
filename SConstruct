# vim: set syntax=python
SConscript('core/SConscript', variant_dir='build', duplicate=0)
SConscript('test_stream/static/test_stream/SConscript')
env=DefaultEnvironment()
env.Install('.','build/core.so')
