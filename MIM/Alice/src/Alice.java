import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.math.BigInteger;
import java.net.Socket;

/**
 * Created by vazzup on 9/8/17.
 */
public class Alice {
    private static BigInteger G = BigInteger.valueOf(2), Y = BigInteger.valueOf(677), P = new BigInteger("22801763489");
    public static void main(String args[]) {
        try {
            Socket socket = new Socket("localhost", 20011);
            DataInputStream dataInputStream = new DataInputStream(socket.getInputStream());
            DataOutputStream dataOutputStream = new DataOutputStream(socket.getOutputStream());
            dataOutputStream.writeUTF(G.modPow(Y, P).toString());
            dataOutputStream.flush();
            BigInteger key = new BigInteger(dataInputStream.readUTF().trim()).modPow(Y, P);
            System.out.println("Connected to a machine...");
            System.out.println("G is " + G.toString());
            System.out.println("Y is " + Y.toString());
            System.out.println("P is " + P.toString());
            System.out.println("The key is " + key.toString());
            dataInputStream.close();
            dataOutputStream.close();
            socket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}
