/*
 | embed Heap
 | 文件名称: rsalloc.rs
 | 文件作用: Rust的分配器实现
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
//! Allocator based `heap.rs` for Rust alloc crate.
use super::heap;
use core::alloc::{GlobalAlloc, Layout};
use core::{marker::Sync, panic};

pub struct Allocator {}

unsafe impl Sync for Allocator {}

unsafe impl GlobalAlloc for Allocator {
    unsafe fn alloc(&self, layout: Layout) -> *mut u8 {
        let r = heap::allocate(layout.size(), layout.align());
        match r {
            Some(p) => p,
            None => panic!("allocator out of memory"),
        }
    }

    unsafe fn dealloc(&self, ptr: *mut u8, _layout: Layout) {
        heap::free(ptr);
    }
}

