#include "fvm.h"
#include <stdlib.h>
#include <time.h>

#define pop2(x) x = stack_pop(&stk)
#define pushv(x) stack_push(&stk, x)
#define gettop(n) stack_gettop(&stk, n)

int bgetchar() {
	return getchar();
}

int bgetint() {
	int res;
	int s = -1;

	while (s < 0) {
		s = scanf("%d", &res);

		if (s < 0 && bgetchar() == EOF) {
			s = 1;
		}
	}

	return res;
}

void bputchar(int o) {
	printf("%c", (char)o);
}

void bputint(int o) {
	printf("%i ", o);
}

int stack_gettop(bstack* s, int t) {
	int loc = s->top - t;
	int res;

	if (loc < 0) {
		res = 0;
	}
	else {
		res = s->values[loc];
	}

	return res;
}

int stack_pop(bstack* s) {
	if (s->top) s->top--;
	return stack_gettop(s, 0);
}

void stack_push(bstack* s, int v) {
	if (s->top > (s->size - 1)) {
		s->size *= 2;
		s->values = realloc(s->values, s->size);
	}

	s->values[s->top++] = v;
}

void execute_code(collection* co) {
	coord* ip = co->start;
	enum flow_dir dir = right; /* direction */
	bstack stk;

	int running = 1; /* is interpreting? */
	int string = 0; /* is reading string? 0 = no, 1 = yes, 2 = yes & escaped */

	stk.size = 36;
	stk.top = 0;
	stk.values = (int*)malloc(stk.size * sizeof(int));

	int a, b, c; /* temp 'registers' */
	coord* gr; /* for reading */

	srand(time(0)); /* random seed */

	while (running) {
		int jlen = 1;
		uchar ipv = ip->v;

		if (string) {
			if (string == 1 && ipv == '"') {
				string = 0; /* exit */
			}
			else if (string == 1 && ipv == '\\') {
				string = 2; /* escape */
			}
			else {
				pushv(ipv);
				string = 1;
			}
		}
		else {
			switch (ipv) { /* execute */
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					pushv(ipv - '0');
					break;
				case '+':
					pop2(a);
					pop2(b);
					pushv(a + b);
					break;
				case '-':
					pop2(a);
					pop2(b);
					pushv(b - a);
					break;
				case '*':
					pop2(a);
					pop2(b);
					pushv(a * b);
					break;
				case '/':
					pop2(a);
					pop2(b);
					pushv(b / a);
					break;
				case '%':
					pop2(a);
					pop2(b);
					pushv(b % a);
					break;
				case '!':
					pop2(a);
					pushv(!a);
					break;
				case '`':
					pop2(a);
					pop2(b);
					pushv(b > a);
					break;
				case '>':
					dir = right;
					break;
				case '<':
					dir = left;
					break;
				case '^':
					dir = up;
					break;
				case 'v':
					dir = down;
					break;
				case '?':
					dir = rand() % 4;
					break;
				case '_':
					pop2(a);
					if (a == 0) {
						dir = right;
					}
					else {
						dir = left;
					}
					break;
				case '|':
					pop2(a);
					if (a == 0) {
						dir = down;
					}
					else {
						dir = up;
					}
					break;
				case '"':
					string = 1;
					break;
				case ':':
					pushv(gettop(1));
					break;
				case '\\':
					pop2(a);
					pop2(b);
					pushv(a);
					pushv(b);
					break;
				case '$':
					pop2(a);
					break;
				case '.':
					pop2(a);
					bputint(a);
					break;
				case ',':
					pop2(a);
					bputchar(a);
					break;
				case '#':
					jlen++;
					break;
				case 'p':
					pop2(a); /* y */
					pop2(b); /* x */
					pop2(c); /* v */
					chain_set(co, b, a, c);
					break;
				case 'g':
					pop2(a);
					pop2(b);
					gr = chain_get(co, b, a);
					pushv(gr ? gr->v : 0);
					break;
				case '&':
					pushv(bgetint());
					break;
				case '~':
					pushv(bgetchar());
					break;
				case '@':
					running = 0;
					break;
			}
		}

		if (running) {
			size_t lx = ip->x;
			size_t ly = ip->y;

			do {
				switch (dir) { /* advance */
					case right:
						lx += jlen;
						break;
					case left:
						lx -= jlen;
						break;
					case down:
						ly += jlen;
						break;
					case up:
						ly -= jlen;
						break;
					default:
						running = 0;
						break;
				}
				ip = chain_get(co, lx, ly);
				jlen = 1;
			} while (running && !ip);
		}
	}
	free(stk.values);
}