#include "my_std.hpp"
#include "helpers.hpp"
#include "geometry.hpp"

vector<Line> single_letter2lines(char c) {
    c = toupper(c);
    if (c == 'A') {
        return make_vector(
            Line(Point(0, 0), Point(0.5, 1)),
            Line(Point(0.5, 1), Point(1, 0)),
            Line(Point(0.2, 1.0/3), Point(0.8, 1.0/3))
        );
    } else if (c == 'B') {
        return make_vector(
            Line(Point(0.1, 0), Point(0.1, 1)),
            Line(Point(0.1, 0), Point(0.9, 0.20)),
            Line(Point(0.9, 0.20), Point(0.9, 0.40)),
            Line(Point(0.1, 0.6), Point(0.9, 0.40)),
            Line(Point(0.1, 0.6), Point(0.7, 0.75)),
            Line(Point(0.7, 0.75), Point(0.7, 0.85)),
            Line(Point(0.1, 1), Point(0.7, 0.85))
        );
    } else if (c == 'C') {
        return make_vector(
            Line(Point(0.3, 0), Point(0.6, 0)),
            Line(Point(0.6, 0), Point(0.9, 0.15)),
            Line(Point(0.3, 1), Point(0.6, 1)),
            Line(Point(0.6, 1), Point(0.9, 0.85)),
            Line(Point(0.1, 0.15), Point(0.1, 0.85)),
            Line(Point(0.1, 0.15), Point(0.3, 0)),
            Line(Point(0.1, 0.85), Point(0.3, 1))
        );
    } else if (c == 'D') {
        return make_vector(
            Line(Point(0.1, 0), Point(0.6, 0)),
            Line(Point(0.6, 0), Point(0.9, 0.15)),
            Line(Point(0.1, 1), Point(0.6, 1)),
            Line(Point(0.6, 1), Point(0.9, 0.85)),
            Line(Point(0.1, 0), Point(0.1, 1)),
            Line(Point(0.9, 0.15), Point(0.9, 0.85))
        );
    } else if (c == 'E') {
        return make_vector(
            Line(Point(0.1, 0), Point(0.1, 1)),
            Line(Point(0.1, 0), Point(0.9, 0)),
            Line(Point(0.1, 0.5), Point(0.6, 0.5)),
            Line(Point(0.1, 1), Point(0.9, 1))
        );
    } else if (c == 'F') {
        return make_vector(
            Line(Point(0.1, 0), Point(0.1, 1)),
            Line(Point(0.1, 0.5), Point(0.6, 0.5)),
            Line(Point(0.1, 1), Point(0.9, 1))
        );
    } else if (c == 'G') {
        return make_vector(
            Line(Point(0.3, 0), Point(0.6, 0)),
            Line(Point(0.6, 0), Point(0.9, 0.3)),
            Line(Point(0.6, 0.3), Point(0.9, 0.3)),
            Line(Point(0.3, 1), Point(0.6, 1)),
            Line(Point(0.6, 1), Point(0.9, 0.85)),
            Line(Point(0.1, 0.15), Point(0.1, 0.85)),
            Line(Point(0.1, 0.15), Point(0.3, 0)),
            Line(Point(0.1, 0.85), Point(0.3, 1))
        );
    } else if (c == 'H') {
        return make_vector(
            Line(Point(0.1, 0), Point(0.1, 1)),
            Line(Point(0.9, 0), Point(0.9, 1)),
            Line(Point(0.1, 0.5), Point(0.9, 0.5))
        );
    } else if (c == 'I') {
        return make_vector(
            Line(Point(0.5, 0), Point(0.5, 1)),
            Line(Point(0.4, 0), Point(0.7, 0)),
            Line(Point(0.4, 1), Point(0.7, 1))
        );
    } else if (c == 'J') {
        return make_vector(
            Line(Point(0.7, 0.15), Point(0.7, 1)),
            Line(Point(0.5, 0), Point(0.7, 0.15)),
            Line(Point(0.4, 0), Point(0.5, 0)),
            Line(Point(0.2, 0.3), Point(0.4, 0))
        );
    } else if (c == 'K') {
        return make_vector(
            Line(Point(0.1, 0), Point(0.1, 1)),
            Line(Point(0.1, 0.5), Point(0.9, 0)),
            Line(Point(0.1, 0.5), Point(0.9, 1))
        );
    } else if (c == 'L') {
        return make_vector(
            Line(Point(0.1, 0), Point(0.1, 1)),
            Line(Point(0.1, 0), Point(0.9, 0))
        );
    } else if (c == 'M') {
        return make_vector(
            Line(Point(0, 0), Point(0, 1)),
            Line(Point(1, 0), Point(1, 1)),
            Line(Point(0, 1), Point(0.5, 0.7)),
            Line(Point(0.5, 0.7), Point(1, 1))
        );
    } else if (c == 'N') {
        return make_vector(
            Line(Point(0.1, 0), Point(0.1, 1)),
            Line(Point(0.9, 0), Point(0.9, 1)),
            Line(Point(0.1, 1), Point(0.9, 0))
        );
    } else if (c == 'O') {
        return make_vector(
            Line(Point(0.4, 0), Point(0.6, 0)),
            Line(Point(0.6, 0), Point(0.9, 0.15)),
            Line(Point(0.4, 1), Point(0.6, 1)),
            Line(Point(0.6, 1), Point(0.9, 0.85)),
            Line(Point(0.1, 0.15), Point(0.1, 0.85)),
            Line(Point(0.1, 0.15), Point(0.4, 0)),
            Line(Point(0.1, 0.85), Point(0.4, 1)),
            Line(Point(0.9, 0.15), Point(0.9, 0.85))
        );
    } else if (c == 'P') {
        return make_vector(
            Line(Point(0.1, 0), Point(0.1, 1)),
            Line(Point(0.1, 0.4), Point(0.6, 0.4)),
            Line(Point(0.6, 0.4), Point(0.9, 0.5)),
            Line(Point(0.1, 1), Point(0.6, 1)),
            Line(Point(0.6, 1), Point(0.9, 0.85)),
            Line(Point(0.9, 0.5), Point(0.9, 0.85))
        );
    } else if (c == 'Q') {
        return make_vector(
            Line(Point(0.4, 0), Point(0.6, 0)),
            Line(Point(0.6, 0), Point(0.9, 0.15)),
            Line(Point(0.4, 1), Point(0.6, 1)),
            Line(Point(0.6, 1), Point(0.9, 0.85)),
            Line(Point(0.1, 0.15), Point(0.1, 0.85)),
            Line(Point(0.1, 0.15), Point(0.4, 0)),
            Line(Point(0.1, 0.85), Point(0.4, 1)),
            Line(Point(0.9, 0.15), Point(0.9, 0.85)),
            Line(Point(0.6, 0.15), Point(1, 0))
        );
    } else if (c == 'R') {
        return make_vector(
            Line(Point(0.1, 0), Point(0.1, 1)),
            Line(Point(0.1, 0.4), Point(0.6, 0.4)),
            Line(Point(0.6, 0.4), Point(0.9, 0.5)),
            Line(Point(0.1, 1), Point(0.6, 1)),
            Line(Point(0.6, 1), Point(0.9, 0.85)),
            Line(Point(0.9, 0.5), Point(0.9, 0.85)),
            Line(Point(0.5, 0.4), Point(0.9, 0))
        );
    } else if (c == 'S') {
        return make_vector(
            Line(Point(0.7, 1), Point(1, 0.7)),
            Line(Point(0.3, 1), Point(0.7, 1)),
            Line(Point(0.1, 0.7), Point(0.3, 1)),
            Line(Point(0.1, 0.6), Point(0.1, 0.7)),
            Line(Point(0.1, 0.6), Point(0.9, 0.4)),
            Line(Point(0.9, 0.3), Point(0.9, 0.4)),
            Line(Point(0.7, 0), Point(0.9, 0.3)),
            Line(Point(0.3, 0), Point(0.7, 0)),
            Line(Point(0, 0.3), Point(0.3, 0))
        );
    } else if (c == 'T') {
        return make_vector(
            Line(Point(0.5, 0), Point(0.5, 1)),
            Line(Point(0.1, 1), Point(0.9, 1))
        );
    } else if (c == 'U') {
        return make_vector(
            Line(Point(0.1, 1), Point(0.1, 0.3)),
            Line(Point(0.9, 1), Point(0.9, 0.3)),
            Line(Point(0.1, 0.3), Point(0.3, 0.0)),
            Line(Point(0.3, 0.0), Point(0.7, 0.0)),
            Line(Point(0.7, 0.0), Point(0.9, 0.3))
        );
    } else if (c == 'V') {
        return make_vector(
            Line(Point(0.1, 1), Point(0.5, 0)),
            Line(Point(0.5, 0), Point(0.9, 1))
        );
    } else if (c == 'W') {
        return make_vector(
            Line(Point(0, 1), Point(0.3, 0)),
            Line(Point(0.3, 0), Point(0.5, 0.8)),
            Line(Point(0.5, 0.8), Point(0.7, 0)),
            Line(Point(0.7, 0), Point(1, 1))
        );
    } else if (c == 'X') {
        return make_vector(
            Line(Point(0.1, 0), Point(0.9, 1)),
            Line(Point(0.1, 1), Point(0.9, 0))
        );
    } else if (c == 'Y') {
        return make_vector(
            Line(Point(0.5, 0), Point(0.5, 0.5)),
            Line(Point(0.1, 1), Point(0.5, 0.5)),
            Line(Point(0.5, 0.5), Point(0.9, 1))
        );
    } else if (c == 'Z') {
        return make_vector(
            Line(Point(0.1, 0), Point(0.9, 1)),
            Line(Point(0.1, 0), Point(0.9, 0)),
            Line(Point(0.1, 1), Point(0.9, 1))
        );
    }

    else if (c == ' ') {
        return vector<Line>();
    } else if (c == ',') {
        return make_vector(
            Line(Point(0.1, 0.1), Point(0, -0.1))
        );
    } else if (c == '.') {
        return make_vector(
            Line(Point(0, 0), Point(0.1, 0))
        );
    } else throw runtime_error("Not supported character");
}

vector<Line> string2lines(const string_view& str, f32 size) {
    vector<Line> result;

    for (usize i = 0; i < str.length(); i++) {
        let local_res = single_letter2lines(str[i]);
        for (let& l : local_res) {
            result.emplace_back(
                l.scale(size * 0.8, size) + Point(i * size, 0) + Point(0.1, 0)
            );
        }
    }
    return result;
}

vector<Line> strings2lines(const vector<string_view>& str, f32 size) {
    vector<Line> result;

    for (usize i = 0; i < str.size(); i++) {
        let local_res = string2lines(str.at(i), size);
        for (let& l : local_res) {
            result.emplace_back(
                l + Point(0, i * size * 1.4) + Point(0, size * 0.2)
            );
        }
    }
    return result;
}
