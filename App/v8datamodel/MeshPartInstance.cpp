#include "stdafx.h"
#include "v8datamodel/MeshPartInstance.h"
#include "util/MD5Hasher.h"
#include "v8world/TriangleMesh.h"
#include "v8world/Primitive.h"
#include "util/BinaryString.h"
#include "util/RunStateOwner.h"
#include "util/StandardOut.h"

#include <vector>

namespace ARL
{

    const char* const sMeshPartInstance = "MeshPart";

    const Reflection::PropDescriptor<MeshPartInstance, ContentId> MeshPartInstance::prop_meshPartId("MeshId", category_Appearance, &MeshPartInstance::getMeshId, &MeshPartInstance::setMeshId);
    const Reflection::PropDescriptor<MeshPartInstance, ContentId> MeshPartInstance::prop_meshPartTextureId("TextureID", category_Appearance, &MeshPartInstance::getTextureId, &MeshPartInstance::setTextureId);
    const Reflection::PropDescriptor<MeshPartInstance, bool> MeshPartInstance::prop_doubleSided("DoubleSided", category_Appearance, &MeshPartInstance::getDoubleSided, &MeshPartInstance::setDoubleSided, Reflection::PropertyDescriptor::PUBLIC_SERIALIZED);
    const Reflection::EnumPropDescriptor<MeshPartInstance, CollisionFidelity> MeshPartInstance::prop_CollisionFidelity("CollisionFidelity", category_Behavior, &MeshPartInstance::getCollisionFidelity, &MeshPartInstance::setCollisionFidelity, Reflection::PropertyDescriptor::PUBLIC_SERIALIZED);
    const Reflection::PropDescriptor<MeshPartInstance, BinaryString> MeshPartInstance::prop_PhysicsData("PhysicsData", category_Data, &MeshPartInstance::getPhysicsData, &MeshPartInstance::setPhysicsData, Reflection::PropertyDescriptor::STREAMING, Security::ANORRL);
    
    MeshPartInstance::MeshPartInstance()
        : collisionFidelity(CollisionFidelity_Default)
        , initialSize(1.0f, 1.0f, 1.0f)
        , doubleSided(false)
    {
        setName("MeshPart");
        getPrimitive(this)->setGeometryType(Geometry::GEOMETRY_TRI_MESH);
    }

    MeshPartInstance::~MeshPartInstance() {}

    PartType MeshPartInstance::getPartType() const
    {
        return MESH_PART;
    }

    bool MeshPartInstance::isActualMeshIdSet()
    {
        return !(!this->getMeshId().isAssetId() && !this->getMeshId().isAsset() && !this->getMeshId().isHttp() &&
            !this->getMeshId().isARLHttp());
    }

    bool MeshPartInstance::isActualTextureIdSet()
    {
        return !(!this->getTextureId().isAssetId() && !this->getTextureId().isAsset() &&
            !this->getTextureId().isHttp() && !this->getTextureId().isARLHttp());
    }

    void MeshPartInstance::setMeshId(const ContentId& value)
    {
        if (meshId != value)
        {
            meshId = value;

            raisePropertyChanged(prop_meshPartId);
            updatePrimitiveState();
        }
    }

    void MeshPartInstance::setTextureId(const ContentId& value)
    {
        if (textureId != value)
        {
            textureId = value;
            raisePropertyChanged(prop_meshPartTextureId);
        }
    }

    void MeshPartInstance::setDoubleSided(const bool& value)
    {
        if (doubleSided != value)
        {
            doubleSided = value;
            raisePropertyChanged(prop_doubleSided);
        }
    }

    void MeshPartInstance::setCollisionFidelity(const CollisionFidelity value)
    {
        if (value != getCollisionFidelity())
        {
            collisionFidelity = value;
            raisePropertyChanged(prop_CollisionFidelity);
            RunService* rs = ServiceProvider::find<RunService>(this);
            if (rs && (rs->getRunState() == RS_RUNNING || rs->getRunState() == RS_PAUSED))
            {
                ARL::StandardOut::singleton()->printf(MESSAGE_WARNING, "Cannot change MeshPart CollisionFidelity during Run-Time");
            }
        }
    }

    void MeshPartInstance::setPhysicsData(const BinaryString& mData)
    {
        if (physicsData != mData)
        {
            physicsData = mData;
            raisePropertyChanged(prop_PhysicsData);
        }

        trySetPhysicsData();
    }

