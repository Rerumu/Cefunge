#pragma once
#include "funge.h"

int bgetint();
int bgetchar();
void bputint(int o);
void bputchar(int o);

int stack_gettop(bstack* s, int t); /* get value relative to top and `t` */
int stack_pop(bstack* s); /* pop the top of the stack */
void stack_push(bstack* s, int v); /* push a value onto the stack */
void execute_code(collection* c);
