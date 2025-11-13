#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // Para std::max

/**
 * @brief Enum para clareza na tabela 'd' (decisões).
 * Isso ajuda a rastrear se incluímos o item ou herdamos o valor anterior.
 */
enum Decision {
    NONE, // Usado para inicialização (casos base)
    SKIP, // Item NÃO incluído (valor veio da linha de cima)
    KEEP  // Item INCLUÍDO (valor veio da soma do item + subproblema restante)
};

/**
 * @brief Resolve o Problema da Mochila 0/1 usando Programação Dinâmica.
 * * Esta função preenche as tabelas 'K' (valores) e 'd' (decisões).
 * O valor máximo possível estará em K[n][W].
 * * @param W Capacidade máxima da mochila.
 * @param wt Vetor de pesos dos itens (0-indexed).
 * @param val Vetor de valores dos itens (0-indexed).
 * @param n Número total de itens.
 * @param K Tabela de DP (passada por referência) a ser preenchida.
 * K[i][w] guardará o valor máximo considerando itens 1..i com capacidade w.
 * @param d Tabela de decisões (passada por referência) para reconstrução.
 */
void knapsack01(int W, const std::vector<int>& wt, const std::vector<int>& val, int n,
                std::vector<std::vector<int>>& K,
                std::vector<std::vector<Decision>>& d) {
    
    // Redimensiona as tabelas para (n+1) x (W+1)
    // Usamos +1 para acomodar os casos base (0 itens ou capacidade 0).
    K.resize(n + 1, std::vector<int>(W + 1));
    d.resize(n + 1, std::vector<Decision>(W + 1, Decision::NONE));

    // --- Construção da Tabela (Bottom-Up) ---
    // 'i' representa o item atual sendo considerado (de 1 a n)
    // 'w' representa a capacidade atual da mochila (de 0 a W)
    for (int i = 0; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            
            // Casos base: sem itens ou sem capacidade
            if (i == 0 || w == 0) {
                K[i][w] = 0;
                d[i][w] = Decision::NONE;
            }
            // Se o peso do item atual cabe na capacidade atual 'w'
            // Nota: wt[i-1] porque os vetores de entrada são 0-indexed
            else if (wt[i - 1] <= w) {
                
                int val_exclude = K[i - 1][w]; // Valor se NÃO pegarmos o item
                int val_include = val[i - 1] + K[i - 1][w - wt[i - 1]]; // Valor se PEGARMOS
                
                if (val_include > val_exclude) {
                    // --- Decisão: KEEP (Vale a pena levar) ---
                    K[i][w] = val_include;
                    d[i][w] = Decision::KEEP;
                } else {
                    // --- Decisão: SKIP (Melhor deixar pra lá) ---
                    K[i][w] = val_exclude;
                    d[i][w] = Decision::SKIP;
                }
            }
            else {
                // Se o item é mais pesado que a capacidade atual 'w', não podemos levar.
                // --- Decisão: SKIP Obrigatório ---
                K[i][w] = K[i - 1][w];
                d[i][w] = Decision::SKIP;
            }
        }
    }
    // Ao final, K[n][W] contém o valor máximo acumulado.
}

/**
 * @brief Imprime os itens selecionados recursivamente usando a tabela de decisões.
 * * @param d A tabela de decisões preenchida.
 * @param wt Vetor de pesos (para referência na impressão).
 * @param val Vetor de valores (para referência na impressão).
 * @param i Índice do item atual (começa em n).
 * @param w Capacidade atual (começa em W).
 */
void printSelectedItems(const std::vector<std::vector<Decision>>& d, 
                        const std::vector<int>& wt, 
                        const std::vector<int>& val, 
                        int i, int w) {
    
    // Caso base: sem itens ou capacidade zerada
    if (i == 0 || w == 0) {
        return;
    }

    // Se a decisão foi KEEP, significa que este item faz parte da solução ótima.
    if (d[i][w] == Decision::KEEP) {
        // Chama recursivamente para os itens anteriores, com capacidade reduzida
        // Nota: Chamamos a recursão ANTES de imprimir para mostrar na ordem do índice (1..n)
        // Se quiser ordem inversa (pilha), basta mover o print para antes da chamada.
        printSelectedItems(d, wt, val, i - 1, w - wt[i - 1]);
        
        std::cout << " -> Item " << i << " (Peso: " << wt[i - 1] 
                  << ", Valor: " << val[i - 1] << ")" << std::endl;
    } 
    else { // Decision::SKIP
        // Se pulamos o item, a capacidade 'w' se mantém, vamos para o item anterior.
        printSelectedItems(d, wt, val, i - 1, w);
    }
}

