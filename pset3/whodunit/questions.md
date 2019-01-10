# Questions

## What's `stdint.h`?

Defines integer types where information about how much bytes of memory they will take up is know.
This enhances the portability of code across different hardware.

Eg you can define an `int8_t` or a `uint64_t` which will always take up 8 and 64 bytes respectively. There are other int types that will take up a minumium of bytes.

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

Specifies the exact amount of memory the program will need for the values stored in them and so will make the code more portable. Different processors may store the values in different ways otherwise which could cause unintended behaviours.

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

1, 4, 4, 2 respectively from the type definitions:

```
typedef uint8_t  BYTE;
typedef uint32_t DWORD;
typedef int32_t  LONG;
typedef uint16_t WORD;
```

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

BM in ASCII, 16973 in decimal, 01000010 01001101 in binary or 0x424D in hexadecimal.

## What's the difference between `bfSize` and `biSize`?

`bfSize` is the size of the whole bitmap file, `biSize` is the size of `BITMAPINFOHEADER` (one of the two headers).

## What does it mean if `biHeight` is negative?

Image starts from top left, as opposed to the usual bottom up way where the pixels start from the bottom right.

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

`biBitCount`

## Why might `fopen` return `NULL` in lines 24 and 32 of `copy.c`?

If it failed to open the given file name.

## Why is the third argument to `fread` always `1` in our code? (For example, see lines 40, 44, and 75.)

Because we're always reading 1 of the second argument (a header, pixel etc) at a time.

## What value does line 63 of `copy.c` assign to `padding` if `bi.biWidth` is `3`?

Three bytes of zeros will be added on the end of the row (eg 0x00 0x00 0x00)

## What does `fseek` do?

Using `fseek(inptr, padding, SEEK_CUR);` as an example it will move the file position of `inptr` from the current position, `SEEK_CUR`, forward by a number of bytes specified by `padding`.

## What is `SEEK_CUR`?

The current position of the file pointer in the passed file.
