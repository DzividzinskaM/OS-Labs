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
const int HEADER_SIZE = AVAILABLITY_OFFSET + SIZE_OFFSET;


struct PageDescriptor {

    uint8_t* firstAvailableBlockPtr;
    bool availabilty = true;
    int numberFreeBlocks;
    int blockSize;

    /*PageDescriptor(uint8_t* start, int blockSize) {
        firstAvailableBlockPtr = start;
        this->blockSize = blockSize;
        numberFreeBlocks = PAGE_SIZE / blockSize;
    }*/

};

class Allocator {
public:
    
    uint8_t* startPtr;

    int* numbers = new int[NUMBER_PAGES];

    PageDescriptor* pageDescriptors =  new PageDescriptor[NUMBER_PAGES];

    Allocator();

    void* mem_alloc(size_t size);

    void mem_free(void* addr);

    void* mem_realloc(void* addr, size_t size);

    void mem_dump();

private: 

    void init_pages();

    void* alloc_block_on_page(size_t needSize);

    int get_page_number(size_t needSize);

    void mem_dump_page_blocks(PageDescriptor page, int pageNumber);

};




Allocator::Allocator()
{
    startPtr = (uint8_t*)malloc(1);
    init_pages();
    
}




void Allocator::init_pages()
{
    uint8_t* currPtr = startPtr;
    int currBlockSize = MIN_BLOCK_SIZE;
    for (int i = 0; i < NUMBER_PAGES; i++)
    {
        PageDescriptor pageDesciptor;
        pageDesciptor.firstAvailableBlockPtr = currPtr;
        pageDesciptor.blockSize = currBlockSize;
        pageDesciptor.numberFreeBlocks = PAGE_SIZE / currBlockSize;

        pageDescriptors[i] = pageDesciptor;

        currPtr += PAGE_SIZE;

        if (currBlockSize != MAX_BLOCK_SIZE) {
            currBlockSize *= EXPANSION;
        }

    }
}




void* Allocator::mem_alloc(size_t size)
{
    size_t needSize = size + HEADER_SIZE;

    if (needSize < PAGE_SIZE) {

        return(alloc_block_on_page(needSize));
    }
}


void* Allocator::alloc_block_on_page(size_t needSize)
{
    int needPage = get_page_number(needSize);
    
    PageDescriptor currPageDescriptor = pageDescriptors[needPage];

    if (currPageDescriptor.availabilty) {
        uint8_t* currPtr = currPageDescriptor.firstAvailableBlockPtr;
        
        *currPtr = false;
        *(currPtr + HEADER_SIZE) = needSize;

        currPageDescriptor.firstAvailableBlockPtr += currPageDescriptor.blockSize;
        currPageDescriptor.numberFreeBlocks--;
        pageDescriptors[needPage] = currPageDescriptor;
        return (void*)currPtr;
    }

   
}


int Allocator::get_page_number(size_t needSize)
{
    size_t currSize = MIN_BLOCK_SIZE;
    int needPage = 0;

    while (currSize != MAX_BLOCK_SIZE) {
        if (currSize > needSize)
            return needPage;
        else {
            currSize *= EXPANSION;
            needPage++;
        }
    }
}











void Allocator::mem_dump()
{
    int countPage = 1;

    void* currPagePtr = startPtr;

    cout << "------------------MEM DUMP ------------------" << endl;
    for (int i=0; i<NUMBER_PAGES; i++)
    {
        cout << "Page " << countPage;
        cout << "\t Start " << currPagePtr;
        cout << "\t Block Size" << pageDescriptors[i].blockSize;
        cout << "\t Free blocks" << pageDescriptors[i].numberFreeBlocks;
        if (pageDescriptors[i].availabilty)
            cout << "\tAvailabile" << endl;
        else
            cout << "\t Not available" << endl;
        if (PAGE_SIZE / pageDescriptors[i].blockSize != pageDescriptors[i].numberFreeBlocks) {
            mem_dump_page_blocks(pageDescriptors[i], i);
        }
        cout << endl;

        currPagePtr = (void*)((size_t)currPagePtr + PAGE_SIZE);
        countPage++;
    }
}

void Allocator::mem_dump_page_blocks(PageDescriptor page, int pageNumber)
{
    uint8_t* currentPtr = startPtr;
    if (pageNumber != 0)
        currentPtr = startPtr + (pageNumber * PAGE_SIZE);

    int blockNumber = PAGE_SIZE / page.blockSize;


    size_t localOffset = 0;
    for (int i = 0; i < blockNumber - page.numberFreeBlocks; i++) {
        bool isAvailable = *currentPtr;
        size_t dataSize = *(currentPtr + HEADER_SIZE);

        cout << "\tBlock " << i+1;
        cout << "\tBlock address " << (void*)currentPtr;
        if (!isAvailable) {
            cout << "\tNot available";
            cout << "\tData size " << dataSize;
        }
        cout << endl;

        currentPtr += page.blockSize;
    }

    for (int i = blockNumber - page.numberFreeBlocks; i < blockNumber; i++) {
        cout << "\tBlock " << i + 1;
        cout << "\tBlock address " << (void*)currentPtr;
        cout << "\tAvailable";
        
        cout << endl;

        currentPtr += page.blockSize;
    }

   
  
}









int main()
{
    Allocator allocator = Allocator();
    allocator.mem_dump();

    cout << "------------------Allocation 25 ------------------" << endl;
    cout << allocator.mem_alloc(25) << endl;
    cout << endl;
    allocator.mem_dump();

    cout << "------------------Allocation 18 ------------------" << endl;
    cout << allocator.mem_alloc(18) << endl;
    cout << endl;
    allocator.mem_dump();


    cout << "------------------Allocation 10 ------------------" << endl;
    cout << allocator.mem_alloc(10) << endl;
    cout << endl;
    allocator.mem_dump();


}