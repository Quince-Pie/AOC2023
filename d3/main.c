#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

size_t
readFile(char **dest, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Unable to open file, are sure it exists?");
        return 0;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        perror("fseek() failed");
        fclose(file);
        return 0;
    }

    long file_size = ftell(file);
    if (file_size == -1) {
        perror("ftell() failed");
        fclose(file);
        return 0;
    }

    if (fseek(file, 0, SEEK_SET) != 0) {
        perror("fseek() failed");
        fclose(file);
        return 0;
    }

    char *buffer = (char *)malloc(file_size * sizeof(char) + 1);
    if (!buffer) {
        fprintf(stderr, "Failed to allocate memory\n");
        fclose(file);
        return 0;
    }

    size_t bytes_read = fread(buffer, sizeof(char), file_size, file);
    if (bytes_read != file_size) {
        perror("fread() failed");
        free(buffer);
        fclose(file);
        return 0;
    }

    buffer[bytes_read] = '\0';
    *dest = buffer;

    fclose(file);
    return bytes_read;
}

bool
symCheck(char c) {
    if (!isdigit(c) && c != '\n' && c != '\0' && c != '.') {
        return true;
    };
    return false;
}

typedef struct {
    uint32_t nums[2];
} gear;

int
main() {
    char *input_txt = NULL;
    size_t input_len = readFile(&input_txt, "input");
    assert(input_txt != NULL);

    // PART 1
    uint32_t sum = 0;
    uint32_t tmp_num = 0;
    int32_t cols = 0;
    bool sym_found = false;
    // loop to find the number of cols
    for (size_t idx = 0; idx < input_len; idx++) {
        char c = input_txt[idx];
        if (c == '\n') {
            cols = idx + 1;
            break;
        }
    }
    // PART 1
    for (int idx = 0; idx < input_len; idx++) {
        char c = input_txt[idx];
        if (c >= '0' && c <= '9') {
            tmp_num *= 10;
            tmp_num += c - '0';

            if (!sym_found) {
                // directly above
                if (idx - cols >= 0) {
                    if (symCheck(input_txt[idx - cols])) {
                        sym_found = true;
                    }
                }
                // directly below
                if (idx + cols < input_len) {
                    if (symCheck(input_txt[idx + cols])) {
                        sym_found = true;
                    }
                }
                // left
                if (idx - 1 >= 0) {
                    if (symCheck(input_txt[idx - 1])) {
                        sym_found = true;
                    }
                }
                // right
                if (idx + 1 < input_len) {
                    if (symCheck(input_txt[idx + 1])) {
                        sym_found = true;
                    }
                }
                // above + 1
                if (idx - cols + 1 >= 0) {
                    if (symCheck(input_txt[idx - cols + 1])) {
                        sym_found = true;
                    }
                }
                // above - 1
                if (idx - cols - 1 >= 0) {
                    if (symCheck(input_txt[idx - cols - 1])) {
                        sym_found = true;
                    }
                }
                // below + 1
                if (idx + cols + 1 < input_len) {
                    if (symCheck(input_txt[idx + cols + 1])) {
                        sym_found = true;
                    }
                }
                // below - 1
                if (idx + cols - 1 < input_len) {
                    if (symCheck(input_txt[idx + cols - 1])) {
                        sym_found = true;
                    }
                }
            }

        } else {
            if (!sym_found) {
                tmp_num = 0;
            } else {
                sum += tmp_num;
                tmp_num = 0;
                sym_found = false;
            }
        }
    }
    printf("sum: %d\n", sum);

    // PART 2
    // way more than needed
    gear *gears = (gear *)calloc(input_len, sizeof(gear));
    assert(gears != NULL);
    tmp_num = 0;
    sym_found = false;
    int sym_idx = 0;

    for (int idx = 0; idx < input_len; idx++) {
        char c = input_txt[idx];
        if (c >= '0' && c <= '9') {
            tmp_num *= 10;
            tmp_num += c - '0';

            if (!sym_found) {
                // directly above
                if (idx - cols >= 0) {
                    if (input_txt[idx - cols] == '*') {
                        sym_found = true;
                        sym_idx = idx - cols;
                    }
                }
                // directly below
                if (idx + cols < input_len) {
                    if (input_txt[idx + cols] == '*') {
                        sym_found = true;
                        sym_idx = idx + cols;
                    }
                }
                // left
                if (idx - 1 >= 0) {
                    if (input_txt[idx - 1] == '*') {
                        sym_found = true;
                        sym_idx = idx - 1;
                    }
                }
                // right
                if (idx + 1 < input_len) {
                    if (input_txt[idx + 1] == '*') {
                        sym_found = true;
                        sym_idx = idx + 1;
                    }
                }
                // above + 1
                if (idx - cols + 1 >= 0) {
                    if (input_txt[idx - cols + 1] == '*') {
                        sym_found = true;
                        sym_idx = idx - cols + 1;
                    }
                }
                // above - 1
                if (idx - cols - 1 >= 0) {
                    if (input_txt[idx - cols - 1] == '*') {
                        sym_found = true;
                        sym_idx = idx - cols - 1;
                    }
                }
                // below + 1
                if (idx + cols + 1 < input_len) {
                    if (input_txt[idx + cols + 1] == '*') {
                        sym_found = true;
                        sym_idx = idx + cols + 1;
                    }
                }
                // below - 1
                if (idx + cols - 1 < input_len) {
                    if (input_txt[idx + cols - 1] == '*') {
                        sym_found = true;
                        sym_idx = idx + cols - 1;
                    }
                }
            }

        } else {
            if (!sym_found) {
                tmp_num = 0;
            } else {
                // if both nums are not 0, zero both of them
                if (gears[sym_idx].nums[0] != 0 && gears[sym_idx].nums[1] != 0) {
                    gears[sym_idx].nums[0] = 0;
                    gears[sym_idx].nums[1] = 0;
                } else if (gears[sym_idx].nums[0] == 0) {
                    gears[sym_idx].nums[0] = tmp_num;
                } else {
                    gears[sym_idx].nums[1] = tmp_num;
                }
                tmp_num = 0;
                sym_found = false;
                sym_idx = 0;
            }
        }
    }
    // for all gears that have two numbers that are not 0, multiply them and sum them
    long sum2 = 0;
    for (int idx = 0; idx < input_len; idx++) {
        if (gears[idx].nums[0] != 0 && gears[idx].nums[1] != 0) {
            sum2 += gears[idx].nums[0] * gears[idx].nums[1];
        }
    }
    printf("sum2: %ld\n", sum2);

    // PART 2
    free(gears);
    free(input_txt);
    return 0;
}
