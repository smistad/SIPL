#ifndef VISUALIZATION_HPP_
#define VISUALIZATION_HPP_

#include "Core.hpp"
#include <vector>
#include <map>
#include <string>

namespace SIPL {

enum visualizationType {
    SLICE, MIP
};

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
        void setType(visualizationType);
        void addImage();
        void addVolume();
        void display();
        void update();
        void draw();
        static void keyPressed(GtkWidget * widget, GdkEventKey * event, gpointer user_data);
        slice_plane getDirection() const;
        void setDirection(slice_plane direction);
        int getSlice() const;
        void setSlice(int slice);
        int3 getSize();
        float getAngle() const;
        void setAngle(float angle);

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
        void renderMIP(int, GdkPixbuf *);
        std::string title;
        float scale;
        float angle;
        int width, height;
        int3 size;
        GtkWidget * gtkImage;
        GtkWidget * scaledImage;
        void zoomIn();
        void zoomOut();
        visualizationType type;
};


}; // END SIPL NAMESPACE

#endif /* VISUALIZATION_HPP_ */
