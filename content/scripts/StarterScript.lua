local GameSettings = UserSettings().GameSettings
local currentVirtualVersion = nil
local RobloxGui = game:GetService("CoreGui"):WaitForChild("RobloxGui")
local ContextActionService = game:GetService("ContextActionService")

local function clearCorescripts()
    for _, obj in pairs(RobloxGui:GetChildren()) do
        if obj.Name ~= "Modules" and obj.Name ~= "CoreScripts/2016/DeveloperConsole" and obj.Name ~= "ControlFrame" then
			obj:Destroy()
        end
    end
end

function load2016StarterScript()
	local scriptContext = game:GetService("ScriptContext")
	local touchEnabled = game:GetService("UserInputService").TouchEnabled

	local RobloxGui = game:GetService("CoreGui"):WaitForChild("RobloxGui")
	
	if not RobloxGui:FindFirstChild("ControlFrame") then
		clearCorescripts()
		local controlFrame = Instance.new("Frame")
		controlFrame.Name = "ControlFrame"
		controlFrame.Size = UDim2.new(1, 0, 1, 0)
		controlFrame.BackgroundTransparency = 1
		controlFrame.RobloxLocked = true
		controlFrame.Parent = RobloxGui

		local bottomLeftControl = Instance.new("Frame")
		bottomLeftControl.Name = "BottomLeftControl"
		bottomLeftControl.Size = UDim2.new(0, 130, 0, 46)
		bottomLeftControl.Position = UDim2.new(0, 0, 1, -46)
		bottomLeftControl.BackgroundTransparency = 1
		bottomLeftControl.RobloxLocked = true
		bottomLeftControl.Parent = controlFrame

		local bottomRightControl = Instance.new("Frame")
		bottomRightControl.Name = "BottomRightControl"
		bottomRightControl.Size = UDim2.new(0, 180, 0, 41)
		bottomRightControl.Position = UDim2.new(1, -180, 1, -41)
		bottomRightControl.BackgroundTransparency = 1
		bottomRightControl.RobloxLocked = true
		bottomRightControl.Parent = controlFrame

		local topLeftControl = Instance.new("Frame")
		topLeftControl.Name = "TopLeftControl"
		topLeftControl.Size = UDim2.new(0.05, 0, 0.05, 0)
		topLeftControl.BackgroundTransparency = 1
		topLeftControl.RobloxLocked = true
		topLeftControl.Parent = controlFrame
	end

	local soundFolder = Instance.new("Folder")
	soundFolder.Name = "Sounds"
	soundFolder.Parent = RobloxGui

	-- TopBar
	local topbarSuccess, topbarFlagValue = pcall(function() return settings():GetFFlag("UseInGameTopBar") end)
	local useTopBar = (topbarSuccess and topbarFlagValue == true)
	if useTopBar then
		scriptContext:AddCoreScriptLocal("CoreScripts/2016/Topbar", RobloxGui)
	end

	-- SettingsScript
	local luaControlsSuccess, luaControlsFlagValue = pcall(function() return settings():GetFFlag("UseLuaCameraAndControl") end)

	-- MainBotChatScript (the Lua part of Dialogs)
	scriptContext:AddCoreScriptLocal("CoreScripts/2016/MainBotChatScript2", RobloxGui)

	-- Developer Console Script
	scriptContext:AddCoreScriptLocal("CoreScripts/2016/DeveloperConsole", RobloxGui)

	-- In-game notifications script
	scriptContext:AddCoreScriptLocal("CoreScripts/2016/NotificationScript2", RobloxGui)

	-- Chat script
	if useTopBar then
		spawn(function() require(RobloxGui.Modules.Chat) end)
		spawn(function() require(RobloxGui.Modules.Emotes) end)
		spawn(function() require(RobloxGui.Modules.PlayerlistModule) end)
	end

	local luaBubbleChatSuccess, luaBubbleChatFlagValue = pcall(function() return settings():GetFFlag("LuaBasedBubbleChat") end)
	if luaBubbleChatSuccess and luaBubbleChatFlagValue then
		scriptContext:AddCoreScriptLocal("CoreScripts/2016/BubbleChat", RobloxGui)
	end

	-- Purchase Prompt Script
	scriptContext:AddCoreScriptLocal("CoreScripts/2016/PurchasePromptScript2", RobloxGui)

	-- Health Script
	if not useTopBar then
		scriptContext:AddCoreScriptLocal("CoreScripts/2016/HealthScript", RobloxGui)
	end

	do -- Backpack!
		spawn(function() require(RobloxGui.Modules.BackpackScript) end)
	end

	if useTopBar then
		scriptContext:AddCoreScriptLocal("CoreScripts/2016/VehicleHud", RobloxGui)
	end

	scriptContext:AddCoreScriptLocal("CoreScripts/2016/GamepadMenu", RobloxGui)

	if touchEnabled then -- touch devices don't use same control frame
		-- only used for touch device button generation
		scriptContext:AddCoreScriptLocal("CoreScripts/2016/ContextActionTouch", RobloxGui)

		RobloxGui:WaitForChild("ControlFrame")
		RobloxGui.ControlFrame:WaitForChild("BottomLeftControl")
		RobloxGui.ControlFrame.BottomLeftControl.Visible = false
	end
