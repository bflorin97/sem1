import com.sun.org.apache.xpath.internal.operations.Bool;

import java.util.ArrayList;

public class Table {
    public String tableName;
    public String[] columnNames;
    public String[] columnTypes;
    public ArrayList<ArrayList<Object>> data;

    public Table (String tableName, String[] columnNames, String[] columnTypes) {
        this.tableName = tableName;
        this.columnNames = columnNames;
        this.columnTypes = columnTypes;
        data = new ArrayList<>();
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
        return result;
    }
}
