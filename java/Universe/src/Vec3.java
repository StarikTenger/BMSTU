public class Vec3 {
    public double x = 0;
    public double y = 0;
    public double z = 0;

    Vec3(){}

    Vec3(double x, double y, double z) {
        this.x = x;
        this.y = y;
        this.z = z;
    }

    void add(Vec3 a) {
        this.x += a.x;
        this.y += a.y;
        this.z += a.z;
    }

    void substract(Vec3 a) {
        this.x -= a.x;
        this.y -= a.y;
        this.z -= a.z;
    }

    void mul(double a) {
        this.x *= a;
        this.y *= a;
        this.z *= a;
    }

    double normalize() {
        double norm = Math.sqrt(this.x * this.x + this.y * this.y + this.z * this.z);
        if (Double.compare(norm, 0) != 0) {
            this.x /= norm;
            this.y /= norm;
            this.z /= norm;
        }
        return norm;
    }
}
