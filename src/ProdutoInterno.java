import lombok.SneakyThrows;

import java.io.DataInputStream;
import java.io.FileInputStream;

public class ProdutoInterno implements Runnable{

    private final int start;
    private final int end;
    private double produto = 0. ;

    public ProdutoInterno(int start, int end) {
        this.start = start;
        this.end = end;
    }

    public double getProduto() {return produto;}

    @SneakyThrows
    @Override
    public void run() {
        try(DataInputStream vetorA = new DataInputStream(new FileInputStream("A.bin"))){
            try(DataInputStream vetorB = new DataInputStream(new FileInputStream("B.bin"))){

                // pula o offset referente ao ponteiro do arquivo para cada thread
                vetorA.skipBytes(start * Double.BYTES);
                vetorB.skipBytes(start * Double.BYTES);

                for (int i = start; i < end; i++){
                    produto += vetorA.readDouble()*vetorB.readDouble();
                }
            }
        }
    }
}