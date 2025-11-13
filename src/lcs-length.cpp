#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // Para std::max

/**
 * @brief Enum para clareza na tabela 'b' (direções)..
 */
enum Direction
{
    NONE,     // Usado para inicialização (ou c[0][0])
    DIAGONAL, // Seta aponta para Cima-Esquerda (match)
    UP,       // Seta aponta para Cima
    LEFT      // Seta aponta para Esquerda
};

/**
 * @brief Calcula as tabelas de comprimento (c) e direção (b) para a LCS.
 * (Baseado no algoritmo LCS-LENGTH do Cormen, 15.4)
 *
 * Esta função preenche as tabelas 'c' e 'b' usando programação dinâmica.
 * O comprimento final da LCS estará em c[m][n].
 *
 * @param X A primeira string (sequência), de comprimento m.
 * @param Y A segunda string (sequência), de comprimento n.
 * @param c Tabela de DP (passada por referência) a ser preenchida.
 * c[i][j] guardará o comprimento da LCS de X[1..i] e Y[1..j].
 * @param b Tabela de direções (passada por referência) para reconstrução.
 */
void lcsLength(const std::string &X, const std::string &Y,
               std::vector<std::vector<int>> &c,
               std::vector<std::vector<Direction>> &b)
{

    int m = X.length();
    int n = Y.length();

    // --- Correção Importante ---
    // Recria as tabelas do zero para evitar tamanhos incorretos entre testes.
    // Isso garante que todas as linhas tenham o mesmo número de colunas.
    c = std::vector<std::vector<int>>(m + 1, std::vector<int>(n + 1, 0));
    b = std::vector<std::vector<Direction>>(m + 1, std::vector<Direction>(n + 1, Direction::NONE));

    // --- Casos Base ---
    // Se uma das sequências é vazia (i=0 ou j=0), a LCS é 0.

    // Loop para preencher c[i][0] = 0 para todo i
    for (int i = 0; i <= m; i++)
    {
        c[i][0] = 0;
    }
    // Loop para preencher c[0][j] = 0 para todo j
    for (int j = 0; j <= n; j++)
    {
        c[0][j] = 0;
    }

    // --- Construção da Tabela (Bottom-Up) ---
    // 'i' representa o prefixo de X (de 1 a m)
    // 'j' representa o prefixo de Y (de 1 a n)
    for (int i = 1; i <= m; i++)
    {
        for (int j = 1; j <= n; j++)
        {

            // Ajuste de índice:
            // No livro, as sequências são 1-indexadas (X[i]).
            // Em C++, std::string é 0-indexada (X[i-1]).

            if (X[i - 1] == Y[j - 1])
            {
                // --- Caso 1: Match! (X[i] == Y[j]) ---
                // O comprimento é 1 + o comprimento da LCS de X[i-1] e Y[j-1].
                c[i][j] = c[i - 1][j - 1] + 1;
                // A seta aponta para a diagonal (cima-esquerda).
                b[i][j] = Direction::DIAGONAL;
            }
            else if (c[i - 1][j] >= c[i][j - 1])
            {
                // --- Caso 2: Não há match. ---
                // O comprimento é o máximo entre as sub-soluções.
                // Aqui, o subproblema "de cima" (LCS de X[i-1] e Y[j]) é maior ou igual.
                c[i][j] = c[i - 1][j];
                // A seta aponta para Cima.
                b[i][j] = Direction::UP;
            }
            else
            {
                // --- Caso 3: Não há match. ---
                // O subproblema "da esquerda" (LCS de X[i] e Y[j-1]) é maior.
                c[i][j] = c[i][j - 1];
                // A seta aponta para Esquerda.
                b[i][j] = Direction::LEFT;
            }
        }
    }
    // Ao final, as tabelas 'c' e 'b' estão preenchidas.
    // O comprimento da LCS(X, Y) está em c[m][n].
}

/**
 * @brief Imprime a LCS recursivamente usando a tabela de direções 'b'.
 * (Baseado no algoritmo PRINT-LCS do Cormen, 15.4)
 *
 * @param b A tabela de direções preenchida por lcsLength.
 * @param X A string original X (necessária para imprimir os caracteres).
 * @param i O índice atual em X (deve ser chamado com X.length()).
 * @param j O índice atual em Y (deve ser chamado com Y.length()).
 */
