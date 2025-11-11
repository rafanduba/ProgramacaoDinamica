#include <iostream>
#include <vector>
#include <string>
#include <limits> // Para std::numeric_limits (o DBL_MAX)
#include <iomanip> // Para std::setprecision

/**
 * @brief Preenche as tabelas de DP para o problema da Árvore de Busca Binária Ótima.
 * (Baseado no algoritmo OPTIMAL-BST do Cormen, 15.5)
 *
 * @param p Vetor de probabilidades das chaves REAIS (1-indexado, p[1..n]).
 * p[i] é a probabilidade de buscar a chave k_i.
 * @param q Vetor de probabilidades das chaves FICTÍCIAS (0-indexado, q[0..n]).
 * q[i] é a probabilidade de buscar um valor entre k_i e k_{i+1}.
 * q[0] é para valores < k_1. q[n] é para valores > k_n.
 * @param n O número de chaves reais.
 * @param e Tabela de custo (passada por referência). e[i][j] guardará
 * o custo esperado mínimo da árvore para as chaves k_i ... k_j.
 * @param w Tabela de pesos (passada por referência). w[i][j] guardará
 * a soma das probabilidades de p[i..j] e q[i-1..j].
 * @param root Tabela de raízes (passada por referência). root[i][j] guardará
 * o índice 'r' da raiz k_r que otimiza a árvore [i..j].
 */
void optimalBST(const std::vector<double>& p,
                const std::vector<double>& q,
                int n,
                std::vector<std::vector<double>>& e,
                std::vector<std::vector<double>>& w,
                std::vector<std::vector<int>>& root) {

    // --- 1. Redimensionar as tabelas ---
    // 'e' e 'w' precisam de índices [1..n+1][0..n]
    // 'root' precisa de índices [1..n][1..n]
    // Para simplificar, alocamos um pouco a mais (n+2 x n+2)
    e.resize(n + 2, std::vector<double>(n + 2));
    w.resize(n + 2, std::vector<double>(n + 2));
    root.resize(n + 2, std::vector<int>(n + 2));

    // --- 2. Casos Base (Comprimento l=0, ou j = i-1) ---
    // Uma árvore para o intervalo k_i..k_{i-1} só contém
    // a chave fictícia d_{i-1}. O custo é q[i-1].
    for (int i = 1; i <= n + 1; ++i) {
        e[i][i - 1] = q[i - 1];
        w[i][i - 1] = q[i - 1];
    }

    // --- 3. Construção da Tabela (Bottom-Up) ---
    // 'l' é o comprimento da cadeia de chaves (de 1 até n)
    for (int l = 1; l <= n; ++l) {
        
        // 'i' é a chave inicial da sub-árvore (de 1 até n-l+1)
        for (int i = 1; i <= n - l + 1; ++i) {
            
            // 'j' é a chave final da sub-árvore
            int j = i + l - 1;

            // Inicializa o custo como "infinito"
            e[i][j] = std::numeric_limits<double>::max();
            
            // Calcula w[i][j] usando a fórmula de DP
            // w(i, j) = w(i, j-1) + p[j] + q[j]
            w[i][j] = w[i][j - 1] + p[j] + q[j];

            // --- Encontra a raiz 'r' ótima (de i até j) ---
            // Este é o passo central da recorrência:
            // e[i,j] = min_{i<=r<=j} { e[i,r-1] + e[r+1,j] + w(i,j) }
            for (int r = i; r <= j; ++r) {
                
                // Custo se 'r' for a raiz:
                // (custo da sub-árvore esquerda) + (custo da sub-árvore direita) + (soma das probs)
                double cost = e[i][r - 1] + e[r + 1][j] + w[i][j];

                // Se este 'r' dá um custo menor, atualiza
                if (cost < e[i][j]) {
                    e[i][j] = cost;
                    root[i][j] = r; // Armazena 'r' como a melhor raiz para [i,j]
                }
            }
        }
    }
}

/**
 * @brief Imprime a estrutura da Árvore Ótima recursivamente.
 * (Baseado no procedimento CONSTRUCT-OPTIMAL-BST do Cormen)
 *
 * @param root A tabela de raízes preenchida.
 * @param keys Nomes das chaves reais (k1, k2, ...)
 * @param dummies Nomes das chaves fictícias (d0, d1, ...)
 * @param i Início do intervalo da sub-árvore.
 * @param j Fim do intervalo da sub-árvore.
 * @param parent_r A raiz da árvore-pai (para fins de impressão).
 * @param is_left_child Flag para saber se esta é uma sub-árvore esquerda.
 */
