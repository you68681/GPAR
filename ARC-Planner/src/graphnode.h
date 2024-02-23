#ifndef __GRAPHNODE_H__
#define __GRAPHNODE_H__

#include "common.h"
#include "pixel.h"


class GraphNode{
public:
    explicit GraphNode( const int &nodeId =0 ): nodeID( nodeId ) {
//        containPixel = {};
        color=0;
        similar_color=0;
        vertical={};
        horizontal={};
        connected={};
        shape=0;
        boundary={};
        contain={};
        same={};
        same_color={};
        incomplete_contain={};
        pixels={};
        left={};
        right={};
        up={};
        down={};

        leftup={};
        leftdown={};
        rightup={};
        rightdown={};

        diagonal={};

        max_size_node= false;
        min_size_node= false;
        most_color_node= false;
        min_color_node= false;
        max_size_double= false;
        min_size_double= false;
        most_color_double= false;
        min_color_double= false;
        odd_node= false;
        even_node= false;
        left_diagonal_node= false;
        right_diagonal_node= false;
        left_border_node= false;
        right_border_node= false;
        up_border_node= false;
        down_border_node= false;
        vertical_middle_node = false;
        horizontal_middle_node= false;
        node_background_color=0;
        x_edge_min=100;
        y_edge_min=100;
        x_edge_max=0;
        y_edge_max=0;
        size=0;



    }



    explicit GraphNode(GraphNode*  node ){


        nodeID=node->getNodeID();
//       containPixel=node->getPixelCopy();
        pixels=node->getPixelsIDs();
        color=node->getColor();
        node_background_color=node->getBackGround();
        x_edge_min=node->get_x_edge_min();
        y_edge_min=node->get_y_edge_min();
        x_edge_max=node->get_x_edge_max();
        y_edge_max=node->get_y_edge_max();
        size=node->getSize();
        vertical=node->getVertical();
        horizontal=node->getHorizontal();
        connected=node->getConnect();
        same=node->getSame();
        same_color=node->getSameColor();
        incomplete_contain=node->getIncompleteContain();
        shape=node->getShape();
        boundary=node->getBoundary();
        contain=node->getContain();
        x_locations=node->get_x_locations();
        y_locations=node->get_y_locations();

        locations=node->getLocations();

        max_size_node=node->get_size_max();
        min_size_node=node->get_size_min();
        most_color_node=node->get_color_max();
        min_color_node=node->get_color_min();
        odd_node=node->get_odd();
        even_node=node->get_even();
        left_diagonal_node=node->get_left_diagonal_node();
        right_diagonal_node=node->get_right_diagonal_node();

        left_border_node= node->get_left_border_node();
        right_border_node= node->get_right_border_node();
        up_border_node= node->get_up_border_node();
        down_border_node= node->get_down_border_node();;


        vertical_middle_node=node->get_vertical_middle_node();
        horizontal_middle_node=node->get_horizontal_middle_node();


        max_size_double=node->getMaxDouble();
        min_size_double=node->getMinDouble();
        most_color_double=node->getMostDouble();
        similar_color=node->getSimilar();

        left=node->getLeft();
        right=node->getRight();
        up=node->getUp();
        down=node->getDown();

        leftup=node->getLeftUp();
        leftdown=node->getLeftDown();
        rightup=node->getRightUp();
        rightdown=node->getRightDown();

        diagonal=node->getDiagonal();



    }

    GraphNode* copy() {
        return new GraphNode( this );
    }


    ~GraphNode() {
//        for (auto p: containPixel){
//            delete p.second;
//        }
//        containPixel.clear();
        vertical.clear();
        horizontal.clear();
        connected.clear();
        boundary.clear();
        contain.clear();
        x_locations.clear();
        y_locations.clear();
        pixels.clear();
        incomplete_contain.clear();
        same.clear();
        same_color.clear();

        left.clear();
        right.clear();
        up.clear();
        down.clear();

        leftdown.clear();
        rightup.clear();
        leftup.clear();
        rightdown.clear();

        diagonal.clear();

        locations.clear();


    }
    bool operator==(const GraphNode& node1){
        if (this->getNodeID()==node1.getNodeID())
            return true;
        return false;
    }


    int getPixelNumber() const{
        return pixels.size();
    }

