#pragma once
#include <stddef.h>
#include <stdio.h>

#define SPACE ' '

typedef unsigned char uchar;

typedef struct bstack_s {
	int top;
	int size;
	int* values;
} bstack;

typedef struct coord_s {
	struct coord_s* prev;
	struct coord_s* next;
	size_t x;
	size_t y;
	uchar v;
} coord;

typedef struct collection_s {
	coord* start;
	coord** buckets;
	size_t num;
} collection;

enum flow_dir {
	right, left,
	up, down
};

void free_collection(collection* c);
collection* make_collection(size_t size);

size_t coord_id(collection* c, size_t x, size_t y);
coord* chain_get(collection* c, size_t x, size_t y);
void chain_rem(collection* c, size_t x, size_t y);
void chain_set(collection* c, size_t x, size_t y, uchar v);

collection* parse_code(size_t s, char* t);
collection* parse_file(FILE* fp);
