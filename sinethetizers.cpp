#include "my_std.hpp"
#include "helpers.hpp"

struct SoundFragment;

struct SoundBuffer {
    vector<f32> buffer;

    explicit SoundBuffer(vector<f32>&& buffer) : buffer{buffer} {}

    SoundFragment to_fragment(f32 start_time = 0.0, f32 volume = 1.0);

    RECORD_BOILERPLATE(SoundBuffer)
};

struct SoundFragment {
    shared_ptr<SoundBuffer> buffer;
    f32 start_time;
    f32 volume;

    SoundFragment(shared_ptr<SoundBuffer> buffer, f32 start_time, f32 volume = 1.0) : buffer{move(buffer)}, start_time{start_time}, volume{volume} {
        if (this->buffer->buffer.size() == 0) throw "WTF, this throw always SegFaults... Whatever, it guards anyway.";
        //     ^ explicit use of this pointer since buffer is the parameter that is destroyed by move. Thank you C++ for your help in programming, I appreciate the intuitive error message (SIGSEGV)
        //                            ^ yes, because WTF
    }

    SoundFragment(SoundBuffer buffer, f32 start_time, f32 volume = 1.0) : buffer{make_shared<SoundBuffer>(move(buffer))}, start_time{start_time}, volume{volume} {}

    RECORD_BOILERPLATE(SoundFragment)
};
SoundFragment SoundBuffer::to_fragment(f32 start_time, f32 volume) { return SoundFragment(make_shared<SoundBuffer>(move(this->buffer)), start_time, volume); }

struct MixedSounds {
    /// Sounds ordered by start_time
    vector<SoundFragment> sounds;
    explicit MixedSounds(vector<SoundFragment>&& sounds) : sounds{move(sounds)} {
        std::sort(ITERATE(this->sounds), COMPARE_LT_BY(it.start_time));
    }
    explicit MixedSounds(SoundFragment&& sound) : sounds{make_vector(move(sound))} {}

    // static MixedSounds from(SoundFragment &&s) { return MixedSounds(vector<SoundFragment>{move(s)}); }
    // static MixedSounds from(vector<Sound>&& s) {
    //     vector<shared_ptr<Sound>> ss;
    //     for (auto& s_ : s)
    //         ss.emplace_back(make_shared<Sound>(move(s_)));

    //     return MixedSounds(move(ss));
    // }

    RECORD_BOILERPLATE(MixedSounds)
};

SoundBuffer sinewave(f32 freq, i32 samplerate, f32 length, f32 freq_increase) {
    let buffer_size = (i32)(samplerate * length);
    if (buffer_size <= 0) throw "";
    if (freq < 0) throw "";
    if (freq + freq_increase < 0) throw "";

    vector<f32> buffer;
    let freq_increment = freq_increase / buffer_size / 2.0;

    for (int i = 0; i < buffer_size; ++i) // Single cycle
    {
        double tmp = sin(((freq + ((f64)i * freq_increment)) * (2 * M_PI) * i) / (f64)samplerate) * 0.7;
        buffer.emplace_back(tmp);
    }

    // cerr << "Buffer of size " << buffer.size() << ", expected " << buffer_size << "\n";

    return SoundBuffer(move(buffer));
}

struct Point {
    f32 x, y;
    Point(f32 x, f32 y) : x{x}, y{y} {}

    Point norm_coords(const Point& boundary) const { return Point(x / boundary.x, y / boundary.y); }
    static Point min_coords(const Point& a, const Point& b) { return Point(min(a.x, b.x), min(a.y, b.y)); }
    static Point max_coords(const Point& a, const Point& b) { return Point(max(a.x, b.x), max(a.y, b.y)); }

    RECORD_BOILERPLATE(Point)
};
Point operator+(const Point& a, const Point& b) { return Point(a.x + b.x, a.y + b.y); }
Point operator-(const Point& a) { return Point(-a.x, -a.y); }
Point operator-(const Point& a, const Point& b) { return a + (-b); }
Point operator*(const Point& a, f32 coef) { return Point(a.x * coef, a.y * coef); }
Point operator/(const Point& a, f32 coef) { return Point(a.x / coef, a.y / coef); }

struct Rectangle {
    Point left_bottom, right_upper;
    Rectangle(Point left_bottom, Point right_upper) : left_bottom{move(left_bottom)}, right_upper{move(right_upper)} {}

    static Rectangle outer_box(const Rectangle& a, const Rectangle& b) {
        return Rectangle(Point::min_coords(a.left_bottom, b.left_bottom), Point::max_coords(a.right_upper, b.right_upper));
    }

    RECORD_BOILERPLATE(Rectangle);
};

struct Line {
    Point from, to;
    Line(Point from, Point to) : from{move(from)}, to{move(to)} {}

    template<typename Fn>
    Line map(Fn fn) const { return Line(fn(from), fn(to)); }
    Rectangle bounding_box() const { return Rectangle(from * 1, to * 1); }

