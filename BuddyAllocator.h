#ifndef _BuddyAllocator_h_                   // include file only once
#define _BuddyAllocator_h_
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
typedef unsigned int uint;

/* declare types as you need */

class BlockHeader{
public:
        // think about what else should be included as member variables
        int block_size;  // size of the block
        BlockHeader* next; // pointer to the next block
        bool is_free = false;
};

class LinkedList{
        // this is a special linked list that is made out of BlockHeader s.
public:
        BlockHeader* head;              // you need a head of the list
        int size = 0;
public:

        int get_size() {
                return size;
        }

        BlockHeader *get_head() {
                return head;
        }

        void insert (BlockHeader* b){   // adds a block to the list
                cout << "Insert called" << endl;
                b->is_free = true;
                BlockHeader* holder = nullptr;
                holder = head;
                head = b;
                head->next = holder;
                size +=1;
		cout << "Inserted: " << holder << endl;
        }

        void remove (BlockHeader* b){  // removes a block from the list
                BlockHeader* curr = head;
                if(head == b) {
                        if(curr->next != nullptr){
				head = curr->next;			
				size -= 1;
			}
			else {
			head = nullptr;
			size = 0;
			}
                }
                else{
			for (int i = 1; i < size; i++) {
				if (curr->next == b) {
					if (curr->next->next != nullptr){
						curr->next = curr->next->next;
						size -= 1;
					}
					else {
						curr->next = nullptr;
						size -= 1;
					}
				}
				else {
					curr = curr->next;
				}
			}
                }
                return;
        }
};


class BuddyAllocator{
private:
        /* declare more member variables as necessary */
        vector<LinkedList> FreeList;
        int basic_block_size;
        int total_memory_size;
        int level_index;
        char* base = nullptr;

private:
        /* private function you are required to implement
         this will allow you and us to do unit test */

        BlockHeader* getbuddy (BlockHeader * addr);
        // given a block address, this function returns the address of its buddy

        bool arebuddies (BlockHeader* block1, BlockHeader* block2);
        // checks whether the two blocks are buddies are not
        // note that two adjacent blocks are not buddies when they are different sizes

        BlockHeader* merge (BlockHeader* block1, BlockHeader* block2);
        // this function merges the two blocks returns the beginning address of the merged block
        // note that either block1 can be to the left of block2, or the other way around

        BlockHeader* split (BlockHeader* block){
        // splits the given block by putting a new header halfway through the block also, the original header needs to be corrected
                cout << "Split called" << endl;
		cout << "Block Address: " << block << endl;
		cout << "Block size: " << block->block_size << endl;
                int temp_level = 0; //This should be the level the block is at
                for (int i = basic_block_size; i < block->block_size; i *=2 ){
                        temp_level += 1;
                }
		cout << "Temp level: " << temp_level << endl;
                char* c_block2 = nullptr;
		c_block2 = (char*) block;
                if (temp_level != 0){
			cout << "Removing block at start of split" << endl;
                        FreeList[temp_level].remove(block);
                        block->block_size = block->block_size/2;
                        c_block2 = c_block2 + block->block_size;
			BlockHeader* block2 = (BlockHeader*) c_block2; 
			block2->block_size = block->block_size; 
			cout << "Block size: " << block->block_size << endl;
			cout << "Block2 size: " << block2->block_size << endl;
			cout << "Block: " << block << endl;
			cout << "Block2: " << block2 << endl;
                        FreeList[temp_level-1].insert(block); //not working
                        FreeList[temp_level-1].insert(block2); //passing in block 1 for some reason
                }
                else {
                cout << "You can't split this block. It is too small" << endl;
                }
		cout << "Made it to the end of split" << endl;
                return block;
        }

public:
        BuddyAllocator (int _basic_block_size, int _total_memory_length);
        /* This initializes the memory allocator and makes a portion of
           ’_total_memory_length’ bytes available. The allocator uses a ’_basic_block_size’ as
           its minimal unit of allocation. The function returns the amount of
           memory made available to the allocator.
        */

        ~BuddyAllocator();
        /* Destructor that returns any allocated memory back to the operating system.
           There should not be any memory leakage (i.e., memory staying allocated).
        */

        void* alloc(int _length);
        /* Allocate _length number of bytes of free memory and returns the
                address of the allocated portion. Returns 0 when out of memory. */

        void free(void* _a);
        /* Frees the section of physical memory previously allocated
           using alloc(). */

        void printlist ();
        /* Mainly used for debugging purposes and running short test cases */
        /* This function prints how many free blocks of each size belong to the allocator
        at that point. It also prints the total amount of free memory available just by summing
        up all these blocks.
        Aassuming basic block size = 128 bytes):

        [0] (128): 5
        [1] (256): 0
        [2] (512): 3
        [3] (1024): 0
        ....
        ....
         which means that at this point, the allocator has 5 128 byte blocks, 3 512 byte blocks and so on.*/
};

#endif
