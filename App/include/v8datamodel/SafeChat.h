#pragma once

#include "v8tree/Instance.h"
#include "v8tree/Service.h"

namespace ARL {
	class ChatOption;

//	extern const char *const sSafeChat;
	class SafeChat
		
	{
	public:
		SafeChat()
		{
		//	setName("SafeChat");
			loadChatTree();
		}

		static SafeChat& singleton();

		ChatOption *getChatRoot() { return chatRoot.get(); }

		std::string getMessage(std::vector<std::string> code);
	private:
		boost::scoped_ptr<ChatOption> chatRoot;

		void loadChatTree();

		void loadChildren(ChatOption *node, const XmlElement *DOMsubTree);
	};

	class ChatOption
	{
	public:
		ChatOption() {}
		~ChatOption();
		ChatOption(std::string text) {this->text = text;}
		
		std::vector<ChatOption *> children;
		std::string text;


	};
} // namespace
