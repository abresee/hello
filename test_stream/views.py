# Create your views here.
from django.http import HttpResponse
from django.template import RequestContext
from django.shortcuts import render_to_response
from os.path import basename
from core import Player, WaveSynth, Offset, Beat, Sample, Note  

remember={}
def make_test_player():
    p = Player(Player.BackendType.vorbis,Offset(44100),220,"prelude_py.ogg")

    ws = WaveSynth(p.sample_rate(), p.freq_reference(),"wavesynth_dump.log")

    note_length = Beat(1,2) 
    note_intensity = Sample.max_intensity()/2
    scale_degrees = [0, 2, 4, 7]
    octave_count = 4
    notes = []

    def note_start(note_length):
        count = 0
        while True:
            yield count*note_length
            count+=1

    note_s = note_start(note_length)

    for octave in range(octave_count):
        for v in scale_degrees:
            notes.append(
                Note(v,octave,note_intensity,note_s.__next__(),note_length))

    note_count = len(notes)
    scale_degrees = [12, 7, 4, 2]

    for octave in reversed(range(octave_count)):
        for v in scale_degrees:
            notes.append(
                Note(v,octave,note_intensity,note_s.__next__(),note_length)) 

    for note in notes:
        n = models.Note(note)
        n.save()
        ws.add_note(note) 

    p.add_instrument(ws)
    p.play()
    return p


def index(request):
    remember["last"]=make_test_player()
    return render_to_response(
        "test_stream/test_stream.html",
        context_instance=RequestContext(request))

def is_ready(request):
    p=remember["last"]
    path=basename(p.wait_until_ready())
    return HttpResponse("/static/test_stream/"+path)

