#ifndef __ABSTRACTGRAPH_H__
#define __ABSTRACTGRAPH_H__

#include "common.h"
#include "graphnode.h"
#include "pixel.h"

class AbstractGraph{
public:


    explicit AbstractGraph(){
        imageNode={};
        imageOutputNode={};
        height=0;
        width=0;
        node_number=0;
        imagePixel={};
        background_color=0;
        corners={};
        most_color=0;
        least_color=0;
        goalPixel={};

    }

    explicit AbstractGraph( AbstractGraph*  abstractGraph ){
        imageNode=abstractGraph->getImageNodeCopy();
        height=abstractGraph->getHeight();
        width=abstractGraph->getWidth();
        node_number=abstractGraph->getnodenumber();
        imagePixel=abstractGraph->getImagePixelCopy();
        background_color=abstractGraph->getBackGroundColor();
        corners=abstractGraph->getCorners();
        most_color=abstractGraph->getMostColor();
        least_color=abstractGraph->getLeastColor();
        goalPixel=abstractGraph->getGoalPixelColor();
        imagePixelLocation=abstractGraph->getimagePixelLocation();
        imageOutputNode=abstractGraph->getOutputNodeCopy();
    }

    AbstractGraph* copy() {
        return new AbstractGraph( this );
    }





    ~AbstractGraph() {
        for (auto node: imageNode){
            delete node.second;
        }
        imageNode.clear();

        for (auto pixel: imagePixel ){
            delete pixel.second;
        }

        _name.clear();


        imagePixelLocation.clear();
        imagePixel.clear();
        corners.clear();
        goalPixel.clear();




    }



    map <int,GraphNode*> getImageNode() const{
        return imageNode;
    }

    map <int,GraphNode*> getImageNodeCopy() const{
        map<int,GraphNode* > imageNodeCopy;
        for (auto it:imageNode ){
            imageNodeCopy[it.first]=it.second->copy();
        }
        return imageNodeCopy;
    }



    int getnodenumber() const{
        return node_number;
    }

    map<int,Pixel*> getImagePixel() const{
        return imagePixel;
    }

    map<int,Pixel*> getImagePixelCopy() const{
        map<int,Pixel*> imagePixelCopy;
        for (auto pixel: imagePixel){
            imagePixelCopy[pixel.first]=pixel.second->copy();
        }
        return imagePixelCopy;
    }

    vector<int> getCorners() const{
        return corners;
    }

    int getMostColor() const{
        return most_color;
    }

    int getLeastColor() const{
        return least_color;
    }



    void addNode(int nodeID, GraphNode *node){
        imageNode[nodeID]=node;
    }

    void addOutputNode(int nodeID, GraphNode *node){
        imageOutputNode[nodeID]=node;
    }


    void setHeight(int h){
        height=h;
    }
    void setWidth(int w){
        width=w;
    }
    void setNodeNumber(){
        node_number+=1;
    }
    void addImagePixel(int pixelId, Pixel *pixel){
        imagePixel[pixelId]=pixel;
        imagePixelLocation[pixel->getLocation()]=pixelId;
        if (pixel->getX()>height)
            height=pixel->getX();
        if (pixel->getY()>width)
            width=pixel->getY();
    }

    void addGoalImagePixel(int pixelId, int color){
        goalPixel[pixelId]=color;
    }



    int getWidth() const{
        return width;
    }

    int getHeight() const{
        return height;
    }
    void setBackgroundColor(int color){
        background_color=color;
    }
    void setCorners(int pixel){
        corners.push_back(pixel);
    }
    void setMostColor(int c){
        most_color=c;
    }

    void setLeastColor(int c){
        least_color=c;
    }

    GraphNode* getNode(int nodeID) const {
        auto it=imageNode.find(nodeID);
        assert(it!=imageNode.end());
        return it->second;
    }

    GraphNode* getOutputNode(int nodeID) const {
        auto it=imageOutputNode.find(nodeID);
        assert(it!=imageOutputNode.end());
        return it->second;
    }

    vector<GraphNode*> getNode() const{
        vector<GraphNode*> nodes;
        for (auto node:imageNode){
            nodes.push_back(node.second);
        }
        return nodes;
    }

    vector<GraphNode*> getOutputNode() const{
        vector<GraphNode*> nodes;
        for (auto node:imageOutputNode){
            nodes.push_back(node.second);
        }
        return nodes;
    }


    map <int,GraphNode*> getOutputNodeCopy() const{
        map<int,GraphNode* > imageOutputNodeCopy;
        for (auto it:imageOutputNode ){
            imageOutputNodeCopy[it.first]=it.second->copy();
        }
        return imageOutputNodeCopy;
    }




    map<vector<int>,int> getimagePixelLocation(){
        return imagePixelLocation;
    }




