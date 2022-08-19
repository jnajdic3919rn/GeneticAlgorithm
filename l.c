#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double x_r[50] = {
    0.00, 0.10, 0.20, 0.30, 0.40, 0.50, 0.60, 0.70, 0.80, 0.90,
    1.00, 1.10, 1.20, 1.30, 1.40, 1.50, 1.60, 1.70, 1.80, 1.90,
    2.00, 2.10, 2.20, 2.30, 2.40, 2.50, 2.60, 2.70, 2.80, 2.90,
    3.00, 3.10, 3.20, 3.30, 3.40, 3.50, 3.60, 3.70, 3.80, 3.90,
    4.00, 4.10, 4.20, 4.30, 4.40, 4.50, 4.60, 4.70, 4.80, 4.90
};

double y_l[50] = {
    4.0000, 4.0100, 4.0395, 4.0873, 4.1516, 4.2298, 4.3188, 4.4150, 4.5146, 4.6136,
    4.7081, 4.7943, 4.8687, 4.9284, 4.9711, 4.9950, 4.9991, 4.9834, 4.9484, 4.8955,
    4.8268, 4.7451, 4.6537, 4.5561, 4.4563, 4.3582, 4.2657, 4.1827, 4.1122, 4.0572,
    4.0199, 4.0017, 4.0034, 4.0249, 4.0653, 4.1230, 4.1958, 4.2807, 4.3744, 4.4730,
    4.5728, 4.6696, 4.7596, 4.8394, 4.9055, 4.9556, 4.9874, 4.9998, 4.9923, 4.9652
};


// one neuron
double calc_neuron(int neurons, double input_weights[], double input_values[], double bias) {
	int i, j;
	double sum = bias;
	for (i = 0; i < neurons; i++) {
		sum += input_weights[i] * input_values[i];
	}
	return sum;
}

// calc net

#define L1N 5
#define L2N 3

double layer1[L1N][1];
double layer2[L2N][L1N];
double layer3[1][L2N];

double bias1[L1N];
double bias2[L2N];
double bias3[1];

double output0[1];
double output1[L1N];
double output2[L2N];

int main(int argc, char* argv[]) {
    double *y_r = y_l;

    int total_expected_args = 1 + L1N + L1N * L2N + L2N + L1N + L2N + 1;

    if (argc != total_expected_args) {
        // broj argumenata nije odgovarajuci
        fprintf(stderr, "Broj argumenata nije odgovarajuci, ocekivano %d realnih vrednosti.\n", total_expected_args - 1);
        exit(1);
    }

    // parsiranje
    int i, j, k, ai = 1;
    // layer1
    for (i = 0; i < L1N; i++, ai++) {
        layer1[i][0] = atof(argv[ai]);
    }
    // layer2
    for (i = 0; i < L2N; i++) {
        for (j = 0; j < L1N; j++, ai++) {
            layer2[i][j] = atof(argv[ai]);
        }
    }
    // layer3
    for (i = 0; i < L2N; i++) {
        layer3[0][i] = atof(argv[ai]);
    }
    // bias1
    for (i = 0; i < L1N; i++, ai++) {
        bias1[i] = atof(argv[ai]);
    }
    // bias2
    for (i = 0; i < L2N; i++, ai++) {
        bias2[i] = atof(argv[ai]);
    }
    // bias3
    bias3[0] = atof(argv[ai]);

    // idemo dalje!
    double mse = 0;

    for (k = 0; k < 50; k++) {
        output0[0] = x_r[k];
        // layer1
        for (i = 0; i < L1N; i++) {
            output1[i] = calc_neuron(1, layer1[i], output0, bias1[i]);
        }
        // layer2
        for (i = 0; i < L2N; i++) {
            output2[i] = calc_neuron(L1N, layer2[i], output1, bias2[i]);
        }
        // layer3
        double val = calc_neuron(L2N, layer3[0], output2, bias3[0]);

        double err = pow(y_r[k] - val, 2);
        mse += err;
    }

    /// upis vrednosti u fajl

    FILE *fp;
    fp = fopen("vrednost.txt", "w");
    if(fp == NULL) {
        printf("Fajl se ne moze otvoriti\n");
        exit(1);
    }
    fprintf(fp,"%lf", mse/50);
    fclose(fp);

   /// printf("%.5f\n", mse / 50);
    return 0;
}


