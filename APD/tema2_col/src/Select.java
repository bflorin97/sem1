import java.util.ArrayList;
import java.util.StringTokenizer;
import java.util.concurrent.Semaphore;

public class Select implements Runnable {

    private String condition;
    private String[] operations;
    private Table workingTable;
    private ArrayList<ArrayList<Object>> result = new ArrayList<>();

    public Select(Table workingTable, String[] operations, String condition) {
        this.condition = condition;
        this.operations = operations;
        this.workingTable = workingTable;
    }

    @Override
    public void run() {

        ArrayList<ArrayList<Object>> preResult = new ArrayList<>();
        ArrayList<String> parameters = parseCondition(condition);

        if (parameters.size() == 0) {
            ArrayList<Object> line = new ArrayList<>();
            for (int i = 0; i < workingTable.data.get(0).size(); i++) {
                for (int j = 0; j < workingTable.data.size(); j++) {
                    line.add(workingTable.data.get(j).get(i));
                }
            }
        } else {
            int index = workingTable.indexOfColumn(parameters.get(0));
            for (int i = 0; i < workingTable.data.get(0).size(); i++) {
                ArrayList<Object> line = new ArrayList<>();

                if (parameters.get(1).equals("==")) {
                    if (workingTable.columnTypes[index].equalsIgnoreCase("string")) {
                        if (workingTable.data.get(index).get(i).equals(parameters.get(2))) {
                            for (int j = 0; j < workingTable.data.size(); j++) {
                                line.add(workingTable.data.get(j).get(i));
                            }
                        }
                    } else {
                        if (workingTable.data.get(index).get(i) == parameters.get(2)) {
                            for (int j = 0; j < workingTable.data.size(); j++) {
                                line.add(workingTable.data.get(j).get(i));
                            }
                        }
                    }
                } else if (parameters.get(1).equals("<")) {
                    if((Integer)workingTable.data.get(index).get(i) < Integer.valueOf(parameters.get(2))) {
                        for (int j = 0; j < workingTable.data.size(); j++) {
                            line.add(workingTable.data.get(j).get(i));
                        }
                    }
                } else if (parameters.get(1).equals(">")) {
                    if((Integer)workingTable.data.get(index).get(i) > Integer.valueOf(parameters.get(2))) {
                        for (int j = 0; j < workingTable.data.size(); j++) {
                            line.add(workingTable.data.get(j).get(i));
                        }
                    }
                }

                preResult.add(line);
            }
        }

        if (operations[0].equalsIgnoreCase("")) {
            result = preResult;
        } else {
            for (String command : operations) {
                parameters = parseOperation(command);
                if (parameters.get(0).equalsIgnoreCase("min")) {
                    int index = workingTable.indexOfColumn(parameters.get(1));
                    ArrayList<Object> min = new ArrayList<>();
                    min.add(preResult.get(0).get(index));

                    for (ArrayList<Object> line : preResult) {
                        if ((Integer) line.get(index) < (Integer) min.get(0)) {
                            min.clear();
                            min.add(line.get(index));
                        }
                    }

                    result.add(min);
                } else if (parameters.get(0).equalsIgnoreCase("max")) {
                    int index = workingTable.indexOfColumn(parameters.get(1));
                    ArrayList<Object> max = new ArrayList<>();
                    max.add(preResult.get(0).get(index));

                    for (ArrayList<Object> line : preResult) {
                        if ((Integer) max.get(0) < (Integer) line.get(index)) {
                            max.clear();
                            max.add(line.get(index));
                        }
                    }

                    result.add(max);
                } else if (parameters.get(0).equalsIgnoreCase("sum")) {
                    int index = workingTable.indexOfColumn(parameters.get(1));
                    int sum = 0;
                    ArrayList<Object> array = new ArrayList<>();

                    for (ArrayList<Object> line : preResult) {
                        sum += (Integer) line.get(index);
                    }

                    array.add(sum);
                    result.add(array);
                } else if (parameters.get(0).equalsIgnoreCase("count")) {
                    ArrayList<Object> size = new ArrayList<>();
                    size.add(preResult.size());
                    result.add(size);
                } else if (parameters.get(0).equalsIgnoreCase("avg")) {
                    int index = workingTable.indexOfColumn(parameters.get(1));
                    float avg = 0;
                    ArrayList<Object> array = new ArrayList<>();

                    for (ArrayList<Object> line : preResult) {
                        avg += (Integer) line.get(index);
                    }

                    array.add(avg / preResult.size());
                    result.add(array);
                } else {
                    int index = workingTable.indexOfColumn(parameters.get(0));
                    ArrayList<Object> array = new ArrayList<>();

                    for (ArrayList<Object> line : preResult) {
                        array.add(line.get(index));
                    }

                    result.add(array);
                }
            }
        }
    }

    public ArrayList<ArrayList<Object>> getResult() {
        return result;
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

    private ArrayList<String> parseOperation(String operation) {
        ArrayList<String> result = new ArrayList<>();

        StringTokenizer tokenizer = new StringTokenizer(operation, "() ");
        while (tokenizer.hasMoreTokens()) {
            String e = tokenizer.nextToken();
            result.add(e);
        }

        return result;
    }
}
