#pragma once

#include "matrix.h"
#include "object.h"
#include "tgaimage.h"

class Camera {
    Vec3f camera_location;
    Vec3f up_direction;
    Vec3f lookat;
    float distance;
    float *z_buffer;
    TGAImage image;
    void triangle(Vec3f *pts, Vec2f *texture_coords, Vec3f *normals,
                  Matrix4f MV, TGAImage &texture);
    void triangle_orthogonal_projection(Vec3f *pts, Vec2f *texture_coords,
                                        Vec3f *normals, Matrix4f MV,
                                        TGAImage &texture);
    std::pair<Vec2i, Vec2i> getBBox(Vec3f *world_coords);

   public:
    Camera(Vec3f camera_location, Vec3f up_direction, Vec3f lookat)
        : camera_location(camera_location),
          up_direction(up_direction.normalize()),
          lookat(lookat.normalize()),
          distance(0),
          z_buffer(0),
          image() {}
    Camera()
        : camera_location(Vec3f(0, 0, 0)),
          up_direction(Vec3f(0, 1, 0)),
          lookat(Vec3f(0, 0, -1)),
          distance(0),
          z_buffer(0),
          image() {}
    void clean(TGAColor color);
    void setCurtain(int height, int width, float distance);
    void draw(Object &obj);
    void save(const char *filename) {
        image.flip_vertically();
        image.write_tga_file(filename);
        image.flip_vertically();
    }
};
