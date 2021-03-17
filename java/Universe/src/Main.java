import java.util.Arrays;

public class Main {

    public static void main(String[] args) {
        // Universe 1
        Universe universe1 = new Universe("1");
        universe1.addParticle(new Particle(new Vec3(0, 0, 0)));

        // Universe 2
        Universe universe2 = new Universe("2");
        universe2.addParticle(new Particle(new Vec3(10, 0, 0)));

        // Universe 3
        Universe universe3 = new Universe("3");
        universe3.addParticle(new Particle(new Vec3(0, 0, 0)));
        universe3.addParticle(new Particle(new Vec3(40, 0, 0)));

        Universe[] arr = {
                universe1,
                universe2,
                universe3,
        };

        Arrays.sort(arr);
        for (Universe u : arr)
            System.out.println(u.getName() + " : " + u.getAveragePos().normalize());
    }
}