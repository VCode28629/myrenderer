
#include "camera.h"

#include <limits>
#include <utility>

#include "matrix.h"
#include "transformation.h"

const Vec3f light_dir = Vec3f(0, 0, -1).normalize();

const TGAColor BLACK = TGAColor(0, 0, 0, 255);

void Camera::clean(TGAColor color = BLACK) {
    int width = image.get_width();
    int height = image.get_height();
    for (int i = width * height - 1; i >= 0; --i)
        z_buffer[i] = -std::numeric_limits<float>::max();
}

void Camera::setCurtain(int width, int height, float distance) {
    image = TGAImage(width, height, TGAImage::RGB);
    this->distance = distance;
    delete z_buffer;
    z_buffer = new float[width * height];
    clean();
}

std::pair<Vec2i, Vec2i> Camera::getBBox(Vec3f *world_coords) {
    Vec2i center = Vec2i(image.get_width() / 2, image.get_height() / 2);
    Vec2f bboxmin(std::numeric_limits<float>::max(),
                  std::numeric_limits<float>::max());
    Vec2f bboxmax(-std::numeric_limits<float>::max(),
                  -std::numeric_limits<float>::max());
    Vec2f clamp(image.get_width() - 1 - center.x,
                image.get_height() - 1 - center.y);
    for (int i = 0; i < 3; ++i) {
        Vec2f view_coords =
            Vec2f(world_coords[i].x * -distance / world_coords[i].z,
                  world_coords[i].y * -distance / world_coords[i].z);
        for (int j = 0; j < 2; ++j) {
            bboxmin[j] =
                std::max(0.f - center[j], std::min(bboxmin[j], view_coords[j]));
            bboxmax[j] =
                std::min(clamp[j], std::max(bboxmax[j], view_coords[j]));
        }
    }
    return std::make_pair(
        Vec2i(bboxmin.x, bboxmin.y),
        Vec2i(std::ceil(bboxmax.x) + 0.5, std::ceil(bboxmax.y) + 0.5));
}

Vec3f intersection(Vec3f now, Vec3f *pts) {
    Vec4f n =
        cross(Vec4f(pts[0].x, (pts[1] - pts[0]).x, (pts[2] - pts[0]).x, now.x),
              Vec4f(pts[0].y, (pts[1] - pts[0]).y, (pts[2] - pts[0]).y, now.y),
              Vec4f(pts[0].z, (pts[1] - pts[0]).z, (pts[2] - pts[0]).z, now.z));
    if (n.x == 0) return Vec3f(-1, 1, 1);
    return Vec3f(1.0 - (n.y + n.z) / n.x, n.y / n.x, n.z / n.x);
}

void Camera::triangle(Vec3f *pts, Vec2f *texture_coords, Vec3f *normals,
                      Matrix4f MV, TGAImage &texture) {
    Vec2i center = Vec2i(image.get_width() / 2, image.get_height() / 2);
    int width = image.get_width();
    Vec3f world_coords[3];
    for (int i = 0; i < 3; ++i) {
        world_coords[i] = toVec3(MV * toVec4(pts[i]));
        normals[i] = toVec3(MV * toVec4(normals[i]));
    }
    std::pair<Vec2i, Vec2i> bbox = getBBox(world_coords);
    Vec2i bboxmin = bbox.first;
    Vec2i bboxmax = bbox.second;
    for (int x = bboxmin[0]; x <= bboxmax[0]; ++x) {
        for (int y = bboxmin[1]; y <= bboxmax[1]; ++y) {
            int i = x + center.x;
            int j = y + center.y;
            Vec3f view_point(x, y, -distance);
            Vec3f barycentric = intersection(view_point, world_coords);
            if (barycentric.x < 0 || barycentric.y < 0 || barycentric.z < 0)
                continue;
            // get point in world_coord
            Vec3f world_point;
            for (int i = 0; i < 3; ++i) {
                world_point = world_point + world_coords[i] * barycentric[i];
            }
            // z-buffer
            if (world_point.z >= 0) continue;
            if (world_point.z <= z_buffer[i + j * width]) continue;
            z_buffer[i + j * width] = world_point.z;
            // get texture
            Vec2f color_location(0, 0);
            for (int i = 0; i < 3; ++i)
                color_location =
                    color_location + texture_coords[i] * barycentric[i];
            TGAColor color = texture.get(color_location.x, color_location.y);
            // get normal direction
            Vec3f n(0, 0, 0);
            for (int i = 0; i < 3; ++i) n = n + normals[i] * barycentric[i];
            n.normalize();
            // get light intensity
            float intensity = std::abs(n * light_dir);
            // add global illumination
            const float global = 0.3;
            for (int i = 0; i < 3; ++i)
                color.raw[i] =
                    color.raw[i] * (global + (1 - global) * intensity);
            image.set(i, j, color);
        }
    }
}

