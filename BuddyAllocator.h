#ifndef _BuddyAllocator_h_                   // include file only once
#define _BuddyAllocator_h_
#include <iostream>
#include <vector>
using namespace std;
typedef unsigned int uint;

/* declare types as you need */

class BlockHeader{
public:
	// think about what else should be included as member variables
	int block_size;  // size of the block
	BlockHeader* next; // pointer to the next block
	bool is_free; //Boolean to see if the memory is free
};

class LinkedList{
	// this is a special linked list that is made out of BlockHeader s. 
private:
	BlockHeader* head = nullptr;		// you need a head of the list
	int size = 0;
public:
	void insert (BlockHeader* b){	// adds a block to the list
		if(size <= 0){    // Empty case
			head = b;
			size = 1;
			}
		else { //Blockheader becomes new block
			BlockHeader* old_head = nullptr;
			old_head = head;
			head = b;
			head->next = old_head;
			size+=1;
			}

	}

	void remove (BlockHeader* b){  // removes a block from the list
		BlockHeader* curr = head;
		//If you're removing the head and there is a next
		if((curr == b) && (curr->next != nullptr)){
			head = curr->next;
			size-=1;
		}
		//If you're removing the head and there isn't a next
		else if((curr == b) && (curr->next == nullptr)){
			head = nullptr;
			size =0;
		}
		//If you aren't removing the head and need to find it
		else{
			for(int i = 1; i < size; i++) {
				//skips over the b in the chain, removing it
				if ((curr->next == b) && (curr->next->next != nullptr)){
					curr->next = curr->next->next;
					size-=1;
					//Might want to put in an early break from the for loop to increase speed.
				}
				else if((curr->next == b && (curr->next->next == nullptr){
					curr->next == nullptr;
					size-=1;
				}
				else {
					curr = curr->next;
				}
			}
		}

	}
//Consider Blockheader* get_head() {return head;}
//Consider int get_size(){return size;}
//Consider ~LinkedList(){Stuff} to get rid of all the blockheader* nodes

};


class BuddyAllocator{
private:
	/* declare more member variables as necessary */
	vector<LinkedList> FreeList;
	int basic_block_size;
	int total_memory_size;
	//mine below
	int memory_levels;	
	int free_size;
	char start_point;

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

	BlockHeader* split (BlockHeader* block);
	// splits the given block by putting a new header halfway through the block
	// also, the original header needs to be corrected


public:
	BuddyAllocator (int _basic_block_size, int _total_memory_length); 
	

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
