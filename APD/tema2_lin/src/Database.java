import java.util.ArrayList;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.Semaphore;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;

public class Database implements MyDatabase {

    ExecutorService tda;
    ArrayList<Table> tables = new ArrayList<>();
    AtomicBoolean transactionOn = new AtomicBoolean(false);
    int numWorkerThreads;

    Semaphore resourceAccess = new Semaphore(1);
    Semaphore readCountAccess = new Semaphore(1);
    Semaphore serviceQueue = new Semaphore(1);

    AtomicInteger readCount = new AtomicInteger(0);

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
        ArrayList<ArrayList<Object>> result = null;
        for (Table t : tables) {
            if (t.tableName.equals(tableName)) {

                Select mySelect = new Select(t, operations, condition);

                try {
                    if (transactionOn.get()) {

                        serviceQueue.acquire();

                        Future<?> future = tda.submit(mySelect);
                        future.get();
                        if (!future.isDone()) {
                            throw new Exception();
                        }
                        result = mySelect.getResult();

                        serviceQueue.release();

                    } else {
                        serviceQueue.acquire();
                        mySelect.run();
                        result = mySelect.getResult();
                        serviceQueue.release();
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
                        serviceQueue.acquire();

                        Future<?> future = tda.submit(myInsert);
                        future.get();
                        if (!future.isDone()) {
                            throw new Exception();
                        }

                        serviceQueue.release();
                    } else {
                        serviceQueue.acquire();
                        myInsert.run();
                        serviceQueue.release();
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
                        serviceQueue.acquire();

                        Future<?> future = tda.submit(myUpdate);
                        future.get();
                        if (!future.isDone()) {
                            throw new Exception();
                        }

                        serviceQueue.release();

                        break;
                    } else {
                        serviceQueue.acquire();
                        myUpdate.run();
                        serviceQueue.release();
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