#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include "variable.h"
#include "state_descriptor.h"

class Expression{
public:
	explicit Expression( const string &name = "", Variable *lhs = nullptr, Variable *rhs = nullptr ){
		_name = name;
		_lhs = lhs;
		_rhs = rhs;
	}
	
	virtual ~Expression(){
		delete _lhs;
		delete _rhs;
	}

	virtual int update( State *s ) const{ return 0; }

	virtual string getName() const{
		return _name;
	}

	virtual string toString( bool titled ) const{
		string ret;
		if( titled ){
			ret += "[EXPRESSION]: ";
		}
		ret += "( " + _lhs->toString() + " " + _name + " " + _rhs->toString() + " )\n";
		return ret;
	}

	// Set the left-hand side variable of the expression
	virtual void setLHS( Variable *v ){
		_lhs = v;
	}
	
	// Set the right-hand side of the variable of the expression
	virtual void setRHS( Variable *v ){
		_rhs = v;
	}
    // Set the content of left-hand side variable

    virtual int  update_color(StateDescriptor *sd, State *s, int number) const{

        int id = _lhs->getID();
        string vtype = _lhs->getVType();
        vector< int > params = _lhs->getParameterIDs();
        int node_value= s->getPointer(sd, params[0] );
        int node_type_id=s->getNodeID(node_value);
        auto graph=s->getGraph();
        auto graphNode=graph->getNode(node_type_id);
        vector<int> pixels=graphNode->getPixelsIDs();

        vector< int > param_obj_idx ( 2 );

        int old_color;

        int new_color;

        int new_color_id;

        if (sd->checkpointer(params[1])){
            int old_color_pointer = s->getPointer(sd, params[1] );
            old_color=s->getColorID(old_color_pointer);

        }
        else{
            old_color =params[1];
        }

        if (sd->checkpointer(params[2])){
            int new_color_pointer = s->getPointer(sd, params[2] );
            new_color=s->getColorID(new_color_pointer);
        }
        else{
            new_color=params[2];

        }

        if (new_color==old_color)
            return -1;

        for (auto pixelID: pixels){
            auto pixel=graph->getPixel(pixelID);


            param_obj_idx[0]=s->getObject(sd,pixelID);
            param_obj_idx[1]=s->getObject(sd,old_color);;
            s->addRegister( sd, "pixel-color", param_obj_idx, 0 );
            param_obj_idx[1]=s->getObject(sd,new_color);;
            s->addRegister( sd, "pixel-color", param_obj_idx, 1 );
            pixel->setColor(new_color);
        }



        param_obj_idx[0]=node_value;
        param_obj_idx[1]=s->getObject(sd,old_color);;
        s->addRegister( sd, "node-color", param_obj_idx, 0 );
        param_obj_idx[1]=s->getObject(sd,new_color);
        s->addRegister( sd, "node-color", param_obj_idx, 1 );

        graphNode->setColor(new_color);


        return  1;


    }


    virtual int  swap_color(StateDescriptor *sd, State *s, int number) const{

        int id = _lhs->getID();
        string vtype = _lhs->getVType();
        vector< int > params = _lhs->getParameterIDs();
        auto graph=s->getGraph();
        int node_value1= s->getPointer(sd, params[0] );
        int node_type_id1=s->getNodeID(node_value1);
        auto graphNode1=graph->getNode(node_type_id1);
        vector<int> pixels1=graphNode1->getPixelsIDs();

        int node_value2= s->getPointer(sd, params[1] );
        int node_type_id2=s->getNodeID(node_value2);
        auto graphNode2=graph->getNode(node_type_id2);
        vector<int> pixels2=graphNode2->getPixelsIDs();

        if (graphNode2->getNodeID()==graphNode1->getNodeID()){
            return -1;
        }

        vector< int > param_obj_idx ( 2 );

        int color1=graphNode1->getColor();
        int color2=graphNode2->getColor();



        for (auto pixelID: pixels1){
            auto pixel=graph->getPixel(pixelID);
            int old_color=pixel->getColor();

            param_obj_idx[0]=s->getObject(sd,pixelID);
            param_obj_idx[1]=s->getObject(sd,old_color);;
            s->addRegister( sd, "pixel-color", param_obj_idx, 0 );
            param_obj_idx[1]=s->getObject(sd,color2);;
            s->addRegister( sd, "pixel-color", param_obj_idx, 1 );
            pixel->setColor(color2);
        }

        vector< int > node_obj_idx ( 2 );
        node_obj_idx[0]=s->getObject(sd,graphNode1->getNodeID());
        node_obj_idx[1]=s->getObject(sd,graphNode1->getColor());
        s->addRegister( sd, "node-color", node_obj_idx, 0 );

        graphNode1->setColor(color2);

        node_obj_idx[1]=s->getObject(sd,graphNode1->getColor());
        s->addRegister( sd, "node-color", node_obj_idx, 1 );




        for (auto pixelID: pixels2){
            auto pixel=graph->getPixel(pixelID);
            int old_color=pixel->getColor();

            param_obj_idx[0]=s->getObject(sd,pixelID);
            param_obj_idx[1]=s->getObject(sd,old_color);;
            s->addRegister( sd, "pixel-color", param_obj_idx, 0 );
            param_obj_idx[1]=s->getObject(sd,color1);;
            s->addRegister( sd, "pixel-color", param_obj_idx, 1 );
            pixel->setColor(color1);
        }

        node_obj_idx[0]=s->getObject(sd,graphNode2->getNodeID());
        node_obj_idx[1]=s->getObject(sd,graphNode2->getColor());
        s->addRegister( sd, "node-color", node_obj_idx, 0 );

        graphNode2->setColor(color1);

        node_obj_idx[1]=s->getObject(sd,graphNode2->getColor());
        s->addRegister( sd, "node-color", node_obj_idx, 1 );



        return  1;





    }

    virtual int  copy_color(StateDescriptor *sd, State *s, int number) const{

        int id = _lhs->getID();
        string vtype = _lhs->getVType();
        vector< int > params = _lhs->getParameterIDs();
        auto graph=s->getGraph();
        int node_value1= s->getPointer(sd, params[0] );
        int node_type_id1=s->getNodeID(node_value1);
        auto graphNode1=graph->getNode(node_type_id1);
        vector<int> pixels1=graphNode1->getPixelsIDs();

        int node_value2= s->getPointer(sd, params[1] );
        int node_type_id2=s->getNodeID(node_value2);
        auto graphNode2=graph->getNode(node_type_id2);
        vector<int> pixels2=graphNode2->getPixelsIDs();

        if (graphNode2->getNodeID()==graphNode1->getNodeID()){
            return -1;
        }

        vector< int > param_obj_idx ( 2 );

        int color1=graphNode1->getColor();
        int color2=graphNode2->getColor();

        vector< int > node_obj_idx ( 2 );

        for (auto pixelID: pixels2){
            auto pixel=graph->getPixel(pixelID);
            int old_color=pixel->getColor();

            param_obj_idx[0]=s->getObject(sd,pixelID);
            param_obj_idx[1]=s->getObject(sd,old_color);;
            s->addRegister( sd, "pixel-color", param_obj_idx, 0 );
            param_obj_idx[1]=s->getObject(sd,color1);;
            s->addRegister( sd, "pixel-color", param_obj_idx, 1 );
            pixel->setColor(color1);
        }

        node_obj_idx[0]=s->getObject(sd,graphNode2->getNodeID());
        node_obj_idx[1]=s->getObject(sd,graphNode2->getColor());
        s->addRegister( sd, "node-color", node_obj_idx, 0 );

        graphNode2->setColor(color1);

        node_obj_idx[1]=s->getObject(sd,graphNode2->getColor());
        s->addRegister( sd, "node-color", node_obj_idx, 1 );



        return  1;

    }


    virtual int  move_node_direction(StateDescriptor *sd, State *s, int number) const{

        int id = _lhs->getID();
        string vtype = _lhs->getVType();

        vector< int > params = _lhs->getParameterIDs();
        int node_value= s->getPointer(sd, params[0] );
        int node_type_id=s->getNodeID(node_value);
        auto graph=s->getGraph();
        auto graphNode=graph->getNode(node_type_id);
        vector<int> pixels=graphNode->getPixelsIDs();

        int stepID =params[1];


        int directionID;
        if (sd->checkpointer(params[2])){
            int type_id=sd->getPointerVarType(params[2]);
            string type_name=sd->getVarName(type_id);
            int move_value = s->getPointer(sd, params[2] );

            if (type_name=="move-direction"){
                directionID=s->getDirectionID(move_value);
            }
            else if(type_name=="move-direction-diagonal"){
                directionID=s->getDirectionDiagonalID(move_value);
            }
            else{
                assert(type_name=="move-direction" or type_name=="move-direction-diagonal" );
            }


        }
        else{
            directionID =params[2];
        }




            int delta_x = 0;
            int delta_y = 0;

            string direction_name=sd->getObjectName(directionID);

            if (direction_name == "up" )
                delta_x = -1;
            if (direction_name == "down")
                delta_x = 1;
            if (direction_name == "left")
                delta_y = -1;
            if (direction_name == "right")
                delta_y = 1;

            if (direction_name=="left-up")
            {
                delta_x=-1;
                delta_y=-1;
            }
            if (direction_name=="right-up"){
                delta_x=-1;
                delta_y=1;
            }
            if (direction_name=="left-down"){
                delta_x=1;
                delta_y=-1;
            }
            if (direction_name=="right-down"){
                delta_x=1;
                delta_y=1;
            }

        vector<int> newPixel;
        map<int,int> newPixelColor;

        string step_name=sd->getObjectName(stepID);

            if (step_name=="one"){

                for (int i=0; i<pixels.size();i++){

                    auto pixel=graph->getPixel(pixels[i]);
                    auto location=pixel->getLocation();
                    int new_x=location[0]+delta_x;
                    int new_y=location[1]+delta_y;
                    if (not graph->inbound(new_x,new_y))
                        return -1;

                    vector<int> new_location;
                    new_location.push_back(new_x);
                    new_location.push_back(new_y);

                    auto new_pixel_id =graph->getPixel(new_location);
                    auto new_pixel=graph->getPixel(new_pixel_id);

                    newPixelColor[new_pixel_id]=pixel->getColor();
                    newPixel.push_back(new_pixel_id);
                }
            }

            if(step_name=="max"){


                int max_allowed=1000;
                int distance=0;

                vector<int> new_x_location;
                vector<int> new_y_location;

                for (int j=0; j<pixels.size();j++){
                    auto pixel=graph->getPixel(pixels[j]);
                    new_x_location.push_back(pixel->getX());
                    new_y_location.push_back(pixel->getY());

                }


                for (int i=0; i<max_allowed;i++){
                    for (int j=0; j<pixels.size();j++){
                        new_x_location[j]=new_x_location[j]+delta_x;
                        new_y_location[j]=new_y_location[j]+delta_y;
                    }
                    distance+=1;
                    if ( not graph->inbound(new_x_location,new_y_location))
                        break;
                }

                if (distance<=1)
                    return -1;

                for (int j=0; j<pixels.size();j++){
                    int new_x=new_x_location[j];
                    int new_y=new_y_location[j];
                    if (distance!=0){
                        new_x=new_x_location[j]-delta_x;
                        new_y=new_y_location[j]-delta_y;
                    }
                    vector<int> new_location;
                    new_location.push_back(new_x);
                    new_location.push_back(new_y);
                    int new_pixel_id=graph->getPixel(new_location);
                    auto new_pixel=graph->getPixel(new_pixel_id);
                    newPixel.push_back(new_pixel_id);
                    newPixelColor[new_pixel_id]=graph->getPixel(pixels[j])->getColor();
                }


            }





        graphNode->clearPixelKeepSize();


            sort(pixels.begin(), pixels.end());
            sort(newPixel.begin(), newPixel.end());
            vector<int> resultPixel;
            vector<int> resultNewPixel;
            vector<int> commonPixel;
            set_difference(pixels.begin(), pixels.end(),
                           newPixel.begin(), newPixel.end(),
                           std::back_inserter(resultPixel));

            set_difference(newPixel.begin(), newPixel.end(),
                           pixels.begin(), pixels.end(),
                           std::back_inserter(resultNewPixel));
            std::set_intersection(pixels.begin(), pixels.end(),
                                  newPixel.begin(), newPixel.end(),
                                  std::back_inserter(commonPixel));

        for (auto pixelID:resultPixel){
            auto pixel=graph->getPixel(pixelID);



            vector< int > pixel_obj_idx ( 2 );
            pixel_obj_idx[0]=s->getObject(sd,pixelID);
            pixel_obj_idx[1]=s->getObject(sd,pixel->getColor());

            s->addRegister( sd, "pixel-color", pixel_obj_idx, 0 );
            pixel->setColor(graph->getBackGroundColor());

            pixel_obj_idx[1]=s->getObject(sd,pixel->getColor());

            s->addRegister( sd, "pixel-color", pixel_obj_idx, 1 );

            pixel_obj_idx[0]=s->getObject(sd,graphNode->getNodeID());
            pixel_obj_idx[1]=s->getObject(sd,pixelID);


            s->addRegister( sd, "contain-pixel", pixel_obj_idx, 0 );



        }



        for (auto pixelID: commonPixel){
            auto coPixel=graph->getPixel(pixelID);
            graphNode->setPixel(pixelID,coPixel);

        }

        for (auto pixelID:resultNewPixel){

            auto newPixel=graph->getPixel(pixelID);


            graphNode->setPixel(pixelID,newPixel);

            vector< int > pixel_obj_idx ( 2);
            pixel_obj_idx[0]=s->getObject(sd,pixelID);
            pixel_obj_idx[1]=s->getObject(sd,newPixel->getColor());

            s->addRegister( sd, "pixel-color", pixel_obj_idx, 0 );

            newPixel->setColor(newPixelColor[pixelID]);

            pixel_obj_idx[1]=s->getObject(sd,newPixel->getColor());

            s->addRegister( sd, "pixel-color", pixel_obj_idx, 1 );

            pixel_obj_idx[0]=s->getObject(sd,graphNode->getNodeID());
            pixel_obj_idx[1]=s->getObject(sd,newPixel->getID());

            s->addRegister( sd, "contain-pixel", pixel_obj_idx, 1 );

        }
        return  1;


    }

