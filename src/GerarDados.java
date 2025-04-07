import java.io.*;
import java.util.Random;

public class GerarDados {
    public static void gerarVetor(Vetor vetor) throws IOException {
        Random random = new Random(System.currentTimeMillis());
        try (DataOutputStream dataOutputStream = new DataOutputStream(new FileOutputStream(vetor.getNome()+".bin"))){
            for(int linha = 0; linha < vetor.getLinhas(); linha++) {
                dataOutputStream.writeDouble(Math.random());
            }
        }
    }

    public static void printarVetor(Vetor vetorA, Vetor vetorB) throws IOException {
        System.out.println("Vetor A \t Vetor B");
        try(DataInputStream dataInputStream = new DataInputStream(new FileInputStream(vetorA.getNome()+".bin"))){
            try(DataInputStream dataInputStream2 = new DataInputStream(new FileInputStream(vetorB.getNome()+".bin"))) {
                for (int linha = 0; linha < vetorA.getLinhas(); linha++) {
                    System.out.println(dataInputStream.readDouble()+"\t"+dataInputStream2.readDouble());
                }
            }
        }
    }

}
