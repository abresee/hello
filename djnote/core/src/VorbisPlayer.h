#ifndef VORBISPLAYER_H
#define VORBISPLAYER_H 
#include "Player.h"
class VorbisPlayer : public Player {
    GstElement * vorbisencoder_;
    GstElement * oggmuxer_;
    std::string where_;
public:
    VorbisPlayer(const std::string output_name);    
    const std::string& where();

};
#endif /* VORBISPLAYER_H */
