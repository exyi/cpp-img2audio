#pragma once
#include "my_std.hpp"
#include "helpers.hpp"

struct Point {
    f32 x, y;
    Point(f32 x, f32 y) : x{x}, y{y} {}

    Point scale(f32 coef_x, f32 coef_y) const {
        return Point(x * coef_x, y * coef_y);
    }
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

ostream& operator<<(ostream& s, const Point& p) {
    return s << "[" << p.x << ", " << p.y << "]";
}

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

    Line scale(f32 x, f32 y) const {
        return Line(from.scale(x, y), to.scale(x, y));
    }

    RECORD_BOILERPLATE(Line);
};

Line operator+(const Line& a, const Point& b) { return Line(a.from + b, a.to + b); }
Line operator-(const Line& a, const Point& b) { return Line(a.from - b, a.to - b); }
ostream& operator<<(ostream& s, const Line& l) {
    return s << l.from << " -> " << l.to;
}
