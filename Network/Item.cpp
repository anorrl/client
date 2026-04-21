
#include "Item.h"
#include "streaming.h"

namespace ARL { 
namespace Network {

void Item::writeItemType(RakNet::BitStream& stream, Item::ItemType value)
{
    // using a 5b item id.
	BOOST_STATIC_ASSERT(ItemTypeMaxValue < 19);

	if (value>=1 && value<=3)
	{
		// Compact form
		stream.WriteBits((const unsigned char*) &value, 2);
	}
	else
	{
		const char zero = 0;
		stream.WriteBits((const unsigned char*) &zero, 2);
		// Long form
		stream.WriteBits((const unsigned char*) &value, 5);

	}
}

void Item::readItemType(RakNet::BitStream& stream, Item::ItemType& value)
{
	unsigned int t;
	readFastN<2>( stream, t );
	value = static_cast<ItemType>(t);

	if (value != 0)
	{
		// Compact form
	}
	else
	{
		unsigned int t;
		readFastN<5>( stream, t );
		value = static_cast<ItemType>(t);
	}
}

	

bool ItemQueue::validate() const 
{
	return true;
}

bool ItemQueue::preValidate(int i)  
{
	ARLASSERT(inCode == 0);
	inCode = i;
	ARLASSERT(validate());
	return true;
}

bool ItemQueue::postValidate(int i)
{
	ARLASSERT(validate());
	ARLASSERT(inCode == i);
	inCode = 0;
	return true;
}

ItemQueue::ItemQueue()
{
	inCode = 0;
}

ItemQueue::~ItemQueue()
{
	ARLASSERT(inCode == 0);
}

bool ItemQueue::empty() const
{
	return itemList.empty();
}

size_t ItemQueue::size() const
{
	return itemList.size();
}

ARL::Time::Interval ItemQueue::head_wait() const
{
	if (itemList.empty())
		return ARL::Time::Interval::zero();

	return ARL::Time::now<ARL::Time::Fast>() - itemList.front().timestamp;
}

ARL::Time ItemQueue::head_time() const
{
	if (itemList.empty())
		return ARL::Time();

	return itemList.front().timestamp;
}

void ItemQueue::deleteAll() 
{
	Item* item;
	while (pop_if_present(item))
		delete item;
}

void ItemQueue::clear()
{
	itemList.clear();
}

bool ItemQueue::pop_if_present(Item*& item) 
{
	ARLASSERT(preValidate(3));

	if (itemList.empty())
	{
		ARLASSERT(postValidate(3));
		return false;
	}

	item = &itemList.front();
	itemList.pop_front();

	ARLASSERT(postValidate(3));
	return true;
}


void ItemQueue::push_back(Item* item) 
{
	item->timestamp = ARL::Time::now<ARL::Time::Fast>();
	ARLASSERT(preValidate(4));
	itemList.push_back(*item);
	ARLASSERT(postValidate(4));
}


void ItemQueue::push_front(Item* item) 
{
	item->timestamp = ARL::Time::now<ARL::Time::Fast>();
	ARLASSERT(preValidate(5));
	itemList.push_front(*item);
	ARLASSERT(postValidate(5));
}

void ItemQueue::push_front_preserve_timestamp(Item* item)
{
	ARLASSERT(preValidate(5));
	itemList.push_front(*item);
	ARLASSERT(postValidate(5));
}



}}	// namespace

