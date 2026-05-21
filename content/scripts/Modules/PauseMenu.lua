-- Main Entry Point
local CoreGui = game:GetService("CoreGui")
local ANORRLGui = CoreGui:WaitForChild("ANORRLGui")
local utility = require(ANORRLGui.Modules.NewSettings.Utility)
local ContextActionService = game:GetService("ContextActionService")
local GuiService = game:GetService("GuiService")
local UserInputService = game:GetService("UserInputService")
local RunService = game:GetService("RunService")

--[[ CORE MODULES ]]
local playerList = require(ANORRLGui.Modules.PlayerlistModule)
local chat = require(ANORRLGui.Modules.Chat)
local emotes = require(ANORRLGui.Modules.Emotes)
local backpack = require(ANORRLGui.Modules.BackpackScript)

function MakeButton(name, text, size, position, parent, func)
	local button = utility:MakeStyledButton(name, text, size, func)
	button.ZIndex = parent.ZIndex + 1
	for _, v in pairs(button:GetDescendants()) do
		if v:IsA("GuiObject") then
			v.ZIndex = v.ZIndex + parent.ZIndex
		end
		if v:IsA("TextLabel") then
			v.Font = Enum.Font.Fingerpaint
		end
	end
	button.Modal = true
	button.Position = position
	button.Parent = parent
	
	return button
end

function PlaySound(id, volume)
	if not volume then
		volume = 0.5
	end
	local sound = Instance.new("Sound", game.CoreGui)
	sound.SoundId = id
	sound.Volume = volume
	sound.Ended:connect(function()
		sound:Remove()
	end)
	sound:Play()
end

