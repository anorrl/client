#pragma once

#include "rbx/Debug.h"
#include <map>

// Poor man's version of a Bi-MultiMap.  Restriction is that each "pair" can only be here once
// Assumes we can have many A's, many B's
// Need to re-write with dual indexed multi-set or some other data structure

namespace ARL {

	template <typename Left, typename Right>
	class BiMultiMap {
	public:
		typedef std::multimap<Left, Right> InternalMap;
		typedef typename InternalMap::iterator InternalMapIt;
		InternalMap internalMap;
		
		bool pairInMap(const Left& left, const Right& right) {
			InternalMapIt it;
			for (it = internalMap.lower_bound(left); it != internalMap.upper_bound(left); ++it) {
				if (it->second == right) {
					return true;
				}
			}
			return false;
		}

		void insertPair(const Left& left, const Right& right) {
			ARLASSERT_SLOW(!pairInMap(left, right));
			internalMap.insert(std::make_pair(left, right));
		}

		void removePair(const Left& left, const Right& right) {
			ARLASSERT_SLOW(pairInMap(left, right));
			typename InternalMap::iterator it;
			for (it = internalMap.lower_bound(left); it != internalMap.upper_bound(left); ++it) {
				if (it->second == right) {
					internalMap.erase(it);
					ARLASSERT(!pairInMap(left, right));
					return;
				}
			}
			ARLASSERT(0);
		}

		bool empty() const {
			return internalMap.empty();
		}

		bool emptyLeft(const Left& left) const {
			return (internalMap.lower_bound(left) == internalMap.upper_bound(left));
		}
	
		template<class Func>
		inline void visitEachLeft(const Left& left, const Func& func) const {
			typename InternalMap::const_iterator it;
			for (it = internalMap.lower_bound(left); it != internalMap.upper_bound(left); ++it) {
				const Right& right = it->second;
				func(left, right);
			}
		}

	};
} // namespace ARL
