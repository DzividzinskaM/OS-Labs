# Lab 1: Custom allocator

## **Data structure**
<br>
Custom allocator consist of blocks. Every block has

- header
- data

Header occupies 5 byte, where first byte is availability of current block
and other for size of current block.

<br>

## **Implementation**
<br>

Custom allocator implement next **functions**:

- allocate memory
- reallocate memory
- free memory

<br>


#### **Initial allocator state**
<br>

Firstly allocate the specified amount of memory and create only first block
and place pointer on start of memory area - *startPtr*. Set *isAvailable* - true,
and *dataSize* - *(amount memory - size of header)*
<br>
<br>

*Picture 1. Initial allocator state*
<br>


![initial State](./img/initialState.jpg)
<br>
<br>

#### **Allocate memory**
<br>


Implementation this function in program -  __void* mem_alloc(size_t size)__, where *size* - 
size - which needed. This function will *return address* if there is free block with needed 
size or will *return NULL* if there isn't free blocks.

When we allocate memory we find first free block and check that this block size is more than
size, which need for user. If block size is less than we need we go to next block and find
suitable block.

When we find block, we divide this block by 2 blocks
<br>

- First block we notice that it is not available - *isAvailable = false* and 
using function for calculating dataSize with adjustment on 4 bytes, and set this value 
*dataSize*.

- Second block we notice that it is available - *isAvailable =true* and *dataSize* set 
value initial block size subtract need size
<br>
<br>

*Picture 2. Allocate memory*
<br>


![mem_alloc](./img/mem_alloc.jpg)
<br>
<br>


#### **Free memory**

Implementation this function in program - __void mem_free(void* addr)__, where *addr* - is a 
pointer, which note on start of need block.

Firstly, we find need block.
<br>
<br>

_Picture 3. Find needed block by addr_
![find_needed_block](./img/find_need_block.jpg)
<br>
<br>


If we find it, we will notice this block available (*isAvailable = true*), else exit.
<br>
<br>

*Picture 4. Free block*
<br>


![free_block](./img/free_block.jpg)
<br>
<br>


Then we check blocks, is there are some free block **_one by one_** we merge these blocks
<br>
<br>


*Picture 5.Sequence of free blocks*
<br>


![sequence_free_blocks](./img/sequence_free_blocks.jpg)
<br>
<br>


*Picture 6.Merging free blocks*
<br>


![merging_free_blocks](./img/merging_free_blocks.jpg)
<br>
<br>


#### **Reallocate memory**
Implementation this function in program - __void* mem_realloc(void* addr, size_t size)__, 
where *addr* - is a pointer, which note on start of need block and *size* - is a new size for 
block. This function *return start pointer of block* with changed size.

Firstly we find need block, secondly: 
* if size current block is less then new size, we will create new block, using function mem_alloc.
   
   - if mem_alloc return null we will leave old block
   - else we will free old block with function free_block

* else we divide current block by 2 blocks, where one with new size, and second block is available 
with rest size

At last we check sequence of free blocks.

**_For example_** change block with address *14535902DED*(start state such as on Picture 3)
<br>
<br>

*Picture 7. Reallocate bigger size*
![reallocate_bigger_size](./img/reallocate_bigger_size.jpg)
<br>
<br>


*Picture 8. Reallocate smaller size*
![reallocate_smaller_size](./img/reallocate_smaller_size.jpg)
<br>
<br>


## Examples


*Picture 9.Allocate memory*


![allocate](./img/allocation.jpg)



*Picture 10. Reallocate memory*


![reallocate](./img/reallocation.jpg)


Picture 11. Free memory


![free](./img/free.jpg)