    virtual int  move_node_max(StateDescriptor *sd, State *s, int number) const{

        int id = _lhs->getID();
        string vtype = _lhs->getVType();
        vector< int > params = _lhs->getParameterIDs();
        int node_value= s->getPointer(sd, params[0] );
        int node_type_id=s->getNodeID(node_value);
        auto graph=s->getGraph();
        auto graphNode=graph->getNode(node_type_id);

        int node_value_2= s->getPointer(sd, params[1] );
        int node_type_id_2=s->getNodeID(node_value_2);
        auto graphNode2=graph->getNode(node_type_id_2);

        if (graphNode2->getNodeID()==graphNode->getNodeID()){
            return -1;
        }

        if (graphNode->getColor()==graph->getBackGroundColor() or graphNode2->getColor()==graph->getBackGroundColor() ){
            return -1;
        }



        vector<int> pixels=graphNode->getPixelsIDs();



        int delta_x = 0;
        int delta_y = 0;

        if (graphNode->checkLeft(node_type_id_2)){
            delta_y = -1;
        }
        else if (graphNode->checkRight(node_type_id_2)){
            delta_y = 1;
        }
        else if (graphNode->checkUp(node_type_id_2)){
            delta_x = -1;
        }
        else if (graphNode->checkDown(node_type_id_2)){
            delta_x = 1;
        }

        else if (graphNode->checkLeftUp(node_type_id_2))
        {
            delta_x=-1;
            delta_y=-1;
        }
        else if (graphNode->checkRightUp(node_type_id_2))
        {
            delta_x=-1;
            delta_y=1;
        }
        else if (graphNode->checkLeftDown(node_type_id_2))
        {
            delta_x=1;
            delta_y=-1;
        }
        else if (graphNode->checkRightDown(node_type_id_2))
        {
            delta_x=1;
            delta_y=1;
        } else
            return -1;




        int max_allowed=1000;
        int distance=0;

        vector<int> new_x_location;
        vector<int> new_y_location;

        for (int j=0; j<pixels.size();j++){
            auto pixel=graph->getPixel(pixels[j]);
            new_x_location.push_back(pixel->getX());
            new_y_location.push_back(pixel->getY());

        }


        for (int i=0; i<max_allowed;i++){
            for (int j=0; j<pixels.size();j++){
                new_x_location[j]=new_x_location[j]+delta_x;
                new_y_location[j]=new_y_location[j]+delta_y;
            }
            distance+=1;
            if (graph->check_meet(new_x_location,new_y_location,graphNode2) or not graph->inbound(new_x_location,new_y_location))
                break;
        }

        if (distance<=1)
            return -1;

        vector<int> newPixels;
        map<int,int> newPixelColor;

        for (int j=0; j<pixels.size();j++){
            int new_x=new_x_location[j];
            int new_y=new_y_location[j];
            if (distance!=0){
                new_x=new_x_location[j]-delta_x;
                new_y=new_y_location[j]-delta_y;
            }
            vector<int> new_location;
            new_location.push_back(new_x);
            new_location.push_back(new_y);
            int new_pixel_id=graph->getPixel(new_location);
            auto new_pixel=graph->getPixel(new_pixel_id);
            newPixels.push_back(new_pixel_id);
            newPixelColor[new_pixel_id]=graph->getPixel(pixels[j])->getColor();
        }


        graphNode->clearPixelKeepSize();



            sort(pixels.begin(), pixels.end());
            sort(newPixels.begin(), newPixels.end());
            vector<int> resultPixel;
            vector<int> resultNewPixel;
            vector<int> commonPixel;
            set_difference(pixels.begin(), pixels.end(),
                           newPixels.begin(), newPixels.end(),
                           std::back_inserter(resultPixel));

            set_difference(newPixels.begin(), newPixels.end(),
                           pixels.begin(), pixels.end(),
                           std::back_inserter(resultNewPixel));
            std::set_intersection(pixels.begin(), pixels.end(),
                                  newPixels.begin(), newPixels.end(),
                                  std::back_inserter(commonPixel));



            for (auto pixelID:resultPixel){
                auto pixel=graph->getPixel(pixelID);


                vector< int > pixel_obj_idx ( params.size() );
                pixel_obj_idx[0]=s->getObject(sd,pixelID);
                pixel_obj_idx[1]=s->getObject(sd,pixel->getColor());

                s->addRegister( sd, "pixel-color", pixel_obj_idx, 0 );

                pixel->setColor(graph->getBackGroundColor());

                pixel_obj_idx[1]=s->getObject(sd,pixel->getColor());
                s->addRegister( sd, "pixel-color", pixel_obj_idx, 1 );

                pixel_obj_idx[0]=s->getObject(sd,graphNode->getNodeID());
                pixel_obj_idx[1]=s->getObject(sd,pixelID);


                s->addRegister( sd, "contain-pixel", pixel_obj_idx, 0 );

            }



            for (auto pixelID: commonPixel){
                auto coPixel=graph->getPixel(pixelID);
                graphNode->setPixel(pixelID,coPixel);
            }

            for (auto pixelID:resultNewPixel){

                auto newPixel=graph->getPixel(pixelID);


                graphNode->setPixel(pixelID,newPixel);


                vector< int > pixel_obj_idx ( params.size() );
                pixel_obj_idx[0]=s->getObject(sd,pixelID);
                pixel_obj_idx[1]=s->getObject(sd,newPixel->getColor());

                s->addRegister( sd, "pixel-color", pixel_obj_idx, 0 );

                newPixel->setColor(newPixelColor[pixelID]);

                pixel_obj_idx[1]=s->getObject(sd,newPixel->getColor());

                s->addRegister( sd, "pixel-color", pixel_obj_idx, 1 );

                pixel_obj_idx[0]=s->getObject(sd,graphNode->getNodeID());
                pixel_obj_idx[1]=s->getObject(sd,newPixel->getID());

                s->addRegister( sd, "contain-pixel", pixel_obj_idx, 1 );

            }
        return 1;


    }



