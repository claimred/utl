/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package alphatilesutil;

import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.math.BigInteger;
import java.util.BitSet;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.imageio.ImageIO;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;

/**
 *
 * @author fzentsev
 */
public class Main extends JFrame {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here

        /* Use an appropriate Look and Feel */
        try {
            //UIManager.setLookAndFeel("com.sun.java.swing.plaf.windows.WindowsLookAndFeel");
            //UIManager.setLookAndFeel("com.sun.java.swing.plaf.gtk.GTKLookAndFeel");
            UIManager.setLookAndFeel("javax.swing.plaf.metal.MetalLookAndFeel");


        } catch (UnsupportedLookAndFeelException ex) {
            ex.printStackTrace();
        } catch (IllegalAccessException ex) {
            ex.printStackTrace();
        } catch (InstantiationException ex) {
            ex.printStackTrace();
        } catch (ClassNotFoundException ex) {
            ex.printStackTrace();
        }
        /* Turn off metal's use of bold fonts */
        UIManager.put("swing.boldMetal", Boolean.FALSE);


        //Schedule a job for event dispatch thread:
        //creating and showing this application's GUI.
        javax.swing.SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                createAndShowGUI();
            }
        });
    }

    private void addComponentsToPane() {

        counter = 0;
        Buttons = new JButton[35];
        ConfigLabel = new JLabel("<html>000<br>000<br>000</html>");
        MainPanel = new JPanel();
        
        //MainPanel.setLayout(new BoxLayout(MainPanel, BoxLayout.Y_AXIS));

        MainPanel.setPreferredSize(new Dimension(9 * 133, 900));

        String filename = new String();

        BigInteger bi = new BigInteger("1234");

        BitSet bs = new BitSet(8);

        Integer pop = new Integer(25);


        
        for (int i = 1; i <= 35; i++)
        {
            filename = "res//" + String.format("%d.png", i);

                try {

                    if (i != 35) {
                        BufferedImage buttonIcon = ImageIO.read(new File(filename));
                        Buttons[i - 1] = new JButton(new ImageIcon(buttonIcon));
                        Buttons[i - 1].setText(String.format("OFS_%d", i));
                    } else {
                        Buttons[i - 1] = new JButton();
                        Buttons[i - 1].setText(String.format("OFS_0"));
                    }
                    

                Buttons[i - 1].addActionListener(new ActionListener() {

                    public void actionPerformed(ActionEvent e) {
                        PrintWriter out = null;
                        try {
                            out = new PrintWriter(new BufferedWriter(new FileWriter("confs.txt", true)));
                            StringBuilder tmp = new StringBuilder("000000000");
                            System.out.println(e.getActionCommand());
                            counter++;
                            String bin = Integer.toBinaryString(counter);
                            String bin1 = new StringBuffer(bin).reverse().toString();
                            for (int k = 0; k < 8 - bin.length(); k++) {
                                bin1 += "0";
                            }
                            ConfigLabel.setText(String.format("<html>%c%c%c<br>%c%d%c<br>%c%c%c</html>", bin1.charAt(0), bin1.charAt(1), bin1.charAt(2), bin1.charAt(7), 0, bin1.charAt(3), bin1.charAt(6), bin1.charAt(5), bin1.charAt(4)));
                            out.print(String.format("_conf[%d]=%s;\n", counter - 1, e.getActionCommand()));
                            System.out.println(bin1);
                            out.close();
                        } catch (IOException ex) {
                            Logger.getLogger(Main.class.getName()).log(Level.SEVERE, null, ex);
                        } finally {
                            out.close();
                        }


                    }
                });

                MainPanel.add(Buttons[i - 1]);
            }
            catch ( IOException e) {

            }

        }

        



        MainPanel.add(ConfigLabel);
        getContentPane().add(MainPanel);
    }

    private static void createAndShowGUI() {

        Main frame = new Main("LiveHard tile util");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        frame.setResizable(false);

        //Set up the content pane.
        frame.addComponentsToPane();

        //Display the window.
        frame.pack();
        frame.setVisible(true);
    }

    public Main(String name) {
        super(name);
    }

    private Integer counter;
    private JButton[] Buttons;
    private JPanel MainPanel;
    private JLabel ConfigLabel;
}
