# Elasticidade-Preço da Demanda em C (OLS)

Estimativa da elasticidade-preço da demanda de um produto a partir de dados
de preço e quantidade vendida, usando regressão linear por **Mínimos
Quadrados Ordinários (OLS)** implementada do zero em C — sem bibliotecas
estatísticas prontas.

## Metodologia

A elasticidade-preço não é constante em modelos lineares simples (`Q = a + bP`),
então o projeto usa a especificação **log-log**, padrão em econometria para
estimar elasticidades diretamente:

```
log(Q) = β₀ + β₁ · log(P) + ε
```

Nesse modelo, **β₁ é a própria elasticidade-preço da demanda**: mede a
variação percentual esperada na quantidade demandada para cada 1% de
variação no preço.

O programa estima β₀ e β₁ por OLS e reporta:

- **β₁ (elasticidade)** e seu **erro padrão**
- **Estatística t** e um teste informal de significância a ~5% (`|t| > 2`)
- **R²** — quanto da variância de `log(Q)` o modelo explica
- **Erro padrão da regressão** (RMSE dos resíduos)
- Interpretação econômica automática (demanda elástica/inelástica/atípica)

## Estrutura do projeto

```
.
├── data/
│   ├── vendas.csv        # dados de entrada (preço, quantidade)
│   └── resultados.csv    # gerado após rodar: previsões + resíduos
├── include/
│   └── ols.h              # interface do módulo estatístico
├── src/
│   ├── ols.c              # estimação OLS, métricas e leitor de CSV
│   └── main.c              # orquestração: carrega dados, roda, exporta
├── Makefile
└── README.md
```

## Como rodar

```bash
make run
```

Ou manualmente:

```bash
gcc -Wall -Wextra -std=c11 -Iinclude -o elasticidade.exe src/main.c src/ols.c -lm
.\elasticidade.exe data/avocado_total_us.csv
```

O programa aceita qualquer CSV com colunas `price,quantity` (com cabeçalho)
como argumento — troque `data/vendas.csv` pelo seu próprio dataset.

## Dataset real: preços de abacate nos EUA

Além do exemplo sintético, o projeto inclui `data/avocado_total_us.csv`: preço
médio e volume total de vendas semanais de abacates convencionais no mercado
agregado dos EUA (região "TotalUS"), de jan/2015 a mar/2018 — 169 observações.

Fonte original: **Hass Avocado Board**, dados de scanner de varejo,
compilados e disponibilizados publicamente em
[ryanhomer/dsci522-group411-data](https://github.com/ryanhomer/dsci522-group411-data)
(também conhecido como o dataset "avocado prices" do Kaggle).

```bash
./elasticidade data/avocado_total_us.csv
```

Resultado obtido:

```
Observacoes (n)         : 169
Elasticidade (beta_1)   : -0.6176
Estatistica t           : -8.2008
R-quadrado              : 0.2871  (28.7% da variancia explicada)

Interpretacao economica:
Demanda INELASTICA: uma variacao percentual no preco gera uma
variacao percentual MENOR na quantidade demandada.
```

Vale notar a diferença em relação ao exemplo sintético: aqui o R² é bem mais
baixo (28,7% vs. 98,3%), o que é esperado e **mais honesto** — dados reais de
mercado têm ruído (sazonalidade, promoções, clima, etc.) que um modelo
log-log simples de uma variável não captura. A elasticidade de -0,62 ainda é
estatisticamente significativa (|t| = 8,2) e faz sentido econômico: abacate é
um item de compra relativamente rotineira, sem substitutos próximos óbvios,
por isso a demanda reage pouco a variações de preço.

## Exemplo de saída

```
=== Regressao Log-Log: log(Q) = b0 + b1 * log(P) ===

Observacoes (n)         : 7
Intercepto (beta_0)     : 13.4294
Elasticidade (beta_1)   : -1.5572
Erro padrao de beta_1   : 0.0913
Estatistica t           : -17.0544
R-quadrado              : 0.9831  (98.3% da variancia explicada)
Erro padrao da regressao: 0.0378

Significancia: beta_1 e estatisticamente significativo a ~5% (|t| > 2).

Interpretacao economica:
Demanda ELASTICA: uma variacao percentual no preco gera uma variacao
percentual MAIOR na quantidade demandada.
```

Com β₁ ≈ -1,56, um aumento de 10% no preço reduz a quantidade demandada em
aproximadamente 15,6% — típico de um bem com substitutos próximos disponíveis.

## `resultados.csv`

Cada execução gera um CSV com preço, quantidade, seus logs, o valor previsto
pelo modelo e o resíduo — pronto para plotar em Python (`pandas`/`matplotlib`),
Excel, ou qualquer outra ferramenta de BI, o que ajuda a validar visualmente
o ajuste do modelo (heterocedasticidade, outliers, etc.).

## Possíveis extensões

- Intervalo de confiança para β₁ (exige tabela/aproximação da distribuição t)
- Suporte a regressão múltipla (ex: incluir renda, sazonalidade)
- Teste de normalidade dos resíduos
- Validação cruzada com múltiplos produtos/categorias no mesmo CSV