    virtual int  move_node_max_direction(StateDescriptor *sd, State *s, int number) const{

        int id = _lhs->getID();
        string vtype = _lhs->getVType();
        vector< int > params = _lhs->getParameterIDs();
        int node_value= s->getPointer(sd, params[0] );
        int node_type_id=s->getNodeID(node_value);
        auto graph=s->getGraph();
        auto graphNode=graph->getNode(node_type_id);

        vector<int> pixels=graphNode->getPixelsIDs();

        int directionID;
        if (sd->checkpointer(params[1])){

            int type_id=sd->getPointerVarType(params[1]);
            string type_name=sd->getVarName(type_id);
            int move_value = s->getPointer(sd, params[1] );

            if (type_name=="move-direction"){
                directionID=s->getDirectionID(move_value);
            }
            else if(type_name=="move-direction-diagonal"){
                directionID=s->getDirectionDiagonalID(move_value);
            }
            else{
                assert(type_name=="move-direction" or type_name=="move-direction-diagonal" );
            }

        }
        else{
            directionID =params[1];
        }

        int delta_x = 0;
        int delta_y = 0;

        string direction_name=sd->getObjectName(directionID);

        if (direction_name == "up" )
            delta_x = -1;
        if (direction_name == "down")
            delta_x = 1;
        if (direction_name == "left")
            delta_y = -1;
        if (direction_name == "right")
            delta_y = 1;

        if (direction_name=="left-up")
        {
            delta_x=-1;
            delta_y=-1;
        }
        if (direction_name=="right-up")
        {
            delta_x=-1;
            delta_y=1;
        }
        if (direction_name=="left-down")
        {
            delta_x=1;
            delta_y=-1;
        }
        if (direction_name=="right-down")
        {
            delta_x=1;
            delta_y=1;
        }

        auto nodes=graph->check_move_node(graphNode,delta_x,delta_y);


        while (!nodes.empty()){
            int nodeID=nodes.back();
            nodes.pop_back();
            graphNode=graph->getNode(nodeID);
            pixels=graphNode->getPixelsIDs();

            int max_allowed=1000;
            int distance=0;

            vector<int> new_x_location;
            vector<int> new_y_location;

            for (int j=0; j<pixels.size();j++){
                auto pixel=graph->getPixel(pixels[j]);
                new_x_location.push_back(pixel->getX());
                new_y_location.push_back(pixel->getY());

            }


            for (int i=0; i<max_allowed;i++){
                for (int j=0; j<pixels.size();j++){
                    new_x_location[j]=new_x_location[j]+delta_x;
                    new_y_location[j]=new_y_location[j]+delta_y;
                }
                distance+=1;
                if (graph->check_collision(graphNode,new_x_location,new_y_location) or not graph->inbound(new_x_location,new_y_location))
                    break;
            }

            if (distance<=1)
                continue;

            vector<int> newPixels;
            map<int,int> newPixelColor;

            for (int j=0; j<pixels.size();j++){
                int new_x=new_x_location[j];
                int new_y=new_y_location[j];
                if (distance!=0){
                    new_x=new_x_location[j]-delta_x;
                    new_y=new_y_location[j]-delta_y;
                }
                vector<int> new_location;
                new_location.push_back(new_x);
                new_location.push_back(new_y);
                int new_pixel_id=graph->getPixel(new_location);
                auto new_pixel=graph->getPixel(new_pixel_id);
                newPixels.push_back(new_pixel_id);
                newPixelColor[new_pixel_id]=graph->getPixel(pixels[j])->getColor();
            }


            graphNode->clearPixelKeepSize();



            sort(pixels.begin(), pixels.end());
            sort(newPixels.begin(), newPixels.end());
            vector<int> resultPixel;
            vector<int> resultNewPixel;
            vector<int> commonPixel;
            set_difference(pixels.begin(), pixels.end(),
                           newPixels.begin(), newPixels.end(),
                           std::back_inserter(resultPixel));

            set_difference(newPixels.begin(), newPixels.end(),
                           pixels.begin(), pixels.end(),
                           std::back_inserter(resultNewPixel));
            std::set_intersection(pixels.begin(), pixels.end(),
                                  newPixels.begin(), newPixels.end(),
                                  std::back_inserter(commonPixel));



            for (auto pixelID:resultPixel){
                auto pixel=graph->getPixel(pixelID);

                vector< int > pixel_obj_idx ( params.size() );
                pixel_obj_idx[0]=s->getObject(sd,pixelID);
                pixel_obj_idx[1]=s->getObject(sd,pixel->getColor());

                s->addRegister( sd, "pixel-color", pixel_obj_idx, 0 );
                pixel->setColor(graph->getBackGroundColor());

                pixel_obj_idx[1]=s->getObject(sd,pixel->getColor());
                s->addRegister( sd, "pixel-color", pixel_obj_idx, 1 );

                pixel_obj_idx[0]=s->getObject(sd,graphNode->getNodeID());
                pixel_obj_idx[1]=s->getObject(sd,pixelID);


                s->addRegister( sd, "contain-pixel", pixel_obj_idx, 0 );

            }



            for (auto pixelID: commonPixel){
                auto coPixel=graph->getPixel(pixelID);
                graphNode->setPixel(pixelID,coPixel);
            }

            for (auto pixelID:resultNewPixel){

                auto newPixel=graph->getPixel(pixelID);


                graphNode->setPixel(pixelID,newPixel);


                vector< int > pixel_obj_idx ( params.size() );
                pixel_obj_idx[0]=s->getObject(sd,pixelID);
                pixel_obj_idx[1]=s->getObject(sd,newPixel->getColor());

                s->addRegister( sd, "pixel-color", pixel_obj_idx, 0 );

                newPixel->setColor(newPixelColor[pixelID]);

                pixel_obj_idx[1]=s->getObject(sd,newPixel->getColor());

                s->addRegister( sd, "pixel-color", pixel_obj_idx, 1 );

                pixel_obj_idx[0]=s->getObject(sd,graphNode->getNodeID());
                pixel_obj_idx[1]=s->getObject(sd,newPixel->getID());

                s->addRegister( sd, "contain-pixel", pixel_obj_idx, 1 );

            }


        }

        return 1;


    }
    virtual int  move_between_nodes_direction(StateDescriptor *sd, State *s, int number) const{

        int id = _lhs->getID();
        string vtype = _lhs->getVType();
        vector< int > params = _lhs->getParameterIDs();
        int node_value= s->getPointer(sd, params[0] );
        int node_type_id=s->getNodeID(node_value);
        auto graph=s->getGraph();
        auto graphNode=graph->getNode(node_type_id);

        vector<int> pixels=graphNode->getPixelsIDs();

        int node_value_2= s->getPointer(sd, params[1] );
        int node_type_id_2=s->getNodeID(node_value_2);
        auto graphNode2=graph->getNode(node_type_id_2);

        if (graphNode2->getNodeID()==graphNode->getNodeID()){
            return -1;
        }

        if (graphNode->getColor()==graph->getBackGroundColor() or graphNode2->getColor()==graph->getBackGroundColor() ){
            return -1;
        }



        int delta_x = 0;
        int delta_y = 0;

        if (graphNode->checkLeft(node_type_id_2)){
            delta_y = -1;
        }
        else if (graphNode->checkRight(node_type_id_2)){
            delta_y = 1;
        }
        else if (graphNode->checkUp(node_type_id_2)){
            delta_x = -1;
        }
        else if (graphNode->checkDown(node_type_id_2)){
            delta_x = 1;
        }

        else if (graphNode->checkLeftUp(node_type_id_2))
        {
            delta_x=-1;
            delta_y=-1;
        }
        else if (graphNode->checkRightUp(node_type_id_2))
        {
            delta_x=-1;
            delta_y=1;
        }
        else if (graphNode->checkLeftDown(node_type_id_2))
        {
            delta_x=1;
            delta_y=-1;
        }
        else if (graphNode->checkRightDown(node_type_id_2))
        {
            delta_x=1;
            delta_y=1;
        } else
            return -1;

        auto nodes=graph->check_move_node_with_destination(graphNode,delta_x,delta_y,node_type_id_2);

        while (!nodes.empty()){

            int nodeID=nodes.back();
            nodes.pop_back();
            graphNode=graph->getNode(nodeID);
            pixels=graphNode->getPixelsIDs();

            int max_allowed=1000;
            int distance=0;

            vector<int> new_x_location;
            vector<int> new_y_location;

            for (int j=0; j<pixels.size();j++){
                auto pixel=graph->getPixel(pixels[j]);
                new_x_location.push_back(pixel->getX());
                new_y_location.push_back(pixel->getY());

            }


            for (int i=0; i<max_allowed;i++){
                for (int j=0; j<pixels.size();j++){
                    new_x_location[j]=new_x_location[j]+delta_x;
                    new_y_location[j]=new_y_location[j]+delta_y;
                }
                distance+=1;
                if (graph->check_collision(graphNode,new_x_location,new_y_location) or not graph->inbound(new_x_location,new_y_location))
                    break;
            }

            if (distance<=1)
                continue;

            vector<int> newPixels;
            map<int,int> newPixelColor;

            for (int j=0; j<pixels.size();j++){
                int new_x=new_x_location[j];
                int new_y=new_y_location[j];
                if (distance!=0){
                    new_x=new_x_location[j]-delta_x;
                    new_y=new_y_location[j]-delta_y;
                }
                vector<int> new_location;
                new_location.push_back(new_x);
                new_location.push_back(new_y);
                int new_pixel_id=graph->getPixel(new_location);
                auto new_pixel=graph->getPixel(new_pixel_id);
                newPixels.push_back(new_pixel_id);
                newPixelColor[new_pixel_id]=graph->getPixel(pixels[j])->getColor();
            }


            graphNode->clearPixelKeepSize();



            sort(pixels.begin(), pixels.end());
            sort(newPixels.begin(), newPixels.end());
            vector<int> resultPixel;
            vector<int> resultNewPixel;
            vector<int> commonPixel;
            set_difference(pixels.begin(), pixels.end(),
                           newPixels.begin(), newPixels.end(),
                           std::back_inserter(resultPixel));

            set_difference(newPixels.begin(), newPixels.end(),
                           pixels.begin(), pixels.end(),
                           std::back_inserter(resultNewPixel));
            std::set_intersection(pixels.begin(), pixels.end(),
                                  newPixels.begin(), newPixels.end(),
                                  std::back_inserter(commonPixel));



            for (auto pixelID:resultPixel){
                auto pixel=graph->getPixel(pixelID);

                vector< int > pixel_obj_idx ( 2 );
                pixel_obj_idx[0]=s->getObject(sd,pixelID);
                pixel_obj_idx[1]=s->getObject(sd,pixel->getColor());

                s->addRegister( sd, "pixel-color", pixel_obj_idx, 0 );
                pixel->setColor(graph->getBackGroundColor());


                pixel_obj_idx[1]=s->getObject(sd,pixel->getColor());
                s->addRegister( sd, "pixel-color", pixel_obj_idx, 1 );

                pixel_obj_idx[0]=s->getObject(sd,graphNode->getNodeID());
                pixel_obj_idx[1]=s->getObject(sd,pixelID);


                s->addRegister( sd, "contain-pixel", pixel_obj_idx, 0 );

            }



            for (auto pixelID: commonPixel){
                auto coPixel=graph->getPixel(pixelID);
                graphNode->setPixel(pixelID,coPixel);
            }

            for (auto pixelID:resultNewPixel){

                auto newPixel=graph->getPixel(pixelID);


                graphNode->setPixel(pixelID,newPixel);


                vector< int > pixel_obj_idx ( 2);
                pixel_obj_idx[0]=s->getObject(sd,pixelID);
                pixel_obj_idx[1]=s->getObject(sd,newPixel->getColor());

                s->addRegister( sd, "pixel-color", pixel_obj_idx, 0 );

                newPixel->setColor(newPixelColor[pixelID]);

                pixel_obj_idx[1]=s->getObject(sd,newPixel->getColor());

                s->addRegister( sd, "pixel-color", pixel_obj_idx, 1 );

                pixel_obj_idx[0]=s->getObject(sd,graphNode->getNodeID());
                pixel_obj_idx[1]=s->getObject(sd,newPixel->getID());

                s->addRegister( sd, "contain-pixel", pixel_obj_idx, 1 );

            }


        }

        return 1;


    }


    virtual int extend_node_direction(StateDescriptor *sd, State *s, int number){

        int id = _lhs->getID();
        string vtype = _lhs->getVType();
        vector< int > params = _lhs->getParameterIDs();
        int node_value= s->getPointer(sd, params[0] );
        int node_type_id=s->getNodeID(node_value);

        auto graph=s->getGraph();
        auto graphNode=graph->getNode(node_type_id);


        vector<int> pixels=graphNode->getPixelsIDs();

        int delta_x = 0;
        int delta_y = 0;


        int directionID;
        if (sd->checkpointer(params[1])){
            int type_id=sd->getPointerVarType(params[1]);
            string type_name=sd->getVarName(type_id);
            int move_value = s->getPointer(sd, params[1] );

            if (type_name=="move-direction"){
                directionID=s->getDirectionID(move_value);
            }
            else if(type_name=="move-direction-diagonal"){
                directionID=s->getDirectionDiagonalID(move_value);
            }
            else{
                assert(type_name=="move-direction" or type_name=="move-direction-diagonal" );
            }

        }
        else{
            directionID =params[1];
        }



        string direction_name=sd->getObjectName(directionID);

        if (direction_name == "up" )
            delta_x = -1;
        if (direction_name == "down")
            delta_x = 1;
        if (direction_name == "left")
            delta_y = -1;
        if (direction_name == "right")
            delta_y = 1;

        if (direction_name=="left-up")
        {
            delta_x=-1;
            delta_y=-1;
        }
        if (direction_name=="right-up"){
            delta_x=-1;
            delta_y=1;
        }
        if (direction_name=="left-down"){
            delta_x=1;
            delta_y=-1;
        }
        if (direction_name=="right-down"){
            delta_x=1;
            delta_y=1;
        }

        int max_allowed=1000;


        vector<int> new_x_location;
        vector<int> new_y_location;
        vector<vector<int>> nodeLocations=graphNode->getLocations();


        for (auto pixelID: pixels){

            int new_x=graph->getPixel(pixelID)->getX();
            int new_y=graph->getPixel(pixelID)->getY();

            vector<int>check={new_x+delta_x,new_y+delta_y};

            if (std::find(nodeLocations.begin(), nodeLocations.end(),check)!=nodeLocations.end()){
                continue;
            }

            for (int i=0; i<max_allowed; i++){
                new_x_location.push_back(new_x);
                new_y_location.push_back(new_y);
                new_x+=delta_x;
                new_y+=delta_y;
                if (graph->check_collision(graphNode,new_x, new_y) or not graph->inbound(new_x,new_y))
                {
                    new_x-=delta_x;
                    new_y-=delta_y;

                    break;
                }
            }

        }

        int newColor=graphNode->getColor();
        for (int i=0; i<new_x_location.size();i++){


            int x=new_x_location[i];
            int y=new_y_location[i];
            vector<int> position={x,y};

            int new_pixelID=graph->getPixel(position);
            auto pixel=graph->getPixel(new_pixelID);


            if (graphNode->isPixel(new_pixelID))
                continue;




            vector< int > pixel_obj_idx ( params.size() );
            pixel_obj_idx[0]=s->getObject(sd,new_pixelID);
            pixel_obj_idx[1]=s->getObject(sd,pixel->getColor());
            s->addRegister( sd, "pixel-color", pixel_obj_idx, 0 );
            pixel->setColor(newColor);

            pixel_obj_idx[1]=s->getObject(sd,pixel->getColor());
            s->addRegister( sd, "pixel-color", pixel_obj_idx, 1 );


        }

        return 1;

    }


