import java.util.ArrayList;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.Semaphore;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;

public class Database implements MyDatabase {

    ExecutorService tda;
    public ArrayList<Table> tables = new ArrayList<>();
    AtomicBoolean transactionOn = new AtomicBoolean(false);
    int numWorkerThreads;

    Semaphore e;
    Semaphore r = new Semaphore(1);
    Semaphore w = new Semaphore(1);
    AtomicInteger nw = new AtomicInteger(0);
    AtomicInteger nr = new AtomicInteger(0);
    AtomicInteger dw = new AtomicInteger(0);
    AtomicInteger dr = new AtomicInteger(0);

    @Override
    public void initDb(int numWorkerThreads) {
        this.numWorkerThreads = numWorkerThreads;
        tda = Executors.newFixedThreadPool(numWorkerThreads);
        e = new Semaphore(1);
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
        ArrayList<ArrayList<Object>> result = null;
        for (Table t : tables) {
            if (t.tableName.equals(tableName)) {

                Select mySelect = new Select(t, operations, condition);

                try {
                    if (transactionOn.get()) {
                        w.acquire();
                        r.acquire();
                        Future future = tda.submit(mySelect);
                        future.get();
                        result = mySelect.getResult();
                        r.release();
                        w.release();

                    } else {
                        e.acquire();
                        mySelect.run();
                        result = mySelect.getResult();
                        e.release();
                    }
                } catch (Exception e) {
                    System.err.println("Select");
                    e.printStackTrace();
                }

                break;
            }
        }
        return result;
    }

    @Override
    public void insert(String tableName, ArrayList<Object> values) {
        for (Table t : tables) {
            if (t.tableName.equals(tableName)) {

                Insert myInsert = new Insert(t, values);

                try {
                    if (transactionOn.get()) {
                    w.acquire();
                    Future future = tda.submit(myInsert);
                    future.get();
                    w.release();
                    } else {
                        e.acquire();
                        myInsert.run();
                        e.release();
                    }
                } catch (Exception e) {
                    System.err.println("Insert");
                    e.printStackTrace();
                }

                break;
            }
        }
    }

    @Override
    public void update(String tableName, ArrayList<Object> values, String condition) {
        for (Table t : tables) {
            if (t.tableName.equals(tableName)) {

                Update myUpdate = new Update(t, values, condition);

                try {
                    if (transactionOn.get()) {
                    w.acquire();
                    Future future = tda.submit(myUpdate);
                    future.get();
                    w.release();
                    break;
                    } else {
                        e.acquire();
                        myUpdate.run();
                        e.release();
                    }
                } catch (Exception e) {
                    System.err.println("Update");
                    e.printStackTrace();
                }

                break;
            }
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


}