    RECORD_BOILERPLATE(Line);
};

Line operator+(const Line& a, const Point& b) { return Line(a.from + b, a.to + b); }
Line operator-(const Line& a, const Point& b) { return Line(a.from - b, a.to - b); }

struct SoundConfig {
    f32 length;
    i32 sample_rate;
    f32 max_frequency;
    f32 min_frequency;
    SoundConfig(f32 length, i32 sample_rate, f32 max_frequency, f32 min_frequency):
        length{length}, sample_rate{sample_rate}, max_frequency{max_frequency}, min_frequency{min_frequency} {}

    RECORD_BOILERPLATE(SoundConfig);
};

SoundFragment draw_line(const Line& line, const Rectangle& boundaries, const SoundConfig& sconfig) {
    let boundary_size = boundaries.right_upper - boundaries.left_bottom;
    let normLine = (line - boundaries.left_bottom).map([&](let& p) { return p.norm_coords(boundary_size); });

    if (normLine.from.x > normLine.to.x) throw "";

    let sound_start = sconfig.length * normLine.from.x;
    let sound_end = sconfig.length * normLine.to.x;
    let f_range = sconfig.max_frequency - sconfig.min_frequency;
    let f_start = f_range * normLine.from.y + sconfig.min_frequency;
    let f_end = f_range * normLine.to.y + sconfig.min_frequency;

    return sinewave(f_start, sconfig.sample_rate, sound_end - sound_start, f_end - f_start)
           .to_fragment(sound_start);
}

MixedSounds draw_lines(vector<Line> lines, const SoundConfig& sconfig) {
    assert(lines.size() >= 1);

    auto bounding_box = make_unique<Rectangle>(lines[0].bounding_box());
    for (let& l : lines) {
        bounding_box = make_unique<Rectangle>(Rectangle::outer_box(*bounding_box, l.bounding_box()));
        //             ^ in C++, they have neither sane functional primitives (iterators+reduce) nor sane imperative primitives (assignment)
        //               fortunatelly they have a ton of other insane features and unique_ptr seems to work in this case...
    }

    vector<SoundFragment> fragments;

    for (let& l : lines) {
        fragments.emplace_back(
            draw_line(l, *bounding_box, sconfig)
        );
    }

    return MixedSounds(move(fragments));
}

SoundBuffer mix_into_one(const MixedSounds& buffers, const i32 sample_rate) {
    let end_time = [sample_rate](const SoundFragment& f) { return (u32)(f.start_time * sample_rate) + (u32)f.buffer->buffer.size(); };
    let comparer = COMPARE_GT_BY(end_time(buffers.sounds.at(it))); //[&](int left, int right) { return end_time(buffers.sounds.at(left)) > end_time(buffers.sounds.at(right));};
    auto q = priority_queue<int, vector<int>, decltype(comparer)>(comparer);
    f64 normal_volume = 0.0;
    f64 current_volume = 0.0;
    u32 global_end_time = 0;

    for (u32 i = 0; i < buffers.sounds.size(); i++) {
        let& f = buffers.sounds.at(i);
        let start = (u32)(f.start_time * sample_rate);
        // remove from the queue
        while (q.size() > 0 && end_time(buffers.sounds.at(q.top())) < start)
        {
            let& fragment = buffers.sounds.at(q.top());
            q.pop();
            current_volume -= fragment.volume;
        }
        current_volume += f.volume;
        normal_volume = max(normal_volume, current_volume);
        global_end_time = max(global_end_time, end_time(f));
        q.push(i);
    }

    vector<pair<u32, const SoundFragment&>*> current;
    //                                    ^ when it's not a pointer, the fucking C++ complains that you need copy constructor for vector.erase and pair does not have one
    u32 next_remove = 0;
    u32 next_index = 0;
    vector<f32> output_buffer;

    for (u32 si = 0; si < global_end_time; si++)
    {
        while (buffers.sounds.size() > next_index && buffers.sounds[next_index].start_time * sample_rate <= si)
        {
            current.emplace_back(new pair<u32, const SoundFragment&>{(u32)(buffers.sounds[next_index].start_time * sample_rate), buffers.sounds[next_index]});
            next_index++;
            next_remove = si;
        }
        while (next_remove == si && current.size() > 0)
        {
            let min = min_element(ITERATE(current), COMPARE_LT_BY(end_time(it->second)));
            if (end_time((**min).second) > si)
                next_remove = end_time((**min).second);
            else
            {
                let ptr = *min;
                current.erase(min, min + 1);
                delete ptr; // fuck exceptions, they not going to occur
            }
        }

        f32 mix = 0.0;

        for (let* c : current)
        {
            let& [start, fragment] = *c;
            mix += fragment.volume * fragment.buffer->buffer.at(si - start);
        }

        output_buffer.push_back(mix / normal_volume);
    }

    for (let x : current) delete x;

    return SoundBuffer(move(output_buffer));
}
