/*
 | embed Heap
 | 文件名称: heap.rs
 | 文件作用: C程序的包装
 | 创建日期: 2022-04-10
 | 更新日期: 2022-04-10
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
//! wrapper for C implement

/// # Allocate memory
/// 
/// Allocate memory in heap.
/// 
/// ## Arguments
/// 
///  * `size`: need size, bytes
///  * `align`: memory alignment, bytes
/// 
/// ## Return Value
/// 
/// If succeeded, this function will return the start pointer for the block,
/// and if failed, it will return `None`.
pub fn allocate(size: usize, align: usize) -> Option<*mut u8> {
    let ptr = unsafe { emheap_impl_Heap_Alloc(size, align) };
    if ptr.is_null() {
        None
    } else {
        Some(ptr)
    }
}

/// free
pub fn free(ptr: *mut u8) {
    unsafe { emheap_impl_Heap_Free(ptr) };
}

/// # Initialize heap
/// 
/// Initialize heap, it will initialize the free list in heap memory.
pub fn init() {
    unsafe { emheap_impl_Heap_Init() };
}

/// Get free memory size
pub fn get_free_size() -> usize {
    unsafe { free_size }
}

extern "C" {
    fn emheap_impl_Heap_Init();
    fn emheap_impl_Heap_Alloc(size: usize, align: usize) -> *mut u8;
    fn emheap_impl_Heap_Free(ptr: *mut u8);
    //
    static free_size: usize;
}
