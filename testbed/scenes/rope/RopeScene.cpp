/********************************************************************************
* ReactPhysics3D physics library, http://www.reactphysics3d.com                 *
* Copyright (c) 2010-2016 Daniel Chappuis                                       *
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

// Libraries
#include "RopeScene.h"
#include <cmath>

// Namespaces
using namespace openglframework;
using namespace ropescene;

// Constructor
RopeScene::RopeScene(const std::string& name, EngineSettings& settings)
      : SceneDemo(name, settings, true, SCENE_RADIUS), nbIterations(0), nbTorqueIterations(200) {

    std::string meshFolderPath("meshes/");

    mEngineSettings.timeStep = std::chrono::duration<double>(1.0f / 120.0f);

    // Compute the radius and the center of the scene
    openglframework::Vector3 center(0, 0, 0);

    // Set the center of the scene
    setScenePosition(center, SCENE_RADIUS);

    // Gravity vector in the physics world
    rp3d::Vector3 gravity(0, rp3d::decimal(-9.81), 0);

    rp3d::PhysicsWorld::WorldSettings worldSettings;
    worldSettings.worldName = name;

    const float linearDamping = 0.03f;
    const float angularDamping = 0.03f;

    // Logger
    rp3d::DefaultLogger* defaultLogger = mPhysicsCommon.createDefaultLogger();
    uint logLevel = static_cast<uint>(rp3d::Logger::Level::Information) | static_cast<uint>(rp3d::Logger::Level::Warning) |
            static_cast<uint>(rp3d::Logger::Level::Error);
    defaultLogger->addFileDestination("rp3d_log_" + name + ".html", logLevel, rp3d::DefaultLogger::Format::HTML);
    mPhysicsCommon.setLogger(defaultLogger);

    // Create the physics world for the physics simulation
    rp3d::PhysicsWorld* physicsWorld = mPhysicsCommon.createPhysicsWorld(worldSettings);
    physicsWorld->setEventListener(this);
    mPhysicsWorld = physicsWorld;

    // ---------- Create the ropes  --------- //

    for (int r=0; r < NB_ROPES; r++) {

        // Create all the capsules of the scene
        for (int i=0; i<NB_CAPSULES_PER_ROPE; i++) {

            const uint capsuleIndex = r * NB_CAPSULES_PER_ROPE + i;

            // Create a capsule and a corresponding rigid in the physics world
            mCapsules[capsuleIndex] = new Capsule(true, CAPSULE_RADIUS, CAPSULE_HEIGHT, mPhysicsCommon, mPhysicsWorld, meshFolderPath);

            // Set the capsule color
            mCapsules[capsuleIndex]->setColor(mObjectColorDemo);
            mCapsules[capsuleIndex]->setSleepingColor(mSleepingColorDemo);

            // Change the material properties of the rigid body
            rp3d::Material& material = mCapsules[capsuleIndex]->getCollider()->getMaterial();
            material.setBounciness(rp3d::decimal(0.0));
            material.setMassDensity(rp3d::decimal(0.1));

            mCapsules[capsuleIndex]->getRigidBody()->setAngularDamping(angularDamping);
            mCapsules[capsuleIndex]->getRigidBody()->setLinearDamping(linearDamping);

            if (i == 0) {
                mCapsules[capsuleIndex]->getRigidBody()->setType(rp3d::BodyType::STATIC);
            }

            // Add the capsule the list of capsules in the scene
            mPhysicsObjects.push_back(mCapsules[capsuleIndex]);
        }
    }

    // ---------- Create the first box --------- //

    // Create a box and a corresponding rigid in the physics world
    mBox1 = new Box(true, Vector3(BOX_SIZE, BOX_SIZE, BOX_SIZE), mPhysicsCommon, mPhysicsWorld, meshFolderPath);
    mBox1->getRigidBody()->setAngularDamping(angularDamping);
    mBox1->getRigidBody()->setLinearDamping(linearDamping);

    // Set the box color
    mBox1->setColor(mObjectColorDemo);
    mBox1->setSleepingColor(mSleepingColorDemo);

    // Change the material properties of the rigid body
    rp3d::Material& material1 = mBox1->getCollider()->getMaterial();
    material1.setBounciness(rp3d::decimal(0.0));
    material1.setMassDensity(rp3d::decimal(0.02));
    mBox1->getRigidBody()->updateMassPropertiesFromColliders();

    std::cout << "Mass: " << mBox1->getRigidBody()->getMass() << std::endl;

    mPhysicsObjects.push_back(mBox1);

    // ---------- Create the second box --------- //

    // Create a box and a corresponding rigid in the physics world
    mBox2 = new Box(true, Vector3(BOX_SIZE, BOX_SIZE, BOX_SIZE), mPhysicsCommon, mPhysicsWorld, meshFolderPath);
    mBox2->getRigidBody()->setAngularDamping(angularDamping);
    mBox2->getRigidBody()->setLinearDamping(linearDamping);

    // Set the box color
    mBox2->setColor(mObjectColorDemo);
    mBox2->setSleepingColor(mSleepingColorDemo);

    // Change the material properties of the rigid body
    rp3d::Material& material2 = mBox2->getCollider()->getMaterial();
    material2.setBounciness(rp3d::decimal(0.0));
    material2.setMassDensity(rp3d::decimal(0.7));
    mBox2->getRigidBody()->updateMassPropertiesFromColliders();

    mPhysicsObjects.push_back(mBox2);

    // ---------- Create plank box --------- //

    // Create a box and a corresponding rigid in the physics world
    mPlank = new Box(true, Vector3(10, 2, 15), mPhysicsCommon, mPhysicsWorld, meshFolderPath);
    mPlank->getRigidBody()->setType(rp3d::BodyType::STATIC);

    // Set the box color
    mPlank->setColor(mObjectColorDemo);
    mPlank->setSleepingColor(mSleepingColorDemo);

    // Change the material properties of the rigid body
    rp3d::Material& material3 = mPlank->getCollider()->getMaterial();
    material3.setBounciness(rp3d::decimal(0.5));

    mPhysicsObjects.push_back(mPlank);

    // Initialize the bodies positions
    initializeBodiesPositions();

    // Create the Ball-and-Socket joints
    createJoints();

    // Get the physics engine parameters
    mEngineSettings.isGravityEnabled = mPhysicsWorld->isGravityEnabled();
    rp3d::Vector3 gravityVector = mPhysicsWorld->getGravity();
    mEngineSettings.gravity = openglframework::Vector3(gravityVector.x, gravityVector.y, gravityVector.z);
    mEngineSettings.isSleepingEnabled = mPhysicsWorld->isSleepingEnabled();
    mEngineSettings.sleepLinearVelocity = mPhysicsWorld->getSleepLinearVelocity();
    mEngineSettings.sleepAngularVelocity = mPhysicsWorld->getSleepAngularVelocity();
    mEngineSettings.nbPositionSolverIterations = mPhysicsWorld->getNbIterationsPositionSolver();
    mEngineSettings.nbVelocitySolverIterations = mPhysicsWorld->getNbIterationsVelocitySolver();
    mEngineSettings.timeBeforeSleep = mPhysicsWorld->getTimeBeforeSleep();
}

// Destructor
RopeScene::~RopeScene() {

    // Destroy the joints
    for (uint i=0; i < mBallAndSocketJoints.size(); i++) {

        mPhysicsWorld->destroyJoint(mBallAndSocketJoints[i]);
    }

    // Destroy all the rigid bodies of the scene
    for (int r=0; r < NB_ROPES; r++) {
        for (int i=0; i<NB_CAPSULES_PER_ROPE; i++) {

            const uint capsuleIndex = r * NB_CAPSULES_PER_ROPE + i;
            mPhysicsWorld->destroyRigidBody(mCapsules[capsuleIndex]->getRigidBody());
        }
    }

    // Destroy the boxes
    mPhysicsWorld->destroyRigidBody(mBox1->getRigidBody());
    mPhysicsWorld->destroyRigidBody(mBox2->getRigidBody());

    // Destroy the physics world
    mPhysicsCommon.destroyPhysicsWorld(mPhysicsWorld);
}

// Create the joints
void RopeScene::createJoints() {

    for (int r=0; r < NB_ROPES; r++) {

        for (int i=0; i < NB_CAPSULES_PER_ROPE-1; i++) {

            const uint capsuleIndex = r * NB_CAPSULES_PER_ROPE + i;

            // Create the joint info object
            rp3d::RigidBody* body1 = mCapsules[capsuleIndex]->getRigidBody();
            rp3d::RigidBody* body2 = mCapsules[capsuleIndex+1]->getRigidBody();
            rp3d::Vector3 body1Position = body1->getTransform().getPosition();
            rp3d::Vector3 body2Position = body2->getTransform().getPosition();
            const rp3d::Vector3 anchorPointWorldSpace = 0.5f * (body1Position + body2Position);
            rp3d::BallAndSocketJointInfo jointInfo(body1, body2, anchorPointWorldSpace);
            jointInfo.isCollisionEnabled = false;
            rp3d::BallAndSocketJoint* joint = dynamic_cast<rp3d::BallAndSocketJoint*>(mPhysicsWorld->createJoint(jointInfo));
            mBallAndSocketJoints.push_back(joint);
        }

        // Create the joint between the rope and the box
        rp3d::RigidBody* body1 = mCapsules[r * NB_CAPSULES_PER_ROPE + NB_CAPSULES_PER_ROPE - 1]->getRigidBody();
        rp3d::RigidBody* body2 = r > 0 ? mBox2->getRigidBody() : mBox1->getRigidBody();
        rp3d::Vector3 body1Position = body1->getTransform().getPosition();
        const rp3d::Vector3 anchorPointWorldSpace = body1Position + rp3d::Vector3(0, -CAPSULE_HEIGHT * 0.5f , 0);
        rp3d::BallAndSocketJointInfo jointInfo(body1, body2, anchorPointWorldSpace);
        jointInfo.isCollisionEnabled = false;
        rp3d::BallAndSocketJoint* joint = dynamic_cast<rp3d::BallAndSocketJoint*>(mPhysicsWorld->createJoint(jointInfo));
        mBallAndSocketJoints.push_back(joint);
    }
}

void RopeScene::initializeBodiesPositions() {

    rp3d::Quaternion initOrientation = rp3d::Quaternion::identity();

    for (int r=0; r < NB_ROPES; r++) {

        rp3d::Vector3 initPosition = rp3d::Vector3(r == 0 ? -5 : 25, 15, 0);

        switch(r) {
            case 1: initPosition += rp3d::Vector3(BOX_SIZE, 0, -BOX_SIZE) * 0.5f; break;
            case 2: initPosition += rp3d::Vector3(BOX_SIZE, 0, BOX_SIZE) * 0.5f; break;
            case 3: initPosition += rp3d::Vector3(-BOX_SIZE, 0, BOX_SIZE) * 0.5f; break;
            case 4: initPosition += rp3d::Vector3(-BOX_SIZE, 0, -BOX_SIZE) * 0.5f; break;
            default: break;
        }

        for (int i=0; i<NB_CAPSULES_PER_ROPE; i++) {

            const uint capsuleIndex = r * NB_CAPSULES_PER_ROPE + i;

            initPosition += rp3d::Vector3(0, - CAPSULE_HEIGHT, 0);

            rp3d::Transform transform(initPosition, initOrientation);

            // Create a box and a corresponding rigid in the physics world
            mCapsules[capsuleIndex]->setTransform(transform);
        }
    }

    rp3d::Vector3 box1Position(-5, 15 - NB_CAPSULES_PER_ROPE * CAPSULE_HEIGHT - BOX_SIZE * 0.5, 0);
    const rp3d::Transform box1Transform(box1Position, rp3d::Quaternion::identity());
    mBox1->setTransform(box1Transform);

    rp3d::Vector3 box2Position(25, 15 - NB_CAPSULES_PER_ROPE * CAPSULE_HEIGHT - BOX_SIZE * 0.5, 0);
    const rp3d::Transform box2Transform(box2Position, rp3d::Quaternion::identity());
    mBox2->setTransform(box2Transform);

    rp3d::Vector3 plankPosition(-9, 5, 0);
    const rp3d::Transform plankTransform(plankPosition, rp3d::Quaternion::identity());
    mPlank->setTransform(plankTransform);
}

// Reset the scene
void RopeScene::reset() {

    SceneDemo::reset();

    // Initialize the bodies positions
    initializeBodiesPositions();

    nbIterations = 0;

    // ---------- Move the first rope in a horizontal position ---------- //

    rp3d::Quaternion initOrientation = rp3d::Quaternion::fromEulerAngles(0, 0, -90 * rp3d::PI_RP3D / 180.0f);

    for (int i=1; i<NB_CAPSULES_PER_ROPE-1; i++) {

        // Initial position and orientation of the rigid body
        rp3d::Vector3 initPosition;

        initPosition = rp3d::Vector3(-5 - (i + 0.5) * CAPSULE_HEIGHT, 15 - CAPSULE_HEIGHT, 0);

        rp3d::Transform transform(initPosition, initOrientation);

        // Create a box and a corresponding rigid in the physics world
        mCapsules[i]->setTransform(transform);
    }

    rp3d::Vector3 box1Position(-5 - (NB_CAPSULES_PER_ROPE + 0.5) * CAPSULE_HEIGHT - BOX_SIZE * 0.5, 15 - CAPSULE_HEIGHT, 0);
    const rp3d::Transform box1Transform(box1Position, initOrientation);
    mBox1->setTransform(box1Transform);
}

/// Update the physics world (take a simulation step)
/// Can be called several times per frame
void RopeScene::updatePhysics() {

    if (nbIterations < nbTorqueIterations) {
        mBox2->getRigidBody()->applyLocalTorque(rp3d::Vector3(0, 1000, 0));
    }

    nbIterations++;

    SceneDemo::updatePhysics();
}
