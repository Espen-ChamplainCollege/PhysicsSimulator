/* SM Color
*/

#ifndef ___SM_LIB_COLOR
#define ___SM_LIB_COLOR

#include <cstdlib>
#include <random>
#include <string>
struct Color {
  float r, g, b, a;
  Color(float r_, float g_, float b_, float a_ = 1.0) : r(r_), g(g_), b(b_), a(a_){
    if(r > 1 || g > 1 || b > 1 || a > 1){
      r /= 255;
      g /= 255;
      b /= 255;
      a = a == 1.0 ? 255 : a / 255;
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
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, 255);
    return Color(dist(mt), dist(mt), dist(mt));
  }
};


#endif
