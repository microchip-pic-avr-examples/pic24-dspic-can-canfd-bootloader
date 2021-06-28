/**
  @Generated 16-bit Bootloader Source File

  @Company:
    Microchip Technology Inc.

  @File Name: 
    sha256_asm.S

  @Summary:
    This is the sha256_asm.S file generated using 16-bit Bootloader

  @Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  16-bit Bootloader - 1.18.4
        Device            :  dsPIC33CK256MP508
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.36B
        MPLAB             :  MPLAB X v5.15
*/
/*
Copyright (c) [2012-2019] Microchip Technology Inc.  

    All rights reserved.

    You are permitted to use the accompanying software and its derivatives 
    with Microchip products. See the Microchip license agreement accompanying 
    this software, if any, for additional info regarding your rights and 
    obligations.
    
    MICROCHIP SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT 
    WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT 
    LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT 
    AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP OR ITS
    LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT 
    LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE 
    THEORY FOR ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT 
    LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES, 
    OR OTHER SIMILAR COSTS. 
    
    To the fullest extend allowed by law, Microchip and its licensors 
    liability will not exceed the amount of fees, if any, that you paid 
    directly to Microchip to use this software. 
    
    THIRD PARTY SOFTWARE:  Notwithstanding anything to the contrary, any 
    third party software accompanying this software is subject to the terms 
    and conditions of the third party's license agreement.  To the extent 
    required by third party licenses covering such third party software, 
    the terms of such license will apply in lieu of the terms provided in 
    this notice or applicable license.  To the extent the terms of such 
    third party licenses prohibit any of the restrictions described here, 
    such restrictions will not apply to such third party software.
*/

.include "xc.inc"
   
.section *,bss,near
//If we align the KTable to 512, then the lower byte becomes the current offset
//  as well and thus we don't have to track the offset in a working register
.align 512
KTable: .space 256 
.global _workingBuffer
_workingBuffer: .space 256
_workingBufferPointer: .space 2
_hash: .space 32
_a: .space 4
_b: .space 4
_c: .space 4
_d: .space 4
_e: .space 4
_f: .space 4
_g: .space 4
_h: .space 4
_temp: .space 8
  
.text
        
KTableData:
.long 0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5 
.long 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174 
.long 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da 
.long 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967 
.long 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85 
.long 0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070 
.long 0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3 
.long 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2

