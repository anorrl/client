/* Copyright 2003-2007 ROBLOX Corporation, All Rights Reserved */
#include "stdafx.h"

#include "v8datamodel/PluginMouse.h"
#include "v8datamodel/PluginManager.h"
#include "v8world/ContactManager.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/Workspace.h"
#include "v8world/World.h"

namespace ARL {

const char *const sPluginMouse = "PluginMouse";

REFLECTION_BEGIN();
static Reflection::EventDesc<PluginMouse, void(shared_ptr<const Instances>)> evt_dragEnterEvent(&PluginMouse::dragEnterEventSignal, "DragEnter", "instances", Security::Plugin);
REFLECTION_END();


PluginMouse::PluginMouse()
{
}

PluginMouse::~PluginMouse()
{
}

void PluginMouse::fireDragEnterEvent(shared_ptr<const ARL::Instances> instances, shared_ptr<InputObject> input)
{
	update(input);
	dragEnterEventSignal(instances);
}

} // namespace

