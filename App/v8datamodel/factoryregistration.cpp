/* Copyright 2003-2010 ROBLOX Corporation, All Rights Reserved */
#include "stdafx.h"

#include "V8DataModel/factoryregistration.h"

#include "FastLog.h"

#include "V8DataModel/Adornment.h"
#include "V8DataModel/BillboardGui.h"
#include "v8datamodel/SurfaceGui.h"
#include "V8DataModel/Accoutrement.h"
#include "V8DataModel/Backpack.h"
#include "V8DataModel/BadgeService.h"
#include "V8DataModel/BasicPartInstance.h"
#include "V8DataModel/BevelMesh.h"
#include "V8DataModel/BlockMesh.h"
#include "v8datamodel/CacheableContentProvider.h"
#include "V8DataModel/Camera.h"
#include "V8DataModel/ChangeHistory.h"
#include "V8DataModel/CharacterAppearance.h"
#include "V8DataModel/CharacterMesh.h"
#include "V8DataModel/ChatService.h"
#include "V8DataModel/ClickDetector.h"
#include "V8DataModel/ContentProvider.h"
#include "V8DataModel/Configuration.h"
#include "V8DataModel/CollectionService.h"
#include "V8DataModel/CSGDictionaryService.h"
#include "V8DataModel/CustomEvent.h"
#include "V8DataModel/CustomEventReceiver.h"
#include "V8DataModel/CylinderMesh.h"
#include "V8DataModel/VirtualUser.h"
#include "V8DataModel/LogService.h"
#include "V8DataModel/DataModelMesh.h"
#include "V8DataModel/DebrisService.h"
#include "V8DataModel/Decal.h"
#include "V8DataModel/DialogRoot.h"
#include "V8DataModel/DialogChoice.h"
#include "V8DataModel/DebugSettings.h"
#include "V8DataModel/PhysicsSettings.h"
#include "V8DataModel/ExtrudedPartInstance.h"
#include "V8DataModel/FriendService.h"
#include "V8DataModel/Folder.h"
#include "V8DataModel/RenderHooksService.h"
#include "V8DataModel/Test.h"
#include "V8DataModel/CookiesEngineService.h"
#include "V8DataModel/TeleportService.h"
#include "V8DataModel/PersonalServerService.h"
#include "V8DataModel/ScriptService.h"
#include "V8DataModel/UserInputService.h"
#include "v8datamodel/AssetService.h"
#include "v8datamodel/HttpService.h"
#include "v8datamodel/HttpRbxApiService.h"
#include "v8datamodel/DataStoreService.h"
#include "v8datamodel/TerrainRegion.h"
#include "v8datamodel/PathfindingService.h"
#include "v8datamodel/StarterPlayerService.h"
#include "v8datamodel/HandleAdornment.h"
#include "util/CellID.h"

#ifdef _PRISM_PYRAMID_
#include "V8DataModel/PrismInstance.h"
#include "V8DataModel/PyramidInstance.h"
#include "V8DataModel/ParallelRampInstance.h"
#include "V8DataModel/RightAngleRampInstance.h"
#include "V8DataModel/CornerWedgeInstance.h"
#endif

