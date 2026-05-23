//
//  LoginService.h
//  App
//
//  Created by Ben Tkacheff on 5/1/13.
//
//
#include "V8Tree/Instance.h"
#include "V8Tree/Service.h"

namespace ARL
{
    
	extern const char* const sLoginService;
	class LoginService
    : public DescribedNonCreatable<LoginService, Instance, sLoginService>
    , public Service
    
	{
	public:
		LoginService();
        
        arl::signal<void(std::string)> loginSucceededSignal;
        arl::signal<void(std::string)> loginFailedSignal;
        
        arl::signal<void()> promptLoginSignal;
        arl::signal<void()> promptLogoutSignal;
        
        void promptSignup();
        void promptLogin();
        void logout();
	};
    
}