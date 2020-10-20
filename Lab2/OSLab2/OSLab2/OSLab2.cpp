// OSLab2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <list>

using namespace std;

const int PAGE_SIZE = 1024;
const int NUMBER_PAGES = 10;
const int EXPANSION = 2;
const int MIN_BLOCK_SIZE = 16;
const int MAX_BLOCK_SIZE = 1024;
const int AVAILABLITY_OFFSET = 1;
const int SIZE_OFFSET = 4;
const int MAX_CELL_SIZE = 256;
const int HEADER_SIZE = AVAILABLITY_OFFSET + SIZE_OFFSET;


struct PageDescriptor {

    uint8_t* firstAvailableBlockPtr;
    bool availabilty = true;
    int numberFreeBlocks;
    //size_t blockSize = PAGE_SIZE;
};


class Allocator {
public:
    
    uint8_t* startPtr;

    PageDescriptor* pageDescriptors =  new PageDescriptor[NUMBER_PAGES];

    Allocator();

    void* mem_alloc(size_t size);

    void mem_free(void* addr);

    void* mem_realloc(void* addr, size_t size);

    void mem_dump();

private: 

    void init_pages();

    void* mem_alloc_block(size_t size);

    void* mem_alloc_page(size_t size);

    int find_start_page(size_t size);

    void write_header(size_t size, uint8_t* start);

    size_t get_size_from_header(uint8_t* start);

    size_t calculate_size(size_t size);

};




Allocator::Allocator()
{
    startPtr = (uint8_t*)malloc(PAGE_SIZE*NUMBER_PAGES);
    init_pages();
    
}




void Allocator::init_pages()
{
    uint8_t* currPtr = startPtr;
    for (int i = 0; i < NUMBER_PAGES; i++)
    {
        PageDescriptor descriptor = PageDescriptor();

        descriptor.firstAvailableBlockPtr = currPtr;
        *(currPtr) = true;
        *(currPtr + HEADER_SIZE) = PAGE_SIZE - AVAILABLITY_OFFSET;

        pageDescriptors[i] = descriptor;

        currPtr += PAGE_SIZE;
    }
}



void* Allocator::mem_alloc(size_t size)
{
    if (size <= PAGE_SIZE / 2)
        return mem_alloc_block(size);
    else
        return mem_alloc_page(size);
}


void* Allocator::mem_alloc_block(size_t size)
{
    size_t neededSize = calculate_size(size);

    for (int i = 0; i < NUMBER_PAGES; i++)
    {
        if (!pageDescriptors[i].availabilty) {
            uint8_t* startBlock = startPtr + (i * PAGE_SIZE);
            size_t blockSize = calculate_size(get_size_from_header(startBlock + AVAILABLITY_OFFSET));

            if (blockSize == neededSize && pageDescriptors[i].numberFreeBlocks > 0) {
                uint8_t* blockCurrPtr = pageDescriptors[i].firstAvailableBlockPtr;

                *blockCurrPtr = false;
                write_header(size, blockCurrPtr + AVAILABLITY_OFFSET);

                pageDescriptors[i].firstAvailableBlockPtr += blockSize;
                pageDescriptors[i].numberFreeBlocks--;

                return (void*)blockCurrPtr;
            }
        }
    }
    
    for (int i = 0; i < NUMBER_PAGES; i++) {
        if (pageDescriptors[i].availabilty) {
            uint8_t* blockCurrPtr = pageDescriptors[i].firstAvailableBlockPtr;

            *blockCurrPtr = false;
            write_header(size, blockCurrPtr + AVAILABLITY_OFFSET);
            
            pageDescriptors[i].firstAvailableBlockPtr += neededSize;
            pageDescriptors[i].availabilty = false;
            pageDescriptors[i].numberFreeBlocks = (PAGE_SIZE / neededSize) - 1;

            return (void*)blockCurrPtr;
        }
    }

}


void* Allocator::mem_alloc_page(size_t size)
{
    cout << "size " << size << endl;
    if (size + HEADER_SIZE < PAGE_SIZE) {
        for (int i = 0; i < NUMBER_PAGES; i++) {
            if (pageDescriptors[i].availabilty) {
                uint8_t* blockCurrPtr = pageDescriptors[i].firstAvailableBlockPtr;
                *blockCurrPtr = false;
                write_header(size, blockCurrPtr + AVAILABLITY_OFFSET);
                      

                blockCurrPtr += (PAGE_SIZE - 1);
                pageDescriptors[i].firstAvailableBlockPtr = blockCurrPtr + HEADER_SIZE + size;
                pageDescriptors[i].availabilty = false;
                pageDescriptors[i].numberFreeBlocks = 0;
                return blockCurrPtr;
            }
        }
    }
    else {
        int startPage = find_start_page(size);
        int needPages = ceil(size / PAGE_SIZE);
        int currSize = size;
        for (int i = startPage; i < startPage+needPages+1; i++)
        {
            pageDescriptors[i].availabilty = false;
            pageDescriptors[i].numberFreeBlocks = 0;
            uint8_t* currPtr = pageDescriptors[i].firstAvailableBlockPtr;

            *currPtr = false;

            if (currSize >= PAGE_SIZE) {

                write_header(PAGE_SIZE, currPtr + AVAILABLITY_OFFSET);

                pageDescriptors[i].firstAvailableBlockPtr += PAGE_SIZE - 1;
                
                currSize -= PAGE_SIZE;
            }
            else {
                write_header(currSize, currPtr + AVAILABLITY_OFFSET);

                pageDescriptors[i].firstAvailableBlockPtr += (currSize + HEADER_SIZE);

                return (void*)pageDescriptors[startPage].firstAvailableBlockPtr;
            }
        }

    }
}

