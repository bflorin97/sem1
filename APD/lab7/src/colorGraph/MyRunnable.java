package colorGraph;

import java.util.concurrent.ExecutorService;

public class MyRunnable implements Runnable{
    ExecutorService tde;
    int step;
    int[] colors;

    public MyRunnable(ExecutorService tde, int[] colors, int step) {
        this.tde = tde;
        this.step = step;
        this.colors = colors;
    }

    @Override
    public void run() {
        if (step == Main.N) {
            Main.printColors(colors);
            tde.shutdown();
            return;
        }

        // for the node at position step try all possible colors
        for (int i = 0; i < Main.COLORS; i++) {
            int[] newColors = colors.clone();
            newColors[step] = i;
            if (Main.verifyColors(newColors, step)) {
                tde.submit(new MyRunnable(tde, newColors, step + 1));
            }
        }
    }
}
