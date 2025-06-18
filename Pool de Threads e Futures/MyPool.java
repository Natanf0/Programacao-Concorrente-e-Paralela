/* Disciplina: Programacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 11 */
/* Codigo: Criando um pool de threads em Java */

import java.util.LinkedList;

//-------------------------------------------------------------------------------
//!!! Documentar essa classe !!!
class FilaTarefas {
    private final int nThreads;
    private final MyPoolThreads[] threads;
    private final LinkedList<Runnable> queue;
    private boolean shutdown;

    public FilaTarefas(int nThreads) {
        this.shutdown = false;
        this.nThreads = nThreads;
        queue = new LinkedList<Runnable>();
        threads = new MyPoolThreads[nThreads];
        for (int i=0; i<nThreads; i++) {
            threads[i] = new MyPoolThreads();
            threads[i].start();
        } 
    }

    public void execute(Runnable r) {
        synchronized(queue) {
            if (this.shutdown) return;
            queue.addLast(r);
            queue.notify();
        }
    }
    
    public void shutdown() {
        synchronized(queue) {
            this.shutdown=true;
            queue.notifyAll();
        }
        for (int i=0; i<nThreads; i++) {
          try { threads[i].join(); } catch (InterruptedException e) { return; }
        }
    }

    private class MyPoolThreads extends Thread {
       public void run() {
         Runnable r;
         while (true) {
           synchronized(queue) {
             while (queue.isEmpty() && (!shutdown)) {
               try { queue.wait(); }
               catch (InterruptedException ignored){}
             }
             if (queue.isEmpty()) return;   
             r = (Runnable) queue.removeFirst();
           }
           try { r.run(); }
           catch (RuntimeException e) {}
         } 
       } 
    } 
}
//-------------------------------------------------------------------------------

//--PASSO 1: cria uma classe que implementa a interface Runnable 
class Hello implements Runnable {
   String msg;
   public Hello(String m) { msg = m; }

   //--metodo executado pela thread
   public void run() {
      System.out.println(msg);
   }
}

class Primo implements Runnable {
  //...completar implementacao, recebe um numero inteiro positivo e imprime se esse numero eh primo ou nao
  int num;

  public Primo(int n){
    this.num=n;
  }

  //funcao para determinar se um numero  ́e primo
  int ehPrimo(long n) {
    int i;
    if(n<=1) return 0;
    if(n==2) return 1;
    if(n%2==0) return 0;
    for(i=3; i< Math.sqrt(n)+1; i+=2) {
      if(n%i==0) return 0;
    } 
    return 1;
}
   
  public void run() {
    // também poderia apenas colocar o corpo da função ehPrimo aqui, mas achei mais "limpo" assim :)
    if(ehPrimo(num)==1) System.out.println(num+" é primo!");
  }
}

//Classe da aplicação (método main)
class MyPool {
    private static final int NTHREADS = 10;

    public static void main (String[] args) {
      //--PASSO 2: cria o pool de threads
      FilaTarefas pool = new FilaTarefas(NTHREADS); 
      
      //--PASSO 3: dispara a execução dos objetos runnable usando o pool de threads
      for (int i = 0; i < 25; i++) {
        //final String m = "Hello da tarefa " + i;
        //Runnable hello = new Hello(m);
        //pool.execute(hello);
        Runnable primo = new Primo(i);
        pool.execute(primo);
      }

      //--PASSO 4: esperar pelo termino das threads
      pool.shutdown();
      System.out.println("Terminou");
   }
}
