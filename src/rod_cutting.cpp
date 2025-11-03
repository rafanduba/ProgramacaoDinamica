#include <iostream> 
#include <vector>   
#include <algorithm> // Para std::max
#include <climits>   // Para INT_MIN (o menor valor inteiro possível)

/**
 * @brief Soluciona o Problema do Corte de Barras (Rod Cutting) 
 * usando programação dinâmica (abordagem bottom-up).
 *
 * @param prices Um vetor onde prices[i] é o preço de uma barra de comprimento i+1.
 * Ex: prices[0] = preço de barra tam 1
 * prices[1] = preço de barra tam 2
 * ...
 * O tamanho do vetor (prices.size()) é o maior pedaço
 * que temos um preço definido.
 * @param n O comprimento total da barra original a ser cortada.
 * @return int O lucro máximo que pode ser obtido.
 */
int cutRod(const std::vector<int>& prices, int n) {
    
    // Tabela de DP. val[i] vai guardar o lucro máximo para uma barra de comprimento i.
    // Criamos com tamanho n+1 para incluir os índices de 0 a n.
    std::vector<int> val(n + 1);

    // Caso base: uma barra de comprimento 0 tem lucro 0.
    val[0] = 0;

    int m = prices.size(); // O maior pedaço que tem um preço (ex: 10 no seu exemplo)

    // Loop para construir a tabela val[] de 1 até n.
    // 'j' é o comprimento da barra para o qual estamos calculando o lucro máximo (de 1 a n).
    for (int j = 1; j <= n; j++) {
        
        int max_val = INT_MIN; // Inicializa o lucro máximo para o comprimento 'j' como o menor valor

        // Loop interno para testar todos os cortes possíveis.
        // 'i' é o comprimento do *primeiro* pedaço cortado (de 1 até j).
        for (int i = 1; i <= j; i++) {
            
            // Precisamos do preço para um pedaço de comprimento 'i'.
            // Esse preço está em 'prices[i-1]' (porque o vetor é 0-indexado).
            // SÓ podemos fazer esse corte se tivermos um preço para ele,
            // ou seja, se 'i' for no máximo 'm' (tamanho do vetor de preços).
            if (i <= m) {
                // O lucro para este corte é:
                // (preço do pedaço 'i') + (lucro máximo do pedaço restante 'j-i')
                // O lucro de 'j-i' já foi calculado e está em val[j-i].
                max_val = std::max(max_val, prices[i - 1] + val[j - i]);
            }
        }
        
        // Após testar todos os 'i', guardamos o melhor lucro encontrado para o comprimento 'j'.
        // Se nenhum corte foi possível (ex: n=1 mas 'prices' estava vazio), max_val seria INT_MIN.
        val[j] = (max_val == INT_MIN) ? 0 : max_val;
    }

    // O resultado final é o lucro máximo para a barra de tamanho original 'n'.
    return val[n];
}

// Main para teste
int main() {
    // Este é o exemplo clássico do livro do Cormen
    // Comprimento (i): 1  2  3  4  5   6   7   8   9   10
    // Preço (p_i):     1  5  8  9  10  17  17  20  24  30

    std::cout << "---" << std::endl;
    std::cout << "Exemplo classico do livro do Cormen:" << std::endl;
    std::cout << "Comprimento (i): 1 2  3  4  5   6   7   8   9   10" << std::endl;
    std::cout << "Preco (p_i):     1 5  8  9  10  17  17  20  24  30" << std::endl;
    std::cout << "---" << std::endl;

    // Nosso vetor `prices` é 0-indexado:
    // prices[0] = 1 (preço p/ tam 1)
    // prices[1] = 5 (preço p/ tam 2)
    // ...
    // prices[9] = 30 (preço p/ tam 10)
    std::vector<int> prices = {1, 5, 8, 9, 10, 17, 17, 20, 24, 30};

    // --- Teste 1 ---
    // Comprimento total n = 7
    std::cout << "--- Teste 1: Comprimento total n = 7 ---" << std::endl;
    int n1 = 7;
    int max_revenue1 = cutRod(prices, n1);
    std::cout << "Comprimento da barra: " << n1 << std::endl;
    std::cout << "Lucro maximo: " << max_revenue1 << std::endl; // Resultado esperado: 18
    std::cout << "(Corte em pedacos de 1 (1) e 6 (17))" << std::endl;
    std::cout << std::endl;
    std::cout << "---" << std::endl;

    // --- Teste 2 ---
    // Comprimento total n = 10
    std::cout << "--- Teste 2: Comprimento total n = 10 ---" << std::endl;
    int n2 = 10;
    int max_revenue2 = cutRod(prices, n2);
    std::cout << "Comprimento da barra: " << n2 << std::endl;
    std::cout << "Lucro maximo: " << max_revenue2 << std::endl; // Resultado esperado: 30
    std::cout << "(Pedaco unico de 10)" << std::endl;
    std::cout << "---" << std::endl;

    // --- Teste 3 ---
    // Teste onde n > tamanho do vetor de preços (n=13, mas preços só são tabelados até 10)
    // O algoritmo deve encontrar a melhor combinação de pedaços de 1 a 10.
    std::cout << "--- Teste 3: Comprimento total n = 13 ---" << std::endl;
    int n3 = 13;
    int max_revenue3 = cutRod(prices, n3);
    std::cout << "Comprimento da barra: " << n3 << " (precos so ate 10)" << std::endl;
    std::cout << "Lucro maximo: " << max_revenue3 << std::endl; // Resultado esperado: 38
    std::cout << "(Corte em pedacos de 3 (8) e 10 (30))" << std::endl;
    std::cout << "---" << std::endl;

    return 0;
}