cdef extern from "player.h":
    cdef cppclass Player:
        Player()
        void _d_Player()
        void print_gst_version()
        void play(char* track_name)
        void play_sample(char* sample_name)

cdef class PyPlayer:
    cdef Player *thisptr
    def __cinit__(self):
        self.thisptr = new Player()
    def __dealloc__(self):
        if self.thisptr is not NULL:
            self.thisptr._d_Player()
    def play(self, track_name):
        self.thisptr.play(track_name)
    def play_sample(self, sample_name):
        self.thisptr.play_sample(sample_name)
    def print_version(self):
        self.thisptr.print_gst_version()
