import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Objects;
import java.util.Set;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class Main {
    public static void main(String[] args) throws IOException {
//        if (args[1] == null) System.err.println("Please provide an .asm file"); System.exit(1);
//        String asm = Files.readString(Paths.get(args[1]));
        Set<String> filesToAssemble = Stream.of(Objects.requireNonNull(new File("resources").listFiles()))
                .filter(file -> !file.isDirectory())
                .map(File::getAbsolutePath)
                .collect(Collectors.toSet());

        for (String files: filesToAssemble) {
            Path path = Paths.get(files);
            String asm = Files.readString(path);

            File ext = path.toFile();
            ext = changeExtension(ext, ".hack");

            Parser parser = new Parser(asm);
            Code code = new Code();
            SymbolTable symbolTable = new SymbolTable();
            StringBuilder bin = new StringBuilder();
            while (parser.hasMoreInstructions()) { // First pass, detect labels
                parser.advance();
                if (parser.instructionKind() == InstructionKind.L_COMMAND) {
                    String s = parser.symbol().replaceAll("[()]", "");
                    int addr = parser.getLine();
                    symbolTable.addEntry(s, addr);
                }
            }
            parser.resetPos();
            while (parser.hasMoreInstructions()) { // Second pass, translate to binary
                parser.advance();
                switch (parser.instructionKind()) {
                    case L_COMMAND -> {}
                    case C_COMMAND -> {
                        String c = parser.comp();
                        String d = parser.dest();
                        String j = parser.jump();

                        String cc = code.comp(c);
                        String dd = code.dest(d);
                        String jj = code.jump(j);

                        String out = "111" + cc + dd + jj;
                        bin.append(out).append("\r\n");
                    }
                    case A_COMMAND -> {
                        String symbol = parser.symbol();
                        String extract = symbol.replace("@", "");
                        if (symbolTable.contains(extract)) {
                            Integer addr = symbolTable.getAddress(extract);
                            String out = code.symbol(addr);
                            bin.append(padding(out)).append("\r\n");
                        } else {
                            if (extract.codePoints().allMatch(Character::isDigit)) {
                                String out = code.symbol(Integer.valueOf(extract));
                                bin.append(padding(out)).append("\r\n");
                            } else {
                                Integer addr = symbolTable.mallocVar(extract);
                                String out = code.symbol(addr);
                                bin.append(padding(out)).append("\r\n");
                            }
                        }
                    }
                    case null, default -> System.err.println("Cannot identify instruction at line " + parser.getLine());
                }
            }
            if (ext.createNewFile()) {
                bin.deleteCharAt(bin.lastIndexOf("\n"));
                bin.deleteCharAt(bin.lastIndexOf("\r"));
                try (BufferedWriter writer = new BufferedWriter(new FileWriter(ext))) {
                    writer.append(bin);
                }
            }
        }

    }

    public static File changeExtension(File f, String newExtension) {
        int i = f.getName().lastIndexOf('.');
        String name = f.getName().substring(0,i);
        return new File(f.getParent(), name + newExtension);
    }

    public static String padding(String val) {
        return String.format("%16s", val).replace(' ', '0');
    }
}