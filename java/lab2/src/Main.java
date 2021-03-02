public class Main {
    public static void main(String[] args) {
        Matrix matrix = new Matrix(2, 2);
        matrix.set(0,0, 1);
        matrix.set(1,1, 5);
        matrix.display();
        matrix.addColumn(0);
        matrix.display();
        matrix.addLine(0);
        matrix.display();
        matrix.removeColumn(2);
        matrix.display();
        matrix.removeLine(2);
        matrix.display();
    }
}