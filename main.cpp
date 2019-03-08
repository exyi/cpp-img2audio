#include "file_output.cpp"
#include "sinethetizers.cpp"

int main() {
    let samplerate = 48000;
    let sconfig = SoundConfig(5, samplerate, 50, 5000);
    let s = draw_lines(make_vector(
        Line(Point(0, 0), Point(1, 1)),
        Line(Point(0, 1), Point(1, 0)),
        Line(Point(0.3, 0.5), Point(0.7, 0.5))
    ), sconfig);
    // vector<float> emptyBuffer(samplerate * 20, 0.0);
    let b = mix_into_one(s, samplerate);
    save_waveform("outputfile.wav", b.buffer, samplerate);
    return 0;
}
