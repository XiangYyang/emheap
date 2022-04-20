# emheap

The `emheap` crate is a very simple memory manager for embedded system and microprocessor.

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

