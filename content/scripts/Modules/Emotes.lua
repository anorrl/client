--[[
	// FileName: Emote.lua
	// Written by: grace & phil
	// Description: Code for lua emotes for ANORRL.
]]

local RunService = game:GetService('RunService')
local CoreGuiService = game:GetService('CoreGui')
local PlayersService = game:GetService('Players')
local GuiService = game:GetService('GuiService')
local InputService = game:GetService('UserInputService')
local StarterGui = game:GetService('StarterGui')
local HttpService = game:GetService('HttpService')
local HttpRbxApiService = game:GetService('HttpRbxApiService')
local ContentProvider = game:GetService('ContentProvider')

local Settings = UserSettings()
local GameSettings = Settings.GameSettings

--[[ SCRIPT VARIABLES ]]

while PlayersService.LocalPlayer == nil do PlayersService.ChildAdded:wait() end
local Player = PlayersService.LocalPlayer

local mouse = Player:GetMouse()

local GuiRoot = CoreGuiService:WaitForChild('RobloxGui')

local topbarEnabled = true

local Util = {}
do
	function Util.Signal()
		local sig = {}

		local mSignaler = Instance.new('BindableEvent')

		local mArgData = nil
		local mArgDataCount = nil

		function sig:fire(...)
			mArgData = {...}
			mArgDataCount = select('#', ...)
			mSignaler:Fire()
		end

		function sig:connect(f)
			if not f then error("connect(nil)", 2) end
			return mSignaler.Event:connect(function()
				f(unpack(mArgData, 1, mArgDataCount))
			end)
		end

		function sig:wait()
			mSignaler.Event:wait()
			assert(mArgData, "Missing arg data, likely due to :TweenSize/Position corrupting threadrefs.")
			return unpack(mArgData, 1, mArgDataCount)
		end

		return sig
	end
end

