#include "stdafx.h"

#include "V8DataModel/GuiObject.h"
#include "V8DataModel/ScreenGui.h"
#include "V8DataModel/PlayerGui.h"
#include "V8DataModel/GuiService.h"
#include "GfxBase/Adorn.h"
#include "FastLog.h"

DYNAMIC_FASTFLAGVARIABLE(ForceIgnoreGuiInset, false)

namespace ARL {
	const char* const  sScreenGui = "ScreenGui";

    REFLECTION_BEGIN();
	static const Reflection::PropDescriptor<ScreenGui, Vector2int16> prop_ReplicateAbsoluteSize("ReplicatingAbsoluteSize", category_Data, &ScreenGui::getAbsoluteSize, &ScreenGui::setReplicatingAbsoluteSize,  Reflection::PropertyDescriptor::REPLICATE_ONLY);
	static const Reflection::PropDescriptor<ScreenGui, Vector2int16> prop_ReplicateAbsolutePosition("ReplicatingAbsolutePosition", category_Data, &ScreenGui::getAbsolutePosition, &ScreenGui::setReplicatingAbsolutePosition, Reflection::PropertyDescriptor::REPLICATE_ONLY);
	static const Reflection::PropDescriptor<ScreenGui, bool> prop_ignoreGuiInset("IgnoreGuiInset", category_Data, &ScreenGui::getIgnoreGuiInsetConst, &ScreenGui::setIgnoreGuiInset, Reflection::PropertyDescriptor::STANDARD, Security::None);
	REFLECTION_END();

	ScreenGui::ScreenGui()
		:DescribedCreatable<ScreenGui, GuiLayerCollector, sScreenGui>(sScreenGui)
		,renderable(false)
		,ignoreGuiInset(DFFlag::ForceIgnoreGuiInset)
		,bufferedViewport(Rect2D::xywh(0.0f, 0.0f, 800.0f, 600.0f))
	{
	}

	ScreenGui::ScreenGui(const char* name)
		:DescribedCreatable<ScreenGui, GuiLayerCollector, sScreenGui>(name)
		,renderable(false)
		,ignoreGuiInset(DFFlag::ForceIgnoreGuiInset)
	{
	}
    
    ScreenGui::~ScreenGui()
    {
        modalGuiObjects.clear();
        connections.clear();
    }

	void ScreenGui::setReplicatingAbsoluteSize(Vector2int16 value)
	{
		// We do need to handle a resize since it came in over the replication engine.
		handleResize(getRect2D(), false);
	}
	void ScreenGui::setReplicatingAbsolutePosition(Vector2int16 value)
	{
		// We do need to handle a resize since it came in over the replication engine.
		handleResize(getRect2D(), false);
	}
    
    void ScreenGui::onServiceProvider(ServiceProvider* oldProvider, ServiceProvider* newProvider)
    {
        if (oldProvider)
        {
            modalGuiObjects.clear();
            connections.clear();
        }
        
        
        Super::onServiceProvider(oldProvider, newProvider);
        
        if (newProvider)
        {
            // if we have core gui set up, get it's absolute size and use that (instead of 800x600)
            if(CoreGuiService* coreGui = ServiceProvider::create<CoreGuiService>(newProvider))
            {
                if (shared_ptr<ARL::ScreenGui> coreScreenGui = coreGui->getRobloxScreenGui())
                {
                    if (coreScreenGui.get() != this)
                    {
                        setBufferedViewport(coreScreenGui->getViewport());
                    }
                }
            }
        }
    }
    
	void ScreenGui::onPropertyChanged(const Reflection::PropertyDescriptor& descriptor)
	{
		Super::onPropertyChanged(descriptor);

		if(descriptor == Super::prop_AbsoluteSize){
			handleResize(bufferedViewport, false);
			raisePropertyChanged(prop_ReplicateAbsoluteSize);
		}
		else if(descriptor == Super::prop_AbsolutePosition){
			handleResize(bufferedViewport, false);
			raisePropertyChanged(prop_ReplicateAbsolutePosition);
		}
	}

	//The main windows parent can be any kind of non-GUI instance
	bool ScreenGui::askSetParent(const Instance* instance) const
	{
		return (Instance::fastDynamicCast<GuiBase2d>(instance) == NULL);
	}

	bool ScreenGui::isAncestorRenderableGui() const
	{
		const Instance* ancestor = getParent();
		while(ancestor != NULL)
		{
			if( ancestor->fastDynamicCast<BasePlayerGui>() )
				return true;
			ancestor = ancestor->getParent();
		}
		return false;
	}

