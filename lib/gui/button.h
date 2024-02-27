#ifndef BUTTON_H
#define BUTTON_H

#include "../../bin/point.h"
#include <string>

struct Button {
    Point pos;
    float width;
    float height;
    bool pressed;
    std::string text;
    Button(const Point &_pos, float _width, float _height, const std::string &_text)
        : pos(_pos), width(_width), height(_height), pressed(false), text(_text){}
        const bool areaContains(const Point &p) const {
            return p.x < pos.x + width
                && p.x > pos.x
                && p.y < pos.y + height
                && p.y > pos.y;
        }
        const bool operator==(const Button &o) const {
            return o.pos == pos
                && o.width == width
                && o.height == height;
        }
};

// So the button can be hashed into a map, I'd be happy to explain in detail
// if anyone wants, just not gonna type it all right here
namespace std {
    template<> struct hash<Button> {
        std::size_t operator()(const Button &b) const {
            std::size_t hsh = 0;
            std::size_t h1 = std::hash<Point>{}(b.pos);
            std::size_t h2 = std::hash<float>{}(b.width);
            std::size_t h3 = std::hash<float>{}(b.height);
            hsh ^= h1 + 0x9e3779b9 + (hsh << 6) + (hsh >> 2);
            hsh ^= h2 + 0x9e3779b9 + (hsh << 6) + (hsh >> 2);
            hsh ^= h3 + 0x9e3779b9 + (hsh << 6) + (hsh >> 2);
            return hsh;
        }
    };
}

#endif
