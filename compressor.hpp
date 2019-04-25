#include "my_std.hpp"
#include "helpers.hpp"
#include "soundobjects.hpp"

void compress_sound(SoundBuffer& s) {
    let max = *max_element(ITERATE(s.buffer));
    for (int i = 0; i < s.buffer.size(); i++) {
        s.buffer[i] = sgn(s.buffer.at(i)) * sqrt(abs(s.buffer.at(i)) / max);
    }
}
