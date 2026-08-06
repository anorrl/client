#include "stdafx.h"

#include "Util/LegacyContentTable.h"
#include <boost/algorithm/string/predicate.hpp>

FASTFLAGVARIABLE(DebugRenderDownloadAssets, false)

namespace
{
	inline bool isSlash(char ch)
	{
		return ch == '\\' || ch == '/';
	}
	
	void normalizeUrl(std::string& url)
	{
		char* data = &url[0];
		size_t size = url.size();
		size_t write = 0;
		
		for (size_t i = 0; i < size; ++i)
		{
			// that's faster than tolower
			if (static_cast<unsigned int>(data[i] - 'A') < 26)
				data[write++] = (data[i] - 'A') + 'a';
			else if (isSlash(data[i]))
			{
				data[write++] = '/';
				
				// skip subsequent slashes
				while (i + 1 < size && isSlash(data[i + 1])) i++;
			}
			else
				data[write++] = data[i];
		}
		
		url.resize(write);
	}
}

namespace ARL
{
	LegacyContentTable::LegacyContentTable()
	{
		AddEntry("arlasset://textures/AnchorCursor.png", "arlasset://retros/textures/AnchorCursor.png");
		AddEntry("arlasset://textures/AnchorTool.png", "arlasset://retros/textures/AnchorTool.png");
		AddEntry("arlasset://textures/AnchorTool_dn.png", "arlasset://retros/textures/AnchorTool_dn.png");
		AddEntry("arlasset://textures/ArrowTool.png", "arlasset://retros/textures/ArrowTool.png");
		AddEntry("arlasset://textures/ArrowTool_dn.png", "arlasset://retros/textures/ArrowTool_dn.png");
		AddEntry("arlasset://textures/ArrowTool_ds.png", "arlasset://retros/textures/ArrowTool_ds.png");
		AddEntry("arlasset://textures/ArrowTool_ovr.png", "arlasset://retros/textures/ArrowTool_ovr.png");
		AddEntry("arlasset://textures/A_Key.png", "arlasset://retros/textures/A_Key.png");
		AddEntry("arlasset://textures/A_Key_dn.png", "arlasset://retros/textures/A_Key_dn.png");
		AddEntry("arlasset://textures/BaseballCapRed.png", "arlasset://retros/textures/BaseballCapRed.png");
		AddEntry("arlasset://textures/Blank.png", "arlasset://retros/textures/Blank.png");
		AddEntry("arlasset://textures/Bomb.png", "arlasset://retros/textures/Bomb.png");
		AddEntry("arlasset://textures/bombtex.png", "arlasset://retros/textures/bombtex.png");
		AddEntry("arlasset://textures/CameraCenter.png", "arlasset://retros/textures/CameraCenter.png");
		AddEntry("arlasset://textures/CameraCenter_dn.png", "arlasset://retros/textures/CameraCenter_dn.png");
		AddEntry("arlasset://textures/CameraCenter_ds.png", "arlasset://retros/textures/CameraCenter_ds.png");
		AddEntry("arlasset://textures/CameraCenter_ovr.png", "arlasset://retros/textures/CameraCenter_ovr.png");
		AddEntry("arlasset://textures/CameraPanLeft.png", "arlasset://retros/textures/CameraPanLeft.png");
		AddEntry("arlasset://textures/CameraPanLeft_dn.png", "arlasset://retros/textures/CameraPanLeft_dn.png");
		AddEntry("arlasset://textures/CameraPanLeft_ovr.png", "arlasset://retros/textures/CameraPanLeft_ovr.png");
		AddEntry("arlasset://textures/CameraPanRight.png", "arlasset://retros/textures/CameraPanRight.png");
		AddEntry("arlasset://textures/CameraPanRight_dn.png", "arlasset://retros/textures/CameraPanRight_dn.png");
		AddEntry("arlasset://textures/CameraPanRight_ovr.png", "arlasset://retros/textures/CameraPanRight_ovr.png");
		AddEntry("arlasset://textures/CameraTiltDown.png", "arlasset://retros/textures/CameraTiltDown.png");
		AddEntry("arlasset://textures/CameraTiltDown_dn.png", "arlasset://retros/textures/CameraTiltDown_dn.png");
		AddEntry("arlasset://textures/CameraTiltDown_ovr.png", "arlasset://retros/textures/CameraTiltDown_ovr.png");
		AddEntry("arlasset://textures/CameraTiltUp.png", "arlasset://retros/textures/CameraTiltUp.png");
		AddEntry("arlasset://textures/CameraTiltUp_dn.png", "arlasset://retros/textures/CameraTiltUp_dn.png");
		AddEntry("arlasset://textures/CameraTiltUp_ovr.png", "arlasset://retros/textures/CameraTiltUp_ovr.png");
		AddEntry("arlasset://textures/CameraZoomIn.png", "arlasset://retros/textures/CameraZoomIn.png");
		AddEntry("arlasset://textures/CameraZoomIn_dn.png", "arlasset://retros/textures/CameraZoomIn_dn.png");
		AddEntry("arlasset://textures/CameraZoomIn_ovr.png", "arlasset://retros/textures/CameraZoomIn_ovr.png");
		AddEntry("arlasset://textures/CameraZoomOut.png", "arlasset://retros/textures/CameraZoomOut.png");
		AddEntry("arlasset://textures/CameraZoomOut_dn.png", "arlasset://retros/textures/CameraZoomOut_dn.png");
		AddEntry("arlasset://textures/CameraZoomOut_ovr.png", "arlasset://retros/textures/CameraZoomOut_ovr.png");
		AddEntry("arlasset://textures/Card.png", "arlasset://retros/textures/Card.png");
		AddEntry("arlasset://textures/Card_dn.png", "arlasset://retros/textures/Card_dn.png");
		AddEntry("arlasset://textures/Card_ds.png", "arlasset://retros/textures/Card_ds.png");
		AddEntry("arlasset://textures/Card_ovr.png", "arlasset://retros/textures/Card_ovr.png");
		AddEntry("arlasset://textures/Character.png", "arlasset://retros/textures/Character.png");
		AddEntry("arlasset://textures/Chat.png", "arlasset://retros/textures/Chat.png");
		AddEntry("arlasset://textures/Chat_dn.png", "arlasset://retros/textures/Chat_dn.png");
		AddEntry("arlasset://textures/Chat_ds.png", "arlasset://retros/textures/Chat_ds.png");
		AddEntry("arlasset://textures/Chat_ovr.png", "arlasset://retros/textures/Chat_ovr.png");
		AddEntry("arlasset://textures/Clone.png", "arlasset://retros/textures/Clone.png");
		AddEntry("arlasset://textures/CloneCursor.png", "arlasset://retros/textures/CloneCursor.png");
		AddEntry("arlasset://textures/CloneDownCursor.png", "arlasset://retros/textures/CloneDownCursor.png");
		AddEntry("arlasset://textures/CloneOverCursor.png", "arlasset://retros/textures/CloneOverCursor.png");
		AddEntry("arlasset://textures/CloneWandTexture.png", "arlasset://retros/textures/CloneWandTexture.png");
		AddEntry("arlasset://textures/Controller1.png", "arlasset://retros/textures/Controller1.png");
		AddEntry("arlasset://textures/Controller1Tool.png", "arlasset://retros/textures/Controller1Tool.png");
		AddEntry("arlasset://textures/Controller1Tool_dn.png", "arlasset://retros/textures/Controller1Tool_dn.png");
		AddEntry("arlasset://textures/Controller1_dn.png", "arlasset://retros/textures/Controller1_dn.png");
		AddEntry("arlasset://textures/Controller1_ds.png", "arlasset://retros/textures/Controller1_ds.png");
		AddEntry("arlasset://textures/Controller1_ovr.png", "arlasset://retros/textures/Controller1_ovr.png");
		AddEntry("arlasset://textures/Controller2.png", "arlasset://retros/textures/Controller2.png");
		AddEntry("arlasset://textures/Controller2Tool.png", "arlasset://retros/textures/Controller2Tool.png");
		AddEntry("arlasset://textures/Controller2Tool_dn.png", "arlasset://retros/textures/Controller2Tool_dn.png");
		AddEntry("arlasset://textures/Controller2_dn.png", "arlasset://retros/textures/Controller2_dn.png");
		AddEntry("arlasset://textures/Controller2_ds.png", "arlasset://retros/textures/Controller2_ds.png");
		AddEntry("arlasset://textures/Controller2_ovr.png", "arlasset://retros/textures/Controller2_ovr.png");
		AddEntry("arlasset://textures/ControllerAI1Tool.png", "arlasset://retros/textures/ControllerAI1Tool.png");
		AddEntry("arlasset://textures/ControllerAI1Tool_dn.png", "arlasset://retros/textures/ControllerAI1Tool_dn.png");
		AddEntry("arlasset://textures/ControllerAI2Tool.png", "arlasset://retros/textures/ControllerAI2Tool.png");
		AddEntry("arlasset://textures/ControllerAI2Tool_dn.png", "arlasset://retros/textures/ControllerAI2Tool_dn.png");
		AddEntry("arlasset://textures/ControllerNoneTool.png", "arlasset://retros/textures/ControllerNoneTool.png");
		AddEntry("arlasset://textures/ControllerNoneTool_dn.png", "arlasset://retros/textures/ControllerNoneTool_dn.png");
		AddEntry("arlasset://textures/ControllerPanel.png", "arlasset://retros/textures/ControllerPanel.png");
		AddEntry("arlasset://textures/Delete.png", "arlasset://retros/textures/Delete.png");
		AddEntry("arlasset://textures/Delete_dn.png", "arlasset://retros/textures/Delete_dn.png");
		AddEntry("arlasset://textures/Delete_ds.png", "arlasset://retros/textures/Delete_ds.png");
		AddEntry("arlasset://textures/Delete_ovr.png", "arlasset://retros/textures/Delete_ovr.png");
		AddEntry("arlasset://textures/Detonator.png", "arlasset://retros/textures/Detonator.png");
		AddEntry("arlasset://textures/Down_key.png", "arlasset://retros/textures/Down_key.png");
		AddEntry("arlasset://textures/Down_key_dn.png", "arlasset://retros/textures/Down_key_dn.png");
		AddEntry("arlasset://textures/DragCursor.png", "arlasset://retros/textures/DragCursor.png");
		AddEntry("arlasset://textures/DropperCursor.png", "arlasset://retros/textures/DropperCursor.png");
		AddEntry("arlasset://textures/DropperTool.png", "arlasset://retros/textures/DropperTool.png");
		AddEntry("arlasset://textures/DropperTool_dn.png", "arlasset://retros/textures/DropperTool_dn.png");
		AddEntry("arlasset://textures/D_Key.png", "arlasset://retros/textures/D_Key.png");
		AddEntry("arlasset://textures/D_Key_dn.png", "arlasset://retros/textures/D_Key_dn.png");
		AddEntry("arlasset://textures/FillCursor.png", "arlasset://retros/textures/FillCursor.png");
		AddEntry("arlasset://textures/FillTool.png", "arlasset://retros/textures/FillTool.png");
		AddEntry("arlasset://textures/FillTool_dn.png", "arlasset://retros/textures/FillTool_dn.png");
		AddEntry("arlasset://textures/FireWand.png", "arlasset://retros/textures/FireWand.png");
		AddEntry("arlasset://textures/FirstPersonIndicator.png", "arlasset://retros/textures/FirstPersonIndicator.png");
		AddEntry("arlasset://textures/FirstPersonIndicator_ds.png", "arlasset://retros/textures/FirstPersonIndicator_ds.png");
		AddEntry("arlasset://textures/FlagCursor.png", "arlasset://retros/textures/FlagCursor.png");
		AddEntry("arlasset://textures/Flamethrower.png", "arlasset://retros/textures/Flamethrower.png");
		AddEntry("arlasset://textures/FlatCursor.png", "arlasset://retros/textures/FlatCursor.png");
		AddEntry("arlasset://textures/FlatTool.png", "arlasset://retros/textures/FlatTool.png");
		AddEntry("arlasset://textures/FlatTool_dn.png", "arlasset://retros/textures/FlatTool_dn.png");
		AddEntry("arlasset://textures/GameTool.png", "arlasset://retros/textures/GameTool.png");
		AddEntry("arlasset://textures/Glue.png", "arlasset://retros/textures/Glue.png");
		AddEntry("arlasset://textures/GlueCursor.png", "arlasset://retros/textures/GlueCursor.png");
		AddEntry("arlasset://textures/Grab.png", "arlasset://retros/textures/Grab.png");
		AddEntry("arlasset://textures/GrabCursor.png", "arlasset://retros/textures/GrabCursor.png");
		AddEntry("arlasset://textures/GrabRotateCursor.png", "arlasset://retros/textures/GrabRotateCursor.png");
		AddEntry("arlasset://textures/Gun.png", "arlasset://retros/textures/Gun.png");
		AddEntry("arlasset://textures/GunCursor.png", "arlasset://retros/textures/GunCursor.png");
		AddEntry("arlasset://textures/GunWaitCursor.png", "arlasset://retros/textures/GunWaitCursor.png");
		AddEntry("arlasset://textures/HalloweenAsylum.png", "arlasset://retros/textures/HalloweenAsylum.png");
		AddEntry("arlasset://textures/HalloweenGhost.png", "arlasset://retros/textures/HalloweenGhost.png");
		AddEntry("arlasset://textures/HalloweenPumpkin.png", "arlasset://retros/textures/HalloweenPumpkin.png");
		AddEntry("arlasset://textures/HalloweenRocket.png", "arlasset://retros/textures/HalloweenRocket.png");
		AddEntry("arlasset://textures/HalloweenSkull.png", "arlasset://retros/textures/HalloweenSkull.png");
		AddEntry("arlasset://textures/Hammer.png", "arlasset://retros/textures/Hammer.png");
		AddEntry("arlasset://textures/HammerCursor.png", "arlasset://retros/textures/HammerCursor.png");
		AddEntry("arlasset://textures/HammerDownCursor.png", "arlasset://retros/textures/HammerDownCursor.png");
		AddEntry("arlasset://textures/HammerOverCursor.png", "arlasset://retros/textures/HammerOverCursor.png");
		AddEntry("arlasset://textures/hammertex128.png", "arlasset://retros/textures/hammertex128.png");
		AddEntry("arlasset://textures/HingeCursor.png", "arlasset://retros/textures/HingeCursor.png");
		AddEntry("arlasset://textures/HingeTool.png", "arlasset://retros/textures/HingeTool.png");
		AddEntry("arlasset://textures/HingeTool_dn.png", "arlasset://retros/textures/HingeTool_dn.png");
		AddEntry("arlasset://textures/HopperPanel.png", "arlasset://retros/textures/HopperPanel.png");
		AddEntry("arlasset://textures/H_Key.png", "arlasset://retros/textures/H_Key.png");
		AddEntry("arlasset://textures/H_Key_dn.png", "arlasset://retros/textures/H_Key_dn.png");
		AddEntry("arlasset://textures/JohnHead.png", "arlasset://retros/textures/JohnHead.png");
		AddEntry("arlasset://textures/JohnTex.png", "arlasset://retros/textures/JohnTex.png");
		AddEntry("arlasset://textures/J_Key.png", "arlasset://retros/textures/J_Key.png");
		AddEntry("arlasset://textures/J_Key_dn.png", "arlasset://retros/textures/J_Key_dn.png");
		AddEntry("arlasset://textures/K_Key.png", "arlasset://retros/textures/K_Key.png");
		AddEntry("arlasset://textures/K_Key_dn.png", "arlasset://retros/textures/K_Key_dn.png");
		AddEntry("arlasset://textures/Laser.png", "arlasset://retros/textures/Laser.png");
		AddEntry("arlasset://textures/LeftMotorFastTool.png", "arlasset://retros/textures/LeftMotorFastTool.png");
		AddEntry("arlasset://textures/LeftMotorFastTool_dn.png", "arlasset://retros/textures/LeftMotorFastTool_dn.png");
		AddEntry("arlasset://textures/LeftMotorTool.png", "arlasset://retros/textures/LeftMotorTool.png");
		AddEntry("arlasset://textures/LeftMotorTool_dn.png", "arlasset://retros/textures/LeftMotorTool_dn.png");
		AddEntry("arlasset://textures/Left_key.png", "arlasset://retros/textures/Left_key.png");
		AddEntry("arlasset://textures/Left_key_dn.png", "arlasset://retros/textures/Left_key_dn.png");
		AddEntry("arlasset://textures/LockCursor.png", "arlasset://retros/textures/LockCursor.png");
		AddEntry("arlasset://textures/LockTool.png", "arlasset://retros/textures/LockTool.png");
		AddEntry("arlasset://textures/LockTool_dn.png", "arlasset://retros/textures/LockTool_dn.png");
		AddEntry("arlasset://textures/MedKit.png", "arlasset://retros/textures/MedKit.png");
		AddEntry("arlasset://textures/MissingCursor.png", "arlasset://retros/textures/MissingCursor.png");
		AddEntry("arlasset://textures/MotorCursor.png", "arlasset://retros/textures/MotorCursor.png");
		AddEntry("arlasset://textures/MoveTool.png", "arlasset://retros/textures/MoveTool.png");
		AddEntry("arlasset://textures/MoveTool_dn.png", "arlasset://retros/textures/MoveTool_dn.png");
		AddEntry("arlasset://textures/MoveTool_ds.png", "arlasset://retros/textures/MoveTool_ds.png");
		AddEntry("arlasset://textures/MoveTool_ovr.png", "arlasset://retros/textures/MoveTool_ovr.png");
		AddEntry("arlasset://textures/Multirocket.png", "arlasset://retros/textures/Multirocket.png");
		AddEntry("arlasset://textures/MultiSelection.png", "arlasset://retros/textures/MultiSelection.png");
		AddEntry("arlasset://textures/PaintballGunTex128.png", "arlasset://retros/textures/PaintballGunTex128.png");
		AddEntry("arlasset://textures/PaintballIcon.png", "arlasset://retros/textures/PaintballIcon.png");
		AddEntry("arlasset://textures/pirate.png", "arlasset://retros/textures/pirate.png");
		AddEntry("arlasset://textures/PlayDelete.png", "arlasset://retros/textures/PlayDelete.png");
		AddEntry("arlasset://textures/PlayDelete_dn.png", "arlasset://retros/textures/PlayDelete_dn.png");
		AddEntry("arlasset://textures/PlayDelete_ds.png", "arlasset://retros/textures/PlayDelete_ds.png");
		AddEntry("arlasset://textures/PlayDelete_ovr.png", "arlasset://retros/textures/PlayDelete_ovr.png");
		AddEntry("arlasset://textures/PoliceCap.png", "arlasset://retros/textures/PoliceCap.png");
		AddEntry("arlasset://textures/ReportAbuse.PNG", "arlasset://retros/textures/ReportAbuse.PNG");
		AddEntry("arlasset://textures/ReportAbuse_dn.PNG", "arlasset://retros/textures/ReportAbuse_dn.PNG");
		AddEntry("arlasset://textures/ReportAbuse_ds.PNG", "arlasset://retros/textures/ReportAbuse_ds.PNG");
		AddEntry("arlasset://textures/ReportAbuse_ovr.PNG", "arlasset://retros/textures/ReportAbuse_ovr.PNG");
		AddEntry("arlasset://textures/Reset.png", "arlasset://retros/textures/Reset.png");
		AddEntry("arlasset://textures/ResetOrg.png", "arlasset://retros/textures/ResetOrg.png");
		AddEntry("arlasset://textures/ResetOrg_dn.png", "arlasset://retros/textures/ResetOrg_dn.png");
		AddEntry("arlasset://textures/ResetOrg_ds.png", "arlasset://retros/textures/ResetOrg_ds.png");
		AddEntry("arlasset://textures/ResetOrg_ovr.png", "arlasset://retros/textures/ResetOrg_ovr.png");
		AddEntry("arlasset://textures/Reset_dn.png", "arlasset://retros/textures/Reset_dn.png");
		AddEntry("arlasset://textures/Reset_ds.png", "arlasset://retros/textures/Reset_ds.png");
		AddEntry("arlasset://textures/Reset_ovr.png", "arlasset://retros/textures/Reset_ovr.png");
		AddEntry("arlasset://textures/ResizeCursor.png", "arlasset://retros/textures/ResizeCursor.png");
		AddEntry("arlasset://textures/Right_key.png", "arlasset://retros/textures/Right_key.png");
		AddEntry("arlasset://textures/Right_key_dn.png", "arlasset://retros/textures/Right_key_dn.png");
		AddEntry("arlasset://textures/Rocket.png", "arlasset://retros/textures/Rocket.png");
		AddEntry("arlasset://textures/RocketBoots.png", "arlasset://retros/textures/RocketBoots.png");
		AddEntry("arlasset://textures/rocketlaunchertex.png", "arlasset://retros/textures/rocketlaunchertex.png");
		AddEntry("arlasset://textures/Run.png", "arlasset://retros/textures/Run.png");
		AddEntry("arlasset://textures/Run_dn.png", "arlasset://retros/textures/Run_dn.png");
		AddEntry("arlasset://textures/Run_ds.png", "arlasset://retros/textures/Run_ds.png");
		AddEntry("arlasset://textures/Run_ovr.png", "arlasset://retros/textures/Run_ovr.png");
		AddEntry("arlasset://textures/script.png", "arlasset://retros/textures/script.png");
		AddEntry("arlasset://textures/SelectionDown.png", "arlasset://retros/textures/SelectionDown.png");
		AddEntry("arlasset://textures/SelectionDown_ds.png", "arlasset://retros/textures/SelectionDown_ds.png");
		AddEntry("arlasset://textures/SelectionDown_ovr.png", "arlasset://retros/textures/SelectionDown_ovr.png");
		AddEntry("arlasset://textures/SelectionRotate.png", "arlasset://retros/textures/SelectionRotate.png");
		AddEntry("arlasset://textures/SelectionRotate_ds.png", "arlasset://retros/textures/SelectionRotate_ds.png");
		AddEntry("arlasset://textures/SelectionRotate_ovr.png", "arlasset://retros/textures/SelectionRotate_ovr.png");
		AddEntry("arlasset://textures/SelectionTilt.png", "arlasset://retros/textures/SelectionTilt.png");
		AddEntry("arlasset://textures/SelectionTilt_ds.png", "arlasset://retros/textures/SelectionTilt_ds.png");
		AddEntry("arlasset://textures/SelectionTilt_ovr.png", "arlasset://retros/textures/SelectionTilt_ovr.png");
		AddEntry("arlasset://textures/SelectionUp.png", "arlasset://retros/textures/SelectionUp.png");
		AddEntry("arlasset://textures/SelectionUp_ds.png", "arlasset://retros/textures/SelectionUp_ds.png");
		AddEntry("arlasset://textures/SelectionUp_ovr.png", "arlasset://retros/textures/SelectionUp_ovr.png");
		AddEntry("arlasset://textures/Slingshot.png", "arlasset://retros/textures/Slingshot.png");
		AddEntry("arlasset://textures/SlingshotTexture.png", "arlasset://retros/textures/SlingshotTexture.png");
		AddEntry("arlasset://textures/Snowball.png", "arlasset://retros/textures/Snowball.png");
		AddEntry("arlasset://textures/Snowflake.png", "arlasset://retros/textures/Snowflake.png");
		AddEntry("arlasset://textures/sombrero.png", "arlasset://retros/textures/sombrero.png");
		AddEntry("arlasset://textures/sparkle.png", "arlasset://retros/textures/sparkle.png");
		AddEntry("arlasset://textures/SpawnCursor.png", "arlasset://retros/textures/SpawnCursor.png");
		AddEntry("arlasset://textures/SpawnLocation.png", "arlasset://retros/textures/SpawnLocation.png");
		AddEntry("arlasset://textures/Stop.png", "arlasset://retros/textures/Stop.png");
		AddEntry("arlasset://textures/StopReset.png", "arlasset://retros/textures/StopReset.png");
		AddEntry("arlasset://textures/StopReset_dn.png", "arlasset://retros/textures/StopReset_dn.png");
		AddEntry("arlasset://textures/StopReset_ds.png", "arlasset://retros/textures/StopReset_ds.png");
		AddEntry("arlasset://textures/StopReset_ovr.png", "arlasset://retros/textures/StopReset_ovr.png");
		AddEntry("arlasset://textures/Stop_dn.png", "arlasset://retros/textures/Stop_dn.png");
		AddEntry("arlasset://textures/Stop_ds.png", "arlasset://retros/textures/Stop_ds.png");
		AddEntry("arlasset://textures/Stop_ovr.png", "arlasset://retros/textures/Stop_ovr.png");
		AddEntry("arlasset://textures/Superball.png", "arlasset://retros/textures/Superball.png");
		AddEntry("arlasset://textures/SurfacePanel.png", "arlasset://retros/textures/SurfacePanel.png");
		AddEntry("arlasset://textures/Surfaces.png", "arlasset://retros/textures/Surfaces.png");
		AddEntry("arlasset://textures/SurfacesStrip.png", "arlasset://retros/textures/SurfacesStrip.png");
		AddEntry("arlasset://textures/Sword128.png", "arlasset://retros/textures/Sword128.png");
		AddEntry("arlasset://textures/SwordTexture.png", "arlasset://retros/textures/SwordTexture.png");
		AddEntry("arlasset://textures/S_Key.png", "arlasset://retros/textures/S_Key.png");
		AddEntry("arlasset://textures/S_Key_dn.png", "arlasset://retros/textures/S_Key_dn.png");
		AddEntry("arlasset://textures/test2_texture.JPG", "arlasset://retros/textures/test2_texture.JPG");
		AddEntry("arlasset://textures/test_texture.JPG", "arlasset://retros/textures/test_texture.JPG");
		AddEntry("arlasset://textures/TopHatPurple.png", "arlasset://retros/textures/TopHatPurple.png");
		AddEntry("arlasset://textures/treetex.png", "arlasset://retros/textures/treetex.png");
		AddEntry("arlasset://textures/TrowelTexture.png", "arlasset://retros/textures/TrowelTexture.png");
		AddEntry("arlasset://textures/UnAnchorCursor.png", "arlasset://retros/textures/UnAnchorCursor.png");
		AddEntry("arlasset://textures/UnlockCursor.png", "arlasset://retros/textures/UnlockCursor.png");
		AddEntry("arlasset://textures/Up_key.png", "arlasset://retros/textures/Up_key.png");
		AddEntry("arlasset://textures/Up_key_dn.png", "arlasset://retros/textures/Up_key_dn.png");
		AddEntry("arlasset://textures/U_Key.png", "arlasset://retros/textures/U_Key.png");
		AddEntry("arlasset://textures/U_Key_dn.png", "arlasset://retros/textures/U_Key_dn.png");
		AddEntry("arlasset://textures/VelocityTool.png", "arlasset://retros/textures/VelocityTool.png");
		AddEntry("arlasset://textures/VelocityTool_dn.png", "arlasset://retros/textures/VelocityTool_dn.png");
		AddEntry("arlasset://textures/VelocityTool_ds.png", "arlasset://retros/textures/VelocityTool_ds.png");
		AddEntry("arlasset://textures/VelocityTool_ovr.png", "arlasset://retros/textures/VelocityTool_ovr.png");
		AddEntry("arlasset://textures/viking.png", "arlasset://retros/textures/viking.png");
		AddEntry("arlasset://textures/Wall.png", "arlasset://retros/textures/Wall.png");
		AddEntry("arlasset://textures/WeldCursor.png", "arlasset://retros/textures/WeldCursor.png");
		AddEntry("arlasset://textures/wrench.png", "arlasset://retros/textures/wrench.png");
		AddEntry("arlasset://textures/W_Key.png", "arlasset://retros/textures/W_Key.png");
		AddEntry("arlasset://textures/W_Key_dn.png", "arlasset://retros/textures/W_Key_dn.png");

		AddEntry("arlasset://textures/advancedMove.png", "arlasset://retros/textures/advancedMove.png");
		AddEntry("arlasset://textures/advancedMoveResize.png", "arlasset://retros/textures/advancedMoveResize.png");
		AddEntry("arlasset://textures/advancedMove_joint.png", "arlasset://retros/textures/advancedMove_joint.png");
		AddEntry("arlasset://textures/advancedMove_keysOnly.png", "arlasset://retros/textures/advancedMove_keysOnly.png");
		AddEntry("arlasset://textures/advancedMove_noJoint.png", "arlasset://retros/textures/advancedMove_noJoint.png");

		AddEntry("arlasset://textures/chatBubble_botBlue_bkg.png", "arlasset://retros/textures/chatBubble_botBlue_bkg.png");
		AddEntry("arlasset://textures/chatBubble_botBlue_notify_bkg.png", "arlasset://retros/textures/chatBubble_botBlue_notify_bkg.png");
		AddEntry("arlasset://textures/chatBubble_botBlue_tail.png", "arlasset://retros/textures/chatBubble_botBlue_tail.png");
		AddEntry("arlasset://textures/chatBubble_botBlue_tailRight.png", "arlasset://retros/textures/chatBubble_botBlue_tailRight.png");
		AddEntry("arlasset://textures/chatBubble_botGreen_bkg.png", "arlasset://retros/textures/chatBubble_botGreen_bkg.png");
		AddEntry("arlasset://textures/chatBubble_botGreen_notify_bkg.png", "arlasset://retros/textures/chatBubble_botGreen_notify_bkg.png");
		AddEntry("arlasset://textures/chatBubble_botGreen_tail.png", "arlasset://retros/textures/chatBubble_botGreen_tail.png");
		AddEntry("arlasset://textures/chatBubble_botGreen_tailRight.png", "arlasset://retros/textures/chatBubble_botGreen_tailRight.png");
		AddEntry("arlasset://textures/chatBubble_botRed_bkg.png", "arlasset://retros/textures/chatBubble_botRed_bkg.png");
		AddEntry("arlasset://textures/chatBubble_botRed_notify_bkg.png", "arlasset://retros/textures/chatBubble_botRed_notify_bkg.png");
		AddEntry("arlasset://textures/chatBubble_botRed_tail.png", "arlasset://retros/textures/chatBubble_botRed_tail.png");
		AddEntry("arlasset://textures/chatBubble_botRed_tailRight.png", "arlasset://retros/textures/chatBubble_botRed_tailRight.png");
		AddEntry("arlasset://textures/chatBubble_bot_notifyGray_dotDotDot.png", "arlasset://retros/textures/chatBubble_bot_notifyGray_dotDotDot.png");
		AddEntry("arlasset://textures/chatBubble_bot_notify_bang.png", "arlasset://retros/textures/chatBubble_bot_notify_bang.png");
		AddEntry("arlasset://textures/chatBubble_bot_notify_dotDotDot.png", "arlasset://retros/textures/chatBubble_bot_notify_dotDotDot.png");
		AddEntry("arlasset://textures/chatBubble_bot_notify_money.png", "arlasset://retros/textures/chatBubble_bot_notify_money.png");
		AddEntry("arlasset://textures/chatBubble_bot_notify_question.png", "arlasset://retros/textures/chatBubble_bot_notify_question.png");
		AddEntry("arlasset://textures/chatBubble_white_bkg.png", "arlasset://retros/textures/chatBubble_white_bkg.png");
		AddEntry("arlasset://textures/chatBubble_white_notify_bkg.png", "arlasset://retros/textures/chatBubble_white_notify_bkg.png");
		AddEntry("arlasset://textures/chatBubble_white_tail.png", "arlasset://retros/textures/chatBubble_white_tail.png");
		AddEntry("arlasset://textures/DialogHelp.png", "arlasset://retros/textures/DialogHelp.png");
		AddEntry("arlasset://textures/DialogQuest.png", "arlasset://retros/textures/DialogQuest.png");
		AddEntry("arlasset://textures/DialogShop.png", "arlasset://retros/textures/DialogShop.png");
		
		AddEntry("arlasset://sounds/bass.wav", "arlasset://retros/sounds/bass.wav");
		AddEntry("arlasset://sounds/button.wav", "arlasset://retros/sounds/button.wav");
		AddEntry("arlasset://sounds/clickfast.wav", "arlasset://retros/sounds/clickfast.wav");
		AddEntry("arlasset://sounds/collide.wav", "arlasset://retros/sounds/collide.wav");
		AddEntry("arlasset://sounds/electronicpingshort.wav", "arlasset://retros/sounds/electronicpingshort.wav");
		AddEntry("arlasset://sounds/flashbulb.wav", "arlasset://retros/sounds/flashbulb.wav");
		AddEntry("arlasset://sounds/glassbreak.wav", "arlasset://retros/sounds/glassbreak.wav");
		AddEntry("arlasset://sounds/HalloweenGhost.wav", "arlasset://retros/sounds/HalloweenGhost.wav");
		AddEntry("arlasset://sounds/HalloweenLightning.wav", "arlasset://retros/sounds/HalloweenLightning.wav");
		AddEntry("arlasset://sounds/HalloweenThunder.wav", "arlasset://retros/sounds/HalloweenThunder.wav");
		AddEntry("arlasset://sounds/hit.wav", "arlasset://retros/sounds/hit.wav");
		AddEntry("arlasset://sounds/Kerplunk.wav", "arlasset://retros/sounds/Kerplunk.wav");
		AddEntry("arlasset://sounds/Kid saying Ouch.wav", "arlasset://retros/sounds/Kid saying Ouch.wav");
		AddEntry("arlasset://sounds/Launching rocket.wav", "arlasset://retros/sounds/Launching rocket.wav");
		AddEntry("arlasset://sounds/pageturn.wav", "arlasset://retros/sounds/pageturn.wav");
		AddEntry("arlasset://sounds/paintball.wav", "arlasset://retros/sounds/paintball.wav");
		AddEntry("arlasset://sounds/Rocket shot.wav", "arlasset://retros/sounds/Rocket shot.wav");
		AddEntry("arlasset://sounds/Rocket whoosh 01.wav", "arlasset://retros/sounds/Rocket whoosh 01.wav");
		AddEntry("arlasset://sounds/Rubber band sling shot.wav", "arlasset://retros/sounds/Rubber band sling shot.wav");
		AddEntry("arlasset://sounds/Rubber band.wav", "arlasset://retros/sounds/Rubber band.wav");
		AddEntry("arlasset://sounds/Short spring sound.wav", "arlasset://retros/sounds/Short spring sound.wav");
		AddEntry("arlasset://sounds/Shoulder fired rocket.wav", "arlasset://retros/sounds/Shoulder fired rocket.wav");
		AddEntry("arlasset://sounds/snap.wav", "arlasset://retros/sounds/snap.wav");
		AddEntry("arlasset://sounds/splat.wav", "arlasset://retros/sounds/splat.wav");
		AddEntry("arlasset://sounds/switch.wav", "arlasset://retros/sounds/switch.wav");
		AddEntry("arlasset://sounds/SWITCH3.wav", "arlasset://retros/sounds/SWITCH3.wav");
		AddEntry("arlasset://sounds/swoosh.wav", "arlasset://retros/sounds/swoosh.wav");
		AddEntry("arlasset://sounds/swordlunge.wav", "arlasset://retros/sounds/swordlunge.wav");
		AddEntry("arlasset://sounds/swordslash.wav", "arlasset://retros/sounds/swordslash.wav");
		AddEntry("arlasset://sounds/unsheath.wav", "arlasset://retros/sounds/unsheath.wav");
		AddEntry("arlasset://sounds/uuhhh.wav", "arlasset://retros/sounds/uuhhh.wav");
		AddEntry("arlasset://sounds/victory.wav", "arlasset://retros/sounds/victory.wav");

		AddEntry("arlasset://icons/surface_ds.png",		"23576067");
		AddEntry("arlasset://icons/surface.png",		"23576066");
		AddEntry("arlasset://icons/rotate.png",			"23576065");
		AddEntry("arlasset://icons/color_sel.png",		"23576064");
		AddEntry("arlasset://icons/color.png",			"23576062");
		AddEntry("arlasset://icons/insert.png",			"23576057");
		AddEntry("arlasset://icons/freemove_ovr.png",	"23576055");	
		AddEntry("arlasset://icons/surface_sel.png",	"23575961");	
		AddEntry("arlasset://icons/surface_ovr.png",	"23575960");	
		AddEntry("arlasset://icons/rotate_ovr.png",		"23575949");
		AddEntry("arlasset://icons/rotate_ds.png",		"23575946");
		AddEntry("arlasset://icons/resize_sel.png",		"23575942");
		AddEntry("arlasset://icons/resize_ovr.png",		"23575941");
		AddEntry("arlasset://icons/resize_ds.png",		"23575940");
		AddEntry("arlasset://icons/resize.png",			"23575937");
		AddEntry("arlasset://icons/color_ovr.png",		"23575933");
		AddEntry("arlasset://icons/color_ds.png",		"23575928");
		AddEntry("arlasset://icons/material_sel.png",	"23575919");	
		AddEntry("arlasset://icons/material_ovr.png",	"23575918");	
		AddEntry("arlasset://icons/material_ds.png",	"23575917");	
		AddEntry("arlasset://icons/material.png",		"23575916");
		AddEntry("arlasset://icons/lock_sel.png",		"23575914");
		AddEntry("arlasset://icons/lock_ovr.png",		"23575912");
		AddEntry("arlasset://icons/lock_ds.png",		"23575909");
		AddEntry("arlasset://icons/lock.png",			"23575908");
		AddEntry("arlasset://icons/insert_sel.png",		"23575907");
		AddEntry("arlasset://icons/insert_ovr.png",		"23575906");
		AddEntry("arlasset://icons/insert_ds.png",		"23575905");
		AddEntry("arlasset://icons/freemove_sel.png",	"23575901");	
		AddEntry("arlasset://icons/freemove_ds.png",	"23575889");	
		AddEntry("arlasset://icons/freemove.png",		"23575887");
		AddEntry("arlasset://icons/delete_sel.png",		"23575885");
		AddEntry("arlasset://icons/delete_ovr.png",		"23575884");
		AddEntry("arlasset://icons/delete_ds.png",		"23575883");
		AddEntry("arlasset://icons/delete.png",			"23575880");
		AddEntry("arlasset://icons/configure_sel.png",	"23575879");	
		AddEntry("arlasset://icons/configure_ovr.png",	"23575878");	
		AddEntry("arlasset://icons/configure_ds.png",	"23575877");	
		AddEntry("arlasset://icons/configure.png",		"23575875");
		AddEntry("arlasset://icons/axismove_sel.png",	"23575874");	
		AddEntry("arlasset://icons/axismove_ovr.png",	"23575873");	
		AddEntry("arlasset://icons/axismove_ds.png",	"23575872");	
		AddEntry("arlasset://icons/axismove.png",		"23575871");
		AddEntry("arlasset://icons/anchor_sel.png",		"23575870");
		AddEntry("arlasset://icons/anchor_ovr.png",		"23575869");
		AddEntry("arlasset://icons/anchor_ds.png",		"23575867");
		AddEntry("arlasset://icons/anchor.png",			"23575866");
		AddEntry("arlasset://icons/surface_dn.png",		"23629198"); 
		AddEntry("arlasset://icons/rotate_dn.png",		"23629197"); 
		AddEntry("arlasset://icons/resize_dn.png",		"23629194"); 
		AddEntry("arlasset://icons/color_dn.png",		"23629193"); 
		AddEntry("arlasset://icons/material_dn.png",	"23629192"); 
		AddEntry("arlasset://icons/lock_dn.png",		"23629189"); 
		AddEntry("arlasset://icons/insert_dn.png",		"23629186"); 
		AddEntry("arlasset://icons/freemove_dn.png",	"23629182"); 
		AddEntry("arlasset://icons/delete_dn.png",		"23629179"); 
		AddEntry("arlasset://icons/configure_dn.png",	"23629177"); 
		AddEntry("arlasset://icons/axismove_dn.png",	"23629176"); 
		AddEntry("arlasset://icons/anchor_dn.png",		"23629212"); 
		
		AddEntry("arlasset://fonts/BaseballCap.mesh", "arlasset://retros/hats/BaseballCap.mesh");
		AddEntry("arlasset://fonts/fusedgirl.mesh", "arlasset://retros/hats/fusedgirl.mesh");
		AddEntry("arlasset://fonts/girlhair.mesh", "arlasset://retros/hats/girlhair.mesh");
		AddEntry("arlasset://fonts/NinjaMask.mesh", "arlasset://retros/hats/NinjaMask.mesh");
		AddEntry("arlasset://fonts/pawn.mesh", "arlasset://retros/hats/pawn.mesh");
		AddEntry("arlasset://fonts/PirateHat.mesh", "arlasset://retros/hats/PirateHat.mesh");
		AddEntry("arlasset://fonts/PoliceCap.mesh", "arlasset://retros/hats/PoliceCap.mesh");
		AddEntry("arlasset://fonts/sombrero.mesh", "arlasset://retros/hats/sombrero.mesh");
		AddEntry("arlasset://fonts/tophat.mesh", "arlasset://retros/hats/tophat.mesh");
		AddEntry("arlasset://fonts/VikingHelmet.mesh", "arlasset://retros/hats/VikingHelmet.mesh");
		
		AddEntry("arlasset://sounds/grass.ogg", "arlasset://sounds/grass.mp3");
		AddEntry("arlasset://sounds/grass2.ogg", "arlasset://sounds/grass2.mp3");
		AddEntry("arlasset://sounds/grass3.ogg", "arlasset://sounds/grass3.mp3");
		AddEntry("arlasset://sounds/grassstone.ogg", "arlasset://sounds/grassstone.mp3");
		AddEntry("arlasset://sounds/grassstone2.ogg", "arlasset://sounds/grassstone2.mp3");
		AddEntry("arlasset://sounds/grassstone3.ogg", "arlasset://sounds/grassstone3.mp3");
		AddEntry("arlasset://sounds/ice.ogg", "arlasset://sounds/ice.mp3");
		AddEntry("arlasset://sounds/ice2.ogg", "arlasset://sounds/ice2.mp3");
		AddEntry("arlasset://sounds/ice3.ogg", "arlasset://sounds/ice3.mp3");
		AddEntry("arlasset://sounds/icegrass.ogg", "arlasset://sounds/icegrass.mp3");
		AddEntry("arlasset://sounds/icegrass2.ogg", "arlasset://sounds/icegrass2.mp3");
		AddEntry("arlasset://sounds/icegrass3.ogg", "arlasset://sounds/icegrass3.mp3");
		AddEntry("arlasset://sounds/icemetal.ogg", "arlasset://sounds/icemetal.mp3");
		AddEntry("arlasset://sounds/icemetal2.ogg", "arlasset://sounds/icemetal2.mp3");
		AddEntry("arlasset://sounds/icemetal3.ogg", "arlasset://sounds/icemetal3.mp3");
		AddEntry("arlasset://sounds/icestone.ogg", "arlasset://sounds/icestone.mp3");
		AddEntry("arlasset://sounds/icestone2.ogg", "arlasset://sounds/icestone2.mp3");
		AddEntry("arlasset://sounds/icestone3.ogg", "arlasset://sounds/icestone3.mp3");
		AddEntry("arlasset://sounds/metal.ogg", "arlasset://sounds/metal.mp3");
		AddEntry("arlasset://sounds/metal2.ogg", "arlasset://sounds/metal2.mp3");
		AddEntry("arlasset://sounds/metal3.ogg", "arlasset://sounds/metal3.mp3");
		AddEntry("arlasset://sounds/metalgrass.ogg", "arlasset://sounds/metalgrass.mp3");
		AddEntry("arlasset://sounds/metalgrass2.ogg", "arlasset://sounds/metalgrass2.mp3");
		AddEntry("arlasset://sounds/metalgrass3.ogg", "arlasset://sounds/metalgrass3.mp3");
		AddEntry("arlasset://sounds/metalstone.ogg", "arlasset://sounds/metalstone.mp3");
		AddEntry("arlasset://sounds/metalstone2.ogg", "arlasset://sounds/metalstone2.mp3");
		AddEntry("arlasset://sounds/metalstone3.ogg", "arlasset://sounds/metalstone3.mp3");
		AddEntry("arlasset://sounds/plasticgrass.ogg", "arlasset://sounds/plasticgrass.mp3");
		AddEntry("arlasset://sounds/plasticgrass2.ogg", "arlasset://sounds/plasticgrass2.mp3");
		AddEntry("arlasset://sounds/plasticgrass3.ogg", "arlasset://sounds/plasticgrass3.mp3");
		AddEntry("arlasset://sounds/plasticice.ogg", "arlasset://sounds/plasticice.mp3");
		AddEntry("arlasset://sounds/plasticice2.ogg", "arlasset://sounds/plasticice2.mp3");
		AddEntry("arlasset://sounds/plasticice3.ogg", "arlasset://sounds/plasticice3.mp3");
		AddEntry("arlasset://sounds/plasticmetal.ogg", "arlasset://sounds/plasticmetal.mp3");
		AddEntry("arlasset://sounds/plasticmetal2.ogg", "arlasset://sounds/plasticmetal2.mp3");
		AddEntry("arlasset://sounds/plasticmetal3.ogg", "arlasset://sounds/plasticmetal3.mp3");
		AddEntry("arlasset://sounds/plasticplastic.ogg", "arlasset://sounds/plasticplastic.mp3");
		AddEntry("arlasset://sounds/plasticplastic2.ogg", "arlasset://sounds/plasticplastic2.mp3");
		AddEntry("arlasset://sounds/plasticplastic3.ogg", "arlasset://sounds/plasticplastic3.mp3");
		AddEntry("arlasset://sounds/plasticstone.ogg", "arlasset://sounds/plasticstone.mp3");
		AddEntry("arlasset://sounds/plasticstone2.ogg", "arlasset://sounds/plasticstone2.mp3");
		AddEntry("arlasset://sounds/plasticstone3.ogg", "arlasset://sounds/plasticstone3.mp3");
		AddEntry("arlasset://sounds/stone.ogg", "arlasset://sounds/stone.mp3");
		AddEntry("arlasset://sounds/stone2.ogg", "arlasset://sounds/stone2.mp3");
		AddEntry("arlasset://sounds/stone3.ogg", "arlasset://sounds/stone3.mp3");
		AddEntry("arlasset://sounds/woodgrass.ogg", "arlasset://sounds/woodgrass.mp3");
		AddEntry("arlasset://sounds/woodgrass2.ogg", "arlasset://sounds/woodgrass2.mp3");
		AddEntry("arlasset://sounds/woodgrass3.ogg", "arlasset://sounds/woodgrass3.mp3");
		AddEntry("arlasset://sounds/woodice.ogg", "arlasset://sounds/woodice.mp3");
		AddEntry("arlasset://sounds/woodice2.ogg", "arlasset://sounds/woodice2.mp3");
		AddEntry("arlasset://sounds/woodice3.ogg", "arlasset://sounds/woodice3.mp3");
		AddEntry("arlasset://sounds/woodmetal.ogg", "arlasset://sounds/woodmetal.mp3");
		AddEntry("arlasset://sounds/woodmetal2.ogg", "arlasset://sounds/woodmetal2.mp3");
		AddEntry("arlasset://sounds/woodmetal3.ogg", "arlasset://sounds/woodmetal3.mp3");
		AddEntry("arlasset://sounds/woodplastic.ogg", "arlasset://sounds/woodplastic.mp3");
		AddEntry("arlasset://sounds/woodplastic2.ogg", "arlasset://sounds/woodplastic2.mp3");
		AddEntry("arlasset://sounds/woodplastic3.ogg", "arlasset://sounds/woodplastic3.mp3");
		AddEntry("arlasset://sounds/woodstone.ogg", "arlasset://sounds/woodstone.mp3");
		AddEntry("arlasset://sounds/woodstone2.ogg", "arlasset://sounds/woodstone2.mp3");
		AddEntry("arlasset://sounds/woodstone3.ogg", "arlasset://sounds/woodstone3.mp3");
		AddEntry("arlasset://sounds/woodwood.ogg", "arlasset://sounds/woodwood.mp3");
		AddEntry("arlasset://sounds/woodwood2.ogg", "arlasset://sounds/woodwood2.mp3");
		AddEntry("arlasset://sounds/woodwood3.ogg", "arlasset://sounds/woodwood3.mp3");

		AddEntry("arlasset://fonts/clonewand.mesh", "arlasset://retros/tools/clonewand.mesh");
		AddEntry("arlasset://fonts/hammer.mesh", "arlasset://retros/tools/hammer.mesh");
		AddEntry("arlasset://fonts/PaintballGun.mesh", "arlasset://retros/tools/PaintballGun.mesh");
		AddEntry("arlasset://fonts/Rocket.rbxm", "arlasset://retros/tools/Rocket.arlm");
		AddEntry("arlasset://fonts/rocketlauncher.mesh", "arlasset://retros/tools/rocketlauncher.mesh");
		AddEntry("arlasset://fonts/slingshot.mesh", "arlasset://retros/tools/slingshot.mesh");
		AddEntry("arlasset://fonts/SlingshotPellet.rbxm", "arlasset://retros/tools/SlingshotPellet.arlm");
		AddEntry("arlasset://fonts/sword.mesh", "arlasset://retros/tools/sword.mesh");
		AddEntry("arlasset://fonts/timebomb.mesh", "arlasset://retros/tools/timebomb.mesh");
		AddEntry("arlasset://fonts/tree.mesh", "arlasset://retros/tools/tree.mesh");
		AddEntry("arlasset://fonts/trowel.mesh", "arlasset://retros/tools/trowel.mesh");

	}
	
	void LegacyContentTable::AddEntry(const std::string& path, const std::string& contentId)
	{
		std::string url = path;
		normalizeUrl(url);
		
		mMap[url] = contentId;
	}
	
	void LegacyContentTable::AddEntryProd(const std::string& path, const std::string& contentId)
	{
		std::string url = path;
		normalizeUrl(url);
		
		mMap[url] = format("http://www.anorrl.com/asset/?id=%s", contentId.c_str());
	}
	
	const std::string& LegacyContentTable::FindEntry(const std::string& path)
	{
		std::string url = path;
		normalizeUrl(url);
		
		UrlMap::const_iterator it = mMap.find(url);
		
		return (it == mMap.end()) ? mEmpty : it->second;
	}
}