void Camera::triangle_orthogonal_projection(Vec3f *pts, Vec2f *texture_coords,
                                            Vec3f *normals, Matrix4f MV,
                                            TGAImage &texture) {
    Vec2i center = Vec2i(image.get_width() / 2, image.get_height() / 2);
    int width = image.get_width();
    Vec3f world_coords[3];
    for (int i = 0; i < 3; ++i) {
        world_coords[i] = toVec3(MV * toVec4(pts[i]));
    }
    auto bbox = [&center, this, &world_coords]() {
        Vec2f bboxmin(std::numeric_limits<float>::max(),
                      std::numeric_limits<float>::max());
        Vec2f bboxmax(-std::numeric_limits<float>::max(),
                      -std::numeric_limits<float>::max());
        Vec2f clamp(image.get_width() - 1 - center.x,
                    image.get_height() - 1 - center.y);
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 2; ++j) {
                bboxmin[j] = std::max(0.f - center[j],
                                      std::min(bboxmin[j], world_coords[i][j]));
                bboxmax[j] = std::min(clamp[j],
                                      std::max(bboxmax[j], world_coords[i][j]));
            }
        }
        return std::make_pair(
            Vec2i(bboxmin.x, bboxmin.y),
            Vec2i(std::ceil(bboxmax.x) + 0.5, std::ceil(bboxmax.y) + 0.5));
    }();

    Vec2i bboxmin = bbox.first;
    Vec2i bboxmax = bbox.second;
    for (int i = bboxmin[0]; i <= bboxmax[0]; ++i) {
        for (int j = bboxmin[1]; j <= bboxmax[1]; ++j) {
            Vec3f p(i, j, 0);
            // get barycentric coordinates
            auto barycentric = [](Vec3f *pts, Vec3f p) {
                Vec3f u = Vec3f(pts[0].x - p.x, pts[1].x - pts[0].x,
                                pts[2].x - pts[0].x) ^
                          Vec3f(pts[0].y - p.y, pts[1].y - pts[0].y,
                                pts[2].y - pts[0].y);
                if (u.x == 0) return Vec3f(-1.0, 1.0, 1.0);
                return Vec3f(1.0 - (u.y + u.z) / u.x, u.y / u.x, u.z / u.x);
            };
            Vec3f bc_screen = barycentric(world_coords, p);
            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;
            // z-buffer
            float z = 0;
            for (int i = 0; i < 3; ++i) z += bc_screen[i] * world_coords[i][2];
            if (z <= z_buffer[i + center.x + (j + center.y) * width]) continue;
            z_buffer[i + center.x + (j + center.y) * width] = z;
            // get texture
            Vec2f color_location(0, 0);
            for (int i = 0; i < 3; ++i)
                color_location =
                    color_location + texture_coords[i] * bc_screen[i];
            TGAColor color = texture.get(color_location.x, color_location.y);
            // color = white;
            // get normal direction
            Vec3f n(0, 0, 0);
            for (int i = 0; i < 3; ++i) n = n + normals[i] * bc_screen[i];
            n.normalize();
            // get light intensity
            float intensity = std::abs(n * light_dir);
            // add global illumination
            const float global = 0.3;
            for (int i = 0; i < 3; ++i)
                color.raw[i] =
                    color.raw[i] * (global + (1 - global) * intensity);
            image.set(i + center.x, j + center.y, color);
        }
    }
}

void Camera::draw(Object &obj) {
    Matrix4f model = modelMatrix(obj.scale, obj.rotation, obj.location);
    Matrix4f view = viewMatrix(camera_location, up_direction, lookat);
    Matrix4f MV = view * model;
    const int texture_width = obj.texture.get_width();
    const int texture_height = obj.texture.get_height();
    for (int i = 0; i < obj.nfaces(); ++i) {
        std::vector<Vec3i> face = obj.getFace(i);
        Vec3f model_coords[3];
        Vec2f texture_coords[3];
        Vec3f normals[3];
        for (int j = 0; j < 3; ++j) {
            model_coords[j] = obj.getVert(face[j][0]);
            texture_coords[j] = obj.getTexture(face[j][1]);
            texture_coords[j][0] *= texture_width - 1;
            texture_coords[j][1] *= texture_height - 1;
            normals[j] = obj.getNormal(face[j][2]);
        }
        // triangle_orthogonal_projection(model_coords, texture_coords, normals,
        //                                MV, obj.texture);
        triangle(model_coords, texture_coords, normals, MV, obj.texture);
    }
}