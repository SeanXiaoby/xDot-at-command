#include "CmdFreeMemory.h"
#include <algorithm>

CmdFreeMemory::CmdFreeMemory() :
        Command("Free Memory", "AT+MEM",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Show amount of free RAM available",
#else
    "",
#endif
    "")
{
    _queryable = true;
}

uint32_t CmdFreeMemory::action(const std::vector<std::string>& args)
{
    // In order to get free mem within RTOS
    // we need to get the main thread's stack pointer
    // and subtract it with the top of the heap
    // ------+-------------------+   Last Address of RAM (INITIAL_SP)
    //       | Scheduler Stack   |
    //       +-------------------+
    //       | Main Thread Stack |
    //       |         |         |
    //       |         v         |
    //       +-------------------+ <- bottom_of_stack/__get_MSP()
    // RAM   |                   |
    //       |  Available RAM    |
    //       |                   |
    //       +-------------------+ <- top_of_heap
    //       |         ^         |
    //       |         |         |
    //       |       Heap        |
    //       +-------------------+ <- __end__ / HEAP_START (linker defined var)
    //       | ZI                |
    //       +-------------------+
    //       | ZI: Shell Stack   |
    //       +-------------------+
    //       | ZI: Idle Stack    |
    //       +-------------------+
    //       | ZI: Timer Stack   |
    //       +-------------------+
    //       | RW                |
    // ------+===================+  First Address of RAM
    //       |                   |
    // Flash |                   |
    //

    uint32_t bottom_of_stack = __get_MSP();
    char* top_of_heap =  (char *) malloc(sizeof(char));
    uint32_t diff = bottom_of_stack - (uint32_t) top_of_heap;

    free((void *) top_of_heap);

    CommandTerminal::Serial()->writef("%lu bytes\r\n", diff);

    return 0;
}

bool CmdFreeMemory::verify(const std::vector<std::string>& args)
{
    if (args.size() == 1)
        return true;

    return false;
}