    void setPixel( int pixelID, Pixel *pixel){
        pixels.push_back(pixelID);

        auto plocation= pixel->getLocation();

        x_locations.push_back(plocation[0]);
        y_locations.push_back(plocation[1]);

        locations.push_back(plocation);

        if (plocation[0]>x_edge_max)
            x_edge_max=plocation[0];
        if (plocation[0]<x_edge_min)
            x_edge_min=plocation[0];
        if (plocation[1]>y_edge_max)
            y_edge_max=plocation[1];
        if (plocation[1]<y_edge_min)
            y_edge_min=plocation[1];

    }

    vector<int> get_x_locations() const{
        return x_locations;
    }
    vector<int> get_y_locations() const{
        return y_locations;
    }

    vector<vector<int>> getLocations() const{
        return locations;
    }

    void setBackgroundColor(int color) {
        node_background_color=color;
    }

    int getBackGround () const{
        return  node_background_color;
    }
    void setColor(int c){
        color=c;
    }

    void setSize(int s){
        size=s;
    }



    int getSize()const{
        return size;
    }


    int getSimilar() {
        return similar_color;
    }

    void setSimilar(int colorID){
        similar_color=colorID;
    }

    bool checkSimilar(int colorID){
        return similar_color==colorID;
    }

    void setVer(int nodeID){
        vertical.push_back(nodeID);
    }

    void setHor(int nodeID){
        horizontal.push_back(nodeID);
    }

    void setLeft(int nodeID){
        left.push_back(nodeID);
    }

    void setRight(int nodeID){
        right.push_back(nodeID);
    }


    void setUp(int nodeID){
        up.push_back(nodeID);
    }

    void setDown(int nodeID){
        down.push_back(nodeID);
    }



    void setLeftUp(int nodeID){
        leftup.push_back(nodeID);
    }

    void setLeftDown(int nodeID){
        leftdown.push_back(nodeID);
    }


    void setRightUp(int nodeID){
        rightup.push_back(nodeID);
    }

    void setRightDown(int nodeID){
        rightdown.push_back(nodeID);
    }


    void setDiagonal(int nodeID){
        diagonal.push_back(nodeID);
    }



    void setIncompleteContain(int nodeID){
        incomplete_contain.push_back(nodeID);
    }

    void setSame(int nodeID){
        same.push_back(nodeID);
    }

    void setSameColor(int nodeID){
        same_color.push_back(nodeID);
    }

    void setCon(int nodeID){
     connected.push_back(nodeID);
    }

    void setContain(int nodeID){
        contain.push_back(nodeID);
    }

    void setMaxSize(){
        max_size_node=true;
    }

    void setMinSize(){
        min_size_node=true;
    }

    void setMostColor(){
        most_color_node=true;
    }

    void setMinColor(){
        min_color_node=true;
    }

    void setMaxDouble(){
        max_size_double=true;
    }

    void setMinDouble(){
        min_size_double=true;

    }


    void setMostDouble(){
        most_color_double=true;
    }

    void setOdd(){
        odd_node=true;

    }


    void setEven(){
        even_node=true;
    }

    void setLeftDiagonalNode(){
        left_diagonal_node=true;
    }

    void setRightDiagonalNode(){
        right_diagonal_node=true;
    }

    void setUpBorderNode(){
        up_border_node=true;
    }


    void setDownBorderNode(){
        down_border_node=true;
    }

    void setLeftBorderNode(){
        left_border_node=true;
    }

    void setRightBorderNode(){
        right_border_node=true;
    }

    void setVerticalMiddleNode(){
        vertical_middle_node=true;
    }

    void setHorizontalMiddleNode(){
        horizontal_middle_node=true;
    }




    void  setShape(int s){
        shape=s;
    }

    void  setBoundary(int nodeID ){
        boundary.push_back(nodeID);
    }
    void clearPixelKeepSize(){
        pixels.clear();
        x_locations.clear();
        y_locations.clear();
        locations.clear();
        x_edge_min=100;
        y_edge_min=100;
        x_edge_max=0;
        y_edge_max=0;

    }

    void clearPixelChangSize(){
        pixels.clear();
        size=0;
        x_locations.clear();
        y_locations.clear();
        locations.clear();
        x_edge_min=100;
        y_edge_min=100;
        x_edge_max=0;
        y_edge_max=0;

    }


    int getNodeID() const{
        return nodeID;
    }

    vector<int> getVertical() const{
        return vertical;
    }
    vector<int> getHorizontal() const{
        return  horizontal;
    }


    vector<int> getLeft() const{
        return left;
    }
    vector<int> getRight() const{
        return  right;
    }