end

function load2014StarterScript()

	-- Creates all neccessary scripts for the gui on initial load, everything except build tools
	-- Created by Ben T. 10/29/10
	-- Please note that these are loaded in a specific order to diminish errors/perceived load time by user
	local scriptContext = game:GetService("ScriptContext")
	local touchEnabled = game:GetService("UserInputService").TouchEnabled

	Game:GetService("CoreGui"):WaitForChild("RobloxGui")
	local screenGui = Game:GetService("CoreGui"):FindFirstChild("RobloxGui")
	
	local controlFrame = Instance.new("Frame")
	controlFrame.Name = "ControlFrame"
	controlFrame.Size = UDim2.new(1, 0, 1, 0)
	controlFrame.BackgroundTransparency = 1
	controlFrame.Parent = RobloxGui

	local topLeftControl = Instance.new("Frame")
	topLeftControl.Name = "TopLeftControl"
	topLeftControl.Size = UDim2.new(0, 100, 0, 50)
	topLeftControl.Position = UDim2.new(0, 0, 0, 0)
	topLeftControl.BackgroundColor3 = Color3.new(1, 0, 0)
	topLeftControl.BackgroundTransparency = 1 
	topLeftControl.Parent = controlFrame
	
	local bottomLeftControl = Instance.new("Frame")
	bottomLeftControl.Name = "BottomLeftControl"
	bottomLeftControl.Size = UDim2.new(0, 100, 0, 50) 
	bottomLeftControl.Position = UDim2.new(0, 0, 1, -50) 
	bottomLeftControl.BackgroundColor3 = Color3.new(0, 1, 0)
	bottomLeftControl.BackgroundTransparency = 1 
	bottomLeftControl.Parent = controlFrame

	local bottomRightControl = Instance.new("Frame")
	bottomRightControl.Name = "BottomRightControl"
	bottomRightControl.Size = UDim2.new(0, 100, 0, 50) 
	bottomRightControl.Position = UDim2.new(0, 0, 1, -50) 
	bottomRightControl.BackgroundColor3 = Color3.new(0, 1, 0)
	bottomRightControl.BackgroundTransparency = 1 
	bottomRightControl.Parent = controlFrame

	-- SettingsScript 
	scriptContext:AddCoreScriptLocal("CoreScripts/2014/Settings", screenGui)

	if not touchEnabled then
		-- ToolTipper  (creates tool tips for gui)
		scriptContext:AddCoreScriptLocal("CoreScripts/2014/ToolTip", screenGui)
	else
		scriptContext:AddCoreScriptLocal("CoreScripts/2014/TouchControls", screenGui)
	end

	-- MainBotChatScript
	scriptContext:AddCoreScriptLocal("CoreScripts/2014/MainBotChatScript", screenGui)

	-- Developer Console Script
	scriptContext:AddCoreScriptLocal("CoreScripts/2014/DeveloperConsole", screenGui)

	-- Popup Script
	scriptContext:AddCoreScriptLocal("CoreScripts/2014/PopupScript", screenGui)
	-- Friend Notification Script (probably can use this script to expand out to other notifications)
	scriptContext:AddCoreScriptLocal("CoreScripts/2014/NotificationScript", screenGui)
	-- Chat script
	scriptContext:AddCoreScriptLocal("CoreScripts/2014/ChatScript", screenGui)	
	-- Purchase Prompt Script
	scriptContext:AddCoreScriptLocal("CoreScripts/2014/PurchasePromptScript", screenGui)
	-- Health Script
	scriptContext:AddCoreScriptLocal("CoreScripts/2014/HealthScript", screenGui)

	local luaBubbleChatSuccess, luaBubbleChatFlagValue = pcall(function() return settings():GetFFlag("LuaBasedBubbleChat") end)
	if luaBubbleChatSuccess and luaBubbleChatFlagValue then
		scriptContext:AddCoreScriptLocal("CoreScripts/2014/BubbleChat", RobloxGui)
	end

	if not touchEnabled then 
		-- New Player List
		scriptContext:AddCoreScriptLocal("CoreScripts/2014/PlayerListScript", screenGui)
	elseif Game:GetService("GuiService"):GetScreenResolution().Y >= 500 then 	
		-- New Player List
		scriptContext:AddCoreScriptLocal("CoreScripts/2014/PlayerListScript", screenGui)
	end

	-- Backpack Builder, creates most of the backpack gui
	scriptContext:AddCoreScriptLocal("CoreScripts/2014/BackpackScripts/BackpackBuilder", screenGui)

	screenGui:WaitForChild("CurrentLoadout")
	screenGui:WaitForChild("Backpack")
	local Backpack = screenGui.Backpack
		
	-- Manager handles all big backpack state changes, other scripts subscribe to this and do things accordingly
	scriptContext:AddCoreScriptLocal("CoreScripts/2014/BackpackScripts/BackpackManager", Backpack)
		
	-- Backpack Gear (handles all backpack gear tab stuff)
	scriptContext:AddCoreScriptLocal("CoreScripts/2014/BackpackScripts/BackpackGear", Backpack)
	-- Loadout Script, used for gear hotkeys
	scriptContext:AddCoreScriptLocal("CoreScripts/2014/BackpackScripts/LoadoutScript", screenGui.CurrentLoadout)

	if touchEnabled then -- touch devices don't use same control frame
		-- only used for touch device button generation
		scriptContext:AddCoreScriptLocal("CoreScripts/2014/ContextActionTouch", screenGui)

		screenGui:WaitForChild("ControlFrame")
		screenGui.ControlFrame:WaitForChild("BottomLeftControl")
		screenGui.ControlFrame.BottomLeftControl.Visible = true
	end

	for _, v in pairs(screenGui:WaitForChild("ControlFrame"):children()) do
		print(v.Name)
	end

