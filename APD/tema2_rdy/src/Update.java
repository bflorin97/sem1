import java.util.ArrayList;
import java.util.StringTokenizer;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.Semaphore;

public class Update implements Runnable {

    private ArrayList<String> parameters;
    private ArrayList<Object> values;
    private Table workingTable;
    private int index;
    private int i;
    private CountDownLatch latch;

    public Update(Table workingTable,
                  CountDownLatch latch,
                  int index,
                  int i,
                  ArrayList<String> parameters,
                  ArrayList<Object> values) {
        this.workingTable = workingTable;
        this.latch = latch;
        this.index = index;
        this.i = i;
        this.parameters = parameters;
        this.values = values;
    }

    @Override
    public void run() {
        ArrayList<Object> line;
        synchronized (workingTable) {
            line = workingTable.data.get(i);
        }
        boolean isSatisfying = false;

        if (workingTable.columnTypes[index].equalsIgnoreCase("string")) {
            if (parameters.get(1).equals("==")) {
                if (line.get(index).equals(parameters.get(2))) {
                    isSatisfying = true;
                }
            }
        } else if (workingTable.columnTypes[index].equalsIgnoreCase("int")) {
            if (parameters.get(1).equals("==")) {
                if (line.get(index) == Integer.valueOf(parameters.get(2))) {
                    isSatisfying = true;
                }
            } else if (parameters.get(1).equals("<")) {
                if ((Integer) line.get(index) < Integer.valueOf(parameters.get(2))) {
                    isSatisfying = true;
                }
            } else if (parameters.get(1).equals(">")) {
                if ((Integer) line.get(index) > Integer.valueOf(parameters.get(2))) {
                    isSatisfying = true;
                }
            }
        } else if (workingTable.columnTypes[index].equalsIgnoreCase("bool")) {
            if (parameters.get(1).equals("==")) {
                if (line.get(index) == Boolean.valueOf(parameters.get(2))) {
                    isSatisfying = true;
                }
            }
        }

        synchronized (workingTable) {
            if (isSatisfying) {
                workingTable.data.set(i, values);
            }
        }

        try {
            if (latch != null) {
                latch.countDown();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
