#ifndef _BETTER_LOCKED_HASH_TABLE_H_
#define _BETTER_LOCKED_HASH_TABLE_H_


#include <iostream>
#include <mutex>
#include <thread>
#include "hash_table.h"
#include "bucket.h"

class better_locked_probing_hash_table : public hash_table {

  private:
    Bucket* table;
    const int TABLE_SIZE; //we do not consider resizing. Thus the table has to be larger than the max num items.
    std::mutex global_mutex;
        int sub_tables; // dividing the Bucket* table into smaller tables each with locks
        int items_sub_tables; // numer of items that are under one smaller table
        std::mutex *locks; // locks holder for the sub tables.
        int num_threaads = 16;
        int items_per_table = 625000; // TODO: change this later
    public:

    better_locked_probing_hash_table(int table_size):TABLE_SIZE(table_size){
      this->table = new Bucket[TABLE_SIZE]();
      for(int i=0;i<TABLE_SIZE;i++) {
        this->table[i].valid=0; //means empty
      }
        
      // I should'ved remove the initialization code.
      // thre must be some delays because of this
        /* ADDITIONAL INITIALIZATION CODE */
        this->items_sub_tables = TABLE_SIZE / 16; // tables divided by 16 threads....
        this->sub_tables = TABLE_SIZE / items_sub_tables;

        // LOCKS TABLE
        this->locks = new std::mutex[16]; // 16 locks

    }

    // THE ACTUAL READING FUNCTION
    bool read_(uint32_t key, uint64_t* value_buffer,uint64_t index){
        // returns 1 if successfully read | 0 if needs to change index
        int lock_index = index % 16;

        std::lock_guard<std::mutex> lock(locks[lock_index]);
        if(table[index].key == key) {
                *value_buffer = table[index].value;
                return true;
        } else {
                return false; // needs to increase probe count, give next index
        }
    }

    // THE ACTUAL INSERTING FUNCTION
    bool insert_(uint32_t key, uint64_t value, uint64_t index) {
        int lock_index = index % 16;

        std::lock_guard<std::mutex> lock(locks[lock_index]);
        if (table[index].key == key || table[index].valid == false) {
                table[index].valid = true;
                table[index].key = key;
                table[index].value = value;
                return true;
        } else {
                return false;
        }
    }

    virtual uint32_t hash(uint32_t x)
    {
      //https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
      x = ((x >> 16) ^ x) * 0x45d9f3b;
      x = ((x >> 16) ^ x) * 0x45d9f3b;
      x = (x >> 16) ^ x;
      return (x % TABLE_SIZE);
    }

    virtual uint32_t hash_next(uint32_t key, uint32_t prev_index)
    {
      //linear probing. no special secondary hashfunction
      return ((prev_index + 1)% TABLE_SIZE);
    }

    //the buffer has to be allocated by the caller
    // read() and write() functions are buffer for independent locks
    // for each segment of the HASH TABLE.

        // stripping
        bool read(uint32_t key, uint64_t* value_buffer) {
                uint64_t index = this->hash(key);
                int probe_count = 0;
                int success = 0;
                success = read_(key, value_buffer, index); // first check
                // check subsequent buckets.
                while(success == 0 && table[index].valid == true) {
                        probe_count++;
                        index = this->hash_next(key, index);
                        if (probe_count >= TABLE_SIZE) break;
                        success = read_(key, value_buffer, index);
                }
                if (success)
                        return true;
                else
                        return false;
        }

        bool insert(uint32_t key, uint64_t value) {
                uint64_t index = this->hash(key);
                int probe_count = 0;
                int success = 0;
                success = insert_(key, value, index);
                while(success == 0) {
                        probe_count++;
                        index = this->hash_next(key, index);
                        if (probe_count >= TABLE_SIZE) break;
                        success = insert_(key, value, index);
                }
                if (success) return true;
                else return false;
        }


    int num_items() {
      int count=0;
      for(int i=0;i<TABLE_SIZE;i++) {
        if(table[i].valid==true) count++;
      }
      return count;
    }
};

#endif
