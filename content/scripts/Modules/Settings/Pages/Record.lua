--[[r
		Filename: Record.lua
		Written by: jeditkacheff
		Version 1.0
		Description: Takes care of the Record Tab in Settings Menu
--]]
-------------- SERVICES --------------
local CoreGui = game:GetService("CoreGui")
local ANORRLGui = CoreGui:WaitForChild("ANORRLGui")
local GuiService = game:GetService("GuiService")
local Settings = UserSettings()
local GameSettings = Settings.GameSettings

----------- UTILITIES --------------
ANORRLGui:WaitForChild("Modules"):WaitForChild("TenFootInterface")
local utility = require(ANORRLGui.Modules.Settings.Utility)
local isTenFootInterface = require(ANORRLGui.Modules.TenFootInterface):IsEnabled()

------------ Variables -------------------
local PageInstance = nil

----------- CLASS DECLARATION --------------

local function Initialize()
	local settingsPageFactory = require(ANORRLGui.Modules.Settings.SettingsPageFactory)
	local this = settingsPageFactory:CreateNewPage()
	
	------ TAB CUSTOMIZATION -------
	this.TabHeader.Name = "RecordTab"

	this.TabHeader.Icon.Image = "arlasset://textures/ui/Settings/MenuBarIcons/RecordTab.png"
	this.TabHeader.Icon.Size = UDim2.new(0,41,0,40)
	this.TabHeader.Icon.Position = UDim2.new(0,5,0.5,-20)

	this.TabHeader.Icon.Title.Text = "Record"

	this.TabHeader.Size = UDim2.new(0,130,1,0)


	------ PAGE CUSTOMIZATION -------
	this.Page.Name = "Record"

	local function makeTextLabel(name, text, bold, size, pos, parent)
		local textLabel = utility:Create'TextLabel'
		{
			Name = name,
			BackgroundTransparency = 1,
			Text = text,
			TextWrapped = true,
			Font = Enum.Font.SourceSans,
			FontSize = Enum.FontSize.Size24,
			TextColor3 = Color3.new(1,1,1),
			Size = size,
			Position = pos,
			TextXAlignment = Enum.TextXAlignment.Left,
			TextYAlignment = Enum.TextYAlignment.Top,
			ZIndex = 2,
			Parent = parent
		};
		if bold then textLabel.Font = Enum.Font.SourceSansBold end

		return textLabel
	end

	-- need to override this function from SettingsPageFactory
	-- DropDown menus require hub to to be set when they are initialized
	function this:SetHub(newHubRef)
		this.HubRef = newHubRef

		---------------------------------- SCREENSHOT -------------------------------------
		local screenshotTitle = makeTextLabel("ScreenshotTitle", 
												"Screenshot",
												true, UDim2.new(1,0,0,36), UDim2.new(0,10,0.05,0), this.Page)
		screenshotTitle.FontSize = Enum.FontSize.Size36

		local screenshotBody = makeTextLabel("ScreenshotBody", 
												"By clicking the 'Take Screenshot' button, the menu will close and take a screenshot and save it to your computer.",
												false, UDim2.new(1,-10,0,70), UDim2.new(0,0,1,0), screenshotTitle)

		local closeSettingsFunc = function()
			this.HubRef:SetVisibility(false, true)
		end
		this.ScreenshotButton = utility:MakeStyledButton("ScreenshotButton", "Take Screenshot", UDim2.new(0,300,0,44), closeSettingsFunc, this)
		
		this.ScreenshotButton.Position = UDim2.new(0,400,1,0)
		this.ScreenshotButton.Parent = screenshotBody

		this.ScreenshotButton:SetVerb("Screenshot")

		this.Page.Size = UDim2.new(1,0,0,400)
	end

	return this
end


----------- Public Facing API Additions --------------
PageInstance = Initialize()

PageInstance.Displayed.Event:connect(function(switchedFromGamepadInput)
	if switchedFromGamepadInput then
		GuiService.SelectedCoreObject = PageInstance.ScreenshotButton
	end
end)


return PageInstance