    vector<int> getUp() const{
        return up;
    }
    vector<int> getDown() const{
        return  down;
    }

    vector<int> getLeftUp() const{
        return leftup;
    }
    vector<int> getLeftDown() const{
        return  leftdown;
    }

    vector<int> getRightUp() const{
        return rightup;
    }
    vector<int> getRightDown() const{
        return  rightdown;
    }

    vector<int>getDiagonal() const{
        return diagonal;
    }


    vector<int> getSame() const{
        return same;
    }

    vector<int> getSameColor() const{
        return same_color;
    }

    vector<int>getIncompleteContain(){
        return incomplete_contain;
    }

    vector<int> getConnect() const{
        return connected;
    }

    vector<int>getBoundary() const{
        return boundary;
    }

    vector<int> getContain() const{
        return contain;
    }
    int getShape() const{
        return shape;
    }

//    map<int,Pixel*> getPixelCopy(){
//        map<int,Pixel*> pixelsCopy;
//        for (auto it: containPixel){
//            pixelsCopy[it.first]=it.second->copy();
//        }
//        return pixelsCopy;
//    }
//    vector<Pixel *> getPixels() {
//        vector<Pixel*> pixels;
//        for (auto pixel: containPixel){
//            pixels.push_back(pixel.second);
//        }
//        return pixels;
//    }

//    vector< int> getPixelsIDs () {
//        vector<int> pixels;
//        for (auto pixel: containPixel){
//            pixels.push_back(pixel.first);
//        }
//        return pixels;
//    }


    vector< int> getPixelsIDs () {
        return  pixels;
    }



    bool check_spatial_vertical_relationship(int node){

        return std::find(vertical.begin(), vertical.end(),node)!=vertical.end();
}

    bool check_spatial_horizontal_relationship(int node){

        return std::find(horizontal.begin(), horizontal.end(),node)!=horizontal.end();
    }

    bool check_contain(int node){

        return std::find(contain.begin(), contain.end(),node)!=contain.end();
    }


    bool checkLeft(int node){
        return std::find(left.begin(), left.end(),node)!=left.end();
    }

    bool checkRight(int node){
        return std::find(right.begin(), right.end(),node)!=right.end();
    }


    bool checkUp(int node){
        return std::find(up.begin(), up.end(),node)!=up.end();
    }

    bool checkDown(int node){
        return std::find(down.begin(), down.end(),node)!=down.end();
    }


    bool checkLeftUp(int node){
        return std::find(leftup.begin(), leftup.end(),node)!=leftup.end();
    }

    bool checkLeftDown(int node){
        return std::find(leftdown.begin(), leftdown.end(),node)!=leftdown.end();
    }


    bool checkRightUp(int node){
        return std::find(rightup.begin(), rightup.end(),node)!=rightup.end();
    }

    bool checkRightDown(int node){
        return std::find(rightdown.begin(), rightdown.end(),node)!=rightdown.end();
    }


    bool checkDiagonal(int node){
        return std::find(diagonal.begin(), diagonal.end(),node)!=diagonal.end();
    }




    bool check_same(int node){
        return std::find(same.begin(), same.end(),node)!=same.end();
    }

    bool check_same_color(int node){
        return std::find(same_color.begin(), same_color.end(),node)!=same_color.end();
    }

    bool check_incomplete_contain(int node){
        return std::find(incomplete_contain.begin(), incomplete_contain.end(),node)!=incomplete_contain.end();
    }

    bool check_size_max(){

        return max_size_node;
    }

    bool check_size_min(){

        return min_size_node;
    }


    bool check_color_max(){

        return most_color_node;
    }

    bool check_color_min(){

        return min_color_node;
    }



    bool check_odd(){

        return odd_node;
    }

    bool check_even(){

        return even_node;
    }


    bool check_left_diagonal_node(){

        return left_diagonal_node;
    }

    bool check_right_diagonal_node(){

        return right_diagonal_node;
    }

    bool check_up_border_node(){

        return up_border_node;
    }

    bool check_down_border_node(){

        return down_border_node;
    }



    bool check_left_border_node(){

        return left_border_node;
    }


    bool check_right_border_node(){

        return right_border_node;
    }


    bool check_vertical_middle_node(){

        return vertical_middle_node;
    }

    bool check_horizontal_middle_node(){

        return horizontal_middle_node;
    }




    bool get_size_max(){

        return max_size_node;
    }

    bool get_size_min(){

        return min_size_node;
    }


    bool get_color_max(){

        return most_color_node;
    }