HashInit:
.long 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    
//******************************************************************************
// void void SHA_Initialize(void)
//
// Working registers used: W0-W2
// Modifies TBLPAG, but saves/restores original value.
//
// Loads RAM variables for Hash calculation.  Must be called at the start of
// each hash calculation.
//******************************************************************************
.global _SHA_Initialize
_SHA_Initialize:
    //Load KTable data from flash into RAM variable for quicker access.  This
    // burns quite a bit of RAM, but saves a good amount of time.
    push _TBLPAG
    mov #KTable,W0
    mov tblpage(#KTableData),W1
    mov W1,_TBLPAG
    mov tbloffset(#KTableData),W1
    mov #0x80,W2
KTableCopyLoop:
    tblrdl [W1++],[W0++]
    dec W2,W2
    bra NZ,KTableCopyLoop
            
    //Load hash RAM variable with appropriate data
    mov #_hash,W0
    mov tblpage(#HashInit),W1
    mov W1,_TBLPAG
    mov tbloffset(#HashInit),W1
    mov #0x10,W2
HashInitCopyLoop:
    tblrdl [W1++],[W0++]
    dec W2,W2
    bra NZ,HashInitCopyLoop
    pop _TBLPAG
    return
              
//******************************************************************************
// void void SHA_HashBlock(void)
//
// Working registers used: W0-W13
//
// Calculates one block of the hash.  Entire block must be provided.  Caller is 
// responsible for padding with 0s and adding length on last block so that it is
// still a completely block.
//******************************************************************************
.global _SHA_HashBlock
_SHA_HashBlock:
    push.d W8
    push.d W10
    push.d W12
            
PrepareWorkingBuffer:
    mov #_workingBuffer,W0
    mov W0,W10  //This will become workingBuffer[i-16] after the first loop completes.
    mov W0,W1
    mov #0x10,W4
PrepareFirstBlock:
    mov [W1++],W2
    mov [W1++],W3
    swap W2
    swap W3
    mov W3,[W0++]
    mov W2,[W0++]
    dec W4,W4
    bra NZ,PrepareFirstBlock
   
    mov W0,W8 //workingBuffer[i]   
    sub W0,#0x1C,W9 //workingBuffer[i-7]
    //workingBuffer[i-16] is already in W10 from earlier

    mov #0x30,W11 //loop counter 0x30=64-16
    mov #_temp,W4

PrepareSecondBlock:
    //L0 operation
    mov [W8-0x3C],W0
    mov [W8-0x3A],W1
    
    // SHA2_ROTR(x,7)
    lsr W0,#7,W2
    sl W1,#9,W3
    ior W2,W3,[W4++]
    lsr W1,#7,W3
    sl W0,#9,W2
    ior W3,W2,[W4--]

    // SHA2_ROTR(x,7) ^ SHA2_ROTR(x,18)
    sl W0,#14,W2
    lsr W1,#2,W3
    xor W2,[W4],W2
    xor W3,W2,[W4++]
    sl W1,#14,W3
    lsr W0,#2,W2
    xor W3,[W4],W3
    xor W2,W3,[W4--]
            
    // SHA2_ROTR(x,7) ^ SHA2_ROTR(x,18) ^ SHA2_SHR(x,3)
    lsr W0,#3,W2
    sl W1,#13,W3
    xor W2,[W4++],W2
    xor W3,W2,W0
    lsr W1,#3,W3
    xor W3,[W4--],W1
    
    add W0,[W9++],W0
    addc W1,[W9++],W1
    
    add W0,[W10++],[W4++]
    addc W1,[W10++],[W4++] //Increment up to next 32-bit temp location
            
    //L1 operation
    mov [W8-0x8],W0
    mov [W8-0x6],W1
            
    //SHA2_ROTR(x,17)
    sl W0,#15,W2
    lsr W1,#1,W3
    ior W2,W3,[W4++]
    sl W1,#15,W3
    lsr W0,#1,W2
    ior W3,W2,[W4--]

    // SHA2_ROTR(x,17) ^ SHA2_ROTR(x,19)
    sl W0,#13,W2
    lsr W1,#3,W3
    xor W2,[W4],W2
    xor W3,W2,[W4++]
    sl W1,#13,W3
    lsr W0,#3,W2
    xor W3,[W4],W3
    xor W2,W3,[W4--]
            
    // SHA2_ROTR(x,17) ^ SHA2_ROTR(x,19) ^ SHA2_SHR(x,10)
    lsr W0,#10,W2
    sl W1,#6,W3
    xor W2,[W4++],W0
    xor W3,W0,W0
    lsr W1,#10,W3
    xor W3,[W4--],W1
    
    sub W4,#4,W4
    add W0,[W4++],[W8++]
    addc W1,[W4--],[W8++]
    
    dec W11,W11
    bra NZ,PrepareSecondBlock
    ////////////////// End of Prepare Loops ////////////////////
    
    mov #_workingBuffer,W0
    mov W0,_workingBufferPointer

    mov #_hash,W0

    mov #_a,W5
    mov #_b,W6
    mov #_c,W7
    mov #_d,W8
    mov #_e,W9
    mov #_f,W10
    mov #_g,W11
    mov #_h,W12
    mov #KTable,W13

    mov [W0++],[W5++]   //a = hash[0];            
    mov [W0++],[W5--]   //a = hash[0];

    mov [W0++],[W6++]   //b = hash[1];            
    mov [W0++],[W6--]   //b = hash[1];

    mov [W0++],[W7++]   //c = hash[2];            
    mov [W0++],[W7--]   //c = hash[2];

    mov [W0++],[W8++]   //d = hash[3];            
    mov [W0++],[W8--]   //d = hash[3];

    mov [W0++],[W9++]   //e = hash[4];            
    mov [W0++],[W9--]   //e = hash[4];

    mov [W0++],[W10++]   //f = hash[5];            
    mov [W0++],[W10--]   //f = hash[5];

    mov [W0++],[W11++]   //g = hash[6];            
    mov [W0++],[W11--]   //g = hash[6];

    mov [W0++],[W12++]   //h = hash[7];            
    mov [W0++],[W12--]   //h = hash[7];
       

//////////////////////////////////////////////////////////////    
//////////////////////// MAIN LOOP ///////////////////////////
//////////////////////////////////////////////////////////////

//for (i = 0; i < 64; i++)
//{   
//    t1 = T1(e,f,g,i);   
//    t2 = T2(a,b,c);
//    h = g;
//    g = f;
//    f = e;
//    e = d + t1;
//    d = c;
//    c = b;
//    b = a;
//    a = t1 + t2;
//}

//WORKING REGISTER USAGE:
// W0-W3: various data calculations
// W4: pointer to temp variable storage
// W5: pointer to a
// W6: pointer to b
// W7: pointer to c
// W8: pointer to d
// W9: pointer to e
// W10: pointer to f
// W11: pointer to g
// W12: pointer to h
// W13: KTable Pointer / loop counter
MainBlockLoop:                        
T2:
    //move a -> W0:W1
    mov [W5++],W0
    mov [W5--],W1
            
    //SHA2_ROTR(a,2)
    lsr W0,#2,W2
    sl W1,#14,W3
    ior W2,W3,[W4++]           
    lsr W1,#2,W2
    sl W0,#14,W3
    ior W2,W3,[W4--]

    // SHA2_ROTR(a,2) ^ SHA2_ROTR(a,13)
    lsr W0,#13,W2
    sl W1,#3,W3
    xor W2,[W4],W2
    xor W3,W2,[W4++]
    lsr W1,#13,W3
    sl W0,#3,W2
    xor W3,[W4],W3
    xor W2,W3,[W4--]

    // SHA2_ROTR(a,2) ^ SHA2_ROTR(a,13) ^ SHA2_ROTR(a,22)
    sl W0,#10,W2
    lsr W1,#6,W3
    xor W2,[W4],W2
    xor W3,W2,[W4++] 
    sl W1,#10,W3
    lsr W0,#6,W2
    xor W3,[W4],W3
    xor W2,W3,[W4--]
            
    // a & c - Least significant word (LSW)
    mov [W7++],W2
    and W2,W0,W3
    // b & c - LSW
    and W2,[W6],W2
    // a & b - LSW
    and W0,[W6++],W0
    // (a & b) ^ (a & c) - LSW
    xor W3,W0,W0
    // (a & b) ^ (a & c) ^ (b & c) - LSW
    xor W0,W2,W0
         
    // (a & b) ^ (a & c) ^ (b & c) + SHA2_ROTR(a,2) ^ SHA2_ROTR(a,13) ^ SHA2_ROTR(a,22) - LSW
    add W0,[W4],[W4++]
      
    // a & c - Most significant word (MSW)
    mov [W7--],W2
    and W2,W1,W3
    // b & c - MSW
    and W2,[W6],W2
    // a & b - MSW
    and W1,[W6--],W1
    // (a & b) ^ (a & c) - MSW
    xor W3,W1,W1
    // (a & b) ^ (a & c) ^ (b & c) - MSW
    xor W1,W2,W1

    // (a & b) ^ (a & c) ^ (b & c) + SHA2_ROTR(a,2) ^ SHA2_ROTR(a,13) ^ SHA2_ROTR(a,22) - MSW   
    addc W1,[W4],[W4++] //increment temp pointer from T2 result to next temp 32-bit value
             
    //END T2 CALCULATION
            
T1:   
    // move e-> W0:W1
    mov [W9++],W0
    mov [W9--],W1

    //SHA2_ROTR(e,6)
    lsr W0,#6,W2
    sl W1,#10,W3
    ior W2,W3,[W4++]       
    lsr W1,#6,W3
    sl W0,#10,W2
    ior W3,W2,[W4--]

    // SHA2_ROTR(e,6) ^ SHA2_ROTR(e,11)
    lsr W0,#11,W2
    sl W1,#5,W3
    xor W2,[W4],W2
    xor W3,W2,[W4++]           
    lsr W1,#11,W3
    sl W0,#5,W2
    xor W3,[W4],W3
    xor W2,W3,[W4--]

    // SHA2_ROTR(e,6) ^ SHA2_ROTR(e,11) ^ SHA2_ROTR(e,25)
    sl W0,#7,W2
    lsr W1,#9,W3
    xor W2,[W4],W2
    xor W3,W2,[W4++]  
    sl W1,#7,W3
    lsr W0,#9,W2
    xor W3,[W4],W3
    xor W2,W3,[W4--]
      
    // ((e & f) ^ ((~e) & g)) - Least significant word
    // ~e
    com W0,W2           
    // ((~e) & g))
    and W2,[W11++],W2
    // e & f            
    and W0,[W10++],W3
    // ((e & f) ^ ((~e) & g))
    xor W3,W2,W3

    // ((e & f) ^ ((~e) & g)) + SHA2_ROTR(e,6) ^ SHA2_ROTR(e,11) ^ SHA2_ROTR(e,25) - LSW
    add W3,[W4++],W0
    
    // ((e & f) ^ ((~e) & g)) - Most significant word
    // ~e
    com W1,W3       
    // ((~e) & g))
    and W3,[W11--],W3
    // e & f            
    and W1,[W10--],W2
    // ((e & f) ^ ((~e) & g))
    xor W2,W3,W2
    mov _workingBufferPointer,W3 //Loading workingBufferPointer early to avoid RAW dependency
    
    // ((e & f) ^ ((~e) & g)) + SHA2_ROTR(e,6) ^ SHA2_ROTR(e,11) ^ SHA2_ROTR(e,25) - MSW
    addc W2,[W4--],W1
    
    sub W4,#4,W4 //point back to T2 temp variable
                
    //+ workingBuffer[loopIndex]
    add W0,[W3++],W0
    addc W1,[W3++],W1
    mov W3,_workingBufferPointer
    
    add W0,[W12++],W0
    addc W1,[W12--],W1

    //+ KTable[loopIndex]
    add W0,[W13++],W0
    addc W1,[W13++],W1
    
    //END OF T1 Calculation    

    //store the pointer where W12 was pointing so we can recover it later
    mov W12,W2
    
    //We move the pointer values rather than the data to save cycles.  
    // So instead of copying the value of g to the value of h we copy the
    // pointer to g into the pointer to h so the pointer of h now points to the
    // correct value.
    
    //h = g;
    mov W11,W12

    //g = f;
    mov W10,W11
          
    //  The next two steps are combined to remove a RAW dependency on W8
    //    f = e;            
    //    e = d ( + t1 will be applied later ) ;
    add W0,[W8],[W8++]
    mov W9,W10  //    f = e;
    addc W1,[W8],[W8--]
    mov W8,W9

    //d = c;
    mov W7,W8
            
    //c = b;
    mov W6,W7
    
    //b = a;
    mov W5,W6         
    
    //a = t1 + t2;  //new a is old h location which is cached in W2
    add W0,[W4++],[W2++]
    addc W1,[W4--],[W2--]
    mov W2,W5  

    //We are using the KTable pointer as a loop index as well.  This requires
    //the KTable to be aligned to 256 so that for the first iteration of the
    //loop, the KTable pointers lower byte is 0x00.           
    //See if we have completed 64 iterations (at +4 each loop = 256, or bit 8)
    btss W13,#8
    bra MainBlockLoop
        
//////////////////////////////////////////////////////////////    
//////////////////////// END MAIN LOOP ///////////////////////
//////////////////////////////////////////////////////////////
    
//    hash[0] += a;
//    hash[1] += b;
//    hash[2] += c;
//    hash[3] += d;
//    hash[4] += e;
//    hash[5] += f;
//    hash[6] += g;
//    hash[7] += h;
    
CompleteBlock:
    mov [W5++],W0           
    add _hash+0x0
    mov [W5++],W0           
    addc _hash+0x2
            
    mov [W5++],W0           
    add _hash+0x4
    mov [W5++],W0           
    addc _hash+0x6
            
    mov [W5++],W0           
    add _hash+0x8
    mov [W5++],W0           
    addc _hash+0xA
            
    mov [W5++],W0           
    add _hash+0xC
    mov [W5++],W0           
    addc _hash+0xE
            
    mov [W5++],W0           
    add _hash+0x10
    mov [W5++],W0           
    addc _hash+0x12
            
    mov [W5++],W0           
    add _hash+0x14
    mov [W5++],W0           
    addc _hash+0x16
            
    mov [W5++],W0           
    add _hash+0x18
    mov [W5++],W0           
    addc _hash+0x1A
            
    mov [W5++],W0           
    add _hash+0x1C
    mov [W5++],W0           
    addc _hash+0x1E
    
    pop.d W12
    pop.d W10
    pop.d W8
    return    
            

//******************************************************************************
// void void SHA_HashGet(void)
//
// Working registers used: W0-W4
//
// Copies the final hash into specified buffer.  Calculations are done in big
// endian.  Results are normally specified/used in little endian.
//******************************************************************************
.global _SHA_HashGet
_SHA_HashGet:
    mov #_hash,W1
    mov #0x8,W4 //loop counter

//Reverses the endianness of the result to expected format.
SHA_HashGetLoop:
    mov [W1++],W2
    mov [W1++],W3
    swap W2
    swap W3
    mov W3,[W0++]
    mov W2,[W0++]
    dec W4,W4
    bra NZ, SHA_HashGetLoop

    return