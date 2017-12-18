import java.util.ArrayList;
import java.util.StringTokenizer;
import java.util.concurrent.Semaphore;

public class Update implements Runnable {

    private String condition;
    private ArrayList<Object> values;
    private Table workingTable;

    public Update(Table workingTable, ArrayList<Object> values, String condition) {
        this.workingTable = workingTable;
        this.condition = condition;
        this.values = values;
    }

    @Override
    public void run() {

        ArrayList<String> parameters = parseCondition(condition);

        int index = workingTable.indexOfColumn(parameters.get(0));

        synchronized (workingTable) {
            for (int i = 0; i < workingTable.data.size(); i++) {
                ArrayList<Object> line = workingTable.data.get(i);
                if (workingTable.columnTypes[index].equalsIgnoreCase("string")) {
                    if (parameters.get(1).equals("==")) {
                        if (line.get(index).equals(parameters.get(2))) {
                            workingTable.data.set(i, values);
                        }
                    }
                } else if (workingTable.columnTypes[index].equalsIgnoreCase("int")) {
                    if (parameters.get(1).equals("==")) {
                        if (line.get(index) == Integer.valueOf(parameters.get(2))) {
                            workingTable.data.set(i, values);
                        }
                    } else if (parameters.get(1).equals("<")) {
                        if ((Integer) line.get(index) < Integer.valueOf(parameters.get(2))) {
                            workingTable.data.set(i, values);
                        }
                    } else if (parameters.get(1).equals(">")) {
                        if ((Integer) line.get(index) > Integer.valueOf(parameters.get(2))) {
                            workingTable.data.set(i, values);
                        }
                    }
                } else if (workingTable.columnTypes[index].equalsIgnoreCase("bool")) {
                    if (parameters.get(1).equals("==")) {
                        if (line.get(index) == Boolean.valueOf(parameters.get(2))) {
                            workingTable.data.set(i, values);
                        }
                    }
                }
            }
        }
    }

    private ArrayList<String> parseCondition(String condition) {
        ArrayList<String> result = new ArrayList<>();

        StringTokenizer tokenizer = new StringTokenizer(condition, " ");
        while (tokenizer.hasMoreTokens()) {
            String e = tokenizer.nextToken();
            result.add(e);
        }

        return result;
    }
}