#include "V8DataModel/Explosion.h"
#include "V8DataModel/FaceInstance.h"
#include "V8DataModel/Feature.h"
#include "V8DataModel/FileMesh.h"
#include "V8DataModel/Fire.h"
#include "V8DataModel/Flag.h"
#include "V8DataModel/FlagStand.h"
#include "V8DataModel/FlyweightService.h"
#include "V8DataModel/ForceField.h"
#include "V8DataModel/GameSettings.h"
#include "V8DataModel/GameBasicSettings.h"
#include "Script/LuaSettings.h"
#include "Script/DebuggerManager.h"
#include "Script/ModuleScript.h"
#include "Script/LuaSourceContainer.h"
#include "V8DataModel/GeometryService.h"
#include "V8DataModel/GlobalSettings.h"
#include "V8DataModel/Gyro.h"
#include "V8DataModel/Handles.h"
#include "V8DataModel/HandlesBase.h"
#include "V8DataModel/ArcHandles.h"
#include "V8DataModel/Hopper.h"
#include "V8DataModel/JointInstance.h"
#include "V8DataModel/JointsService.h"
#include "V8DataModel/Lighting.h"
#include "V8DataModel/MeshContentProvider.h"
#include "V8DataModel/Message.h"
#include "V8DataModel/Mouse.h"
#include "V8DataModel/NonReplicatedCSGDictionaryService.h"
#include "V8DataModel/ParametricPartInstance.h"
#include "V8DataModel/PartInstance.h"
#include "V8DataModel/PhysicsService.h"
#include "V8DataModel/Platform.h"
#include "V8DataModel/PlayerGui.h"
#include "V8DataModel/PlayerScripts.h"
#include "V8DataModel/PlayerMouse.h"
#include "V8DataModel/PluginManager.h"
#include "V8DataModel/PluginMouse.h"
#include "V8DataModel/Seat.h"
#include "V8DataModel/SelectionBox.h"
#include "V8DataModel/SelectionSphere.h"
#include "V8dataModel/Sky.h"
#include "V8dataModel/SkateboardPlatform.h"
#include "V8DataModel/SkateboardController.h"
#include "V8DataModel/Smoke.h"
#include "v8datamodel/CustomParticleEmitter.h"
#include "V8DataModel/SolidModelContentProvider.h"
#include "V8DataModel/Sparkles.h"
#include "V8DataModel/SpawnLocation.h"
#include "V8Datamodel/SpecialMesh.h"
#include "V8DataModel/Stats.h"
#include "V8DataModel/SurfaceSelection.h"
#include "V8DataModel/Teams.h"
#include "V8DataModel/TextService.h"
#include "V8DataModel/TextureContentProvider.h"
#include "V8DataModel/TimerService.h"
#include "V8DataModel/Tool.h"
#include "V8DataModel/StudioTool.h"
#include "V8DataModel/TouchTransmitter.h"
#include "V8DataModel/usercontroller.h"
#include "V8DataModel/Value.h"
#include "V8DataModel/VehicleSeat.h"
#include "V8DataModel/Visit.h"
#include "V8DataModel/Workspace.h"
#include "V8DataModel/LocalWorkspace.h"
#include "Humanoid/Humanoid.h"
#include "Humanoid/StatusInstance.h"
#include "Humanoid/HumanoidState.h"
#include "script/scriptcontext.h"
#include "Script/Script.h"
#include "Script/CoreScript.h"
#include "V8DataModel/MarketplaceService.h"
#include "V8DataModel/GuiService.h"
#include "V8DataModel/GuiBase.h"
#include "V8DataModel/GuiBase3d.h"
#include "V8DataModel/TweenService.h"
#include "V8DataModel/GuiObject.h"
#include "V8DataModel/ScreenGui.h"
#include "V8DataModel/Frame.h"
#include "V8DataModel/Scale9Frame.h"
#include "V8DataModel/ImageButton.h"
#include "V8DataModel/ImageLabel.h"
#include "V8DataModel/TextButton.h"
#include "V8DataModel/TextLabel.h"
#include "V8DataModel/TextBox.h"
#include "V8DataModel/SelectionLasso.h"
#include "V8DataModel/TextureTrail.h"
#include "V8DataModel/FloorWire.h"
#include "V8DataModel/Animation.h"
#include "V8DataModel/AnimationController.h"
#include "V8DataModel/AnimationTrack.h"
#include "V8DataModel/AnimationTrackState.h"
#include "V8DataModel/Animator.h"
#include "V8DataModel/KeyframeSequenceProvider.h"
#include "V8DataModel/KeyframeSequence.h"
#include "V8DataModel/Keyframe.h"
#include "V8DataModel/Pose.h"
#include "V8DataModel/MegaCluster.h"
#include "V8DataModel/Bindable.h"
#include "V8DataModel/Light.h"
#include "V8DataModel/Remote.h"
#include "V8DataModel/PartOperation.h"
#include "V8DataModel/PartOperationAsset.h"
#include "V8DataModel/Attachment.h"
#include "v8datamodel/TouchInputService.h"
#include "v8datamodel/HapticService.h"

#include "util/CellID.h"

#include "network/NetworkPacketCache.h"
#include "network/NetworkClusterPacketCache.h"
#include "network/ChatFilter.h"

#include "util/Sound.h"
#include "util/SoundService.h"
#include "util/UDim.h"
#include "util/Faces.h"
#include "util/Axes.h"
#include "Util/ScriptInformationProvider.h"
#include "util/Action.h"
#include "util/Region3.h"
#include "util/KeywordFilter.h"
#include "util/SystemAddress.h"
#include "util/LuaWebService.h"
#include "util/rbxrandom.h"
#include "util/RunStateOwner.h"
#include "util/PhysicalProperties.h"
#include "V8DataModel/InsertService.h"
#include "V8DataModel/SocialService.h"
#include "V8DataModel/GamePassService.h"
#include "V8DataModel/ContextActionService.h"
#include "V8DataModel/LoginService.h"
#include "Util/ContentFilter.h"
#include "Tool/LuaDragger.h"
#include "Tool/AdvLuaDragger.h"
#include "RbxG3D/RbxTime.h"
#include "v8datamodel/InputObject.h"
#include "v8datamodel/ReplicatedStorage.h"
#include "v8datamodel/RobloxReplicatedStorage.h"
#include "v8datamodel/ReplicatedFirst.h"
#include "v8datamodel/ServerScriptService.h"
#include "v8datamodel/ServerStorage.h"
#include "util/standardout.h"
#include "util/KeyCode.h"
#include "v8datamodel/PointsService.h"
#include "v8datamodel/ScrollingFrame.h"
#include "V8datamodel/AdService.h"
#include "V8datamodel/NotificationService.h"
#include "V8datamodel/GroupService.h"
#include "V8datamodel/GamepadService.h"

#include "v8datamodel/NumberSequence.h"
#include "v8datamodel/NumberRange.h"
#include "v8datamodel/ColorSequence.h"



