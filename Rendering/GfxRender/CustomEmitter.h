#pragma once

#include "util/G3DCore.h"

#include "RenderNode.h"

namespace ARL
{
    class PartInstance;
    class Instance;
    class CustomParticleEmitter;
}

namespace ARL
{
namespace Graphics
{

class Emitter;

class CustomEmitter: public RenderNode
{
    typedef RenderNode Super;

public:
    CustomEmitter(VisualEngine* visualEngine);
    ~CustomEmitter();

    void bind(const shared_ptr<ARL::PartInstance>& part, const shared_ptr<ARL::Instance>& instance);
    
    // GfxBinding overrides
    virtual void invalidateEntity();
    virtual void updateEntity(bool assetsUpdated);
    virtual void unbind();

    // GfxPart overrides
    virtual void updateCoordinateFrame(bool recalcLocalBounds);

    virtual void updateRenderQueue(RenderQueue& queue, const RenderCamera& camera, RenderQueue::Pass pass);

private:
    enum { 
        kDirty_None = 0,
        kDirty_Invalidate = 1,
        kDirty_Transform = 2, 
        kDirty_Fast = 4, 
        kDirty_Slow = 8,
        kDirty_Curves = 16,
        kDirty_All = ~(unsigned)kDirty_Invalidate,
    }; // dirty flags

    typedef const ARL::Reflection::PropertyDescriptor* pd;

    void onCombinedSignalEx(Instance::CombinedSignalType type, const Instance::ICombinedSignalData* data);
    void onPropertyChangedEx(const ARL::Reflection::PropertyDescriptor* descriptor);
    void onAncestorChangedEx();
    void onSleepingChangedEx(bool sleeping);
	void requestEmit(int particleCount);

    void onParentSize(pd p);

    void applyAllSettings();
    
    shared_ptr<ARL::PartInstance> part;
    shared_ptr<ARL::CustomParticleEmitter> effect;
    CoordinateFrame transform;
    Extents localBox;
    bool enabled;

    unsigned dirtyFlags;

    scoped_ptr<Emitter> emitter;

	int requestedEmitCount;
};

}
}
