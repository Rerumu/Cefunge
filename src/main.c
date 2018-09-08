#include <stdio.h>
#include "funge.h"
#include "fvm.h"

/* todo */
/* better bucketing for larger programs */
/* faster ip traversal via some cache */
/* ip wrap around when end of code reached */

int main(int argc, char** argv) {
	for (int i = 1; i < argc; i++) {
		FILE* fl = fopen(argv[i], "r");

		if (fl == NULL) {
			printf("\"%s\" failed to open", argv[i]);
		}
		else {
			collection* c = parse_file(fl);
			fclose(fl);

			execute_code(c);
			free_collection(c);
		}
	}
}