#ifndef VORBISPLAYER_H
#define VORBISPLAYER_H 
#include "Player.h"
class VorbisPlayer : public Player {
    GstElement * vorbisencoder_;
    GstElement * oggmuxer_;
public:
    VorbisPlayer(const std::string output_name);    

};
#endif /* VORBISPLAYER_H */
