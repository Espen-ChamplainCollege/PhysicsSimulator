#ifndef SIZE_H
#define SIZE_H

#include <random>
#include <utility>
#ifdef QGLOBAL_H
#include <qsize.h>
#endif

struct Size {
    double width, height;
    Size(double w, double h) : width(w), height(h){};
    Size(const Size &s) : width(s.width), height(s.height){};
    Size(const std::pair<double, double> &p) : width(p.first), height(p.second){};
    #ifdef QGLOBAL_H
    Size(const QSize &s) : width(s.width()), height(s.height()){};
    Size(const QSizeF &s) : width(s.width()), height(s.height()){};
    #endif
    Size() : width(0), height(0){};

    const void transpose(){ std::swap(width, height); }
    const Size transposed() const { return Size(height, width); }
    const static Size randomSize(double minW, double maxW, double minH, double maxH){
        std::random_device device;
        std::mt19937 mt(device());
        std::uniform_real_distribution<> distW(minW, maxW);
        std::uniform_real_distribution<> distH(minH, maxH);
        return Size(distW(mt), distH(mt));
    }
    const bool operator==(const Size &o) const { return o.width == width && o.height == height; }
    const bool operator!=(const Size &o) const { return o.width != width && o.height != height; }
    // <> compares area
    const bool operator>(const Size &o) const { return (width * height) > (o.width * o.height); }
    const bool operator<(const Size &o) const { return (width * height) < (o.width * o.height); }
    const Size operator+(const double &v) const { return Size(width + v, height + v); }
    const Size operator-(const double &v) const { return Size(width - v, height - v); }
    const Size operator*(const double &v) const { return Size(width * v, height * v); }
    const Size operator/(const double &v) const { return Size(width / v, height / v); }
    const void operator+=(const double &v) { width += v; height += v; }
    const void operator-=(const double &v) { width -= v; height -= v; }
    const void operator*=(const double &v) { width *= v; height *= v; }
    const void operator/=(const double &v) { width /= v; height /= v; }
    const Size operator+(const Size &o) const { return Size(width + o.width, height + o.height); }
    const Size operator-(const Size &o) const { return Size(width - o.width, height - o.height); }
    const Size operator*(const Size &o) const { return Size(width * o.width, height * o.height); }
    const Size operator/(const Size &o) const { return Size(width / o.width, height / o.height); }
    const void operator+=(const Size &o) { width += o.width; height += o.height; }
    const void operator-=(const Size &o) { width -= o.width; height -= o.height; }
    const void operator*=(const Size &o) { width *= o.width; height *= o.height; }
    const void operator/=(const Size &o) { width /= o.width; height /= o.height; }
    explicit operator const std::string() const { return std::string('{' + std::to_string(width) + ", " + std::to_string(height) + '}'); }

    #ifdef QGLOBAL_H
    explicit operator const QSize() const { return QSize(width, height); }
    explicit operator const QSizeF() const {return QSizeF(width, height); }
    #endif
    

};
static std::ostream &operator<<(std::ostream &os, const Size &s) { os << '{' + std::to_string(s.width) + " ," + std::to_string(s.height) + '}'; return os; }
#endif
