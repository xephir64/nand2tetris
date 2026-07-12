import java.util.Map;

public class Code {
    private static final Map<String, String> COMP = Map.ofEntries(
            Map.entry("0", "0101010"),
            Map.entry("1", "0111111"),
            Map.entry("-1", "0111010"),
            Map.entry("D", "0001100"),
            Map.entry("A", "0110000"),
            Map.entry("!D", "0001101"),
            Map.entry("!A", "0110001"),
            Map.entry("-D", "0001111"),
            Map.entry("-A", "0110011"),
            Map.entry("D+1", "0011111"),
            Map.entry("A+1", "0110111"),
            Map.entry("D-1", "0001110"),
            Map.entry("A-1", "0110010"),
            Map.entry("D+A", "0000010"),
            Map.entry("D-A", "0010011"),
            Map.entry("A-D", "0000111"),
            Map.entry("D&A", "0000000"),
            Map.entry("D|A", "0010101"),
            Map.entry("M", "1110000"),
            Map.entry("!M", "1110001"),
            Map.entry("-M", "1110011"),
            Map.entry("M+1", "1110111"),
            Map.entry("M-1", "1110010"),
            Map.entry("D+M", "1000010"),
            Map.entry("D-M", "1010011"),
            Map.entry("M-D", "1000111"),
            Map.entry("D&M", "1000000"),
            Map.entry("D|M", "1010101")
            );

    private static final Map<String, String> DEST = Map.of(
            "", "000",
            "M", "001",
            "D", "010",
            "MD", "011",
            "A", "100",
            "AM", "101",
            "AD", "110",
            "AMD", "111"
    );

    private static final Map<String, String> JUMP = Map.of(
            "", "000",
            "JGT", "001",
            "JEQ", "010",
            "JGE", "011",
            "JLT", "100",
            "JNE", "101",
            "JLE", "110",
            "JMP", "111"
    );


    public String comp(String comp) {
        return COMP.get(comp);
    }

    public String dest(String dest) {
        return DEST.get(dest);
    }

    public String jump(String jump) {
        return JUMP.get(jump);
    }

    public String symbol(Integer symbol) {
        int remainder, quotient = symbol;
        StringBuilder binaryNum = new StringBuilder();
        while (quotient > 0) {
            remainder = quotient % 2;
            binaryNum.insert(0, Integer.toString(remainder));
            quotient = quotient / 2;
        }
        return binaryNum.toString();
    }
}
