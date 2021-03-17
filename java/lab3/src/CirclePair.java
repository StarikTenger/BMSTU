public class CirclePair implements Comparable<CirclePair>{
    private Circle circleA = new Circle();
    private Circle circleB = new Circle();
    private String name = "_";

    CirclePair() {}

    CirclePair(Circle circleA, Circle circleB) {
        this.circleA = circleA;
        this.circleB = circleB;
    }

    CirclePair(Circle circleA, Circle circleB, String name) {
         this.circleA = circleA;
         this.circleB = circleB;
         this.name = name;
    }

    public Circle getCircleA() {
        return circleA;
    }

    public void setCircleA(Circle circleA) {
        this.circleA = circleA;
    }

    public Circle getCircleB() {
        return circleB;
    }

    public void setCircleB(Circle circleB) {
        this.circleB = circleB;
    }

    public double calculateDist() {
        // Distance between centers
        double dist = Math.sqrt(Math.pow(circleA.x - circleB.x, 2) + Math.pow(circleA.y - circleB.y, 2));
        if (dist > circleA.r + circleA.r)
            return 0;
        if (Double.compare(dist, 0) == 0)
            return Double.POSITIVE_INFINITY;
        // Semi-perimeter
        double p = (circleA.r + circleA.r + dist) / 2;
        // Area
        double s = Math.sqrt(p * (p - circleA.r) * (p - circleB.r) * (p - dist));
        return 4 * s / dist;
    }

    public int compareTo(CirclePair cp) {
        return Double.compare(this.calculateDist(), cp.calculateDist());
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }
}
