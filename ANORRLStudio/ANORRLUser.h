/**
 * ANORRLUser.h
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

// Qt headers
#include <QObject>
#include "util/HttpAsync.h"

class ANORRLUser : public QObject
{
	Q_OBJECT

Q_SIGNALS:
	void userLoaded();

public:
	static ANORRLUser& singleton();	
	void init();
	int getUserId();
	std::string getUserName();

public Q_SLOTS:
	void onAuthenticationChanged(bool);

private:
	ANORRLUser();
	~ANORRLUser();
	void getWebkitUserId();
	void currentUserReplied(ARL::HttpFuture future);
	int m_webKitUserId; // -1 means not initialized, 0 means not authenticated, > 0 means logged in as that userid
	std::string m_userName;
    boost::shared_future<void> m_webKitUserIDQuery;
};
