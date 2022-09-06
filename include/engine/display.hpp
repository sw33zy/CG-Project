#ifndef DISPLAY_HPP
#define DISPLAY_HPP

class FramesPerSecond {
    static int timebase;
    static float frame;

public:
    static void init();

    static void update();

};

extern float px , py,pz;

void set_camara();

void changeSize(int w, int h);

bool camera_keyboard_handler(unsigned char key, int x, int y);

bool camara_special_keys_handler(int key, int px, int py);

void camara_mouseMove(int x, int y);

void camara_mouseButton(int button, int state, int xx, int yy);

#endif // DISPLAY_HPP