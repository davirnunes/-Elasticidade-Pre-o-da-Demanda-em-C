#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ols.h"

/* Exporta P, Q, log(P), log(Q) e o valor previsto pelo modelo para um CSV,
 * para depois plotar em Python/Excel/etc (fica bom no portfólio mostrar
 * a saída sendo consumida por outra ferramenta de análise). */
static void export_results(const char *filename, const double *price,
                            const double *quantity, const double *log_p,
                            const double *log_q, const OLSResult *r) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "Aviso: nao foi possivel salvar '%s'\n", filename);
        return;
    }

    fprintf(fp, "price,quantity,log_price,log_quantity,predicted_log_quantity,residual\n");
    for (int i = 0; i < r->n; i++) {
        double pred = r->beta_0 + r->beta_1 * log_p[i];
        double residual = log_q[i] - pred;
        fprintf(fp, "%.4f,%.4f,%.6f,%.6f,%.6f,%.6f\n",
                price[i], quantity[i], log_p[i], log_q[i], pred, residual);
    }

    fclose(fp);
    printf("Resultados exportados para: %s\n", filename);
}

int main(int argc, char **argv) {
    const char *input_file = (argc > 1) ? argv[1] : "data/vendas.csv";
    const char *output_file = "data/resultados.csv";

    double price[MAX_OBS], quantity[MAX_OBS];
    int n = load_csv(input_file, price, quantity, MAX_OBS);

    if (n < 0) {
        fprintf(stderr, "Erro: nao foi possivel abrir o arquivo '%s'\n", input_file);
        return EXIT_FAILURE;
    }
    if (n < 3) {
        fprintf(stderr, "Erro: sao necessarias pelo menos 3 observacoes validas "
                         "(encontradas: %d)\n", n);
        return EXIT_FAILURE;
    }

    double log_p[MAX_OBS], log_q[MAX_OBS];
    for (int i = 0; i < n; i++) {
        if (price[i] <= 0.0 || quantity[i] <= 0.0) {
            fprintf(stderr, "Erro: preco/quantidade devem ser positivos "
                             "(linha %d invalida)\n", i + 1);
            return EXIT_FAILURE;
        }
        log_p[i] = log(price[i]);
        log_q[i] = log(quantity[i]);
    }

    printf("=== Analise de Elasticidade-Preco da Demanda ===\n");
    printf("Fonte de dados: %s (%d observacoes)\n\n", input_file, n);

    OLSResult result;
    ols_fit(log_p, log_q, n, &result);
    ols_print_report(&result);

    printf("\n");
    export_results(output_file, price, quantity, log_p, log_q, &result);

    return EXIT_SUCCESS;
}