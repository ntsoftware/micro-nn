#include "pch.h"

static const size_t n_input = 2;
static const size_t n_layer1 = 10;
static const size_t n_layer2 = 10;
static const size_t n_output = 1;

struct nn {
    // hidden layer 1
    float w1[n_layer1][n_input];
    float b1[n_layer1];
    float a1[n_layer1];
    float d1[n_layer1];

    float d_w1[n_layer1][n_input];
    float d_b1[n_layer1];

    // hidden layer 2
    float w2[n_layer2][n_layer1];
    float b2[n_layer2];
    float a2[n_layer2];
    float d2[n_layer2];

    float d_w2[n_layer2][n_layer1];
    float d_b2[n_layer2];

    // output layer
    float wy[n_output][n_layer2];
    float by[n_output];
    float ay[n_output];
    float dy[n_output];

    float d_wy[n_output][n_layer2];
    float d_by[n_output];
};

static struct nn nn;

static void rand_array(float* a, const size_t n) {
    for (size_t i = 0; i < n; ++i) {
        a[i] = 2.0f * rand() / RAND_MAX - 1.0f;
    }
}

void nn_init_weights() {
    rand_array(&nn.w1[0][0], n_layer1 * n_input);
    rand_array(&nn.b1[0], n_layer1);

    rand_array(&nn.w2[0][0], n_layer2 * n_layer1);
    rand_array(&nn.b2[0], n_layer2);

    rand_array(&nn.wy[0][0], n_output * n_layer2);
    rand_array(&nn.by[0], n_output);
}

void nn_update_weights(const float eps) {
    for (size_t i = 0; i < n_layer1; ++i) {
        for (size_t j = 0; j < n_input; ++j) {
            nn.w1[i][j] -= eps * nn.d_w1[i][j];
        }
        nn.b1[i] -= eps * nn.d_b1[i];
    }

    for (size_t i = 0; i < n_layer2; ++i) {
        for (size_t j = 0; j < n_layer1; ++j) {
            nn.w2[i][j] -= eps * nn.d_w2[i][j];
        }
        nn.b2[i] -= eps * nn.d_b2[i];
    }

    for (size_t i = 0; i < n_output; ++i) {
        for (size_t j = 0; j < n_layer2; ++j) {
            nn.wy[i][j] -= eps * nn.d_wy[i][j];
        }
        nn.by[i] -= eps * nn.d_by[i];
    }
}

int nn_read_weights(const char* filename) {
    // TODO not implemented
    return 0;
}

int nn_write_weights(const char* filename) {
    // TODO not implemented
    return 0;
}

static float dot(const float* a, const float* b, const size_t n) {
    float x = 0.0f;
    for (size_t i = 0; i < n; ++i) {
        x += a[i] * b[i];
    }
    return x;
}

static float dot2(const float* a, const float* b, const size_t n, const size_t m) {
    float x = 0.0f;
    for (size_t i = 0; i < n; ++i) {
        x += a[i * m] * b[i];
    }
    return x;
}

static float sigmoid(const float x) {
    return 1.0f / (1.0f + expf(-x));
}

static float d_sigmoid(const float y) {
    return y * (1.0f - y);
}

static float relu(const float x) {
    return x > 0.0f ? x : 0.0f;
}

static float d_relu(const float y) {
    return y > 0.0f ? 1.0f : 0.0f;
}

void nn_forward(const float* x) {
    for (size_t i = 0; i < n_layer1; ++i) {
        const float z = dot(nn.w1[i], x, n_input) + nn.b1[i];
        nn.a1[i] = relu(z);
    }

    for (size_t i = 0; i < n_layer2; ++i) {
        const float z = dot(nn.w2[i], nn.a1, n_layer1) + nn.b2[i];
        nn.a2[i] = relu(z);
    }

    for (size_t i = 0; i < n_output; ++i) {
        const float z = dot(nn.wy[i], nn.a2, n_layer2) + nn.by[i];
        nn.ay[i] = relu(z);
    }
}

void nn_backward(const float* x, const float* y) {
    for (size_t i = 0; i < n_output; ++i) {
        nn.dy[i] = (nn.ay[i] - y[i]) * d_relu(nn.ay[i]);
    }

    for (size_t i = 0; i < n_layer2; ++i) {
        nn.d2[i] = dot2(&nn.wy[0][i], nn.dy, n_output, n_layer2) * d_relu(nn.a2[i]);
    }

    for (size_t i = 0; i < n_layer1; ++i) {
        nn.d1[i] = dot2(&nn.w2[0][i], nn.d2, n_layer2, n_layer1) * d_relu(nn.a1[i]);
    }

    for (size_t i = 0; i < n_layer1; ++i) {
        for (size_t j = 0; j < n_input; ++j) {
            nn.d_w1[i][j] += nn.d1[i] * x[j];
        }
        nn.d_b1[i] += nn.d1[i];
    }

    for (size_t i = 0; i < n_layer2; ++i) {
        for (size_t j = 0; j < n_layer1; ++j) {
            nn.d_w2[i][j] += nn.d2[i] * nn.a1[j];
        }
        nn.d_b2[i] += nn.d2[i];
    }

    for (size_t i = 0; i < n_output; ++i) {
        for (size_t j = 0; j < n_layer2; ++j) {
            nn.d_wy[i][j] += nn.dy[i] * nn.a2[j];
        }
        nn.d_by[i] += nn.dy[i];
    }
}

void nn_zero_grad() {
    memset(nn.d_w1, 0, sizeof(nn.d_w1));
    memset(nn.d_b1, 0, sizeof(nn.d_b1));

    memset(nn.d_w2, 0, sizeof(nn.d_w2));
    memset(nn.d_b2, 0, sizeof(nn.d_b2));

    memset(nn.d_wy, 0, sizeof(nn.d_wy));
    memset(nn.d_by, 0, sizeof(nn.d_by));
}

size_t nn_size() {
    return sizeof(nn);
}

size_t nn_n_inputs() {
    return n_input;
}

size_t nn_n_outputs() {
    return n_output;
}

const float* nn_y() {
    return nn.ay;
}