#ifndef VISUALIZATION_HPP_
#define VISUALIZATION_HPP_

#include "Core.hpp"
#include <vector>
#include <map>
#include <string>

namespace SIPL {
class Visualization {
    public:
        Visualization(BaseDataset * image);
        Visualization(BaseDataset * image, BaseDataset * image2);
        Visualization(BaseDataset * image, BaseDataset * image2, BaseDataset * image3);
        void setLevel(float level);
        void setWindow(float window);
        void setLevel(BaseDataset * image, float level);
        void setWindow(BaseDataset * image, float window);
        void setTitle(std::string);
        void addImage();
        void addVolume();
        void display();
        void update();
        static void keyPressed(GtkWidget * widget, GdkEventKey * event, gpointer user_data);
        static int windowCounter;
    private:
        bool isVolumeVisualization;
        std::vector<BaseDataset *> images;
        std::map<BaseDataset *, float> level;
        std::map<BaseDataset *, float> window;
        slice_plane direction;
        int slice;
        GdkPixbuf * render();
        void renderSlice(int, GdkPixbuf *);
        void renderImage(int, GdkPixbuf *);
        std::string title;
        float scale;
        int width, height;
        GtkWidget * gtkImage;
        GtkWidget * scaledImage;
        void zoomIn();
        void zoomOut();
        void draw();
};


}; // END SIPL NAMESPACE

#endif /* VISUALIZATION_HPP_ */
