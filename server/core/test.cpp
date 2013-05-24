#include <cmath>
#include "Player.h"
#include "Instrument.h"
#include "WaveGenerator.h"

int main(int argc, char ** argv)
{
    boost::shared_ptr<Player> p(new LocalPlayer());
    std::function<double(double)> sin= static_cast<double(*)(double)>(std::sin);
    InstrumentHandle wg = InstrumentHandle(new WaveGenerator(sin));
    Note n(0,Config::max_volume/4,0,0,Config::sample_rate*2);
    wg->add_note(n);
    std::cout<<wg->stream_end()<<std::endl;
    p->add_instrument(wg);
    p->play();
}
