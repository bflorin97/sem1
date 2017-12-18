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

        if (parameters.size() == 0) {
//            for (int i = 0; i < workingTable.data.size(); i++) {
//                workingTable.setData(i, values);
//            }
//            for (ArrayList<Object> line : workingTable.data) {
//                line.se
//            }
//            return;
        }

        int index = workingTable.indexOfColumn(parameters.get(0));

        synchronized (workingTable) {
            for (int i = 0; i < workingTable.data.get(0).size(); i++) {
                ArrayList<Object> line = new ArrayList<>();

                if (parameters.get(1).equals("==")) {
                    if (workingTable.columnTypes[index].equalsIgnoreCase("string")) {
                        if (workingTable.data.get(index).get(i).equals(parameters.get(2))) {
                            for (int j = 0; j < workingTable.data.size(); j++) {
                                workingTable.data.get(j).set(i, values.get(j));
                            }
                        }
                    } else {
                        if (workingTable.data.get(index).get(i) == parameters.get(2)) {
                            for (int j = 0; j < workingTable.data.size(); j++) {
                                workingTable.data.get(j).set(i, values.get(j));
                            }
                        }
                    }
                } else if (parameters.get(1).equals("<")) {
                    if((Integer)workingTable.data.get(index).get(i) < Integer.valueOf(parameters.get(2))) {
                        for (int j = 0; j < workingTable.data.size(); j++) {
                            workingTable.data.get(j).set(i, values.get(j));
                        }
                    }
                } else if (parameters.get(1).equals(">")) {
                    if((Integer)workingTable.data.get(index).get(i) > Integer.valueOf(parameters.get(2))) {
                        for (int j = 0; j < workingTable.data.size(); j++) {
                            workingTable.data.get(j).set(i, values.get(j));
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
