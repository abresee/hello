#include "Player.h"

int main(int argc, char ** argv)
{
    LocalPlayer p;

    spInstrument wg = spInstrument(new WaveGenerator(std::sin));
    wg.add_note(Note(0,Player::max_volume/4, 0, Player::sample_rate*2);
    p.add_instrument(wg);
    p.play();
}
