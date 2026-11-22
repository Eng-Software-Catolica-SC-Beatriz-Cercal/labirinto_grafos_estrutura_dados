//------------------------------------------------------------------//
// Centro Universitário - Católica de Santa Catarina - Joinville/SC //
// Engenharia de Software - Terceiro período                        //
// Estrutura de Dados - Grafos                                      //
// Professor Manfred Heil Junior                                    //
// 22/11/2026                                                       //
//------------------------------------------------------------------//

//------------------------------------------------------------------//
//                           DECISÕES                               //
// 1- Labirinto: 25 x 71 em char labirinto [LINHAS] [COLUNAS + 1];  //
// 2- Validação: 71 caracteres por linha. Só valem: espaço (normal) //
// ~ (lama), # (parede), E (entrada) e S (saída);                   //
// 3- Peso: entrar na lama = 3, resto = 1;                          //
// 4- Vizinhos: cima, baixo, esquerda e direita;                    //
// 5- Desempate Dijkstra: menor índice. Pai só troca se achar um    //
// caminho mais barato (em empate, mantém o primeiro);              //
// 6- Saída: (linha, coluna) a partir de 0, contando a borda.       //
//------------------------------------------------------------------//

