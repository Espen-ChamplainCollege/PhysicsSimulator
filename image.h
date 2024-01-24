#ifndef ___SM_LIB_IMAGE
#define ___SM_LIB_IMAGE
#include "GL/gl.h"
#include <vector>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


struct Image {
  uint8_t *raw;
  int w, h, nch; // width, height, # channels
  Image(uint8_t *raw_) : raw(raw_){}
  Image(const std::string &path){
    raw = stbi_load(path.c_str(), &w, &h, &nch, 4);
    stbi__vertical_flip(raw, w, h, 4);
    
  }
  const uint8_t operator[](int i) const { return raw[i]; }
  uint8_t operator[](int i){return raw[i];}
};


#endif
