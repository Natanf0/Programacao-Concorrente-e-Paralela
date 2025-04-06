fluxo: gcc -o gerar_vetor gerar_vetor.c
       gcc -o produto_interno produto_interno.c
       ./gerar_vetor 200 vetor.bin
       ./produto_interno vetor.bin 20
       