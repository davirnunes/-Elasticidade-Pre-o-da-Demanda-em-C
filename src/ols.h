#ifndef OLS_H
#define OLS_H

#define MAX_OBS 1000

/* Guarda todos os resultados de uma regressão log-log simples:
 *   log(Q) = beta_0 + beta_1 * log(P) + erro
 * beta_1 é, por construção, a elasticidade-preço da demanda. */
typedef struct {
    int n;              /* número de observações */
    double beta_0;       /* intercepto */
    double beta_1;       /* elasticidade (coeficiente angular) */
    double se_beta_1;    /* erro padrão de beta_1 */
    double t_stat;        /* estatística t de beta_1 (H0: beta_1 = 0) */
    double r_squared;     /* poder explicativo do modelo (0 a 1) */
    double std_error;     /* erro padrão da regressão (RMSE dos resíduos) */
} OLSResult;

/* Estima os coeficientes por Mínimos Quadrados Ordinários e preenche
 * todas as métricas estatísticas em 'result'. */
void ols_fit(const double *x, const double *y, int n, OLSResult *result);

/* Imprime um relatório legível dos resultados no terminal. */
void ols_print_report(const OLSResult *result);

/* Carrega um CSV com colunas "price,quantity" (com cabeçalho) para os
 * vetores price[] e quantity[]. Retorna o número de linhas lidas,
 * ou -1 em caso de erro ao abrir o arquivo. */
int load_csv(const char *filename, double *price, double *quantity, int max_rows);

#endif