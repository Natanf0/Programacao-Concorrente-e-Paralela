import java.io.IOException;

public class Main {
    static int NUM_THREADS = 15;

    static int TAMANHO_VETOR = 100000000; // Tamanho dos vetores

    public static void main(String[] args) throws IOException {
        long tempoInicialSEMAlocacao = System.currentTimeMillis();

        Thread[] threads = new Thread[NUM_THREADS];
        ProdutoInterno[] produtosInterno = new ProdutoInterno[NUM_THREADS];
        int fatia = TAMANHO_VETOR/NUM_THREADS;

        // Sejam Sejam A = (a1, a2, . . . , aN )^T e B = (b1, b2, . . . , bN )^T dois vetores de double para o produto interno
        Vetor vetorA = new Vetor(TAMANHO_VETOR, 'A');
        Vetor vetorB = new Vetor(TAMANHO_VETOR, 'B');

        //GerarDados.gerarVetor(vetorA); // popula o arquivo A.bin com os dados do vetor A
        //GerarDados.gerarVetor(vetorB); // popula o arquivo B.bin com os dados do vetor B

        //GerarDados.printarVetor(vetorA, vetorB);

        long tempoInicialComAlocacao = System.currentTimeMillis();

        for(int i = 0; i<NUM_THREADS;i++){
            produtosInterno[i] = new ProdutoInterno(i*fatia, (i+1)*fatia);
             threads[i] = new Thread(produtosInterno[i]);
             threads[i].start();
        }

        double resultadoProdutoInterno = 0.;
        for(int i = 0; i<NUM_THREADS;i++){
            try{
                threads[i].join();
                resultadoProdutoInterno += produtosInterno[i].getProduto();
            }catch(InterruptedException e){
                e.printStackTrace();
            }
        } System.out.println("O metodo executou em " + (System.currentTimeMillis() - tempoInicialSEMAlocacao));
        System.out.println("Tempo total de execução = "+ (System.currentTimeMillis() - tempoInicialComAlocacao));

        System.out.println("produto interno : "+resultadoProdutoInterno);

    }

}