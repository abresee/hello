from core_py import LocalPlayer, WaveGenerator, Note
import core_py

p = LocalPlayer()

wg = WaveGenerator()

note_length = core_py.sample_rate//8
note_intensity = core_py.max_intensity//4
print(type(note_length),type(note_intensity))
notes = [Note(i, -5, note_intensity, note_length*i, note_length*(i+1)) for i in range(140)]

for note in notes:
    wg.add_note(note)

p.add_instrument(wg)
p.play()
