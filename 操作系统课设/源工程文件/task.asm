; ==========================================
; task1.asm
; 编译方法：nasm task1.asm -o task1.com
; ==========================================
;可以在LDT中增加更多的段，比如数据段，堆栈段，这样一来，我们可以把一个单独的任务所用到的所有东西封装在一个LDT中，
;这种思想是多任务处理的一个雏形
%include	"pm.inc"	; 常量, 宏, 以及一些说明

PageDirBase0		equ	200000h	; 页目录开始地址:	2M
PageTblBase0		equ	201000h	; 页表开始地址:		2M +  4K
PageDirBase1		equ	210000h	; 页目录开始地址:	2M + 64K
PageTblBase1		equ	211000h	; 页表开始地址:		2M + 64K + 4K
PageDirBase2		equ	220000h	; 页目录2
PageTblBase2		equ	221000h	; 页表2
PageDirBase3		equ	230000h	; 页目录3
PageTblBase3		equ	231000h	; 页表3


LinearAddrDemo	equ	00401000h
ProcHUST		equ	00401000h
ProcMRSU		equ	00501000h
ProcVERY		equ	00601000h
ProcLOVE		equ	00701000h
ProcPagingDemo	equ	00301000h

org	0100h
	jmp	LABEL_BEGIN

; GDT
[SECTION .gdt]
;                                  段基址,             段界限, 属性
LABEL_GDT:			Descriptor          0,                 0, 0                          ; 空描述符
LABEL_DESC_NORMAL:	Descriptor          0,            0ffffh, DA_DRW                     ; Normal 描述符
LABEL_DESC_FLAT_C:	Descriptor          0,           0fffffh, DA_CR | DA_32 | DA_LIMIT_4K; 0 ~ 4G
LABEL_DESC_FLAT_RW:	Descriptor          0,           0fffffh, DA_DRW | DA_LIMIT_4K       ; 0 ~ 4G
LABEL_DESC_CODE32:	Descriptor          0,  SegCode32Len - 1, DA_CR | DA_32              ; 非一致代码段, 32
LABEL_DESC_CODE16:	Descriptor          0,            0ffffh, DA_C                       ; 非一致代码段, 16
LABEL_DESC_DATA:	Descriptor          0,       DataLen - 1, DA_DRW                     ; Data
LABEL_DESC_STACK:	Descriptor          0,        TopOfStack, DA_DRWA | DA_32            ; Stack, 32 位
LABEL_DESC_VIDEO:	Descriptor    0B8000h,            0ffffh, DA_DRW                     ; 显存首地址
LABEL_DESC_LDT0:    Descriptor          0,         LDT0Len-1, DA_LDT                     ;LDT0
LABEL_DESC_LDT1:    Descriptor          0,         LDT1Len-1, DA_LDT                     ;LDT1
LABEL_DESC_LDT2:    Descriptor          0,         LDT2Len-1, DA_LDT                     ;LDT2
LABEL_DESC_LDT3:    Descriptor          0,         LDT3Len-1, DA_LDT                     ;LDT3
LABEL_DESC_TSS0:    Descriptor          0,         TSS0Len-1, DA_386TSS                  ;TSS0
LABEL_DESC_TSS1:    Descriptor          0,         TSS1Len-1, DA_386TSS                  ;TSS1
LABEL_DESC_TSS2:    Descriptor          0,         TSS2Len-1, DA_386TSS                  ;TSS2
LABEL_DESC_TSS3:    Descriptor          0,         TSS3Len-1, DA_386TSS                  ;TSS3
; GDT 结束

GdtLen  equ	$ - LABEL_GDT    ; GDT长度
GdtPtr  dw	GdtLen - 1       ; GDT界限
		dd	0                ; GDT基地址

; GDT 选择子
SelectorNormal		equ	LABEL_DESC_NORMAL	- LABEL_GDT
SelectorFlatC		equ	LABEL_DESC_FLAT_C	- LABEL_GDT
SelectorFlatRW		equ	LABEL_DESC_FLAT_RW	- LABEL_GDT
SelectorCode32		equ	LABEL_DESC_CODE32	- LABEL_GDT
SelectorCode16		equ	LABEL_DESC_CODE16	- LABEL_GDT
SelectorData		equ	LABEL_DESC_DATA		- LABEL_GDT
SelectorStack		equ	LABEL_DESC_STACK	- LABEL_GDT
SelectorVideo		equ	LABEL_DESC_VIDEO	- LABEL_GDT
SelectorLDT0		equ	LABEL_DESC_LDT0		- LABEL_GDT
SelectorLDT1		equ	LABEL_DESC_LDT1		- LABEL_GDT
SelectorLDT2		equ	LABEL_DESC_LDT2		- LABEL_GDT
SelectorLDT3		equ	LABEL_DESC_LDT3		- LABEL_GDT
SelectorTSS0		equ	LABEL_DESC_TSS0		- LABEL_GDT
SelectorTSS1		equ	LABEL_DESC_TSS1		- LABEL_GDT
SelectorTSS2		equ	LABEL_DESC_TSS2		- LABEL_GDT
SelectorTSS3		equ	LABEL_DESC_TSS3		- LABEL_GDT
; END of [SECTION .gdt]


; LDT0
[SECTION .ldt0]
ALIGN	32
LABEL_LDT0:
;                                     段基址,              段界限, 属性
LABEL_LDT0_DESC_CODEA:  Descriptor         0,       CodeALen - 1, DA_C + DA_32 		; CodeA, 32 位
LABEL_LDT0_DESC_STACK:  Descriptor         0,     TopOfStackLDT0, DA_DRW + DA_32	; Stack0, 32 位
LDT0Len		equ	$ - LABEL_LDT0

; LDT 选择子
SelectorLDT0CodeA   equ LABEL_LDT0_DESC_CODEA - LABEL_LDT0 + SA_TIL
SelectorLDT0Stack   equ LABEL_LDT0_DESC_STACK - LABEL_LDT0 + SA_TIL
; END of [SECTION .ldt0]


; LDT1
[SECTION .ldt1]
ALIGN	32
LABEL_LDT1:
;                                     段基址,              段界限, 属性
LABEL_LDT1_DESC_CODEB:  Descriptor         0,       CodeBLen - 1, DA_C + DA_32		; CodeB, 32 位
LABEL_LDT1_DESC_STACK:  Descriptor         0,     TopOfStackLDT1, DA_DRW + DA_32	; Stack1, 32 位
LDT1Len		equ	$ - LABEL_LDT1

