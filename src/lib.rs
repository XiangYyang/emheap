/*
 | embed Heap
 | 文件名称: lib.rs
 | 文件作用: 顶层模块
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
//! # emheap
//! 
//! The `emheap` crate is a very simple memory manager for embedded systems and microprocessors.
//! 
//! Here are the main features:
//! 
//!  1. small:  total no more than 350 lines of code.
//!  2. fragmentation collection:
//!     the programming will merge the closest memory block to avoid fragmentation.
//!  3. cross-platform: it does not rely on any libraries.
//!  4. memory alignment: support memory alignment.
//! 
//! WARNING： **DO NOT** use this library on your PC.
//!
#![cfg_attr(not(test), no_std)]

pub mod heap;
pub mod rsalloc;

#[cfg(test)]
mod tests {
    #[test]
    fn it_works() {
        let result = 2 + 2;
        assert_eq!(result, 4);
    }
}
