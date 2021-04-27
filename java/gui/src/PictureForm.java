import javax.swing.*;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import java.awt.*;
import java.util.Arrays;

public class PictureForm {
    private JPanel mainPanel;
    private Canvas canvas;
    private JSlider step;
    private JSlider scale;
    private JSlider offsetY;
    private JSlider offsetX;
    private JSlider scaleY;
    private JLabel stepText;
    private JCheckBox stripes;

    public PictureForm() {
        scale.addChangeListener(new ChangeListener() {
            public void stateChanged(ChangeEvent e) {
                canvas.setScale(scale.getValue());
            }
        });
        scaleY.addChangeListener(new ChangeListener() {
            public void stateChanged(ChangeEvent e) {
                canvas.setScaleY(scaleY.getValue());
            }
        });
        step.addChangeListener(new ChangeListener() {
            public void stateChanged(ChangeEvent e) {
                canvas.setStep(step.getValue());
                stepText.setText("Step = " +  String.format("%.2f", step.getValue()/100.));
            }
        });
        offsetX.addChangeListener(new ChangeListener() {
            public void stateChanged(ChangeEvent e) {
                canvas.setOffsetX(offsetX.getValue());
            }
        });
        offsetY.addChangeListener(new ChangeListener() {
            public void stateChanged(ChangeEvent e) {
                canvas.setOffsetY(offsetY.getValue());
            }
        });
        offsetY.addChangeListener(new ChangeListener() {
            public void stateChanged(ChangeEvent e) {
                canvas.setOffsetY(offsetY.getValue());
            }
        });
        stripes.addChangeListener(new ChangeListener() {
            public void stateChanged(ChangeEvent e) {
                canvas.setStriped(stripes.isSelected());
            }
        });

        step.setValue(50);
        scale.setValue(100);
        scaleY.setValue(10);
        offsetX.setValue(0);
        offsetY.setValue(0);
        canvas.setScale(scale.getValue());
        canvas.setScaleY(scaleY.getValue());
        canvas.setStep(step.getValue());
        stepText.setText("Step = " +  String.format("%.2f", step.getValue()/100.));
        canvas.setOffsetX(offsetX.getValue());
        canvas.setOffsetY(offsetY.getValue());
    }

    public static void main(String[] args) {
        JFrame frame = new JFrame("WINDOW!!!!!");
        frame.setContentPane(new PictureForm().mainPanel);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.pack();
        frame.setVisible(true);
    }

    private void createUIComponents() {
        canvas = new Canvas();
    }
}

class Canvas extends JPanel {
    private double scaleY = 1;
    private double b = 1;
    private double scale = 10;
    private double offsetX = 0;
    private double offsetY = 0;
    private double step = 10;
    private boolean isStriped = false;

    protected void paintComponent(Graphics g) {
        super.paintComponent(g);

        for (int i = -600; i < 600; i++) {
            g.setColor(Color.BLACK);
            if (i % 2 == 0 && isStriped) {
                g.setColor(Color.DARK_GRAY);
            }
            double xs[] = {i * step, i * step + step, i * step + step, i * step};
            int xPoly[] = Arrays.stream(xs).map(x->(x-offsetX)*scale + 300).mapToInt(x -> (int)x).toArray();
            double ys[] = {0, 0, f(i * step + step), f(i * step)};
            int yPoly[] = Arrays.stream(ys).map(x->-(x-offsetY)*scale*scaleY + 300).mapToInt(x -> (int)x).toArray();
            Polygon poly = new Polygon(xPoly, yPoly, xPoly.length);
            g.fillPolygon(poly);
        }
    }

    private double f(double x) {
        return x * Math.cos(x);
    }

    public void setRadius(int radius) {
        repaint();
    }

    public void setScaleY(double scaleY) {
        this.scaleY = scaleY / 10;
        repaint();
    }

    public void setB(double b) {
        this.b = b;
        repaint();
    }

    public void setScale(double scale) {
        this.scale = scale;
        repaint();
    }

    public void setOffsetX(double offsetX) {
        this.offsetX = offsetX;
        repaint();
    }

    public void setOffsetY(double offsetY) {
        this.offsetY = offsetY;
        repaint();
    }

    public void setStep(double step) {
        this.step = step / 100;
        repaint();
    }

    public void setStriped(boolean striped) {
        isStriped = striped;
        repaint();
    }
}
