import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.math.BigInteger;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Vector;

/**
 * Created by vazzup on 4/8/17.
 */
public class Eve {
    private static BigInteger G = BigInteger.valueOf(2), Z = BigInteger.valueOf(557), P = new BigInteger("22801763489");
    public static void main(String args[]) {
        ServerSocket server;
        Socket socket;
        DataInputStream dataInputStream;
        DataOutputStream dataOutputStream;
        Vector<BigInteger> keys = new Vector<BigInteger>();
        try {
            server = new ServerSocket(20011);
            int i=0;
            while(true) {
                socket = server.accept();
                dataInputStream = new DataInputStream(socket.getInputStream());
                dataOutputStream = new DataOutputStream(socket.getOutputStream());
                BigInteger key = new BigInteger(dataInputStream.readUTF().trim());
                dataOutputStream.writeUTF(G.modPow(Z, P).toString());
                dataOutputStream.flush();
                dataOutputStream.close();
                dataInputStream.close();
                key = key.modPow(Z, P);
                keys.add(key);
                System.out.println("Connected to a machine...");
                System.out.println("G is " + G.toString());
                System.out.println("Z is " + Z.toString());
                System.out.println("P is " + P.toString());
                System.out.println("The key is " + key.toString());
                socket.close();
                i++;
                if(i>=2) {
                    break;
                }
            }
            server.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
