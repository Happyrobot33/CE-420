/* 
a0: A0-pointer
a1: A1-pointer
a2: Z-pointer
a3: Undone array pointer, could be either A0-pointer or A1-pointer, 
*/
.global main 
.data 			/* place your data in .data segment */

A0: .byte 0xE6, 0xA5, 0x95, 0x80, 0x79, 0x53, 0x4B, 0x32, 0x27, 0x14, 0x07, 0x00	
A1: .byte 0xDF, 0xA4, 0xA3, 0x70, 0x64, 0x58, 0x57, 0x56, 0x30, 0x12, 0x00
Z: .space 40  					# 40-byte space is reserved for array Z

.text 							/* place your code in .text segment */
.set noreorder				/* Do not optimize. Do not add nop after branch*/
.ent main 						# Used in ELF file
main:
	la	$a0, A0				# $a0 points to the base of array A0.			
	la	$a1, A1				# $a1 points to the base of array A1.
	la	$a2, Z				# $a2 points to the base of array Z.\
	
	Loop:
	lbu $t0, 0($a0)			# read next byte from array A0
	lbu $t1, 0($a1)			# read next byte from array A1
	
	bgtu $t0, $t1, A0Larger # Branch decision, jump to A0Larger if a0 is a larger value
	nop
	j A1Larger # Jump to A1Larger Otherwise
	nop

	
	A0Larger:
	beq $t0, 0, CheckEndState # if the value is 0, skip pushing the value to the z
	nop
	sb $t0, 0($a2) # store the value of t0 into the pointed location of a2
	addiu $a0, $a0, 1 # Increment the pointer of a0
	j IncrementZPointer
	nop
    
	A1Larger:
	beq $t1, 0, CheckEndState # if the value is 0, skip pushing the value to the z
	nop
	sb $t1, 0($a2) # store the value of t0 into the pointed location of a2
	addiu $a1, $a1, 1 # Increment the pointer of a0
	j IncrementZPointer
	nop
    
	IncrementZPointer:
	addiu $a2, $a2, 1 # increment the a2 register
	j Loop # Continue the loop
	nop
    
	# This label is only reached if one of the values is 0
	CheckEndState:
	beq $t0, $t1, End # If both values are 0, then go to the end
	nop
	j Loop # Continue the loop
	nop
	
	End:
.end main