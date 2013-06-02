#include "Packet.h"
#include <algorithm>
Packet::Packet(const size_type size):
    packet_(size) {
}
Packet::Packet(const size_type size, Sample sample):
    packet_(size,sample) {
}

Packet::size_type Packet::size() const {
    return packet_.size(); 
}

void Packet::reserve(size_type size) {
    packet_.reserve(size);
}

Sample& Packet::operator[](const size_type pos) {
    return packet_[pos];
}

const Sample& Packet::operator[](const size_type pos) const {
    return packet_[pos];
}

Packet::iterator Packet::begin() {
    return packet_.begin();
}

Packet::const_iterator Packet::begin() const {
    return packet_.begin();
}

Packet::iterator Packet::end() {
    return packet_.end();
}

Packet::const_iterator Packet::end() const {
    return packet_.end();
}

Sample* Packet::data() {
    return packet_.data();    
}

Packet Packet::operator+(const Packet& other) const {
    Packet ret(std::max(size(),other.size()));
    for(size_type i = 0; i < std::min(size(),other.size()); ++i) {
        ret[i]=packet_[i]+other[i];
    }
    return ret;
}

Packet Packet::operator+=(const Packet& other) {
    for(size_type i=0; i < std::min(size(),other.size()); ++i) {
        (*this)[i]+=other[i];
    }
    return *this;
}

