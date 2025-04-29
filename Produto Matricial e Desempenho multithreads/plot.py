import matplotlib.pyplot as plt
import csv
from collections import defaultdict
import numpy as np

# Dicionário para armazenar os dados por número de threads
dados = defaultdict(list)
threads_atual = None

# Leitura e processamento do arquivo
with open('saida.csv', 'r') as f:
    reader = csv.reader(f)
    for linha in reader:
        if not linha or 'Carregamento' in linha[0] or linha[0].strip() == '':
            continue
        elif 'Executando com' in linha[0]:
            threads_atual = int(linha[0].split(' ')[2])
        else:
            tempos = list(map(float, linha))
            dados[threads_atual].append(tempos)

# Cálculo das médias
medias = {}
for threads, tempos in dados.items():
    tempos_transpostos = list(zip(*tempos))
    medias[threads] = [sum(coluna) / len(coluna) for coluna in tempos_transpostos]

# Organiza os dados para o gráfico
threads_ordenadas = sorted(medias.keys())
carregamento = [medias[t][0] for t in threads_ordenadas]
execucao     = [medias[t][1] for t in threads_ordenadas]
desalocacao  = [medias[t][2] for t in threads_ordenadas]

# Gráfico de barras empilhadas
fig, ax = plt.subplots()

x = np.arange(len(threads_ordenadas))

bar1 = ax.bar(x, carregamento, label='Carregamento', color='skyblue')
bar2 = ax.bar(x, execucao, bottom=carregamento, label='Execução', color='orange')
bar3 = ax.bar(x, desalocacao, bottom=[c+e for c, e in zip(carregamento, execucao)], label='Desalocação', color='lightgreen')

# Configurações do gráfico
ax.set_xlabel('Número de Threads')
ax.set_ylabel('Tempo Médio (s)')
ax.set_title('Tempo Médio por Etapa (Barras Empilhadas)')
ax.set_xticks(x)
ax.set_xticklabels(threads_ordenadas)
ax.legend()
plt.tight_layout()
plt.show()
