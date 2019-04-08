#include "pch.h"

struct csv {
    size_t n_rows;
    size_t n_cols;
    float* data;
};

void csv_delete(struct csv* csv) {
    if (csv) {
        free(csv->data);
        free(csv);
    }
}

static const size_t csv_row_increment = 1024;

struct csv* csv_read(const char* filename, const size_t n_cols) {
    struct csv* csv = NULL;
    FILE* f = NULL;
    size_t n_rows = 0;
    size_t max_rows = csv_row_increment;

    f = fopen(filename, "r");
    if (!f) {
        log_e("cannot open file %s", filename);
        goto error;
    }

    csv = (struct csv*)malloc(sizeof(struct csv));
    if (!csv) {
        log_e("cannot allocate CSV data");
        goto error;
    }
    csv->data = (float*)malloc(sizeof(float) * n_cols * max_rows);
    if (!csv->data) {
        log_e("cannot allocate CSV data buffer (%d rows)", max_rows);
        goto error;
    }

    char buf[1024];
    while (fgets(buf, sizeof(buf), f)) {
        const char* s = buf;
        for (size_t i = 0; i < n_cols; ++i) {
            float x;
            if (sscanf(s, "%g", &x) != 1) {
                log_e("cannot parse value number %u (line %u)", i, n_rows + 1);
                goto error;
            }
            csv->data[n_rows * n_cols + i] = x;
            while (*s && *s != ',') {
                ++s;
            }
            if (*s == ',') {
                ++s;
            }
        }

        ++n_rows;
        if (n_rows == max_rows) {
            max_rows += csv_row_increment;
            csv->data = (float*)realloc(csv->data, sizeof(float) * n_cols * max_rows);
            if (!csv->data) {
                log_e("cannot allocate CSV data buffer (%d rows)", max_rows);
                goto error;
            }
        }
    }

    if (ferror(f)) {
        log_e("error reading file %s", filename);
        goto error;
    }

    csv->n_rows = n_rows;
    csv->n_cols = n_cols;

    fclose(f);
    return csv;

error:
    if (csv) {
        free(csv->data);
        free(csv);
    }
    if (f) {
        fclose(f);
    }
    return NULL;
}

size_t csv_n_rows(const struct csv* csv) {
    assert(csv);
    return csv->n_rows;
}

size_t csv_n_cols(const struct csv* csv) {
    assert(csv);
    return csv->n_cols;
}

const float* csv_row(const struct csv* csv, size_t i) {
    assert(csv);
    return csv->data + csv->n_cols * i;
}