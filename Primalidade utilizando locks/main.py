import csv
from collections import defaultdict


dados = []

with open('saida.csv', 'r') as f:
    reader = csv.reader(f)
    for linha in reader:
        try:
            n_threads = int(linha[0])
            n_valor = int(linha[1])
            tempo = float(linha[2])
            dados.append((n_valor, n_threads, tempo))
        except ValueError:
            continue 

# Agrupa tempos por valor de N
tempos_por_N = defaultdict(list)
tempos_Tp1 = {}  # tempo com 1 thread para cada N

for n, threads, tempo in dados:
    tempos_por_N[n].append((threads, tempo))
    if threads == 1:
        tempos_Tp1[n] = tempo

# Calcula e exibe acelerações e eficiências
print("\n\nCalculando as acelerações A(n,p) e Eficiências E(n,p) :\n\n")

for n in sorted(tempos_por_N.keys()):
    tp1 = tempos_Tp1[n]
    for threads, tempo in sorted(tempos_por_N[n]):
        aceleracao = tp1 / tempo
        if(aceleracao==1.0):
            continue # não é necessário printar quando é 1 (sequencial)
        eficiencia = aceleracao / threads
        print(f"A({n}, {threads}) = {aceleracao:.4f}, E({n}, {threads}) = {eficiencia:.4f}")
    print()