using namespace ARL;

ARL_REGISTER_TYPE(void);
ARL_REGISTER_TYPE(bool);
ARL_REGISTER_TYPE(float);
ARL_REGISTER_TYPE(int);
ARL_REGISTER_TYPE(long);
ARL_REGISTER_TYPE(double);
ARL_REGISTER_TYPE(std::string);
ARL_REGISTER_TYPE(ARL::ProtectedString);
ARL_REGISTER_TYPE(const Reflection::PropertyDescriptor*);
ARL_REGISTER_TYPE(ARL::BrickColor);
ARL_REGISTER_TYPE(ARL::SystemAddress);
ARL_REGISTER_TYPE(ARL::MeshId);
ARL_REGISTER_TYPE(ARL::AnimationId);
ARL_REGISTER_TYPE(boost::shared_ptr<const Reflection::Tuple>);
ARL_REGISTER_TYPE(G3D::Vector3);
ARL_REGISTER_TYPE(G3D::Vector3int16);
ARL_REGISTER_TYPE(ARL::Region3);
ARL_REGISTER_TYPE(ARL::RbxRay);
ARL_REGISTER_TYPE(G3D::Rect2D);
ARL_REGISTER_TYPE(ARL::PhysicalProperties);
ARL_REGISTER_TYPE(ARL::Vector2);
ARL_REGISTER_TYPE(G3D::Vector2int16);
ARL_REGISTER_TYPE(G3D::Color3);
ARL_REGISTER_TYPE(G3D::CoordinateFrame);
ARL_REGISTER_TYPE(ARL::ContentId);
ARL_REGISTER_TYPE(ARL::TextureId);
ARL_REGISTER_TYPE(ARL::UDim);
ARL_REGISTER_TYPE(ARL::UDim2);
ARL_REGISTER_TYPE(ARL::Faces);
ARL_REGISTER_TYPE(ARL::Axes);
ARL_REGISTER_TYPE(boost::shared_ptr<const Instances>);
ARL_REGISTER_TYPE(boost::shared_ptr<class Reflection::DescribedBase>);
ARL_REGISTER_TYPE(boost::shared_ptr<class ARL::Instance>);
ARL_REGISTER_TYPE(Lua::WeakFunctionRef);
ARL_REGISTER_TYPE(shared_ptr<Lua::GenericFunction>);
ARL_REGISTER_TYPE(shared_ptr<Lua::GenericAsyncFunction>);
ARL_REGISTER_TYPE(boost::shared_ptr<const Reflection::ValueArray>);
ARL_REGISTER_TYPE(boost::shared_ptr<const Reflection::ValueMap>);
ARL_REGISTER_TYPE(boost::shared_ptr<const Reflection::ValueTable>);
ARL_REGISTER_TYPE(CellID);
ARL_REGISTER_TYPE(Soundscape::SoundId);
ARL_REGISTER_TYPE(ARL::NumberSequenceKeypoint);
ARL_REGISTER_TYPE(ARL::ColorSequenceKeypoint);
ARL_REGISTER_TYPE(ARL::NumberSequence);
ARL_REGISTER_TYPE(ARL::ColorSequence);
ARL_REGISTER_TYPE(ARL::NumberRange);
ARL_REGISTER_TYPE(ARL::Guid::Data);

