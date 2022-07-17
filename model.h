#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>

#include "matrix.h"

class Model {
   private:
    std::vector<Vec3f> verts_;
    std::vector<Vec3f> normals_;
    std::vector<Vec2f> textures_;
    std::vector<std::vector<Vec3i>> faces_;

   public:
    Model(const char *filename);
    ~Model();

    int nverts() const;
    int nfaces() const;
    Vec3f vert(int i) const;
    Vec3f normal(int i) const;
    Vec2f texture(int i) const;
    std::vector<Vec3i> face(int idx) const;
};

#endif  //__MODEL_H__