; LDT 选择子
SelectorLDT1CodeB   equ LABEL_LDT1_DESC_CODEB - LABEL_LDT1 + SA_TIL
SelectorLDT1Stack   equ LABEL_LDT1_DESC_STACK - LABEL_LDT1 + SA_TIL
; END of [SECTION .ldt1]

; LDT2
[SECTION .ldt2]
ALIGN	32
LABEL_LDT2:
;                                     段基址,              段界限, 属性
LABEL_LDT2_DESC_CODEC:  Descriptor         0,       CodeCLen - 1, DA_C + DA_32		; CodeC, 32 位
LABEL_LDT2_DESC_STACK:  Descriptor         0,     TopOfStackLDT2, DA_DRW + DA_32	; Stack2, 32 位
LDT2Len		equ	$ - LABEL_LDT2

; LDT 选择子
SelectorLDT2CodeC   equ LABEL_LDT2_DESC_CODEC - LABEL_LDT2 + SA_TIL
SelectorLDT2Stack   equ LABEL_LDT2_DESC_STACK - LABEL_LDT2 + SA_TIL
; END of [SECTION .ldt2]

; LDT3
[SECTION .ldt3]
ALIGN	32
LABEL_LDT3:
;                                     段基址,              段界限, 属性
LABEL_LDT3_DESC_CODED:  Descriptor         0,       CodeDLen - 1, DA_C + DA_32		; CodeD, 32 位
LABEL_LDT3_DESC_STACK:  Descriptor         0,     TopOfStackLDT3, DA_DRW + DA_32	; Stack3, 32 位
LDT3Len		equ	$ - LABEL_LDT3

; LDT 选择子
SelectorLDT3CodeD   equ LABEL_LDT3_DESC_CODED - LABEL_LDT3 + SA_TIL
SelectorLDT3Stack   equ LABEL_LDT3_DESC_STACK - LABEL_LDT3 + SA_TIL
; END of [SECTION .ldt3]


[SECTION .data1]	 ; 数据段
ALIGN	32
[BITS	32]
LABEL_DATA:
; 实模式下使用这些符号
; 字符串
_szPMMessage:			db	"In Protect Mode now. ^-^", 0Ah, 0Ah, 0						; 进入保护模式后显示此字符串
_szMemChkTitle:			db	"BaseAddrL BaseAddrH LengthLow LengthHigh   Type", 0Ah, 0	; 进入保护模式后显示此字符串
_szRAMSize				db	"RAM size:", 0
_szReturn				db	0Ah, 0
; 变量
_wSPValueInRealMode		dw	0
_dwMCRNumber:			dd	0					; Memory Check Result
_dwDispPos:				dd	(80 * 6 + 0) * 2	; 屏幕第 6 行, 第 0 列。
_dwMemSize:				dd	0
_ARDStruct:										; Address Range Descriptor Structure
	_dwBaseAddrLow:		dd	0
	_dwBaseAddrHigh:	dd	0
	_dwLengthLow:		dd	0
	_dwLengthHigh:		dd	0
	_dwType:			dd	0
_PageTableNumber:		dd	0
_SavedIDTR:				dd	0					; 用于保存 IDTR
						dd	0
_SavedIMREG:			db	0					; 中断屏蔽寄存器值
_MemChkBuf:	times	256	db	0

_RunningTask				dd	5
_NextTask				dd  0
_Priority				dd  60, 40, 90,70
_time				dd  60, 40, 90,70

; 保护模式下使用这些符号
szPMMessage			equ	_szPMMessage	- $$
szMemChkTitle		equ	_szMemChkTitle	- $$
szRAMSize			equ	_szRAMSize	- $$
szReturn			equ	_szReturn	- $$
dwDispPos			equ	_dwDispPos	- $$
dwMemSize			equ	_dwMemSize	- $$
dwMCRNumber			equ	_dwMCRNumber	- $$
ARDStruct			equ	_ARDStruct	- $$
	dwBaseAddrLow	equ	_dwBaseAddrLow	- $$
	dwBaseAddrHigh	equ	_dwBaseAddrHigh	- $$
	dwLengthLow		equ	_dwLengthLow	- $$
	dwLengthHigh	equ	_dwLengthHigh	- $$
	dwType			equ	_dwType		- $$
MemChkBuf			equ	_MemChkBuf	- $$
SavedIDTR			equ	_SavedIDTR	- $$
SavedIMREG			equ	_SavedIMREG	- $$
PageTableNumber		equ	_PageTableNumber - $$
RunningTask				equ _RunningTask	- $$
Priority				equ _Priority - $$
time				equ _time - $$
NextTask 			equ _NextTask - $$
DataLen				equ	$ - LABEL_DATA
; END of [SECTION .data1]


; IDT
[SECTION .idt]
ALIGN	32
[BITS	32]
LABEL_IDT:
; 门                 目标选择子,            偏移, DCount, 属性
%rep 32
		Gate	SelectorCode32, SpuriousHandler,      0, DA_386IGate
%endrep
.020h:	Gate	SelectorCode32,    ClockHandler,      0, DA_386IGate
%rep 95
		Gate	SelectorCode32, SpuriousHandler,      0, DA_386IGate
%endrep
.080h:	Gate	SelectorCode32,  UserIntHandler,      0, DA_386IGate

IdtLen		equ	$ - LABEL_IDT
IdtPtr		dw	IdtLen - 1		; 段界限
			dd	0				; 基地址
; END of [SECTION .idt]


; 全局堆栈段
[SECTION .gs]
ALIGN	32
[BITS	32]
LABEL_STACK:
	times 512 db 0
TopOfStack	equ	$ - LABEL_STACK - 1
; END of [SECTION .gs]


; LDT0 堆栈段
[SECTION .ldt0s]
ALIGN	32
[BITS	32]
LABEL_LDT0_STACK:
	times 512 db 0
TopOfStackLDT0	equ	$ - LABEL_LDT0_STACK - 1
; END of [SECTION .ldt0s]


; LDT1 堆栈段
[SECTION .ldt1s]
ALIGN	32
[BITS	32]
LABEL_LDT1_STACK:
	times 512 db 0
TopOfStackLDT1	equ	$ - LABEL_LDT1_STACK - 1
; END of [SECTION .ldt1s]

