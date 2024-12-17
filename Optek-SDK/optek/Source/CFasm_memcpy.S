
/* void *CFasm_memcpy (void *dst, const void *src, size_t len)
   
   The algorithm is as follows:

   If the destination is unaligned, align it by conditionally
   copying 1- and/or 2-byte pieces.

   If the source is aligned, copy 16 bytes with a loop, and then finish up
   with 8, 4, 2, and 1-byte copies conditional on the length.

   Else (if source is unaligned), do the same, but use SRC to align the
   source data.

   This code tries to use fall-through branches for the common
   case of aligned source and destination and multiple of 4 (or 8) length.
*/

/* Byte by byte copy.  */

	.text
	.align	4
	.global CFasm_memcpy
	.type CFasm_memcpy, @function
CFasm_memcpy:
    /* a0 = dst, a1 = src, a2 = len */
    mv a5, a0        # save destination address in a5
    andi a6, a5, 3   # check if destination is aligned to 4 bytes
    bnez a6, .Ldst1mod2 # if not aligned, handle unaligned destination

.Ldstaligned:
    srli a7, a2, 4   # number of 16-byte chunks
    beqz a7, .Lsrcunaligned # if length is less than 16 bytes, handle unaligned src

    # Process 16-byte chunks
1:
    lw a6, 0(a1)     # load 4 bytes from src
    lw a7, 4(a1)     # load next 4 bytes from src
    lw a8, 8(a1)     # load next 4 bytes from src
    lw a9, 12(a1)    # load next 4 bytes from src
    sw a6, 0(a5)     # store 4 bytes to dst
    sw a7, 4(a5)     # store 4 bytes to dst
    sw a8, 8(a5)     # store 4 bytes to dst
    sw a9, 12(a5)    # store 4 bytes to dst
    addi a1, a1, 16  # increment src by 16 bytes
    addi a5, a5, 16  # increment dst by 16 bytes
    addi a7, a7, -1  # decrement 16-byte chunk count
    bnez a7, 1b      # if chunks remaining, continue

    # Process any leftover pieces smaller than 16 bytes
.Lsrcunaligned:
    andi a6, a1, 3   # check if src is aligned to 4 bytes
    bnez a6, .Lsrcunaligned_bytecopy # if not aligned, handle bytecopy

    # Copy remaining 8 bytes
2:
    lw a6, 0(a1)     # load 4 bytes from src
    lw a7, 4(a1)     # load next 4 bytes from src
    sw a6, 0(a5)     # store 4 bytes to dst
    sw a7, 4(a5)     # store 4 bytes to dst
    addi a1, a1, 8   # increment src by 8 bytes
    addi a5, a5, 8   # increment dst by 8 bytes
    bnez a2, 3f      # if length remaining, continue

    # Copy remaining 4 bytes
3:
    lw a6, 0(a1)     # load 4 bytes from src
    sw a6, 0(a5)     # store 4 bytes to dst
    addi a1, a1, 4   # increment src by 4 bytes
    addi a5, a5, 4   # increment dst by 4 bytes
    bnez a2, 4f      # if length remaining, continue

    # Copy remaining 2 bytes
4:
    lh a6, 0(a1)     # load 2 bytes from src
    sh a6, 0(a5)     # store 2 bytes to dst
    addi a1, a1, 2   # increment src by 2 bytes
    addi a5, a5, 2   # increment dst by 2 bytes
    bnez a2, 5f      # if length remaining, continue

    # Copy remaining 1 byte
5:
    lb a6, 0(a1)     # load 1 byte from src
    sb a6, 0(a5)     # store 1 byte to dst
    addi a1, a1, 1   # increment src by 1 byte
    addi a5, a5, 1   # increment dst by 1 byte

    ret              # return from function

.Lsrcunaligned_bytecopy:
    lb a6, 0(a1)     # load 1 byte from src
    sb a6, 0(a5)     # store 1 byte to dst
    addi a1, a1, 1   # increment src by 1 byte
    addi a5, a5, 1   # increment dst by 1 byte
    bnez a2, 2b      # if length remaining, continue

.Ldone:
    ret
