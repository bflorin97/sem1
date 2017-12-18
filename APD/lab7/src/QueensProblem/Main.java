package QueensProblem;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Main {
    static int N = 4;
    public static void solve(int[] positions, int step) {
        if (step == N) {
            print(positions);
            return;
        }
        for (int i = 0; i < N; ++i) {
            int[] newPositions = positions.clone();
            newPositions[step] = i;
            if (check(newPositions, step)) {
                solve(newPositions, step+1);
            }
        }
    }

    public static boolean check(int[] positions, int step) {
        for (int i = 0; i <= step - 1; ++i) {
            if (positions[i] == positions[step] ||
                    positions[i] - i == positions[step] - step ||
                    positions[i] + i == positions[step] + step) {
                return false;
            }
        }

        return true;
    }

    public static void print(int[] positions) {
        for (int i = 0; i < positions.length; ++i) {
            System.out.print(positions[i] + " ");
        }
        System.out.println();
    }

    public static void main(String[] args) {
        int[] positions = new int[N];
//        solve(positions, 0);
        ExecutorService tde = Executors.newFixedThreadPool(4);
        tde.submit(new MyRunnable(tde, 0, positions));
    }
}
