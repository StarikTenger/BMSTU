import java.util.Iterator;

public class Matrix implements Iterable<Integer> {
    private long data = 0;
    private int lines = 0;
    private int columns = 0;

    // Bits in data
    private int getBit(int bit) {
        long dataCopy = this.data;
        dataCopy >>= bit;
        return Math.abs((int) (dataCopy % 2));
    }

    private void setBit(int bit, int val) {
        if (val == 1)
            this.data |= ((long) 1 << bit);
        if (val == 0)
            this.data &= ~((long) 1 << bit);
    }

    // Constructors
    public Matrix() {}

    public Matrix(int lines, int columns) {
        initNull(lines, columns);
    }

    // Initiates with nulls
    public void initNull(int lines, int columns) {
        this.lines = lines;
        this.columns = columns;
        data = 0;
    }

    // Bit assign
    public int getBit(int line, int column) {
        return getBit(line * this.lines + column);
    }

    public void setBit(int line, int column, int val) {
        setBit(line * this.lines + column, val);
    }

    // Displays matrix
    public void display() {
        for (int i = 0; i < lines; i++) {
            for (int j = 0; j < columns; j++) {
                System.out.print(this.getBit(i, j));
            }
            System.out.println();
        }

    }

    // Xor of a line
    public int lineXor(int line) {
        int val = 0;
        for (int i = line * this.lines; i < line * this.lines + this.columns; i++) {
            val ^= getBit(i);
        }
        return val;
    }

    // Iterators
    public Iterator<Integer> iterator() {
        return new LineIterator();
    }

    private class LineIterator implements Iterator<Integer> {
        private int line = 0;

        public boolean hasNext() {
            return this.line < lines;
        }

        public Integer next() {
            line++;
            return lineXor(line - 1);
        }
    }
}


