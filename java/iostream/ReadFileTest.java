package com.demo;

import java.io.*;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.LinkedList;
import java.util.List;
import java.util.Scanner;
import java.util.function.Function;

public class ReadFileTest {
    public static void main(String[] args) {
        String filepath = "...";
        measureTime("bufferReader", ReadFileTest::readFileByBufferReader, filepath);
        measureTime("nio readAllLines", ReadFileTest::readFileByNioFiles, filepath);
        measureTime("randomAccessFile", ReadFileTest::readFileByRandomAccessFile, filepath);
        measureTime("scanner", ReadFileTest::readFileByScanner, filepath);
    }

    private static void measureTime(String name, Function<String, List<String>> fn, String path) {
        long startTime = System.nanoTime();
        fn.apply(path);
        long estimatedTime = System.nanoTime() - startTime;
        System.out.println(name + " takes " + estimatedTime / 1000000 + "ms");
    }

    private static List<String> readFileByBufferReader(String path) {
        LinkedList<String> list = new LinkedList<>();
        try {
            final BufferedReader in = new BufferedReader(
                    new InputStreamReader(new FileInputStream(path), StandardCharsets.UTF_8));
            String line;
            while ((line = in.readLine()) != null) {
                list.add(line);
            }
            in.close();
        } catch (final IOException e) {
            e.printStackTrace();
        }
        return list;
    }

    /**
     *
     * @param path
     * @return
     */
    private static List<String> readFileByNioFiles(String path) {
        try {
            return Files.readAllLines(Paths.get(path));
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }

    private static List<String> readFileByRandomAccessFile(String path) {
        LinkedList<String> list = new LinkedList<>();
        try {
            RandomAccessFile file = new RandomAccessFile(path, "r");
            String str;
            while ((str = file.readLine()) != null) {
                list.add(str);
            }
            file.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return list;
    }

    private static List<String> readFileByScanner(String path) {
        LinkedList<String> list = new LinkedList<>();
        try {
            Scanner scanner = new Scanner(new File(path));
            while (scanner.hasNextLine()) {
                list.add(scanner.nextLine());
            }
            scanner.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
        return list;
    }

}
