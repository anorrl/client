local ANORRLGui = game:GetService("CoreGui"):WaitForChild("ANORRLGui")
local ContextActionService = game:GetService("ContextActionService")
local scriptContext = game:GetService("ScriptContext")
local touchEnabled = game:GetService("UserInputService").TouchEnabled

if not ANORRLGui:FindFirstChild("ControlFrame") then
	local controlFrame = Instance.new("Frame")
	controlFrame.Name = "ControlFrame"
	controlFrame.Size = UDim2.new(1, 0, 1, 0)
	controlFrame.BackgroundTransparency = 1
	controlFrame.ANORRLLocked = true
	controlFrame.Parent = ANORRLGui

	local bottomLeftControl = Instance.new("Frame")
	bottomLeftControl.Name = "BottomLeftControl"
	bottomLeftControl.Size = UDim2.new(0, 130, 0, 46)
	bottomLeftControl.Position = UDim2.new(0, 0, 1, -46)
	bottomLeftControl.BackgroundTransparency = 1
	bottomLeftControl.ANORRLLocked = true
	bottomLeftControl.Parent = controlFrame

	local bottomRightControl = Instance.new("Frame")
	bottomRightControl.Name = "BottomRightControl"
	bottomRightControl.Size = UDim2.new(0, 180, 0, 41)
	bottomRightControl.Position = UDim2.new(1, -180, 1, -41)
	bottomRightControl.BackgroundTransparency = 1
	bottomRightControl.ANORRLLocked = true
	bottomRightControl.Parent = controlFrame

	local topLeftControl = Instance.new("Frame")
	topLeftControl.Name = "TopLeftControl"
	topLeftControl.Size = UDim2.new(0.05, 0, 0.05, 0)
	topLeftControl.BackgroundTransparency = 1
	topLeftControl.ANORRLLocked = true
	topLeftControl.Parent = controlFrame
end

local soundFolder = Instance.new("Folder")
soundFolder.Name = "Sounds"
soundFolder.Parent = ANORRLGui

-- TopBar
local topbarSuccess, topbarFlagValue = pcall(function() return settings():GetFFlag("UseInGameTopBar") end)
local useTopBar = (topbarSuccess and topbarFlagValue == true)
if useTopBar then
	scriptContext:AddCoreScriptLocal("CoreScripts/Topbar", ANORRLGui)
end

-- SettingsScript
local luaControlsSuccess, luaControlsFlagValue = pcall(function() return settings():GetFFlag("UseLuaCameraAndControl") end)

-- MainBotChatScript (the Lua part of Dialogs)
scriptContext:AddCoreScriptLocal("CoreScripts/MainBotChatScript", ANORRLGui)

-- Developer Console Script
scriptContext:AddCoreScriptLocal("CoreScripts/DeveloperConsole", ANORRLGui)

-- In-game notifications script
scriptContext:AddCoreScriptLocal("CoreScripts/NotificationScript2", ANORRLGui)

-- Chat script
if useTopBar then
	spawn(function() require(ANORRLGui.Modules.Chat) end)
	spawn(function() require(ANORRLGui.Modules.Emotes) end)
	spawn(function() require(ANORRLGui.Modules.PlayerlistModule) end)
end

local luaBubbleChatSuccess, luaBubbleChatFlagValue = pcall(function() return settings():GetFFlag("LuaBasedBubbleChat") end)
if luaBubbleChatSuccess and luaBubbleChatFlagValue then
	scriptContext:AddCoreScriptLocal("CoreScripts/BubbleChat", ANORRLGui)
end

-- Purchase Prompt Script
scriptContext:AddCoreScriptLocal("CoreScripts/PurchasePromptScript2", ANORRLGui)

-- Health Script
if not useTopBar then
	scriptContext:AddCoreScriptLocal("CoreScripts/HealthScript", ANORRLGui)
end

do -- Backpack!
	spawn(function() require(ANORRLGui.Modules.BackpackScript) end)
end

if useTopBar then
	scriptContext:AddCoreScriptLocal("CoreScripts/VehicleHud", ANORRLGui)
end

scriptContext:AddCoreScriptLocal("CoreScripts/GamepadMenu", ANORRLGui)

if touchEnabled then -- touch devices don't use same control frame
	-- only used for touch device button generation
	scriptContext:AddCoreScriptLocal("CoreScripts/ContextActionTouch", ANORRLGui)

	ANORRLGui:WaitForChild("ControlFrame")
	ANORRLGui.ControlFrame:WaitForChild("BottomLeftControl")
	ANORRLGui.ControlFrame.BottomLeftControl.Visible = false
end
