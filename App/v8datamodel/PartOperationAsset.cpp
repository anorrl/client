/* Copyright 2014 ROBLOX Corporation, All Rights Reserved */
#include "stdafx.h"

#include "v8datamodel/PartOperationAsset.h"
#include "v8datamodel/PartOperation.h"
#include "v8datamodel/GameBasicSettings.h"

#include "g3d/g3dmath.h"
#include "g3d/CollisionDetection.h"
#include "network/Players.h"
#include "reflection/Reflection.h"
#include "arl/rbxTime.h"
#include "util/BinaryString.h"
#include "util/NormalId.h"
#include "util/SurfaceType.h"
#include "util/stringbuffer.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/Workspace.h"
#include "v8datamodel/CSGMesh.h"
#include "v8world/ContactManager.h"
#include "v8world/Primitive.h"
#include "v8world/World.h"
#include "v8world/BulletGeometryPoolObjects.h"
#include "v8datamodel/CSGDictionaryService.h"
#include "v8datamodel/NonReplicatedCSGDictionaryService.h"
#include "v8datamodel/FlyweightService.h"
#include "v8world/TriangleMesh.h"
#include "v8datamodel/ContentProvider.h"
#include "v8xml/Serializer.h"
#include "v8xml/SerializerBinary.h"
#include "v8datamodel/SolidModelContentProvider.h"

FASTFLAGVARIABLE(CSGFixForNoChildData, true)

