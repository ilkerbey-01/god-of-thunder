#include "1_DEFINE.H"
#include "1_PROTO.H"

uint16_t draw_page;

int main(int argc, char** argv) {
    int16_t x = 0;
    int16_t y = 0;
    xcopyd2d(x, y, x + 16, y + 16, x, y, PAGE2, draw_page, 320, 320);
    return 0;
}