; LDT2 堆栈段
[SECTION .ldt2s]
ALIGN	32
[BITS	32]
LABEL_LDT2_STACK:
	times 512 db 0
TopOfStackLDT2	equ	$ - LABEL_LDT2_STACK - 1
; END of [SECTION .ldt2s]

; LDT3 堆栈段
[SECTION .ldt3s]
ALIGN	32
[BITS	32]
LABEL_LDT3_STACK:
	times 512 db 0
TopOfStackLDT3	equ	$ - LABEL_LDT3_STACK - 1
; END of [SECTION .ldt3s]

; TSS0 ---------------------------------------------------------------------------------------------
;初始化任务状态堆栈段(TSS0)
[SECTION .tss0]         ;求得各段的大小
ALIGN	32              ;align是一个让数据对齐的宏。通常align的对象是1、4、8等。这里的align 32是没有意义的，因为本来就是只有32b的地址总线宽度。
[BITS	32]             ;32位模式的机器运行
LABEL_TSS0:             ;定义LABEL_TSS0
		DD	0					; Back
		DD	TopOfStack			; 0 级堆栈
		DD	SelectorStack		; 
		DD	0					; 1 级堆栈
		DD	0					; 
		DD	0					; 2 级堆栈
		DD	0					; 
		DD	PageDirBase0		; CR3
		DD	0					; EIP
		DD	200h				; EFLAGS
		DD	0					; EAX
		DD	0					; ECX
		DD	0					; EDX
		DD	0					; EBX
		DD	TopOfStackLDT0		; ESP
		DD	0					; EBP
		DD	0					; ESI
		DD	0					; EDI
		DD	0					; ES
		DD	SelectorLDT0CodeA	; CS
		DD	SelectorLDT0Stack	; SS
		DD	SelectorData		; DS
		DD	0					; FS
		DD	SelectorVideo		; GS
		DD	SelectorLDT0		; LDT
		DW	0					; 调试陷阱标志
		DW	$ - LABEL_TSS0 + 2	; I/O位图基址
		DB	0ffh				; I/O位图结束标志
TSS0Len		equ	$ - LABEL_TSS0  ;求得段的大小
; TSS0 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


; TSS1 ---------------------------------------------------------------------------------------------
;初始化任务状态堆栈段(TSS1)
[SECTION .tss1]         ;求得各段的大小
ALIGN	32              ;align是一个让数据对齐的宏。通常align的对象是1、4、8等。这里的align 32是没有意义的，因为本来就是只有32b的地址总线宽度。
[BITS	32]             ;32位模式的机器运行
LABEL_TSS1:             ;定义LABEL_TSS1
		DD	0					; Back
		DD	TopOfStack			; 0 级堆栈
		DD	SelectorStack		; 
		DD	0					; 1 级堆栈
		DD	0					; 
		DD	0					; 2 级堆栈
		DD	0					; 
		DD	PageDirBase1		; CR3
		DD	0					; EIP
		DD	200h				; EFLAGS
		DD	0					; EAX
		DD	0					; ECX
		DD	0					; EDX
		DD	0					; EBX
		DD	TopOfStackLDT1		; ESP
		DD	0					; EBP
		DD	0					; ESI
		DD	0					; EDI
		DD	0					; ES
		DD	SelectorLDT1CodeB	; CS
		DD	SelectorLDT1Stack	; SS
		DD	SelectorData		; DS
		DD	0					; FS
		DD	SelectorVideo		; GS
		DD	SelectorLDT1		; LDT
		DW	0					; 调试陷阱标志
		DW	$ - LABEL_TSS1 + 2	; I/O位图基址
		DB	0ffh				; I/O位图结束标志
TSS1Len		equ	$ - LABEL_TSS1  ;求得段的大小
; TSS1 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

; TSS2 ---------------------------------------------------------------------------------------------
;初始化任务状态堆栈段(TSS2)
[SECTION .tss2]         ;求得各段的大小
ALIGN	32              ;align是一个让数据对齐的宏。通常align的对象是1、4、8等。这里的align 32是没有意义的，因为本来就是只有32b的地址总线宽度。
[BITS	32]             ;32位模式的机器运行
LABEL_TSS2:             ;定义LABEL_TSS0
		DD	0					; Back
		DD	TopOfStack			; 0 级堆栈
		DD	SelectorStack		; 
		DD	0					; 1 级堆栈
		DD	0					; 
		DD	0					; 2 级堆栈
		DD	0					; 
		DD	PageDirBase2		; CR3
		DD	0					; EIP
		DD	200h				; EFLAGS
		DD	0					; EAX
		DD	0					; ECX
		DD	0					; EDX
		DD	0					; EBX
		DD	TopOfStackLDT2		; ESP
		DD	0					; EBP
		DD	0					; ESI
		DD	0					; EDI
		DD	0					; ES
		DD	SelectorLDT2CodeC	; CS
		DD	SelectorLDT2Stack	; SS
		DD	SelectorData		; DS
		DD	0					; FS
		DD	SelectorVideo		; GS
		DD	SelectorLDT2		; LDT
		DW	0					; 调试陷阱标志
		DW	$ - LABEL_TSS2 + 2	; I/O位图基址
		DB	0ffh				; I/O位图结束标志
TSS2Len		equ	$ - LABEL_TSS2  ;求得段的大小
; TSS0 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

; TSS3 ---------------------------------------------------------------------------------------------
;初始化任务状态堆栈段(TSS3)
[SECTION .tss3]         ;求得各段的大小
ALIGN	32              ;align是一个让数据对齐的宏。通常align的对象是1、4、8等。这里的align 32是没有意义的，因为本来就是只有32b的地址总线宽度。
[BITS	32]             ;32位模式的机器运行
LABEL_TSS3:             ;定义LABEL_TSS0
		DD	0					; Back
		DD	TopOfStack			; 0 级堆栈
		DD	SelectorStack		; 
		DD	0					; 1 级堆栈
		DD	0					; 
		DD	0					; 2 级堆栈
		DD	0					; 
		DD	PageDirBase3		; CR3
		DD	0					; EIP
		DD	200h				; EFLAGS
		DD	0					; EAX
		DD	0					; ECX
		DD	0					; EDX
		DD	0					; EBX
		DD	TopOfStackLDT3		; ESP
		DD	0					; EBP
		DD	0					; ESI
		DD	0					; EDI
		DD	0					; ES
		DD	SelectorLDT3CodeD	; CS
		DD	SelectorLDT3Stack	; SS
		DD	SelectorData		; DS
		DD	0					; FS
		DD	SelectorVideo		; GS
		DD	SelectorLDT3		; LDT
		DW	0					; 调试陷阱标志
		DW	$ - LABEL_TSS3 + 2	; I/O位图基址
		DB	0ffh				; I/O位图结束标志
