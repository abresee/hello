from core_py import LocalPlayer, WaveGenerator, Note
import core_py

p = LocalPlayer()

wg = WaveGenerator()

note_length = core_py.sample_rate//8
note_intensity = core_py.max_intensity//4
notes = [Note(i, -1, note_intensity, note_length*i, note_length*(i+1)) for i in range(50)]

for note in notes:
    wg.add_note(note)

print(type(wg))
p.add_instrument(wg)
p.play()