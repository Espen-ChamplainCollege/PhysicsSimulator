
#include "window.h"
 



int main() {

  Window::initializeContext();
  Window window = Window(600, 600);
  Window::mainWindow = &window;

  

  glutMainLoop();
  return 0;
}