TSS3Len		equ	$ - LABEL_TSS3  ;求得段的大小
; TSS0 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^




[SECTION .s16]
[BITS	16]
LABEL_BEGIN:
	mov	ax, cs
	mov	ds, ax
	mov	es, ax
	mov	ss, ax
	mov	sp, 0100h

	mov	[_wSPValueInRealMode], sp

	; 得到内存数
	mov	ebx, 0
	mov	di, _MemChkBuf
.loop:
	mov	eax, 0E820h
	mov	ecx, 20
	mov	edx, 0534D4150h
	int	15h
	jc	LABEL_MEM_CHK_FAIL
	add	di, 20
	inc	dword [_dwMCRNumber]
	cmp	ebx, 0
	jne	.loop
	jmp	LABEL_MEM_CHK_OK
LABEL_MEM_CHK_FAIL:
	mov	dword [_dwMCRNumber], 0
LABEL_MEM_CHK_OK:

	;初始化 32 位代码段描述符
	xor	eax, eax
	mov	ax, cs
	shl	eax, 4
	add	eax, LABEL_SEG_CODE32
	mov	word [LABEL_DESC_CODE32 + 2], ax
	shr	eax, 16
	mov	byte [LABEL_DESC_CODE32 + 4], al
	mov	byte [LABEL_DESC_CODE32 + 7], ah

	; 初始化数据段描述符
	xor	eax, eax
	mov	ax, ds
	shl	eax, 4
	add	eax, LABEL_DATA
	mov	word [LABEL_DESC_DATA + 2], ax
	shr	eax, 16
	mov	byte [LABEL_DESC_DATA + 4], al
	mov	byte [LABEL_DESC_DATA + 7], ah

	; 初始化堆栈段描述符
	xor	eax, eax
	mov	ax, ds
	shl	eax, 4
	add	eax, LABEL_STACK
	mov	word [LABEL_DESC_STACK + 2], ax
	shr	eax, 16
	mov	byte [LABEL_DESC_STACK + 4], al
	mov	byte [LABEL_DESC_STACK + 7], ah

	; 初始化 LDT0 堆栈段描述符
	xor	eax, eax
	mov	ax, ds
	shl	eax, 4
	add	eax, LABEL_LDT0_STACK
	mov	word [LABEL_LDT0_DESC_STACK + 2], ax
	shr	eax, 16
	mov	byte [LABEL_LDT0_DESC_STACK + 4], al
	mov	byte [LABEL_LDT0_DESC_STACK + 7], ah

	; 初始化 LDT0 在 GDT 中的描述符，LABEL_LDT0 为 LDT0 的定义地址
	xor	eax, eax
	mov	ax, ds
	shl	eax, 4
	add	eax, LABEL_LDT0
	mov	word [LABEL_DESC_LDT0 + 2], ax
	shr	eax, 16
	mov	byte [LABEL_DESC_LDT0 + 4], al
	mov	byte [LABEL_DESC_LDT0 + 7], ah

	; 初始化 LDT0 中的描述符，LABEL_CODE_A 才是真正的 LDT0 代码
	xor	eax, eax
	mov	ax, ds
	shl	eax, 4
	add	eax, LABEL_CODE_A
	mov	word [LABEL_LDT0_DESC_CODEA + 2], ax
	shr	eax, 16
	mov	byte [LABEL_LDT0_DESC_CODEA + 4], al
	mov	byte [LABEL_LDT0_DESC_CODEA + 7], ah
	
	; 初始化 TSS0 描述符
	xor	eax, eax
	mov	ax, ds
	shl	eax, 4
	add	eax, LABEL_TSS0
	mov	word [LABEL_DESC_TSS0 + 2], ax
	shr	eax, 16
	mov	byte [LABEL_DESC_TSS0 + 4], al
	mov	byte [LABEL_DESC_TSS0 + 7], ah
	
	; 初始化 LDT1 堆栈段描述符
	xor	eax, eax
	mov	ax, ds
	shl	eax, 4
	add	eax, LABEL_LDT1_STACK
	mov	word [LABEL_LDT1_DESC_STACK + 2], ax
	shr	eax, 16
	mov	byte [LABEL_LDT1_DESC_STACK + 4], al
    mov	byte [LABEL_LDT1_DESC_STACK + 7], ah
	
	; 初始化 LDT1 在 GDT 中的描述符	LABEL_STACK1 为 LDT1 的定义地址
	xor	eax, eax
	mov	ax, ds
	shl	eax, 4
	add	eax, LABEL_LDT1
	mov	word [LABEL_DESC_LDT1 + 2], ax
	shr	eax, 16
	mov	byte [LABEL_DESC_LDT1 + 4], al
	mov	byte [LABEL_DESC_LDT1 + 7], ah

	; 初始化 LDT1 中的描述符，LABEL_CODE_B 才是真正的 LDT1 代码
	xor	eax, eax
	mov	ax, ds
	shl	eax, 4
	add	eax, LABEL_CODE_B
	mov	word [LABEL_LDT1_DESC_CODEB + 2], ax
	shr	eax, 16
	mov	byte [LABEL_LDT1_DESC_CODEB + 4], al
	mov	byte [LABEL_LDT1_DESC_CODEB + 7], ah

	; 初始化 TSS1 描述符
	xor	eax, eax
	mov	ax, ds
	shl	eax, 4
	add	eax, LABEL_TSS1
	mov	word [LABEL_DESC_TSS1 + 2], ax
	shr	eax, 16
	mov	byte [LABEL_DESC_TSS1 + 4], al
	mov	byte [LABEL_DESC_TSS1 + 7], ah

	; 初始化 LDT2 堆栈段描述符
	xor	eax, eax
	mov	ax, ds
	shl	eax, 4
	add	eax, LABEL_LDT2_STACK
	mov	word [LABEL_LDT2_DESC_STACK + 2], ax
	shr	eax, 16
	mov	byte [LABEL_LDT2_DESC_STACK + 4], al
    mov	byte [LABEL_LDT2_DESC_STACK + 7], ah
	
	; 初始化 LDT2 在 GDT 中的描述符	LABEL_STACK2 为 LDT2 的定义地址
	xor	eax, eax
	mov	ax, ds
	shl	eax, 4
	add	eax, LABEL_LDT2
	mov	word [LABEL_DESC_LDT2 + 2], ax
	shr	eax, 16
	mov	byte [LABEL_DESC_LDT2 + 4], al
	mov	byte [LABEL_DESC_LDT2 + 7], ah

	; 初始化 LDT2 中的描述符，LABEL_CODE_C 才是真正的 LDT2 代码
	xor	eax, eax
	mov	ax, ds
	shl	eax, 4
	add	eax, LABEL_CODE_C
	mov	word [LABEL_LDT2_DESC_CODEC + 2], ax
	shr	eax, 16
	mov	byte [LABEL_LDT2_DESC_CODEC + 4], al
	mov	byte [LABEL_LDT2_DESC_CODEC + 7], ah

	; 初始化 TSS2 描述符
	xor	eax, eax
	mov	ax, ds
	shl	eax, 4
	add	eax, LABEL_TSS2
	mov	word [LABEL_DESC_TSS2 + 2], ax
	shr	eax, 16
	mov	byte [LABEL_DESC_TSS2 + 4], al
	mov	byte [LABEL_DESC_TSS2 + 7], ah

