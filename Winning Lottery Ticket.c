import java.io.*;
import java.math.*;
import java.security.*;
import java.text.*;
import java.util.*;
import java.util.concurrent.*;
import java.util.regex.*;

class Result {

    public static long winningLotteryTicket(List<String> tickets) {
        int[] maskCount = new int[1 << 10];
        
        for (String ticket : tickets) {
            int mask = 0;
            for (char c : ticket.toCharArray()) {
                int digit = c - '0';
                mask |= (1 << digit);
            }
            maskCount[mask]++;
        }
        
        long pairs = 0;
        
        for (int i = 0; i < (1 << 10); i++) {
            if (maskCount[i] == 0) continue;
            
            for (int j = i + 1; j < (1 << 10); j++) {
                if (maskCount[j] == 0) continue;
                if ((i | j) == (1 << 10) - 1) {
                    pairs += (long) maskCount[i] * maskCount[j];
                }
            }
            
            if ((i | i) == (1 << 10) - 1) {
                pairs += (long) maskCount[i] * (maskCount[i] - 1) / 2;
            }
        }
        
        return pairs;
    }

}

public class Solution {
    public static void main(String[] args) throws IOException {
        BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(System.in));
        BufferedWriter bufferedWriter = new BufferedWriter(new FileWriter(System.getenv("OUTPUT_PATH")));

        int n = Integer.parseInt(bufferedReader.readLine().trim());

        List<String> tickets = new ArrayList<>();

        for (int i = 0; i < n; i++) {
            String ticketsItem = bufferedReader.readLine();
            tickets.add(ticketsItem);
        }

        long result = Result.winningLotteryTicket(tickets);

        bufferedWriter.write(String.valueOf(result));
        bufferedWriter.newLine();

        bufferedReader.close();
        bufferedWriter.close();
    }
}
