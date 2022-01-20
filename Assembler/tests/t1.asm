

MAIN:
	mov %sp, 1024
	mov %r0, %block_index
	shl %r0, %r0, 3
	add %sp, %sp, %r0  # now %sp = { 1024, 1024+8, 1024 + 16, etc. }
	
	mov %r0, 123

	mov %r3, %r0
	mov %r1, 43
	add %r2, %r3, %r1

	mov %r4, 512
		
	cmp %r4, %r1

	store 8(%sp), %r4
	load %r4, 8(%sp)

	
	je retlabel
	jne retlabel
	
	jg retlabel
	jge retlabel
	
	jl retlabel
	jle retlabel

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