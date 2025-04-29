Autor: Natan Ferreira 

Objetivo: Calcular o produto matricial A x B de forma concorrente.
Como executar (Linux): Na raiz do projeto, abra o terminal e digite "bash pipeline.sh". Para outras execuções, se nao quiser regerar as matrizes, basta executar apénas "bash processamento.sh"

Considerações: 
1. O programa 'geraMatrizBinario.c' recebe via CLI as dimensões da matriz, o nome do arquivo binário que deve gerar e uma flag se deve gerar ou não a transposta da matriz. Mas por quê estamos interessados em gerar uma matriz transposta ? Resumidamente, para aproveitar a memória cache. O produto A x BT (Onde BT é B Transposta) faz o melhor uso do princípio da localidade e economiza tempo de acesso à memória. Claro que neste caso está sendo gasto tempo gerando a transposta, mas repare que são códigos separados, então uma vez que as matrizes são geradas (executando dados.sh), os arquivos binários correspondentes serão gerados e poderão ser usados quantas vezes desejarmos. Assim, é possível processá-las de diversas formas, diversas vezes. Caso contrário, a cada processamento seria gasto mais tempo com cache miss. O script em 'dados.sh' executa este programa pedindo para gerar uma matriz A 45.000 x 45.000 (sem a transposta) e uma matriz B 45.000 x 45.000 (com a transposta).

2. No script "processamento.sh" é definido que o produto matricial, realizado no programa 'matmat.c', deve ser executado com 1, 2, 3, 5, 10 e 20 threads onde é repetido 10 vezes para cada número de threads.
O programa 'matmat.c' recebe por entrada via CLI os descritores dos arquivos das matrizes e o número de threads. Durante a execução, é marcado os tempos de carregamento de dados, execução propriamente dita pelas threads, desalocação dos recursos e o total.

3. O programa 'plot.py' consome os tempos 