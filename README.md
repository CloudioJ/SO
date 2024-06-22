# SO

## Trabalho Escalonador

Este trabalho envolve a implementação do algoritmo de escalonamento "Shortest Job First" (SJF) para processar tarefas. Ele lê uma lista de processos e seus tempos de execução a partir de um arquivo, escalona-os em múltiplos processadores e salva os resultados do escalonamento em um arquivo.

### Funcionalidades

- Suporta escalonamento em até 5 processadores.
- Lê nomes dos processos e seus tempos a partir de um arquivo de entrada.
- Utiliza o algoritmo SJF para encontrar a menor tarefa e atribuí-la aos processadores.
- Salva os resultados do escalonamento em um arquivo de saída chamado resultados.txt.

### Formato do Arquivo de Entrada

O arquivo de entrada deve conter os nomes dos processos e seus tempos de execução, separados por espaços. Cada nome de processo e seu tempo de execução devem estar em uma nova linha. Por exemplo:

    process1 10
    process2 5
    process3 7
    process4 3

### Argumentos da Linha de Comando

O programa espera dois argumentos na linha de comando:

- Arquivo de Entrada: O caminho para o arquivo de entrada contendo os nomes e tempos dos processos.
- Número de Processadores: O número de processadores a serem usados para o escalonamento (de 1 a 5).

Exemplo de Comando:

      ./trabalho_escalonador entrada.txt 3

  Este comando executa o escalonador usando os processos listados em entrada.txt e os escalona em 3 processadores.

### Saída

O programa gera um arquivo de saída chamado resultados.txt com os resultados do escalonamento. As tarefas de cada processador são listadas junto com seus tempos de início e fim.
