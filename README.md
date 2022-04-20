﻿emheap
========

The `emheap` crate is a simple memory manager for embedded systems and microprocessors.

Here are the main features:

 1. small:  total no more than 350 lines of code.
 2. fragmentation collection:
    the programming will merge the closest memory block to avoid fragmentation.
 3. cross-platform: it does not rely on any libraries.
 4. memory alignment: support memory alignment.

WARNING： **DO NOT** use this library on your PC.

## HOW TO USE

In `cargo.toml`

```toml
[dependencies]
emheap = { git = "https://github.com/XiangYyang/emheap" }
```

Then, in `heap.c`, change the heap memory size:

```c
#define HEAP_SIZE           (4 * 1024)
```

## Example

Consider a Direct Computer Control System, it uses the ARM Cortex-M0+ microprocessor. Now, we want to use the `alloc` crate.

At first, we should check out to the nightly channel:

```bash
rustup default nightly
```

Now, declare the `alloc` crate in your codes:

```rust
#![feature(alloc_error_handler)]

extern crate alloc;
```

This crate is not dependent on unstable features, however, you need to use `alloc_error_handler` to cause panic. Let's special the global allocator and the error handler:

```rust
use alloc::alloc::Layout;
use emheap::{heap, rsalloc::Allocator};

#[global_allocator]
pub static HEAP: Allocator = Allocator {};

#[alloc_error_handler]
fn alloc_error(_layout: Layout) -> ! {
    // your code...
    loop{}
}
```

Once all that is in place, now you can finally use the collections in `alloc`:

```rust
use alloc::vec;

fn test() {
    let arr = vec![1, 2, 3, 4, 5];
    for i in arr {
        do_other(i);
    }
}
```





