import Instruction.Instruction;
import Instruction.AInstruction;
import Instruction.CInstruction;
import Instruction.LabelInstruction;


import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Scanner;

public class Parser {
    private final List<Instruction> instructions = new ArrayList<>();

    private Iterator<Instruction> instructionIterator;
    private Instruction actualInstruction;

    private int actualLine = 0;

    public Parser(String code) {
        parse(code);
        instructionIterator = instructions.iterator();
    }

    private void parse(String code) {
        Scanner scanner = new Scanner(code);
        while (scanner.hasNextLine()) {
            String line = scanner.nextLine();
            if (line.contains("//")) continue;
            if (line.isBlank()) continue;
            String instruction = line.replaceAll("\\s+", "");
            if (instruction.contains("@")) instructions.add(new AInstruction(instruction));
            else if (instruction.contains("(") && instruction.contains(")")) {
                instructions.add(new LabelInstruction(instruction));
            }
            else {
                String[] splitInstruction = instruction.split("[=;]");
                if(instruction.contains("=")) {
                    String dest = splitInstruction[0];
                    String comp = splitInstruction[1];
                    instructions.add(new CInstruction(dest, comp, ""));
                } else if(instruction.contains(";")) {
                    String comp = splitInstruction[0];
                    String jump = splitInstruction[1];
                    instructions.add(new CInstruction("", comp, jump));
                }
            }
        }
        scanner.close();
    }

    public int getLine() {
        return actualLine;
    }

    public void advance() {
        actualInstruction = instructionIterator.next();
        if (!(actualInstruction instanceof LabelInstruction)) actualLine++;
    }

    public boolean hasMoreInstructions() {
        return instructionIterator.hasNext();
    }

    public void resetPos() {
        instructionIterator = instructions.iterator();
        actualLine = 0;
    }

    public InstructionKind instructionKind() {
        if (actualInstruction instanceof AInstruction) return InstructionKind.A_COMMAND;
        if (actualInstruction instanceof CInstruction) return InstructionKind.C_COMMAND;
        if (actualInstruction instanceof LabelInstruction) return InstructionKind.L_COMMAND;
        return null;
    }

    public String symbol() {
        if (actualInstruction instanceof LabelInstruction(String symbol)) {
            return symbol;
        } else if (actualInstruction instanceof AInstruction(String symbol)) {
            return symbol;
        }
        return "";
    }

    public String comp() {
        if (actualInstruction instanceof CInstruction c) return c.comp();
        return "";
    }

    public String dest() {
        if (actualInstruction instanceof CInstruction c) return c.dest();
        return "";
    }

    public String jump() {
        if (actualInstruction instanceof CInstruction c) return c.jump();
        return "";
    }
}
