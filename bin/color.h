#ifndef COLOR_H
#define COLOR_H


#include <random>
#ifdef QGLOBAL_H
#include <qcolor.h>
#endif
#include <string>
#include <ostream>

struct Color {
    float r, g, b, a;
    Color(float r_, float g_, float b_, float a_ = 1.0) : r(r_), g(g_), b(b_), a(a_){
        if(r > 1 || g > 1 || b > 1 || a > 1){
            r /= 255;
            g /= 255;
            b /= 255;
            a = a == 1.0 ? a : a / 255;
        }
    }
    Color() : r(0), g(0), b(0), a(1){};
    Color(const std::string &hex){
        std::string rr = hex.substr(1, 2);
        std::string gg = hex.substr(3, 2);
        std::string bb = hex.substr(5, 2);
        r = ((float)std::stoi(rr, 0, 16) / 255);
        g = ((float)std::stoi(gg, 0, 16) / 255);
        b = ((float)std::stoi(bb, 0, 16) / 255);
        a = 1.0;
    }
    const static Color randomColor(){
        std::random_device device;
        std::mt19937 mt(device());
        std::uniform_int_distribution<> dist(0, 255);
        return Color(dist(mt), dist(mt), dist(mt));
    }
    explicit operator const std::string() const { return std::string('(' + std::to_string(r) + ", " + std::to_string(g) + ", " + std::to_string(b) + ", " + std::to_string(a) + ')');}

    #ifdef QGLOBAL_H
    explicit operator const QColor() const { return QColor::fromRgbF(r, g, b, a); }
    #endif
};
static std::ostream &operator<<(std::ostream &os, const Color &c) { os << '(' << c.r << ", " << c.g << ", " << c.b << ", " << c.a << ')'; return os; }


#endif