    Pixel* getPixel(int pixelID) const{
        auto  it =imagePixel.find(pixelID);
        assert(it!=imagePixel.end());
        return  it->second;
    }

    map<int,int> getPixelColor() const{
        map<int,int> pixelcolors;
        for (auto it : imagePixel){
            pixelcolors[it.first]=it.second->getColor();
        }
        return pixelcolors;
    }


    int getGoalPixelColor(int  pixelID) const{
        auto  it =goalPixel.find(pixelID);
        assert(it!=goalPixel.end());
        return  it->second;
    }

    map<int,int> getGoalPixelColor() const{
        return goalPixel;
    }

    int getPixel(vector<int>& location) const{
        auto  it =imagePixelLocation.find(location);
        assert(it!=imagePixelLocation.end());
        return  it->second;
    }
    bool inbound(vector<int> x_location, vector<int> y_location){
        for (auto x: x_location){
            if (x<0 or x>height)
                return false;
        }
        for (auto y: y_location){
            if (y<0 or y>width)
                return false;
        }
        return true;
    }

    bool inbound(int x_location, int y_location){
        if (x_location<0 or x_location>height)
            return false;

        if (y_location<0 or y_location>width)
                return false;
        return true;
    }


    bool check_collision(GraphNode* givennode, vector<int> given_x_locations, vector<int> given_y_locations){
        vector<vector<int>> check;

        for (int i=0;i<given_x_locations.size();i++){
            check.push_back({given_x_locations[i],given_y_locations[i]});
        }

        for (auto node: imageNode){
            if (givennode==node.second)
                continue;
            vector<int> x_locations=node.second->get_x_locations();

            vector<int> y_locations=node.second->get_y_locations();

            vector<vector<int>> locations;

            for (int i=0;i<x_locations.size();i++){
                locations.push_back({x_locations[i],y_locations[i]});
            }


            for (auto it: check){
                if ( std::find(locations.begin(), locations.end(),it)!=locations.end())
                    return true;
            }

        }
        return false;
    }


    bool check_meet(vector<int>x_locations,vector<int> y_locations,GraphNode* givennode2 ){

        vector<vector<int>> check;

        for (auto i=0;i<x_locations.size();i++){
            check.push_back({x_locations[i],y_locations[i]});
        }


        vector<vector<int>>node2=givennode2->getLocations();

        for (auto it: check){
                if ( std::find(node2.begin(), node2.end(),it)!=node2.end())
                    return true;
            }

        return false;
    }


    bool check_meet(int x_locations, int y_locations,GraphNode* givennode2 ){

        vector<vector<int>> check;

        check.push_back({x_locations,y_locations});



        vector<vector<int>>node2=givennode2->getLocations();

        for (auto it: check){
            if ( std::find(node2.begin(), node2.end(),it)!=node2.end())
                return true;
        }

        return false;
    }

    bool check_collision(GraphNode* givennode, int given_x_locations, int given_y_locations){
        vector<int> check={given_x_locations,given_y_locations};

        for (auto node: imageNode){
            if (givennode==node.second)
                continue;

            vector<vector<int>> locations=node.second->getLocations();

            if ( std::find(locations.begin(), locations.end(),check)!=locations.end())
                return true;
        }
        return false;
    }

    vector<int> check_move_node(GraphNode* graphNode, int     delta_x, int     delta_y){
        vector<int> nodes;
        int max_allowed=1000;
        nodes.push_back(graphNode->getNodeID());

        vector<int> pixels=graphNode->getPixelsIDs();

        vector<int> new_x_location;
        vector<int> new_y_location;


        for (int j=0; j<pixels.size();j++){
            auto pixel=this->getPixel(pixels[j]);
            new_x_location.push_back(pixel->getX());
            new_y_location.push_back(pixel->getY());

        }


        for (int i=0; i<max_allowed;i++){
            for (int j=0; j<pixels.size();j++){
                new_x_location[j]=new_x_location[j]+delta_x;
                new_y_location[j]=new_y_location[j]+delta_y;
            }
            if ( this->inbound(new_x_location,new_y_location))
            {
                if ( this->check_collision_different_color(graphNode,new_x_location,new_y_location)){
                    return nodes;
                }
                auto node=this->check_collision_same_color(graphNode,new_x_location,new_y_location);

                if (node!=nullptr and std::find(nodes.begin(), nodes.end(),node->getNodeID())==nodes.end() ){
                    nodes.push_back(node->getNodeID());

                }
            } else{
                break;
            }

        }
        return  nodes;

    }

