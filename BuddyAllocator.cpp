#include "BuddyAllocator.h"
#include <iostream>
#include <vector>
using namespace std;

BuddyAllocator::BuddyAllocator (int _basic_block_size, int _total_memory_length){
        //This portion is for fixing non-binary input
        int binary_block_fixer;
        for(int i = 1; i < _basic_block_size; i*=2) {
                binary_block_fixer = i;
        }
        binary_block_fixer *= 2;
        basic_block_size = binary_block_fixer;

        int totmem_fixer;
        level_index = 0;
        for(int i = basic_block_size; i < _total_memory_length; i *= 2) {
                totmem_fixer = i;
                level_index += 1;
        }
        totmem_fixer *= 2;
        total_memory_size = totmem_fixer;

        cout << "New Basic Block Size: " << basic_block_size << endl;
        cout << "New Total Memory Length: " << total_memory_size << endl;
        cout << "Level Index Generated: 0->" << level_index << endl;

        //This is going to set up the first block header on the free list
        base = new char[total_memory_size];
        BlockHeader* base_head = reinterpret_cast<BlockHeader*>(base);
        base_head->block_size = total_memory_size;
	base_head->next = nullptr;
        base_head->is_free = true;
        FreeList.resize(level_index+1);//level index does not match (resize)
        FreeList[level_index].insert(base_head);//
}

BuddyAllocator::~BuddyAllocator (){
        delete[] base;

}

void* BuddyAllocator::alloc(int _length) {
  /* This preliminary implementation simply hands the call over the
     the C standard library!
     Of course this needs to be replaced by your implementation.
  */
        int real_size = _length + sizeof(BlockHeader);
        int find_index = 0;
	int block_size_holder = basic_block_size;
        for (int i = basic_block_size; i < real_size; i*=2){
                find_index += 1;
		
        }
        cout << "The find index is " << find_index << endl;
        cout << "The level index is " << level_index << endl;

        //Now that we know which level of free list the needed memory is, we work our way up the list.
        int hold_index = find_index; //This is just to use for removal later
        LinkedList *list = nullptr;
        int split_counter = 0;
        BlockHeader* return_ptr = nullptr;
	
        while ((find_index<=level_index)&&(list==nullptr)){ //Finds a non empty list
                if(FreeList[find_index].get_size()!=0){
                        list = &(FreeList[find_index]);
                        cout << "address found: " << &(FreeList[find_index]) << endl;
                        //cout << "IF LOOP" << endl;
                }
                else
                {
                        find_index += 1;
                        split_counter += 1;
                        //cout << "ELSE" << endl;
                }
                //cout << "While loop " << endl;
        }
	
	for (int i = 0; i < find_index; i++) {
		block_size_holder *= 2;
	}

        cout << "split counter = " << split_counter << endl;
        if (list == nullptr){
                cout << "RETURN NULL / No memory left" << endl;
                return nullptr;
        }
	//This just removes the found memory from FL
	return_ptr = list->head; 
	list->head = return_ptr->next;
	list->size -= 1;
	
	return_ptr->block_size = block_size_holder;
	cout << "return ptr set " << endl;
	cout << "return_ptr address: " << return_ptr << endl;
	cout << "return_ptr: " << return_ptr->block_size << endl;
        //Splits as necessary
        while(split_counter != 0) {
                return_ptr = split(return_ptr);
                split_counter -= 1;
		
        }
        //Takes the block to be used
        return_ptr->is_free = false;
	cout << "return ptr to be removed: " << return_ptr << endl;
	cout << "rtn ptr size to removed: " << return_ptr->block_size << endl;
        FreeList[hold_index].remove(return_ptr);
        //return malloc (_length); <----- (original given code)
	cout <<"Made it to the end of alloc" << endl;
        return return_ptr;
}

void BuddyAllocator::free(void* a) {
  /* Same here! */
  //::free (a);
}

void BuddyAllocator::printlist (){
  cout << "Printing the Freelist in the format \"[index] (block size) : # of blocks\"" << endl;
  int64_t total_free_memory = 0;
  for (int i=0; i<FreeList.size()+1; i++){
    int blocksize = ((1<<i) * basic_block_size); // all blocks at this level are this size
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
    cout << "Amount of available free memory: " << total_free_memory << " bytes" << endl;
  }
}
