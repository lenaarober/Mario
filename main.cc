#include <vector>
#include "window.hh"
using namespace std;

using pro2::Colors;

const int WIDTH = 256, HEIGHT = 240;
const int ZOOM = 2;

// Constantes de colores
const int _ = -1;
const int r = Colors::red;
const int s = 0xecc49b;
const int b = 0x5e6ddc;
const int y = Colors::yellow;
const int h = Colors::black;
const int g = 0xaaaaaa;
const int w = 0x8d573c;
const int a = 0xfad6a5;
const int o = 0xff8000;
const int p = 0xffd6bb;


// Skin mario
const vector<vector<int>> mario_sprite = {
    {_, _, _, r, r, r, r, r, _, _, _, _},
    {_, _, r, r, r, r, r, r, r, r, r, _},
    {_, _, h, h, h, s, s, h, s, _, _, _},
    {_, h, s, h, s, s, s, h, s, s, s, _},
    {_, h, s, h, h, s, s, s, h, s, s, s},
    {_, h, h, s, s, s, s, h, h, h, h, _},
    {_, _, _, s, s, s, s, s, s, s, _, _},
    {_, _, r, r, b, r, r, r, _, _, _, _},
    {_, r, r, r, b, r, r, b, r, r, r, _},
    {r, r, r, r, b, b, b, b, r, r, r, r},
    {g, g, r, b, y, b, b, y, b, r, g, g},
    {g, g, g, b, b, b, b, b, b, g, g, g},
    {g, g, b, b, b, b, b, b, b, b, g, g},
    {_, _, b, b, b, _, _, b, b, b, _, _},
    {_, w, w, w, _, _, _, _, w, w, w, _},
    {w, w, w, w, _, _, _, _, w, w, w, w},
};

// Skin goomba
const vector<vector<int>> goomba_sprite = {
    {_, _, _, _, _, _, w, w, w, w, _, _, _, _, _, _},
    {_, _, _, _, _, w, w, w, w, w, w, _, _, _, _, _},
    {_, _, _, _, w, w, w, w, w, w, w, w, _, _, _, _},
    {_, _, _, w, w, w, w, w, w, w, w, w, w, _, _, _},
    {_, _, w, h, h, w, w, w, w, w, w, h, h, w, _, _},
    {_, w, w, w, g, h, w, w, w, w, h, g, w, w, w, _},
    {_, w, w, w, g, h, h, h, h, h, h, g, w, w, w, _},
    {w, w, w, w, g, h, g, w, w, g, h, g, w, w, w, w},
    {w, w, w, w, g, g, g, w, w, g, g, g, w, w, w, w},
    {w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w},
    {_, w, w, w, w, a, a, a, a, a, a, w, w, w, w, _},
    {_, _, _, _, a, a, a, a, a, a, a, a, _, _, _, _},
    {_, _, h, h, a, a, a, a, a, a, a, a, h, h, _, _},
    {_, h, h, h, h, h, a, a, a, a, h, h, h, h, h, _},
    {_, h, h, h, h, h, h, _, _, h, h, h, h, h, h, _},
    {_, _, h, h, h, h, h, _, _, h, h, h,    h, h, _} 
};

// Block
const vector<vector<int>> block_floor_sprite = {
    {o, p, p, p, p, p, p, p, p, h, o, p, p, p, p, o},
    {p, o, o, o, o, o, o, o, o, h, p, o, o, o, o, h},
    {p, o, o, o, o, o, o, o, o, h, p, o, o, o, o, h},
    {p, o, o, o, o, o, o, o, o, h, p, o, o, o, o, h},
    {p, o, o, o, o, o, o, o, o, h, p, h, o, o, o, h},
    {p, o, o, o, o, o, o, o, o, h, o, h, h, h, h, o},
    {p, o, o, o, o, o, o, o, o, h, p, p, p, p, p, h},
    {p, o, o, o, o, o, o, o, o, h, p, o, o, o, o, h},
    {p, o, o, o, o, o, o, o, o, h, p, o, o, o, o, h},
    {p, o, o, o, o, o, o, o, o, h, p, o, o, o, o, h},
    {h, h, o, o, o, o, o, o, h, p, p, o, o, o, o, h},
    {p, p, h, h, o, o, o, o, h, p, o, o, o, o, o, h},
    {p, o, p, p, h, h, h, h, p, p, o, o, o, o, o, h},
    {p, o, o, o, p, p, p, h, p, o, o, o, o, o, o, h},
    {p, o, o, o, o, o, o, h, p, o, o, o, o, o, h, h},
    {o, h, h, h, h, h, h, o, p, h, h, h, h, h, h, o}
};

const char arrow_up = 17;
const char arrow_down = 18;
const char arrow_left = 20;
const char arrow_right = 19;
const char space = 32;

void put_sprite(pro2::Window& window, pro2::Pt orig, const vector<vector<int>>& sprite) {
    for (int i = 0; i < sprite.size(); i++) {
        const vector<int>& line = sprite[i];
        for (int j = 0; j < line.size(); j++) {
            if (line[j] >= 0) {
                window.set_pixel({orig.x + j, orig.y + i}, line[j]);
            }
        }
    }
}

void set_background(pro2::Window& window) {
    // Floor
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 16; ++j) {
                put_sprite(window, {0 + j*16, HEIGHT - (16 + i*16)}, block_floor_sprite);
        }
    }
}

void set_initial_elements(pro2::Window& window) {
    // Mario 
    put_sprite(window, {WIDTH/2 - 6, HEIGHT - 16*4}, mario_sprite);
    // Goomba
    put_sprite(window, {8, HEIGHT - 16*4}, goomba_sprite);
}

int main() {
    // Create window
    pro2::Window window("Mario Bros", WIDTH, HEIGHT, ZOOM);

    // Initial set
    window.clear();
    set_background(window);
    set_initial_elements(window);

    int i_mario = WIDTH/2 - 8;
    int j_mario = HEIGHT - 4*16;
    int i_goomba = 8;
    int I_goomba = 1;
    
    while (window.next_frame()) {
        window.clear();
 
        if(window.is_key_pressed(arrow_up)) {
            j_mario -= 1;
            window.clear();
            put_sprite(window, {i_mario, j_mario}, mario_sprite);
        }
        if(window.is_key_pressed(arrow_left)) {
            i_mario -= 1;
            window.clear();
            put_sprite(window, {i_mario, j_mario}, mario_sprite);
        }
        if(window.is_key_pressed(arrow_down)) {
            j_mario += 1;
            window.clear();
            put_sprite(window, {i_mario, j_mario}, mario_sprite);
        }
        if(window.is_key_pressed(arrow_right)) {
            i_mario += 1;
            window.clear();
            put_sprite(window, {i_mario, j_mario}, mario_sprite);
        }

        set_background(window);    
        put_sprite(window, {i_goomba, HEIGHT - 16*4}, goomba_sprite);
        put_sprite(window, {i_mario, j_mario}, mario_sprite);
        
        // Mario 
        //// Limits
        if (i_mario > (WIDTH - 16) ) i_mario = WIDTH - 16;
        if (i_mario < 0) i_mario = 0;
        if (j_mario > (HEIGHT - 16*4)) j_mario = HEIGHT - 16*4;
        if (j_mario < 0) j_mario= 0;
        
        i_goomba += I_goomba;
        // Goomba limits
        if (i_goomba > (WIDTH - 16) || i_goomba < 0) I_goomba *= -1;
    }
}