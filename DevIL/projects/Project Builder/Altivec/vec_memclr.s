.text
	.align 2
	.globl _vec_memclr

; memset(r3,0,r4);
; but more than 4 times faster and more then 10 on a lot of data (> 64bytes)
; r3 -> start pointer  r4 -> lenght

_vec_memclr:
        vspltisw v0,0
        stw r5,8(r30)
        stw r6,16(r30)
        stw r8,20(r30)
        li r5,0
        stw r9,24(r30)
        stw r4,4(r30)
        li r6,16
        li r8,32
        li r9,48
        
        
L_start_64:
        cmpwi r4,64
        blt L_end_64
        
        stvx v0,r5,r3
        subi r4,r4,64
        addi r5,r5,64
        stvx v0,r6,r3
        addi r6,r6,64
        stvx v0,r8,r3
        addi r8,r8,64
        stvx v0,r9,r3
        addi r9,r9,64
        
        b L_start_64
L_end_64:
        lwz r9,24(r30)
        lwz r8,20(r30)
        lwz r6,16(r30)


L_L_start_16:
        cmpwi r4,16
        blt L_L_end_16
        
        stvx v0,r5,r3
        subi r4,r4,16
        addi r5,r5,16
        
        b L_L_start_16
L_L_end_16:


L_start_4:
        cmpwi r4,4
        blt L_end_4
        
        stvewx v0,r5,r3
        subi r4,r4,4
        addi r5,r5,4
        
        b L_start_4
L_end_4:


L_start_1:
        cmpwi r4,1
        blt L_end_1
        
        stvebx v0,r5,r3
        subi r4,r4,1
        addi r5,r5,1
L_end_1:
        lwz r5,8(r30)
        lwz r4,4(r30)
        blr
