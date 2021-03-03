import java.util.ArrayList;
import java.util.Collections;

public class Matrix {
    // Здесь хранятся значения матрицы
    private ArrayList<ArrayList<Integer>> values = new ArrayList<ArrayList<Integer>>();

    public Matrix() {}

    public Matrix(int lines, int columns) {
        initNull(lines, columns);
    }

    public Matrix(ArrayList<ArrayList<Integer>> values) {
        this.values = values;
    }

    // Создаёт матрицу размера lines columns, заполненную нулями
    public void initNull(int lines, int columns) {
        if (lines <= 0) {
            System.err.println("Matrix.initNull: lines must be > 0");
            return;
        }
        if (columns <= 0) {
            System.err.println("Matrix.initNull: columns must be > 0");
            return;
        }

        values.clear();
        for (int i = 0; i < lines; i++)
            values.add(new ArrayList<Integer>(Collections.nCopies(columns, 0)));
    }

    // Выводит матрицу в поток вывода
    public void display() {
        for (ArrayList<Integer> line : values) {
            for (Integer element : line)
                System.out.print(element + " ");
            System.out.println();
        }
        System.out.println();
    }

    // Чтение элемента
    public int get(int line, int column) {
        return values.get(line).get(column);
    }

    // Зарпись элемента
    public void set(int line, int column, int value) {
        values.get(line).set(column, value);
    }

    // Количество строк
    public int getLineNumber() {
        return values.size();
    }

    // Количество столбцов
    public int getColumnNumber() {
        if (getLineNumber() == 0)
            return 0;
        return values.get(0).size();
    }

    // Добавление строки
    public void addLine(int line) {
        values.add(line, new ArrayList<Integer>(Collections.nCopies(getColumnNumber(), 0)));
    }

    // Добавдение столбца
    public void addColumn(int column) {
        for (ArrayList<Integer> line : values)
            line.add(column, 0);
    }

    // Удаление строки
    public void removeLine(int line) {
        values.remove(line);
    }

    // Удаление столбца
    public void removeColumn(int column) {
        for (ArrayList<Integer> line : values)
            line.remove(column);
    }
}