    bool MeshPartInstance::createPhysicsData(const FileMeshData* mData)
    {
        if (!mData || mData->vnts.empty() || mData->faces.empty())
            return false;

        // Convert FileMeshData to bullet format and calculate bounds
        std::vector<btVector3> vertices;
        std::vector<unsigned int> indices;

        vertices.reserve(mData->vnts.size());

        // Calculate mesh bounds to set initialSize properly
        Vector3 minBounds(FLT_MAX, FLT_MAX, FLT_MAX);
        Vector3 maxBounds(-FLT_MAX, -FLT_MAX, -FLT_MAX);

        for (const auto& vnt : mData->vnts)
        {
            vertices.push_back(btVector3(vnt.vx, vnt.vy, vnt.vz));
            minBounds.x = std::min(minBounds.x, vnt.vx);
            minBounds.y = std::min(minBounds.y, vnt.vy);
            minBounds.z = std::min(minBounds.z, vnt.vz);
            maxBounds.x = std::max(maxBounds.x, vnt.vx);
            maxBounds.y = std::max(maxBounds.y, vnt.vy);
            maxBounds.z = std::max(maxBounds.z, vnt.vz);
        }

        // Set initialSize to actual mesh bounds (this is the "unit" size for scaling)
        initialSize = maxBounds - minBounds;

        // Prevent zero or near-zero dimensions
        if (initialSize.x < 0.01f)
            initialSize.x = 0.01f;
        if (initialSize.y < 0.01f)
            initialSize.y = 0.01f;
        if (initialSize.z < 0.01f)
            initialSize.z = 0.01f;

        indices.reserve(mData->faces.size() * 3);
        for (const auto& face : mData->faces)
        {
            indices.push_back(face.a);
            indices.push_back(face.b);
            indices.push_back(face.c);
        }

        // Generate physics decomposition data from the mesh
        std::string data;
        ARL::CollisionFidelity fidelity = getCollisionFidelity();

        try
        {
            switch (fidelity)
            {
            case CollisionFidelity_Default:
                data = TriangleMesh::generateDecompositionData(mData->faces.size(), &indices[0], mData->vnts.size(), (btScalar*)&vertices[0].x());
                getPrimitive(this)->resetGeometryType(Geometry::GEOMETRY_TRI_MESH);
                break;
            case CollisionFidelity_Hull:
                data = TriangleMesh::generateConvexHullData(mData->faces.size(), &indices[0], mData->vnts.size(), &vertices[0]);
                getPrimitive(this)->resetGeometryType(Geometry::GEOMETRY_TRI_MESH);
                break;
            case CollisionFidelity_Box:
                data = TriangleMesh::getBlockData();
                getPrimitive(this)->resetGeometryType(Geometry::GEOMETRY_BLOCK);
                break;
            default:
                return false;
            }

            setPhysicsData(BinaryString(data));
        }
        catch (std::exception& e)
        {
            StandardOut::singleton()->printf(MESSAGE_ERROR, "Generating MeshPart decomp physics data for %s failed: %s", getName().c_str(), e.what());
            return false;
        }

        return true;
    }

    std::string MeshPartInstance::generateHashKey(const std::string& data) const
    {
        boost::scoped_ptr<ARL::MD5Hasher> hasher(ARL::MD5Hasher::create());
        hasher->addData(data);
        return hasher->c_str();
    }


    Vector3 MeshPartInstance::calculateSizeDifference(const Vector3& newBoundingBoxSize)
    {
        Vector3 v = newBoundingBoxSize / initialSize;

        // If we are adjusting for BulletMargin we CANNOT force symmetry
        if (formFactor == PartInstance::SYMETRIC)
        {
            float uniformScale = std::min(std::min(v.x, v.y), v.z);
            return Vector3(uniformScale, uniformScale, uniformScale);
        }

        return v;
    }

    Vector3 MeshPartInstance::calculateAdjustedSizeDifference(const Vector3& newBoundingBoxSize)
    {
        Vector3 result = calculateSizeDifference(newBoundingBoxSize) - ((Vector3(2 * bulletCollisionMargin)) / initialSize);
        return result;
    }

    void MeshPartInstance::trySetPhysicsData()
    {
        TriangleMesh* primitiveMesh = arl_static_cast<TriangleMesh*>(getPrimitive(this)->getGeometry());

        if ((!primitiveMesh->getCompound() && hasMeshData()) || (primitiveMesh->getCompound() && oldMeshData != meshData))
        {
            oldMeshData = meshData;

            // Check if we have physics data before attempting to set it
            if (!hasPhysicsData())
            {
                // If no physics data exists but we have mesh data, generate it
                if (hasMeshData())
                {
                    createPhysicsData(meshData.get());
                }
                else
                {
                    return; // Can't create physics without mesh data
                }
            }

            // Calculate shrunken scale
            const Vector3 scale = calculateAdjustedSizeDifference(getPartSizeXml());
            const Vector3 meshScale = calculateSizeDifference(getPartSizeXml());
            std::string hashKey = generateHashKey(getPhysicsData().value());

            if (primitiveMesh->setCompoundMeshData(hashKey, getPhysicsData().value(), btVector3(scale.x, scale.y, scale.z)))
            {
                primitiveMesh->setStaticMeshData(hashKey, getPhysicsData().value(), btVector3(meshScale.x, meshScale.y, meshScale.z));
                setBulletCollisionObject();
                return;
            }
        }
    }

    void MeshPartInstance::setBulletCollisionObject()
    {
        getPrimitive(this)->getGeometry()->setUpBulletCollisionData();
    }

    void MeshPartInstance::setPartSizeXml(const Vector3& rbxSize)
    {
        if (rbxSize != getPartSizeXml())
        {
            // Has to happen before Primitive->setSize
            setBulletObjectsScale(rbxSize);

            getPartPrimitive()->setSize(rbxSize);

            raisePropertyChanged(prop_Size);

            refreshPartSizeUi();
        }
    }


    bool MeshPartInstance::checkDecompExists()
    {
        if (getPrimitive(this)->getGeometryType() == Geometry::GEOMETRY_TRI_MESH)
        {
            TriangleMesh* primitiveMesh = arl_static_cast<TriangleMesh*>(getPrimitive(this)->getGeometry());
            if (primitiveMesh->getCompound())
                return true;
        }
        return false;
    }

    void MeshPartInstance::setBulletObjectsScale(const Vector3& newBoundingBoxSize)
    {
        if (checkDecompExists())
        {
            TriangleMesh* primitiveMesh = arl_static_cast<TriangleMesh*>(getPrimitive(this)->getGeometry());
            primitiveMesh->updateObjectScale(generateHashKey(getPhysicsData().value()), getPhysicsData().value(),
                calculateAdjustedSizeDifference(newBoundingBoxSize), calculateSizeDifference(newBoundingBoxSize));
        }
    }

    bool MeshPartInstance::hasThreeDimensionalSize()
    {
        return formFactor != SYMETRIC;
    }


}