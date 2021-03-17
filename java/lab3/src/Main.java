import java.util.Arrays;

public class Main {
    public static void main(String[] args) {
        CirclePair[] pairs = {
                new CirclePair(new Circle(0, 0, 1), new Circle(1, 0, 1), "1"),
                new CirclePair(new Circle(0, 0, 1), new Circle(1, 1, 1), "2"),
                new CirclePair(new Circle(0, 0, 1), new Circle(0, 0, 1), "3"),
                new CirclePair(new Circle(0, 0, 1), new Circle(10, 0, 1), "4"),
                new CirclePair(new Circle(-10, -10, 100), new Circle(10, 10, 100), "5"),
                new CirclePair(new Circle(0, 0, 10), new Circle(0, 0, 100), "6"),
        };

        Arrays.sort(pairs);
        for (var pair : pairs)
            System.out.println(pair.getName() + " " + pair.calculateDist());
    }
}