/*
 | LLC
 | 文件名称: companion_heap.c
 | 文件作用: 堆
 | 创建日期: 2022-03-14
 | 更新日期: 2022-03-14
 | 开发人员: 向阳
 +----------------------------
 Copyright (C) 向阳, all rights reserved.

 MIT License

 Copyright (c) 2022 向阳
 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge,
 publish, distribute, sublicense, and/or sell copies of the Software,
 and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
*/
#include <iso646.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// 堆区大小
#define HEAP_SIZE           (4 * 1024)

// 堆区的字节对齐(4bytes)
#define HEAP_BYTE_ALIGN     4

//! 内存
static uint8_t memory[HEAP_SIZE];

/*! 内存块
*
*   @note   其内存结构为:
*
*   @note   | align_fill | MemBlock | Memory |
*
*   @note   实现细节上, 它们按照内存首地址顺序排列,
*           这样方便进行相邻块的合并。
*/
typedef struct tagMemBlock {
    //! 下一个空闲内存块的位置
    struct tagMemBlock* next;

    //! 本空闲块的大小, 包括块结构本身
    size_t size;

    //! 对齐填充
    size_t align_fill;
} MemBlock;

//! freelist的header,
//! header不放数据
static MemBlock* freelist = NULL;

//! freelist末尾
static MemBlock* plistend = NULL;

//! 当前的空闲内存块大小
//! 被 @rco/heap.rs引用
size_t free_size = 0;

static void InsertToFreeList(MemBlock* node);

/*! 初始化堆分配器
*
*   @note   对分配器进行初始化
*/
extern void emheap_impl_Heap_Init(void)
{
    intptr_t head_addr = (intptr_t)memory;
    size_t total_size = HEAP_SIZE;
    // 进行内存对齐
    head_addr = (head_addr + HEAP_BYTE_ALIGN - 1) & ~(HEAP_BYTE_ALIGN - 1);
    total_size = HEAP_SIZE - (head_addr - (intptr_t)memory);
    // 起始位置的node
    freelist = (MemBlock*)head_addr;
    freelist->size = 0;
    freelist->align_fill = 0;
    // end位置
    intptr_t end_addr = head_addr + total_size - sizeof(MemBlock);
    end_addr = (end_addr + HEAP_BYTE_ALIGN - 1) & ~(HEAP_BYTE_ALIGN - 1);
    plistend = (MemBlock*)end_addr;
    plistend->size = 0;
    plistend->next = NULL;
    plistend->align_fill = 0;
    // 第一个块
    MemBlock* pfirst = (MemBlock*)(head_addr + sizeof(MemBlock));
    pfirst->size = end_addr - head_addr - sizeof(MemBlock);
    pfirst->next = plistend;
    pfirst->align_fill = 0;
    //
    freelist->next = pfirst;
    //
    free_size = pfirst->size;
}

/*! 分配内存
*
*   @note   在堆上分配一个内存区
*
*   @param[in]  sz: 需要分配的内存大小
*   @param[in]  align: 内存对齐, 必须是8的整数倍, 单位是位
*
*   @retval     void*: 分配成功返回分配的内存地址, 否则返回NULL
*/
extern void* emheap_impl_Heap_Alloc(size_t sz, size_t align)
{
    align /= 8;
    if (align == 0)
    {
        align = 1;
    }
    size_t need_sz = sz + sizeof(MemBlock);
    size_t align_mask = align - 1;
    if (need_sz & align_mask)
    {
        need_sz = (need_sz + align - 1) & ~align_mask;
    }
    if (need_sz > free_size)
    {
        return NULL;
    }
    // 使用首次适应分配策略寻找到一个合适的块
    MemBlock* pblock = freelist->next;
    MemBlock* pprev = freelist;
    while (pblock->size < need_sz and pblock->next != NULL)
    {
        // 找到合适的块, 并保证块不是pEnd
        pprev = pblock;
        pblock = pblock->next;
    }
    // 如果找不到合适的块, 则返回NULL
    if (pblock == plistend)
    {
        return NULL;
    }
    else {
        // 移走block这个node
        pprev->next = pblock->next;
        if (pblock->size > sizeof(MemBlock) * 2)
        {
            // 可以把block变成两个新node
            intptr_t last_addr = (intptr_t)pblock;
            intptr_t next_addr = last_addr + need_sz;
            //
            MemBlock* new_node = (MemBlock*)next_addr;
            new_node->size = pblock->size - need_sz;
            new_node->next = pblock->next;
            new_node->align_fill = 0;
            //
            InsertToFreeList(new_node);
        }
        // 计算对齐后的地址
        intptr_t head_addr = (intptr_t)pblock;
        intptr_t align_addr = (head_addr + align - 1) & ~align_mask;
        intptr_t align_offset = align_addr - head_addr;
        // | align_fill | MemBlock | Memory |
        // 之前的计算确保了sizeof(Memory) + align_fill == need_sz
        MemBlock* used_block = (MemBlock*)align_addr;
        used_block->size = sz;
        used_block->next = NULL;
        used_block->align_fill = align_offset;
        //
        void* res_mem = (void*)(align_addr + sizeof(MemBlock));
        // 减去使用了的大小
        free_size -= need_sz;
        return res_mem;
    }
}

/*! 释放内存
*
*   @note   释放分配的内存区
*/
extern void emheap_impl_Heap_Free(void* ptr)
{
    MemBlock* block = (MemBlock*)((intptr_t)ptr - sizeof(MemBlock));
    //
    size_t block_sz = block->size + block->align_fill;
    intptr_t block_head = (intptr_t)ptr - block->align_fill - sizeof(MemBlock);
    // 构造正确的node
    MemBlock* new_node = (MemBlock*)block_head;
    new_node->size = block_sz + sizeof(MemBlock);
    new_node->next = NULL;
    new_node->align_fill = 0;
    //
    free_size += new_node->size;
    // 插入到free list
    InsertToFreeList(new_node);
}

/*! 插入node到free list中
*
*/
static void InsertToFreeList(MemBlock* node)
{
    // 找到一个位置, 使得it->next的地址大于node
    MemBlock* it = freelist;
    for (; it->next < node; it = it->next);
    MemBlock* insert_pos = it;
    MemBlock* be_inserted = node;
    // 检查it后面是不是刚好是node
    intptr_t insert_beg = (intptr_t)insert_pos;
    intptr_t node_addr = (intptr_t)be_inserted;
    if (insert_beg + (intptr_t)insert_pos->size == node_addr)
    {
        // 这时候说明, insert_pos后面是node, 合并它们,
        // 以此来减少内存碎片
        insert_pos->size += it->next->size;
        be_inserted = insert_pos;
    }
    // 检查it->next前面是不是刚好是node
    intptr_t next_beg = (intptr_t)it->next;
    node_addr = (intptr_t)be_inserted;
    if (node_addr + (intptr_t)be_inserted->size == next_beg)
    {
        // 这时候说明, it->next前面是node, 合并它们,
        // 以此来减少内存碎片
        if (it->next != plistend)
        {
            // it后面不是end, 可以合并
            be_inserted->size += it->next->size;
            be_inserted->next = it->next->next;
        }
        else {
            // it后面是end, 不能合并
            be_inserted->next = plistend;
        }
    }
    else {
        // 没有连起来, 把be_inserted后面接上it->next
        be_inserted->next = it->next;
    }
    // 如果be_inserted和it没有合并, 还需要在it后面接上be_inserted
    if (be_inserted != it)
    {
        it->next = be_inserted;
    }
}
