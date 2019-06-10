#pragma once
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
        if (this->buffer->buffer.size() == 0) throw runtime_error("SoundBuffer must be non-empty");
        //     ^ explicit use of this pointer since buffer is the parameter that is destroyed by move. Thank you C++ for your help in programming, I appreciate the intuitive error message (SIGSEGV)
    }

    SoundFragment(SoundBuffer buffer, f32 start_time, f32 volume = 1.0) : buffer{make_shared<SoundBuffer>(move(buffer))}, start_time{start_time}, volume{volume} {}

    auto move_start(f32 offset) const {
        return SoundFragment(buffer, start_time + offset, volume);
    }

    auto normalize_volume(f32 target_max_volume) const {
        let max = *max_element(ITERATE(buffer->buffer));
        return SoundFragment(buffer, start_time, (1 / max) * target_max_volume);
    }

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

    auto add_start_pause(f32 start_time) const {
        assert(start_time >= 0);
        MUTABLE_LET_BECAUSE_CPP_COMMITTEE_ARE_IDIOTS_AND_WE_CANT_MOVE_CONST_VARS v = CREATE_VEC(SoundFragment, i, sounds.size(), sounds[i].move_start(start_time));
        // ^ KTERÝ KOKOT VYMYSLEL, ŽE KDYŽ UDĚLÁM move NA const PROMĚNNOU, TAK DOSTANU "no matching conversion for functional-style cast from 'typename std::remove_reference<const vector<SoundFragment, allocator<SoundFragment> > &>::type' (aka 'const std::vector<SoundFragment, std::allocator<SoundFragment> >') to 'MixedSounds'". TO FAKT NEJDE VYPINDAT NĚCO JAKO "can't move const variable"? JSEM PRVNÍ KDO TUTO KOKOTINU POUŽÍVÁ NEBO CO?
        // A KTERÝ ZMRD VYMYSLEL, ŽE PROGRAMOVÁNÍ V TAKOVÉM VYPÍČENÉM BUDE (PRAKTICKY) POVINNÉ VE ŠKOLE? NÁM JAKO CHTĚJÍ UKÁZAT, ŽE SI MÁME VÁŽIT ČISTOTY PHPČKA A SNADNÉHO POUŽITÍ SHELLU?

        // JO A PROČ TO DO PRDELE NEJDE move-NOUT? JASNĚ, CELÉ TO NARHOVAL DEBIL A PROTO TEN MOVE KONSTRUKTOR MODIFIKUJE PŮVODNÍ PROMĚNNOU NA TO ABY OBSAHOVALA NĚJAKÉ INITIAL HODNOTY, ALE PROČ PROBOHA?
        return MixedSounds(move(v));
    }

    // static MixedSounds from(SoundFragment &&s) { return MixedSounds(vector<SoundFragment>{move(s)}); }
    // static MixedSounds from(vector<Sound>&& s) {
    //     vector<shared_ptr<Sound>> ss;
    //     for (auto& s_ : s)
    //         ss.emplace_back(make_shared<Sound>(move(s_)));

    //     return MixedSounds(move(ss));
    // }

    RECORD_BOILERPLATE(MixedSounds)
};


SoundBuffer mix_into_one(const MixedSounds& buffers, const i32 sample_rate) {
    let end_time = [sample_rate](const SoundFragment& f) { return (u32)(f.start_time * sample_rate) + (u32)f.buffer->buffer.size(); };
    let max_volumes = vec_map(buffers.sounds, [](let& b) { return *max_element(ITERATE(b.buffer->buffer)) * b.volume; });
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
            current_volume -= max_volumes.at(q.top());
            q.pop();
        }
        current_volume += max_volumes.at(i);
        normal_volume = max(normal_volume, current_volume);
        global_end_time = max(global_end_time, end_time(f));
        q.push(i);
    }

    vector<pair<u32, const SoundFragment&>*> current;
    //                                    ^ when it's not a pointer, the fucking C++ complains that you need copy constructor for vector.erase and the pair does not have one
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
                delete ptr; // fuck exceptions, they're not going to occur
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
    for (let x : current) delete x; // fuck exceptions, they're not going to occur

    return SoundBuffer(move(output_buffer));
}
