#pragma once

#include "v8datamodel/contentprovider.h"

namespace ARL {

		class AsyncResult
		{
		public:
			AsyncResult()
				: reqResult(ARL::AsyncHttpQueue::Succeeded)
			{
			};
			
			// make result always more restrictive only.
			// Succeeded < Waiting < Failed.
			void returnResult(ARL::AsyncHttpQueue::RequestResult reqResult)
			{
				switch(reqResult)
				{
				case ARL::AsyncHttpQueue::Succeeded:
					break;
				case ARL::AsyncHttpQueue::Waiting:
					if(this->reqResult == ARL::AsyncHttpQueue::Succeeded)
					{
						this->reqResult = reqResult;
					}
					break;
				case ARL::AsyncHttpQueue::Failed:
					this->reqResult = reqResult;
					break;
				}
			}

			void returnWaitingFor(const ARL::ContentId& id)
			{
				returnResult(ARL::AsyncHttpQueue::Waiting);
				waitingFor.push_back(id);
			}

			ARL::AsyncHttpQueue::RequestResult reqResult;
			std::vector<ARL::ContentId> waitingFor;
		};


}