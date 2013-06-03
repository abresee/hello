#include "../Note.h"
#define BOOST_TEST_MODULE Note_test
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(note_test) {
    Sample intensity=Config::max_intensity/4;
    offset_t pos = 0;
    offset_t length = 1000;
    for(int pitch_class = -20; pitch_class < 20; ++pitch_class) {
        for(int octave = -20; octave < 20; ++octave) {
            Note note(pitch_class,octave,intensity,pos,length);
            BOOST_CHECK_EQUAL(
                    Config::pc_count*octave+pitch_class,
                    Config::pc_count*note.octave()+note.pitch_class());
            BOOST_CHECK_EQUAL(intensity,note.intensity());
            BOOST_CHECK_EQUAL(pos,note.position());
            BOOST_CHECK_EQUAL(length,note.length());
        }
    }
}
