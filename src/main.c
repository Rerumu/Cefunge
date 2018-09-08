#include <stdio.h>
#include "funge.h"
#include "fvm.h"

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