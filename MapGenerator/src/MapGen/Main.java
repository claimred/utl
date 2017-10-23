
/*
 * Copyright (c) 1995, 2008, Oracle and/or its affiliates. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   - Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   - Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 *   - Neither the name of Oracle or the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

package MapGen;

/*
* Main
*/

import com.sun.jna.Library;
import com.sun.jna.Native;
import java.awt.GraphicsDevice;
import java.awt.GraphicsEnvironment;
import java.awt.Rectangle;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.io.File;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
 
public class Main extends JFrame
{
    public interface LHCoreLibrary extends Library {        

        void lhcoreGenerateMap( int[] map, int size, int coastwidth, int trees,
                               int lakes, int grave_fields, int drygrass_fields,
                               int trees_in_forest, int lake_area, int gravel_area,
                               int drygrass_area );

        void lhcoreSaveMapToFile( int[] map, int size, String filename );
    }

    /*public static void main(String[] args) {
        CLibrary.INSTANCE.printf("Hello, World\n");
        for (int i=0;i < args.length;i++) {
            CLibrary.INSTANCE.printf("Argument %d: %s\n", i, args[i]);
        }
    }*/

    public static void main(String[] args) {

        String userdir = System.getProperty("user.dir");
        System.setProperty("jna.library.path", userdir + "\\lib\\");
        System.setProperty("java.library.path", userdir + "\\lib\\");


        //System.setProperty("jna.library.path", "lib/");

        //String str = System.getProperty("os.arch");

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

        //System.out.print("HELLO FUCKERZ\n");

        //String userdir = System.getProperty("user.dir");
        //System.out.print(userdir);
        


        try {

            //String dll = System.getProperty("jna.library.pa"lhcore_c.dll";
            //System.out.print(dll);
            lhcore = (LHCoreLibrary) Native.loadLibrary("lhcore_c.dll", LHCoreLibrary.class);
        }
        catch (Exception e)
        {
            System.out.printf(e.getMessage());
            System.out.print(Native.getLastError());
            
        }



        //Schedule a job for event dispatch thread:
        //creating and showing this application's GUI.
        javax.swing.SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                
                createAndShowGUI();
            }
        });
    }

    /**
     * Create the GUI and show it.  For thread safety,
     * this method should be invoked from the
     * event-dispatching thread.
     */
    private static void createAndShowGUI() {
        //Create and set up the window.
        Main frame = new Main("LiveHard map generator");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        frame.setResizable(false);
        


        

        //Set up the content pane.
        frame.addComponentsToPane();


        //Display the window.
        frame.pack();
        frame.setVisible(true);
        //frame.setLocationRelativeTo(null);
        //frame.setExtendedState(JFrame.MAXIMIZED_BOTH);


    }

    private void addComponentsToPane() {


       

        MainPanel = new JPanel();
        SettingsPanel = new JPanel();
        IPanel = new ImagePanel();
        GenerateButton = new JButton("Generate");
        MiniMap = new MiniImagePanel();

        MiniMap.setFocusable(true);
        

        GenerateButton.addActionListener(new ActionListener() {

            public void actionPerformed(ActionEvent e) {

                if (!MiniMapFrame.isVisible())
                {
                    GraphicsEnvironment ge = GraphicsEnvironment.getLocalGraphicsEnvironment();
                    GraphicsDevice defaultScreen = ge.getDefaultScreenDevice();
                    Rectangle rect = defaultScreen.getDefaultConfiguration().getBounds();
                    int x = (int) rect.getMaxX() - MiniMapFrame.getWidth();
                    int y = 0;
                    MiniMapFrame.setLocation(x, y);
                    MiniMapFrame.setVisible(true);
                }

                if (!MapFrame.isVisible())
                {
                    MapFrame.setLocationRelativeTo(null);
                    MapFrame.setVisible(true);
                }

                if (x100.isSelected())
                    size = 100;

                if (x200.isSelected())
                    size = 200;

                if (x300.isSelected())
                    size = 300;

                map = new int[size * size];

                
                int coastwidth = CoastWidthSlider.getValue();
                int trees = Integer.parseInt(TreesTextField.getText());
                int lakes = Integer.parseInt(LakesTextField.getText());
                int grave_fields = Integer.parseInt(GravelTextField.getText());
                int drygrass_fields = Integer.parseInt(DryGrassTextField.getText());
                int trees_in_forest = TreesInForestSlider.getValue();
                int lake_area = LakeAreaSlider.getValue();
                int gravel_area = GravelAreaSlider.getValue();
                int drygrass_area = DryGrassAreaSlider.getValue();


                lhcore.lhcoreGenerateMap(map, size, coastwidth, trees, lakes,
                        grave_fields, drygrass_fields, trees_in_forest, lake_area, gravel_area, drygrass_area);

                lhcore.lhcoreSaveMapToFile(map, size, "test.map");

                SaveButton.setEnabled(true);

                IPanel.resetViewPort(size);
                MiniMap.resetViewPort(size);


                IPanel.drawMap(map, size);
                IPanel.repaint();
                IPanel.requestFocus();

                MiniMap.drawmap(map, size);
                MiniMap.repaint();
                //MiniMap.setPreferredSize(new Dimension(size, size));
                //SettingsPanel.repaint();





                //lhcore.lhcoreSaveMapToFile(map, size, "test.map");
                
            }
        });

        SaveButton = new JButton("Save");

        SaveButton.addActionListener(new ActionListener() {

            @Override
            public void actionPerformed(ActionEvent arg0) {
                JFileChooser saveFile = new JFileChooser(System.getProperty("user.dir"));
                FileNameExtensionFilter filter = new FileNameExtensionFilter(
                    "LiveHard maps", "lhmap");
                saveFile.setFileFilter(filter);
                saveFile.setDialogTitle("Type file name");
                saveFile.setSelectedFile(new File("new1.lhmap"));
                //saveFile.changeToParentDirectory();
                int retval = saveFile.showSaveDialog(null);

                if (retval == JFileChooser.APPROVE_OPTION)
                {
                    String filename = saveFile.getSelectedFile().getName();
                    
                    lhcore.lhcoreSaveMapToFile(map, size, filename);
                }
            }
        });
        
        MapSizesToolbar = new JToolBar();
        CoastWidthToolbar = new JToolBar();
        CoastWidthSlider = new JSlider(1, 5, 1);
        TreesInForestSlider = new JSlider(2, 10, 5);
        LakeAreaSlider = new JSlider(5, 10, 5);
        DryGrassAreaSlider = new JSlider(5, 10, 5);
        GravelAreaSlider = new JSlider(5, 10, 5);
        AreasToolbar = new JToolBar();
        NumbersToolbar = new JToolBar();
        TreesTextField = new JTextField("300");
        LakesTextField = new JTextField("10");
        GravelTextField = new JTextField("10");
        DryGrassTextField = new JTextField("10");

        //Areas toolbar

        AreasToolbar.setBorder(BorderFactory.createTitledBorder("Areas"));
        AreasToolbar.setLayout(new BoxLayout(AreasToolbar, BoxLayout.Y_AXIS));

        GravelAreaSlider.setBorder(BorderFactory.createTitledBorder("Gravel"));
        GravelAreaSlider.setPaintLabels(true);
        GravelAreaSlider.setPaintTicks(true);
        GravelAreaSlider.setMinorTickSpacing(1);
        GravelAreaSlider.setMajorTickSpacing(1);

        LakeAreaSlider.setBorder(BorderFactory.createTitledBorder("Lake"));
        LakeAreaSlider.setPaintLabels(true);
        LakeAreaSlider.setPaintTicks(true);
        LakeAreaSlider.setMinorTickSpacing(1);
        LakeAreaSlider.setMajorTickSpacing(1);

        DryGrassAreaSlider.setBorder(BorderFactory.createTitledBorder("Dry grass"));
        DryGrassAreaSlider.setPaintLabels(true);
        DryGrassAreaSlider.setPaintTicks(true);
        DryGrassAreaSlider.setMinorTickSpacing(1);
        DryGrassAreaSlider.setMajorTickSpacing(1);

        AreasToolbar.add(LakeAreaSlider);
        AreasToolbar.add(GravelAreaSlider);
        AreasToolbar.add(DryGrassAreaSlider);

        //Numbers toolbar

        NumbersToolbar.setBorder(BorderFactory.createTitledBorder("Numbers"));
        NumbersToolbar.setLayout(new BoxLayout(NumbersToolbar, BoxLayout.Y_AXIS));

        JPanel pan1 = new JPanel();
        pan1.add(new JLabel("Single trees (100-300):"));
        //TreesTextField.setMaximumSize(TreesTextField.getPreferredSize());
        pan1.add(TreesTextField);
        pan1.setMaximumSize(pan1.getPreferredSize());

        NumbersToolbar.add(pan1);

        JPanel pan2 = new JPanel();
        pan2.add(new JLabel("Lakes (0-20):"));
        //LakesTextField.setMaximumSize(LakesTextField.getPreferredSize());
        pan2.add(LakesTextField);
        pan2.setMaximumSize(pan2.getPreferredSize());

        NumbersToolbar.add(pan2);

        JPanel pan3 = new JPanel();
        pan3.add(new JLabel("Grave fields (0-20):"));
        pan3.add(GravelTextField);
        pan3.setMaximumSize(pan3.getPreferredSize());

        NumbersToolbar.add(pan3);

        JPanel pan4 = new JPanel();
        pan4.add(new JLabel("Dry grass fields (0-20):"));
        pan4.add(DryGrassTextField);
        pan4.setMaximumSize(pan4.getPreferredSize());

        NumbersToolbar.add(pan4);
        
        TreesInForestSlider.setBorder(BorderFactory.createTitledBorder("Trees in forest"));
        TreesInForestSlider.setPaintLabels(true);
        TreesInForestSlider.setPaintTicks(true);
        TreesInForestSlider.setMinorTickSpacing(1);
        TreesInForestSlider.setMajorTickSpacing(1);

        //TreesInForestSlider.setAlignmentX(Component.LEFT_ALIGNMENT);
        
        NumbersToolbar.add(TreesInForestSlider);


        //Coastwidth slider
        CoastWidthSlider.setPaintLabels(true);
        CoastWidthSlider.setPaintTicks(true);
        CoastWidthSlider.setMinorTickSpacing(1);
        CoastWidthSlider.setMajorTickSpacing(1);

        CoastWidthToolbar.add(CoastWidthSlider);
        CoastWidthToolbar.setBorder(BorderFactory.createTitledBorder("Coast width"));

        // Map sizes
        MapSizesButtonGroup = new ButtonGroup();

        x100 = new JRadioButton("100x100");
                x200 = new JRadioButton("200x200");
                x300 = new JRadioButton("300x300");

        x100.setSelected(true);
        
        MapSizesButtonGroup.add(x100);
        MapSizesButtonGroup.add(x200);
        MapSizesButtonGroup.add(x300);

        MapSizesToolbar.add(x100);
        MapSizesToolbar.add(x200);
        MapSizesToolbar.add(x300);

        MapSizesToolbar.setBorder(BorderFactory.createTitledBorder("Map size"));

        

        // Settings panel init



        SettingsPanel.setLayout(new BoxLayout(SettingsPanel, BoxLayout.Y_AXIS));

        //SettingsPanel.add(MiniMap);
        SettingsPanel.add(MapSizesToolbar);
        SettingsPanel.add(CoastWidthToolbar);
        SettingsPanel.add(NumbersToolbar);
        SettingsPanel.add(AreasToolbar);
        JPanel ButtonPanel = new JPanel();
        ButtonPanel.add(GenerateButton);
        SaveButton.setEnabled(false);
        ButtonPanel.add(SaveButton);
        SettingsPanel.add(ButtonPanel);
        

        IPanel.setFocusable(true);
        

        MainPanel.setLayout(new BoxLayout(MainPanel, BoxLayout.X_AXIS));
        
        MainPanel.add(SettingsPanel);
        //MainPanel.add(IPanel);


        //MainPanel.addKeyListener();

        

        getContentPane().add(MainPanel);

        MiniMapFrame = new JFrame("MiniMap");
        MiniMapFrame.setResizable(false);
        MiniMap.setFocusable(true);
        MiniMapFrame.add(MiniMap);
        MiniMapFrame.pack();

        MiniMap.addKeyListener(new KeyListener() {

            public void keyTyped(KeyEvent e) {

            }

            public void keyPressed(KeyEvent e) {
                int x = 0, y = 0, zoom = 0;

                switch (e.getKeyCode())
                {
                    case KeyEvent.VK_LEFT:
                        x = -1;
                        break;
                    case KeyEvent.VK_RIGHT:
                        x = 1;
                        break;
                    case KeyEvent.VK_UP:
                        y = -1;
                        break;
                    case KeyEvent.VK_DOWN:
                        y = 1;
                        break;
                    case KeyEvent.VK_PLUS:
                        break;
                    
                }

                MiniMap.update(map, size, x, y);
                MiniMap.repaint();
                
                IPanel.moveViewPort(x, y, size);
                IPanel.drawMap(map, size);
                IPanel.repaint();
            }

            public void keyReleased(KeyEvent e) {

            }
        });



        MapFrame = new JFrame("Map");
        MapFrame.setResizable(false);
        MapFrame.add(IPanel);
        MapFrame.pack();

        //MapFrame.setVisible(true);
    }

    public Main(String name) {
        super(name);
    }

    int[] map;

    int size;
    
    private static LHCoreLibrary lhcore;

    //Containers

    private JFrame MapFrame, MiniMapFrame;
    private JPanel MainPanel, SettingsPanel;
    private MiniImagePanel MiniMap;
    private ImagePanel IPanel;

    // Sliders
    private JSlider CoastWidthSlider, TreesInForestSlider, LakeAreaSlider,
            GravelAreaSlider, DryGrassAreaSlider;

    // Text fields
    private JTextField TreesTextField, LakesTextField, GravelTextField,
            DryGrassTextField;

    //Buttons
    private JButton GenerateButton, SaveButton;

    private JRadioButton x100, x200, x300;

    //Toolbars
    private JToolBar MapSizesToolbar, AreasToolbar, NumbersToolbar, CoastWidthToolbar;

    private ButtonGroup MapSizesButtonGroup;
}