    virtual int extend_node(StateDescriptor *sd, State *s, int number){

        int id = _lhs->getID();
        string vtype = _lhs->getVType();
        vector< int > params = _lhs->getParameterIDs();
        int node_value= s->getPointer(sd, params[0] );
        int node_type_id=s->getNodeID(node_value);

        auto graph=s->getGraph();
        auto graphNode=graph->getNode(node_type_id);


        int node_value_2= s->getPointer(sd, params[1] );
        int node_type_id_2=s->getNodeID(node_value_2);
        auto graphNode2=graph->getNode(node_type_id_2);

        if (graphNode2->getNodeID()==graphNode->getNodeID()){
            return -1;
        }

        if (graphNode->getColor()==graph->getBackGroundColor() or graphNode2->getColor()==graph->getBackGroundColor() ){
            return -1;
        }



        vector<int> pixels=graphNode->getPixelsIDs();



        int delta_x = 0;
        int delta_y = 0;

        if (graphNode->checkLeft(node_type_id_2)){
            delta_y = -1;
        }
        else if (graphNode->checkRight(node_type_id_2)){
            delta_y = 1;
        }
        else if (graphNode->checkUp(node_type_id_2)){
            delta_x = -1;
        }
        else if (graphNode->checkDown(node_type_id_2)){
            delta_x = 1;
        }

        else if (graphNode->checkLeftUp(node_type_id_2))
        {
            delta_x=-1;
            delta_y=-1;
        }
        else if (graphNode->checkRightUp(node_type_id_2))
        {
            delta_x=-1;
            delta_y=1;
        }
        else if (graphNode->checkLeftDown(node_type_id_2))
        {
            delta_x=1;
            delta_y=-1;
        }
        else if (graphNode->checkRightDown(node_type_id_2))
        {
            delta_x=1;
            delta_y=1;
        } else
            return -1;


        int max_allowed=1000;


        vector<int> new_x_location;
        vector<int> new_y_location;
        vector<vector<int>> nodeLocations=graphNode->getLocations();
        for (auto pixelID: pixels){

            int new_x=graph->getPixel(pixelID)->getX();
            int new_y=graph->getPixel(pixelID)->getY();

            vector<int>check={new_x+delta_x,new_y+delta_y};

            if (std::find(nodeLocations.begin(), nodeLocations.end(),check)!=nodeLocations.end()){
                continue;
            }

            for (int i=0; i<max_allowed; i++){
                new_x_location.push_back(new_x);
                new_y_location.push_back(new_y);
                new_x+=delta_x;
                new_y+=delta_y;
                if (graph->check_meet(new_x, new_y,graphNode2) or not graph->inbound(new_x,new_y))
                {
                    new_x-=delta_x;
                    new_y-=delta_y;

                    break;
                }
            }

        }

        int newColor=graphNode->getColor();
        for (int i=0; i<new_x_location.size();i++){


            int x=new_x_location[i];
            int y=new_y_location[i];
            vector<int> position={x,y};

            int new_pixelID=graph->getPixel(position);
            auto pixel=graph->getPixel(new_pixelID);


            if (graphNode->isPixel(new_pixelID))
                continue;



            vector< int > pixel_obj_idx ( params.size() );
            pixel_obj_idx[0]=s->getObject(sd,new_pixelID);
            pixel_obj_idx[1]=s->getObject(sd,pixel->getColor());
            s->addRegister( sd, "pixel-color", pixel_obj_idx, 0 );
            pixel->setColor(newColor);

            pixel_obj_idx[1]=s->getObject(sd,pixel->getColor());
            s->addRegister( sd, "pixel-color", pixel_obj_idx, 1 );


        }

        return 1;

    }


    virtual int rotate_node(StateDescriptor *sd, State *s, int number){

        int id = _lhs->getID();
        string vtype = _lhs->getVType();
        vector< int > params = _lhs->getParameterIDs();
        int node_value= s->getPointer(sd, params[0] );
        int node_type_id=s->getNodeID(node_value);

        auto graph=s->getGraph();
        auto graphNode=graph->getNode(node_type_id);

        int rotate_times = 1;
        int mul=0;

        auto pixelsIDs=graphNode->getPixelsIDs();

        string direction_name=sd->getObjectName(params[1]);

        if (direction_name == "90" )
            mul=1;

        if (direction_name == "180")
        {
            mul = -1;
            rotate_times=2;
        }

        if (direction_name == "270")
            mul = -1;



        vector<int> new_x_location=graphNode->get_x_locations();
        vector<int> new_y_location=graphNode->get_y_locations();


        for (int i=0; i<rotate_times;i++){
            int center_x= (int) round((accumulate(new_x_location.begin(),new_x_location.end(),0)+ (int) round(new_x_location.size()/2))/new_x_location.size());
            int center_y= (int) round((accumulate(new_y_location.begin(),new_y_location.end(),0)+(int) round(new_y_location.size()/2))/new_y_location.size());

            for (int j=0; j<pixelsIDs.size();j++){

                new_x_location[j]=new_x_location[j]-center_x;
                new_y_location[j]=new_y_location[j]-center_y;

                int tmp=new_x_location[j];

                new_x_location[j]=-new_y_location[j]*mul;


                new_y_location[j]=tmp*mul;

                new_x_location[j]=new_x_location[j]+center_x;
                new_y_location[j]=new_y_location[j]+center_y;

                if (not graph->inbound(new_x_location[j],new_y_location[j])){
                    return -1;
                }

            }
        }


        vector<int> newPixel;
        map<Pixel*,int> newPixelColor;


        for (int j=0; j<pixelsIDs.size();j++){

            auto oldpixel=graph->getPixel(pixelsIDs[j]);


            int new_x=new_x_location[j];
            int new_y=new_y_location[j];

            vector<int> new_location;
            new_location.push_back(new_x);
            new_location.push_back(new_y);



            int new_pixelID=graph->getPixel(new_location);
            auto new_pixel=graph->getPixel(new_pixelID);


            newPixelColor[new_pixel]=oldpixel->getColor();
            newPixel.push_back(new_pixel->getID());


        }

        if (not graph->check_collision(graphNode,new_x_location,new_y_location)){

            sort(pixelsIDs.begin(), pixelsIDs.end());
            sort(newPixel.begin(), newPixel.end());


            vector<int> resultPixel;

            set_difference(pixelsIDs.begin(), pixelsIDs.end(),
                           newPixel.begin(), newPixel.end(),
                           std::back_inserter(resultPixel));



            graphNode->clearPixelKeepSize();

            for (auto pixel: resultPixel){

                auto oldpixel=graph->getPixel(pixel);



                vector< int > pixel_obj_idx ( params.size() );
                pixel_obj_idx[0]=s->getObject(sd,pixel);
                pixel_obj_idx[1]=s->getObject(sd,oldpixel->getColor());
                s->addRegister( sd, "pixel-color", pixel_obj_idx, 0 );

                graph->getPixel(pixel)->setColor(graph->getBackGroundColor());



                pixel_obj_idx[1]=s->getObject(sd,graph->getBackGroundColor());

                s->addRegister( sd, "pixel-color", pixel_obj_idx, 1 );



                pixel_obj_idx[0]=s->getObject(sd,graphNode->getNodeID());
                pixel_obj_idx[1]=s->getObject(sd,pixel);

                s->addRegister( sd, "contain-pixel", pixel_obj_idx, 0 );

            }




            for (auto it: newPixelColor){

                auto newPixel=it.first;
                int newPixelID=newPixel->getID();



                vector< int > pixel_obj_idx ( params.size() );
                pixel_obj_idx[0]=s->getObject(sd,newPixelID);
                pixel_obj_idx[1]=s->getObject(sd,newPixel->getColor());

                s->addRegister( sd, "pixel-color", pixel_obj_idx, 0 );

                newPixel->setColor(it.second);

                pixel_obj_idx[0]=s->getObject(sd,newPixelID);
                pixel_obj_idx[1]=s->getObject(sd,it.second);

                s->addRegister( sd, "pixel-color", pixel_obj_idx, 1 );


                pixel_obj_idx[0]=s->getObject(sd,graphNode->getNodeID());
                pixel_obj_idx[1]=s->getObject(sd,newPixelID);

                graphNode->setPixel(newPixelID,newPixel);



                s->addRegister( sd, "contain-pixel", pixel_obj_idx, 1 );


            }
            return 1;
        }
        else{
            return -1;
        }



    }


    virtual int add_border(StateDescriptor *sd, State *s, int number){

        int id = _lhs->getID();
        string vtype = _lhs->getVType();
        vector< int > params = _lhs->getParameterIDs();
        int node_value= s->getPointer(sd, params[0] );
        int node_type_id=s->getNodeID(node_value);

        auto graph=s->getGraph();
        auto graphNode=graph->getNode(node_type_id);

        vector<int> pixels= graphNode->getPixelsIDs();

        int rotate_times = 1;
        int mul=0;

        int color_value;

        if (sd->checkpointer(params[1])){
            int old_color_pointer = s->getPointer(sd, params[1] );
            color_value=s->getColorID(old_color_pointer);

        }
        else{
            color_value =params[1];
        }


        vector <int> delta = {-1,0,1};

        vector<vector<int>> newborder;

        auto x_locations=graphNode->get_x_locations();
        auto y_locations=graphNode->get_y_locations();
        for (int j=0; j<pixels.size();j++){
            auto pixel=graph->getPixel(pixels[j]);
            for (auto x: delta)
            {
                for (auto y:delta)
                {
                    auto new_x=pixel->getX()+x;
                    auto new_y=pixel->getY()+y;
                    vector<int> location={new_x,new_y};
                    if  ( not graph->check_collision(graphNode,new_x,new_y) and graph->inbound(new_x,new_y) and graph->getPixel(graph->getPixel(location))->getColor()==graph->getBackGroundColor())
                        if (std::find(x_locations.begin(), x_locations.end(),new_x)==x_locations.end() or std::find(y_locations.begin(), y_locations.end(),new_y)==y_locations.end())
                        {
                            vector<int> new_pixel={new_x,new_y};
                            bool exist= false;
                            for (auto v: newborder){
                                if (v==new_pixel){
                                    exist= true;
                                    break;
                                }
                            }
                            if (not exist)
                                newborder.push_back({new_x,new_y});
                        }
                }

            }

        }
        if (newborder.size()==0)
            return -1;


        for (auto pixel_x_y:newborder){

            auto pixelID =graph->getPixel(pixel_x_y);
            auto pixel=graph->getPixel(pixelID);



            vector< int > pixel_obj_idx ( params.size() );
            pixel_obj_idx[0]=s->getObject(sd,pixelID);
            pixel_obj_idx[1]=s->getObject(sd,pixel->getColor());

            s->addRegister( sd, "pixel-color", pixel_obj_idx, 0 );

            pixel->setColor(color_value);

            pixel_obj_idx[1]=s->getObject(sd,pixel->getColor());

            s->addRegister( sd, "pixel-color", pixel_obj_idx, 1 );


        }

        return 1;


        // should we treat the border pixel as a new node or should we add then into original node
        // if we treat a border as a new node, can we add a new object during the search?

    }


