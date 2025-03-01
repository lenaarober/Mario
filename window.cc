#include "fenster.h"  // Has to be first!!

#include "window.hh"

namespace pro2 {

Window::Window(const char *title, int width, int height, int zoom)
    : fenster_{.title = title, .width = width * zoom, .height = height * zoom},
      zoom_(zoom),
      pixels_size_(width * height * zoom * zoom)  //
{
    pixels_ = new uint32_t[width * height * zoom * zoom];
    fenster_.buf = pixels_;
    fenster_open(&fenster_);
    last_time_ = fenster_time();
}

Window::~Window() {
    fenster_close(&fenster_);
    delete[] pixels_;
}

bool Window::next_frame() {
    int wait = 16 - (fenster_time() - last_time_);
    if (wait > 0) {
        fenster_sleep(wait);
    }
    last_time_ = fenster_time();
    frame_count_++;
    return fenster_loop(&fenster_) == 0;
}

void Window::clear(int color) {
    for (size_t i = 0; i < pixels_size_; i++) {
        pixels_[i] = color;
    }
}

Pt Window::mouse_pos() const {
    const int width = fenster_.width / zoom_;
    const int height = fenster_.height / zoom_;
    int       x = fenster_.x / zoom_;
    int       y = fenster_.y / zoom_;

    if (x >= width) {
        x = width - 1;
    } else if (x < 0) {
        x = 0;
    }
    if (y >= height) {
        y = height - 1;
    } else if (y < 0) {
        y = 0;
    }
    return Pt{x, y};
}

void Window::set_pixel(Pt pt, int color) {
    for (int i = 0; i < zoom_; i++) {
        for (int j = 0; j < zoom_; j++) {
            const int _i = pt.x * zoom_ + i;
            const int _j = pt.y * zoom_ + j;
            if (_i >= 0 && _i < fenster_.width && _j >= 0 && _j < fenster_.height) {
                fenster_pixel(&fenster_, _i, _j) = color;
            }
        }
    }
}

}  // namespace pro2