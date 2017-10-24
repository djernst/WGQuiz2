//============================================================================
// Name        : LRUCache.cpp
// Author      : Dan Ernst
// Version     :
// Copyright   : Ernstco
// Description : LRUCache in C++, Ansi-style
//============================================================================

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


		//retVal=T{};
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
	void removeLastItem(int key){
		LRUItem lruItem = itemsList.back();
		listKeys[lruItem.keyIndex].itemRef = itemsList.end();
		itemsList.pop_back();

	};
};



int main() {
	LRUCache<string> myStringCache (2);
	LRUCache<float> myfloatCache (2);

	myStringCache.put(1,"foo");
	myStringCache.put(2,"bar");
	cout << "get(1) " << myStringCache.get(1) << endl;
	cout << "get(2) " << myStringCache.get(2) << endl;

	try {cout << "get(5) " << myStringCache.get(5) << endl;} catch( const exception& e ) {cout << e.what() << endl;}

	myStringCache.dumpList();


	myfloatCache.put(1,0.4);
	myfloatCache.put(2,1.2);
	cout << "get(1) " << myfloatCache.get(1) << endl;
	cout << "get(2) " << myfloatCache.get(2) << endl;
	try {cout << "get(5) " << myfloatCache.get(5) << endl;} catch( const exception& e ) {cout << e.what() << endl;}
	cout << "get(2) " << myfloatCache.get(2) << endl;

	myfloatCache.dumpList();


/*
	myCache.put(2,2);
	myCache.put(3,3);
	myCache.put(4,4);
	myCache.put(5,5);
	myCache.put(5,6);

	//myCache.dumpKeys();
	myCache.dumpList();
	myCache.put(4,8);

	//myCache.removeLastItem(3);
	//myCache.put(4,55);

	//myCache.dumpKeys();
	myCache.dumpList();

	cout << "get(1) " << myCache.get(1) << endl;
	//myCache.dumpKeys();
	myCache.dumpList();

	cout << "clearing myCache" << endl;

	myCache.clear();
	myCache.put(5,5);


	myCache.dumpList();
*/
	return 0;
}

