.386

_text   segment dword public 'code'
        assume  cs:_text

_FixedMul proc near
        public _FixedMul
        align 4

        push ebp
        mov  ebp,esp
        mov  eax,[ebp+8]
        imul dword ptr [ebp+12]
        shrd eax,edx,16
        pop  ebp
        ret
        endp

_FixedDiv2 proc near
        public _FixedDiv2
        align 4

        push ebp
        mov  ebp,esp
        mov  eax,[ebp+8]
        cdq
        shld edx,eax,16
        sal  eax,16
        idiv dword ptr [ebp+12]
        pop  ebp
        ret
        endp

_text   ends
        end
