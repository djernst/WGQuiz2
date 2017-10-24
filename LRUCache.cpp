//============================================================================
// Name        : LRUCache.cpp
// Author      : Dan Ernst
// Version     :
// Copyright   : Ernstco
// Description : LRUCache in C++, Ansi-style
//============================================================================
// I believe put() and get() operates in O(1) complexity. Notice that there are no loops iterating of any lists or arrays except
// in the initializer
// clear() operates in O(1), constant complexity because the list is held to being less than or equal to m_maxCapacity
//
// dumpList is a utility.  It operates in O(1); constant complexity, because no more than m_maxCapacity elements are allowed in the array.
// dumpKeys() is a utility and operates in O(1). It dumps the contents of the key lookup table and is fixed in size (256 elements)
//
// The theory of operation is such that when an item is new, it is added to the front of the itemsList.  If the list is already at
// capacity, the oldest (last) item is removed and then the new item is added to the front of the itemsList.
// If an item is being updated (its already in the itemsList) it is first removed, then updated and then added to the front of the itemsList.
// If an item is being accessed (get) the item is first extracted, then it is deleted from the itemsList and then added back to the front of
// itemsList. Then the extracted is returned to the caller.
// By using this method, the more frequently accessed items will bubble to the top of the list, while the less frequently accessed items
// will be at the bottom of the list.
// Perhaps using a different (or custom) linked list, this operation could save on the copying of data.


#include <iostream>
#include <list>

using namespace std;
class cacheHitException: public exception
{
  virtual const char* what() const throw()
  {
    return "item not in cache";
  }
};

class indexOutOfRangeException: public exception
{
  virtual const char* what() const throw()
  {
    return "index out of range. Must be 0-255";
  }
};


template <typename T>
class LRUCache{
	struct LRUItem{
		T value;
		int keyIndex;
	};

	struct LRUKey{
		typename std::list<LRUItem>::iterator itemRef;
	};

	struct LRUKey listKeys[256];
	unsigned int m_maxCapacity;
	std::list<LRUItem> itemsList;

public:

	LRUCache(int maxCapacity){
		//listKeys = new LRUKey[256];
		m_maxCapacity = maxCapacity;
		for (int i = 0; i < 256; i++){
			listKeys[i].itemRef = itemsList.end();
		}
		itemsList.clear();

	};

	bool put(int key, T value){
		LRUItem lruItem;
		if (key >= 256) return false;

		if (listKeys[key].itemRef != itemsList.end()){    	// Key already in list.  Lets replace value and update the list
			lruItem = (*listKeys[key].itemRef);
			lruItem.value = value;
			itemsList.erase(listKeys[key].itemRef);
			itemsList.push_front(lruItem);
			listKeys[key].itemRef = itemsList.begin();
			cout << "updating value to itemList" << endl;


		} else if (itemsList.size()< m_maxCapacity){    	// We are not yet a capacity, so just add the entry
			lruItem.keyIndex= key;
			lruItem.value = value;
			itemsList.push_front(lruItem);
			listKeys[key].itemRef =  itemsList.begin();
			cout << "inserting new value to itemList" << endl;

		} else {											// We are capacity.  So we must remove the oldest entry before entering a new one
			typename std::list<LRUItem>::iterator lastItem = itemsList.end();
			lastItem--;
			lruItem = (*lastItem);
			int lastItemKeyIndex = lruItem.keyIndex;
			listKeys[lastItemKeyIndex].itemRef = itemsList.end();  // invalidate the old keys reference

			lruItem.keyIndex= key;
			lruItem.value = value;
			itemsList.push_front(lruItem);
			listKeys[key].itemRef =  itemsList.begin();

			cout << "removing oldest entry and inserting new value to itemList" << endl;


		}
		cout << "Key: " << key << " Value: " << value << endl;
		return	true;
	};

	T get(unsigned char key){
		T retVal;
		LRUItem lruItem;
		cacheHitException cacheHitEx;
		indexOutOfRangeException idxOOREx;


		if (key >= 256) throw idxOOREx;

		if (listKeys[key].itemRef != itemsList.end()){
			lruItem = (*listKeys[key].itemRef);
			retVal = (*listKeys[key].itemRef).value;
			itemsList.erase(listKeys[key].itemRef);
			itemsList.push_front(lruItem);
			listKeys[key].itemRef = itemsList.begin();
			return retVal;
		} else {
			throw cacheHitEx;
		}
	};

	void clear(){
		for (int i = 0; i < 256; i++){
			listKeys[i].itemRef = itemsList.end();
		}
		itemsList.clear();
		return;
	};

	void dumpList(){
		typename std::list<LRUItem>::iterator listIt;

		for (listIt = itemsList.begin();listIt != itemsList.end(); listIt++){
			cout << "dumpList: keyIndex " << (*listIt).keyIndex << " value " << (*listIt).value << endl;
		}
	};
	void dumpKeys(){
		for (int i = 0; i < 256; i++){
			if (listKeys[i].itemRef != itemsList.end()){
				cout << "dumpKeys: " << i << " keyIndex " << (*listKeys[i].itemRef).keyIndex << " value " << (*listKeys[i].itemRef).value<< endl;

			}
		}
	};
};



int main() {

	// Show that this class works with several types (string, float and int)

	LRUCache<string> myStringCache (2);
	LRUCache<float> myfloatCache (2);
	LRUCache<int> myIntCache (4);

	// Minimal test with string
	myStringCache.put(1,"foo");
	myStringCache.put(2,"bar");
	cout << "get(1) " << myStringCache.get(1) << endl;
	cout << "get(2) " << myStringCache.get(2) << endl;
	try {cout << "get(5) " << myStringCache.get(5) << endl;} catch( const exception& e ) {cout << e.what() << endl;}
	myStringCache.dumpList();

	// Minimal test with float
	myfloatCache.put(1,0.4);
	myfloatCache.put(2,1.2);
	cout << "get(1) " << myfloatCache.get(1) << endl;
	cout << "get(2) " << myfloatCache.get(2) << endl;
	try {cout << "get(5) " << myfloatCache.get(5) << endl;} catch( const exception& e ) {cout << e.what() << endl;}
	cout << "get(2) " << myfloatCache.get(2) << endl;
	myfloatCache.dumpList();


	// More in-depth test with int

	myIntCache.put(2,2);
	myIntCache.put(3,3);
	myIntCache.put(4,4);
	myIntCache.put(5,5);
	myIntCache.put(5,6);
	myIntCache.dumpList();

	try {cout << "get(6) " << myIntCache.get(6) << endl;} catch( const exception& e ) {cout << e.what() << endl;}
	try {cout << "get(5) " << myIntCache.get(5) << endl;} catch( const exception& e ) {cout << e.what() << endl;}
	try {cout << "get(1) " << myIntCache.get(1) << endl;} catch( const exception& e ) {cout << e.what() << endl;}
	try {cout << "get(2) " << myIntCache.get(2) << endl;} catch( const exception& e ) {cout << e.what() << endl;}
	try {cout << "get(3) " << myIntCache.get(3) << endl;} catch( const exception& e ) {cout << e.what() << endl;}
	try {cout << "get(4) " << myIntCache.get(4) << endl;} catch( const exception& e ) {cout << e.what() << endl;}
	try {cout << "get(2) " << myIntCache.get(2) << endl;} catch( const exception& e ) {cout << e.what() << endl;}

	myIntCache.dumpList();

	myIntCache.clear();


	return 0;
}

