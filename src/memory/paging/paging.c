#include "paging.h"
#include "memory/heap/kheap.h"
#include "status.h"

void paging_load_directory(uint32_t* directory);

static uint32_t* current_directory = 0;

struct paging_4gb_chunk* paging_new_4gb(uint8_t flags)
{
    uint32_t* directory = kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE);

    int offset = 0;

    for (int i = 0; i < PAGING_TOTAL_ENTRIES_PER_TABLE; i++)
    {
        uint32_t* entry = kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE);

        for (int k = 0; k < PAGING_TOTAL_ENTRIES_PER_TABLE; k++)
        {
            entry[k] = (offset + (k * PAGING_PAGE_SIZE)) | flags;
        }

        offset += (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE);

        directory[i] = (uint32_t)entry | flags | PAGING_IS_WRITEABLE;
    }

    struct paging_4gb_chunk* chunk_4gb = kzalloc(sizeof(struct paging_4gb_chunk));

    chunk_4gb->directory_entry = directory;

    return chunk_4gb;
}

void paging_switch(struct paging_4gb_chunk* directory)
{
    paging_load_directory(directory->directory_entry);

    current_directory = directory->directory_entry;
}

void paging_free_4gb(struct paging_4gb_chunk* chunk)
{
    for (int i = 0; i < 1024; i++)
    {
        uint32_t entry = chunk->directory_entry[i];
        uint32_t* table = (uint32_t*)(entry & 0xfffff000);
        kfree(table);
    }

    kfree(chunk->directory_entry);
    kfree(chunk);
}

uint32_t* paging_4gb_chunk_get_directory(struct paging_4gb_chunk* chunk)
{
    return chunk->directory_entry;
}

bool paging_is_aligned(void* address)
{
    return ((uint32_t)address % PAGING_PAGE_SIZE) == 0;
}

int paging_get_indexes(void* virtual_address, uint32_t* directory_index_out, uint32_t* table_index_out)
{
    int res = 0;
    if (!paging_is_aligned(virtual_address))
    {
        res = -EINVARG;
        goto out;
    }

    *directory_index_out = ((uint32_t)virtual_address / (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE));
    *table_index_out = ((uint32_t)virtual_address % (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE) / PAGING_PAGE_SIZE);

out:
    return res;
}

void* paging_align_address(void* ptr)
{
    if ((uint32_t)ptr % PAGING_PAGE_SIZE)
    {
        return (void*)((uint32_t)ptr + PAGING_PAGE_SIZE - ((uint32_t)ptr % PAGING_PAGE_SIZE));
    }

    return ptr;
}

int paging_map(struct paging_4gb_chunk* directory, void* virtual_address, void* physical_address, int flags)
{
    if (((unsigned int)virtual_address % PAGING_PAGE_SIZE) || ((unsigned int)physical_address % PAGING_PAGE_SIZE))
    {
        return -EINVARG;
    }

    return paging_set(directory->directory_entry, virtual_address, (uint32_t)physical_address | flags);
}

int paging_map_range(struct paging_4gb_chunk* directory, void* virtual_address, void* physical_address, int count, int flags)
{
    int res = 0;

    for (int i = 0; i < count; i++)
    {
        res = paging_map(directory, virtual_address, physical_address, flags);
        if (res == 0)
        {
            break;
        }

        virtual_address += PAGING_PAGE_SIZE;
        physical_address += PAGING_PAGE_SIZE;
    }

    return res;
}

int paging_map_to(struct paging_4gb_chunk* directory, void* virtual_address, void* physical_address, void* physical_end_address, int flags)
{
    int res = 0;

    if ((uint32_t)virtual_address % PAGING_PAGE_SIZE)
    {
        res = -EINVARG;
        goto out;
    }

    if ((uint32_t)physical_address % PAGING_PAGE_SIZE)
    {
        res = -EINVARG;
        goto out;
    }

    if ((uint32_t)physical_end_address % PAGING_PAGE_SIZE)
    {
        res = -EINVARG;
        goto out;
    }

    if ((uint32_t)physical_end_address < (uint32_t)physical_address)
    {
        res = -EINVARG;
        goto out;
    }

    uint32_t total_bytes = physical_end_address - physical_address;
    int total_pages = total_bytes / PAGING_PAGE_SIZE;

    res = paging_map_range(directory, virtual_address, physical_address, total_pages, flags);

out:
    return res;
}

int paging_set(uint32_t* directory, void* virtual_address, uint32_t value)
{
    int res = 0;
    if (!paging_is_aligned(virtual_address))
    {
        res = -EINVARG;
        goto out;
    }

    uint32_t directory_index = 0;
    uint32_t table_index = 0;

    res = paging_get_indexes(virtual_address, &directory_index, &table_index);

    if (res < 0)
    {
        goto out;
    }

    uint32_t entry = directory[directory_index];
    uint32_t* table = (uint32_t*)(entry & 0xfffff000);
    table[table_index] = value;

out:
    return res;
}