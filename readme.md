<div align="center">

# Labirinto em Grafos

### Busca em Largura × Dijkstra: o caminho mais curto nem sempre é o mais barato

Trabalho final de **Estrutura de Dados**, do curso de Engenharia de Software (2º período) do<br>
**Centro Universitário – Católica de Santa Catarina**, em Joinville/SC.

</div>

---

## Sobre o projeto

Um labirinto de **25 × 71** casas é modelado como **grafo** e resolvido duas vezes:

| Etapa | Algoritmo | Pergunta que responde |
|:---:|:---:|---|
| 1 | **Busca em largura (BFS)** | Qual é o caminho com **menos passos**? |
| 2 | **Dijkstra** | Qual é o caminho **mais barato**, sabendo que a lama custa mais? |

No final, o programa **compara os dois caminhos**: passos, custo e se são a mesma sequência de casas.

---

## O labirinto

| Símbolo | Significado | Custo para entrar |
|:---:|---|:---:|
| `␣` (espaço) | caminho livre | 1 |
| `~` | lama | **3** |
| `#` | parede | não passa |
| `E` | entrada | não conta |
| `S` | saída | 1 |

---

## Validação

Antes de qualquer busca, o programa confere se:

- cada linha tem exatamente **71 caracteres**;
- só aparecem os símbolos válidos (`espaço`, `~`, `#`, `E`, `S`);
- existe **exatamente um `E`** e **um `S`**.

Se algo estiver errado, ele mostra o erro e **não executa as buscas**.

---

## Tecnologias

- **C puro**: só `stdio.h` e `stdlib.h`, sem recursos de C++
- Fila em vetor alocado com `malloc` para a busca em largura
- Busca linear pelo mínimo no Dijkstra

---

## Equipe

<table>
  <tr>
    <td align="center">
      <a href="https://github.com/BeatrizCercal">
        <img src="https://github.com/BeatrizCercal.png" width="100px;" alt="Beatriz"/><br>
        <sub><b>Beatriz Cercal Cachoeira</b></sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/miguelrcha">
        <img src="https://github.com/miguelrcha.png" width="100px;" alt="Miguel"/><br>
        <sub><b>Miguel Rochar Xavier</b></sub>
      </a>
    </td>
  </tr>
</table>

**Professor:** Manfred Heil Junior