; 初始化 LDT3 堆栈段描述符
	xor	eax, eax
	mov	ax, ds
	shl	eax, 4
	add	eax, LABEL_LDT3_STACK
	mov	word [LABEL_LDT3_DESC_STACK + 2], ax
	shr	eax, 16
	mov	byte [LABEL_LDT3_DESC_STACK + 4], al
    mov	byte [LABEL_LDT3_DESC_STACK + 7], ah
	
	; 初始化 LDT3 在 GDT 中的描述符	LABEL_STACK3 为 LDT3 的定义地址
	xor	eax, eax
	mov	ax, ds
	shl	eax, 4
	add	eax, LABEL_LDT3
	mov	word [LABEL_DESC_LDT3 + 2], ax
	shr	eax, 16
	mov	byte [LABEL_DESC_LDT3 + 4], al
	mov	byte [LABEL_DESC_LDT3 + 7], ah

	; 初始化 LDT3 中的描述符，LABEL_CODE_D 才是真正的 LDT3 代码
	xor	eax, eax
	mov	ax, ds
	shl	eax, 4
	add	eax, LABEL_CODE_D
	mov	word [LABEL_LDT3_DESC_CODED + 2], ax
	shr	eax, 16
	mov	byte [LABEL_LDT3_DESC_CODED + 4], al
	mov	byte [LABEL_LDT3_DESC_CODED + 7], ah

	; 初始化 TSS3 描述符
	xor	eax, eax
	mov	ax, ds
	shl	eax, 4
	add	eax, LABEL_TSS3
	mov	word [LABEL_DESC_TSS3 + 2], ax
	shr	eax, 16
	mov	byte [LABEL_DESC_TSS3 + 4], al
	mov	byte [LABEL_DESC_TSS3 + 7], ah


	; 为加载 GDTR 作准备
	xor	eax, eax
	mov	ax, ds
	shl	eax, 4
	add	eax, LABEL_GDT		; eax <- gdt 基地址
	mov	dword [GdtPtr + 2], eax	; [GdtPtr + 2] <- gdt 基地址

	; 为加载 IDTR 作准备
	xor	eax, eax
	mov	ax, ds
	shl	eax, 4
	add	eax, LABEL_IDT		; eax <- idt 基地址
	mov	dword [IdtPtr + 2], eax	; [IdtPtr + 2] <- idt 基地址

	; 保存 IDTR
	sidt	[_SavedIDTR]

	; 保存中断屏蔽寄存器(IMREG)值
	in	al, 21h
	mov	[_SavedIMREG], al

	; 加载 GDTR
	lgdt	[GdtPtr]
	; 关中断
	;cli
	; 加载 IDTR
	lidt	[IdtPtr]
	; 打开地址线A20
	in	al, 92h
	or	al, 00000010b
	out	92h, al
	; 准备切换到保护模式
	mov	eax, cr0
	or	eax, 1
	mov	cr0, eax
	; 真正进入保护模式
	jmp	dword SelectorCode32:0	; 执行这一句会把 SelectorCode32 装入 cs, 并跳转到 Code32Selector:0  处
; END of [SECTION .s16]


[SECTION .s32]; 32 位代码段. 由实模式跳入.
[BITS	32]

LABEL_SEG_CODE32:
	mov	ax, SelectorData
	mov	ds, ax			; 数据段选择子
	mov	es, ax
	mov	ax, SelectorVideo
	mov	gs, ax			; 视频段选择子
	mov	ax, SelectorStack
	mov	ss, ax			; 堆栈段选择子

	mov	esp, TopOfStack

	xor eax, eax 
    mov dword [RunningTask], eax

	call	Init8259A

	; 下面显示一个字符串
	push	szPMMessage
	call	DispStr
	add	esp, 4

	push	szMemChkTitle
	call	DispStr
	add	esp, 4

	call	DispMemSize		; 显示内存信息
	call	PagingDemo		; 分页

	
	int	080h
	sti

	mov ax, SelectorTSS0
	ltr ax
	mov ax, SelectorLDT0
	lldt ax
	
	push SelectorLDT0Stack
	push TopOfStackLDT0
	pushf ;默认的操作对象是：所有的标志寄存器
	push SelectorLDT0CodeA
	push 0
	iret

	jmp	$

; Init8259A ---------------------------------------------------------------------------------------------
Init8259A:
	mov	al, 011h
	out	020h, al	; 主8259, ICW1.
	call	io_delay

	out	0A0h, al	; 从8259, ICW1.
	call	io_delay

	mov	al, 020h	; IRQ0 对应中断向量 0x20
	out	021h, al	; 主8259, ICW2.
	call	io_delay

	mov	al, 028h	; IRQ8 对应中断向量 0x28
	out	0A1h, al	; 从8259, ICW2.
	call	io_delay

	mov	al, 004h	; IR2 对应从8259
	out	021h, al	; 主8259, ICW3.
	call	io_delay

	mov	al, 002h	; 对应主8259的 IR2
	out	0A1h, al	; 从8259, ICW3.
	call	io_delay

	mov	al, 001h
	out	021h, al	; 主8259, ICW4.
	call	io_delay

	out	0A1h, al	; 从8259, ICW4.
	call	io_delay

	mov	al, 11111110b	; 仅仅开启定时器中断
    ;mov	al, 11111111b	; 屏蔽主8259所有中断
	out	021h, al	; 主8259, OCW1.
	call	io_delay

	mov	al, 11111111b	; 屏蔽从8259所有中断
	out	0A1h, al	; 从8259, OCW1.
	call	io_delay

	ret
