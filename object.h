#pragma once

#include "model.h"
#include "tgaimage.h"

struct Object {
    Model model;
    TGAImage texture;
    Vec3f location;
    Vec3f scale;
    Vec3f rotation;
    Object(const char *filename, const char *texturename, Vec3f location,
           Vec3f scale, Vec3f rotation);
    ~Object();

    int nverts() const;
    int nfaces() const;
    Vec3f getVert(int i) const;
    Vec3f getNormal(int i) const;
    Vec2f getTexture(int i) const;
    std::vector<Vec3i> getFace(int idx) const;
};
