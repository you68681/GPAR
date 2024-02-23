#ifndef __POINTER_H__
#define __POINTER_H__

#include "common.h"


class Pointer{
public:
    explicit Pointer( const int &pixelId =0, const int &value=0 ): pointerID( pixelId ),pointerValue(value) {

    }

    explicit Pointer( Pointer*  pointer ){
        pointerID=pointer->getID();
        pointerValue=pointer->getValue();
    }

    Pointer* copy() {
        return new Pointer( this );
    }



    ~Pointer() {
    }


    void setID(int ID){
        pointerID=ID;
    }

    int getID() const{
        return pointerID;
    }

    void setValue(int v){
        pointerValue=v;
    }
    int getValue(){
        return  pointerValue;
    }



private:
    int pointerID;
    int pointerValue;




};
#endif