; Init8259A ---------------------------------------------------------------------------------------------

io_delay:
	nop
	nop
	nop
	nop
	ret

; int handler ---------------------------------------------------------------
_ClockHandler:
ClockHandler	equ	_ClockHandler - $$

	push gs                   
	push ds
	pushad                                ;保存所有通用寄存器的值

	mov	al, 20h
	out	20h, al				; 发送 EOI（End Of Interrupt）信号给中断控制器，表示中断已经处理完毕


	mov ebx,[RunningTask]
	cmp ebx,5
	je start
	mov ebx,[time+ebx*4]
	cmp ebx,0
	je start
	mov ebx,[RunningTask]
	dec dword [time + ebx*4]
	jmp Final

start:
	mov ebx,0
	mov ecx, 4
.1:                               ;循环四次找到时间片不为0其最多的任务，将序号存入NextTask,时间片存入ebx
	mov edx, ecx
	dec edx
	cmp ebx, dword [time+edx*4]
	jg .2
	mov ebx, dword [time+edx*4]
	mov dword [NextTask], edx
.2:
	loop .1

	cmp ebx, 0
	je .reset
	jmp .3
	
.reset:
	mov dword[RunningTask], 5
	mov dword[NextTask], 0
	mov eax, dword [Priority]
	mov dword[time], eax
	mov eax, dword [Priority + 4]
	mov dword[time + 4], eax
	mov eax, dword [Priority + 8]
	mov dword[time + 8], eax
	mov eax, dword [Priority + 12]
	mov dword[time + 12], eax
	jmp Final
	
.3:
	mov eax, dword [NextTask]  
	cmp eax,3
	je Task3
	cmp eax, 2
	je Task2
	cmp eax, 1
	je Task1
	cmp eax, 0
	je Task0
	
Task0:
	mov ebx, dword [NextTask]
	mov dword [RunningTask], ebx
	jmp SelectorTSS0:0
	dec dword [time + eax*4]
	jmp Final


Task1:
	mov ebx, dword [NextTask]
	mov dword [RunningTask], ebx
	jmp SelectorTSS1:0
	dec dword [time + eax*4]
	jmp Final

		
Task2:
	mov ebx, dword [NextTask]
	mov dword [RunningTask], ebx
	jmp SelectorTSS2:0
	dec dword [time + eax*4]
	jmp Final

Task3:
	mov ebx, dword [NextTask]
	mov dword [RunningTask], ebx
	jmp SelectorTSS3:0
	dec dword [time + eax*4]

Final:
	popad
	pop ds
	pop gs
	iretd

_UserIntHandler:
UserIntHandler	equ	_UserIntHandler - $$
	mov	ah, 0Ch				; 0000: 黑底    1100: 红字
	mov	al, 'H'
	mov	[gs:((80 * 4 + 0) * 2)], ax	; 屏幕第 4 行, 第 0 列。
	mov	al, 'Y'
	mov	[gs:((80 * 4 + 1) * 2)], ax	; 屏幕第 4 行, 第 1 列。
	iretd

_SpuriousHandler:
SpuriousHandler	equ	_SpuriousHandler - $$
	nop
    nop
	nop
    nop
	iretd
; ---------------------------------------------------------------------------

; 测试分页机制 --------------------------------------------------------------
PagingDemo:
	mov	ax, cs
	mov	ds, ax
	mov	ax, SelectorFlatRW
	mov	es, ax

	push	LenHUST
	push	OffsetHUST
	push	ProcHUST
	call	MemCpy
	add	esp, 12

	push	LenMRSU
	push	OffsetMRSU
	push	ProcMRSU
	call	MemCpy
	add	esp, 12
	
	push	LenLOVE
	push	OffsetLOVE
	push	ProcLOVE
	call	MemCpy
	add	esp, 12
	
	push	LenVERY
	push	OffsetVERY
	push	ProcVERY
	call	MemCpy
	add	esp, 12

	push	LenPagingDemoAll
	push	OffsetPagingDemoProc
	push	ProcPagingDemo
	call	MemCpy
	add	esp, 12

	mov	ax, SelectorData
	mov	ds, ax			; 数据段选择子
	mov	es, ax

	call	SetupPaging		; 启动分页
	;call	PSwitch

	ret
; ---------------------------------------------------------------------------

; 启动分页机制 --------------------------------------------------------------
SetupPaging:
	; 根据内存大小计算应初始化多少PDE以及多少页表
	xor	edx, edx
	mov	eax, [dwMemSize]
	mov	ebx, 400000h	; 400000h = 4M = 4096 * 1024, 一个页表对应的内存大小
	div	ebx
	mov	ecx, eax	; 此时 ecx 为页表的个数，也即 PDE 应该的个数
	test	edx, edx
	jz	.no_remainder
	inc	ecx		; 如果余数不为 0 就需增加一个页表
.no_remainder:
	mov	[PageTableNumber], ecx	; 暂存页表个数

	; 为简化处理, 所有线性地址对应相等的物理地址. 并且不考虑内存空洞.

	; 首先初始化页目录----------------
    ;task0
	mov	ax, SelectorFlatRW
	mov	es, ax
	mov	edi, PageDirBase0	; 此段首地址为 PageDirBase
	xor	eax, eax
	mov	eax, PageTblBase0 | PG_P  | PG_USU | PG_RWW
.0:
	stosd
	add	eax, 4096		; 为了简化, 所有页表在内存中是连续的.
	loop	.0

    ;task1
    mov	edi, PageDirBase1	; 此段首地址为 PageDirBase
	xor	eax, eax
	mov	eax, PageTblBase1 | PG_P  | PG_USU | PG_RWW
	mov	ecx, [PageTableNumber]
.1:
	stosd
	add	eax, 4096		; 为了简化, 所有页表在内存中是连续的.
	loop	.1

    ;task2
    mov	edi, PageDirBase2	; 此段首地址为 PageDirBase
	xor	eax, eax
	mov	eax, PageTblBase2 | PG_P  | PG_USU | PG_RWW
	mov	ecx, [PageTableNumber]
