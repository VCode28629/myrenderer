#include "model.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

Model::Model(const char *filename) : verts_(), faces_() {
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            for (int i = 0; i < 3; i++) iss >> v[i];
            verts_.push_back(v);
        } else if (!line.compare(0, 2, "f ")) {
            std::vector<Vec3i> f;
            int idx, vt, vn;
            iss >> trash;
            while (iss >> idx >> trash >> vt >> trash >> vn) {
                // in wavefront obj all indices start at 1, not zero
                f.push_back(Vec3i(idx - 1, vt - 1, vn - 1));
            }
            faces_.push_back(f);
        } else if (!line.compare(0, 3, "vt ")) {
            float ftrash;
            iss >> trash;
            iss >> trash;
            Vec2f v;
            for (int i = 0; i < 2; i++) iss >> v[i];
            iss >> ftrash;
            textures_.push_back(v);
        } else if (!line.compare(0, 3, "vn ")) {
            iss >> trash;
            iss >> trash;
            Vec3f v;
            for (int i = 0; i < 3; i++) iss >> v[i];
            normals_.push_back(v);
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# " << faces_.size()
              << std::endl;
}
Model::~Model() {}

int Model::nverts() const { return (int)verts_.size(); }

int Model::nfaces() const { return (int)faces_.size(); }

std::vector<Vec3i> Model::face(int idx) const { return faces_[idx]; }

Vec3f Model::vert(int i) const { return verts_[i]; }
Vec3f Model::normal(int i) const { return normals_[i]; }
Vec2f Model::texture(int i) const { return textures_[i]; }
