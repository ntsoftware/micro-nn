#pragma once

struct csv;

void csv_delete(struct csv* csv);

struct csv* csv_read(const char* filename, const size_t n_cols);

size_t csv_n_rows(const struct csv* csv);
size_t csv_n_cols(const struct csv* csv);

const float* csv_row(const struct csv* csv, size_t i);