    virtual int mirror (StateDescriptor *sd, State *s, int number){

        int id = _lhs->getID();
        string vtype = _lhs->getVType();
        vector< int > params = _lhs->getParameterIDs();
        int node_value= s->getPointer(sd, params[0] );
        int node_type_id=s->getNodeID(node_value);
        auto graph=s->getGraph();
        auto graphNode=graph->getNode(node_type_id);

        int node_value2= s->getPointer(sd, params[1] );
        int node_type_id2=s->getNodeID(node_value2);

        auto graphNode2 =graph->getNode(node_type_id2);

        if (graphNode->getNodeID()==graphNode2->getNodeID())
            return -1;

        int mirror_x_axis_value= -1;
        int mirror_y_axis_value= -1;

        vector<int> x_location=graphNode2->get_x_locations();
        vector<int> y_location=graphNode2->get_y_locations();

        int center_x= (int) round((accumulate(x_location.begin(),x_location.end(),0)+(int) round(x_location.size()/2))/x_location.size());
        int center_y= (int) round((accumulate(y_location.begin(),y_location.end(),0)+(int) round(y_location.size()/2))/y_location.size());

        if (graphNode->checkUp(graphNode2->getNodeID()) or graphNode->checkDown(graphNode2->getNodeID())) {
            mirror_x_axis_value = center_x;
        }

        if(graphNode->checkRight(graphNode2->getNodeID()) or graphNode->checkLeft(graphNode2->getNodeID())){
            mirror_y_axis_value = center_y;
        }

        if (mirror_x_axis_value==-1 and mirror_y_axis_value==-1){
            return -1;
        }




        auto pixels= graphNode->getPixelsIDs();


        vector<int> newPixels;
        map<int,int> newPixelColor;


        if (mirror_x_axis_value!=-1 ){

            for (int j=0; j<pixels.size();j++){
                auto pixel=graph->getPixel(pixels[j]);
                auto new_x=mirror_x_axis_value-(pixel->getX()-mirror_x_axis_value);
                auto new_y=pixel->getY();
                if ( not graph->inbound(new_x,new_y))
                    return -1;
                if (graph->check_collision(graphNode,new_x,new_y))
                    return -1;

                vector<int>new_node_location={new_x,new_y};

                auto new_pixel_id =graph->getPixel(new_node_location);
                auto new_pixel=graph->getPixel(new_pixel_id);

                newPixelColor[new_pixel_id]=pixel->getColor();
                newPixels.push_back(new_pixel_id);

            }
        }
        if (mirror_y_axis_value!=-1){

            for (int j=0; j<pixels.size();j++){
                auto pixel=graph->getPixel(pixels[j]);
                auto new_x=pixel->getX();
                auto new_y=mirror_y_axis_value-(pixel->getY()-mirror_y_axis_value);
                if ( not graph->inbound(new_x,new_y))
                    return -1;
                if (graph->check_collision(graphNode,new_x,new_y))
                    return -1;

                vector<int>new_node_location={new_x,new_y};

                auto new_pixel_id =graph->getPixel(new_node_location);
                auto new_pixel=graph->getPixel(new_pixel_id);

                newPixelColor[new_pixel_id]=pixel->getColor();
                newPixels.push_back(new_pixel_id);
            }
        }



        sort(pixels.begin(), pixels.end());
        sort(newPixels.begin(), newPixels.end());
        vector<int> resultPixel;
        vector<int> resultNewPixel;
        vector<int> commonPixel;
        set_difference(pixels.begin(), pixels.end(),
                       newPixels.begin(), newPixels.end(),
                       std::back_inserter(resultPixel));

        set_difference(newPixels.begin(), newPixels.end(),
                       pixels.begin(), pixels.end(),
                       std::back_inserter(resultNewPixel));
        std::set_intersection(pixels.begin(), pixels.end(),
                              newPixels.begin(), newPixels.end(),
                              std::back_inserter(commonPixel));


        graphNode->clearPixelKeepSize();




        for (auto pixelID:resultPixel){
                auto pixel=graph->getPixel(pixelID);


                vector< int > pixel_obj_idx ( params.size() );
                pixel_obj_idx[0]=s->getObject(sd,pixelID);
                pixel_obj_idx[1]=s->getObject(sd,pixel->getColor());

                s->addRegister( sd, "pixel-color", pixel_obj_idx, 0 );
            pixel->setColor(graph->getBackGroundColor());

                pixel_obj_idx[1]=s->getObject(sd,pixel->getColor());
                s->addRegister( sd, "pixel-color", pixel_obj_idx, 1 );

                pixel_obj_idx[0]=s->getObject(sd,graphNode->getNodeID());
                pixel_obj_idx[1]=s->getObject(sd,pixelID);


                s->addRegister( sd, "contain-pixel", pixel_obj_idx, 0 );

            }



            for (auto pixelID: commonPixel){
                auto coPixel=graph->getPixel(pixelID);
                graphNode->setPixel(pixelID,coPixel);
            }

            for (auto pixelID:resultNewPixel){

                auto newPixel=graph->getPixel(pixelID);


                graphNode->setPixel(pixelID,newPixel);



                vector< int > pixel_obj_idx ( params.size() );
                pixel_obj_idx[0]=s->getObject(sd,pixelID);
                pixel_obj_idx[1]=s->getObject(sd,newPixel->getColor());

                s->addRegister( sd, "pixel-color", pixel_obj_idx, 0 );

                newPixel->setColor(newPixelColor[pixelID]);

                pixel_obj_idx[1]=s->getObject(sd,newPixel->getColor());

                s->addRegister( sd, "pixel-color", pixel_obj_idx, 1 );

                pixel_obj_idx[0]=s->getObject(sd,graphNode->getNodeID());
                pixel_obj_idx[1]=s->getObject(sd,newPixel->getID());

                s->addRegister( sd, "contain-pixel", pixel_obj_idx, 1 );

            }


        return -1;

    }

    virtual int flip(StateDescriptor *sd, State *s, int number) {
        int id = _lhs->getID();
        string vtype = _lhs->getVType();
        vector<int> params = _lhs->getParameterIDs();
        int node_value = s->getPointer(sd, params[0]);
        int node_type_id = s->getNodeID(node_value);
        auto graph = s->getGraph();
        auto graphNode = graph->getNode(node_type_id);

        string direction = sd->getObjectName(params[1]);

        auto pixelsIDs= graphNode->getPixelsIDs();


        vector<int> new_x_location;
        vector<int> new_y_location;
        vector<vector<int>> new_pixel_color;
        vector<int> new_pixels;

        auto x_locations = graphNode->get_x_locations();
        auto y_locations = graphNode->get_y_locations();
        //VERTICAL
        if (direction == "VERTICAL") {
            int x_min = graphNode->get_x_edge_min();
            int x_max = graphNode->get_x_edge_max();
            for (int j = 0; j < pixelsIDs.size(); j++) {
                auto pixel = graph->getPixel(pixelsIDs[j]);
                auto new_x = x_max - (pixel->getX() - x_min);
                auto new_y = pixel->getY();
                if (not graph->inbound(new_x,new_y))
                    return -1;

                new_x_location.push_back(new_x);
                new_y_location.push_back(new_y);
                vector<int> new_location = {new_x, new_y};

                auto new_pixel_id =graph->getPixel(new_location);
                auto new_pixel=graph->getPixel(new_pixel_id);

                new_pixel_color.push_back({new_pixel_id, pixel->getColor()});
                new_pixels.push_back(new_pixel_id);
            }
        }

        //HORIZONTAL
        if (direction == "HORIZONTAL") {
            int y_min = graphNode->get_y_edge_min();
            int y_max = graphNode->get_y_edge_max();
            for (int j = 0; j < pixelsIDs.size(); j++) {
                auto pixel = graph->getPixel(pixelsIDs[j]);
                auto new_x = pixel->getX();
                auto new_y = y_max - (pixel->getY() - y_min);
                if (not graph->inbound(new_x,new_y))
                    return -1;
                vector<int> new_location = {new_x, new_y};
                auto new_pixel_id =graph->getPixel(new_location);
                auto new_pixel=graph->getPixel(new_pixel_id);
                new_pixel_color.push_back({new_pixel_id, pixel->getColor()});
                new_pixels.push_back(new_pixel_id);
            }

        }
        //DIAGONAL_LEFT
        if (direction == "DIAGONAL_LEFT") {
            int x_min = graphNode->get_x_edge_min();
            int y_min = graphNode->get_y_edge_min();
            for (int j = 0; j < pixelsIDs.size(); j++) {
                auto pixel = graph->getPixel(pixelsIDs[j]);
                auto new_x = pixel->getX() - x_min;
                auto new_y = pixel->getY() - y_min;
                auto tmp = new_x;

                new_x = new_y;
                new_y = tmp;

                new_x = new_x + x_min;
                new_y = new_y + y_min;

                if (not graph->inbound(new_x, new_y))
                    return -1;

                vector<int> new_location = {new_x, new_y};
                auto new_pixel_id = graph->getPixel(new_location);
                auto new_pixel = graph->getPixel(new_pixel_id);
                new_pixel_color.push_back({new_pixel_id, pixel->getColor()});
                new_pixels.push_back(new_pixel_id);

            }
        }
            //DIAGONAL_RIGHT
            if (direction == "DIAGONAL_RIGHT") {
                int x_min = graphNode->get_x_edge_min();
                int y_max = graphNode->get_y_edge_max();
                for (int j = 0; j < pixelsIDs.size(); j++) {
                    auto pixel = graph->getPixel(pixelsIDs[j]);
                    auto new_x = pixel->getX() - x_min;
                    auto new_y = pixel->getY() - y_max;
                    auto tmp = new_x;

                    new_x = -new_y;
                    new_y = -tmp;

                    new_x = new_x + x_min;
                    new_y = new_y + y_max;

                    if (not graph->inbound(new_x,new_y))
                        return -1;

                    vector<int> new_location = {new_x, new_y};
                    auto new_pixel_id =graph->getPixel(new_location);
                    auto new_pixel=graph->getPixel(new_pixel_id);
                    new_pixel_color.push_back({new_pixel_id, pixel->getColor()});
                    new_pixels.push_back(new_pixel_id);
                }

            }




            sort(pixelsIDs.begin(), pixelsIDs.end());
            sort(new_pixels.begin(), new_pixels.end());
            vector<int> resultPixel;

            set_difference(pixelsIDs.begin(), pixelsIDs.end(),
                           new_pixels.begin(), new_pixels.end(),
                           std::back_inserter(resultPixel));



            if (not graph->check_collision(graphNode, new_x_location, new_y_location)) {
                graphNode->clearPixelKeepSize();

                for (auto pixel: resultPixel){
                    auto oldpixel=graph->getPixel(pixel);



                    vector< int > pixel_obj_idx ( params.size() );
                    pixel_obj_idx[0]=s->getObject(sd,pixel);
                    pixel_obj_idx[1]=s->getObject(sd,oldpixel->getColor());
                    s->addRegister( sd, "pixel-color", pixel_obj_idx, 0 );

                    graph->getPixel(pixel)->setColor(graph->getBackGroundColor());



                    pixel_obj_idx[1]=s->getObject(sd,graph->getBackGroundColor());


                    s->addRegister( sd, "pixel-color", pixel_obj_idx, 1 );


                    pixel_obj_idx[0]=s->getObject(sd,graphNode->getNodeID());
                    pixel_obj_idx[1]=s->getObject(sd,pixel);

                    s->addRegister( sd, "contain-pixel", pixel_obj_idx, 0 );

                }

                for (int i = 0; i < new_pixel_color.size(); i++) {

                    auto pixelID = new_pixel_color[i][0];
                    auto pixel_new_color = new_pixel_color[i][1];
                    auto pixel = graph->getPixel(pixelID);





                    vector<int> pixel_obj_idx(params.size());
                    pixel_obj_idx[0] = s->getObject(sd, pixelID);
                    pixel_obj_idx[1] = s->getObject(sd, pixel->getColor());

                    s->addRegister(sd, "pixel-color", pixel_obj_idx, 0);

                    pixel->setColor(pixel_new_color);

                    pixel_obj_idx[1] = s->getObject(sd, pixel_new_color);

                    s->addRegister(sd, "pixel-color", pixel_obj_idx, 1);



                    graphNode->setPixel(pixelID,pixel);

                    pixel_obj_idx[0]=s->getObject(sd,graphNode->getNodeID());
                    pixel_obj_idx[1]=s->getObject(sd,pixelID);

                    s->addRegister( sd, "contain-pixel", pixel_obj_idx, 1 );





                }
            }
            else{
                return -1;
        }

        return 1;
    }

    virtual int fill_rectangle(StateDescriptor *sd, State *s, int number){
        int id = _lhs->getID();
        string vtype = _lhs->getVType();
        vector<int> params = _lhs->getParameterIDs();
        int node_value = s->getPointer(sd, params[0]);
        int node_type_id = s->getNodeID(node_value);

        auto graph = s->getGraph();
        auto graphNode = graph->getNode(node_type_id);
        vector<int> pixels=graphNode->getPixelsIDs();

        auto size=graphNode->getSize();
        if (size==1)
            return -1;



        int given_color;

        if (sd->checkpointer(params[1])){
            int color_pointer = s->getPointer(sd, params[1] );
            given_color=s->getColorID(color_pointer);

        }
        else{
            given_color =params[1];
        }



        vector<vector<int>> new_pixels_locations;
        vector<vector<int>> locations;

        auto x_locations = graphNode->get_x_locations();
        auto y_locations = graphNode->get_y_locations();

        for (int i=0;i<x_locations.size();i++){
            locations.push_back({x_locations[i],y_locations[i]});
        }


        int x_min=graphNode->get_x_edge_min();
        int x_max=graphNode->get_x_edge_max();
        int y_min=graphNode->get_y_edge_min();
        int y_max=graphNode->get_y_edge_max();

        for (int i=x_min;i<x_max+1;i++){
            for (int j=y_min;j<y_max+1;j++){
                vector<int> check={i,j};
                    if (std::find(locations.begin(), locations.end(),check)==locations.end() and not graph->check_collision(graphNode,i,j))
                    {
                        new_pixels_locations.push_back(check);
                    }
            }
        }

        if (not new_pixels_locations.empty()){

            for (auto pixel_x_y:new_pixels_locations){


                auto pixelID =graph->getPixel(pixel_x_y);
                auto pixel=graph->getPixel(pixelID);



                vector< int > pixel_obj_idx ( params.size() );
                pixel_obj_idx[0]=s->getObject(sd,pixelID);
                pixel_obj_idx[1]=s->getObject(sd,pixel->getColor());

                s->addRegister( sd, "pixel-color", pixel_obj_idx, 0 );

                pixel->setColor(given_color);

                pixel_obj_idx[1]=s->getObject(sd,pixel->getColor());

                s->addRegister( sd, "pixel-color", pixel_obj_idx, 1 );



            }
            return 1;
        }
        return -1;
    }



