

.data

$vecdata:
	.long 0 
	.long 1
	.long 2 
	.long 3
	.long 4 
	.long 5
	.long 6 
	.long 7
	.long 8 
	.long 9
	.long 10 
	.long 11
	.long 12 
	.long 13
	.long 14 
	.long 15
	.long 16 
	

.code
MAIN:
	mov %sp, 1024
	mov %r0, %block_index
	shl %r0, %r0, 3
	add %sp, %sp, %r0  # now %sp = { 1024, 1024+8, 1024 + 16, etc. }
	
	mov %r0, 123

	mov %r3, %r0
	mov %r1, 43
	add %r2, %r3, %r1

	mov %r4, 3

	// load [$vecdata] into r1
	loadaddress_lo r0, $vecdata
	loadaddress_mid r0, $vecdata
	loadaddress_hi r0, $vecdata
	mov %r1, %block_index
	shl %r1, %r1, 3
	add %r0, %r1, %r0
	load %r1, 0(%r0)
		
	cmp %r4, %r1

	store 8(%sp), %r4
	load %r4, 8(%sp)

	
	
	jg retlabel
	jge retlabel
	
	jl retlabel
	jle retlabel
	
	je retlabel
	jne retlabel

	# call bar
	add %sp, %sp, -40
	mov %r0, retlabel
	store 0(%sp), %r0
	jmp bar
retlabel:
	add %sp, %sp, 40
back:

	halt

bar:
	mov %r0, 42
	restorepc 0(%sp)

foo:
	add %r1, %r1, 1
	jmp back