@ Стартап код для LPC2000

@ Размеры стека
.set FIQ_STACK_SIZE, 128
.set IRQ_STACK_SIZE, 512
.set SVR_STACK_SIZE, 128
.set ABT_STACK_SIZE, 128
.set UND_STACK_SIZE, 128
.set USR_STACK_SIZE, 4096

@ Биты PSR
@ Режимы работы процессора
.set MODE_USER,         0x10
.set MODE_FIQ,          0x11
.set MODE_IRQ,          0x12
.set MODE_SUPERVISOR,   0x13
.set MODE_ABORT,        0x17
.set MODE_UNDEFINED,    0x1B
.set MODE_SYSTEM,       0x1F

@ Биты разрешения IRQ и FIQ
.equ PSR_I_BIT, 0x80                            @ Когда I бит установлен, IRQ выключен
.equ PSR_F_BIT, 0x40                            @ Когда F бит установлен, FIQ выключен

.section .stack, "w"
.code 32
.align 4

.space  (USR_STACK_SIZE + 3) & ~3
.space  (FIQ_STACK_SIZE + 3) & ~3
.space  (IRQ_STACK_SIZE + 3) & ~3
.space  (SVR_STACK_SIZE + 3) & ~3
.space  (ABT_STACK_SIZE + 3) & ~3
.space  (UND_STACK_SIZE + 3) & ~3
_stack_end:

.text
.section .vect, "ax"
.code 32
.align 4

.global _vectors
.func _vectors
_vectors:
ldr     pc, _rst
ldr     pc, _und
ldr     pc, _swi
ldr     pc, _pabt
ldr     pc, _dabt
.word   0x00000000                              @ Контрольная сумма
_t_irq:
@ldr     pc, [pc, #-0x0120]                     @загрузка вектора из VIC в счетчик команд
ldr     pc, _irq                           @@
ldr     pc, _fiq

@ Таблица адресов обработчиков
_rst:
.word   _start

_und:
.word   UndHandler

_swi:
.word   _swi_prehandler

_pabt:
.word   PAbtHandler

_dabt:
.word   DAbtHandler

_irq:
.word   IRQHandler                              @@

_fiq:
.word   FIQHandler

.size _vectors, . - _vectors
.endfunc @ _vectors

.weak UndHandler
.global UndHandler
.func UndHandler
UndHandler:
b       .
.size UndHandler, . - UndHandler
.endfunc @ UndHandler

@ Обработка программного прерывания
.global _swi_prehandler
.func   _swi_prehandler

_swi_prehandler:
stmfd   sp!, {r0-r12, lr}     @ Сохраняем регистры на стеке
ldr     r0, [lr, #-4]         @ Загружаем команду swi в парметр функции
bic     r0, r0, #0xFF000000   @ Выделяем поле comment
bl      SWIHandler            @ Переходим на обработчик прерывания
ldmfd   sp!, {r0-r12, lr}     @ Восстанавливаем значения регистров
movs    pc, lr                @ Возвращаемся к точке вызова
.size   _swi_prehandler, . - _swi_prehandler
.endfunc @ _swi_prehandler

.weak SWIHandler
.global SWIHandler
.func SWIHandler
SWIHandler:
b       .
.size SWIHandler, . - SWIHandler
.endfunc @ SWIHandler

.weak PAbtHandler
.global PAbtHandler
.func PAbtHandler
PAbtHandler:
subs    pc, lr, #4
b       .
.size PAbtHandler, . - PAbtHandler
.endfunc @ PAbtHandler

.weak DAbtHandler
.global DAbtHandler
.func DAbtHandler
DAbtHandler:
subs    pc, lr, #8
@ b       .
.size DAbtHandler, . - DAbtHandler
.endfunc @ DAbtHandler


@@@@@@@@    обработка прерывания IRQ


.weak IRQHandler
.global IRQHandler
.func IRQHandler
.extern irq_func
.extern VICAddress

IRQHandler:
stmdb sp!, {r0-r11, ip, lr}                 /* Save r0-r11, ip, lr */
//ldr r0, = irq_func
mov r0,#0
ldr r0,[r0,#-256]                           /*загружаем вектор из VICAddress*/
mov lr, pc
bx r0                                       /* Use C interruption program*/
ldmia sp!, {r0-r11, ip, lr}                 /* Resume r0, ip, lr */
subs pc, r14, #4                            /* Interruption and return */

.size IRQHandler, . - IRQHandler
.endfunc                        @ IRQHandler

@@@@@@@@



.weak FIQHandler
.global FIQHandler
.func FIQHandler
FIQHandler:
b       .
.size FIQHandler, . - FIQHandler
.endfunc @ FIQHandler

.section .init, "ax"
.code 32
.align 4

.global _start
.func   _start

_start:

@ Инициализация стека и системы прерываний
ldr     R0, =_stack_end

@ Undefined Instruction Mode
msr     CPSR_c, #MODE_UNDEFINED | PSR_I_BIT | PSR_F_BIT
mov     SP, R0
sub     R0, R0, #UND_STACK_SIZE

@ Supervisor Mode
msr     CPSR_c, #MODE_SUPERVISOR | PSR_I_BIT | PSR_F_BIT
mov     SP, R0
sub     R0, R0, #SVR_STACK_SIZE

@ Abort Mode
msr     CPSR_c, #MODE_ABORT | PSR_I_BIT | PSR_F_BIT
mov     SP, R0
sub     R0, R0, #ABT_STACK_SIZE

@ IRQ Mode
msr     CPSR_c, #MODE_IRQ
mov     SP, R0
sub     R0, R0, #IRQ_STACK_SIZE

@ FIQ Mode
msr     CPSR_c, #MODE_FIQ | PSR_I_BIT | PSR_F_BIT
mov     SP, R0
sub     R0, R0, #FIQ_STACK_SIZE

@ System | User Mode
msr     CPSR_c, #MODE_USER
mov     SP, R0

@ Копирование инициализированных данных в RAM
ldr     R1, =_text_end
ldr     R2, =_data_start
ldr     R3, =_data_end

_data_cpy:
cmp     R2, R3
ldrlo   R0, [ R1 ], #4
strlo   R0, [ R2 ], #4
blo     _data_cpy

@ Очистка .bss
mov   R0, #0
ldr   R1, =_bss_start
ldr   R2, =_bss_end

_bss_clr:
cmp   R1, R2
strlo R0, [ R1 ], #4
blo   _bss_clr

@ Вызов C++ конструкторов для объектов в глобальном пространстве
ldr   R0, =_ctors_start
ldr   R1, =_ctors_end

_ctors_loop_start:
cmp   R0, R1
beq   _ctors_loop_end
ldr   R2, [ R0 ], #4
stmfd SP!, { R0 - R1 }
mov   LR, PC
mov   PC, R2
ldmfd SP!, { R0 - R1 }
b     _ctors_loop_start
_ctors_loop_end:

@ Инициализация процессора и переферии
bl      init

@ Вызов функции main
mov   R0, #0
mov   R1, R0
mov   R2, R0
mov   FP, R0
mov   R7, R0          @ FP = 0 для Thumb
ldr   R10, =main
mov   LR, PC
bx    R10
b     .

.size   _start, . - _start
.endfunc @ _start

.weak init
.global init
.func init
init:
bx    lr
.size init, . - init
.endfunc @ init

.end


