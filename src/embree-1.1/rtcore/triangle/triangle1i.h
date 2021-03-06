// ======================================================================== //
// Copyright 2009-2012 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#ifndef __EMBREE_ACCEL_TRIANGLE1I_H__
#define __EMBREE_ACCEL_TRIANGLE1I_H__

#include "triangle.h"

namespace embree
{
  /*! Single triangle from an indexed face set. */
  struct Triangle1i
  {
    /*! block size */
    static const size_t blockSize = 1;
    static const size_t logBlockSize = 0;

    /*! Do we need the vertex array. */
    static const bool needVertices = true;

    /*! Cost of ray/triangle intersection. */
    static const int intCost = 1;

    /*! virtual interface to query information about the triangle type */
    static const struct Type : public TriangleType
    {
      Type () : TriangleType("triangle1i",sizeof(Triangle1i),1,true,1) {}

      size_t blocks(size_t x) const {
        return x;
      }
      
      size_t size(const char* This) const {
        return ((Triangle1i*)This)->size();
      }
      
      float area(const char* This, const Vec3fa* vertices) const {
        return ((Triangle1i*)This)->area(vertices);
      }
      
      void pack(char* This, atomic_set<PrimRefBlock>::block_iterator_unsafe& prims, const BuildTriangle* triangles, const Vec3fa* vertices) const {
        ((Triangle1i*)This)->pack(prims,triangles,vertices);
      }
    } type;

  public:

    /*! Default constructor. */
    __forceinline Triangle1i () {}

    /*! Construction from vertices and IDs. */
    __forceinline Triangle1i (const int& v0, const int& v1, const int& v2, const int& id0, const int& id1)
      : v0(v0), v1(v1), v2(v2), id0(id0), id1(id1) {}

    /*! Returns the number of stored triangles. */
    __forceinline size_t size() const { return 1; }

     /*! Computes the area of the triangle. */
    __forceinline float area(const Vec3fa* vertices) {
      const Vec3f e1 = vertices[v0]-vertices[v1];
      const Vec3f e2 = vertices[v2]-vertices[v0];
      const Vec3f Ng = cross(e1,e2);
      return length(Ng);
    }
    
    /*! Packs triangle taken from primitive list. */
    template<typename Iterator>
    __forceinline void pack(Iterator& prims, const BuildTriangle* triangles, const Vec3fa* vertices)
    {
      const PrimRef& prim = *prims; prims++;
      const BuildTriangle& tri = triangles[prim.id()];
      new (this) Triangle1i(tri.v0,tri.v1,tri.v2,tri.id0 & 0x7FFFFFFF,tri.id1);
    }

  public:
    int32 v0;      //!< Index of 1st vertex.
    int32 v1;      //!< Index of 2nd vertex.
    int32 v2;      //!< Index of 3rd vertex.
    int32 id0;     //!< 1st user ID.
    int32 id1;     //!< 2nd user ID.
  };
}

#endif