local function CreateSettingsHub()
	local this = {}
	this.Visible = false
	this.SettingsShowSignal = utility:CreateSignal()
	this.midAnimation = false
	this.backgroundSound = nil
	
	function openPaper(paperImg)
		coroutine.resume(coroutine.create(function()
			midAnimation = true
			paperImg.Image = "arlasset://textures/ui/crumpled_paper1.png"
			wait(0.1)
			paperImg.Image = "arlasset://textures/ui/crumpled_paper2.png"
			wait(0.1)
			paperImg.Image = "arlasset://textures/ui/paper.png"
			midAnimation = false
		end))
	end
	
	function closePaper(paperImg)
		coroutine.resume(coroutine.create(function()
			midAnimation = true
			paperImg.Image = "arlasset://textures/ui/paper.png"
			wait(0.1)
			paperImg.Image = "arlasset://textures/ui/crumpled_paper2.png"
			wait(0.1)
			paperImg.Image = "arlasset://textures/ui/crumpled_paper1.png"
			midAnimation = false
		end))
	end
	
	function setOverrideMouseIconBehavior()
		pcall(function()
			if UserInputService:GetLastInputType() == Enum.UserInputType.Gamepad1 then
				UserInputService.OverrideMouseIconBehavior = Enum.OverrideMouseIconBehavior.ForceHide
			else
				UserInputService.OverrideMouseIconBehavior = Enum.OverrideMouseIconBehavior.ForceShow
			end
		end)
	end
	
	function CreatePromptButtons(acceptMessage, acceptFunc, declineMessage, declineFunc)
		local y = 0
		local sizeY = 60
		
		local wrappedFunc = function() acceptFunc() end
		if type(acceptFunc) == "string" then
			if acceptFunc ~= "Exit" then
				wrappedFunc = function () this:CloseConfirmationMenu() end
			else
				wrappedFunc = nil
			end
		end
		
		local acceptBtn = MakeButton("ARLConfirmButtonAccept",  acceptMessage,  UDim2.new(0.5, 0, 0, sizeY), UDim2.new(-0.15, 0, 0.5, y), this.skateboardMenu, wrappedFunc)
		if type(acceptFunc) == "string" then
			acceptBtn:SetVerb(acceptFunc)
		end
		
		
		
		MakeButton("ARLConfirmButtonDecline", declineMessage, UDim2.new(0.5, 0, 0, sizeY), UDim2.new(0.65,  0, 0.5, y), this.skateboardMenu, declineFunc)
	end
	
	function this:CloseConfirmationMenu()
		if not self.skateboard.Visible then
			return
		end
		function onSkateboardDone()
			self.skateboard.Visible = false
		end
		self.skateboard:TweenPosition(UDim2.new(0.5,0,2,0), Enum.EasingDirection.Out, Enum.EasingStyle.Quad, 0.75, false, onSkateboardDone)
		wait(0.1)
		PlaySound("arlasset://sounds/pause/trrrks.ogg", 0.2)
		wait(0.1)
		self.skateboardMenu.Visible = false
	end
	
	function this:OpenConfirmationMenu(message, accept, acceptFunc, decline)
		if self.skateboard.Visible then
			return -- we're waiting for another prompt to finish!
		end
		self.skateboard.Position = UDim2.new(0.5,0,-1,0)
		self.skateboard.Visible = true
		self.skateboardMenu:FindFirstChild("SkateboardPromptQuestion").Text = message
		function declineFunc() self:CloseConfirmationMenu() end
		
		
		
		CreatePromptButtons(accept, acceptFunc, decline, declineFunc)
		
		self.skateboard:TweenPosition(UDim2.new(0.5,0,0.5,0), Enum.EasingDirection.Out, Enum.EasingStyle.Quad, 0.75, false)
		wait(0.1)
		self.skateboardMenu.Visible = true
		PlaySound("arlasset://sounds/pause/skrrrt.ogg", 0.2)
		wait(0.3)
		
	end
	
	function this:OpenSettingsMenu()
		if this.settingsFrame.Visible then
			return
		end
		
		self.settingsFrame.Position = UDim2.new(-1,0,0.5,0)
		self.settingsFrame.Visible = true
		
		local menuModule = require(ANORRLGui.Modules.NewSettings.SettingsHub)
		menuModule:SetVisibility(true, true, nil, true)
		
		--UDim2.new(0.5,0,0.5,0),
		
		self.settingsFrame:TweenPosition(UDim2.new(0.5,0,0.5,0), Enum.EasingDirection.Out, Enum.EasingStyle.Quad, 0.5, false)
		wait(0.1)
		PlaySound("arlasset://sounds/pause/ipad.wav")
	end
	
	function this:CloseSettingsMenu()
		if not this.settingsFrame.Visible then
			return
		end
		
		self.settingsFrame.Position = UDim2.new(0.5,0,0.5,0)
		
		local menuModule = require(ANORRLGui.Modules.NewSettings.SettingsHub)
		
		
		--UDim2.new(0.5,0,0.5,0),
		
		function closeFunc() 
			self.settingsFrame.Visible = false
			menuModule:SetVisibility(false, true, nil, true)
		end
		
		self.settingsFrame:TweenPosition(UDim2.new(-1,0,0.5,0), Enum.EasingDirection.Out, Enum.EasingStyle.Quad, 0.5, false, closeFunc)
		wait(0.1)
		PlaySound("arlasset://sounds/pause/ipad.wav")
	end
	
	function createMenu()
		local backing = utility:Create'Frame'
		{
			Name = "PauseMenu",
			BackgroundTransparency = 0.5,
			BackgroundColor3 = Color3.fromRGB(20,10,20),
			Size = UDim2.new(1,0,1,0),
			Parent = ANORRLGui,
			Active = false,
			Visible = this.Visible,
			ZIndex = 5
		}
		
		utility:Create'TextLabel'
		{
			Name = "Title",
			Size = UDim2.new(1, 0, 0, 20),
			TextXAlignment = Enum.TextXAlignment.Left,
			Position = UDim2.new(0, 5, 1, -20),
			AnchorPoint = Vector2.new(0, 1),
			TextScaled = true,
			Text = "Pause Menu by grace",
			ZIndex = backing.ZIndex + 1,
			Parent = backing,
			BackgroundTransparency = 1,
			Font = Enum.Font.Arcade,
			TextColor3 = Color3.new(1,1,1)
		}
		
		utility:Create'TextLabel'
		{
			Name = "Title",
			Size = UDim2.new(1, 0, 0, 20),
			TextXAlignment = Enum.TextXAlignment.Left,
			Position = UDim2.new(0, 5, 1, 0),
			AnchorPoint = Vector2.new(0, 1),
			TextScaled = true,
			Text = "SFX/Music by caleskowitz",
			ZIndex = backing.ZIndex + 1,
			Parent = backing,
			BackgroundTransparency = 1,
			Font = Enum.Font.Arcade,
			TextColor3 = Color3.new(1,1,1)
		}
		
		local paper = utility:Create'ImageLabel'
		{
			Name = "Paper",
			Image = "arlasset://textures/ui/crumpled_paper1.png",
			Size = UDim2.new(0, 450, 0, 450),
			AnchorPoint = Vector2.new(0.5, 0.5),
			Position = UDim2.new(0.5,0,0.5,0),
			BackgroundTransparency = 1,
			ZIndex = backing.ZIndex + 1,
			Parent = backing,
		}
		
		local timeElapsed = 0
		local clocked = false
		local lastSecond = 0
		local randomRotation = 0
		
		local actualMenu = utility:Create'Frame'
		{
			Name = "ActualMenu",
			Size = UDim2.new(0, 350, 0, 300),
			AnchorPoint = Vector2.new(0.5, 0.5),
			Position = UDim2.new(0.5,0,0.5,0),
			BackgroundTransparency = 1,
			ZIndex = paper.ZIndex + 1,
			Parent = backing,
		}
		
		local menuTitle = utility:Create'TextLabel'
		{
			Name = "Title",
			Size = UDim2.new(1, 0, 0, 35),
			Position = UDim2.new(0, 0, 0, 0),
			TextScaled = true,
			Text = "Pause Menu",
			ZIndex = actualMenu.ZIndex + 1,
			Parent = actualMenu,
			BackgroundTransparency = 1,
			Font = Enum.Font.Fingerpaint
		}
		
		local y = 45
		local sizeY = 60
		
		function closeMenu()
			this:SetVisibility(false)
		end
		
		function noAnimMenu()
			this:SetVisibility(false, true)
		end
		
		function openResetMenu()
			local resetCharFunc = function()
				local player = game.Players.LocalPlayer
				if player then
					local character = player.Character
					if character then
						local humanoid = character:FindFirstChild('Humanoid')
						if humanoid then
							humanoid.Health = 0
						end
					end
				end
				this:SetVisibility(false, true)
			end
			this:OpenConfirmationMenu("Are you sure you want to reset your character?", "Yes!", resetCharFunc,  "No...")
		end
		
		function openLeaveMenu()
			
			this:OpenConfirmationMenu("Are you sure you want to leave the game?", "Yes!", "Exit",  "No...")
		end
		
		local settingsFrame = utility:Create'ImageLabel'
		{
			Name = "testFrame",
			Size = UDim2.new(0, 700, 0, 500),
			AnchorPoint = Vector2.new(0.5, 0.5),
			Position = UDim2.new(0.5,0,0.5,0),
			BackgroundTransparency = 0,
			BackgroundColor3 = Color3.fromRGB(255,0,255),
			ZIndex = menuTitle.ZIndex + 2,
			Parent = backing,
			Image = "arlasset://textures/ui/settings_background.png",
			Visible = false
		}
		
		local testIpadFrame = utility:Create'ImageLabel'
		{
			Name = "testFrame2",
			Size = UDim2.new(0, 844, 0, 556),
			AnchorPoint = Vector2.new(0.5, 0.5),
			Position = UDim2.new(0.5,-5,0.5,10),
			BackgroundTransparency = 1,
			BackgroundColor3 = Color3.fromRGB(255,0,255),
			ZIndex = settingsFrame.ZIndex + 2,
			Selectable = false,
			Parent = settingsFrame,
			Image = "arlasset://textures/ui/ipad.png",
			Visible = true
		}
		
		local menuModule = require(ANORRLGui.Modules.NewSettings.SettingsHub)
		menuModule:CreateInstance(settingsFrame)
		
		for _, v in pairs(menuModule.Instance.Shield:GetDescendants()) do
			if v:IsA("GuiObject") and v.ZIndex <= settingsFrame.ZIndex then
				v.ZIndex = v.ZIndex + settingsFrame.ZIndex
			end
		end
		
		function openSettingsMenu()
			this:OpenSettingsMenu()
		end
		
		local resumeBtn   = MakeButton("ARLPauseMenuResumeGameBtn", "Resume Game", UDim2.new(1, 0, 0, sizeY),    UDim2.new(0,0,0,y),   actualMenu, closeMenu); y = y + sizeY + 5
		local settingsBtn = MakeButton("ARLPauseMenuSettingsBtn",   "Settings",    UDim2.new(1, 0, 0, sizeY),    UDim2.new(0,0,0,y),   actualMenu, openSettingsMenu); y = y + sizeY + 5
		local scrnShotBtn = MakeButton("ARLPauseMenuScreenshotBtn", "Screenshot",  UDim2.new(0.5, -5, 0, sizeY), UDim2.new(0,0,0,y),   actualMenu, noAnimMenu)
		local resetBtn    = MakeButton("ARLPauseMenuResetBtn",      "Reset",       UDim2.new(0.5, -5, 0, sizeY), UDim2.new(0.5,5,0,y),   actualMenu, openResetMenu)  y = y + sizeY + 5
		local leaveBtn    = MakeButton("ARLPauseMenuLeaveBtn",      "Leave",       UDim2.new(1, 0, 0, sizeY), UDim2.new(0,0,0,y), actualMenu, openLeaveMenu)
		
		scrnShotBtn:SetVerb("Screenshot")
		
		RunService.Heartbeat:Connect(function(time)
			if this.Visible then
				timeElapsed = timeElapsed + time
				
				if math.floor(timeElapsed) % 1 <= 0.1 and not clocked then
					
					clocked = true
					randomRotation = math.random(-10, 10)
					lastSecond = math.floor(timeElapsed)
				end
				
				if math.floor(timeElapsed) ~= lastSecond then
					clocked = false
				end
				
				paper.Rotation = randomRotation
				
			end
			
			actualMenu.Visible = tostring(paper.Image) == "arlasset://textures/ui/paper.png" and not this.skateboardMenu.Visible
		end)
		
		local skateboard = utility:Create'ImageLabel'
		{
			Name = "Skateboard",
			Size = UDim2.new(0, 376, 0, 1207),
			AnchorPoint = Vector2.new(0.5, 0.5),
			Position = UDim2.new(0.5,0,-1,0),
			BackgroundTransparency = 1,
			ZIndex = leaveBtn.ZIndex + 2,
			Image = "arlasset://textures/ui/skateboard.png",
			Visible = false,
			Parent = backing,
		}
		
		local skateboardMenu = utility:Create'Frame'
		{
			Name = "SkateboardMenu",
			Size = UDim2.new(0, 350, 0, 300),
			AnchorPoint = Vector2.new(0.5, 0.5),
			Position = UDim2.new(0.5,0,0.5,0),
			BackgroundTransparency = 1,
			ZIndex = skateboard.ZIndex + 1,
			Parent = skateboard,
			Visible = false,
		}
		
		local skateboardPromptQuestion = utility:Create'TextLabel'
		{
			Name = "SkateboardPromptQuestion",
			Size = UDim2.new(1.5, 0, 0, 50),
			Position = UDim2.new(0.5, 0, 0.5, -70),
			AnchorPoint = Vector2.new(0.5, 0.5),
			TextScaled = true,
			Text = "",
			TextColor3 = Color3.fromRGB(255,255,255),
			ZIndex = skateboardMenu.ZIndex + 1,
			Parent = skateboardMenu,
			BackgroundColor3 = Color3.fromRGB(40, 30, 40),
			BackgroundTransparency = 0,
			Font = Enum.Font.Fingerpaint
		}
		
		local backgroundSound = Instance.new("Sound", game.CoreGui)
		backgroundSound.SoundId = "arlasset://sounds/pause/background.ogg"
		backgroundSound.Looped = true
		backgroundSound.Volume = 0.15
		this.backgroundSound = backgroundSound
		
		return backing, paper, actualMenu, skateboard, skateboardMenu, settingsFrame
	end
	
	this.backing, this.paper, this.menu, this.skateboard, this.skateboardMenu, this.settingsFrame = createMenu()
	
	function this:SetVisibility(visible, skipAnim)
		if visible == nil then
			visible = false
		end
		
		if self.skateboard.Visible then
			self:CloseConfirmationMenu()
		end
		
		if self.settingsFrame.Visible then
			self:CloseSettingsMenu()
		end
	
		if self.Visible == visible then
			return
		end
		
		while midAnimation do
			if skipAnim then break end
			wait()
		end
		
		if self.Visible == visible then -- you never know...
			return
		end
		
		self.Visible = visible
		
		
		if self.Visible then
			PlaySound("arlasset://sounds/pause/paper_uncrumple.mp3")
			self.backing.Visible = self.Visible
			openPaper(this.paper)
			self.SettingsShowSignal:fire(self.Visible)
			
			setOverrideMouseIconBehavior()
			pcall(function() lastInputChangedCon = UserInputService.LastInputTypeChanged:connect(setOverrideMouseIconBehavior) end)
			if UserInputService.MouseEnabled then
				pcall(function()
					UserInputService.OverrideMouseIconBehavior = Enum.OverrideMouseIconBehavior.ForceShow
				end)
			end
			
			local noOpFunc = function() end
			ContextActionService:BindCoreAction("RbxSettingsHubStopCharacter", noOpFunc, true,
												 Enum.PlayerActions.CharacterForward,
												 Enum.PlayerActions.CharacterBackward,
												 Enum.PlayerActions.CharacterLeft,
												 Enum.PlayerActions.CharacterRight,
												 Enum.PlayerActions.CharacterJump,
												 Enum.KeyCode.LeftShift,
												 Enum.KeyCode.RightShift,
												 Enum.KeyCode.Tab,
												 Enum.UserInputType.Gamepad1, Enum.UserInputType.Gamepad2, Enum.UserInputType.Gamepad3, Enum.UserInputType.Gamepad4)
												 
			pcall(function() GuiService:SetMenuIsOpen(true) end)
			playerList:HideTemp('SettingsMenu', true)
			
			if not this.backgroundSound.IsPlaying and not this.backgroundSound.IsPaused then
				self.backgroundSound:Play()
			else
				self.backgroundSound:Resume()
			end
		else
			self.backgroundSound:Pause()
			if not skipAnim then
				closePaper(this.paper)
				PlaySound("arlasset://sounds/pause/paper_crumpling.mp3")
				while tostring(this.paper.Image) ~= "arlasset://textures/ui/crumpled_paper1.png" do
					if skipAnim then break end
					wait()
				end
				wait(0.1)
			end
			ContextActionService:UnbindCoreAction("RbxSettingsHubStopCharacter")
			
			self.SettingsShowSignal:fire(self.Visible)
			self.backing.Visible = self.Visible
			pcall(function() GuiService:SetMenuIsOpen(false) end)
			playerList:HideTemp('SettingsMenu', false)
			
		end
	end
	
	function this:ToggleVisibility()
		this:SetVisibility(not self.Visible)
	end
	
	function closeOpenFunc()
		this:ToggleVisibility()
	end
	
	local closeMenuFunc = function(name, inputState, input)
		if inputState and inputState ~= Enum.UserInputState.Begin then return end
		
		this:ToggleVisibility()
	end
	
	ContextActionService:BindCoreAction("RBXEscapeMainMenu", closeMenuFunc, false, Enum.KeyCode.Escape)
	
	return this
end

local moduleApiTable = {}

	local settingsInstance = CreateSettingsHub()

	function moduleApiTable:SetVisibility(visible, noAnimation, customStartPage, switchedFromGamepadInput)
		settingsInstance:SetVisibility(visible)
	end
	function moduleApiTable:ToggleVisibility(switchedFromGamepadInput)
		settingsInstance:ToggleVisibility()
	end
	
	function moduleApiTable:GetVisibility()
		return settingsInstance.Visible
	end
	
	function moduleApiTable:CloseSettingsMenu()
		settingsInstance:CloseSettingsMenu()
	end

	moduleApiTable.SettingsShowSignal = settingsInstance.SettingsShowSignal

	moduleApiTable.Instance = settingsInstance

return moduleApiTable