.2:
	stosd
	add	eax, 4096		; 为了简化, 所有页表在内存中是连续的.
	loop	.2

    ;task3
    mov	edi, PageDirBase3	; 此段首地址为 PageDirBase
	xor	eax, eax
	mov	eax, PageTblBase3 | PG_P  | PG_USU | PG_RWW
	mov	ecx, [PageTableNumber]
.3:
	stosd
	add	eax, 4096		; 为了简化, 所有页表在内存中是连续的.
	loop	.3

	; 再初始化所有页表----------------
    ;task0
	mov	eax, [PageTableNumber]	; 页表个数
	mov	ebx, 1024		; 每个页表 1024 个 PTE
	mul	ebx
	mov	ecx, eax		; PTE个数 = 页表个数 * 1024
	mov	edi, PageTblBase0	; 此段首地址为 PageTblBase
	xor	eax, eax
	mov	eax, PG_P  | PG_USU | PG_RWW
.4:
	stosd
	add	eax, 4096		; 每一页指向 4K 的空间
	loop	.4

    ;task1
    mov	eax, [PageTableNumber]	; 页表个数
	mov	ebx, 1024		; 每个页表 1024 个 PTE
	mul	ebx
	mov	ecx, eax		; PTE个数 = 页表个数 * 1024
	mov	edi, PageTblBase1	; 此段首地址为 PageTblBase
	xor	eax, eax
	mov	eax, PG_P  | PG_USU | PG_RWW
.5:
	stosd
	add	eax, 4096		; 每一页指向 4K 的空间
	loop	.5

    ;task2
	mov	eax, [PageTableNumber]	; 页表个数
	mov	ebx, 1024		; 每个页表 1024 个 PTE
	mul	ebx
	mov	ecx, eax		; PTE个数 = 页表个数 * 1024
	mov	edi, PageTblBase2	; 此段首地址为 PageTblBase
	xor	eax, eax
	mov	eax, PG_P  | PG_USU | PG_RWW
.6:
	stosd
	add	eax, 4096		; 每一页指向 4K 的空间
	loop	.6

    ;task3
    mov	eax, [PageTableNumber]	; 页表个数
	mov	ebx, 1024		; 每个页表 1024 个 PTE
	mul	ebx
	mov	ecx, eax		; PTE个数 = 页表个数 * 1024
	mov	edi, PageTblBase3	; 此段首地址为 PageTblBase
	xor	eax, eax
	mov	eax, PG_P  | PG_USU | PG_RWW
.7:
	stosd
	add	eax, 4096		; 每一页指向 4K 的空间
	loop	.7

	mov	eax, LinearAddrDemo
	shr	eax, 22
	mov	ebx, 4096
	mul	ebx
	mov	ecx, eax
	mov	eax, LinearAddrDemo
	shr	eax, 12
	and	eax, 03FFh	; 1111111111b (10 bits)
	mov	ebx, 4
	mul	ebx
	add	eax, ecx	;计算出待修改页表项的偏移
	mov ebx, eax

	add	eax, PageTblBase1
	mov	dword [es:eax], ProcMRSU | PG_P | PG_USU | PG_RWW
	mov eax, ebx
	add	eax, PageTblBase2
	mov	dword [es:eax], ProcVERY | PG_P | PG_USU | PG_RWW
	mov eax, ebx
	add	eax, PageTblBase3
	mov	dword [es:eax], ProcLOVE | PG_P | PG_USU | PG_RWW

	;开始先加载任务0的页表
	mov	eax, PageDirBase0
	mov	cr3, eax
	mov	eax, cr0
	or	eax, 80000000h
	mov	cr0, eax
	jmp	short .8
.8:
	nop

	ret
; 分页机制启动完毕 ----------------------------------------------------------


; 切换页表 ------------------------------------------------------------------
PSwitch:
	; 初始化页目录
	mov	ax, SelectorFlatRW
	mov	es, ax
	mov	edi, PageDirBase1	; 此段首地址为 PageDirBase
	xor	eax, eax
	mov	eax, PageTblBase1 | PG_P  | PG_USU | PG_RWW
	mov	ecx, [PageTableNumber]
.1:
	stosd
	add	eax, 4096		; 为了简化, 所有页表在内存中是连续的.
	loop	.1

	; 再初始化所有页表
	mov	eax, [PageTableNumber]	; 页表个数
	mov	ebx, 1024		; 每个页表 1024 个 PTE
	mul	ebx
	mov	ecx, eax		; PTE个数 = 页表个数 * 1024
	mov	edi, PageTblBase1	; 此段首地址为 PageTblBase
	xor	eax, eax
	mov	eax, PG_P  | PG_USU | PG_RWW
.2:
	stosd
	add	eax, 4096		; 每一页指向 4K 的空间
	loop	.2

	; 在此假设内存是大于 8M 的
	mov	eax, LinearAddrDemo
	shr	eax, 22
	mov	ebx, 4096
	mul	ebx
	mov	ecx, eax
	mov	eax, LinearAddrDemo
	shr	eax, 12
	and	eax, 03FFh	; 1111111111b (10 bits)
	mov	ebx, 4
	mul	ebx
	add	eax, ecx
	add	eax, PageTblBase1
	mov	dword [es:eax], ProcMRSU | PG_P | PG_USU | PG_RWW

	mov	eax, PageDirBase1
	mov	cr3, eax
	jmp	short .3
.3:
	nop

	ret
; ---------------------------------------------------------------------------

; PagingDemoProc ------------------------------------------------------------
PagingDemoProc:
OffsetPagingDemoProc	equ	PagingDemoProc - $$
	mov	eax, LinearAddrDemo
	call	eax
	retf
LenPagingDemoAll	equ	$ - PagingDemoProc
; ---------------------------------------------------------------------------

; HUST ----------------------------------------------------------------------
HUST:
OffsetHUST	equ	HUST - $$
	mov	ah, 0Ch				; 0000: 黑底    1100: 红字
	mov	al, 'H'
	mov	[gs:((80 * 18 + 0) * 2)], ax	; 屏幕第 18 行, 第 0 列。
	mov	al, 'U'
	mov	[gs:((80 * 18 + 1) * 2)], ax	; 屏幕第 18 行, 第 1 列。
	mov	al, 'S'
	mov	[gs:((80 * 18 + 2) * 2)], ax	; 屏幕第 18 行, 第 2 列。
	mov	al, 'T'
	mov	[gs:((80 * 18 + 3) * 2)], ax	; 屏幕第 18 行, 第 3 列。
	ret