ARL_REGISTER_CLASS(InputObject);
ARL_REGISTER_CLASS(TestService);
ARL_REGISTER_CLASS(FunctionalTest);
ARL_REGISTER_CLASS(Lighting);
ARL_REGISTER_CLASS(DebugSettings);
ARL_REGISTER_CLASS(PhysicsSettings);
ARL_REGISTER_CLASS(TaskSchedulerSettings);
ARL_REGISTER_CLASS(DataModel);
ARL_REGISTER_CLASS(PhysicsService);
ARL_REGISTER_CLASS(BadgeService);
ARL_REGISTER_CLASS(DialogRoot);
ARL_REGISTER_CLASS(DialogChoice);
ARL_REGISTER_CLASS(Tool);
ARL_REGISTER_CLASS(StudioTool);
ARL_REGISTER_CLASS(LuaDragger);
ARL_REGISTER_CLASS(AdvLuaDragger);
ARL_REGISTER_CLASS(Accoutrement);
ARL_REGISTER_CLASS(Backpack);
ARL_REGISTER_CLASS(BodyColors);
ARL_REGISTER_CLASS(ClickDetector);
ARL_REGISTER_CLASS(ControllerService);
ARL_REGISTER_CLASS(ChatService);
ARL_REGISTER_CLASS(TextService);
ARL_REGISTER_CLASS(VirtualUser);
ARL_REGISTER_CLASS(Explosion);
ARL_REGISTER_CLASS(Team);
ARL_REGISTER_CLASS(Instance);
ARL_REGISTER_CLASS(Flag);
ARL_REGISTER_CLASS(FlagStand);
ARL_REGISTER_CLASS(FlagStandService);
ARL_REGISTER_CLASS(ForceField);
ARL_REGISTER_CLASS(Fire);
ARL_REGISTER_CLASS(GameSettings);
ARL_REGISTER_CLASS(GameBasicSettings);
ARL_REGISTER_CLASS(GeometryService);
ARL_REGISTER_CLASS(Settings);
ARL_REGISTER_CLASS(GlobalAdvancedSettings);
ARL_REGISTER_CLASS(GlobalBasicSettings);
ARL_REGISTER_CLASS(Hat);
ARL_REGISTER_CLASS(Accessory);
ARL_REGISTER_CLASS(Hint);
ARL_REGISTER_CLASS(Humanoid);
ARL_REGISTER_CLASS(StatusInstance);
ARL_REGISTER_CLASS(RunService);
ARL_REGISTER_CLASS(LegacyHopperService);
ARL_REGISTER_CLASS(LocalScript);
ARL_REGISTER_CLASS(LocalWorkspace);
ARL_REGISTER_CLASS(LuaSettings);
ARL_REGISTER_CLASS(CoreScript);
ARL_REGISTER_CLASS(Message);
ARL_REGISTER_CLASS(Selection);
ARL_REGISTER_CLASS(ShirtGraphic);
ARL_REGISTER_CLASS(Shirt);
ARL_REGISTER_CLASS(Pants);
ARL_REGISTER_CLASS(Smoke);
ARL_REGISTER_CLASS(CustomParticleEmitter);
ARL_REGISTER_CLASS(Sparkles);
ARL_REGISTER_CLASS(StarterPackService);
ARL_REGISTER_CLASS(StarterPlayerService);
ARL_REGISTER_CLASS(StarterGuiService);
ARL_REGISTER_CLASS(UserInputService);
ARL_REGISTER_CLASS(CoreGuiService);
ARL_REGISTER_CLASS(StarterGear);
ARL_REGISTER_CLASS(Visit);
ARL_REGISTER_CLASS(ObjectValue);
ARL_REGISTER_CLASS(IntValue);
ARL_REGISTER_CLASS(DoubleValue);
ARL_REGISTER_CLASS(BoolValue);
ARL_REGISTER_CLASS(StringValue);
ARL_REGISTER_CLASS(BinaryStringValue);
ARL_REGISTER_CLASS(Vector3Value);
ARL_REGISTER_CLASS(RayValue);
ARL_REGISTER_CLASS(CFrameValue);
ARL_REGISTER_CLASS(Color3Value);
ARL_REGISTER_CLASS(BrickColorValue);
ARL_REGISTER_CLASS(IntConstrainedValue);
ARL_REGISTER_CLASS(DoubleConstrainedValue);
ARL_REGISTER_CLASS(Platform);
ARL_REGISTER_CLASS(SkateboardPlatform);
ARL_REGISTER_CLASS(Seat);
ARL_REGISTER_CLASS(VehicleSeat);
ARL_REGISTER_CLASS(DebrisService);
ARL_REGISTER_CLASS(TimerService);
ARL_REGISTER_CLASS(SpawnerService);
ARL_REGISTER_CLASS(ContentFilter);
ARL_REGISTER_CLASS(InsertService);
ARL_REGISTER_CLASS(LuaWebService);
ARL_REGISTER_CLASS(FriendService);
ARL_REGISTER_CLASS(RenderHooksService);
ARL_REGISTER_CLASS(CookiesService);
ARL_REGISTER_CLASS(SocialService);
ARL_REGISTER_CLASS(GamePassService);
ARL_REGISTER_CLASS(MarketplaceService);
ARL_REGISTER_CLASS(GroupService);
ARL_REGISTER_CLASS(ContextActionService);
ARL_REGISTER_CLASS(PersonalServerService);
ARL_REGISTER_CLASS(AssetService);
ARL_REGISTER_CLASS(ScriptService);
ARL_REGISTER_CLASS(ContentProvider);
ARL_REGISTER_CLASS(MeshContentProvider);
ARL_REGISTER_CLASS(TextureContentProvider);
ARL_REGISTER_CLASS(SolidModelContentProvider);
ARL_REGISTER_CLASS(CacheableContentProvider);
ARL_REGISTER_CLASS(ChangeHistoryService);
ARL_REGISTER_CLASS(HttpService);
ARL_REGISTER_CLASS(HttpRbxApiService);
ARL_REGISTER_CLASS(DataStoreService);
ARL_REGISTER_CLASS(PathfindingService);
ARL_REGISTER_CLASS(Path);
ARL_REGISTER_CLASS(Clothing);
ARL_REGISTER_CLASS(Skin);
ARL_REGISTER_CLASS(CharacterMesh);
ARL_REGISTER_CLASS(DataModelMesh);
ARL_REGISTER_CLASS(FileMesh);
ARL_REGISTER_CLASS(SpecialShape);
ARL_REGISTER_CLASS(BevelMesh);
ARL_REGISTER_CLASS(BlockMesh);
ARL_REGISTER_CLASS(CylinderMesh);
//ARL_REGISTER_CLASS(EggMesh);
ARL_REGISTER_CLASS(ServiceProvider);
ARL_REGISTER_CLASS(RootInstance);
ARL_REGISTER_CLASS(ModelInstance);
ARL_REGISTER_CLASS(BaseScript);
ARL_REGISTER_CLASS(Script);
ARL_REGISTER_CLASS(ScriptContext);
ARL_REGISTER_CLASS(RuntimeScriptService);
ARL_REGISTER_CLASS(ScriptInformationProvider);
ARL_REGISTER_CLASS(Workspace);
ARL_REGISTER_CLASS(Controller);
ARL_REGISTER_CLASS(HumanoidController);
ARL_REGISTER_CLASS(VehicleController);
ARL_REGISTER_CLASS(SkateboardController);
ARL_REGISTER_CLASS(Pose);
ARL_REGISTER_CLASS(Keyframe);
ARL_REGISTER_CLASS(KeyframeSequence);
ARL_REGISTER_CLASS(KeyframeSequenceProvider);
ARL_REGISTER_CLASS(Animation);
ARL_REGISTER_CLASS(AnimationController);
ARL_REGISTER_CLASS(AnimationTrack);
ARL_REGISTER_CLASS(AnimationTrackState);
ARL_REGISTER_CLASS(Animator);
ARL_REGISTER_CLASS(TeleportService);
ARL_REGISTER_CLASS(CharacterAppearance);
ARL_REGISTER_CLASS(LogService);
ARL_REGISTER_CLASS(ScrollingFrame);
ARL_REGISTER_CLASS(FlyweightService);
ARL_REGISTER_CLASS(CSGDictionaryService);
ARL_REGISTER_CLASS(NonReplicatedCSGDictionaryService);
ARL_REGISTER_CLASS(TouchInputService);


