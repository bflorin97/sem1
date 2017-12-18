package QueensProblem;

import java.util.concurrent.ExecutorService;

public class MyRunnable implements Runnable {

    ExecutorService tde;
    int step;
    int[] positions;

    public MyRunnable(ExecutorService tde, int step, int[] positions) {
        this.tde = tde;
        this.step = step;
        this.positions = positions;
    }

    @Override
    public void run() {
        if (step == Main.N) {
            Main.print(positions);
            tde.shutdown();
            return;
        }
        for (int i = 0; i < Main.N; ++i) {
            int[] newPositions = positions.clone();
            newPositions[step] = i;
            if (Main.check(newPositions, step)) {
                tde.submit(new MyRunnable(tde, step + 1, newPositions));
            }
        }
    }
}