namespace ARL
{
using namespace Reflection;

const Reflection::PropDescriptor<PartOperationAsset, BinaryString> PartOperationAsset::desc_ChildData("ChildData", category_Data, &PartOperationAsset::getChildData, &PartOperationAsset::setChildData, Reflection::PropertyDescriptor::CLUSTER, Security::ANORRL);
const Reflection::PropDescriptor<PartOperationAsset, BinaryString> PartOperationAsset::desc_MeshData("MeshData", category_Data, &PartOperationAsset::getMeshData, &PartOperationAsset::setMeshData, Reflection::PropertyDescriptor::STREAMING, Security::ANORRL);

const char* const sPartOperationAsset = "PartOperationAsset";

void setAssetOnMatchingPartOperations(shared_ptr<Instance> descendant, const ContentId& url, const BinaryString& key)
{
    if(PartOperation* partOperation = ARL::Instance::fastDynamicCast<PartOperation>(descendant.get()))
    {
        if (partOperation->hasAsset())
            return;

        if (partOperation->getChildData() == key)
        {
            partOperation->setAssetId(url);

			BinaryString noValue;
			partOperation->setChildData(noValue);
			partOperation->setMeshData(noValue);
        }
    }
}

void publishPartOperations(shared_ptr<Instance> descendant, ARL::Time startTime, const int timeoutMills)
{
    if (timeoutMills != -1 && (startTime - ARL::Time::nowFast()).msec() > timeoutMills)
        return;

    if(shared_ptr<PartOperation> partOperation = Instance::fastSharedDynamicCast<PartOperation>(descendant))
    {
        if (partOperation->hasAsset())
		{
			const BinaryString childKey = partOperation->getChildData();
			const BinaryString meshKey = partOperation->getMeshData();
			
			if (meshKey.value().empty() && childKey.value().empty())
				return;

			ARL::ContentId contentId = partOperation->getAssetId();
			
			DataModel* dataModel = DataModel::get(partOperation.get());

			if (CacheableContentProvider* mcp = ServiceProvider::create<SolidModelContentProvider>(dataModel))
			{
				if (boost::shared_ptr<PartOperationAsset> partOperationAsset = boost::static_pointer_cast<PartOperationAsset>(mcp->blockingRequestContent(contentId, true)))
				{
					//ChildData
					if (!childKey.value().empty() && !partOperationAsset->getChildData().value().empty())
					{
						NonReplicatedCSGDictionaryService* nrDictionaryService = ServiceProvider::find<NonReplicatedCSGDictionaryService>(dataModel);

						if (nrDictionaryService->isHashKey(childKey.value()))
							nrDictionaryService->retrieveData(*partOperation);

						partOperation->setChildData(BinaryString(""));
					}

					//MeshData
					if (!meshKey.value().empty() && !partOperationAsset->getMeshData().value().empty())
					{
						CSGDictionaryService* dictionaryService = ServiceProvider::find< CSGDictionaryService >(dataModel);

						if (dictionaryService->isHashKey(meshKey.value()))
							dictionaryService->retrieveMeshData(*partOperation);

						partOperation->setMeshData(BinaryString(""));
					}
				}
			}

			return;
		}

        DataModel* dataModel = DataModel::get(partOperation.get());

        CSGDictionaryService* dictionaryService = ServiceProvider::create< CSGDictionaryService >(dataModel);
        NonReplicatedCSGDictionaryService* nrDictionaryService = ServiceProvider::create<NonReplicatedCSGDictionaryService>(dataModel);
        ContentProvider* contentProvider = ServiceProvider::create<ContentProvider>(dataModel);

        const BinaryString meshKey = partOperation->getMeshData();
        const BinaryString childKey = partOperation->getChildData();

        if (meshKey.value().empty() || childKey.value().empty())
            return;

        if (!dictionaryService->isHashKey(meshKey.value()) || !nrDictionaryService->isHashKey(childKey.value()))
        {
            return;
        }

        const BinaryString meshData = dictionaryService->peekAtData(meshKey);
        BinaryString childData = nrDictionaryService->peekAtData(childKey);

        if (childData.value().empty())
            childData = dictionaryService->peekAtData(childKey);
        bool validChildData = !childData.value().empty();

        shared_ptr<PartOperationAsset> partOperationAsset = Creatable<Instance>::create<PartOperationAsset>();
        partOperationAsset->setMeshData(meshData);
        partOperationAsset->setChildData(childData);

        std::stringstream stream;
        ARL::Instances instances;
        instances.push_back(partOperationAsset);
        SerializerBinary::serialize(stream, instances);

        std::string baseUrl = contentProvider->getBaseUrl();
        ARL::Http http(ARL::format("%s/ide/publish/uploadnewasset?assetTypeName=SolidModel&name=SolidModel&description=SolidModel&isPublic=True&genreTypeId=1&allowComments=False", baseUrl.c_str()));
        try
        {
            std::string response;
            http.post(stream, ARL::Http::kContentTypeApplicationXml, true, response);

            int newAssetId;
            std::stringstream istream(response);
            istream >> newAssetId;
            std::string assetId;
            assetId = ARL::format("%s/asset/?id=%d", baseUrl.c_str(), newAssetId);
            ContentId contentId = ARL::ContentId(assetId.c_str());

            partOperation->setAssetId(contentId);
            
			BinaryString noValue;
			partOperation->setChildData(noValue);
			partOperation->setMeshData(noValue);
            
            if (validChildData || !FFlag::CSGFixForNoChildData)
                dataModel->visitDescendants(boost::bind(&setAssetOnMatchingPartOperations, _1, contentId, childKey));
            
            if (validChildData || !FFlag::CSGFixForNoChildData)
                dictionaryService->removeStringData(meshKey);
            nrDictionaryService->removeStringData(childKey);
        }
        catch(std::exception&)
        {
            throw DataModel::SerializationException("Failed to upload union.  Exceeded limit.  Try again in a few minutes.");
        }
        // awagnerTODO: pass childata to LRU cache in content provider
    }
}

bool PartOperationAsset::publishAll(DataModel* dataModel, int timeoutMills)
{
    ARL::Time startPublish = ARL::Time::nowFast();

    dataModel->visitDescendants(boost::bind(&publishPartOperations, _1, startPublish, timeoutMills));
	
	CSGDictionaryService* dictionaryService = ServiceProvider::find< CSGDictionaryService >(dataModel);
	NonReplicatedCSGDictionaryService* nrDictionaryService = ServiceProvider::find<NonReplicatedCSGDictionaryService>(dataModel);

	dictionaryService->clean();
	nrDictionaryService->clean();

    return true;
}

bool PartOperationAsset::publishSelection(DataModel* dataModel, int timeoutMills)
{
    ARL::Time startPublish = ARL::Time::nowFast();

    ARL::Selection* sel = ARL::ServiceProvider::create<ARL::Selection>(dataModel);
    for (ARL::Instances::const_iterator iter = sel->begin(); iter != sel->end(); ++iter)
    {
        publishPartOperations(*iter, startPublish, timeoutMills);
    }

	CSGDictionaryService* dictionaryService = ServiceProvider::find< CSGDictionaryService >(dataModel);
	NonReplicatedCSGDictionaryService* nrDictionaryService = ServiceProvider::find<NonReplicatedCSGDictionaryService>(dataModel);

	dictionaryService->clean();
	nrDictionaryService->clean();

    return true;
}

} //namespace
