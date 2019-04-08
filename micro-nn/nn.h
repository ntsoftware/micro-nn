#pragma once

void nn_init_weights();
void nn_update_weights(const float eps);

int nn_read_weights(const char* filename);
int nn_write_weights(const char* filename);

void nn_forward(const float* x);
void nn_backward(const float* x, const float* y);

void nn_zero_grad();

size_t nn_size();
size_t nn_n_inputs();
size_t nn_n_outputs();

const float* nn_y();