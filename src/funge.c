#include "funge.h"
#include <stdlib.h>

#define alloc_c(cls) ((cls*)malloc(sizeof(cls)))
#define ncoll() alloc_c(collection)
#define ncoord() alloc_c(coord)
#define BKT_SIZE 9

void free_collection(collection* c) {
	for (size_t bu = 0; bu < c->num; bu++) {
		coord* co = c->buckets[bu]; /* iterate over chain and free all */

		while (co) {
			coord* n = co->next;
			free(co);
			co = n;
		}
	}

	free(c->buckets);
	free(c);
}

collection* make_collection(size_t size) {
	collection* res = ncoll();

	res->num = size;
	res->buckets = (coord**)malloc(sizeof(coord*) * size);
	res->start = NULL;

	return res;
}

size_t coord_id(collection* c, size_t x, size_t y) {
	const size_t id = x + y; /* awkward way of handling buckets */

	for (size_t b = 0; b < c->num; b++) {
		size_t bk = c->num - b;

		if ((id % bk) == 0) {
			return bk - 1;
		}
	}

	return 0;
}

coord* chain_get(collection* c, size_t x, size_t y) {
	size_t n = coord_id(c, x, y); /* search bucket */
	coord* b = c->buckets[n];
	coord* res = NULL;

	while (b) {
		if ((b->x == x) && (b->y == y)) { /* is the right one */
			res = b;
			break;
		}
		b = b->next;
	}

	return res;
}

void chain_rem(collection* c, size_t x, size_t y) {
	size_t n = coord_id(c, x, y);
	coord* b = c->buckets[n];

	while (b) {
		if ((b->x == x) && (b->y == y)) {
			b->prev->next = b->next; /* isolate linkage */
			b->next->prev = b->prev;
			free(b); /* dealloc */
			break;
		}

		b = b->next;
	}
}

void chain_set(collection* c, size_t x, size_t y, uchar v) {
	if (v == SPACE) { /* remove space */
		chain_rem(c, x, y);
		return;
	}

	size_t n = coord_id(c, x, y); /* search bucket */
	coord* b = c->buckets[n];

	if (b) {
		while (1) {
			if ((b->x == x) && (b->y == y)) { /* is the right one */
				b->v = v;
				return;
			}

			if (b->next) {
				b = b->next;
			}
			else {
				break;
			}
		}

		b->next = ncoord(); /* not found, add to chain */
		b = b->next;
	}
	else { /* bucket not initialized */
		b = ncoord();
		c->buckets[n] = b;
	}

	b->x = x;
	b->y = y;
	b->v = v;
}

collection* parse_code(size_t s, char* t) {
	collection* res = ncoll();
	size_t xpos = 0;
	size_t ypos = 0;

	for (size_t idx = 0; idx < s; idx++) {
		uchar uc = (uchar)t[idx];

		switch (uc) {
			case '\n':
				xpos = 0;
				ypos++;
				break;
			case SPACE:
				xpos++;
				break;
			default:
				chain_set(res, xpos, ypos, uc);

				if (!res->start) {
					res->start = chain_get(res, xpos, ypos); /* first instruction */
				}

				xpos++;
				break;
		}
	}

	return res;
}

collection* parse_file(FILE* fp) {
	collection* res = make_collection(BKT_SIZE);
	size_t xpos = 0;
	size_t ypos = 0;
	int c;

	while ((c = fgetc(fp)) != EOF) {
		uchar uc = (uchar)c;

		switch (uc) {
			case '\n':
				xpos = 0;
				ypos++;
				break;
			case SPACE:
				xpos++;
				break;
			default:
				chain_set(res, xpos, ypos, uc);

				if (!res->start) {
					res->start = chain_get(res, xpos, ypos); /* first instruction */
				}

				xpos++;
				break;
		}
	}

	return res;
}