    virtual int hollow_rectangle(StateDescriptor *sd, State *s, int number){
        int id = _lhs->getID();
        string vtype = _lhs->getVType();
        vector<int> params = _lhs->getParameterIDs();
        int node_value = s->getPointer(sd, params[0]);
        int node_type_id = s->getNodeID(node_value);
        auto graph = s->getGraph();
        auto graphNode = graph->getNode(node_type_id);
        vector<int> pixels=graphNode->getPixelsIDs();
        int given_color;

        if (sd->checkpointer(params[1])){
            int color_pointer = s->getPointer(sd, params[1] );
            given_color=s->getColorID(color_pointer);

        }
        else{
            given_color =params[1];
        }

        if (sd->getObjectName(graphNode->getShape())=="square" or sd->getObjectName(graphNode->getShape())=="rectangle" )
        {

        }
        else{
            return -1;
        }






        vector<int> edge_pixels;
        vector<int> inside_pixels;


        int x_min=graphNode->get_x_edge_min();
        int x_max=graphNode->get_x_edge_max();
        int y_min=graphNode->get_y_edge_min();
        int y_max=graphNode->get_y_edge_max();


        for (auto pixel:pixels){
            auto candidate_pixel=graph->getPixel(pixel);
            int x_location=candidate_pixel->getX();
            int y_location=candidate_pixel->getY();
            if (x_location==x_min or x_location==x_max or y_location==y_min or y_location==y_max){
                edge_pixels.push_back(pixel);
            }
            else{
                inside_pixels.push_back(pixel);
            }

        }


        if (not inside_pixels.empty() and given_color!=graph->getBackGroundColor() ){



            for (auto pixel :inside_pixels){

                auto inside_pixel =graph->getPixel(pixel);
                auto pixelID=inside_pixel->getID();



                vector< int > pixel_obj_idx ( params.size() );
                pixel_obj_idx[0]=s->getObject(sd,pixelID);
                pixel_obj_idx[1]=s->getObject(sd,inside_pixel->getColor());

                s->addRegister( sd, "pixel-color", pixel_obj_idx, 0 );

                inside_pixel->setColor(given_color);

                pixel_obj_idx[1]=s->getObject(sd,inside_pixel->getColor());

                s->addRegister( sd, "pixel-color", pixel_obj_idx, 1 );


// should have set new_node contain pixel as 1


                pixel_obj_idx[0]=s->getObject(sd,graphNode->getNodeID());
                pixel_obj_idx[1]=s->getObject(sd,pixelID);

                s->addRegister( sd, "contain-pixel", pixel_obj_idx, 0 );





            }

            for (auto pixel :edge_pixels){

                auto edge_pixel =graph->getPixel(pixel);
                auto pixelID=edge_pixel->getID();

                graphNode->setPixel(pixelID,edge_pixel);



                vector< int > pixel_obj_idx ( params.size() );


                pixel_obj_idx[0]=s->getObject(sd,graphNode->getNodeID());
                pixel_obj_idx[1]=s->getObject(sd,pixelID);


                s->addRegister( sd, "contain-pixel", pixel_obj_idx, 1 );



            }
            return 1;
        }

        if (not inside_pixels.empty() and given_color==graph->getBackGroundColor() ){

            graphNode->clearPixelChangSize();


            for (auto pixel :inside_pixels){

                auto inside_pixel =graph->getPixel(pixel);
                auto pixelID=inside_pixel->getID();


                vector< int > pixel_obj_idx ( params.size() );
                pixel_obj_idx[0]=s->getObject(sd,pixelID);
                pixel_obj_idx[1]=s->getObject(sd,inside_pixel->getColor());

                s->addRegister( sd, "pixel-color", pixel_obj_idx, 0 );

                inside_pixel->setColor(given_color);

                pixel_obj_idx[1]=s->getObject(sd,inside_pixel->getColor());

                s->addRegister( sd, "pixel-color", pixel_obj_idx, 1 );



                pixel_obj_idx[0]=s->getObject(sd,graphNode->getNodeID());
                pixel_obj_idx[1]=s->getObject(sd,pixelID);

                s->addRegister( sd, "contain-pixel", pixel_obj_idx, 0 );

            }

            for (auto pixel :edge_pixels){

                auto edge_pixel =graph->getPixel(pixel);
                auto pixelID=edge_pixel->getID();

                graphNode->setPixel(pixelID,edge_pixel);



                vector< int > pixel_obj_idx ( params.size() );


                pixel_obj_idx[0]=s->getObject(sd,graphNode->getNodeID());
                pixel_obj_idx[1]=s->getObject(sd,pixelID);


                s->addRegister( sd, "contain-pixel", pixel_obj_idx, 1 );



            }
           return 1;
        }


        return -1;

    }


    virtual int insert_input_node(StateDescriptor *sd, State *s, int number){


        int id = _lhs->getID();
        string vtype = _lhs->getVType();
        vector<int> params = _lhs->getParameterIDs();
        int node_value = s->getPointer(sd, params[0]);
        int node_type_id = s->getNodeID(node_value);
        auto graph = s->getGraph();
        auto graphNode = graph->getNode(node_type_id);

        int node_value2= s->getPointer(sd, params[1] );
        int node_type_id2=s->getNodeID(node_value2);
        auto graphNode2 =graph->getNode(node_type_id2);

        if (graphNode==graphNode2){
            return -1;
        }




        auto x_location=graphNode->get_x_locations();
        auto y_location=graphNode->get_y_locations();

        if (x_location.empty() or y_location.empty())
            return -1;


        int center_x= (int) round((accumulate(x_location.begin(),x_location.end(),0)+ (int) round(x_location.size()/2))/x_location.size());
        int center_y= (int) round((accumulate(y_location.begin(),y_location.end(),0)+ (int) round(y_location.size()/2))/y_location.size());


        auto graph_x_location=graphNode2->get_x_locations();
        auto graph_y_location=graphNode2->get_y_locations();


        if (graph_x_location.empty() or graph_y_location.empty())
            return -1;


        int graph_node_center_x= (int) round((accumulate(graph_x_location.begin(),graph_x_location.end(),0) + (int) round(graph_x_location.size()/2))/graph_x_location.size());
        int graph_node_center_y= (int) round((accumulate(graph_y_location.begin(),graph_y_location.end(),0)+(int) round(graph_y_location.size()/2))/graph_y_location.size());


        int target_point_x;
        int target_point_y;

        target_point_x=center_x;
        target_point_y=center_y;




        map<Pixel*,int> pixel_color;
        auto pixels=graphNode2->getPixelsIDs();

        for (auto pixelID: pixels){
            auto pixel=graph->getPixel(pixelID);
            int delta_x=pixel->getX()-graph_node_center_x;
            int delta_y=pixel->getY()-graph_node_center_y;
            vector<int> location={target_point_x+delta_x,target_point_y+delta_y};
            if (graph->inbound(location[0],location[1]))
            {
                auto new_pixel =graph->getPixel(graph->getPixel(location));
                pixel_color[new_pixel]=pixel->getColor();
            }



        }


        auto original_pixels=graphNode->getPixelsIDs();

        for (auto pixelID: original_pixels){

            vector< int > pixel_obj_idx ( 2);
            auto pixel=graph->getPixel(pixelID);
            pixel_obj_idx[0]=s->getObject(sd,graphNode->getNodeID());
            pixel_obj_idx[1]=s->getObject(sd,pixelID);

            s->addRegister( sd, "contain-pixel", pixel_obj_idx, 0);

            pixel_obj_idx[0]=s->getObject(sd,pixelID);
            pixel_obj_idx[1]=s->getObject(sd,pixel->getColor());

            s->addRegister( sd, "pixel-color", pixel_obj_idx, 0 );

            pixel->setColor(graph->getBackGroundColor());

            pixel_obj_idx[1]=s->getObject(sd,pixel->getColor());

            s->addRegister( sd, "pixel-color", pixel_obj_idx, 1 );

        }

        graphNode->clearPixelKeepSize();

        for (auto it:pixel_color){

            auto pixel =it.first;
            auto pixelID=pixel->getID();


            vector< int > pixel_obj_idx ( 2 );
            pixel_obj_idx[0]=s->getObject(sd,pixelID);
            pixel_obj_idx[1]=s->getObject(sd,pixel->getColor());

            s->addRegister( sd, "pixel-color", pixel_obj_idx, 0 );

            pixel->setColor(it.second);


            pixel_obj_idx[1]=s->getObject(sd,pixel->getColor());

            s->addRegister( sd, "pixel-color", pixel_obj_idx, 1 );

            graphNode->setPixel(pixelID,pixel);


            pixel_obj_idx[0]=s->getObject(sd,graphNode->getNodeID());
            pixel_obj_idx[1]=s->getObject(sd,pixelID);

            s->addRegister( sd, "contain-pixel", pixel_obj_idx, 1 );


        }
        vector< int > node_obj_idx ( 2 );
        node_obj_idx[0]=node_value;
        node_obj_idx[1]=s->getObject(sd,graphNode->getColor());;
        s->addRegister( sd, "node-color", node_obj_idx, 0 );
        node_obj_idx[1]=s->getObject(sd,graphNode2->getColor());
        s->addRegister( sd, "node-color", node_obj_idx, 1 );

        if (graphNode->getSize()!=0 and graphNode2->getSize()!=0){
            node_obj_idx[1]=s->getObject(sd,graphNode->getSize());;
            s->addRegister( sd, "node-size", node_obj_idx, 0 );
            node_obj_idx[1]=s->getObject(sd,graphNode2->getSize());
            s->addRegister( sd, "node-size", node_obj_idx, 1 );
        }

        node_obj_idx[1]=s->getObject(sd,graphNode->getShape());;
        s->addRegister( sd, "node-shape", node_obj_idx, 0 );
        node_obj_idx[1]=s->getObject(sd,graphNode2->getShape());
        s->addRegister( sd, "node-shape", node_obj_idx, 1 );


        graphNode->setColor(graphNode2->getColor());

        graphNode->setSize(graphNode2->getSize());

        graphNode->setShape(graphNode2->getShape());

        return 1;


    }


