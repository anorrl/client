
#include "boost/smart_ptr/shared_ptr.hpp"

#include "v8datamodel/PartInstance.h"
#include "v8datamodel/DataModelMesh.h"
#include "GfxBase/FileMeshData.h"
#include "v8datamodel/FileMesh.h"
#include "v8datamodel/PartOperation.h"

namespace ARL
{

    extern const char* const sMeshPartInstance;

    class MeshPartInstance : public DescribedCreatable<MeshPartInstance, PartInstance, sMeshPartInstance>
    {
    protected:
        boost::shared_ptr<FileMeshData> meshData;
        boost::shared_ptr<FileMeshData> oldMeshData;
        ContentId meshId;
        ContentId textureId;
        bool doubleSided;
        CollisionFidelity collisionFidelity;
        BinaryString physicsData;
        Vector3 initialSize;

    public:
        MeshPartInstance();
        ~MeshPartInstance();

        static const Reflection::PropDescriptor<MeshPartInstance, ContentId> prop_meshPartId;
        static const Reflection::PropDescriptor<MeshPartInstance, ContentId> prop_meshPartTextureId;
        static const Reflection::PropDescriptor<MeshPartInstance, bool> prop_doubleSided;
        static const Reflection::EnumPropDescriptor<MeshPartInstance, CollisionFidelity> prop_CollisionFidelity;
        static const Reflection::PropDescriptor<MeshPartInstance, BinaryString> prop_PhysicsData;

        const ContentId& getTextureId() const
        {
            return textureId;
        };
        const ContentId& getMeshId() const
        {
            return meshId;
        };
        const bool& getDoubleSided() const
        {
            return doubleSided;
        }
        boost::shared_ptr<FileMeshData> getMeshData() const
        {
            return meshData;
        }
        virtual void setMeshData(boost::shared_ptr<FileMeshData> data)
        {
            if (oldMeshData != meshData)
                oldMeshData = meshData;

            meshData = data;
            trySetPhysicsData();
        };
        virtual void setTextureId(const ContentId& textureId);
        virtual void setMeshId(const ContentId& meshId);
        virtual void setDoubleSided(const bool& value);

        void setCollisionFidelity(const CollisionFidelity value);
        CollisionFidelity getCollisionFidelity() const
        {
            return collisionFidelity;
        }

        const BinaryString& getPhysicsData() const
        {
            return physicsData;
        }
        void setPhysicsData(const BinaryString& mData);
        bool hasPhysicsData() const
        {
            return physicsData.value() != "";
        }

        bool hasMeshData() const
        {
            return meshData != nullptr && meshData->vnts.size() > 0 && meshData->faces.size() > 0;
        }

        bool isActualMeshIdSet();
        bool isActualTextureIdSet();

        bool createPhysicsData(const FileMeshData* mData);
        void trySetPhysicsData();
        void setBulletCollisionObject();

        bool checkDecompExists();
        std::string generateHashKey(const std::string& dataString) const;
        void setBulletObjectsScale(const Vector3& newBoundingBoxSize);
        Vector3 calculateSizeDifference(const Vector3& newBoundingBoxSize);
        Vector3 calculateAdjustedSizeDifference(const Vector3& newBoundingBoxSize);
        /*override*/ virtual void setPartSizeXml(const Vector3& rbxSize);
        /*override*/ virtual bool hasThreeDimensionalSize();
        /* override */ virtual PartType getPartType() const;
    };
} // namespace ARL