/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package MapGen;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.geom.AffineTransform;
import java.awt.image.AffineTransformOp;
import java.awt.image.BufferedImage;
import java.awt.image.ImageObserver;
import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import javax.imageio.ImageIO;
import javax.swing.JPanel;

public class MiniImagePanel extends JPanel {

    double scale;
    int elem_cw = 7, elem_ch = 8;
    int view_port_x, view_port_y;
    private BufferedImage image, grass, water, sand, tree, grave, drygrass;
    private HashMap<Integer, Integer> _map;
    
     public MiniImagePanel() {

         try {
            image = ImageIO.read(new File("res/minimap.png"));
         }
         catch (IOException e)
         {

         }

         view_port_x = -1;
         view_port_y = -1;

         int grass_c = (new Color(0, 255, 0)).getRGB();
         int water_c = (new Color(0, 0, 255)).getRGB();
         int sand_c = (new Color(255, 255, 0)).getRGB();
         int grave_c = (new Color(200, 200, 200)).getRGB();
         int drygrass_c = (new Color(200, 200, 0)).getRGB();
         int tree_c = (new Color(255, 0, 0)).getRGB();

         _map = new HashMap<Integer, Integer>();

         _map.put(516, grass_c);
         _map.put(552, water_c);
         _map.put(107, sand_c);
         _map.put(972, grave_c);
         _map.put(1007, drygrass_c);
         _map.put(551, tree_c);
         _map.put(1042, tree_c);
         _map.put(1043, tree_c);
         _map.put(1044, tree_c);
         _map.put(1045, tree_c);
    }

     public void resetViewPort( int size )
    {
        view_port_x = size / 2;
        view_port_y = size / 2;
    }

     public void update( int[] map, int size, int viewx, int viewy )
     {
         view_port_x += viewx;
         view_port_y += viewy;
         if (view_port_x < 0)
             view_port_x = 0;
         if (view_port_y < 0)
             view_port_y = 0;

         if (view_port_x > size - elem_cw - 1)
             view_port_x = size - elem_cw - 1;

         if (view_port_y > size - elem_ch - 1)
             view_port_y = size - elem_ch - 1;

        scale = 240.0 / size;
        
        BufferedImage tmp = new BufferedImage(size, size, BufferedImage.TYPE_4BYTE_ABGR);

        int elem_w = 133, elem_h = 100;

        ImageObserver io = new ImageObserver() {

            public boolean imageUpdate(Image img, int infoflags, int x, int y, int width, int height) {
                return true;
            }
        };

        for (int j = 0; j < size; j++)
            for (int i = 0; i < size; i++)
            {
                int mapelem = map[j * size + i];

                int x = i, y = j;

                try {
                tmp.setRGB(x, y, _map.get(mapelem));
                }
                catch (NullPointerException e)
                {
                    //System.out.print("asd");
                }
            }

        Graphics g2d = tmp.createGraphics();

        g2d.setColor(Color.black);
        g2d.drawRect(view_port_x, view_port_y, elem_cw, elem_ch);

        int w = tmp.getWidth();
        int h = tmp.getHeight();
        //BufferedImage after = new BufferedImage(w, h, BufferedImage.TYPE_INT_ARGB);
        AffineTransform at = new AffineTransform();
        at.scale(scale, scale);
        AffineTransformOp scaleOp =
           new AffineTransformOp(at, AffineTransformOp.TYPE_BILINEAR);
        image = scaleOp.filter(tmp, image);

        g2d.dispose();
     
     }

    public void drawmap( int[] map, int size )
    {
        view_port_x = size / 2;
        view_port_y = size / 2;

        scale = 240.0 / size;
        BufferedImage tmp = new BufferedImage(size, size, BufferedImage.TYPE_4BYTE_ABGR);

        int elem_w = 133, elem_h = 100;

        ImageObserver io = new ImageObserver() {

            public boolean imageUpdate(Image img, int infoflags, int x, int y, int width, int height) {
                return true;
            }
        };

        for (int j = 0; j < size; j++)
            for (int i = 0; i < size; i++)
            {
                int mapelem = map[j * size + i];

                int x = i, y = j;

                try
                {
                    tmp.setRGB(x, y, _map.get(mapelem));
                }
                catch (NullPointerException e)
                {
                    tmp.setRGB(x, y, new Color(0, 0, 0).getRGB());
                }
            }

        int w = tmp.getWidth();
        int h = tmp.getHeight();
        //BufferedImage after = new BufferedImage(w, h, BufferedImage.TYPE_INT_ARGB);
        AffineTransform at = new AffineTransform();
        at.scale(scale, scale);
        AffineTransformOp scaleOp =
           new AffineTransformOp(at, AffineTransformOp.TYPE_BILINEAR);
        image = scaleOp.filter(tmp, image);

        update(map, size, 0, 0);
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
