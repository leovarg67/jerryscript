/* Copyright 2014 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define JERRY_MEM_POOL_INTERNAL

#include "globals.h"
#include "mem-allocator.h"
#include "mem-pool.h"
#include "mem-poolman.h"

extern void srand (unsigned int __seed);
extern int rand (void);
extern long int time (long int *__timer);
extern int printf (__const char *__restrict __format, ...);
extern void *memset (void *__s, int __c, size_t __n);

// Pool area size is 8K
const size_t test_pool_area_size = 8 * 1024;

// Iterations count
const uint32_t test_iters = 64;

// Subiterations count
const uint32_t test_max_sub_iters = 1024;

int
main( int __unused argc,
      char __unused **argv)
{
    srand((unsigned int) time(NULL));
    int k = rand();
    printf("seed=%d\n", k);
    srand((unsigned int) k);

    for ( uint32_t i = 0; i < test_iters; i++ )
    {
	uint8_t test_pool[test_pool_area_size] __attribute__((aligned(MEM_ALIGNMENT)));
	mem_pool_state_t* pool_p = (mem_pool_state_t*) test_pool;

	mem_pool_init( pool_p, sizeof (test_pool));

        const size_t subiters = ( (size_t) rand() % test_max_sub_iters ) + 1;
        uint8_t* ptrs[subiters];

        for ( size_t j = 0; j < subiters; j++ )
        {
            ptrs[j] = mem_pool_alloc_chunk( pool_p);

	    // TODO: Enable check with condition that j <= minimum count of chunks that fit in the pool
            // JERRY_ASSERT(ptrs[j] != NULL);

            if ( ptrs[j] != NULL )
            {
                memset(ptrs[j], 0, MEM_POOL_CHUNK_SIZE);
            }
        }

        // mem_heap_print( true);

        for ( size_t j = 0; j < subiters; j++ )
        {
            if ( ptrs[j] != NULL )
            {
                for ( size_t k = 0; k < MEM_POOL_CHUNK_SIZE; k++ )
                {
                    JERRY_ASSERT( ((uint8_t*)ptrs[j])[k] == 0 );
                }

                mem_pool_free_chunk( pool_p, ptrs[j]);
            }
        }
    }

    return 0;
} /* main */
