import java.util.ArrayList;
import java.lang.Math;

public class Universe {
    private ArrayList<Particle> particles = new ArrayList<Particle>();

    public Universe() {}

    public void addParticle(Particle particle) {
        particles.add(particle);
    }

    public void clear() {
        particles.clear();
    }

    public float getOverallMass() {
        float mass = 0.0f;
        for (Particle particle : particles) {
            mass += particle.getMass();
        }
        return mass;
    }

    public int getParticleCount() {
        return particles.size();
    }

    public float getAverageMass() {
        if (particles.size() == 0)
            return 0.0f;
        return getOverallMass() / particles.size();
    }

    public float getMassVariance() {
        if (particles.size() <= 1)
            return 0.0f;
        float sumOfSquares = 0;
        float massExpected = getAverageMass();
        for (Particle particle : particles) {
            sumOfSquares += Math.pow(particle.getMass() - massExpected, 2);
        }
        return sumOfSquares / (getParticleCount() - 1);
    }
}
