#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ols.h"

void ols_fit(const double *x, const double *y, int n, OLSResult *result) {
    double sum_x = 0.0, sum_y = 0.0, sum_xy = 0.0, sum_xx = 0.0;

    for (int i = 0; i < n; i++) {
        sum_x += x[i];
        sum_y += y[i];
        sum_xy += x[i] * y[i];
        sum_xx += x[i] * x[i];
    }

    double mean_x = sum_x / n;
    double mean_y = sum_y / n;

    double s_xy = sum_xy - n * mean_x * mean_y;   /* soma de produtos cruzados */
    double s_xx = sum_xx - n * mean_x * mean_x;   /* soma de quadrados de x */

    double beta_1 = s_xy / s_xx;
    double beta_0 = mean_y - beta_1 * mean_x;

    /* Soma dos quadrados dos resíduos (SSE) e soma total (SST),
     * necessárias para R² e para o erro padrão da regressão. */
    double sse = 0.0, sst = 0.0;
    for (int i = 0; i < n; i++) {
        double y_hat = beta_0 + beta_1 * x[i];
        double residual = y[i] - y_hat;
        sse += residual * residual;
        sst += (y[i] - mean_y) * (y[i] - mean_y);
    }

    double r_squared = 1.0 - (sse / sst);
    double std_error = sqrt(sse / (n - 2));        /* n-2 graus de liberdade */
    double se_beta_1 = std_error / sqrt(s_xx);
    double t_stat = beta_1 / se_beta_1;

    result->n = n;
    result->beta_0 = beta_0;
    result->beta_1 = beta_1;
    result->se_beta_1 = se_beta_1;
    result->t_stat = t_stat;
    result->r_squared = r_squared;
    result->std_error = std_error;
}

static const char *interpret_elasticity(double beta_1) {
    if (beta_1 < -1.0) {
        return "Demanda ELASTICA: uma variacao percentual no preco gera uma "
               "variacao percentual MAIOR na quantidade demandada.";
    } else if (beta_1 < 0.0) {
        return "Demanda INELASTICA: uma variacao percentual no preco gera uma "
               "variacao percentual MENOR na quantidade demandada.";
    } else if (beta_1 == 0.0) {
        return "Demanda com elasticidade nula (caso extremo/improvavel com dados reais).";
    }
    return "Coeficiente positivo: comportamento atipico (bens de Giffen/Veblen) "
           "ou possivel problema na base de dados.";
}

void ols_print_report(const OLSResult *r) {
    printf("=== Regressao Log-Log: log(Q) = b0 + b1 * log(P) ===\n\n");
    printf("Observacoes (n)         : %d\n", r->n);
    printf("Intercepto (beta_0)     : %.4f\n", r->beta_0);
    printf("Elasticidade (beta_1)   : %.4f\n", r->beta_1);
    printf("Erro padrao de beta_1   : %.4f\n", r->se_beta_1);
    printf("Estatistica t           : %.4f\n", r->t_stat);
    printf("R-quadrado              : %.4f  (%.1f%% da variancia explicada)\n",
           r->r_squared, r->r_squared * 100.0);
    printf("Erro padrao da regressao: %.4f\n\n", r->std_error);

    printf("Significancia: %s\n\n",
           fabs(r->t_stat) > 2.0
               ? "beta_1 e estatisticamente significativo a ~5% (|t| > 2)."
               : "beta_1 NAO e estatisticamente significativo a ~5% (|t| <= 2).");

    printf("Interpretacao economica:\n%s\n", interpret_elasticity(r->beta_1));
}

int load_csv(const char *filename, double *price, double *quantity, int max_rows) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        return -1;
    }

    char line[256];
    int row = 0;

    /* Descarta a linha de cabeçalho (ex: "price,quantity") */
    if (fgets(line, sizeof(line), fp) == NULL) {
        fclose(fp);
        return 0;
    }

    while (row < max_rows && fgets(line, sizeof(line), fp)) {
        double p, q;
        if (sscanf(line, "%lf,%lf", &p, &q) == 2) {
            price[row] = p;
            quantity[row] = q;
            row++;
        }
    }

    fclose(fp);
    return row;
}