    virtual int insert_output_node(StateDescriptor *sd, State *s, int number){


        int id = _lhs->getID();
        string vtype = _lhs->getVType();
        vector<int> params = _lhs->getParameterIDs();
        int node_value = s->getPointer(sd, params[0]);
        int node_type_id = s->getNodeID(node_value);
        auto graph = s->getGraph();
        auto graphNode = graph->getNode(node_type_id);

        int node_value2= s->getPointer(sd, params[1] );
        int node_type_id2=s->getOutputNodeID(node_value2);
        auto graphNode2 =graph->getOutputNode(node_type_id2);


        auto x_location=graphNode->get_x_locations();
        auto y_location=graphNode->get_y_locations();

        int center_x= (int) round((accumulate(x_location.begin(),x_location.end(),0)+ (int) round(x_location.size()/2))/x_location.size());
        int center_y= (int) round((accumulate(y_location.begin(),y_location.end(),0)+ (int) round(y_location.size()/2))/y_location.size());


        auto graph_x_location=graphNode2->get_x_locations();
        auto graph_y_location=graphNode2->get_y_locations();

        int graph_node_center_x= (int) round((accumulate(graph_x_location.begin(),graph_x_location.end(),0) + (int) round(graph_x_location.size()/2))/graph_x_location.size());
        int graph_node_center_y= (int) round((accumulate(graph_y_location.begin(),graph_y_location.end(),0)+(int) round(graph_y_location.size()/2))/graph_y_location.size());

        int target_point_x;
        int target_point_y;



        target_point_x=center_x;
        target_point_y=center_y;

        map<Pixel*,int> pixel_color;
        auto pixels=graphNode2->getPixelsIDs();

        for (auto pixelID: pixels){
            auto pixel=graph->getPixel(pixelID);
            int delta_x=pixel->getX()-graph_node_center_x;
            int delta_y=pixel->getY()-graph_node_center_y;
            vector<int> location={target_point_x+delta_x,target_point_y+delta_y};
            if (graph->inbound(location[0],location[1]))
                 pixel_color[graph->getPixel(graph->getPixel(location))]=graph->getGoalPixelColor(pixelID);
        }


        auto original_pixels=graphNode->getPixelsIDs();

        for (auto pixelID: original_pixels){

            vector< int > pixel_obj_idx ( 2);
            auto pixel=graph->getPixel(pixelID);
            pixel_obj_idx[0]=s->getObject(sd,graphNode->getNodeID());
            pixel_obj_idx[1]=s->getObject(sd,pixelID);

            s->addRegister( sd, "contain-pixel", pixel_obj_idx, 0);

            pixel_obj_idx[0]=s->getObject(sd,pixelID);

            pixel_obj_idx[1]=s->getObject(sd,pixel->getColor());

            s->addRegister( sd, "pixel-color", pixel_obj_idx, 0 );

            pixel->setColor(graph->getBackGroundColor());

            pixel_obj_idx[1]=s->getObject(sd,pixel->getColor());

            s->addRegister( sd, "pixel-color", pixel_obj_idx, 1 );



        }

        graphNode->clearPixelKeepSize();

        for (auto it:pixel_color){

            auto pixel =it.first;
            auto pixelID=pixel->getID();


            vector< int > pixel_obj_idx ( 2 );
            pixel_obj_idx[0]=s->getObject(sd,pixelID);
            pixel_obj_idx[1]=s->getObject(sd,pixel->getColor());

            s->addRegister( sd, "pixel-color", pixel_obj_idx, 0 );

            pixel->setColor(it.second);

            pixel_obj_idx[1]=s->getObject(sd,pixel->getColor());

            s->addRegister( sd, "pixel-color", pixel_obj_idx, 1 );

            graphNode->setPixel(pixelID,pixel);


            pixel_obj_idx[0]=s->getObject(sd,graphNode->getNodeID());
            pixel_obj_idx[1]=s->getObject(sd,pixelID);

            s->addRegister( sd, "contain-pixel", pixel_obj_idx, 1 );


        }
        vector< int > node_obj_idx ( 2 );
        node_obj_idx[0]=node_value;
        node_obj_idx[1]=s->getObject(sd,graphNode->getColor());;
        s->addRegister( sd, "node-color", node_obj_idx, 0 );
        node_obj_idx[1]=s->getObject(sd,graphNode2->getColor());
        s->addRegister( sd, "node-color", node_obj_idx, 1 );

        if (graphNode->getSize()!=0 and graphNode2->getSize()!=0){
            node_obj_idx[1]=s->getObject(sd,graphNode->getSize());;
            s->addRegister( sd, "node-size", node_obj_idx, 0 );
            node_obj_idx[1]=s->getObject(sd,graphNode2->getSize());
            s->addRegister( sd, "node-size", node_obj_idx, 1 );
        }

        node_obj_idx[1]=s->getObject(sd,graphNode->getShape());;
        s->addRegister( sd, "node-shape", node_obj_idx, 0 );
        node_obj_idx[1]=s->getObject(sd,graphNode2->getShape());
        s->addRegister( sd, "node-shape", node_obj_idx, 1 );


        graphNode->setColor(graphNode2->getColor());

        graphNode->setSize(graphNode2->getSize());

        graphNode->setShape(graphNode2->getShape());

        return 1;

    }


    virtual bool check(StateDescriptor *sd, State *s){

        int id = _lhs->getID();
        auto pred_type = sd->getPredicateName( id );


        if (pred_type=="color-most"){
            int color;
            auto graph = s->getGraph();
            vector<int> params = _lhs->getParameterIDs();
            if (sd->checkpointer(params[0])){
                int second_pointer = s->getPointer(sd, params[0] );
                color=s->getColorID(second_pointer);

            }
            else{
                color =params[0];
            }

            return graph->getMostColor()==color;
        }

        if (pred_type=="color-least"){
            int color;
            auto graph = s->getGraph();
            vector<int> params = _lhs->getParameterIDs();
            if (sd->checkpointer(params[0])){
                int second_pointer = s->getPointer(sd, params[0] );
                color=s->getColorID(second_pointer);

            }
            else{
                color =params[0];
            }

            return graph->getLeastColor()==color;
        }



        vector<int> params = _lhs->getParameterIDs();
        int node_value = s->getPointer(sd, params[0]);
        int node_type_id = s->getNodeID(node_value);
        auto graph = s->getGraph();
        auto graphNode = graph->getNode(node_type_id);

        int second;


        if (pred_type=="node-size")
        {

            if (sd->checkpointer(params[1])){
                int second_pointer = s->getPointer(sd, params[1] );
                second=s->getSizeID(second_pointer);

            }
            else{
                second =params[1];
            }

            return graphNode->getSize()==second;
        }
        if (pred_type=="node-color"){
            if (sd->checkpointer(params[1])){
                int second_pointer = s->getPointer(sd, params[1] );
                second=s->getColorID(second_pointer);

            }
            else{
                second =params[1];
            }

            return graphNode->getColor()==second;
        }

        if (pred_type=="node-shape"){

            if (sd->checkpointer(params[1])){
                int second_pointer = s->getPointer(sd, params[1] );
                second=s->getShapeID(second_pointer);

            }
            else{
                second =params[1];
            }

            return graphNode->getShape()==second;
        }

        if (pred_type=="vec-neighbor"){
            if (sd->checkpointer(params[1])){
                int second_pointer = s->getPointer(sd, params[1] );
                second=s->getNodeID(second_pointer);

            }
            else{
                second =params[1];
            }

            return graphNode->check_spatial_vertical_relationship(second);
        }

        if (pred_type=="hor-neighbor"){

            if (sd->checkpointer(params[1])){
                int second_pointer = s->getPointer(sd, params[1] );
                second=s->getNodeID(second_pointer);

            }
            else{
                second =params[1];
            }
            return graphNode->check_spatial_horizontal_relationship(second);
        }

        if (pred_type=="contain-node"){

            if (sd->checkpointer(params[1])){
                int second_pointer = s->getPointer(sd, params[1] );
                second=s->getNodeID(second_pointer);

            }
            else{
                second =params[1];
            }
            return graphNode->check_contain(second);
        }

        if (pred_type=="incomplete-contain-node"){

            if (sd->checkpointer(params[1])){
                int second_pointer = s->getPointer(sd, params[1] );
                second=s->getNodeID(second_pointer);

            }
            else{
                second =params[1];
            }
            return graphNode->check_incomplete_contain(second);
        }

        if (pred_type=="same-node"){

            if (sd->checkpointer(params[1])){
                int second_pointer = s->getPointer(sd, params[1] );
                second=s->getNodeID(second_pointer);

            }
            else{
                second =params[1];
            }
            return graphNode->check_same(second);
        }

        if (pred_type=="same-color"){

            if (sd->checkpointer(params[1])){
                int second_pointer = s->getPointer(sd, params[1] );
                second=s->getNodeID(second_pointer);

            }
            else{
                second =params[1];
            }
            return graphNode->check_same_color(second);
        }


        if (pred_type=="node-similar"){

            if (sd->checkpointer(params[1])){
                int second_pointer = s->getPointer(sd, params[1] );
                second=s->getColorID(second_pointer);

            }
            else{
                second =params[1];
            }
            return graphNode->checkSimilar(second);
        }



        if (pred_type=="node-diagonal"){

            if (sd->checkpointer(params[1])){
                int second_pointer = s->getPointer(sd, params[1] );
                second=s->getNodeID(second_pointer);

            }
            else{
                second =params[1];
            }
            return graphNode->checkDiagonal(second);
        }


        int spatial_id;
        if (pred_type=="node-relative-position"){

            if (sd->checkpointer(params[1])){
                int second_pointer = s->getPointer(sd, params[1] );
                second=s->getNodeID(second_pointer);

            }
            else{
                second =params[1];
            }

            if (sd->checkpointer(params[2])){
                int second_pointer = s->getPointer(sd, params[2] );
                 spatial_id=s->getDirectionID(second_pointer);

            }
            else{
                 spatial_id =params[2];
            }

            auto spatial_name=sd->getObjectName(spatial_id);

            if (spatial_name=="left"){
                return graphNode->checkLeft(second);
            }
            if (spatial_name=="right"){
                return graphNode->checkRight(second);
            }

            if (spatial_name=="up"){
                return graphNode->checkUp(second);
            }

            if (spatial_name=="down"){
                return graphNode->checkDown(second);
            }

            if (spatial_name=="left-up"){
                return graphNode->checkLeftUp(second);
            }
            if (spatial_name=="left-down"){
                return graphNode->checkLeftDown(second);
            }

            if (spatial_name=="right-up"){
                return graphNode->checkRightUp(second);
            }

            if (spatial_name=="right-down"){
                return graphNode->checkRightDown(second);
            }

        }


        if (pred_type=="size-max"){
            return graphNode->check_size_max();
        }

        if (pred_type=="size-min"){
            return graphNode->check_size_min();
        }


        if (pred_type=="color-max"){
            return graphNode->check_color_max();
        }

        if (pred_type=="color-min"){
            return graphNode->check_color_min();

        }

        if (pred_type=="odd-node"){
            return graphNode->check_odd();
        }

        if (pred_type=="even-node"){
            return graphNode->check_even();
        }

        if (pred_type=="left-diagonal-node"){
            return graphNode->check_left_diagonal_node();
        }
        if (pred_type=="right-diagonal-node"){
            return graphNode->check_right_diagonal_node();
        }

        if (pred_type=="right-border-node"){
            return graphNode->check_right_border_node();
        }
        if (pred_type=="left-border-node"){
            return graphNode->check_left_border_node();
        }
        if (pred_type=="up-border-node"){
            return graphNode->check_up_border_node();
        }
        if (pred_type=="down-border-node"){
            return graphNode->check_down_border_node();
        }

        if (pred_type=="vertical-middle-node"){
            return graphNode->check_vertical_middle_node();
        }

        if (pred_type=="horizontal-middle-node"){
            return graphNode->check_horizontal_middle_node();
        }

    }





    int add_new_node_simple (StateDescriptor *sd, State *s) const{
//        int next_object_id = int(sd->getTypeIDSize());
//        string obj_type="node";
//        string obj_name="node"+to_string(next_object_id);

//        sd->addObject(obj_name,obj_type);
        sd->increaseNewTypeID();

        int next_object_id=sd->getNewTypeID();

        string obj_type="node";
        string obj_name="node"+to_string(next_object_id);

//        s->getGraph()->addNode(sd->getTypeID(obj_name),new GraphNode(sd->getTypeID(obj_name)));
        auto new_node=new GraphNode(next_object_id);
        s->getGraph()->addNode(next_object_id,new_node);


        return next_object_id;

    }



    int add_new_node (StateDescriptor *sd, State *s) const{
        int next_object_id = int(sd->getTypeIDSize());
        string obj_type="node";
        string obj_name="node"+to_string(next_object_id);
        sd->addObject(obj_name,obj_type);

        auto num_of_objects = sd->getStateVarBound( obj_type, s->getInstanceID() );
        auto obj_to_type=s->getObjType();
        assert( obj_to_type.find( obj_name ) == obj_to_type.end() );
        s->addObjType(obj_name,obj_type);
        s->addObjAddress(obj_name,num_of_objects);

        s->add_pointer_to_obj(num_of_objects,sd->getTypeID(obj_name));
        s->getGraph()->addNode(sd->getTypeID(obj_name),new GraphNode(sd->getTypeID(obj_name)));


        // we need a structure to save <node,0>=node1 where node is obj_type and 0 is num_of_objects
        sd->setStateVarBound( obj_type, s->getInstanceID() , num_of_objects+1 );

        return next_object_id;

    }



