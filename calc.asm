; calc.asm

.code
calc proc
	; calc(int64_t* arr, size_t count)
	; rcx is arr, rdx is count
	push rsi
	mov rsi,rcx		; rsi <- arr
	mov rcx, rdx	; rcx <- count
	xor rax,rax		; rax: sum; sum = 0
	test rcx,rcx	; if count <= 0
	jle exit		;   return
add_start:
	mov r8, [rsi]	; r8 <- *rsi
	test r8,r8		; if r8 <= 0
	jle next		;   goto next
	add rax,r8		; sum += r8
next:
	lea rsi, 8[rsi]	; rsi += 8
	loop add_start
exit:
	pop rsi
   ret
calc endp

calc_no_jump proc
	; calc(int64_t* arr, size_t count)
	; rcx is arr, rdx is count
	push rsi
	mov rsi,rcx		; rsi <- arr
	mov rcx, rdx	; rcx <- count
	xor rax,rax		; rax: sum; sum = 0
	test rcx,rcx	; if count <= 0
	jle exit		;   return
add_start:
	xor r9,r9
	mov r8, [rsi]	; r8 <- *rsi
	test r8,r8		; if r8 > 0,
	cmovg r9, r8	;    r9 <- r8
	add rax,r9		; sum += r9
next:
	lea rsi, 8[rsi]	; rsi += 8
	loop add_start
exit:
	pop rsi
	ret
calc_no_jump endp

calc_no_jump_long_fetches proc
	; calc(int64_t* arr, size_t count)
	; rcx is arr, rdx is count
	mov r11,rcx		; r11 <- arr
	mov r10, rdx	; r10 <- count
	xor rax,rax		; rax: sum; sum = 0
	test r10,r10	; if count <= 0
	jle exit		;   return
	shr r10, 12		; r10 /= 4096
	push rdi
	push rsi
	mov rdi, 4095	; rdi: index = 4095
outer:
	mov rcx, r10	; init rcx to count/4096
	lea rsi, [r11+rdi*8]	; and rsi to arr + index
add_start:
	xor r9,r9
	mov r8, [rsi]	; r8 <- *rsi
	test r8,r8		; if r8 > 0,
	cmovg r9, r8	;    r9 <- r8
	add rax,r9		; sum += r9
next:
	lea rsi, 32768[rsi]	; rsi += 4096*8
	loop add_start
	dec rdi				; index--
	jge outer			; if index>=0, start the loop again
	pop rsi
	pop rdi
exit:
   ret
calc_no_jump_long_fetches endp
calc_no_jump_not_so_long_fetches proc
	; calc(int64_t* arr, size_t count)
	; rcx is arr, rdx is count
	mov r11,rcx		; r11 <- arr
	mov r10, rdx	; r10 <- count
	test r10,r10	; if count <= 0
	jle exit		;   return
	push rsi
	xor rax,rax		; rax: sum; sum = 0
	shr r10, 10		; r10 /= 1024
	mov rdx, 1023	; rdx: index = 1023
outer:
	mov rcx, r10	; init rcx to count/1023
	lea rsi, [r11+rdx*8]	; and rsi to arr + index
add_start:
	xor r9,r9
	mov r8, [rsi]	; r8 <- *rsi
	test r8,r8		; if r8 > 0,
	cmovg r9, r8	;    r9 <- r8
	add rax,r9		; sum += r9
next:
	lea rsi, 8192[rsi]	; rsi += 1024*8
	loop add_start
	dec rdx				; index--
	jge outer			; if index>=0, start the loop again
	pop rsi
exit:
   ret
calc_no_jump_not_so_long_fetches endp

end