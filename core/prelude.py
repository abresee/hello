from note_core import VorbisPlayer, WaveSynth, Note, Beat, Sample
import note_core 

p = VorbisPlayer("prelude_py.ogg")

wg = WaveSynth()

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
    wg.add_note(note) 

p.add_instrument(wg)
p.play()