    void update_node_relation(StateDescriptor *sd, State *s, AbstractGraph* graph, GraphNode* changedNode){


        for (GraphNode* node: graph->getNode()){
            if (node==changedNode)
                continue;
            auto nodeID=node->getNodeID();
            auto changedNodeID =changedNode->getNodeID();
            bool node_changedNode=graph->containNode(node,changedNode);
            bool changedNode_node=graph->containNode(changedNode,node);
            if (node_changedNode){
                if (node->isContain(changedNodeID))
                    continue;
                else{
                    node->setContain(changedNodeID);
                    vector< int > pixel_obj_idx ( 2);

//                    pixel_obj_idx[0]=nodeID;
//                    pixel_obj_idx[1]=changedNodeID;
//                    s->addRegister( sd, "contain", pixel_obj_idx, 1 );

//
                    pixel_obj_idx[0]=s->getObject(sd,nodeID);
                    pixel_obj_idx[1]=s->getObject(sd,changedNodeID);
                    s->addRegister( sd, "contain", pixel_obj_idx, 1 );
                }

            }
            if (not node_changedNode){
                if (node->isContain(changedNodeID)){
                    node->removeContain(changedNodeID);

//                    vector< int > pixel_obj_idx ( 2);
//                    pixel_obj_idx[0]=nodeID;
//                    pixel_obj_idx[1]=changedNodeID;
//                    s->addRegister( sd, "contain", pixel_obj_idx, 0 );


                    vector< int > pixel_obj_idx ( 2);
                    pixel_obj_idx[0]=s->getObject(sd,nodeID);
                    pixel_obj_idx[1]=s->getObject(sd,changedNodeID);
                    s->addRegister( sd, "contain", pixel_obj_idx, 0 );

                }
            }

            if (not changedNode_node){
                if (changedNode->isContain(nodeID)){
                    changedNode->removeContain(nodeID);
                    vector< int > pixel_obj_idx ( 2);

//                    pixel_obj_idx[0]=changedNodeID;
//                    pixel_obj_idx[1]=nodeID;
//                    s->addRegister( sd, "contain", pixel_obj_idx, 0 );

                    pixel_obj_idx[0]=s->getObject(sd,changedNodeID);
                    pixel_obj_idx[1]=s->getObject(sd,nodeID);
                    s->addRegister( sd, "contain", pixel_obj_idx, 0 );

                }
            }


            if (graph->containNode(changedNode,node)){
                if (changedNode->isContain(nodeID))
                    continue;
                else{
                    changedNode->setContain(nodeID);
//                    vector< int > pixel_obj_idx ( 2);
//                    pixel_obj_idx[0]=changedNodeID;
//                    pixel_obj_idx[1]=nodeID;
//                    s->addRegister( sd, "contain", pixel_obj_idx, 1 );


                    vector< int > pixel_obj_idx ( 2);
                    pixel_obj_idx[0]=s->getObject(sd,changedNodeID);
                    pixel_obj_idx[1]=s->getObject(sd,nodeID);
                    s->addRegister( sd, "contain", pixel_obj_idx, 1 );

                }
            }



            if ((node->get_y_edge_max() >= changedNode->get_y_edge_min() and node->get_y_edge_max() <= changedNode->get_y_edge_max()) or (
                    changedNode->get_y_edge_max() >= node->get_y_edge_min() and changedNode->get_y_edge_max() <= node->get_y_edge_max()))
            {
                if (not changedNode->isVer(nodeID)){
                    changedNode->setVer(nodeID);
                    node->setVer(changedNodeID);

//                    vector< int > pixel_obj_idx ( 2);
//                    pixel_obj_idx[0]=nodeID;
//                    pixel_obj_idx[1]=changedNodeID;
//                    s->addRegister( sd, "vec-neighbor", pixel_obj_idx, 1 );
//                    pixel_obj_idx[0]=changedNodeID;
//                    pixel_obj_idx[1]=nodeID;
//                    s->addRegister( sd, "vec-neighbor", pixel_obj_idx, 1 );


                    vector< int > pixel_obj_idx ( 2);
                    pixel_obj_idx[0]=s->getObject(sd,nodeID);
                    pixel_obj_idx[1]=s->getObject(sd,changedNodeID);
                    s->addRegister( sd, "vec-neighbor", pixel_obj_idx, 1 );
                    pixel_obj_idx[0]=s->getObject(sd,changedNodeID);
                    pixel_obj_idx[1]=s->getObject(sd,nodeID);
                    s->addRegister( sd, "vec-neighbor", pixel_obj_idx, 1 );
                }
            }
            else{
                if (changedNode->isVer(nodeID))
                {
                    changedNode->removeVertical(nodeID);
                    node->removeVertical(changedNodeID);

//                    vector< int > pixel_obj_idx ( 2);
//                    pixel_obj_idx[0]=nodeID;
//                    pixel_obj_idx[1]=changedNodeID;
//                    s->addRegister( sd, "vec-neighbor", pixel_obj_idx, 0 );
//                    pixel_obj_idx[0]=changedNodeID;
//                    pixel_obj_idx[1]=nodeID;
//                    s->addRegister( sd, "vec-neighbor", pixel_obj_idx, 0 );


                    vector< int > pixel_obj_idx ( 2);
                    pixel_obj_idx[0]=s->getObject(sd,nodeID);
                    pixel_obj_idx[1]=s->getObject(sd,changedNodeID);
                    s->addRegister( sd, "vec-neighbor", pixel_obj_idx, 0 );
                    pixel_obj_idx[0]=s->getObject(sd,changedNodeID);
                    pixel_obj_idx[1]=s->getObject(sd,nodeID);
                    s->addRegister( sd, "vec-neighbor", pixel_obj_idx, 0 );

                }
            }

            if ((node->get_x_edge_max() >= changedNode->get_x_edge_min() and node->get_x_edge_max() <= changedNode->get_x_edge_max()) or
            (changedNode->get_x_edge_max() >= node->get_x_edge_min() and changedNode->get_x_edge_max() <= node->get_x_edge_max())){
                if (not changedNode->isHor(nodeID)){
                    changedNode->setHor(nodeID);
                    node->setHor(changedNodeID);

//                    vector< int > pixel_obj_idx ( 2);
//                    pixel_obj_idx[0]=nodeID;
//                    pixel_obj_idx[1]=changedNodeID;
//                    s->addRegister( sd, "hor-neighbor", pixel_obj_idx, 1 );
//                    pixel_obj_idx[0]=changedNodeID;
//                    pixel_obj_idx[1]=nodeID;
//                    s->addRegister( sd, "hor-neighbor", pixel_obj_idx, 1 );


                    vector< int > pixel_obj_idx ( 2);
                    pixel_obj_idx[0]=s->getObject(sd,nodeID);
                    pixel_obj_idx[1]=s->getObject(sd,changedNodeID);
                    s->addRegister( sd, "hor-neighbor", pixel_obj_idx, 1 );
                    pixel_obj_idx[0]=s->getObject(sd,changedNodeID);
                    pixel_obj_idx[1]=s->getObject(sd,nodeID);
                    s->addRegister( sd, "hor-neighbor", pixel_obj_idx, 1 );

                }
            }
            else{
                if (changedNode->isHor(nodeID))
                {
                    changedNode->removeHorizontal(nodeID);
                    node->removeHorizontal(changedNodeID);


//                    vector< int > pixel_obj_idx ( 2);
//                    pixel_obj_idx[0]=nodeID;
//                    pixel_obj_idx[1]=changedNodeID;
//                    s->addRegister( sd, "hor-neighbor", pixel_obj_idx, 1 );
//                    pixel_obj_idx[0]=changedNodeID;
//                    pixel_obj_idx[1]=nodeID;
//                    s->addRegister( sd, "hor-neighbor", pixel_obj_idx, 1 );

//
                    vector< int > pixel_obj_idx ( 2);
                    pixel_obj_idx[0]=s->getObject(sd,nodeID);
                    pixel_obj_idx[1]=s->getObject(sd,changedNodeID);
                    s->addRegister( sd, "hor-neighbor", pixel_obj_idx, 1 );
                    pixel_obj_idx[0]=s->getObject(sd,changedNodeID);
                    pixel_obj_idx[1]=s->getObject(sd,nodeID);
                    s->addRegister( sd, "hor-neighbor", pixel_obj_idx, 1 );

                }
            }


        }

    }



    virtual void setLeftVarNum( StateDescriptor *sd, State *s, int number ) const{
        int id = _lhs->getID();
        string vtype = _lhs->getVType();

        // Content of pointers cannot be modified, only pointers and grounded predicates.
        assert( (vtype == "pointer" and not _lhs->accessMemory()) or vtype == "predicate" );

        if( vtype == "pointer" ){
            s->setPointer( sd, id, number );
//            std::cout<<id<<std::endl;
        }
        else if( vtype == "predicate" ){
            auto pred_type = sd->getPredicateName( id );
            vector< int > params = _lhs->getParameterIDs();
            vector< int > param_obj_idx ( params.size() );
            for( unsigned i = 0; i < params.size(); i++ ){
                param_obj_idx[i] = s->getPointer(sd, params[i] ); // Assign the pointer index
            }
            s->addRegister( sd, pred_type, param_obj_idx, number );
        }
    }

	
	// Value of the variable in current state or constant number.
    virtual int getVarNum( StateDescriptor *sd, const State* s, Variable *v ) const{
		int id = v->getID();
		string vtype = v->getVType();

		assert( vtype == "pointer" or vtype == "predicate" or vtype == "constant" );

		if( vtype == "pointer" ){
		    // ToDo alternative implementation of access to objects,
		    //  which would require the Instance as parameter
			//if( v->accessMemory() ){
			//	return s->getRegister( s->getPointer( id ) );
			//}
			return s->getPointer( sd, id );
		}	

		else if( vtype == "predicate" ){
            auto pred_type = sd->getPredicateName( id );
            vector< int > params = v->getParameterIDs();
            vector< int > param_obj_idx ( params.size() );
            for( unsigned i = 0; i < params.size(); i++ ){
                if( params[i] < 0 ){ // the param is a constant object
                    param_obj_idx[i] = -(params[i]+1); // constants are 0-indexed
                }
                else{

                    if (sd->checkpointer(params[i])){
                        param_obj_idx[i] = s->getPointer(sd, params[i] );
                    }
                    else{
                        param_obj_idx[i] =s->getObject(sd,params[i]);
                    }
                } // the param is a pointer
            }



            return s->getRegister( sd, pred_type, param_obj_idx );
		}

		// Otherwise is a constant
		return id;
    }


    // Return the left-hand side value
    virtual int getLHS( StateDescriptor *sd, const State* s ) const{
		return getVarNum( sd, s, _lhs );
	}
        
	// Return the right-hand side value     
	virtual int getRHS( StateDescriptor *sd, const State* s ) const{
		return getVarNum( sd, s, _rhs );
	}

	virtual string getLeftName( StateDescriptor *sd ) const{
        int id = _lhs->getID();
        string vtype = _lhs->getVType();

        if( vtype != "predicate" ) return "";

        return sd->getPredicateName( id );
	}
    virtual string getType( Variable *v ) const{
        string vtype = v->getVType();
        return vtype;

    }
	virtual vector<int> getLeftObjects( ) const{
        //int id = _lhs->getID();
        string vtype = _lhs->getVType();

        assert( vtype == "predicate"); // The var type must be a predicate

        vector< int > params = _lhs->getParameterIDs();
        vector< int > param_obj_idx ( params.size() );
        for( unsigned i = 0; i < params.size(); i++ ){
            assert( params[i] < 0 ); // the param must be a constant object
            param_obj_idx[i] = -(params[i]+1); // constants are 0-indexed
        }
        return param_obj_idx;
//        return params;
	}
    virtual string getVarType( ) const{
        return getType(_lhs);
    }

    virtual vector<string> getLeftPointerNames( StateDescriptor *sd ) const{
        //int id = _lhs->getID();
        string vtype = _lhs->getVType();

        assert( vtype == "predicate"); // The var type must be a predicate

        vector< int > params = _lhs->getParameterIDs();
        vector< string > pointer_names ( params.size() );
        for( unsigned i = 0; i < params.size(); i++ ){
            assert( params[i] >= 0 ); // the params must be pointers
            pointer_names[ i ] = sd->getPointerName( params[i] );
        }
        return pointer_names;
    }

protected:
	string _name;
	Variable *_lhs;
	Variable *_rhs;
};

#endif
