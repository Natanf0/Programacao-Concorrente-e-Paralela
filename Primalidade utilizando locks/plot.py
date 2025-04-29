import matplotlib.pyplot as plt
import csv
from collections import defaultdict
import numpy as np

# Dicionário para armazenar os tempos por número de threads e valor de N
dados = defaultdict(lambda: defaultdict(list))

# Leitura e processamento do arquivo CSV
with open('saida.csv', 'r') as f:
    reader = csv.reader(f)
    for linha in reader:
        if not linha or linha[0].strip() == '' or 'Nº de Threads' in linha[0] or 'repetição' in linha[0]:
            continue  # Pular cabeçalhos, linhas vazias e linhas de repetição
        else:
            try:
                threads = int(linha[0])  # Número de threads
                N = int(linha[1])        # Valor de N
                tempo_execucao = float(linha[2])  # Tempo de execução
                dados[threads][N].append(tempo_execucao)
            except ValueError:
                continue  # Se a linha não contiver números válidos, ignorar

# Cálculo das médias de tempos para cada (threads, N)
medias = defaultdict(lambda: defaultdict(float))
for threads, valores in dados.items():
    for N, tempos in valores.items():
        medias[threads][N] = sum(tempos) / len(tempos)

# Organiza os dados para o gráfico
threads_ordenadas = sorted(medias.keys())
N_ordenados = sorted(set(N for t in medias.values() for N in t.keys()))

# Preparar os dados para o gráfico
bar_data = {N: [] for N in N_ordenados}
for threads in threads_ordenadas:
    for N in N_ordenados:
        bar_data[N].append(medias[threads].get(N, 0))  # Caso não haja dado para (threads, N), coloca 0

# Gráfico de barras empilhadas
fig, ax = plt.subplots(figsize=(10, 6))

x = np.arange(len(threads_ordenadas))

# Usando a paleta de cores tab20 (isso garante uma boa visualização para múltiplos valores de N)
colors = plt.cm.tab20.colors  # Obtendo a paleta de cores

# Plotando as barras empilhadas
bottom = np.zeros(len(threads_ordenadas))
for i, N in enumerate(N_ordenados):
    ax.bar(x, bar_data[N], label=f'N={N}', bottom=bottom, color=colors[i % len(colors)])  # Ciclando as cores se necessário
    bottom += np.array(bar_data[N])  # Atualizando o fundo das barras empilhadas

# Configurações do gráfico
ax.set_xlabel('Número de Threads')
ax.set_ylabel('Tempo Médio de Execução (s)')
ax.set_title('Tempo Médio de Execução por Número de Threads e Valor de N')

# Definindo o eixo X com os números de threads
ax.set_xticks(x)
ax.set_xticklabels(threads_ordenadas)

# Usando escala logarítmica no eixo Y para capturar melhor os valores pequenos
ax.set_yscale('log')

# Configuração de legenda
ax.legend(title='Valor de N', bbox_to_anchor=(1.05, 1), loc='upper left')

# Exibir o gráfico
plt.tight_layout()
plt.show()
