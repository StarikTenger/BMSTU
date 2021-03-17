import java.util.ArrayList;
import java.lang.Math;

public class Universe implements Comparable<Universe>{
    private ArrayList<Particle> particles = new ArrayList<Particle>();
    private String name = "";

    public Universe() {}

    public Universe(String name) {
        this.name = name;
    }

    public void addParticle(Particle particle) {
        particles.add(particle);
    }

    public void clear() {
        particles.clear();
    }

    public double getOverallMass() {
        double mass = 0.0f;
        for (Particle particle : particles) {
            mass += particle.getMass();
        }
        return mass;
    }

    public int getParticleCount() {
        return particles.size();
    }

    public double getAverageMass() {
        if (particles.size() == 0)
            return 0.0f;
        return getOverallMass() / particles.size();
    }

    public double getMassVariance() {
        if (particles.size() <= 1)
            return 0.0f;
        double sumOfSquares = 0;
        double massExpected = getAverageMass();
        for (Particle particle : particles) {
            sumOfSquares += Math.pow(particle.getMass() - massExpected, 2);
        }
        return sumOfSquares / (getParticleCount() - 1);
    }

    public Vec3 getAveragePos() {
        Vec3 posSum = new Vec3();
        for (Particle particle : particles) {
            posSum.add(particle.getPos());
        }
        posSum.mul(1. / getParticleCount());
        return posSum;
    }

    public int compareTo(Universe unv) {
        return Double.compare(
                this.getAveragePos().normalize(),
                unv.getAveragePos().normalize());
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }
}
