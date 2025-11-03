#include <iostream>
#include <vector>
#include <climits> // Para INT_MAX (infinito)

/**
 * @brief Soluciona o Problema da Multiplicação de Cadeias de Matrizes 
 * usando programação dinâmica (abordagem bottom-up).
 *
 * @param p Um vetor de dimensões. Se temos N matrizes (A1, A2, ..., AN),
 * o vetor 'p' terá N+1 elementos.
 * A matriz Ai tem dimensões p[i-1] x p[i].
 *
 * Ex: p = {10, 20, 30} -> A1 é (10x20), A2 é (20x30).
 *
 * @return int O número mínimo de multiplicações escalares necessárias.
 */
int matrixChainOrder(const std::vector<int>& p) {
    
    // Se p.size() for 1 ou 0, não há matrizes (ou só 1 dimensão),
    // então o custo é 0. (p.size() precisa ser pelo menos 2 para 1 matriz).
    if (p.size() < 2) {
        return 0;
    }

    // 'n' é o número de matrizes.
    // Se p = {p0, p1, p2, ..., pn}, temos n matrizes.
    // p.size() = n + 1.
    int n = p.size() - 1;

    // Tabela de DP. m_table[i][j] vai guardar o custo mínimo
    // para multiplicar a cadeia de matrizes de Ai até Aj.
    // Usamos (n+1) x (n+1) para usar índices 1-based (de 1 a n),
    // que facilita a tradução do pseudo-algoritmo do Cormen.
    std::vector<std::vector<int>> m_table(n + 1, std::vector<int>(n + 1));

    // --- Passo 1: Casos Base ---
    // O custo para "multiplicar" uma cadeia de comprimento 1 (uma única matriz) é 0.
    // m_table[i][i] = 0 para todo i.
    for (int i = 1; i <= n; i++) {
        m_table[i][i] = 0;
    }

    // --- Passo 2: Construir a tabela (Bottom-Up) ---
    // 'L' é o comprimento da cadeia de matrizes que estamos considerando.
    // Começamos com L=2 (ex: A1*A2), depois L=3 (ex: A1*A2*A3), ..., até L=n.
    for (int L = 2; L <= n; L++) {
        
        // 'i' é o índice da matriz inicial da cadeia.
        // Vai de A1 até A(n-L+1).
        for (int i = 1; i <= n - L + 1; i++) {
            
            // 'j' é o índice da matriz final da cadeia.
            // j = i + L - 1
            int j = i + L - 1;

            // Inicializa o custo para m_table[i][j] como infinito.
            m_table[i][j] = INT_MAX;

            // 'k' é o ponto de divisão (o "parêntese" principal).
            // Estamos testando onde dividir a cadeia (Ai ... Aj)
            // (Ai ... Ak) * (Ak+1 ... Aj)
            // 'k' vai de i até j-1.
            for (int k = i; k <= j - 1; k++) {
                
                // Custo = (custo de calcular (Ai...Ak))
                //       + (custo de calcular (Ak+1...Aj))
                //       + (custo de multiplicar os dois resultados)
                
                // (Ai...Ak) resulta numa matriz de p[i-1] x p[k]
                // (Ak+1...Aj) resulta numa matriz de p[k] x p[j]
                // O custo da multiplicação final é p[i-1] * p[k] * p[j]
                
                // Note que o vetor 'p' é 0-indexado.
                int cost = m_table[i][k] + m_table[k + 1][j] + p[i - 1] * p[k] * p[j];

                // Se este 'k' (ponto de divisão) dá um custo menor, atualizamos.
                if (cost < m_table[i][j]) {
                    m_table[i][j] = cost;
                }
            }
        }
    }

    // O resultado final é o custo mínimo para a cadeia inteira, de A1 até An.
    return m_table[1][n];
}