void printLCS(const std::vector<std::vector<Direction>> &b, const std::string &X, int i, int j)
{
    // Caso base da recursão: chegamos a uma string vazia.
    if (i == 0 || j == 0)
    {
        return;
    }

    // Segue a seta armazenada em b[i][j]
    if (b[i][j] == Direction::DIAGONAL)
    {
        // Se a seta é diagonal, encontramos um caractere da LCS.
        // Chamamos recursivamente para (i-1, j-1)
        printLCS(b, X, i - 1, j - 1);
        // Imprimimos o caractere *depois* da chamada recursiva,
        // para que a LCS seja impressa na ordem correta (e não inversa).
        std::cout << X[i - 1]; // (i-1) por causa da indexação-0 da string
    }
    else if (b[i][j] == Direction::UP)
    {
        // Se a seta é para cima, seguimos para (i-1, j)
        printLCS(b, X, i - 1, j);
    }
    else
    { // b[i][j] == Direction::LEFT
        // Se a seta é para esquerda, seguimos para (i, j-1)
        printLCS(b, X, i, j - 1);
    }
}

// Main para teste
int main()
{
    std::cout << "---" << std::endl;
    std::cout << "Algoritmo: Subsequencia Comum Mais Longa (LCS)" << std::endl;
    std::cout << "Secao 15.4 do Cormen (3a ed.)" << std::endl;
    std::cout << "---" << std::endl;

    // Tabelas de DP
    // c[i][j] = armazena o *comprimento*
    // b[i][j] = armazena a *direção* (seta)
    std::vector<std::vector<int>> c;
    std::vector<std::vector<Direction>> b;

    // --- Teste 1: Exemplo do Livro ---
    std::cout << "--- Teste 1: Exemplo do Livro ---" << std::endl;
    std::string X1 = "ABCBDAB";
    std::string Y1 = "BDCABA";

    std::cout << "String X: " << X1 << " (m = " << X1.length() << ")" << std::endl;
    std::cout << "String Y: " << Y1 << " (n = " << Y1.length() << ")" << std::endl;
    std::cout << std::endl;

    lcsLength(X1, Y1, c, b);
    int m1 = X1.length();
    int n1 = Y1.length();
    int lcs_len1 = c[m1][n1];

    std::cout << "Comprimento da LCS: " << lcs_len1 << std::endl; // Resultado esperado: 4
    std::cout << "LCS (reconstruida): ";
    printLCS(b, X1, m1, n1);
    std::cout << std::endl; // Resultado esperado: BCBA
    std::cout << "---" << std::endl;

    // --- Teste 2: Exemplo clássico ---
    std::cout << "--- Teste 2: Exemplo classico ---" << std::endl;
    std::string X2 = "AGGTAB";
    std::string Y2 = "GXTXAYB";

    std::cout << "String X: " << X2 << " (m = " << X2.length() << ")" << std::endl;
    std::cout << "String Y: " << Y2 << " (n = " << Y2.length() << ")" << std::endl;
    std::cout << std::endl;

    lcsLength(X2, Y2, c, b);
    int m2 = X2.length();
    int n2 = Y2.length();
    int lcs_len2 = c[m2][n2];

    std::cout << "Comprimento da LCS: " << lcs_len2 << std::endl; // Resultado esperado: 4
    std::cout << "LCS (reconstruida): ";
    printLCS(b, X2, m2, n2);
    std::cout << std::endl; // Resultado esperado: GTAB
    std::cout << "---" << std::endl;

    // --- Teste 3: Exemplo Biologia (DNA) ---
    // O Cormen usa a comparação de sequências de DNA como a
    // principal motivação para o problema de LCS.
    // DNA usa as bases A, C, G, T.
    std::cout << "--- Teste 3: Exemplo Biologia (DNA) ---" << std::endl;
    std::string X3 = "ACCGGTCGAGT";  // Organismo 1 (m=11)
    std::string Y3 = "GTCGTTCGGAAT"; // Organismo 2 (n=12)

    std::cout << "String X (DNA 1): " << X3 << " (m = " << X3.length() << ")" << std::endl;
    std::cout << "String Y (DNA 2): " << Y3 << " (n = " << Y3.length() << ")" << std::endl;
    std::cout << "(Encontrando a maior sequencia de bases em comum)" << std::endl;
    std::cout << std::endl;

    lcsLength(X3, Y3, c, b);

    int m3 = X3.length();
    int n3 = Y3.length();
    int lcs_len3 = c[m3][n3];

    std::cout << "Comprimento da LCS: " << lcs_len3 << std::endl; // Resultado esperado: 6

    std::cout << "LCS (reconstruida): ";
    printLCS(b, X3, m3, n3);
    std::cout << std::endl; // Resultado esperado: GTCGGA
}
