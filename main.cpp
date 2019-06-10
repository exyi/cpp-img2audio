#include "file_output.hpp"
#include "sinethetizers.hpp"
#include "text2lines.hpp"
#include "compressor.hpp"

template<typename T>
void fucking_print_vector_as_this_shit_cant_do_it_on_its_own(ostream& o, const vector<T>& v) {
    o << '[';
    for (let& i : v) {
        o << i;
        o << ", ";
    }
    o << ']';
}

int main() {
    srand(time(NULL));

    let output = "outputfile.wav";

    let length_secs = 10.0;
    let samplerate = 48000;
    let max_frequency = 5000.0;
    let min_frequency = 500.0;
    let sconfig = SoundConfig(
        length_secs,
        samplerate,
        max_frequency,
        min_frequency
    );


    // let lines = make_vector(
    //     Line(Point(0, 0), Point(1, 1))
    //     // Line(Point(0.7, 0), Point(1, 1)),
    //     // Line(Point(0, 1), Point(0.3, 0)),
    //     // Line(Point(0.5, 0.4), Point(0.5, 0.6))
    //     // Line(Point(0.3, 0.5), Point(0.7, 0.5))
    // );

    let lines = strings2lines(make_vector<string_view>("nejaky", "testovaci", "vstup"), 1);

    // fucking_print_vector_as_this_shit_cant_do_it_on_its_own(cerr, lines);

    let sounds = draw_lines(lines, sconfig);
    auto output_buffer = mix_into_one(sounds, samplerate);
    // compress_sound(bf);
    save_waveform(output, output_buffer.buffer, samplerate);
    return 0;
}
