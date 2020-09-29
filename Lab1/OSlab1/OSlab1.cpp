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
		*(startPtr + headerSize) = totalSize-headerSize;
		blockNumber = 1;
	}

	size_t calculate_size(size_t size) {
		size_t needSize = size + headerSize;
		if (needSize % alignment == 0)
			return size;
		else {
			size_t resSize = needSize + (alignment - (needSize % alignment));
			return resSize - headerSize;
		}
	}


	void* mem_alloc(size_t size) {

		uint8_t* currentPtr = startPtr;

		for (int i = 0; i < totalSize; i++) {

			bool isAvailable = *(currentPtr + offsetBlockAvailablity);
			size_t blockSize = *(currentPtr + headerSize);
			size_t adjustedSize = calculate_size(size);

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
				*(currentPtr + localOffset) = blockSize - adjustedSize - headerSize;

				blockNumber++;
				return startResBlockPtr;

			}
			currentPtr += headerSize;
			currentPtr += blockSize;
		}

		return NULL;
	}


	void mem_free(void* addr) {

		uint8_t* currentPtr = startPtr;

		for (int i = 0; i < blockNumber; i++) {
			bool isAvailable = *(currentPtr + offsetBlockAvailablity);
			size_t dataSize = *(currentPtr + headerSize);


			if ((currentPtr + offsetBlockAvailablity == (uint8_t*)addr) && !isAvailable) {
				*(currentPtr + offsetBlockAvailablity) = true;
				merging_free_blocks();
				return;
			}

			currentPtr += headerSize;
			currentPtr += dataSize;
		}
	}

	void* mem_realloc(void* addr, size_t size) {
		uint8_t* currentPtr = startPtr;

		for (int i = 0; i < blockNumber; i++) {
			bool isAvailable = *(currentPtr + offsetBlockAvailablity);
			size_t dataSize = *(currentPtr + headerSize);


			if ((currentPtr + offsetBlockAvailablity == (uint8_t*)addr) && dataSize < calculate_size(size)) {

				void* resAddress = mem_alloc(size);
				if (resAddress == NULL)
					return NULL;
				else {
					mem_free(addr);
					return resAddress;
				}
			}
			else if((currentPtr + offsetBlockAvailablity == (uint8_t*)addr) && dataSize >= calculate_size(size)) {
				size_t adjustedSize = calculate_size(size);

				size_t localOffset = offsetBlockAvailablity;
				*(currentPtr + localOffset) = false;

				void* startResBlockPtr = currentPtr + localOffset;
				localOffset += offsetCurrBlockSize;

				*(currentPtr + localOffset) = adjustedSize;


				if (dataSize - adjustedSize - headerSize <= headerSize) {
					localOffset += dataSize;
				}

				else {
					localOffset += adjustedSize;
					localOffset += offsetBlockAvailablity;
					*(currentPtr + localOffset) = true;
					localOffset += offsetCurrBlockSize;
					*(currentPtr + localOffset) = dataSize - adjustedSize - headerSize;
					blockNumber++;
				}

				merging_free_blocks();
				return startResBlockPtr;
			}

			currentPtr += headerSize;
			currentPtr += dataSize;
		}
		return NULL;
	}

	void merging_free_blocks() {

		uint8_t* currentPtr = startPtr;

		for (int i = 0; i < blockNumber; i++) {

			bool isAvailbaleBlockI = *(currentPtr + offsetBlockAvailablity);
			size_t dataSizeBlockI = *(currentPtr + headerSize);

			if (isAvailbaleBlockI) {

				uint8_t* firstFreeBlockPtr = currentPtr + offsetBlockAvailablity;
				int numberFreeBlocks = 0;
				size_t freeSize = dataSizeBlockI;

				currentPtr += (headerSize + dataSizeBlockI);

				for (int j = i+1; j < blockNumber; j++) {


					bool isAvailbaleBlockJ = *(currentPtr + offsetBlockAvailablity);
					size_t dataSizeBlockJ = *(currentPtr + headerSize);

					if (isAvailbaleBlockJ)
					{
						numberFreeBlocks++;
						freeSize += dataSizeBlockJ;
						freeSize += headerSize;

					}

					if (!isAvailbaleBlockJ || (isAvailbaleBlockJ && j == blockNumber - 1)) {

						*(firstFreeBlockPtr + offsetCurrBlockSize) = freeSize;
						blockNumber -= numberFreeBlocks;

						return;
					}

					currentPtr += headerSize;
					currentPtr += dataSizeBlockJ;
				}
			}

			currentPtr += headerSize;
			currentPtr += dataSizeBlockI;
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
			cout << "\tBlock address " << (void*)(currentPtr + offsetBlockAvailablity);
			cout << endl;

			currentPtr += headerSize;
			currentPtr += dataSize;
		}
	}

};

void main()
{

	Allocator allocator(256);
	allocator.mem_dump();

	cout << "Start pointer " << (void*)allocator.startPtr << endl;


	cout << "----------Allocation ---- size 20 ----" << endl;
	void* addr1 = allocator.mem_alloc(20);
	cout << addr1 << endl;
	cout << endl;
	allocator.mem_dump();

	cout << endl;
	cout << "----------Allocation ---- size 50 ----" << endl;
	void* addr = allocator.mem_alloc(50);
	cout << addr << endl;
	cout << endl;
	allocator.mem_dump();


	cout << endl;
	cout << "----------Allocation ---- size 40 ----" << endl;
	addr1 = allocator.mem_alloc(40);
	cout << addr1 << endl;
	cout << endl;
	allocator.mem_dump();

	cout << endl;
	cout << "----------Realloc ---- " << addr1 << "---" << "size 50 ----" << endl;
	addr1 = allocator.mem_realloc(addr1, 50);
	cout << addr1 << endl;
	cout << endl;
	allocator.mem_dump();


	cout << endl;
	cout << "----------Free ---- " << addr << "---" << endl;
	allocator.mem_free(addr);
	cout << endl;
	allocator.mem_dump();


	cout << endl;
	cout << "----------Free ---- " << addr1 << "---" << endl;
	allocator.mem_free(addr1);
	cout << endl;
	allocator.mem_dump();


	cout << endl;
	cout << "----------Allocation ---- size 20 ----" << endl;
	addr1 = allocator.mem_alloc(20);
	cout << addr1 << endl;
	cout << endl;
	allocator.mem_dump();
	







}