LenHUST	equ	$ - HUST
; ---------------------------------------------------------------------------

; MRSU ----------------------------------------------------------------------
MRSU:
OffsetMRSU	equ	MRSU - $$
	mov	ah, 0Ch				; 0000: 黑底    1100: 红字
	mov	al, 'M'
	mov	[gs:((80 * 18 + 0) * 2)], ax	; 屏幕第 18 行, 第 0 列。
	mov	al, 'R'
	mov	[gs:((80 * 18 + 1) * 2)], ax	; 屏幕第 18 行, 第 1 列。
	mov	al, 'S'
	mov	[gs:((80 * 18 + 2) * 2)], ax	; 屏幕第 18 行, 第 2 列。
	mov	al, 'U'
	mov	[gs:((80 * 18 + 3) * 2)], ax	; 屏幕第 18 行, 第 3 列。
	ret
LenMRSU	equ	$ - MRSU
; ---------------------------------------------------------------------------

; VERY ----------------------------------------------------------------------
VERY:
OffsetVERY	equ	VERY - $$
	mov	ah, 0Ch				; 0000: 黑底    1100: 红字
	mov	al, 'V'
	mov	[gs:((80 * 18 + 0) * 2)], ax	; 屏幕第 18 行, 第 0 列。
	mov	al, 'E'
	mov	[gs:((80 * 18 + 1) * 2)], ax	; 屏幕第 18 行, 第 1 列。
	mov	al, 'R'
	mov	[gs:((80 * 18 + 2) * 2)], ax	; 屏幕第 18 行, 第 2 列。
	mov	al, 'Y'
	mov	[gs:((80 * 18 + 3) * 2)], ax	; 屏幕第 18 行, 第 3 列。
	ret
LenVERY	equ	$ - VERY
; ---------------------------------------------------------------------------

; LOVE ----------------------------------------------------------------------
LOVE:
OffsetLOVE	equ	LOVE - $$
	mov	ah, 0Ch				; 0000: 黑底    1100: 红字
	mov	al, 'L'
	mov	[gs:((80 * 18 + 0) * 2)], ax	; 屏幕第 18 行, 第 0 列。
	mov	al, 'O'
	mov	[gs:((80 * 18 + 1) * 2)], ax	; 屏幕第 18 行, 第 1 列。
	mov	al, 'V'
	mov	[gs:((80 * 18 + 2) * 2)], ax	; 屏幕第 18 行, 第 2 列。
	mov	al, 'E'
	mov	[gs:((80 * 18 + 3) * 2)], ax	; 屏幕第 18 行, 第 3 列。
	ret
LenLOVE	equ	$ - LOVE
; ---------------------------------------------------------------------------



; 显示内存信息 --------------------------------------------------------------
DispMemSize:
	push	esi
	push	edi
	push	ecx

	mov	esi, MemChkBuf
	mov	ecx, [dwMCRNumber]		;for(int i=0;i<[MCRNumber];i++) // 每次得到一个ARDS(Address Range Descriptor Structure)结构
.loop:							;{
	mov	edx, 5					;	for(int j=0;j<5;j++)	// 每次得到一个ARDS中的成员，共5个成员
	mov	edi, ARDStruct			;	{			// 依次显示：BaseAddrLow，BaseAddrHigh，LengthLow，LengthHigh，Type
.1:								;
	push	dword [esi]			;
	call	DispInt				;		DispInt(MemChkBuf[j*4]); // 显示一个成员
	pop	eax						;
	stosd						;		ARDStruct[j*4] = MemChkBuf[j*4];
	add	esi, 4					;
	dec	edx						;
	cmp	edx, 0					;
	jnz	.1						;	}
	call	DispReturn			;	printf("\n");
	cmp	dword [dwType], 1		;	if(Type == AddressRangeMemory) // AddressRangeMemory : 1, AddressRangeReserved : 2
	jne	.2						;	{
	mov	eax, [dwBaseAddrLow]	;
	add	eax, [dwLengthLow]		;
	cmp	eax, [dwMemSize]		;		if(BaseAddrLow + LengthLow > MemSize)
	jb	.2						;
	mov	[dwMemSize], eax		;			MemSize = BaseAddrLow + LengthLow;
.2:								;	}
	loop	.loop				;}
								;
	call	DispReturn			;printf("\n");
	push	szRAMSize			;
	call	DispStr				;printf("RAM size:");
	add	esp, 4					;
								;
	push	dword [dwMemSize]	;
	call	DispInt				;DispInt(MemSize);
	add	esp, 4

	pop	ecx
	pop	edi
	pop	esi
	ret
; ---------------------------------------------------------------------------

%include	"lib.inc"	; 库函数

SegCode32Len	equ	$ - LABEL_SEG_CODE32
; END of [SECTION .s32]


; CodeA (LDT0, 32 位代码段)
[SECTION .la]
ALIGN	32
[BITS	32]
LABEL_CODE_A:
	call SelectorFlatC:ProcPagingDemo		; 显示 HUST
	jmp LABEL_CODE_A
CodeALen	equ	$ - LABEL_CODE_A
; END of [SECTION .la]


; CodeB (LDT1, 32 位代码段)
[SECTION .lb]
ALIGN	32
[BITS	32]
LABEL_CODE_B:
	call SelectorFlatC:ProcPagingDemo		; 显示 MRSU
	jmp LABEL_CODE_B
CodeBLen	equ	$ - LABEL_CODE_B
; END of [SECTION .lb]

; CodeC (LDT2, 32 位代码段)
[SECTION .lc]
ALIGN	32
[BITS	32]
LABEL_CODE_C:
	call SelectorFlatC:ProcPagingDemo		; 
	jmp LABEL_CODE_C
CodeCLen	equ	$ - LABEL_CODE_C
; END of [SECTION .lc]


; CodeA (LDT3, 32 位代码段)
[SECTION .ld]
ALIGN	32
[BITS	32]
LABEL_CODE_D:
	call SelectorFlatC:ProcPagingDemo		;
	jmp LABEL_CODE_D
CodeDLen	equ	$ - LABEL_CODE_D
; END of [SECTION .ld]


