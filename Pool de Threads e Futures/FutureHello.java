/* Disciplina: Programacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 11 */
/* Codigo: Exemplo de uso de futures */
/* -------------------------------------------------------------------*/

import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

import java.util.ArrayList;
import java.util.List;


//classe runnable
class MyCallable implements Callable<Long> {
  //construtor
  long num;
  MyCallable(long num) {
    this.num=num;
  }
 
  //funcao para determinar se um numero  ́e primo
  Long ehPrimo(long n) {
    int i;
    if(n<=1) return 0L;
    if(n==2) return 1L;
    if(n%2==0) return 0L;
    for(i=3; i< Math.sqrt(n)+1; i+=2) {
      if(n%i==0) return 0L;
    } 
    return 1L;
}
  //método para execução
  public Long call() throws Exception {
    return ehPrimo(num);
  }
}

//classe do método main
public class FutureHello  {
  private static final int N = 300;
  private static final int NTHREADS = 10;

  public static void main(String[] args) {
    //cria um pool de threads (NTHREADS)
    ExecutorService executor = Executors.newFixedThreadPool(NTHREADS);
    //cria uma lista para armazenar referencias de chamadas assincronas
    List<Future<Long>> list = new ArrayList<Future<Long>>();

    for (long i = 0; i < N; i++) {
      Callable<Long> worker = new MyCallable(i);
      Future<Long> submit = executor.submit(worker);
      list.add(submit);
    }
    //recupera os resultados e faz o somatório final. 
    // Nesse caso, como ehPrimo retorna 0 se não for e 1 se for primo, o somatório representa o contador de primos
    long sum = 0;
    for (Future<Long> future : list) {
      try {
        sum += future.get(); //bloqueia se a computação nao tiver terminado
      } catch (InterruptedException e) {
        e.printStackTrace();
      } catch (ExecutionException e) {
        e.printStackTrace();
      }
    }
    System.out.println("Há "+sum+" primos de 0 até "+N);
    executor.shutdown();
  }
}
