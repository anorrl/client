#pragma once

#include "GfxBase/GfxPart.h"
#include "util/Extents.h"

#include "RenderQueue.h"

namespace ARL
{	
	class Contact;
}

namespace ARL
{
namespace Graphics
{

class RenderCamera;
class RenderQueue;
class VisualEngine;

class SpatialHashNode;

class CullableSceneNode: public ARL::GfxPart
{
public:
    static const bool hasGetFirstContact = false; // simulate __if_exists
    static ARL::Contact* getContact(CullableSceneNode* p0, CullableSceneNode* p1) {ARLASSERT(0); return NULL;}  // shouldn't be called if hasGetFirstContact is false
    static ARL::Contact* getFirstContact() { ARLASSERT(NULL); return NULL;}
    static ARL::Contact* getNextContact(ARL::Contact* prev) { ARLASSERT(NULL); return NULL;}
    static int getNumContacts(){ ARLASSERT(NULL); return 0; }
    static CullableSceneNode* getContactOther(int id){ ARLASSERT(NULL); return NULL; }

    bool requestFixed() const { return false; }
    const Extents& getFastFuzzyExtents() { return worldBounds; }

public:
    enum CullMode
    {
        CullMode_None,
        CullMode_Simple,
        CullMode_SpatialHash,
    };
    
    enum Flags
    {
        Flags_LightObject = 1 << 0,
        Flags_ShadowCaster = 1 << 1,
    };

    CullableSceneNode(VisualEngine* visualEngine, CullMode cullMode, unsigned int flags);
    ~CullableSceneNode();

    void setBlockCount(int blockCount) { this->blockCount = blockCount; }

    virtual void updateRenderQueue(RenderQueue& queue, const RenderCamera& camera, RenderQueue::Pass pass);

    float getSqDistanceToFocus() const { return sqDistanceToFocus; }

    const Extents& getWorldBounds() const { return worldBounds; }

    void updateWorldBounds(const Extents& aabb);

    VisualEngine* getVisualEngine() const { return visualEngine; }
    
    unsigned int getFlags() const { return flags; }

    bool updateIsCulledByFRM();

    Vector3 getCenter() const override { return worldBounds.center(); }

private:
    VisualEngine* visualEngine;

    CullMode cullMode;
    unsigned int flags;

    int blockCount;
    float sqDistanceToFocus;

    Extents worldBounds;
};

}
}
