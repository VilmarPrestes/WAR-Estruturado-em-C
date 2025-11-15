# 🎲 WAR Estruturado em C

Projeto desenvolvido para a disciplina de Análise e Desenvolvimento de Sistemas, com o objetivo de implementar uma versão simplificada e estruturada do jogo **War** utilizando a linguagem **C**, com foco em:

- Structs
- Vetores e alocação dinâmica de memória
- Ponteiros
- Modularização do código
- Missões estratégicas e condição de vitória

---

## 🧩 Objetivos do Projeto

O projeto foi dividido em **três níveis de complexidade**, simulando a evolução de um sistema real:

### 🟦 Nível 1 – Cadastro de Territórios

- Criar uma `struct` chamada `Territorio` com os campos:
  - `char nome[30];`
  - `char cor[10];`
  - `int tropas;`
- Permitir o **cadastro de territórios** pelo usuário.
- Exibir todos os territórios cadastrados ao final do preenchimento.
- Utilização de:
  - Estruturas compostas (`struct`)
  - Vetor de `Territorio`
  - Laços de repetição para entrada e saída de dados

> ⚠️ No código final, a quantidade de territórios é informada pelo usuário e alocada dinamicamente, generalizando o requisito inicial de “5 territórios fixos”.

---

### 🟧 Nível 2 – Interatividade: Ataque entre Territórios

Evolução do sistema com simulação de **batalhas**:

- Utilização de **alocação dinâmica** para o vetor de territórios:
  - `Territorio* mapa = calloc(qtdTerritorios, sizeof(Territorio));`
- Implementação da função:

```c
void atacar(Territorio* atacante, Territorio* defensor);
