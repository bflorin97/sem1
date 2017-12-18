package getPathSequential;

import java.util.ArrayList;
import java.util.concurrent.ExecutorService;

public class MyRunnable implements Runnable{

    int[][] graph;
    ExecutorService tde;
    ArrayList<Integer> partialPath;
    int destination;

    public MyRunnable(ExecutorService tde, int[][] graph, ArrayList<Integer> newPartialPath, int destination) {
        this.tde = tde;
        this.graph = graph;
        this.partialPath = newPartialPath;
        this.destination = destination;
    }

    public void run() {
        if (partialPath.get(partialPath.size() - 1) == destination) {
            System.out.println(partialPath);
            tde.shutdown();
            return;
        }

        // Try to add all possible next nodes that do not create a cycle.
        int lastNodeInPath = partialPath.get(partialPath.size() - 1);
        for (int i = 0; i < graph.length; i++) {
            if (graph[i][0] == lastNodeInPath) {
                if (partialPath.contains(graph[i][1]))
                    continue;
                ArrayList<Integer> newPartialPath = (ArrayList<Integer>) partialPath.clone();
                newPartialPath.add(graph[i][1]);
                tde.submit(new MyRunnable(tde, graph, newPartialPath, destination));
            }
        }
    }
}