void printOptimalStructure(const std::vector<std::vector<int>>& root,
                           const std::vector<std::string>& keys,
                           const std::vector<std::string>& dummies,
                           int i, int j, int parent_r, bool is_left_child) {
    
    // --- Caso Base: Sub-árvore vazia (contém uma chave fictícia) ---
    if (i > j) {
        // Se i = j+1, a sub-árvore contém a chave fictícia d_j
        if (j == i - 1) {
            std::string side = is_left_child ? "filho esquerdo" : "filho direito";
            std::cout << "\t" << dummies[j] << " e o " << side << " de " << keys[parent_r] << std::endl;
        }
        return;
    }

    // --- Passo Recursivo ---
    // 1. Encontra a raiz 'r' para este intervalo [i, j]
    int r = root[i][j]; // r é o índice da chave (ex: 2 para k2)

    // 2. Imprime a raiz atual
    if (parent_r == 0) {
        std::cout << keys[r] << " e a raiz da arvore." << std::endl;
    } else {
        std::string side = is_left_child ? "filho esquerdo" : "filho direito";
        std::cout << keys[r] << " e o " << side << " de " << keys[parent_r] << std::endl;
    }

    // 3. Chama recursivamente para a sub-árvore esquerda [i, r-1]
    printOptimalStructure(root, keys, dummies, i, r - 1, r, true);

    // 4. Chama recursivamente para a sub-árvore direita [r+1, j]
    printOptimalStructure(root, keys, dummies, r + 1, j, r, false);
}


// Main para teste
int main() {
    std::cout << "---" << std::endl;
    std::cout << "Algoritmo: Arvores de Busca Binaria Otimas (OBST)" << std::endl;
    std::cout << "Secao 15.5 do Cormen (3a ed.)" << std::endl;
    std::cout << "---" << std::endl;

    // --- Teste: Exemplo do Livro (Figuras 15.9 e 15.10) ---
    // n = 5 chaves
    int n = 5;

    // Chaves (para impressão)
    // (índice 0 é dummy, para alinhamento 1-indexado)
    std::vector<std::string> keys = {"", "k1", "k2", "k3", "k4", "k5"};
    std::vector<std::string> dummies = {"d0", "d1", "d2", "d3", "d4", "d5"};

    // Probabilidades das chaves p[1..n]
    // (índice 0 é dummy)
    std::vector<double> p = {0.0, 0.15, 0.10, 0.05, 0.10, 0.20};

    // Probabilidades das chaves fictícias q[0..n]
    std::vector<double> q = {0.05, 0.10, 0.05, 0.05, 0.05, 0.10};
    
    std::cout << "--- Teste: Exemplo do Livro (n=5) ---" << std::endl;
    std::cout << "i: \t 0 \t 1 \t 2 \t 3 \t 4 \t 5" << std::endl;
    std::cout << "p_i: \t \t 0.15 \t 0.10 \t 0.05 \t 0.10 \t 0.20" << std::endl;
    std::cout << "q_i: \t 0.05 \t 0.10 \t 0.05 \t 0.05 \t 0.05 \t 0.10" << std::endl;
    std::cout << "------------------------------------------" << std::endl;

    // Tabelas de DP
    std::vector<std::vector<double>> e; // Custo esperado
    std::vector<std::vector<double>> w; // Pesos (soma das probs)
    std::vector<std::vector<int>> root; // Raiz ótima

    // 1. Executa o algoritmo
    optimalBST(p, q, n, e, w, root);

    // 2. Imprime o resultado final (Custo da árvore completa e[1][n])
    std::cout << std::fixed << std::setprecision(2); // Formata para 2 casas decimais
    std::cout << "Custo esperado minimo (e[1][" << n << "]): " << e[1][n] << std::endl;
    // Resultado esperado: 2.75

    std::cout << "Raiz da arvore completa (root[1][" << n << "]): " << keys[root[1][n]] << std::endl;
    // Resultado esperado: k2
    std::cout << "------------------------------------------" << std::endl;

    // 3. Imprime a estrutura da árvore
    std::cout << "Estrutura da Arvore Otima:" << std::endl;
    // (parent_r=0 indica que é a chamada inicial, sem pai)
    printOptimalStructure(root, keys, dummies, 1, n, 0, false);
    
    std::cout << "---" << std::endl;
    
    // Saída esperada da estrutura (Figura 15.10):
    // k2 e a raiz da arvore.
    //     k1 e o filho esquerdo de k2
    //         d0 e o filho esquerdo de k1
    //         d1 e o filho direito de k1
    //     k5 e o filho direito de k2
    //         k4 e o filho esquerdo de k5
    //             k3 e o filho esquerdo de k4
    //                 d2 e o filho esquerdo de k3
    //                 d3 e o filho direito de k3
    //             d4 e o filho direito de k4
    //         d5 e o filho direito de k5
    
    return 0;
}