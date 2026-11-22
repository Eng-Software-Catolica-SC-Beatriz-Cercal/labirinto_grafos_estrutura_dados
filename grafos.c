//------------------------------------------------------------------//
// Centro Universitário - Católica de Santa Catarina - Joinville/SC //
// Engenharia de Software - Terceiro período                        //
// Estrutura de Dados - Grafos                                      //
// Professor Manfred Heil Junior                                    //
// 22/11/2026                                                       //
//------------------------------------------------------------------//

//------------------------------------------------------------------//
//                           DECISÕES                               //
// 1- 
// 2-
// 3- 
// 4-
// 5- 
// 6- 

Labirinto: 25 x 71 em char lab[LINHAS][COLUNAS + 1].             //
// Validação: 71 caracteres por linha, só ' ' ~ # E S, 1 E e 1 S.   //
// Grafo: grade implícita; célula = linha * COLUNAS + coluna, e os  //
//   vizinhos são calculados na hora (matriz de pesos ficaria       //
//   quase vazia). Vetores grandes globais para não estourar pilha. //
// Peso: entrar na lama = 3, resto = 1 (grafo dirigido).            //
// Vizinhos: cima, baixo, esquerda, direita.                        //
// Desempate Dijkstra: menor índice; pai só muda se custo < atual.  //
// Saída: (linha, coluna) a partir de 0, contando a borda.          //