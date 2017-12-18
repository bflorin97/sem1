import java.util.ArrayList;
import java.util.concurrent.CountDownLatch;

public class ApplyFunction implements Runnable {

    private ArrayList<ArrayList<Object>> result;
    private ArrayList<ArrayList<Object>> preResult;
    private ArrayList<String> params;
    private Table workingTable;
    private CountDownLatch latch;

    public ApplyFunction(Table workingTable,
                         CountDownLatch latch,
                         ArrayList<String> params,
                         ArrayList<ArrayList<Object>> preResult,
                         ArrayList<ArrayList<Object>> result) {
        this.workingTable = workingTable;
        this.latch = latch;
        this.params = params;
        this.preResult = preResult;
        this.result = result;
    }

    @Override
    public void run() {
        synchronized (workingTable) {

            int index;
            ArrayList<Object> myResult = new ArrayList<>();
            if (params.get(0).equalsIgnoreCase("min")) {
                index = workingTable.indexOfColumn(params.get(1));
                myResult.add(preResult.get(0).get(index));

                for (ArrayList<Object> line : preResult) {
                    if ((Integer) line.get(index) < (Integer) myResult.get(0)) {
                        myResult.clear();
                        myResult.add(line.get(index));
                    }
                }
            } else if (params.get(0).equalsIgnoreCase("max")) {
                index = workingTable.indexOfColumn(params.get(1));
                myResult.add(preResult.get(0).get(index));

                for (ArrayList<Object> line : preResult) {
                    if ((Integer) myResult.get(0) < (Integer) line.get(index)) {
                        myResult.clear();
                        myResult.add(line.get(index));
                    }
                }
            } else if (params.get(0).equalsIgnoreCase("sum")) {
                index = workingTable.indexOfColumn(params.get(1));
                int sum = 0;

                for (ArrayList<Object> line : preResult) {
                    sum += (Integer) line.get(index);
                }

                myResult.add(sum);
            } else if (params.get(0).equalsIgnoreCase("count")) {
                myResult.add(preResult.size());
            } else if (params.get(0).equalsIgnoreCase("avg")) {
                index = workingTable.indexOfColumn(params.get(1));
                float avg = 0;

                for (ArrayList<Object> line : preResult) {
                    avg += (Integer) line.get(index);
                }

                myResult.add(avg / preResult.size());
            } else {
                index = workingTable.indexOfColumn(params.get(0));

                for (ArrayList<Object> line : preResult) {
                    myResult.add(line.get(index));
                }
            }

            result.add(myResult);
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
