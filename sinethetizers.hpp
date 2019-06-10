#include "my_std.hpp"
#include "helpers.hpp"
#include "fft.hpp"
#include "geometry.hpp"
#include "soundobjects.hpp"

auto make_window(f32 length) {
    let window_function = [=](f32 pos) -> f32 {
        // cerr << pos << " of " << length << "\n";
        if (pos < 0.03)
            return sin(pos / 0.03 * M_PI / 2);
        else if (pos > length - 0.03)
            return -sin((length - pos) / 0.03 * M_PI / 2);
        else
            return 1.0;
    };
    return window_function;
}

SoundBuffer sinewave(f32 freq, i32 samplerate, f32 length, f32 freq_increase) {
    let window_function = make_window(length);

    let buffer_size = (i32)(samplerate * length);
    if (buffer_size <= 0) throw runtime_error("buffer size must be positive");
    if (freq < 0) throw runtime_error("frequency must be positive");
    if (freq + freq_increase < 0) throw runtime_error("frequency must be positive for the entire duration of sinewave");

    vector<f32> buffer;
    let freq_increment = freq_increase / buffer_size / 2.0;

    for (i32 i = 0; i < buffer_size; ++i) // Single cycle
    {
        f64 tmp = sin(((freq + ((f64)i * freq_increment)) * (2 * M_PI) * i) / (f64)samplerate) * 0.7 * window_function(i / (f32)samplerate);
        buffer.emplace_back(tmp);
    }

    // cerr << "Buffer of size " << buffer.size() << ", expected " << buffer_size << "\n";

    return SoundBuffer(move(buffer));
}

template<typename F>
SoundBuffer generate_wavespectrum_(f32 min, f32 max, F frequency_amplitude, u32 samplerate, usize samplecount) {
    // cerr <<"Wavespectrum " << min << "-" << max <<"\n";

    let window_function = make_window((f32)samplecount * 2 / samplerate);

    let into_freq = [=](i32 index) {
        // let period = samplecount - index;
        // let freq = (f32)samplerate * 70 / period;
        let freq = samplerate / 2 * (f32)index / samplecount;
        return freq;
    };

    let get_ampl = [&](i32 index) {
        // if (index == 500) return 1;
        // else return 0;
        let f = into_freq(index);
        if (f < min || f > max) return 0.0f;
        return (f32)frequency_amplitude(f);
    };

    // cerr << "wavespectrum: " << samplerate << " " << samplecount << "\n";
    let fft_freqs = CREATE_VEC(f64, i, samplecount, get_ampl(i) * 10);

    let raw_result = ifft_real_random_phase(fft_freqs);
    MUTABLE_LET_BECAUSE_CPP_COMMITTEE_ARE_IDIOTS_AND_WE_CANT_MOVE_CONST_VARS
        f32_result = vec_mapi(raw_result, [&](let i, let a) { return (f32)a * window_function(i / (f32)samplerate); });
    return SoundBuffer(move(f32_result));
}

template<typename F>
SoundBuffer generate_wavespectrum(f32 min, f32 max, F frequency_amplitude, i32 samplerate) {
    let fragment_len = 1024;
    MUTABLE_LET_BECAUSE_CPP_COMMITTEE_ARE_IDIOTS_AND_WE_CANT_MOVE_CONST_VARS fragment =
        generate_wavespectrum_(min, max, frequency_amplitude, samplerate, fragment_len);

    return fragment;

    // let count = (i32)(length * samplerate / fragment_len);
    // cerr << "Spectrum: iterate " << count <<"\n";
    // MUTABLE_LET_BECAUSE_CPP_COMMITTEE_ARE_IDIOTS_AND_WE_CANT_MOVE_CONST_VARS
        // fragments = vec_init(count, [&](let i) {
        //                 return fragment.move_start((f32)i * fragment_len / samplerate);
        //             });
    //     fragments = make_vector(move(fragment));
    // return mix_into_one(MixedSounds(move(fragments)), samplerate);
}

struct SoundConfig {
    f32 length;
    i32 sample_rate;
    f32 max_frequency;
    f32 min_frequency;
    SoundConfig(f32 length, i32 sample_rate, f32 max_frequency, f32 min_frequency):
        length{length}, sample_rate{sample_rate}, max_frequency{max_frequency}, min_frequency{min_frequency} {}

    RECORD_BOILERPLATE(SoundConfig);
};

SoundFragment draw_vert_line(const Line& normLine, const SoundConfig& sconfig) {
    // cerr << "Vert line\n";
    let sound_start = sconfig.length * normLine.from.x;
    let freq1 = normLine.from.y * (sconfig.max_frequency - sconfig.min_frequency) + sconfig.min_frequency;
    let freq2 = normLine.to.y * (sconfig.max_frequency - sconfig.min_frequency) + sconfig.min_frequency;
    let min_f = min(freq1, freq2);
    let max_f = max(freq1, freq2);
    return
        generate_wavespectrum(min_f, max_f, [](let&) { return 1; }, sconfig.sample_rate)
        .to_fragment(sound_start)
        .normalize_volume(3.0);
}

SoundFragment draw_line(const Line& line, const Rectangle& boundaries, const SoundConfig& sconfig) {
    let boundary_size = boundaries.right_upper - boundaries.left_bottom;
    let normLine = (line - boundaries.left_bottom).map([&](let& p) { return p.norm_coords(boundary_size); });

    if (normLine.from.x >= normLine.to.x + 0.001) throw runtime_error("lines must always go from left to right");

    if (normLine.from.x >= normLine.to.x - 0.001 / sconfig.length) {
        return draw_vert_line(normLine, sconfig);
    }

    let sound_start = sconfig.length * normLine.from.x;
    let sound_end = sconfig.length * normLine.to.x;
    let f_range = sconfig.max_frequency - sconfig.min_frequency;
    let f_start = f_range * normLine.from.y + sconfig.min_frequency;
    let f_end = f_range * normLine.to.y + sconfig.min_frequency;

    return sinewave(f_start, sconfig.sample_rate, sound_end - sound_start, f_end - f_start)
           .to_fragment(sound_start);
}

MixedSounds draw_lines(const vector<Line>& lines, const SoundConfig& sconfig) {
    assert(lines.size() >= 1);

    auto bounding_box = make_unique<Rectangle>(lines[0].bounding_box());
    for (let& l : lines) {
        bounding_box = make_unique<Rectangle>(Rectangle::outer_box(*bounding_box, l.bounding_box()));
        //             ^ in C++, they have neither sane functional primitives (iterators+reduce) nor sane imperative primitives (assignments)
        //               fortunatelly they have a ton of other insane features and unique_ptr seems to help in this case...
        //               this usage of unique_ptr is simply a hack to allow move-assignment
    }

    vector<SoundFragment> fragments;

    for (let& l : lines) {
        fragments.emplace_back(
            draw_line(l, *bounding_box, sconfig)
        );
    }

    return MixedSounds(move(fragments));
}