int Allocator::find_start_page(size_t size)
{
    int needPage = ceil(size / PAGE_SIZE);
    int startPage;

    for (int i = 0; i < NUMBER_PAGES; i++) {

        if (pageDescriptors[i].availabilty) {
            startPage = i;
            for (int j = i; j < NUMBER_PAGES; j++) {
                if (j == NUMBER_PAGES - 1)
                    return -1;
                if (pageDescriptors[j].availabilty && pageDescriptors[j+1].availabilty) {
                    needPage--;
                    if (needPage == 0)
                        return startPage;
                }
                if (!pageDescriptors[j].availabilty && pageDescriptors[j].availabilty) {
                    startPage = j;
                    needPage = ceil(size / PAGE_SIZE);
                }
            }
        }
    }
}

void Allocator::write_header(size_t size, uint8_t* start)
{
    int offset = 0;
    int max_part = floor(size / MAX_CELL_SIZE);
    //cout << "max part " << max_part << endl;
    if (max_part == 4) {
        while (max_part > 0) {
            *(start + offset) = MAX_CELL_SIZE - 1;
            max_part--;
            offset++;
        }
        return;
    }

    int rest = size - (max_part * MAX_CELL_SIZE);
   // cout << "rest " << rest << endl;
    while (max_part > 0 || offset != 3) {
        if (max_part == 0) 
            *(start + offset) = 0;
        else {
            *(start + offset) = MAX_CELL_SIZE - 1;
            max_part--;
        }        
        offset++;
       
    }
    *(start + offset) = rest;

   /* offset = 0;
    while (offset < 4) {
        cout << "write size" << (size_t) * (start + offset) << endl;
        offset++;
    }*/
}

size_t Allocator::get_size_from_header(uint8_t* start)
{
    int offset = 0;
    size_t size = 0;

    while (offset < SIZE_OFFSET){

        if ((size_t) * (start + offset) == MAX_CELL_SIZE - 1) {
            size += MAX_CELL_SIZE;
        }
        else {
            size += (size_t) * (start + offset);
        }
        offset++;
    }

    //cout << "SIZE " << size << endl;
    return size;
}



size_t Allocator::calculate_size(size_t size)
{
    size_t need_size = size + HEADER_SIZE;
    size_t currSize = MIN_BLOCK_SIZE;

    while (currSize != MAX_BLOCK_SIZE) {
        if (currSize > need_size)
            return currSize;
        else {
            currSize *= EXPANSION;
        }
    }
}









void Allocator::mem_dump()
{
    for (int i = 0; i < NUMBER_PAGES; i++)
    { 
        cout << "Page " << i;
        if (pageDescriptors[i].availabilty) {
            cout << "\t Available";
            cout << "\t Start page address " << (void*)pageDescriptors[i].firstAvailableBlockPtr << endl;
        }
        else {
            cout << "\t Not available";
            uint8_t* currPagePtr = startPtr + (i * PAGE_SIZE);
            cout << "\t Start page address " << (void*)currPagePtr << endl;
            size_t blockSize = get_size_from_header(currPagePtr + AVAILABLITY_OFFSET);
            size_t currBLockSize = calculate_size(blockSize);
            
            int block = 1;
            if (currBLockSize == PAGE_SIZE) {
                cout << "\t BLock " << block;
                cout << "\t address " << (void*)currPagePtr;
                cout << "\t data size " << blockSize;
                cout << endl;
                continue;
            }

            while (currPagePtr != pageDescriptors[i].firstAvailableBlockPtr) {
                cout << "\t BLock " << block;
                cout << "\t address " << (void*)currPagePtr;
                cout << "\t data size " << get_size_from_header(currPagePtr+AVAILABLITY_OFFSET);
                cout << endl;

                block++;
                if ((size_t) * (currPagePtr + HEADER_SIZE) > PAGE_SIZE)
                    break;
                currPagePtr += currBLockSize;
            }
            
        }
    }
    cout << endl;

}








int main()
{
    Allocator allocator = Allocator();
    allocator.mem_dump();

    cout << "------------------Allocation 8 ------------------" << endl;
    cout << allocator.mem_alloc(8) << endl;
    cout << endl;
    allocator.mem_dump();
    cout << "------------------Allocation 12 ------------------" << endl;
    cout << allocator.mem_alloc(12) << endl;
    cout << endl;
    allocator.mem_dump();
    cout << "------------------Allocation 30 ------------------" << endl;
    cout << allocator.mem_alloc(30) << endl;
    cout << endl;
    allocator.mem_dump();
    cout << "------------------Allocation 20 ------------------" << endl;
    cout << allocator.mem_alloc(20) << endl;
    cout << endl;
    allocator.mem_dump();
    cout << "------------------Allocation 800 ------------------" << endl;
    cout << allocator.mem_alloc(800) << endl;
    cout << endl;
    allocator.mem_dump();

    cout << "------------------Allocation 2000 ------------------" << endl;
    cout << allocator.mem_alloc(2000) << endl;
    cout << endl;
    allocator.mem_dump();
   


}