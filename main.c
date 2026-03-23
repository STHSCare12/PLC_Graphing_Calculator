#include "helper.h"

int main(int argc, char *argv[]) {
    char input[256];

    if (!read_input(argc, argv, input)) {
        return 1;
    }
    return 0;
}

