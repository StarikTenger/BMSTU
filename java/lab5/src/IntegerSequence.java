import java.util.*;
import java.util.stream.Collector;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class IntegerSequence {
    private ArrayList<Integer> data;

    IntegerSequence() {
    }

    private int countBits(int val) {
        int bitNumber = 0;
        for (int i = 0; i < 8; i++, val /= 2) {
            bitNumber += val % 2;
        }
        return bitNumber;
    }

    private boolean isPower(int val) {
        return ((val & (val - 1)) == 0);
    }

    public Stream<Integer> stream() {
        return data.stream()
                .map(x -> x = countBits(x));
    }

    public Optional<Integer> maxPower() {
        return data.stream()
                .filter(this::isPower)
                .max(Comparator.comparing(x -> x));
    }

    public ArrayList<Integer> getData() {
        return data;
    }

    public void setData(ArrayList<Integer> data) {
        this.data = data;
    }
}