local function CreateEmotes()
	local this = {}
	
	this.WidgetVisible = false
	this.WholeFrame = nil
	this.EmotesFrame = nil
	this.DetailsFrame = nil
	this.SelectorFrame = nil
	this.EmoteNameLabel = nil
	this.Emotes = {}
	this.LoadedEmotes = {}
	this.CurrentSlot = nil
	this.MaxEmotes = 8
	this.VisibilityStateChanged = nil

	function snapDegrees(in_deg, snap)
		local closest = math.floor((in_deg / snap)+0.5) * snap
		local result = closest
		if math.abs(closest - in_deg) then
			result = closest
		end
		
		return result
	end

	
	function this:positionInsideOuterCircle(centerRelativePos)
		local outerCircleRadius = self.WholeFrame.AbsoluteSize.X / 2
		local innerCircleRadius = outerCircleRadius * 0.45
		
		local magnitude = math.sqrt(centerRelativePos.X*centerRelativePos.X + centerRelativePos.Y*centerRelativePos.Y)
		return magnitude > innerCircleRadius and magnitude < outerCircleRadius
	end

	function this:selectSlot()
		local delta = Vector2.new(mouse.X, mouse.Y) - Vector2.new(
			self.WholeFrame.AbsolutePosition.X + self.WholeFrame.AbsoluteSize.X / 2,
			self.WholeFrame.AbsolutePosition.Y + self.WholeFrame.AbsoluteSize.Y / 2
		)
		
		local angle = math.atan2(delta.Y, delta.X)	
		
		local rotation = snapDegrees(math.deg(angle), 45)
		self.SelectorFrame.Rotation = rotation

		local hasSelection = self:positionInsideOuterCircle(delta)
		self.SelectorFrame.Visible = hasSelection
		
		local index = math.floor((rotation+90) / 45)
		
		if index <= 0 then
			index = 8 + index
		end

		return hasSelection and index or 0
	end

	function createWholeFrame()
		local whole_frame = Instance.new("Frame", GuiRoot)
		
		whole_frame.BackgroundTransparency = 1
		whole_frame.Position = UDim2.new(0.5, -225, 0.5, -225)
		whole_frame.Size = UDim2.new(0, 450, 0, 450)
		whole_frame.Name = "WholeFrame"
		whole_frame.Visible = this.WidgetVisible
		
		local background = Instance.new("ImageLabel", whole_frame)
		background.BackgroundTransparency = 1
		background.Position = UDim2.new(0,0,0,0)
		background.Size = UDim2.new(1,0,1,0)
		background.Image = "rbxasset://textures/ui/Emotes/Wheel/CircleBackground@2x.png"
		background.Name = "Background"
		
		local segments = Instance.new("ImageLabel", whole_frame)
		segments.BackgroundTransparency = 1
		segments.Position = UDim2.new(0,0,0,0)
		segments.Size = UDim2.new(1,0,1,0)
		segments.Image = "rbxasset://textures/ui/Emotes/Wheel/SegmentedCircle@2x.png"
		segments.Name = "Segments"
		segments.ZIndex = 5
		
		return whole_frame
	end
	
	function this:createEmoteTemplate(x, y, name)
		local frame = Instance.new("ImageLabel")
		frame.Name = name
		frame.Position = UDim2.new(x, 0, y, 0)
		frame.Size = UDim2.new(0, 100, 0, 100)
		frame.ZIndex = 10
		frame.Image = "rbxasset://textures/ui/Emotes/EmoteIcon.png"
		frame.BackgroundColor3 = Color3.new(255,255,255)
		frame.BackgroundTransparency = 1
		
		return frame
	end

	function createEmotesFrame(whole_frame)
		local frame = Instance.new("Frame", whole_frame)
		
		frame.BackgroundTransparency = 1
		frame.Position = UDim2.new(0,0,0,0)
		frame.Size = UDim2.new(1,0,1,0)
		frame.Name = "Emotes"
		frame.ZIndex = 6
		
		if #this.Emotes == 0 then 
			return frame
		end
		
		local angle_step = (2 * math.pi) / this.MaxEmotes
		local radius = 0.4
		local offset = 0.5 - 50/whole_frame.AbsoluteSize.X
		
		
		for i, v in ipairs(this.Emotes) do
			if i > this.MaxEmotes then
				break
			end
			
			local max_emotes = #this.Emotes
			if #this.Emotes > this.MaxEmotes then
				max_emotes = this.MaxEmotes
			end
		
			local max_angle = (max_emotes - 1) * angle_step - (math.pi / 2)
			
			local angle = (i - 1) * angle_step - (math.pi / 2)
			
			angle = (max_angle) + math.rad(90) + angle	
			
			local x = offset - radius * math.cos(angle)
			local y = offset - radius * math.sin(angle)
			
			table.insert(this.LoadedEmotes, v)
			
			local emoteFrame = this:createEmoteTemplate(x, y, v.name)
			emoteFrame.Parent = frame
			v.EmoteFrame = emoteFrame
		end
		
		return frame
	end

	function createDetailsFrame(whole_frame)
		local frame = Instance.new("Frame", whole_frame)
		
		frame.BackgroundTransparency = 1
		frame.Position = UDim2.new(0.5, -75, 0.5, -75)
		frame.Size = UDim2.new(0, 150, 0, 150)
		frame.Name = "Details"
		frame.ZIndex = 6
		
		local emote_label = Instance.new("TextLabel", frame)
		emote_label.BackgroundTransparency = 1
		emote_label.TextScaled = true
		emote_label.Position = UDim2.new(0, 0, 0.5, -25)
		emote_label.Size = UDim2.new(1,0,0,50)
		emote_label.ZIndex = 6
		emote_label.Name = "EName"
		emote_label.TextColor3 = Color3.new(255,255,255)
		
		return frame, emote_label
	end

	function createSelector(whole_frame)
		local frame = Instance.new("Frame", whole_frame)
		
		frame.BackgroundTransparency = 1
		frame.Position = UDim2.new(0,0,0,0)
		frame.Size = UDim2.new(1,0,1,0)
		frame.Name = "Selector"
		frame.ZIndex = 6
		
		local gradient = Instance.new("ImageLabel", frame)
		gradient.BackgroundTransparency = 1
		gradient.Image = "rbxasset://textures/ui/Emotes/Wheel/SelectedGradient@2x.png"
		gradient.ZIndex = 2
		gradient.Position = UDim2.new(0.5,0,0.5,-53)
		gradient.Size = UDim2.new(0,138,0,106)
		gradient.Name = "Gradient"
		
		return frame
	end
	
	function this:CharacterAdded(character)
		self.EmoteHandler = Player:FindFirstChild("HandleEmote")
		for i, v in ipairs(self.Emotes) do
			self.EmoteHandler:Fire("register", v.id)
		end
	end
	
	function this:Initialize()
		self.WholeFrame = createWholeFrame()
		local whole_frame = self.WholeFrame
		self.Emotes = self:GrabEmotes()
		self.EmotesFrame = createEmotesFrame(whole_frame)
		self.DetailsFrame, self.EmoteNameLabel = createDetailsFrame(whole_frame)
		self.SelectorFrame = createSelector(whole_frame)
		self.VisibilityStateChanged = Util.Signal()

		InputService.InputChanged:connect(function(inputObj)
			local inputType = inputObj.UserInputType

            if inputType == Enum.UserInputType.MouseMovement or inputType == Enum.UserInputType.Touch then
				local oldEmote = self.LoadedEmotes[self.slot]
                self.slot = self:selectSlot()
				local newEmote = self.LoadedEmotes[self.slot]
				if oldEmote and oldEmote ~= newEmote then
					oldEmote.EmoteFrame.Image = "rbxasset://textures/ui/Emotes/EmoteIcon.png"
				end
				if self.slot == 0 or (not newEmote) then
					self.EmoteNameLabel.Text = #self.Emotes > 0 and "" or "You have no emotes!"
				else
					newEmote.EmoteFrame.Image = "rbxasset://textures/ui/Emotes/EmoteIconHover.png"
					this.EmoteNameLabel.Text = newEmote.name
				end
            end
		end)

		InputService.InputBegan:connect(function(inputObj)
			if not self.WidgetVisible then return end 
		
            local inputType = inputObj.UserInputType

            if inputType == Enum.UserInputType.MouseButton1 or inputType == Enum.UserInputType.Touch then
				local emote = self.LoadedEmotes[self.slot]
				if emote then
					this:PlayEmote(emote.id)
				end
			end
		end)
		
		Player.CharacterAdded:Connect(function(character)
			this:CharacterAdded(character)
		end)
		
		if Player.Character then
			this:CharacterAdded(Player.Character)
		end
	end
	
	function this:PlayEmote(id)
		if self.EmoteHandler and self.EmoteHandler.Parent == Player then
			self.EmoteHandler:Fire("play", id)
			this:ToggleVisibility()
		end
	end
	
	function this:ToggleVisibility(visible)
		if visible ~= self.WidgetVisible then
			if visible == nil then
				self.WidgetVisible = false
			else
				self.WidgetVisible = visible
			end
			
			self.WholeFrame.Visible = self.WidgetVisible
		end
		
		this.VisibilityStateChanged:fire(self.WidgetVisible)
		
		if GameSettings:IsAeroEnabled() then
			local sound = Instance.new("Sound", game.CoreGui)
			sound.SoundId = "rbxasset://sounds/aero/menu_command.mp3"
			sound.Volume = 5
			sound.Ended:connect(function()
				sound:Remove()
			end)
			sound:Play()
		end
	end
	
	function this:ParseEmotes(emotes)
		local new = {}
		for k, v in pairs(emotes) do
			local emote = {}
			
			for j, i in pairs(v) do
				emote[j] = i
			end
			
			table.insert(new, emote)
		end
		
		return new
	end
	
	function this:GrabEmotes()
		local success, result = pcall(function()
			return HttpRbxApiService:GetAsync("users/emotes", true)
		end)
		
		if not success then
			print("GrabEmotes() failed because", result)
			return {}
		end

		-- can now parse web response
		result = HttpService:JSONDecode(result)
		if result["success"] then
			return self:ParseEmotes(result["emotes"])
		else
			print("GrabEmotes() failed because", result["reason"])
			return {}
		end
	end
	
	return this
end


local moduleApiTable = {}
-- Main Entry Point
do
	local EmotesInstance = CreateEmotes()
	EmotesInstance:Initialize()
	
	function moduleApiTable:ToggleVisibility(visible)
		EmotesInstance:ToggleVisibility(visible)
	end
	
	function moduleApiTable:GetVisibility()
		return EmotesInstance.WidgetVisible
	end
	
	function moduleApiTable:GetInstance()
		return EmotesInstance
	end
	
	moduleApiTable.VisibilityStateChanged = EmotesInstance.VisibilityStateChanged
end

return moduleApiTable