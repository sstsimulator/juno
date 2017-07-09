
#ifndef _H_JUNO_OP_CODES
#define _H_JUNO_OP_CODES

#define JUNO_NOOP          0
#define JUNO_LOAD          1
#define JUNO_STORE         8

#define JUNO_ADD           16
#define JUNO_SUB           17
#define JUNO_MUL           18
#define JUNO_DIV           19

#define JUNO_AND           32
#define JUNO_OR            33
#define JUNO_XOR           34
#define JUNO_NOT           35

#define JUNO_PCR_JUMP      128
#define JUNO_PCR_JUMP_ZERO 129
#define JUNO_PCR_JUMP_LTZ  130
#define JUNO_PCR_JUMP_GTZ  131

#define JUNO_HALT          255

#endif
