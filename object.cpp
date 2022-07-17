#include "object.h"

Object::Object(const char *filename, const char *texturename,
               Vec3f location = Vec3f(0, 0, 0), Vec3f scale = Vec3f(1, 1, 1),
               Vec3f rotation = Vec3f(0, 0, 0))
    : model(filename), location(location), scale(scale), rotation(rotation) {
    texture.read_tga_file(texturename);
    texture.flip_vertically();
}

Object::~Object() {}

int Object::nverts() const { return (int)model.nverts(); }

int Object::nfaces() const { return (int)model.nfaces(); }

std::vector<Vec3i> Object::getFace(int idx) const { return model.face(idx); }

Vec3f Object::getVert(int i) const { return model.vert(i); }
Vec3f Object::getNormal(int i) const { return model.normal(i); }
Vec2f Object::getTexture(int i) const { return model.texture(i); }
