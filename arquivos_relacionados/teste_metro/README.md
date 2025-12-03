#Projeto: Construção do metrô de NY e busca de menores caminhos por BFS

## Estrutura do Arquivo de Entrada (.txt)

Cada linha segue o formato:

Linha:Estação A, Estação B, Estação C, ...


### Grafo resultante:

- **Second Av**
  - 96 St ↔ 86 St ↔ Jamaica Center ↔ 72 St ↔ Canal St
- **Queens – Archer**
  - Jamaica Center ↔ Sutphin Blvd-Archer Av ↔ Jamaica-Van Wyck
- **Manhattan Bridge**
  - Canal St ↔ 86 St


### Exemplo de execução:

Entrada:
Start = "Jamaica-Van Wyck"
End   = "96 St"

Saída esperada:
Caminho encontrado (Jamaica-Van Wyck -> 96 St):
Jamaica-Van Wyck 6
96 St 0

Saída:
Caminho encontrado (Jamaica-Van Wyck -> 96 St):
Jamaica-Van Wyck 6
Sutphin Blvd-Archer Av 5
Jamaica Center 2
86 St 1
96 St 0