// Main para teste
int main() {
    std::cout << "---" << std::endl;
    std::cout << "Algoritmo: Problema da Mochila 0/1 (Knapsack)" << std::endl;
    std::cout << "Abordagem Bottom-Up com reconstrucao da solucao" << std::endl;
    std::cout << "---" << std::endl;

    // Tabelas de DP
    std::vector<std::vector<int>> K;
    std::vector<std::vector<Decision>> d;

    // --- Teste 1: Exemplo Clássico Simples ---
    std::cout << "--- Teste 1: Exemplo Basico ---" << std::endl;
    std::vector<int> val1 = {60, 100, 120};
    std::vector<int> wt1 = {10, 20, 30};
    int W1 = 50;
    int n1 = val1.size();

    std::cout << "Capacidade da Mochila: " << W1 << std::endl;
    std::cout << "Itens disponiveis (Peso, Valor): ";
    for(int k=0; k<n1; k++) std::cout << "(" << wt1[k] << ", " << val1[k] << ") ";
    std::cout << std::endl << std::endl;

    knapsack01(W1, wt1, val1, n1, K, d);

    std::cout << "Valor Maximo Obtido: " << K[n1][W1] << std::endl; // Esperado: 220
    std::cout << "Itens Escolhidos:" << std::endl;
    printSelectedItems(d, wt1, val1, n1, W1);
    std::cout << "---" << std::endl;


    // --- Teste 2: A falha do Algoritmo Guloso ---
    // Neste caso, um algoritmo guloso (por densidade) pegaria o item 1 (ratio 2.0),
    // depois não teria espaço para mais nada. Total = 10.
    // O ótimo é pegar os itens 2 e 3. Total = 18.
    std::cout << "--- Teste 2: Caso onde Guloso falha ---" << std::endl;
    std::vector<int> wt2 = {5, 4, 3};
    std::vector<int> val2 = {10, 9, 9}; // Densidades: 2.0, 2.25, 3.0
    int W2 = 7; 
    int n2 = wt2.size();

    std::cout << "Capacidade da Mochila: " << W2 << std::endl;
    std::cout << "Itens (Peso, Valor): ";
    for(int k=0; k<n2; k++) std::cout << "(" << wt2[k] << ", " << val2[k] << ") ";
    std::cout << std::endl << std::endl;

    knapsack01(W2, wt2, val2, n2, K, d);

    std::cout << "Valor Maximo Obtido: " << K[n2][W2] << std::endl; // Esperado: 18
    std::cout << "Itens Escolhidos:" << std::endl;
    printSelectedItems(d, wt2, val2, n2, W2);
    std::cout << "---" << std::endl;


    // --- Teste 3: Exemplo "Real": Kit de Sobrevivência ---
    std::cout << "--- Teste 3: Kit de Sobrevivencia (Capacidade 15kg) ---" << std::endl;
    // Itens: [Agua, Comida, Tenda, KitMedico, Camera]
    std::vector<int> wt3 =  {12, 2, 4, 1, 2};
    std::vector<int> val3 = {40, 50, 30, 10, 5}; 
    // Nota: Água vale muito, mas pesa muito. Comida vale muito e pesa pouco.
    int W3 = 15;
    int n3 = wt3.size();

    std::cout << "Analisando 5 itens..." << std::endl;
    knapsack01(W3, wt3, val3, n3, K, d);

    std::cout << "Valor de Utilidade Maxima: " << K[n3][W3] << std::endl;
    std::cout << "Itens Selecionados para levar:" << std::endl;
    printSelectedItems(d, wt3, val3, n3, W3);
    std::cout << "---" << std::endl;

    return 0;
}