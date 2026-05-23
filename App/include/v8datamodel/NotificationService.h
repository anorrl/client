//
//  NotificationService.h
//  Copyright ROBLOX Corp 2014
//
//  Created by Ganesh Agrawal on 5/15/14.
//
//

#pragma once

#include "V8Tree/Instance.h"
#include "V8Tree/Service.h"

namespace ARL
{
	extern const char* const sNotificationService;
	class NotificationService
    : public DescribedNonCreatable<NotificationService, Instance, sNotificationService, Reflection::ClassDescriptor::INTERNAL>
    , public Service
	{
	private:
		typedef DescribedNonCreatable<NotificationService, Instance, sNotificationService, Reflection::ClassDescriptor::INTERNAL> Super;
        
		bool canUseService();
        
	public:
		NotificationService();
        
        arl::signal<void(int, int, std::string, int)> scheduleNotificationSignal;
        arl::signal<void(int, int)> cancelNotificationSignal;
        arl::signal<void(int)> cancelAllNotificationSignal;
        arl::signal<void(int, boost::function<void(shared_ptr<const Reflection::ValueArray>)>, boost::function<void(std::string)>)> getScheduledNotificationsSignal;

        
        void scheduleNotification(int userId, int alertId, std::string alerMsg, int minutesToFire);
        void cancelNotification(int userId, int alertId);
        void cancelAllNotification(int userId);
        void getScheduledNotifications(int userId, boost::function<void(shared_ptr<const Reflection::ValueArray>)> resumeFunction, boost::function<void(std::string)>	errorFunction);
	};
}