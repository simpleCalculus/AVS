;Narzullozoda S BPI194
;Нарзуллозода Сахижони Зафар БПИ194
;Вариант 14
;Разработать программу, которая вводит одномерный массив A[N], 
;формирует из элементов массива A новый массив B с заменой всех 
;отрицательных элементов значением максимального

format PE console
entry start

include 'win32a.inc'

;--------------------------------------------------------------------------
section '.data' data readable writable

        strVecSize   db 'enter the size of the array A ', 10, 0
        strIncorSize db 'incorrect value', 10, 0
        strVecA db 10, 'vector A:', 10, 0
        strVecB db 10, 'vector B:', 10, 0
        strVecElemI  db 'A[%d] = ', 0
        strScanInt   db '%d', 0
        strMaxValue  db 10, 'max element in vector A is %d', 10, 0
        strVecElemOut  db '[%d] = %d', 10, 0
        strVecAi db 'A[%d] = %d', 10, 0
        strVecBi db 'B[%d] = %d', 10, 0
        strVecAo db '%d, ', 0
        strClose db ']', 10, 0
        strOpen db '[', 0
        strEnd db 10, 'the program ended successfully !!!', 10, 0

        vec_size     dd 0
        max          dd 0
        i            dd ?
        tmp          dd ?
        bel          dd ?
        tva          dd ?
        tvb          dd ?
        tmpStack     dd ?
        vec          rd 50
        vecb         rd 50

;--------------------------------------------------------------------------
section '.code' code readable executable
start:
; 1) input vector A
        call VectorInput
; 2) get max in vector A
        call VectorMax
; 3) output max value
        push [max]
        push strMaxValue
        call [printf]
; 4) create vector B
        call CreateVectorB
; 5) output the vector A
        push strVecA
        call [printf]
        mov ebx, vec
        call VectorAOut
; 5) output the vector B
        push strVecB
        call [printf]
        mov ebx, vecb
        call VectorBOut
        push strEnd
        call [printf]
finish:
        call [getch]

        push 0
        call [ExitProcess]

;--------------------------------------------------------------------------
VectorInput:
        push strVecSize
        call [printf]
        add esp, 4

        push vec_size
        push strScanInt
        call [scanf]
        cmp eax, 1
        jne error
        add esp, 8

        mov eax, [vec_size]
        cmp eax, 0
        jle error
        cmp eax, 50
        jg  error
        jmp getVector
; fail size
error:
        push [vec_size]
        push strIncorSize
        call [printf]
        jmp finish
; else continue...
getVector:
        xor ecx, ecx            ; ecx = 0
        mov ebx, vec            ; ebx = &vec
getVecLoop:
        mov [tmp], ebx
        cmp ecx, [vec_size]
        jge endInputVector       ; to end of loop

        ; input element
        mov [i], ecx
        push ecx
        push strVecElemI
        call [printf]
        add esp, 8

        push ebx
        push strScanInt
        call [scanf]
        cmp eax, 1
        jne error
        add esp, 8

        mov ecx, [i]
        inc ecx
        mov ebx, [tmp]
        add ebx, 4
        jmp getVecLoop
endInputVector:
        ret
;--------------------------------------------------------------------------
VectorMax:
        xor ecx, ecx            ; ecx = 0
        mov ebx, vec
        mov eax, [ebx]
        mov [max], eax
        jmp oldMax
maxVecLoop:
        cmp ecx, [vec_size]
        je endMaxVector      ; to end of loop
        mov eax, [ebx]
        cmp [max], eax
        jge oldMax
        mov [max], eax

oldMax:
        inc ecx
        add ebx, 4
        jmp maxVecLoop
endMaxVector:
        ret
;--------------------------------------------------------------------------
CreateVectorB:
        xor ecx, ecx            ; ecx = 0
        mov ebx, vec
        mov [tva], ebx
        mov ebx, vecb
        mov [tvb], ebx
        mov ebx, 0
createVecLoop:
        mov [tmp], ebx
        cmp ecx, [vec_size]
        jge endCreateVector       ; to end of loop

        ; input element
        mov [i], ecx

        add ebx, vec
        mov eax, [ebx]
        mov [bel], eax
        cmp eax, 0
        jge notNegative
        mov eax, [max]
        mov [bel], eax

notNegative:
        mov ebx, [tmp]
        add ebx, vecb
        mov eax, [bel]
        mov [ebx], eax

        mov ecx, [i]
        inc ecx
        mov ebx, [tmp]
        add ebx, 4
        jmp createVecLoop
endCreateVector:
        ret
;--------------------------------------------------------------------------
VectorBOut:
        mov [tmpStack], esp
        xor ecx, ecx            ; ecx = 0
outVecLoop:
        mov [tmp], ebx
        cmp ecx, [vec_size]
        je endOutputVector      ; to end of loop
        mov [i], ecx

        ; output element
        push dword [ebx]
        push ecx
        push strVecBi
        call [printf]

        mov ecx, [i]
        inc ecx
        mov ebx, [tmp]
        add ebx, 4
        jmp outVecLoop
endOutputVector:
        mov esp, [tmpStack]
        ret

VectorAOut:
        mov [tmpStack], esp
        xor ecx, ecx            ; ecx = 0
outVecLoop2:
        mov [tmp], ebx
        cmp ecx, [vec_size]
        je endOutputVector      ; to end of loop
        mov [i], ecx

        ; output element
        push dword [ebx]
        push ecx
        push strVecAi
        call [printf]

        mov ecx, [i]
        inc ecx
        mov ebx, [tmp]
        add ebx, 4
        jmp outVecLoop2
endOutputVector2:
        mov esp, [tmpStack]
        ret

;-------------------------------third act - including HeapApi--------------------------
                                                 
section '.idata' import data readable
    library kernel, 'kernel32.dll',\
            msvcrt, 'msvcrt.dll',\
            user32,'USER32.DLL'

include 'api\user32.inc'
include 'api\kernel32.inc'
    import kernel,\
           ExitProcess, 'ExitProcess',\
           HeapCreate,'HeapCreate',\
           HeapAlloc,'HeapAlloc'
  include 'api\kernel32.inc'
    import msvcrt,\
           printf, 'printf',\
           scanf, 'scanf',\
           getch, '_getch'
