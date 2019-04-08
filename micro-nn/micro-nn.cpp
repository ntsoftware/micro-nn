#include "pch.h"

static float dist2(const float* a, const float*b, const size_t n) {
    float x = 0.0f;
    for (size_t i = 0; i < n; ++i) {
        const float d = a[i] - b[i];
        x += d * d;
    }
    return x;
}

int main() {
    const unsigned seed = 1234;
    srand(seed);
    log_i("seed=%u", seed);

    struct csv* x = csv_read("x.txt", nn_n_inputs());
    struct csv* y = csv_read("y.txt", nn_n_outputs());

    nn_init_weights();
    log_i("nn size=%d bytes", nn_size());

    float eps = 0.01f;
    for (size_t epoch = 0; epoch < 2000; ++epoch) {
        nn_zero_grad();

        float loss = 0.0f;
        for (size_t i = 0; i < csv_n_rows(x); ++i) {
            const float* sample_x = csv_row(x, i);
            const float* sample_y = csv_row(y, i);

            nn_forward(sample_x);
            nn_backward(sample_x, sample_y);

            loss += 0.5f * dist2(nn_y(), sample_y, csv_n_cols(y));
        }

        //log_i("epoch %u, eps=%g, loss=%g", epoch, eps, loss);

        nn_update_weights(eps);
        eps *= 0.9999f;
    }

    for (size_t i = 0; i < csv_n_rows(x); ++i) {
        const float* sample_x = csv_row(x, i);

        nn_forward(sample_x);

        log_i("x=[%g,%g] y=%g", sample_x[0], sample_x[1], nn_y()[0]);
    }

    csv_delete(x);
    csv_delete(y);
    return 0;
}