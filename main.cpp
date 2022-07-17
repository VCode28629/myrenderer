#include "camera.h"
#include "object.h"

// void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
//     bool steep = false;
//     if (std::abs(x1 - x0) < std::abs(y1 - y0)) {
//         std::swap(x0, y0);
//         std::swap(x1, y1);
//         steep = true;
//     }
//     if (x0 > x1) {
//         std::swap(x0, x1);
//         std::swap(y0, y1);
//     }
//     int dx = x1 - x0;
//     int dy = y1 - y0;
//     int derror2dx = 2 * std::abs(dy);
//     int error2dx = 0;
//     int y = y0;
//     for (int x = x0; x <= x1; ++x) {
//         if (steep) {
//             image.set(y, x, color);
//         } else {
//             image.set(x, y, color);
//         }
//         error2dx += derror2dx;
//         if (error2dx > dx) {
//             y += (y1 > y0 ? 1 : -1);
//             error2dx -= 2 * dx;
//         }
//     }
// }

int main(int argc, char **argv) {
    Object obj1("obj/african_head.obj", "texture/african_head_diffuse.tga",
               Vec3f(0, 0, -1800), Vec3f(400, 400, 400), Vec3f(0.3, 0.3, 0.3));
    Object red_cube("obj/cube.obj", "texture/red.tga",
               Vec3f(50 + 50, 50, -500), Vec3f(50, 50, 50), Vec3f(0, 0.15, 0));
    Camera camera(Vec3f(100, -100, 100), Vec3f(-0.1, 1, 0.2),
                  Vec3f(-0.1, 0.1, -1));
    camera.setCurtain(800, 800, 1400);
    camera.draw(obj1);
    camera.draw(red_cube);
    camera.save("output.tga");
    return 0;
}
