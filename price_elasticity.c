#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Projeto de Econometria em C: Elasticidade-Preço da Demanda usando OLS (Mínimos Quadrados Ordinários)

void compute_ols(double *x, double *y, int n, double *beta_0, double *beta_1) {
    double sum_x = 0, sum_y = 0, sum_xy = 0, sum_xx = 0;
    
    for (int i = 0; i < n; i++) {
        sum_x += x[i];
        sum_y += y[i];
        sum_xy += x[i] * y[i];
        sum_xx += x[i] * x[i];
    }
    
    double mean_x = sum_x / n;
    double mean_y = sum_y / n;
    
    // beta_1 = Cov(x,y) / Var(x)
    *beta_1 = (sum_xy - n * mean_x * mean_y) / (sum_xx - n * mean_x * mean_x);
    // beta_0 = mean_y - beta_1 * mean_x
    *beta_0 = mean_y - (*beta_1) * mean_x;
}

int main() {
    printf("=== Analise de Elasticidade-Preco da Demanda ===\n\n");
    
    // Dados simulados de vendas de um e-commerce
    // Preco (P) e Quantidade Demandada (Q)
    double prices[] = {100.0, 110.0, 120.0, 130.0, 140.0, 150.0, 160.0};
    double quantities[] = {500.0, 450.0, 410.0, 360.0, 310.0, 280.0, 240.0};
    int n = sizeof(prices) / sizeof(prices[0]);
    
    // Para calcular a elasticidade, usamos o modelo Log-Log: log(Q) = beta_0 + beta_1 * log(P)
    // Onde beta_1 eh a elasticidade preco da demanda.
    
    double log_P[n];
    double log_Q[n];
    
    for (int i = 0; i < n; i++) {
        log_P[i] = log(prices[i]);
        log_Q[i] = log(quantities[i]);
    }
    
    double beta_0 = 0, beta_1 = 0;
    compute_ols(log_P, log_Q, n, &beta_0, &beta_1);
    
    printf("Modelo Linear Log-Log Estimado:\n");
    printf("log(Q) = %.4f + (%.4f) * log(P)\n\n", beta_0, beta_1);
    
    printf("Resultados da Econometria:\n");
    printf("- Coeficiente de Elasticidade (beta_1): %.4f\n", beta_1);
    
    if (beta_1 < -1.0) {
        printf("- Interpretacao: Demanda Elastica. Uma variacao no preco afeta significativamente a demanda.\n");
    } else if (beta_1 > -1.0 && beta_1 < 0) {
        printf("- Interpretacao: Demanda Inelastica. Variacoes no preco tem pouco impacto na demanda.\n");
    } else {
        printf("- Interpretacao: Comportamento atipico (bens de Giffen/Veblen) ou elasticidade unitaria.\n");
    }
    
    return 0;
}
