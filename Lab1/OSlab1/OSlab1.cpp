// OS-lab1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

using namespace std;

const int offsetBlockAvailablity = 1;
const int offsetCurrBlockSize = 4;
const int headerSize = 5;
const int alignment = 4;

class Allocator {
public:
	size_t totalSize;
	uint8_t* startPtr;
	int blockNumber;

	Allocator(size_t size) {
		totalSize = size;
		startPtr = (uint8_t*)malloc(totalSize);
		*(startPtr + offsetBlockAvailablity) = true;
		*(startPtr + headerSize) = totalSize - headerSize;
		blockNumber = 1;
	}

	void* mem_alloc(size_t size) {

		uint8_t* currentPtr = startPtr;

		for (int i = 0; i < totalSize; i++) {

			bool isAvailable = *(currentPtr + offsetBlockAvailablity);
			size_t blockSize = *(currentPtr + headerSize);
			size_t adjustedSize = calculateSize(size);

			if (isAvailable && blockSize >= adjustedSize) {

				size_t localOffset = offsetBlockAvailablity;
				*(currentPtr + localOffset) = false;

				void* startResBlockPtr = currentPtr + localOffset;

				localOffset += offsetCurrBlockSize;
				*(currentPtr + localOffset) = adjustedSize;
				localOffset += adjustedSize;

				localOffset += offsetBlockAvailablity;
				*(currentPtr + localOffset) = true;
				localOffset += offsetCurrBlockSize;
				*(currentPtr + localOffset) = blockSize - adjustedSize;

				blockNumber++;
				return startResBlockPtr;

			}
			currentPtr += headerSize;
			currentPtr += blockSize;
		}

		return NULL;
	}

	size_t calculateSize(size_t size) {
		size_t needSize = size + headerSize;
		if (needSize % alignment == 0)
			return size;
		else {
			size_t resSize = needSize + (alignment - (needSize % alignment));
			return resSize - headerSize;
		}
	}

	void mem_dump() {

		uint8_t* currentPtr = startPtr;
		size_t localOffset = 0;
		for (int i = 0; i < blockNumber; i++) {

			bool isAvailable = *(currentPtr + offsetBlockAvailablity);
			size_t dataSize = *(currentPtr + headerSize);

			cout << "Block " << i;
			if (isAvailable)
				cout << "\tAvailable";
			else
				cout << "\tNot available";
			cout << "\tData size " << dataSize;
			cout << "\tGeneral block size " << dataSize + headerSize;
			cout << endl;

			currentPtr += headerSize;
			currentPtr += dataSize;
		}
	}

};

void main()
{

	Allocator allocator(1024);

	cout << "Start pointer " << (void*)allocator.startPtr << endl;


	cout << "----------Allocation ---- size 20 ----" << endl;
	cout << allocator.mem_alloc(20) << endl;
	cout << endl;
	allocator.mem_dump();

	cout << endl;
	cout << "----------Allocation ---- size 50 ----" << endl;
	cout << allocator.mem_alloc(50) << endl;
	cout << endl;
	allocator.mem_dump();






}