    bool get_color_min(){

        return min_color_node;
    }

    bool get_odd(){

        return odd_node;
    }

    bool get_even(){

        return even_node;
    }

    bool get_left_diagonal_node(){

        return left_diagonal_node;
    }

    bool get_right_diagonal_node(){

        return right_diagonal_node;
    }

    bool get_up_border_node(){

        return up_border_node;
    }

    bool get_down_border_node(){

        return down_border_node;
    }

    bool get_left_border_node(){

        return left_border_node;
    }


    bool get_right_border_node(){

        return right_border_node;
    }



    bool get_vertical_middle_node(){

        return vertical_middle_node;
    }

    bool get_horizontal_middle_node(){

        return horizontal_middle_node;
    }




    bool getMaxDouble(){
        max_size_double=true;
    }

    bool getMinDouble(){
        min_size_double=true;

    }


    bool getMostDouble(){
        most_color_double=true;
    }



    int getColor() const{
        return color;
    }


    int get_x_edge_min() const{
        return x_edge_min;
    }
    int get_y_edge_min() const{
        return y_edge_min;
    }

    int get_x_edge_max() const{
        return x_edge_max;
    }

    int get_y_edge_max() const{
        return y_edge_max;
    }
     void removeVertical( int nodeID){
        auto it= find(vertical.begin(),vertical.end(),nodeID);
        if (it!=vertical.end())
            vertical.erase(it);
    }
    void removeHorizontal( int nodeID){
        auto it= find(horizontal.begin(),horizontal.end(),nodeID);
        if (it!=horizontal.end())
            horizontal.erase(it);
    }


    void clearContain() {
        contain.clear();
    }

    void clearSame(){
        same.clear();
    }


    void clearSameColor(){
        same_color.clear();
    }

    void clearVer(){
        vertical.clear();
    }

    void clearHor(){
        horizontal.clear();
    }


    void removeContain(int nodeID){
        auto it= find(contain.begin(),contain.end(),nodeID);
        if (it!=contain.end())
            contain.erase(it);
    }

    void removeSame(int nodeID){
        auto it= find(same.begin(),same.end(),nodeID);
        if (it!=same.end())
            same.erase(it);
    }

    void removeSameColor(int nodeID){
        auto it= find(same_color.begin(),same_color.end(),nodeID);
        if (it!=same_color.end())
            same_color.erase(it);
    }

    void removeIncompleteContain(int nodeID){
        auto it= find(incomplete_contain.begin(),incomplete_contain.end(),nodeID);
        if (it!=incomplete_contain.end())
            incomplete_contain.erase(it);
    }


    bool isVer(int nodeID) const{
        return std::find(vertical.begin(), vertical.end(),nodeID)!=vertical.end();
    }


    bool isHor(int nodeID) const{
        return std::find(horizontal.begin(), horizontal.end(),nodeID)!=horizontal.end();
    }

    bool isContain(int nodeID) const{
        return std::find(contain.begin(), contain.end(),nodeID)!=contain.end();
    }


    bool isPixel(int pixelID) const{
        return std::find(pixels.begin(), pixels.end(),pixelID)!=pixels.end();
    }





private:
    int nodeID;
//    map<int,Pixel*> containPixel;
    vector<int> pixels;
    int color;
    int size;
    int node_background_color;
    int x_edge_min;
    int y_edge_min;
    int x_edge_max;
    int y_edge_max;
    int similar_color;
    vector<int> vertical;
    vector<int> horizontal;
    vector<int> same;
    vector<int> same_color;
    vector<int> incomplete_contain;
    vector<int> connected;
    int shape;
    vector<int> boundary;
    vector<int> contain;
    vector<int> x_locations;
    vector<int> y_locations;

    vector<vector<int>> locations;

    vector<int> left;
    vector<int> right;
    vector<int> up;
    vector<int> down;
    vector<int> leftup;
    vector<int> leftdown;
    vector<int> rightup;
    vector<int> rightdown;

    vector<int> diagonal;



    bool max_size_node;
    bool min_size_node;
    bool most_color_node;
    bool min_color_node;
    bool odd_node;
    bool even_node;
    bool left_diagonal_node;
    bool right_diagonal_node;

    bool up_border_node;
    bool down_border_node;

    bool left_border_node;
    bool right_border_node;

    bool vertical_middle_node;
    bool horizontal_middle_node;

    bool max_size_double;
    bool min_size_double;
    bool most_color_double;
    bool min_color_double;





};
#endif
