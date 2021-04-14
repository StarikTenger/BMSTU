import java.util.ArrayList;
import java.util.Arrays;
import java.util.Optional;

import static java.util.Arrays.*;

public class Main {
    public static void main(String[] args) {
        IntegerSequence seq = new IntegerSequence();
        seq.setData(new ArrayList<>(asList(3, 5, 7, 9)));
        seq.stream().forEach(System.out::println);
        Optional<Integer> maxVal = seq.maxPower();
        System.out.println("Max value:");
        maxVal.ifPresent (System .out:: println);
    }
}