// network
ARL_REGISTER_CLASS(Network::PhysicsPacketCache);
ARL_REGISTER_CLASS(Network::InstancePacketCache);
ARL_REGISTER_CLASS(Network::ClusterPacketCache);
ARL_REGISTER_CLASS(Network::OneQuarterClusterPacketCache);
ARL_REGISTER_CLASS(Network::ChatFilter);

// Joints - in alpha order
ARL_REGISTER_CLASS(JointsService);
ARL_REGISTER_CLASS(Glue);
ARL_REGISTER_CLASS(Motor);
ARL_REGISTER_CLASS(Motor6D);
ARL_REGISTER_CLASS(Rotate);
ARL_REGISTER_CLASS(RotateP);
ARL_REGISTER_CLASS(RotateV);
ARL_REGISTER_CLASS(Snap);
ARL_REGISTER_CLASS(Weld);
ARL_REGISTER_CLASS(ManualSurfaceJointInstance);
ARL_REGISTER_CLASS(ManualWeld);
ARL_REGISTER_CLASS(ManualGlue);
ARL_REGISTER_CLASS(BodyMover);
ARL_REGISTER_CLASS(TouchTransmitter);
ARL_REGISTER_CLASS(FaceInstance);
ARL_REGISTER_CLASS(Sky);
ARL_REGISTER_CLASS(PVInstance);
ARL_REGISTER_CLASS(VelocityMotor);
ARL_REGISTER_CLASS(Feature);
ARL_REGISTER_CLASS(DynamicRotate);
ARL_REGISTER_CLASS(JointInstance);
ARL_REGISTER_CLASS(Attachment);
ARL_REGISTER_CLASS(SpawnLocation);
ARL_REGISTER_CLASS(Mouse);
ARL_REGISTER_CLASS(PlayerMouse);
ARL_REGISTER_CLASS(Teams);
ARL_REGISTER_CLASS(BackpackItem);
ARL_REGISTER_CLASS(HopperBin);
ARL_REGISTER_CLASS(Camera);
ARL_REGISTER_CLASS(BasePlayerGui);
ARL_REGISTER_CLASS(PlayerGui);
ARL_REGISTER_CLASS(PlayerScripts);
ARL_REGISTER_CLASS(StarterPlayerScripts);
ARL_REGISTER_CLASS(StarterCharacterScripts);
ARL_REGISTER_CLASS(PartInstance);
ARL_REGISTER_CLASS(FormFactorPart);
ARL_REGISTER_CLASS(BasicPartInstance);
ARL_REGISTER_CLASS(ExtrudedPartInstance);
ARL_REGISTER_CLASS(PART::Wedge);
ARL_REGISTER_CLASS(Decal);
ARL_REGISTER_CLASS(DecalTexture);
ARL_REGISTER_CLASS(TweenService);
ARL_REGISTER_CLASS(GuiItem);
ARL_REGISTER_CLASS(GuiBase);
ARL_REGISTER_CLASS(GuiBase2d);
ARL_REGISTER_CLASS(GuiBase3d);
ARL_REGISTER_CLASS(GuiRoot);
ARL_REGISTER_CLASS(GuiObject);
ARL_REGISTER_CLASS(GuiButton);
ARL_REGISTER_CLASS(GuiLabel);
ARL_REGISTER_CLASS(GuiMain);
ARL_REGISTER_CLASS(GuiLayerCollector);
ARL_REGISTER_CLASS(BillboardGui);
ARL_REGISTER_CLASS(ScreenGui);
ARL_REGISTER_CLASS(SurfaceGui);
ARL_REGISTER_CLASS(SelectionLasso);
ARL_REGISTER_CLASS(SelectionPartLasso);
ARL_REGISTER_CLASS(SelectionPointLasso);
ARL_REGISTER_CLASS(TextureTrail);
ARL_REGISTER_CLASS(FloorWire);
ARL_REGISTER_CLASS(GuiService);
ARL_REGISTER_CLASS(Frame);
ARL_REGISTER_CLASS(Scale9Frame);
ARL_REGISTER_CLASS(GuiImageButton);
ARL_REGISTER_CLASS(ImageLabel);
ARL_REGISTER_CLASS(GuiTextButton);
ARL_REGISTER_CLASS(TextBox);
ARL_REGISTER_CLASS(TextLabel);
ARL_REGISTER_CLASS(PartAdornment);
ARL_REGISTER_CLASS(PVAdornment);
ARL_REGISTER_CLASS(Handles);
ARL_REGISTER_CLASS(HandlesBase);
ARL_REGISTER_CLASS(ArcHandles);
ARL_REGISTER_CLASS(SelectionBox);
ARL_REGISTER_CLASS(SelectionSphere);
ARL_REGISTER_CLASS(HandleAdornment);
ARL_REGISTER_CLASS(BoxHandleAdornment);
ARL_REGISTER_CLASS(ConeHandleAdornment);
ARL_REGISTER_CLASS(CylinderHandleAdornment);
ARL_REGISTER_CLASS(SphereHandleAdornment);
ARL_REGISTER_CLASS(LineHandleAdornment);
ARL_REGISTER_CLASS(ImageHandleAdornment);
ARL_REGISTER_CLASS(SurfaceSelection);
ARL_REGISTER_CLASS(CollectionService);
ARL_REGISTER_CLASS(Configuration);
ARL_REGISTER_CLASS(Folder);
ARL_REGISTER_CLASS(MotorFeature);
ARL_REGISTER_CLASS(Hole);
ARL_REGISTER_CLASS(MegaClusterInstance);
ARL_REGISTER_CLASS(PluginMouse);
ARL_REGISTER_CLASS(PluginManager);
ARL_REGISTER_CLASS(Plugin);
ARL_REGISTER_CLASS(Toolbar);
ARL_REGISTER_CLASS(ARL::Button);
//Conditional parts here
#ifdef _PRISM_PYRAMID_
ARL_REGISTER_CLASS(PrismInstance);
ARL_REGISTER_CLASS(PyramidInstance);
ARL_REGISTER_CLASS(ParallelRampInstance);
ARL_REGISTER_CLASS(RightAngleRampInstance);
ARL_REGISTER_CLASS(CornerWedgeInstance);
#endif // _PRISM_PYRAMID_
ARL_REGISTER_CLASS(CustomEvent);
ARL_REGISTER_CLASS(CustomEventReceiver);
//ARL_REGISTER_CLASS(PropertyInstance);
ARL_REGISTER_CLASS(BindableFunction);
ARL_REGISTER_CLASS(BindableEvent);
ARL_REGISTER_CLASS(ARL::Scripting::DebuggerManager);
ARL_REGISTER_CLASS(ARL::Scripting::ScriptDebugger);
ARL_REGISTER_CLASS(ARL::Scripting::DebuggerBreakpoint);
ARL_REGISTER_CLASS(ARL::Scripting::DebuggerWatch);
ARL_REGISTER_CLASS(Light);
ARL_REGISTER_CLASS(PointLight);
ARL_REGISTER_CLASS(SpotLight);
ARL_REGISTER_CLASS(SurfaceLight);
ARL_REGISTER_CLASS(LoginService);
ARL_REGISTER_CLASS(ReplicatedStorage);
ARL_REGISTER_CLASS(RobloxReplicatedStorage);
ARL_REGISTER_CLASS(ServerScriptService);
ARL_REGISTER_CLASS(ServerStorage);
ARL_REGISTER_CLASS(RemoteFunction);
ARL_REGISTER_CLASS(RemoteEvent);
ARL_REGISTER_CLASS(TerrainRegion);
ARL_REGISTER_CLASS(ModuleScript);
ARL_REGISTER_CLASS(PointsService);
ARL_REGISTER_CLASS(AdService);
ARL_REGISTER_CLASS(NotificationService);
ARL_REGISTER_CLASS(ReplicatedFirst);
ARL_REGISTER_CLASS(PartOperation);
ARL_REGISTER_CLASS(PartOperationAsset);
ARL_REGISTER_CLASS(UnionOperation);
ARL_REGISTER_CLASS(NegateOperation);
ARL_REGISTER_CLASS(Soundscape::SoundService);
ARL_REGISTER_CLASS(Soundscape::SoundChannel);
ARL_REGISTER_CLASS(GamepadService);
ARL_REGISTER_CLASS(LuaSourceContainer);
ARL_REGISTER_CLASS(HapticService);