    vector<int> check_move_node_with_destination(GraphNode* graphNode, int     delta_x, int     delta_y,int nodeID){
        vector<int> nodes;
        int max_allowed=1000;
        nodes.push_back(graphNode->getNodeID());

        vector<int> pixels=graphNode->getPixelsIDs();

        vector<int> new_x_location;
        vector<int> new_y_location;
        auto desnode=this->getNode(nodeID);


        for (int j=0; j<pixels.size();j++){
            auto pixel=this->getPixel(pixels[j]);
            new_x_location.push_back(pixel->getX());
            new_y_location.push_back(pixel->getY());

        }


        for (int i=0; i<max_allowed;i++){
            for (int j=0; j<pixels.size();j++){
                new_x_location[j]=new_x_location[j]+delta_x;
                new_y_location[j]=new_y_location[j]+delta_y;
            }
            if ( this->inbound(new_x_location,new_y_location))
            {
                if (this->check_collision_destination(desnode,new_x_location,new_y_location))
                    return nodes;
                if ( this->check_collision_different_color(graphNode,new_x_location,new_y_location)){
                    return nodes;
                }
                auto node=this->check_collision_same_color(graphNode,new_x_location,new_y_location);

                if (node!=nullptr and std::find(nodes.begin(), nodes.end(),node->getNodeID())==nodes.end() ){
                    nodes.push_back(node->getNodeID());

                }
            }
            else{
                break;
            }

        }
        return  nodes;

    }


    bool check_collision_destination(GraphNode* desnode, vector<int> given_x_locations, vector<int> given_y_locations){
        vector<vector<int>> check;

        for (int i=0;i<given_x_locations.size();i++){
            check.push_back({given_x_locations[i],given_y_locations[i]});
        }

        vector<vector<int>> locations=desnode->getLocations();


        for (auto it: check){
            if ( std::find(locations.begin(), locations.end(),it)!=locations.end())
                return true;
        }


        return false;
    }



    GraphNode* check_collision_same_color(GraphNode* givennode, vector<int> given_x_locations, vector<int> given_y_locations){
        vector<vector<int>> check;

        for (int i=0;i<given_x_locations.size();i++){
            check.push_back({given_x_locations[i],given_y_locations[i]});
        }

        for (auto node: imageNode){
            if (givennode==node.second)
                continue;


            vector<vector<int>> locations=node.second->getLocations();


            for (auto it: check){
                if ( std::find(locations.begin(), locations.end(),it)!=locations.end() and node.second->getColor()==givennode->getColor())
                    return node.second;
            }

        }
        return nullptr;
    }

    bool check_collision_different_color(GraphNode* givennode, vector<int> given_x_locations, vector<int> given_y_locations){
        vector<vector<int>> check;

        for (int i=0;i<given_x_locations.size();i++){
            check.push_back({given_x_locations[i],given_y_locations[i]});
        }

        for (auto node: imageNode){
            if (givennode==node.second)
                continue;


            vector<vector<int>> locations=node.second->getLocations();


            for (auto it: check){
                if ( std::find(locations.begin(), locations.end(),it)!=locations.end() and node.second->getColor()!=givennode->getColor())
                    return true;
            }

        }
        return false;
    }

    int getBackGroundColor(){
        return this->background_color;
    }

    bool containNode(GraphNode *node1, GraphNode* node2 ) const {
        vector<int> dx = {1, -1, 0, 0, -1, 1, 1, -1};
        vector<int> dy = {0, 0, 1, -1, -1, 1, -1, 1};
        bool flag = false;

        for (auto pixelID: node2->getPixelsIDs()) {
            auto pixel= getPixel(pixelID);
            int x = pixel->getX();
            int y = pixel->getY();
            for (int i = 0; i < 8; i++) {
                int a = x + dx[i];
                int b = y + dy[i];
                if (a < 0 or b < 0 or a > height or b > width)
                    continue;
                if (not(std::find(node2->get_x_locations().begin(), node2->get_x_locations().end(), a) !=
                        node2->get_x_locations().end()
                        or (std::find(node1->get_x_locations().begin(), node1->get_x_locations().end(), a) !=
                            node1->get_x_locations().end())) and
                    (std::find(node2->get_y_locations().begin(), node2->get_y_locations().end(), b) !=
                     node2->get_y_locations().end()
                     or (std::find(node1->get_y_locations().begin(), node1->get_y_locations().end(), b) !=
                         node1->get_y_locations().end()))) {
                    flag = true;
                    break;
                }

            }
            if (flag) {
                break;
            }
        }

        if (not flag){
            return true;
            }
        else{
            return false;
        }

        }





private:
    string _name;
    map <int,GraphNode*> imageNode;
    map <int,GraphNode*> imageOutputNode;
    int width;
    int height;
    int node_number;
    map<int,Pixel*> imagePixel;
    // first int pixelID, second int colorID
    map<int,int> goalPixel;
    int background_color;
    vector<int>corners;
    int most_color;
    int least_color;
    map<vector<int>,int> imagePixelLocation;


};
#endif
