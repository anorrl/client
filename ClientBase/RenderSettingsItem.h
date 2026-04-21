#pragma once

// standard C/C++ Headers
#include <vector>

// Roblox Headers
#include "V8DataModel/GlobalSettings.h"
#include "GfxBase/RenderSettings.h"
#include "Reflection/Reflection.h"


extern const char* const sRenderSettings;
class CRenderSettingsItem : public ARL::GlobalAdvancedSettingsItem<CRenderSettingsItem, sRenderSettings>, public ARL::CRenderSettings
{
	std::string profileName;
	G3D::Vector2int16 currentDisplaySize;
	std::vector<G3D::Vector2int16> fullScreenSizes;
public:
	

	void setShowAggregation(bool value);

	static ARL::Reflection::BoundProp<std::string> prop_profileName;

	CRenderSettingsItem();
	
	void runProfiler(bool overwriteExistingValues);

	void setAASamples(AASamples value);

	void setFullscreenSize(G3D::Vector2int16 value);
	void setWindowSize(G3D::Vector2int16 value);

	bool isSynchronizedWithPhysics;

	void setGraphicsMode(GraphicsMode value);

	void setFrameRateManagerMode(FrameRateManagerMode value);
	void setAntialiasingMode(AntialiasingMode value);
	void setQualityLevel(QualityLevel value);
	void setEditQualityLevel(QualityLevel value);
	void setAutoQualityLevel(int level);
	void setResolutionPreference(ResolutionPreset value);

	void setMinCullDistance(int value);
	void setDebugShowBoundingBoxes(bool value);
	void setEagerBulkExecution(bool value);

	void setEnableFRM(bool value);

	void setTextureCacheSize(unsigned int size);
	void setMeshCacheSize(unsigned int size);

	bool getDebugDisableInterpolation() const;
	void setDebugDisableInterpolation(bool value);

	bool getShowInterpolationPath() const;
	void setShowInterpolationPath(bool value);

    bool getDebugReloadAssets() const;
    void setDebugReloadAssets(bool value);

    bool getObjExportMergeByMaterial() const;
    void setObjExportMergeByMaterial(bool value);

	static ARL::Reflection::EnumPropDescriptor<CRenderSettingsItem, CRenderSettingsItem::ResolutionPreset> prop_resolution;

	// special signal used only in native code
	rbx::signal<void(const ARL::Reflection::PropertyDescriptor*)> settingsChangedSignal;
};
