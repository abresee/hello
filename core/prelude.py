from core_py import VorbisPlayer, WaveSynth, Note
import core_py

p = VorbisPlayer("test.ogg")

wg = WaveSynth()

note_length = core_py.sample_rate//8
note_intensity = core_py.max_intensity//4
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
    for i,v in enumerate(scale_degrees):
        notes.append(
            Note(v,octave,note_intensity,note_s.__next__(),note_length))

note_count = len(notes)
scale_degrees = [12, 7, 4, 2]

for octave in reversed(range(octave_count)):
    for i,v in enumerate(scale_degrees):
        notes.append(
            Note(v,octave,note_intensity,note_s.__next__(),note_length)) 

for note in notes:
    wg.add_note(note) 

p.add_instrument(wg)
p.play()
