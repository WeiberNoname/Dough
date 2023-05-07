import javax.swing.JFrame;

public class MyFrame extends JFrame {
    public MyFrame() 
{
System.setProperty("java.awt.headless", "true");
        setTitle("Weiber");
        setSize(300, 200);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);
        setVisible(true);
    }

    public static void main(String[] args) {
        new MyFrame();
    }
}

