/**
 * @file    tree_mesh_builder.cpp
 *
 * @author  Monika Rosinská
 *
 * @brief   Parallel Marching Cubes implementation using OpenMP tasks + octree early elimination
 *
 * @date    9. 12. 2021
 **/

#include <iostream>
#include <math.h>
#include <limits>

#include "tree_mesh_builder.h"

TreeMeshBuilder::TreeMeshBuilder(unsigned gridEdgeSize)
    : BaseMeshBuilder(gridEdgeSize, "Octree") {
}

unsigned TreeMeshBuilder::splitCube(float edgeLength, Vec3_t<float>middle, const ParametricScalarField &field, int depth){
    //if depth is zero, call buildCube and return number of triangles
    if(depth == 0){
        auto scalingFactor = 1.0f / mGridResolution;
        middle = Vec3_t<float>(std::floor(middle.x*scalingFactor), std::floor(middle.y*scalingFactor), std::floor(middle.z*scalingFactor));
        return buildCube(middle, field);
    }
    int triangleCount = 0;


    //split cube to 8 cubes
    //for each cube:
        //count middle of cube
        //count F(p)
        //if count F(p) <= l+sqrt(3)/2 * edgeLength continue
        //call splitCube for this cube with depth-1

    //we know middle of cube, so we need to get zero point and move by the cube
    float halfEdge = edgeLength/2.0f;
    float limit = mIsoLevel + float(sqrt3o2 * halfEdge);
    for(int z = 0; z < 2; z++) {
        for (int y = 0; y < 2; y++) {
            for (int x = 0; x < 2; x++) {
                #pragma omp task default(none) shared(middle, halfEdge, field, depth, triangleCount, limit) firstprivate(x, y, z)
                {
                    //cube has coordinates [middle - halfEdge + x*halfEdge, middle - halfEdge + y*halfEdge, middle - halfEdge + z*halfEdge] (left down corner)
                    // to [middle - halfEdge + x*halfEdge, middle - halfEdge + y*halfEdge, middle - halfEdge + z*halfEdge] (right up corner)
                    //so middle is [middle - halfEdge + x*halfEdge + halfEdge/2,
                    //              middle - halfEdge + y*halfEdge + halfEdge/2,
                    //              middle - halfEdge + z*halfEdge + halfEdge/2]
                    Vec3_t<float> cubeMiddle = Vec3_t<float>(
                            middle.x - halfEdge + x * halfEdge + halfEdge / 2.0f,
                            middle.y - halfEdge + y * halfEdge + halfEdge / 2.0f,
                            middle.z - halfEdge + z * halfEdge + halfEdge / 2.0f);
                    //count F(p)
                    float Fp = evaluateFieldAt(cubeMiddle, field);
                    if (Fp <= limit) {
                        //call splitCube for this cube with depth-1
                        #pragma omp atomic update
                            triangleCount += splitCube(halfEdge, cubeMiddle, field, depth - 1);
                    }
                }
            }
        }
    }
    #pragma omp taskwait
    return triangleCount;
}

unsigned TreeMeshBuilder::marchCubes(const ParametricScalarField &field){
    // Suggested approach to tackle this problem is to add new method to
    // this class. This method will call itself to process the children.
    // It is also strongly suggested to first implement Octree as sequential
    // code and only when that works add OpenMP tasks to achieve parallelism.
    auto cubeSize = field.getSize();
    float longestEdge = std::max(cubeSize.x, std::max(cubeSize.y, cubeSize.z));
    float middleCoor = longestEdge / 2.0f;
    Vec3_t<float>middle = (middleCoor);
    int depth = log2(mGridSize);
    int triangleCount;
    #pragma omp parallel default(none) shared(longestEdge, middle, field, depth, triangleCount)
        {
            #pragma omp single
            {
                triangleCount = splitCube(longestEdge, middle, field, depth);
            }
        }
    return triangleCount;
}

float TreeMeshBuilder::evaluateFieldAt(const Vec3_t<float> &pos, const ParametricScalarField &field){
    // NOTE: This method is called from "buildCube(...)"!

    // 1. Store pointer to and number of 3D points in the field
    //    (to avoid "data()" and "size()" call in the loop).
    const Vec3_t<float> *pPoints = field.getPoints().data();
    const unsigned count = unsigned(field.getPoints().size());

    float value = std::numeric_limits<float>::max();

    // 2. Find minimum square distance from points "pos" to any point in the
    //    field.
    for(unsigned i = 0; i < count; ++i)
    {
        float distanceSquared  = (pos.x - pPoints[i].x) * (pos.x - pPoints[i].x);
        distanceSquared       += (pos.y - pPoints[i].y) * (pos.y - pPoints[i].y);
        distanceSquared       += (pos.z - pPoints[i].z) * (pos.z - pPoints[i].z);

        // Comparing squares instead of real distance to avoid unnecessary
        // "sqrt"s in the loop.
        value = std::min(value, distanceSquared);
    }

    // NOTE: This method is called from "buildCube(...)"!

    // Store generated triangle into vector (array) of generated triangles.
    // The pointer to data in this array is return by "getTrianglesArray(...)" call
    // after "marchCubes(...)" call ends.
    return sqrt(value);
}

void TreeMeshBuilder::emitTriangle(const BaseMeshBuilder::Triangle_t &triangle) {
    // NOTE: This method is called from "buildCube(...)"!

    // Store generated triangle into vector (array) of generated triangles.
    // The pointer to data in this array is return by "getTrianglesArray(...)" call
    // after "marchCubes(...)" call ends.
    #pragma omp critical
        mTriangles.push_back(triangle);
}
