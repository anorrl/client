#pragma once

#include "v8tree/Verb.h"
#include "arl/CEvent.h"

namespace ARL {

class Game;
class View;
class ViewBase;
class Document;
class WebBrowserDialog;
class Application;

// Request to leave the game.  Results in process shutdown.
class LeaveGameVerb : public Verb
{
	View& v;

public:
	LeaveGameVerb(View& v, VerbContainer* container);
    virtual ~LeaveGameVerb(){}
	virtual void doIt(IDataState* dataState);
};

// Request to take screenshot of current game
class ScreenshotVerb : public ARL::Verb
{
	Game* game;
	ViewBase* view;
	const Document& doc;

	void screenshotFinished(const std::string &filename);
	void askUploadScreenshot(std::string filename);
	void uploadScreenshot(const std::string &filename);

public:
	ScreenshotVerb(const Document& doc, ViewBase* view, Game* game);
	virtual void doIt(IDataState* dataState);
	virtual bool isEnabled() const { return true; }
};


// Request to toggle fullscreen
class ToggleFullscreenVerb : public ARL::Verb
{

private:
	// Needed because toggle fullscreen is disabled while recording
	View& view;

public:
	ToggleFullscreenVerb(View& view, VerbContainer* container);
	virtual bool isChecked() const;
	virtual bool isEnabled() const;
	virtual void doIt(ARL::IDataState* dataState);
};


}  // namespace ARL
