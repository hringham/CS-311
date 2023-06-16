// Student name: Hannah Ingham
// CS-311 - assembler assignment
// isSoldArm64() function
// Language: ARM64 Gnu Assembler

// Each "TODO" requires you to write one instruction or assember directive.
// ==> No help from others is allowed, except for questions in class.

// To cross-compile and run the program
//    aarch64-linux-gnu-gcc -g3 -O0 -o concertArm64 concert.c eventSales.s -lm -static
//    qemu-aarch64 ./concertArm64
// See class slides for debugging instructions.

// Directive to make the funtion name available for linking
.global isSoldARM64

// Defining symbols based on the concert.h file (must match)
//    !!!IMPORTANT!!! - Your EventSales structure must be defined as:
//    struct EventSales {
//       struct Venue *venue;
//       unsigned int **sectionBitArrays;
//       int *sectionCounts;  // number of sold tickets for each section
//    };

// Define FIRST_SECTION_ID
//   syntax for a character (such as a question mark) uses on single quote like:  '?
.equ FIRST_SECTION_ID, 'A

// Define LAST_SECTION_ID
.equ LAST_SECTION_ID, 'Z

// Define SECTION_BIT_ARRAYS_FIELD_OFFSET
//    This is the offset of sectionBitArrays field within the EventSales structure
//    You can the use the diagram from class to get the offset
.equ SECTION_BIT_ARRAYS_FIELD_OFFSET, 8

//   Since WORD_BITS is 32 (a power of 2) in concert.h:
//   The shift amount is the power of 2 equal that yields WORD_BITS
//   ANDing with this remainder mask gives the effect of a modulus ooperation
.set WORD_BITS_SHIFT, 5
.set WORD_BITS_REMAINDER_MASK, 0x1F

// isSoldARM64: Test whether or not a seat is sold
//
// Parameters
//   X0 (pointer): pointer to the EventSales structure
//   W1 (integer): section for the venue
//   W2 (integer): seat number (1-based)
// 
//  Returns:
//   W0 (integer) 0 if seat is not sold, non-zero if sold
//
//  Scratch Register usage:
//   W6  (int)          sectionIndex
//   W7  (int)          seatIndex
//   W8  (int)          wordIndex
//   W9  (int)          bitOffset
//   W10 (unsigned int) mask
//   X11 (pointer)      sales->sectionBits  (base of array of pointers)
//   X12 (pointer)      sales->sectionBits[i] (base of array of words for a section)
//   W13 (unsigned int) seatWord

//  Instructions you will need:
//  AND
//  B.xx  (different conditional branches)
//  BL
//  CMP
//  LDR
//  LSLV  (left shift using a variable)
//  LSR
//  RET
//  STR
//  SUB

// label for start of function
isSoldARM64:
    // Push LR with our standard function entry sequence
    STR     LR, [SP, #-16]!

    // if (sectionId<FIRST_SECTION_ID || sectionId>LAST_SECTION_ID) fail("Invalid section ID");

    // Compare passed in section ID to FIRST_SECTION_ID immediate value
    CMP     W1, #FIRST_SECTION_ID

    // If less than, branch to sectionErr label
    B.LT    sectionErr

    // Compare passed in section ID to max allowed sectionID
    CMP     W1, #LAST_SECTION_ID

    // If less than or equal, branch to sectionOK label
    B.LE   sectionOK

sectionErr:
    // Load "Invalid section ID" message as 1st argument using =label from .data section
    LDR     X0, =invSection

    // Call the fail() function
    BL     fail

sectionOK:   // If we get here, sectionID is in the range A-Z

    // int sectionIndex = sectionId - FIRST_SECTION_ID;
    // Calculate sectionIndex with result in W6. (use immediate of named constant)
    SUB     W6, W1, #FIRST_SECTION_ID

    // Seats are 1-based, but array indexes are zero-based.
    // int seatIndex = seat - 1;
    // Calculate seatIndex into W7
    SUB     W7, W2, #1

    // int wordIndex = seatIndex / WORD_BITS;
    // Calculate wordIndex into W8 using an immediate shift rather than division
    LSR     W8, W7, #WORD_BITS_SHIFT

    //int bitOffset = seatIndex % WORD_BITS;
    // Calculate modulus by ANDing the with an immediate WORD_BITS_REMAINDER_MASK
    AND     W9, W7, #WORD_BITS_REMAINDER_MASK

    //unsigned int mask = 0x1u << bitOffset;  // u suffix makes the right hand side unsigned
    //   (implemented with the next two instructions)
    // Load immediate value of 0x1 into register W10
    MOV     W10, #0x1
    // Shift W10 left by bitOffset thus leaving the needed mask in W10
    LSLV    W10, W10, W9

    //uint32_t *sectionBits = sales->sectionBitArrays[sectionIndex];
    //   (implemented with the next two instructions)
    // Load X11 with the pointer contained in sales->sectionBitArrays
    LDR     X11, [X0, #SECTION_BIT_ARRAYS_FIELD_OFFSET]
    // Index into this array and store sectionBits pointer in X12
    //    (you must use SXTW 3 to multiply the index by 8)
    LDR     X12, [X11, W6, SXTW 3]

    // locate the word that contains this seat and apply the mask to isolate the seat's bit
    // return (sectionBits[word] & mask);
    //  (next two instructions to calculate the return expression)
    // Get sectionBits[word] r this seat into W13
    //    (you must use SXTW 2 to multiply the index by 4)
    LDR     W13, [X12, W8, SXTW 2]

    // TODO Bitwise AND the seat's word with the seat's mask leaving value in W0 (function result register)
    AND     W0, W13, W10

    // Pop stack, restoring LR
    LDR     LR, [SP], #16

    // Return from procedure call
    RET

.data
invSection:
    .asciz  "Invalid section ID\n"