end


function load(newVersion)
	
	if currentVirtualVersion == newVersion then return end
	
	if newVersion ~= currentVirtualVersion then
		clearCorescripts()
	end
	
	if currentVirtualVersion == Enum.VirtualVersion["2016"] and GameSettings.VirtualVersion ~= Enum.VirtualVersion["2016"] then
		if RobloxGui:FindFirstChild("Modules") then
			-- fffffffuck youuu
            for _, module in pairs(RobloxGui.Modules:GetDescendants()) do
				module:Destroy()
            end
        end
	end
	
	if newVersion ==  Enum.VirtualVersion["2016"] then
		load2016StarterScript()
	elseif newVersion ==  Enum.VirtualVersion["2014"] then
		load2014StarterScript()
	end
	
	ContextActionService:UnbindCoreAction("RbxSettingsHubSwitchTab")
	ContextActionService:UnbindCoreAction("RbxSettingsHubStopCharacter")
	ContextActionService:UnbindCoreAction("RbxSettingsScrollHotkey")
	ContextActionService:UnbindCoreAction("RBXEscapeMainMenu")

	currentVirtualVersion = newVersion
end


load(UserSettings().GameSettings.VirtualVersion)

UserSettings().GameSettings.Changed:Connect(function(prop)

	if prop ~= "VirtualVersion" then return end
	
	load(UserSettings().GameSettings.VirtualVersion)
	
end)