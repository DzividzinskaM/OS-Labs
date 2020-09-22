// OSlab1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <list>



using namespace std;

int alignment = 4;


struct Header {
    size_t currBlockSize;
    bool isAvailble = true;
};


struct Block {
    Header header;
    size_t dataSize = 0;
};


class Allocator {
public:
    size_t totalSize;
    void* start_Ptr;
    int blockNumber = 1;
    list<Block> blocksLst;

    Allocator(size_t size) {
        totalSize = size;
        start_Ptr = malloc(totalSize);
        Header firstHeader;
        firstHeader.currBlockSize = totalSize;
        Block firstBlock;
        firstBlock.header = firstHeader;
        blocksLst.push_back(firstBlock);
    }

    void* mem_alloc(size_t size) {
        if (size == NULL) {
            return nullptr;
        }
        size_t nextAddr = (size_t)start_Ptr;

        for (auto iter = blocksLst.begin(); iter != blocksLst.end(); iter++) {
            Block block = *iter;
            if (block.header.isAvailble && block.header.currBlockSize >= calculateNewBlockSize(size)) {
                if (block.header.currBlockSize - calculateNewBlockSize(size) > sizeof(Header)) {
                    Block newBlock;
                    newBlock.header.isAvailble = false;
                    newBlock.header.currBlockSize = calculateNewBlockSize(size);
                    newBlock.dataSize = size;

                    block.header.currBlockSize -= newBlock.header.currBlockSize;
                    block.header.isAvailble = true;


                    blocksLst.insert(iter, block);
                    blocksLst.erase(iter--);
                    blocksLst.insert(iter++, newBlock);

                }
                else {
                    block.header.isAvailble = false;
                    block.dataSize = size;

                    blocksLst.insert(iter, block);
                    blocksLst.erase(iter--);
                }
                
                break;
               
            }
            nextAddr += block.header.currBlockSize;
        }
        if (nextAddr - (size_t)start_Ptr == totalSize)
            return NULL;
        return (void*)nextAddr;
    }

    size_t calculateNewBlockSize(size_t size) {
        size_t currentSize = size + sizeof(Header);
        if (currentSize % alignment == 0) 
            return currentSize;
        else 
            return currentSize + (currentSize % alignment);
        
    }


    void* mem_realloc(void* addr, size_t size) {
        if (addr == NULL)
            mem_alloc(size);

        size_t needAddr = (size_t)addr;
        size_t currAddr = (size_t)start_Ptr;


        for (auto iter = blocksLst.begin(); iter != blocksLst.end(); iter++) {
            Block block = *iter;
            if (currAddr == needAddr) {
                if (block.header.currBlockSize > calculateNewBlockSize(size)) {       
                    Block newBlock;
                    newBlock.header.isAvailble = false;
                    newBlock.header.currBlockSize = calculateNewBlockSize(size);
                    newBlock.dataSize = size;

                    block.header.currBlockSize -= newBlock.header.currBlockSize;
                    block.header.isAvailble = true;


                    blocksLst.insert(iter, block);
                    blocksLst.erase(iter--);
                    blocksLst.insert(iter++, newBlock);

                    return (void*)currAddr;

                }
                else {
                    void* newAddr = mem_alloc(size);
                    if (newAddr != NULL) {
                        mem_free(addr);
                        return newAddr;
                    }
                    else {
                        return addr;
                    }

               
                }
               
            }
            currAddr += block.header.currBlockSize;
        }
        
    }
  


    void mem_free(void* addr) {
        if (addr == NULL)
            return;

        size_t needAddr = (size_t)addr;
        size_t currAddr = (size_t)start_Ptr;

        for (auto iter = blocksLst.begin(); iter != blocksLst.end(); iter++) {
            Block block = *iter;
            if (currAddr == needAddr && !block.header.isAvailble) {
                block.header.isAvailble = true;
                block.dataSize = 0;
                blocksLst.insert(iter, block);
                blocksLst.erase(iter--);
                return;
            }
            currAddr += block.header.currBlockSize;
        }
        
    }

    void mem_dump() {
        int i = 1;
        for (Block block : blocksLst) {

            cout << "Block: " << i << " \tblock size " << block.header.currBlockSize << " \tblock data size " << block.dataSize;
            if (block.header.isAvailble)
                cout << "\t Block is available" << endl;
            else
                cout << "\t Block is NOT available" << endl;

            i++;
        }
        cout << endl;
    }


};






int main()
{
    Allocator allocator(1024);
    
    cout << "Start " << allocator.start_Ptr << endl;
    cout << "------------------Allocate-----------------" << endl;
    cout << "Size " << 50;
    cout <<"\t\tResult address " << allocator.mem_alloc(50) << endl;
    allocator.mem_dump();

   

    cout << "------------------Allocate-----------------" << endl;
    cout << "Size " << 25;
    void* addr = allocator.mem_alloc(25);
    cout <<"\t\tResult address " << addr << endl;
    allocator.mem_dump();

   

    cout << "------------------Allocate-----------------" << endl;
    cout << "Size " << 30;
    void* addr1 = allocator.mem_alloc(30);
    cout << "\t\tResult address " <<  addr1 << endl;
    allocator.mem_dump();


    cout << "------------------Reallocate-----------------" << endl;
    cout << "Address " << addr1 << "\tnew size 35" << endl;
    cout << "Result address " << allocator.mem_realloc(addr1, 35) << endl;
    allocator.mem_dump();


    cout << "------------------Deallocate-----------------" << endl;
    cout << "Address " << addr << endl;
    allocator.mem_free(addr);
    allocator.mem_dump();


    cout << "------------------Allocate-----------------" << endl;
    cout << "Size " << 20;
    void* addr2 = allocator.mem_alloc(20);
    cout << "\t\tResult address " << addr2 << endl;
    allocator.mem_dump();


    cout << "------------------Allocate-----------------" << endl;
    cout << "Size " << 5;
    cout << "\t\tResult address " << allocator.mem_alloc(5) << endl;
    allocator.mem_dump();


    cout << "------------------Reallocate-----------------" << endl;
    cout << "Address " << addr2 << "\t new size 8" << endl;
    cout << "Result address " << allocator.mem_realloc(addr2, 8) << endl;
    allocator.mem_dump();

}




