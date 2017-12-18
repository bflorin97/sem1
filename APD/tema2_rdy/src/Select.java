import java.util.ArrayList;
import java.util.concurrent.CountDownLatch;

public class Select implements Runnable {

    private Table workingTable;
    private ArrayList<String> parameters;
    private ArrayList<Object> line;
    private ArrayList<ArrayList<Object>> preResult;
    private int index;
    private boolean isSatisfying;
    private CountDownLatch latch;

    public Select(Table workingTable,
                  CountDownLatch latch,
                  ArrayList<String> parameters,
                  ArrayList<Object> line,
                  int index,
                  ArrayList<ArrayList<Object>> preResult) {
        this.workingTable = workingTable;
        this.parameters = parameters;
        this.line = line;
        this.index = index;
        this.preResult = preResult;
        this.latch = latch;
    }

    @Override
    public void run() {
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

        if(isSatisfying) {
            synchronized (workingTable) {
                preResult.add(line);
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
