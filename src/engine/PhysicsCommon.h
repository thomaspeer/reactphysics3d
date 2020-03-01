/********************************************************************************
* ReactPhysics3D physics library, http://www.reactphysics3d.com                 *
* Copyright (c) 2010-2019 Daniel Chappuis                                       *
*********************************************************************************
*                                                                               *
* This software is provided 'as-is', without any express or implied warranty.   *
* In no event will the authors be held liable for any damages arising from the  *
* use of this software.                                                         *
*                                                                               *
* Permission is granted to anyone to use this software for any purpose,         *
* including commercial applications, and to alter it and redistribute it        *
* freely, subject to the following restrictions:                                *
*                                                                               *
* 1. The origin of this software must not be misrepresented; you must not claim *
*    that you wrote the original software. If you use this software in a        *
*    product, an acknowledgment in the product documentation would be           *
*    appreciated but is not required.                                           *
*                                                                               *
* 2. Altered source versions must be plainly marked as such, and must not be    *
*    misrepresented as being the original software.                             *
*                                                                               *
* 3. This notice may not be removed or altered from any source distribution.    *
*                                                                               *
********************************************************************************/

#ifndef REACTPHYSICS3D_PHYSICS_COMMON_H
#define REACTPHYSICS3D_PHYSICS_COMMON_H

// Libraries
#include "memory/MemoryManager.h"
#include "engine/PhysicsWorld.h"
#include "collision/shapes/SphereShape.h"
#include "collision/shapes/BoxShape.h"
#include "collision/shapes/CapsuleShape.h"
#include "collision/shapes/HeightFieldShape.h"
#include "collision/shapes/ConvexMeshShape.h"
#include "collision/shapes/ConcaveMeshShape.h"
#include "collision/TriangleMesh.h"

/// ReactPhysics3D namespace
namespace reactphysics3d {

// Class PhysicsCommon
/**
 * This class is a singleton that needs to be instanciated once at the beginning.
 * Then this class is used by the user as a factory to create the physics world and
 * other objects.
 */
class PhysicsCommon {

    private :

        // -------------------- Attributes -------------------- //

        /// Memory manager
        MemoryManager mMemoryManager;

        /// Set of physics worlds
        Set<PhysicsWorld*> mPhysicsWorlds;

        /// Set of sphere shapes
        Set<SphereShape*> mSphereShapes;

        /// Set of box shapes
        Set<BoxShape*> mBoxShapes;

        /// Set of capsule shapes
        Set<CapsuleShape*> mCapsuleShapes;

        /// Set of convex mesh shapes
        Set<ConvexMeshShape*> mConvexMeshShapes;

        /// Set of concave mesh shapes
        Set<ConcaveMeshShape*> mConcaveMeshShapes;

        /// Set of height field shapes
        Set<HeightFieldShape*> mHeightFieldShapes;

        /// Set of polyhedron meshes
        Set<PolyhedronMesh*> mPolyhedronMeshes;

        /// Set of triangle meshes
        Set<TriangleMesh*> mTriangleMeshes;

        /// Set of loggers
        Set<Logger*> mLoggers;

        /// Set of loggers
        Set<Profiler*> mProfilers;

        // -------------------- Methods -------------------- //

        /// Destroy and release everything that has been allocated
        void release();

    public :

        // -------------------- Methods -------------------- //

        /// Constructor
        /**
         * @param baseMemoryAllocator Pointer to a user custom memory allocator
         */
        PhysicsCommon(MemoryAllocator* baseMemoryAllocator = nullptr);

        /// Destructor
        ~PhysicsCommon();

        // TODO : Add better code documentation for all methods in this class (document
        //        the method parameters with the "@param" keyword for Doxygen

        /// Create and return an instance of PhysicsWorld
        PhysicsWorld* createPhysicsWorld(const PhysicsWorld::WorldSettings& worldSettings = PhysicsWorld::WorldSettings(),
                                         Logger* logger = nullptr, Profiler* profiler = nullptr);

        /// Destroy an instance of PhysicsWorld
        void destroyPhysicsWorld(PhysicsWorld* world);

        /// Create and return a sphere collision shape
        SphereShape* createSphereShape(const decimal radius);

        /// Destroy a sphere collision shape
        void destroySphereShape(SphereShape* sphereShape);

        /// Create and return a box collision shape
        BoxShape* createBoxShape(const Vector3& extent);

        /// Destroy a box collision shape
        void destroyBoxShape(BoxShape* boxShape);

        /// Create and return a capsule shape
        CapsuleShape* createCapsuleShape(decimal radius, decimal height);

        /// Destroy a capsule collision shape
        void destroyCapsuleShape(CapsuleShape* capsuleShape);

        /// Create and return a convex mesh shape
        ConvexMeshShape* createConvexMeshShape(PolyhedronMesh* polyhedronMesh, const Vector3& scaling = Vector3(1,1,1));

        /// Destroy a convex mesh shape
        void destroyConvexMeshShape(ConvexMeshShape* convexMeshShape);

        /// Create and return a height-field shape
        HeightFieldShape* createHeightFieldShape(int nbGridColumns, int nbGridRows, decimal minHeight, decimal maxHeight,
                                                 const void* heightFieldData, HeightFieldShape::HeightDataType dataType,
                                                 int upAxis = 1, decimal integerHeightScale = 1.0f,
                                                  const Vector3& scaling = Vector3(1,1,1));

        /// Destroy a height-field shape
        void destroyHeightFieldShape(HeightFieldShape* heightFieldShape);

        /// Create and return a concave mesh shape
        ConcaveMeshShape* createConcaveMeshShape(TriangleMesh* triangleMesh, const Vector3& scaling = Vector3(1, 1, 1));

        /// Destroy a concave mesh shape
        void destroyConcaveMeshShape(ConcaveMeshShape* concaveMeshShape);

        /// Create a polyhedron mesh
        PolyhedronMesh* createPolyhedronMesh(PolygonVertexArray* polygonVertexArray);

        /// Destroy a polyhedron mesh
        void destroyPolyhedronMesh(PolyhedronMesh* polyhedronMesh);

        /// Create a triangle mesh
        TriangleMesh* createTriangleMesh();

        /// Destroy a triangle mesh
        void destroyTriangleMesh(TriangleMesh* triangleMesh);

// If logging is enabled
#ifdef IS_LOGGING_ACTIVE

        /// Create and return a new logger
        Logger* createLogger();

        /// Destroy a logger
        void destroyLogger(Logger* logger);

#endif

// If profiling is enabled
#ifdef IS_PROFILING_ACTIVE

        /// Create and return a new profiler
        Profiler* createProfiler();

        /// Destroy a profiler
        void destroyProfiler(Profiler* profiler);

#endif

};

}

#endif
