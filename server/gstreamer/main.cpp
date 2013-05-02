#include "player.h"

int main(int argc, char ** argv)
{
    Player p(&argc, &argv);
    p.play();
}
