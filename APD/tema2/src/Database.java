import java.util.ArrayList;
import java.util.StringTokenizer;
import java.util.concurrent.*;
import java.util.concurrent.atomic.AtomicBoolean;

public class Database implements MyDatabase {

    ExecutorService tda;
    ArrayList<Table> tables = new ArrayList<>();
    AtomicBoolean transactionOn = new AtomicBoolean(false);
    int numWorkerThreads;

    Semaphore e = new Semaphore(1);;

    @Override
    public void initDb(int numWorkerThreads) {
        this.numWorkerThreads = numWorkerThreads;
        tda = Executors.newFixedThreadPool(numWorkerThreads);
    }

    @Override
    public void stopDb() {
        tda.shutdown();
    }

    @Override
    public void createTable(String tableName, String[] columnNames, String[] columnTypes) {
        tables.add(new Table(tableName, columnNames, columnTypes));
    }

    @Override
    public ArrayList<ArrayList<Object>> select(String tableName, String[] operations, String condition) {
        try {
            e.acquire();

            ArrayList<ArrayList<Object>> result;
            ArrayList<ArrayList<Object>> preResult;
            ArrayList<String> parameters;
            Table workingTable = null;
            CountDownLatch latch;
            int index;

            for (Table t : tables) {
                if (t.tableName.equals(tableName)) {
                    workingTable = t;
                    break;
                }
            }

            parameters = parseString(condition);
            if (parameters.size() == 0) {
                preResult = workingTable.data;
            } else {
                index = workingTable.indexOfColumn(parameters.get(0));
                preResult = new ArrayList<>();
                latch = new CountDownLatch(workingTable.data.size());

                for (int i = 0; i < workingTable.data.size(); i++) {
                    ArrayList<Object> line;
                    synchronized (workingTable) {
                        line = workingTable.data.get(i);
                    }

                    if (!transactionOn.get()) {
                        tda.submit(new Select(workingTable, latch, parameters, line, index, preResult));
                    } else {
                        Select task = new Select(workingTable, null, parameters, line, index, preResult);
                        task.run();
                    }
                }

                if (!transactionOn.get()) {
                    try {
                        latch.await();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            }

            if (operations[0].equalsIgnoreCase("")) {
                result = preResult;
            } else {
                result = new ArrayList<>();
                latch = new CountDownLatch(operations.length);
                for (String command : operations) {
                    parameters = parseString(command);
                    if (!transactionOn.get()) {
                        tda.submit(new ApplyFunction(workingTable, latch, parameters, preResult, result));
                    } else {
                        ApplyFunction task = new ApplyFunction(workingTable, null, parameters, preResult, result);
                        task.run();
                    }
                }

                if (!transactionOn.get()) {
                    try {
                        latch.await();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            }

            e.release();

            return result;
        } catch (Exception e) {
            System.err.println("SELECT ERROR !?");
            e.printStackTrace();
        }

        return null;
    }

    @Override
    public void insert(String tableName, ArrayList<Object> values) {
        try {
            e.acquire();

            Table workingTable = null;
            for (Table t : tables) {
                if (t.tableName.equals(tableName)) {
                    workingTable = t;
                    break;
                }
            }

            workingTable.data.add(values);

            e.release();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    public void update(String tableName, ArrayList<Object> values, String condition) {
        try {
            e.acquire();

            Table workingTable = null;
            ArrayList<String> parameters = parseString(condition);
            int index;

            for (Table t : tables) {
                if (t.tableName.equals(tableName)) {
                    workingTable = t;
                    break;
                }
            }

            index = workingTable.indexOfColumn(parameters.get(0));
            CountDownLatch latch = new CountDownLatch(workingTable.data.size());

            for (int i = 0; i < workingTable.data.size(); i++) {
                if (!transactionOn.get()) {
                    tda.submit(new Update(workingTable, latch, index, i, parameters, values));
                } else {
                    Update task = new Update(workingTable, null, index, i, parameters, values);
                    task.run();
                }
            }

            if (!transactionOn.get()) {
                try {
                    latch.await();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }

            e.release();
        } catch (Exception e) {
            System.err.println("UPDATE ERROR !?");
            e.printStackTrace();
        }
    }

    @Override
    public void startTransaction(String tableName) {
        transactionOn.set(true);
    }

    @Override
    public void endTransaction(String tableName) {
        transactionOn.set(true);
    }


    private ArrayList<String> parseString(String condition) {
        ArrayList<String> result = new ArrayList<>();

        StringTokenizer tokenizer = new StringTokenizer(condition, "() ");
        while (tokenizer.hasMoreTokens()) {
            String e = tokenizer.nextToken();
            result.add(e);
        }

        return result;
    }
}