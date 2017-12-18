import com.sun.org.apache.xpath.internal.operations.Bool;

import java.util.ArrayList;

public class Table {
    public String tableName;
    public String[] columnNames;
    public String[] columnTypes;
    public ArrayList<ArrayList> data;

    public Table (String tableName, String[] columnNames, String[] columnTypes) {
        this.tableName = tableName;
        this.columnNames = columnNames;
        this.columnTypes = columnTypes;
        data = new ArrayList<>();
        for (String type : columnTypes) {
            if (type.equalsIgnoreCase("int")) {
                data.add(new ArrayList<Integer>());
            } else if (type.equalsIgnoreCase("bool")) {
                data.add(new ArrayList<Boolean>());
            } else if (type.equalsIgnoreCase("string")) {
                data.add(new ArrayList<String>());
            }
        }
    }

    public int indexOfColumn(String columnName) {
        int index = 0;
        for (String column : columnNames) {
            if (column.equals(columnName)) {
                return index;
            }
            index ++;
        }
        return -1;
    }

    public String toString() {
        String result = tableName + '\n';
        for (int j = 0; j < data.get(0).size(); j++) {
            for (int i = 0; i < data.size(); i++) {
                result += data.get(i).get(j) + " ";
            }
            result += "\n";
        }
        return result;
    }
}
