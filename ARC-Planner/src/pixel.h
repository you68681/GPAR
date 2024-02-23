#ifndef __PIXEL_H__
#define __PIXEL_H__

#include "common.h"


class Pixel{
public:
    explicit Pixel( const int &pixelId =0,const int &x =0,const int &y =0 ): pixelID( pixelId ),x_location( x ),y_location( y ) {
         color=0;
         location.push_back(x);
         location.push_back(y);

    }

    explicit Pixel( Pixel*  pixel ){
        pixelID=pixel->getID();
        color=pixel->getColor();
        location=pixel->getLocation();
        x_location=pixel->getX();
        y_location=pixel->getY();
    }

    Pixel* copy() {
        return new Pixel( this );
    }



    ~Pixel() {
        location.clear();
    }

    void xLocation( int x){
        x_location=x;
    }
    void yLocation(int y){
        y_location=y;
    }

    void setColor(int c){
        color=c;
    }
    const vector<int>& getLocation() const{
        return location;
    }
    int getID() const{
        return pixelID;
    }
    int getColor() const{
        return color;
    }

    int getX() const{
        return x_location;
    }
    int getY() const{
        return y_location;
    }





private:
    int pixelID;
    int color;
    int x_location;
    int y_location;
    vector<int> location;





};
#endif
