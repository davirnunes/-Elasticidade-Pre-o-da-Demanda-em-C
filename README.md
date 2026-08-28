# Econometria em C: Elasticidade-Preço da Demanda 📈

Este projeto demonstra a aplicação de conceitos de **Econometria e Análise de Dados** utilizando a linguagem **C**. Embora R e Python sejam as linguagens padrão para estatística, a implementação de modelos de regressão a partir do zero em C prova um sólido entendimento matemático, algorítmico e de otimização computacional.

## 🎯 Problema de Negócio

Para uma empresa varejista, entender a sensibilidade do consumidor às variações de preço é essencial. Este projeto calcula a **Elasticidade-Preço da Demanda** para determinar se a demanda de um produto é elástica ou inelástica.

## 🛠️ Metodologia

O modelo desenvolvido utiliza uma **Regressão Linear Simples via Mínimos Quadrados Ordinários (OLS)** sobre os dados log-transformados (Modelo Log-Log).
- A equação matemática base é: `log(Q) = beta_0 + beta_1 * log(P)`
- Onde `beta_1` representa o coeficiente de elasticidade.

A implementação resolve analiticamente os coeficientes `beta_0` e `beta_1` calculando as variâncias e covariâncias através de loops otimizados em C.

## 🚀 Como Executar

Compile o arquivo utilizando o `gcc`:
```bash
gcc price_elasticity.c -o price_elasticity -lm
./price_elasticity
```
