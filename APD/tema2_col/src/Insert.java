import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.concurrent.Semaphore;

public class Insert implements Runnable {

    Table workingTable;
    ArrayList values;

    public Insert(Table workingTable, ArrayList<Object> values) {
        this.workingTable = workingTable;
        this.values = values;
    }

    @Override
    public void run() {
        try {
//            workingTable.data.add(values);
            for (int i = 0; i < values.size(); i++) {
                workingTable.data.get(i).add(values.get(i));
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

//        try {
//            synchronized (workingTable) {
//                workingTable.notify();
//            }
//        } catch (Exception e) {
//            e.printStackTrace();
//        }
    }
}
