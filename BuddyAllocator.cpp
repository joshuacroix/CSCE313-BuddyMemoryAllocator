#include "BuddyAllocator.h"
#include <iostream>
using namespace std;

/* This initializes the memory allocator and makes a portion of ’_total_memory_length’ bytes available. The allocator uses a ’_basic_block_size’ as its minimal unit of allocation. The function 
returns the amount of memory made available to the allocator. */ 
BuddyAllocator::BuddyAllocator (int _basic_block_size, int _total_memory_length){
	//This fixes user putting in a non-binary basic block size.
	int block_binary_fixer = 1;
	for(int i = 1; i <= _basic_block_size, i*=2){
		block_binary_fixer = i;
	}
	basic_block_size = block_binary_fixer;

	//Now we standardize the total memory length to the basic block size, so that memory length can only be a multiple of the basic block size.
	int memory_fixer = 1;
	for(int i = basic_block_size; i <= _total_memory_length; i+=basic_block_size) {
		memory_fixer = i;
	}

	total_memory_size = memory_fixer;
	int level_finder = 0;
	for (int i = basic_block_size; i < total_memory_size; i*=2;){
		level_finder+=1;
	}
	memory_levels = level_finder;
	cout << "Your basic block size has been changed to " << basic_block_size << endl;
	cout << "Your total memory length has been changed to " << total_memory_size << endl;
	cout << "There are " << memory_levels << "levels of blocks" << endl;

	//That should handle getting all of the blocks and memory sizes compatable. I'm not really sure how I'm doing this next bit. Pay attention to this and the header variables required for this.
	
	start_pointer = new char[_total_memory_length];
	free_size = log2(total_memory_size) - log2(basic_block_size) + 1;
	FreeList.resize(free_size);
	FreeList[0].insert((BlockHeader*)start_pointer);
	FreeList[0].head->block_size = total_memory_size;
	FreeList[0].head->is_free = true;
	

}

BuddyAllocator::~BuddyAllocator (){
	delete[] start_pointer;
}

void* BuddyAllocator::alloc(int length) {
  /* This preliminary implementation simply hands the call over the 
     the C standard library! 
     Of course this needs to be replaced by your implementation.
  */

	//So I think what I would do with this one is use ceil(log(length)/log(2)), which should be the smallest block that will fit the program.

	//if first block size > needed allocation, then remove from the list and allocate, else iterate down the list and compare. When you find a block, then remove it from the free store.
  return malloc (length);
}

void BuddyAllocator::free(void* a) {
  /* Same here! */
  free (a);
}

BlockHeader* BuddyAllocator::getbuddy (BlockHeader * addr){
	// given a block address, this function returns the address of its buddy 
	

}

bool BuddyAllocator::arebuddies (BlockHeader* block1, BlockHeader* block2){
	// checks whether the two blocks are buddies are not
	// note that two adjacent blocks are not buddies when they are different sizes
	BlockHeader* buddy_check1 = getbuddy(block1);
	BlockHeader* buddy_check2 = getbuddy(block2);
	if((buddy_check1 == block2) && (buddy_check2 == block1))
		return true;
	else
		return false;

}

BlockHeader* BuddyAllocator::merge (BlockHeader* block1, BlockHeader* block2){
	// this function merges the two blocks returns the beginning address of the merged block
	// note that either block1 can be to the left of block2, or the other way around


}

BlockHeader* BuddyAllocator::split (BlockHeader* block){
	// splits the given block by putting a new header halfway through the block
	// also, the original header needs to be corrected
	BlockHeader* block2 = nullptr;
	int level = log2(block->block_size)-log2(block_size);
	FreeList[level].remove(block);
	block->block_size = block->block_size / 2;
	block2 = block;
	block2 = block2 + block1->block_size;
	block2->block_size = block1->block_size;
	FreeList[level-1].insert(block);
	FreeList[level-1].insert(block2);
	return block;
}

void BuddyAllocator::printlist (){
  cout << "Printing the Freelist in the format \"[index] (block size) : # of blocks\"" << endl;
  int64_t total_free_memory = 0;
  for (int i=0; i<FreeList.size(); i++){
    int blocksize = ((1<<i) * basic_block_size); // all blocks at this  are this size
    cout << "[" << i <<"] (" << blocksize << ") : ";  // block size at index should always be 2^i * bbs
    int count = 0;
    BlockHeader* b = FreeList [i].head;
    // go through the list from head to tail and count
    while (b){
      total_free_memory += blocksize;
      count ++;
      // block size at index should always be 2^i * bbs
      // checking to make sure that the block is not out of place
      if (b->block_size != blocksize){
        cerr << "ERROR:: Block is in a wrong list" << endl;
        exit (-1);
      }
      b = b->next;
    }
    cout << count << endl;
    cout << "Amount of available free memory: " << total_free_memory << " byes" << endl;  
  }
}

