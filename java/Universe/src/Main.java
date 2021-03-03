public class Main {

    public static void main(String[] args) {
        Universe universe = new Universe();
        universe.addParticle(new Particle(10));
        assert universe.getMassVariance() == 0.0;
        universe.addParticle(new Particle(15));
        assert universe.getMassVariance() == 12.5;
        universe.addParticle(new Particle(5));
        assert universe.getOverallMass() == 30;
        assert universe.getAverageMass() == 10;
        assert universe.getMassVariance() == 25.0;
        universe.clear();
        assert universe.getOverallMass() == 0;
        assert universe.getAverageMass() == 0;
        universe.addParticle(new Particle(1));
        assert universe.getAverageMass() == 1;

        // Checking variance
        // Variance must be 0
        universe.clear();
        for (int i = 0; i < 10; i++)
            universe.addParticle(new Particle(1));
        assert universe.getMassVariance() == 0.0;

        universe.clear();
        for (int i = 0; i < 2; i++)
            universe.addParticle(new Particle(1));
        for (int i = 0; i < 2; i++)
            universe.addParticle(new Particle(2));
        System.out.println(universe.getMassVariance());

        System.out.println("succeed");
    }
}