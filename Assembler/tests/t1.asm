

MAIN:
	mov %sp, 1024

	mov %r0, 123
	mov %r3, %r0
	mov %r1, 1
	add %r2, %r3, %r1

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