// Xbox
#if defined(ARL_PLATFORM_DURANGO)
#include "v8datamodel/PlatformService.h"
ARL_REGISTER_CLASS(PlatformService);
#endif

static void onSlotException(std::exception& ex)
{
	FASTLOG(FLog::Error, "Slot Exception");
	ARL::StandardOut::singleton()->printf(MESSAGE_ERROR, "exception while signalling: %s", ex.what());
}

FactoryRegistrator::FactoryRegistrator()
{
	G3D::System::time();// Initialize the Program Start Time.
	registerSound();
	ARL::registerScriptDescriptors();
	registerBodyMovers();

	registerValueClasses();
	ARL::registerStatsClasses();
	ARL::Surface::registerSurfaceDescriptors();

	rbx::signals::slot_exception_handler = onSlotException;

	srand(ARL::randomSeed());

	ModelInstance::hackPhysicalCharacter();
}

// Enum types
ARL_REGISTER_ENUM(ChangeHistoryService::RuntimeUndoBehavior);
ARL_REGISTER_ENUM(FunctionalTest::Result);
ARL_REGISTER_ENUM(TaskScheduler::PriorityMethod);
ARL_REGISTER_ENUM(TaskScheduler::Job::SleepAdjustMethod);
ARL_REGISTER_ENUM(TaskScheduler::ThreadPoolConfig);
ARL_REGISTER_ENUM(Action::ActionType);
ARL_REGISTER_ENUM(Controller::Button);
ARL_REGISTER_ENUM(HopperBin::BinType);
ARL_REGISTER_ENUM(GuiObject::SizeConstraint);
ARL_REGISTER_ENUM(GuiObject::TweenEasingStyle);
ARL_REGISTER_ENUM(GuiObject::TweenStatus);
ARL_REGISTER_ENUM(GuiObject::TweenEasingDirection);
ARL_REGISTER_ENUM(TextService::XAlignment);
ARL_REGISTER_ENUM(TextService::YAlignment);
ARL_REGISTER_ENUM(TextService::FontSize);
ARL_REGISTER_ENUM(TextService::Font);
ARL_REGISTER_ENUM(Camera::CameraType);
ARL_REGISTER_ENUM(Camera::CameraMode);
ARL_REGISTER_ENUM(Camera::CameraPanMode);
ARL_REGISTER_ENUM(LegacyController::InputType);
ARL_REGISTER_ENUM(DataModelArbiter::ConcurrencyModel);
ARL_REGISTER_ENUM(DataModelMesh::LODType);
ARL_REGISTER_ENUM(DebugSettings::ErrorReporting);
ARL_REGISTER_ENUM(EThrottle::EThrottleType);
ARL_REGISTER_ENUM(Feature::InOut);
ARL_REGISTER_ENUM(Feature::LeftRight);
ARL_REGISTER_ENUM(Feature::TopBottom);
ARL_REGISTER_ENUM(Joint::JointType);
ARL_REGISTER_ENUM(KeywordFilterType);
ARL_REGISTER_ENUM(Legacy::SurfaceConstraint);
ARL_REGISTER_ENUM(NormalId);
ARL_REGISTER_ENUM(Vector3::Axis);
ARL_REGISTER_ENUM(Humanoid::Status);
ARL_REGISTER_ENUM(Humanoid::HumanoidRigType);
ARL_REGISTER_ENUM(Humanoid::NameOcclusion);
ARL_REGISTER_ENUM(Humanoid::HumanoidDisplayDistanceType);
ARL_REGISTER_ENUM(ARL::HUMAN::StateType);
ARL_REGISTER_ENUM(DataModel::CreatorType);
ARL_REGISTER_ENUM(DataModel::Genre);
ARL_REGISTER_ENUM(DataModel::GearGenreSetting);
ARL_REGISTER_ENUM(DataModel::GearType);
ARL_REGISTER_ENUM(Instance::SaveFilter);
ARL_REGISTER_ENUM(BasicPartInstance::LegacyPartType);
ARL_REGISTER_ENUM(KeyframeSequence::Priority);
ARL_REGISTER_ENUM(SocialService::StuffType);
ARL_REGISTER_ENUM(PersonalServerService::PrivilegeType);
ARL_REGISTER_ENUM(ExtrudedPartInstance::VisualTrussStyle);
#ifdef _PRISM_PYRAMID_
ARL_REGISTER_ENUM(PrismInstance::NumSidesEnum);
ARL_REGISTER_ENUM(PyramidInstance::NumSidesEnum);
#endif
ARL_REGISTER_ENUM(FriendService::FriendStatus);
ARL_REGISTER_ENUM(FriendService::FriendEventType);
ARL_REGISTER_ENUM(Handles::VisualStyle);
ARL_REGISTER_ENUM(SkateboardPlatform::MoveState);
ARL_REGISTER_ENUM(SoundType);
ARL_REGISTER_ENUM(SpecialShape::MeshType);
ARL_REGISTER_ENUM(SurfaceType);
ARL_REGISTER_ENUM(PartInstance::FormFactor);
ARL_REGISTER_ENUM(CollisionFidelity);
ARL_REGISTER_ENUM(UserInputService::SwipeDirection);
ARL_REGISTER_ENUM(UserInputService::Platform);
ARL_REGISTER_ENUM(UserInputService::MouseType);
ARL_REGISTER_ENUM(PartMaterial);
ARL_REGISTER_ENUM(PhysicalPropertiesMode);
ARL_REGISTER_ENUM(NetworkOwnership);
ARL_REGISTER_ENUM(Time::SampleMethod);
ARL_REGISTER_ENUM(GuiService::SpecialKey);
ARL_REGISTER_ENUM(GuiService::CenterDialogType);
ARL_REGISTER_ENUM(GuiService::UiMessageType);
ARL_REGISTER_ENUM(ChatService::ChatColor);
ARL_REGISTER_ENUM(MarketplaceService::CurrencyType);
ARL_REGISTER_ENUM(MarketplaceService::InfoType);
ARL_REGISTER_ENUM(CharacterMesh::BodyPart);
ARL_REGISTER_ENUM(GameSettings::VideoQuality);
ARL_REGISTER_ENUM(GameSettings::UploadSetting);
ARL_REGISTER_ENUM(GameBasicSettings::ControlMode);
ARL_REGISTER_ENUM(GameBasicSettings::RenderQualitySetting);
ARL_REGISTER_ENUM(GameBasicSettings::CameraMode);
ARL_REGISTER_ENUM(GameBasicSettings::TouchCameraMovementMode);
ARL_REGISTER_ENUM(GameBasicSettings::ComputerCameraMovementMode);
ARL_REGISTER_ENUM(GameBasicSettings::TouchMovementMode);
ARL_REGISTER_ENUM(GameBasicSettings::ComputerMovementMode);
ARL_REGISTER_ENUM(GameBasicSettings::RotationType);
ARL_REGISTER_ENUM(Frame::Style);
ARL_REGISTER_ENUM(GuiButton::Style);
ARL_REGISTER_ENUM(DialogRoot::DialogPurpose);
ARL_REGISTER_ENUM(DialogRoot::DialogTone);
ARL_REGISTER_ENUM(Voxel::CellMaterial);
ARL_REGISTER_ENUM(Voxel::CellBlock);
ARL_REGISTER_ENUM(Voxel::CellOrientation);
ARL_REGISTER_ENUM(Voxel::WaterCellForce);
ARL_REGISTER_ENUM(Voxel::WaterCellDirection);
ARL_REGISTER_ENUM(Explosion::ExplosionType);
ARL_REGISTER_ENUM(InputObject::UserInputType);
ARL_REGISTER_ENUM(InputObject::UserInputState);
ARL_REGISTER_ENUM(AssetService::AccessType);
ARL_REGISTER_ENUM(HttpService::HttpContentType);
ARL_REGISTER_ENUM(StarterGuiService::CoreGuiType);
ARL_REGISTER_ENUM(StarterPlayerService::DeveloperTouchCameraMovementMode);
ARL_REGISTER_ENUM(StarterPlayerService::DeveloperComputerCameraMovementMode);
ARL_REGISTER_ENUM(StarterPlayerService::DeveloperCameraOcclusionMode);
ARL_REGISTER_ENUM(StarterPlayerService::DeveloperTouchMovementMode);
ARL_REGISTER_ENUM(StarterPlayerService::DeveloperComputerMovementMode);
ARL_REGISTER_ENUM(TeleportService::TeleportState);
ARL_REGISTER_ENUM(TeleportService::TeleportType);
ARL_REGISTER_ENUM(KeyCode);
ARL_REGISTER_ENUM(MessageType);
ARL_REGISTER_ENUM(MarketplaceService::ProductPurchaseDecision);
ARL_REGISTER_ENUM(ThrottlingPriority);
ARL_REGISTER_ENUM(Soundscape::ReverbType);
ARL_REGISTER_ENUM(Soundscape::ListenerType);
ARL_REGISTER_ENUM(Soundscape::RollOffMode);
ARL_REGISTER_ENUM(PlayerActionType);
ARL_REGISTER_ENUM(RunService::RenderPriority);
ARL_REGISTER_ENUM(AdvArrowToolBase::JointCreationMode);
ARL_REGISTER_ENUM(GuiObject::ImageScale);
ARL_REGISTER_ENUM(UserInputService::OverrideMouseIconBehavior);
ARL_REGISTER_ENUM(Pose::PoseEasingStyle);
ARL_REGISTER_ENUM(Pose::PoseEasingDirection);
ARL_REGISTER_ENUM(HapticService::VibrationMotor);
ARL_REGISTER_ENUM(UserInputService::UserCFrame);

#if defined(ARL_PLATFORM_DURANGO)
ARL_REGISTER_ENUM(XboxKeyBoardType)
ARL_REGISTER_ENUM(VoiceChatState)
#endif