	bool ScreenGui::canProcessMeAndDescendants() const
	{
        // Each ScreenGui is processed individually so we have to stop the hierarchy traversal
        // to eliminate duplicate processing of elements in nested ScreenGuis
        return false;
	}

	void ScreenGui::onAncestorChanged(const AncestorChanged& event)
	{
		Super::onAncestorChanged(event);

		bool newRenderable = isAncestorRenderableGui();

		if (newRenderable != renderable)
		{
			renderable = newRenderable;
			shouldRenderSetDirty();
		}

		handleResize(bufferedViewport, false);
	}

	void ScreenGui::render2d(Adorn* adorn)
	{
		render2dContext(adorn, NULL);
	}

	void ScreenGui::render2dContext(Adorn* adorn, const Instance* context)
	{
        setBufferedViewport(adorn->getUserGuiRect(!ignoreGuiInset));

		Super::render2dContext(adorn, context);
	}
    
    void ScreenGui::setBufferedViewport(Rect2D newViewport)
    {
        if (newViewport != bufferedViewport)
        {
			bufferedViewport = newViewport;
            handleResize(bufferedViewport, false);
		}
    }

	void ScreenGui::setIgnoreGuiInset(bool value) {
		if (value != ignoreGuiInset)
		{
			ignoreGuiInset = value;
			ignoreGuiInsetChangedSignal(value);
		}
	}
    
    Vector2 ScreenGui::getAbsolutePosition() const
    {
        if (GuiService* guiService = ARL::ServiceProvider::find<GuiService>(this))
        {
			if (!getIgnoreGuiInsetConst()) {
				Vector4 guiInset = guiService->getGlobalGuiInset();
				return Vector2(absolutePosition.x - guiInset.x, absolutePosition.y - guiInset.y);
			}
        }
        return absolutePosition;
    }

	GuiResponse ScreenGui::process(const shared_ptr<InputObject>& event)
	{
		// N.B.: HUD mouse message processing blowing us up on Mac; hack around it for now
		return Super::process(event);
	}
    
    GuiResponse ScreenGui::processGesture(const UserInputService::Gesture gesture, shared_ptr<const ARL::Reflection::ValueArray> touchPositions, shared_ptr<const Reflection::Tuple> args)
    {
        return Super::processGesture(gesture,touchPositions,args);
    }

	bool ScreenGui::removeModalButton(ARL::GuiButton* guiButton)
	{
		for(std::vector<GuiButton*>::iterator iter = modalGuiObjects.begin(); iter != modalGuiObjects.end(); ++iter)
		{
			if( (*iter) == guiButton )
			{
				modalGuiObjects.erase(iter);
				return true;
			}
		}
		return false;
	}

	bool ScreenGui::insertModalButton(ARL::GuiButton* guiButton)
	{
		for(std::vector<GuiButton*>::iterator iter = modalGuiObjects.begin(); iter != modalGuiObjects.end(); ++iter)
			if( (*iter) == guiButton )
				return false;

		modalGuiObjects.push_back(guiButton);
		return true;
	}

	void ScreenGui::onModalButtonChanged(const ARL::Reflection::PropertyDescriptor* desc, ARL::GuiButton* guiButton)
	{
		if(guiButton->getModal())
			insertModalButton(guiButton);
		else
			removeModalButton(guiButton);
	}

	void ScreenGui::onDescendantAdded(Instance* instance)
	{
        Super::onDescendantAdded(instance);
        
		if(ARL::GuiButton* guiButton = Instance::fastDynamicCast<ARL::GuiButton>(instance))
		{
            connections[instance] = guiButton->propertyChangedSignal.connect(boost::bind(&ScreenGui::onModalButtonChanged,this,_1,guiButton));
            
			if(guiButton->getModal())
				insertModalButton(guiButton);
		}
	}
	void ScreenGui::onDescendantRemoving(const shared_ptr<Instance>& instance)
	{
        Super::onDescendantRemoving(instance);
        
		if(ARL::GuiButton* guiButton = Instance::fastDynamicCast<ARL::GuiButton>(instance.get()))
		{
			removeModalButton(guiButton);
            connections.erase(instance.get());
		}
	}

	bool ScreenGui::hasModalDialog()
	{
		for(std::vector<GuiButton*>::iterator iter = modalGuiObjects.begin(); iter != modalGuiObjects.end(); ++iter)
		{
			if( (*iter)->isCurrentlyVisible() )
				return true;
		}
		return false;
	}

	const char* const  sGuiMain = "GuiMain";
	GuiMain::GuiMain()
		:DescribedCreatable<GuiMain, ScreenGui, sGuiMain>(sGuiMain)
	{}

}
