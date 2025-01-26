global sepia_filter_asm

%macro convert 2
    movzx eax, byte [%1]
    cvtsi2ss %2, eax
    shufps %2, %2, 0
%endmacro

section .rodata

r: dd 0.272, 0.349, 0.393, 0
g: dd 0.534, 0.686, 0.769, 0
b: dd 0.131, 0.168, 0.189, 0

section .text
sepia_filter_asm:
    mov bl, [rdi + 3]

    convert rdi, xmm0
    convert rdi + 1, xmm1
    convert rdi + 2, xmm2

    movups xmm3, [r]
    movups xmm4, [g]
    movups xmm5, [b]

    mulps xmm0, xmm3
    mulps xmm1, xmm4
    mulps xmm2, xmm5

    addps xmm0, xmm1
    addps xmm0, xmm2

    cvtps2dq xmm0, xmm0

    packssdw xmm0, xmm0
    packuswb xmm0, xmm0

    movd [rdi], xmm0
    mov [rdi + 3], bl
    ret