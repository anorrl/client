/**
 * VideoControl.cpp
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#include "stdafx.h"
#include "VideoControl.h"

// System Headers
#define NOMINMAX        // need to define this before windows.h
#include <d3d9.h>
#include <d3dx9.h>

// Roblox Headers
#include "v8datamodel/ContentProvider.h"
#include "v8datamodel/GameSettings.h"
#include "util/FileSystem.h"
#include "util/standardout.h"
#include "GfxBase/FrameRateManager.h"
#include "GfxBase/ViewBase.h"

using namespace ARL;

namespace ARL{

	
	static void logError(std::string errorString)
	{
		ARL::StandardOut::singleton()->printf(ARL::MESSAGE_ERROR, "%s\r\n", errorString.c_str());
	}

	VideoControl::VideoControl(IVideoCapture *capture, ARL::ViewBase *rbxView, FrameRateManager *frameRateManager, Verb *verb)
	{
		this->capture.reset(capture);
		ARLASSERT(verb);
		this->verb = verb;

		ARLASSERT(rbxView);
		this->rbxView = rbxView;
		this->frameRateManager = frameRateManager;

		recorded = false;
		videoQuality =  -1;
		setVideoQuality(GameSettings::singleton().getVideoQualitySetting() );
	}

	bool VideoControl::isVideoRecordingStopped() 	
	{
		return !capture->isRunning();
	}

	bool VideoControl::isVideoRecording()
	{	
		return capture->isRunning();
	}

	bool VideoControl::isVideoPaused()
	{
		return false;
	}

	bool VideoControl::isReadyToUpload()
	{
		return recorded && !capture->isRunning();
	}

	void VideoControl::startRecording(ARL::Soundscape::SoundService *soundservice)
	{
		setVideoQuality(GameSettings::singleton().getVideoQualitySetting());
		ARLASSERT(soundservice);

		if(soundservice)
		{
			soundState.reset(new SoundState());

			soundState->createDSPFunction = boost::bind(&ARL::Soundscape::SoundService::createDSP, soundservice, _1);
			soundState->getSampleRateFunction = boost::bind(&ARL::Soundscape::SoundService::getSampleRate, soundservice);
			soundState->enabledFunction = boost::bind(&ARL::Soundscape::SoundService::enabled, soundservice);
		}	

        std::pair<unsigned, unsigned> dimensions = rbxView->setFrameDataCallback(boost::bind(&VideoControl::onFrameData, this, _1));

        bool captureStarted = dimensions.first && dimensions.second && capture->start(dimensions.first, dimensions.second, soundState.get());
        ARLASSERT(captureStarted);

		if (captureStarted)
		{
			ARL::StandardOut::singleton()->printf(ARL::MESSAGE_INFO,  "Video recording started");
			frameRateManager->PauseAutoAdjustment();
			recorded = false;
		}
	}

	void VideoControl::stopRecording()
	{
		recorded = true;

		capture->stop();
		frameRateManager->ResumeAutoAdjustment();

        rbxView->setFrameDataCallback(boost::function<void(void*)>());
	}

	void VideoControl::pause()
	{
	}

	void VideoControl::unPause()
	{
	}

	void VideoControl::setVideoQuality(int vq)
	{	
		capture->setVideoQuality(vq);
	}

    void VideoControl::onFrameData(void* device)
    {
		if (capture->isRunning())
		{
			capture->pushNextFrame(device, verb);
		}
    }
}
