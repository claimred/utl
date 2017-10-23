package MapGen;

import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.image.BufferedImage;
import java.awt.image.ImageObserver;
import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import javax.imageio.ImageIO;
import javax.swing.JPanel;

public class ImagePanel extends JPanel{
    
    private int viewPort_x, viewPort_y;
    private int elem_w, elem_h, elem_cw, elem_ch;
    private int img_w = 931, img_h = 700;
    private BufferedImage image, grass, water, sand, tree, grave, drygrass,
            tree2, tree3, tree4, tree5, tree6, tree7;

    private Map<Integer, BufferedImage> _map;

    public ImagePanel() {

        viewPort_x = -1;
        viewPort_y = -1;
        elem_w = 133;
        elem_h = 100;
        elem_cw = 7;
        elem_ch = 8;

       try {
           image = ImageIO.read(new File("res/test.png"));

           grass = ImageIO.read(new File("res/grass01@pc.png"));
           water = ImageIO.read(new File("res/sea01@pc.png"));
           sand = ImageIO.read(new File("res/sand01@pc.png"));
           grave = ImageIO.read(new File("res/gravel01@pc.png"));
           drygrass = ImageIO.read(new File("res/grass02@pc.png"));
           tree = ImageIO.read(new File("res/Tree_01@pc.png"));
           tree2 = ImageIO.read(new File("res/Tree_02@pc.png"));
           tree3 = ImageIO.read(new File("res/Tree_03@pc.png"));
           tree4 = ImageIO.read(new File("res/Tree_04@pc.png"));
           tree5 = ImageIO.read(new File("res/Tree_05@pc.png"));
           tree6 = ImageIO.read(new File("res/Tree_06@pc.png"));
           tree7 = ImageIO.read(new File("res/Tree_07@pc.png"));

           _map = new HashMap<Integer, BufferedImage>();

           _map.put(516, grass);
           _map.put(552, water);
           _map.put(107, sand);
           _map.put(972, grave);
           _map.put(1007, drygrass);
           _map.put(551, tree);
           _map.put(1042, tree2);
           _map.put(1043, tree3);
           _map.put(1044, tree4);
           _map.put(1045, tree5);


       } catch (IOException ex) {
            // handle exception...
       }
    }

    public void drawMap( int[] map, int size ) {

        if (viewPort_x == -1)
        {
            viewPort_x = size / 2;
            viewPort_y = size / 2;
        }

        ImageObserver io = new ImageObserver() {

            public boolean imageUpdate(Image img, int infoflags, int x, int y, int width, int height) {
                return true;
            }
        };

        Graphics g2d = image.createGraphics();

        for (int j = viewPort_y; j < viewPort_y + elem_ch; j++)
            for (int i = viewPort_x; i < viewPort_x + elem_cw; i++)
            {
                int mapelem = map[j * size + i];

                int x = i - viewPort_x, y = j - viewPort_y;
                    
                g2d.drawImage(_map.get(mapelem), x * elem_w, y * elem_h, io);
            }

        g2d.dispose();
    }

    public void moveViewPort( int x, int y, int size )
    {
        viewPort_x += x;
        viewPort_y += y;

        if (viewPort_x < 0)
            viewPort_x = 0;

        if (viewPort_y < 0)
            viewPort_y = 0;

        if (viewPort_x > size - elem_cw)
            viewPort_x = size - elem_cw;

        if (viewPort_y > size - elem_ch)
            viewPort_y = size - elem_ch;
    }

    public void resetViewPort( int size )
    {
        viewPort_x = size / 2;
        viewPort_y = size / 2;
    }

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        g.drawImage(image, 0, 0, null); // see javadoc for more info on the parameters
        
    }

    @Override
    public Dimension getPreferredSize()
    {
        return new Dimension(image.getWidth(), image.getHeight());
    }
}