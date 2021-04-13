public class Main {
    public static void main(String[] args) {
        Matrix matrix  = new Matrix(4, 4);
        matrix.setBit(0, 0, 1);
        matrix.setBit(1, 0, 1);
        matrix.setBit(1, 1, 1);
        matrix.setBit(3, 0, 1);
        matrix.setBit(3, 1, 1);
        matrix.setBit(3, 2, 1);
        matrix.setBit(3, 2, 1);
        matrix.display();
        for (Integer sum : matrix) {
            System.out.println(sum);
        }
    }
}