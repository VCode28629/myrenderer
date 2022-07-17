#pragma once

#include "matrix.h"

Matrix4f modelMatrix(Vec3f scale, Vec3f rotation, Vec3f location) {
    // Model: scale
    Matrix4f scaleMatrix = Matrix4f::identity();
    for (int i = 0; i < 3; ++i) scaleMatrix[i][i] = scale[i];
    // Model: rotation
    Matrix4f rotationX = Matrix4f::identity();
    rotationX[1][1] = rotationX[2][2] = std::cos(rotation[0]);
    rotationX[2][1] = std::sin(rotation[0]);
    rotationX[1][2] = -rotationX[2][1];

    Matrix4f rotationY = Matrix4f::identity();
    rotationY[0][0] = rotationY[2][2] = std::cos(rotation[1]);
    rotationY[0][2] = std::sin(rotation[1]);
    rotationY[2][0] = -rotationY[2][0];

    Matrix4f rotationZ = Matrix4f::identity();
    rotationZ[0][0] = rotationZ[1][1] = std::cos(rotation[2]);
    rotationZ[1][0] = std::sin(rotation[2]);
    rotationZ[0][1] = -rotationZ[1][0];

    Matrix4f rotationMatrix = rotationX * rotationY * rotationZ;

    // Model: transport
    Matrix4f transportMatrix = Matrix4f::identity();
    for (int i = 0; i < 3; ++i) transportMatrix[i][3] = location[i];

    return transportMatrix * rotationMatrix * scaleMatrix;
}

Matrix4f viewMatrix(Vec3f camera_location, Vec3f up_direction, Vec3f lookat) {
    Matrix4f ret = Matrix4f::identity();
    Vec3f x_direction = lookat ^ up_direction;
    for (int i = 0; i < 3; ++i) {
        ret[0][i] = x_direction[i];
        ret[1][i] = up_direction[i];
        ret[2][i] = -lookat[i];
    }
    return ret;
}