// Main para teste
int main() {
    // Este é o exemplo clássico do Cormen (Cap. 15).
    // Temos 6 matrizes (n=6).
    // A1: 30x35
    // A2: 35x15
    // A3: 15x5
    // A4: 5x10
    // A5: 10x20
    // A6: 20x25
    // O vetor de dimensões 'p' (tamanho n+1 = 7) é:
    // p = {30, 35, 15, 5, 10, 20, 25}

    std::cout << "---" << std::endl;
    std::cout << "Exemplo clássico do livro do Cormen:" << std::endl;
    std::cout << "Matrizes: A1(30x35), A2(35x15), A3(15x5), A4(5x10), A5(10x20), A6(20x25)" << std::endl;
    std::cout << "Dimensoes (p): 30, 35, 15, 5, 10, 20, 25" << std::endl;
    std::cout << std::endl;

    // O algoritmo encontra a parentetização ótima.
    // Existem 42 maneiras de parentetizar 6 matrizes (Número de Catalan C(5)).
    // A solução ótima encontrada pelo algoritmo é: (A1 * (A2 * A3)) * ((A4 * A5) * A6)
    // Vamos calcular o custo dessa solução:
    //
    // 1. (A2 * A3) -> (35x15) * (15x5)
    //    Custo: 35 * 15 * 5 = 2625
    //    Resultado (Res1): 35x5
    //    Custo Total até agora: 2625
    //
    // 2. (A4 * A5) -> (5x10) * (10x20)
    //    Custo: 5 * 10 * 20 = 1000
    //    Resultado (Res2): 5x20
    //    Custo Total até agora: 3625
    //
    // 3. A1 * (Res1) -> (30x35) * (35x5) [Onde Res1 = A2*A3]
    //    Custo: 30 * 35 * 5 = 5250
    //    Resultado (Res3): 30x5
    //    Custo Total até agora: 7875
    //
    // 4. (Res2) * A6 -> (5x20) * (20x25) [Onde Res2 = A4*A5]
    //    Custo: 5 * 20 * 25 = 2500
    //    Resultado (Res4): 5x25
    //    Custo Total até agora: 10375
    //
    // 5. (Res3) * (Res4) -> (30x5) * (5x25) [O "split" final]
    //    Custo: 30 * 5 * 25 = 3750
    // Custo Total = 2625 (p/ A2A3) + 1000 (p/ A4A5) + 5250 (p/ A1(..)) + 2500 (p/ (..)A6) + 3750 (p/ split final)
    // Custo Total = 15125
    // 

    std::vector<int> dims = {30, 35, 15, 5, 10, 20, 25};

    int n_matrizes = dims.size() - 1;
    std::cout << "Numero de matrizes: " << n_matrizes << std::endl;
    
    int min_cost = matrixChainOrder(dims);
    
    std::cout << "Custo minimo de multiplicacoes: " << min_cost << std::endl;
    // Resultado esperado: 15125

    std::cout << "---" << std::endl;

    // Exemplo mais simples: A1(10x20), A2(20x30), A3(30x10)
    // p = {10, 20, 30, 10}
    // Opção 1: (A1*A2)*A3
    //   A1*A2: 10*20*30 = 6000 (Resulta em 10x30)
    //   (Res)*A3: 10*30*10 = 3000
    //   Total: 9000
    // Opção 2: A1*(A2*A3)
    //   A2*A3: 20*30*10 = 6000 (Resulta em 20x10)
    //   A1*(Res): 10*20*10 = 2000
    //   Total: 8000 (Melhor)

     std::cout << "Outro exemplo:" << std::endl;
    std::cout << "Matrizes: A1(10x20), A2(20x30), A3(30x10)" << std::endl;
    std::cout << "Dimensoes (p): 10, 20, 30, 10" << std::endl;
    std::cout << std::endl;

    std::vector<int> dims2 = {10, 20, 30, 10};
    std::cout << "Numero de matrizes: " << (dims2.size() - 1) << std::endl;
    std::cout << "Custo minimo de multiplicacoes: " << matrixChainOrder(dims2) << std::endl;
    // Resultado esperado: 8000

    return 0;
}