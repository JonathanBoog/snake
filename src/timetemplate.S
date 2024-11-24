# timetemplate.S
# Written 2015 by F Lundevall
# Ported 2024/06 by W Szczerek (from MIPS to RISC-V)
# Copyright abandonded - this file is in the public domain.

#############################################################
# Choose the macro syntax for RARS or DTEK-V board.         #
# The syntax for RARS is probably due to its MIPS heritage. #
#############################################################
###################
# Macros for RARS #
###################
#.macro	PUSH (%reg)
#	addi	sp,sp,-4
#	sw	%reg,0(sp) 
#.end_macro
#
#.macro	POP (%reg)
#	lw	%reg,0(sp)
#	addi	sp,sp,4
#.end_macro
###################
# Macros for DTEK #
###################
.macro	PUSH reg
	addi sp,sp,-4
	sw \reg,0(sp) 
.endm

.macro	POP reg
	lw	\reg,0(sp)
	addi	sp,sp,4
.endm
#############################################################

	.data
	.align 2
mytime:	.word 	0x5957
timstr:	.asciz 	"text more text lots of text\0"
	.text
	.globl timetemplate, tick, time2string, delay, display_string

# Function for displaying a string with a newline at the end	
display_string:	
	li a7,4
	ecall
	li a0, 10
	li a7,11
	ecall
	jr ra
	
timetemplate:
	la	a0, timstr
	jal     display_string
	
	# wait a little
	li	a0, 1000		# ms
	jal	delay
	
	# call tick
	la 	a0, mytime
	jal	tick
	
	# call your function time2string
	la	a0, timstr
	la	t0, mytime
	lw	a1, 0(t0)
	jal	time2string
	
	# go back and do it all again
	j	timetemplate

	
# tick: update time pointed to by $a0
tick:	lw	t0, 0(a0)	# get time
	addi	t0, t0, 1	# increase
	andi	t1, t0, 0xf	# check lowest digit
	sltiu	t2, t1, 0xa	# if digit < a, okay
	bnez	t2, tiend
	addi	t0, t0, 0x6	# adjust lowest digit
	
	andi	t1, t0, 0xf0	# check next digit
	sltiu	t2, t1, 0x60	# if digit < 6, okay
	bnez	t2, tiend
	addi	t0, t0, 0xa0	# adjust digit
	
	li	t3, 0xF
	slli	t3, t3, 0x8
	and	t1, t0, t3	# check minute digit
	addi	t3, x0, 0xA
	slli	t3, t3, 0x8
	slt	t2, t1, t3	# if digit < a, okay
	bnez	t2, tiend
	addi	t0, t0, 0x600	# adjust digit - this one's okay, it's lower than 0x7FF 
	
	li	t3, 0xF
	slli	t3, t3, 0xC
	and	t1, t0, t3	# check last digit
	addi	t3, x0, 0x6
	slli	t3, t3, 0xC
	slt	t2, t1, t3	# if digit < 6, okay
	bnez	t2, tiend
	
	li	t3, 0xA
	slli	t3, t3, 0xC
	add	t0, t0, t3	# adjust last digit
tiend:	sw	t0,0(a0)	# save updated result
	jr	ra		# return

#########################################################
# Place for your functions: time2string, hex2asc, delay.#
#########################################################

# hex2asc: Convert a single hex digit (0-15) to ASCII

hex2asc:
	li	a0, 4		# test number (from 0 to 15)
	
	addi    sp,sp,-4
	sw      ra,0(sp)
	
	jal	hexasc		# call hexasc
	
	li	a7, 11		# write a0 to stdout
	ecall

	lw      ra,0(sp)
	addi    sp,sp,4
	jr      ra	

  # You can write your own code for hexasc here
  #

hexasc:
	andi    a0, a0, 0xF    # Mask all but the 4 least significant bits

	li      t0, 10         # Load immediate value 10 into t0
	blt     a0, t0, digit  # If a0 < 10, branch to digit

	li      t0, 0x37       # Load ASCII offset for 'A'
	add     a0, a0, t0     # Convert 10-15 to 'A'-'F'
	j       done           # Jump to done

digit:
	li      t0, 0x30       # Load ASCII offset for '0'
	add     a0, a0, t0     # Convert 0-9 to '0'-'9'

done:
	andi    a0, a0, 0x7F   # Ensure only the 7 least significant bits are returned
	jr ra                   # Return

    
    
delay:
	mv      t0, a0           # t0 = ms	

delay_while:
	blez    t0, delay_end    # If ms <= 0, jump to delay_end
    	addi    t0, t0, -1       # ms = ms - 1
    	li      t1, 15000       # Load 15000 into t1 (loop counter for delay_for)
	
delay_for:
	addi    t1, t1, -1       # i = i - 1
    	bnez    t1, delay_for # If i != 0, continue delay_for
    	j       delay_while # Repeat the outer loop

delay_end:
	ret

time2string:

	PUSH ra
	PUSH s1

    # Extract minutes

    srli     s1, a1, 8           # Shift a1 right by 8 bits to get minutes into s1
    andi    s1, s1, 0xFF         # Mask out all but the lower 8 bits (minutes) 

    # Convert tens of minutes
    PUSH a0
    srli    a0, s1, 4            # Get the most significant digit
    andi    a0, a0, 0xF          # Mask to isolate the digit
    jal     hexasc              # Call hex2asc
    mv	    t2, a0		# move returnvalue to t2
    POP a0
   
    sb      t2, 0(a0)            # Store the ASCII character in memory


    # Convert units of minutes
    PUSH a0
    andi    a0, s1, 0xF          # Get the least significant digit
    jal     hexasc              # Call hex2asc
    mv	    t2, a0		# move returnvalue to t2
    POP a0

    sb      t2, 1(a0)            # Store in memory



    # Store colon

    li      t4, 0x3A             # ASCII for colon ':'
    sb      t4, 2(a0)            # Store in memory


    # Extract seconds

    andi    s1, a1, 0xFF         # Mask to get the lower 8 bits
    


    # Convert tens of seconds
    PUSH a0
    srli    a0, s1, 4            # Get the most significant digit
    andi    a0, a0, 0xF          # Mask to isolate the digit
    jal     hexasc              # Call hex2asc
    mv	    t2, a0		# move returnvalue to t2
    POP a0
    sb      t2, 3(a0)            # Store in memory

	#Convert units of seconds
    PUSH a0
    andi    a0, s1, 0xF          # Get the least significant digit
    jal     hexasc              # Call hex2asc
    mv	    t2, a0		# move returnvalue to t2
    POP a0

    sb      t2, 4(a0)            # Store in memory


    # Store null byte

    li      t4, 0x00             # ASCII code for NULL
    sb      t4, 5(a0) 

	
	POP s1
    	POP ra

    	ret 
    	
    	                                      # Return from subroutine
    


	
