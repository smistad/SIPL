#ifndef VISUALIZATION_HPP_
#define VISUALIZATION_HPP_

#include "Core.hpp"
#include <vector>
#include <map>
#include <string>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gdk/gdkkeysyms.h>

namespace SIPL {

enum visualizationType {
    SLICE, MIP
};

class BaseDataset;

class Visualization {
    public:
        Visualization(BaseDataset * image);
        Visualization(BaseDataset * image, BaseDataset * image2);
        Visualization(BaseDataset * image, BaseDataset * image2, BaseDataset * image3);
        void setLevel(float level);
        float getLevel(BaseDataset * image);
        void setWindow(float window);
        void setLevel(BaseDataset * image, float level);
        void setWindow(BaseDataset * image, float window);
        float getWindow(BaseDataset * image);
        void setTitle(std::string);
        void setType(visualizationType);
        void addImage();
        void addVolume();
        void display();
        void update();
        void draw();
        static void keyPressed(GtkWidget * widget, GdkEventKey * event, gpointer user_data);
        static bool buttonPressed(GtkWidget * widget, GdkEventButton * event, gpointer user_data);
        slice_plane getDirection() const;
        void setDirection(slice_plane direction);
        int getSlice() const;
        void setSlice(int slice);
        int3 getSize();
        float getAngle() const;
        void setAngle(float angle);
        GdkPixbuf * render();
        std::vector<BaseDataset *> getImages();
        GtkWidget * getGtkImage();
    private:
        bool isVolumeVisualization;
        std::vector<BaseDataset *> images;
        std::map<BaseDataset *, float> level;
        std::map<BaseDataset *, float> window;
        slice_plane direction;
        int slice;
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
        GtkWidget * statusBar;
        void zoomIn();
        void zoomOut();
        visualizationType type;
        float3 getValue(int2 position);
        int3 getTrue3DPosition(int2 pos);
        float spacingX;
        float spacingY;
};


}; // END SIPL NAMESPACE

#endif /* VISUALIZATION_HPP_ */
