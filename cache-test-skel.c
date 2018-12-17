/*
David Roster
ECE 154A - Fall 2018
Lab 06 - Caches
Due: 12/7/12, 11:00 pm

Mystery Cache Geometries:
//THIS ONES SUPER SLOW
mystery0:
    block size = 64 bytes
    cache size = 4194304 bytes
    associativity = 16

//THIS ONE IS FAST
mystery1:
    block size = 4 bytes
    cache size = 4096 bytes
    associativity = 1
  
//THIS ONE IS FAST 
mystery2:
    block size = 32 bytes
    cache size = 4128 bytes
    associativity = 128
*/

//CASH SIZE ON MYSTERY 2 GIVES WRONG VALUE WITH THIS IMPLEMENTATION
//I calculated right values earlier but my implementation was taking over a minute
//When i tried to optimize the calculation of blocksize, it received this as only error
//you win some, you lose some. Can fix with some hacky stuff but too tired

//^^^^^^^^^^^^^^^^^^^^^^^^^
//Fixed it by taking out *4  and fixing it by taking set_size and incremented it by multiplying
//and then the return value by subtracting set_size from it

#include <stdlib.h>
#include <stdio.h>

#include "mystery-cache.h"



/* 
   Returns the size (in B) of the cache
*/
int get_cache_size(int block_size) {
  /* YOUR CODE GOES HERE */

  //This is my size
  int temp = 0;
  //block_size = block_size * 4;
  //checks the validity of the cache
  int set_size = block_size;
  //block_size = block_size * 4;
  //clearing set_size
  flush_cache();
  //accessing zero value in set_size to see if its a hit
  access_cache(0);
  while (access_cache(0)) {
    temp = block_size;

    //THIS WAY ALMOST WORKS
    /*
      for (int i =0; i < temp; i++)
      {
        access_cache(0);
        temp * 4;
        //did 0 get replaced??
        if (access_cache(0))
        {
          temp++;
          flush_cache();
        }
      }

    */

//Each cache way size is (in bytes) : 4(bytes per word)*L(line size)*S(sets)

    while (temp <= set_size) {
      temp = temp + block_size;
      //access cache at our temp address
      access_cache(temp);
    }
    //add our block size to our current set size

    //set_size = set_size + block_size;
    set_size = set_size*2;
  
  }
//return temp which stands for our cache size
  return temp - block_size;
}




/*
   Returns the associativity of the cache
*/
int get_cache_assoc(int size) {
  /* YOUR CODE GOES HERE */
  //S = B/N  (Sets = Blocks / Associativity)


  int cache_check = 1;
  //int temp_size = 0;
  int incrementor = 0; //cycles through addresses
  int associativity = 0; //starts at 0 and allows for each address to be tested, this is return value
  //int temp = 0; //tests aqssociativity by finding each max cache size

  flush_cache(); //clears all words in cache
  access_cache(0); //Pushes 0 address to cache

  while (access_cache(0)) {

    //makes our incrementor equal to the block size
    incrementor = size;
    //associativity starts at zero until we add to it in the next while loop
    associativity = 0;
    while (incrementor <= cache_check) {
      incrementor = incrementor + size;
      associativity++;
      //access cache at incrementor address
      access_cache(incrementor);
    }
    cache_check = cache_check + size;
  }

//return our correct associativity value
  return associativity;
}





/*
   Returns the size (in B) of each block in the cache.
*/
int get_block_size() {
  /* YOUR CODE GOES HERE */
//B = C/b (Blocks = capacity / block size)

  // int cache_incrementor = 0;
  // int cache_size = 0;
  // flush_cache();

  // do {
  //     cache_size = cache_size + 1;
  //     cache_incrementor = cache_incrementor + 1;
  // }
  // while (access_cache(cache_incrementor) == TRUE ); 


  // return block_size;
 // int incrementor = 0;
 //initializer
  int block_size = 0;
  //got error when i didnt have this here
  access_cache(0); //adds address 0 to cache

  //Adds to block size until you get a miss
  while (access_cache(block_size)) {
    //increment block size
    block_size++;
  }

  //return block size
  return block_size;
}





int main(void) {
  int size;
  int assoc;
  int block_size;
  
  /* The cache needs to be initialized, but the parameters will be
     ignored by the mystery caches, as they are hard coded.
     You can test your geometry paramter discovery routines by 
     calling cache_init() w/ your own size and block size values. */


//Using Print statements to see which method is the slowest
//Conclusion: Get_cache_size is the slowest BY FAR

 // printf("Start Run time of init: \n");
  cache_init(0,0);
//  printf("End Run time of init: \n");
  
  //Use get method to return block size
//  printf("Start Run time of block_size: \n");
  block_size = get_block_size();
//  printf("End Run time of block_size: \n");

  //Calculates cache_size by plugging in our block size and iterating through cache
//  printf("Start Run time of cache_size: \n");
  size = get_cache_size(block_size);
//  printf("End Run time of cache_size: \n");

  //Gets associativity by using our cache_size
//  printf("Start Run time of cache_assoc: \n");
  assoc = get_cache_assoc(size);
//  printf("End Run time of cache_assoc: \n");

   //Prints out our desired values
  printf("Cache size: %d bytes\n",size);
  printf("Cache associativity: %d\n",assoc);
  printf("Cache block size: %d bytes\n",block_size);
  
  return EXIT_SUCCESS;
}
