public class Particle {
    private double mass = 0;
    private Vec3 pos = new Vec3();

    public Particle() {}

    public Particle(Vec3 pos) {
        this.pos = pos;
    }

    public Particle(Vec3 pos, double mass) {
        this.pos = pos;
        this.mass = mass;
    }

    public double getMass() {
        return mass;
    }

    public void setMass(double mass) {
        this.mass = mass;
    }

    public Vec3 getPos() {
        return pos;
    }

    public void setPos(Vec3 pos) {
        this.pos = pos;
    }
}
