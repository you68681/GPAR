import os.path

from collections import defaultdict

predicates_relation = {"background-color":["color"], "node-color": ["node", "color"],"node-size": ["node", "size"],
                       "node-shape": ["node", "shape"], "contain-pixel": ["node", "pixel"],"contain-pixel-output": ["output-node", "pixel"],
                        "contain-node": ["node", "node"],"incomplete-contain-node":["node","node"], "pixel-color": ["pixel", "color"],
                       "node-background-color":["node","color"],"node-spatial-relation":["node","node","move-direction"],"node-diagonal":["node","node"],
                       "size-max":["node"],"size-min":["node"],"color-most":["color"],"color-least": ["color"],"color-max":["node"],"color-min":["node"],"odd-node":["node"],
                       "even-node":["node"],"node-similar":["node","color"],"left-diagonal-node":["node"],"right-diagonal-node":["node"],"up-border-node":["node"],
                       "down-border-node":["node"],"left-border-node":["node"],"right-border-node":["node"],"vertical-middle-node":["node"],"horizontal-middle-node":["node"]}

predicates_relation_same = {"background-color":["color"], "node-color": ["node", "color"], "node-size": ["node", "size"],"contain-pixel": ["node", "pixel"],
                        "contain-node": ["node", "node"],"incomplete-contain-node":["node","node"], "pixel-color": ["pixel", "color"],
                       "node-background-color":["node","color"],"node-spatial-relation":["node","node","move-direction"],"color-min":["node"],"same-node":["node","node"],"same-color":["node","node"]}

predicates_node_relation = {"background-color":["color"], "node-color": ["node", "color"], "node-size": ["node", "size"],"contain-pixel": ["node", "pixel"], "pixel-color": ["pixel", "color"],
                       "node-background-color":["node","color"],"node-spatial-relation":["node","node","move-direction"],"node-diagonal":["node","node"],"node-relative-position":["node","node","move-direction"]}


types = ["node","size", "color", "shape", "pixel","move-direction","rotation","flip-direction"]


predicates = {"init": { "node-color": [], "node-size": [],"node-shape": [],
                       "contain-pixel": [], "contain-node": [],  "pixel-color": [],"node-background-color":[],"size-max":[],"size-min":[],"color-max":[],"color-min":[],
                       "odd-node":[],"even-node":[],"incomplete-contain-node": [],"node-spatial-relation":[],"node-diagonal":[],"background-color":[]},
              "goal": { "contain-pixel-output": [],  "pixel-color": []}}


predicates_mul = {"init": { "node-color": [],"node-size": [],"node-shape": [],
                       "contain-pixel": [], "contain-node": [],  "pixel-color": [],"node-background-color":[],"size-max":[],"size-min":[],
                           "odd-node":[],"even-node":[],"incomplete-contain-node": [],"node-spatial-relation":[],"node-diagonal":[],"background-color":[]},
              "goal": {"contain-pixel-output": [],  "pixel-color": []}}

predicates_same = {"init": { "node-color": [],"node-size": [],
                       "contain-pixel": [], "contain-node": [],  "pixel-color": [],"node-background-color":[],"color-min":[],
                       "same-node":[],"same-color":[],"incomplete-contain-node": [],"node-spatial-relation":[],"background-color":[]},
              "goal": {"contain-pixel-output": [],  "pixel-color": []}}


predicates_pixel = {"init": { "pixel-color": [],"node-color": [],"node-similar": [],"contain-pixel": [],"background-color":[],"color-most": [],"color-least": [],"left-diagonal-node":[],"right-diagonal-node":[],"up-border-node":[],
                       "down-border-node":[],"left-border-node":[],"right-border-node":[],"vertical-middle-node":[],"horizontal-middle-node":[]},
              "goal": {"contain-pixel-output": [],  "pixel-color": []}}

predicates_none = {"init": {"node-size": [], "contain-pixel": [],  "pixel-color": []},
              "goal": {"contain-pixel-output": [],  "pixel-color": []}}


predicates_node= {"init":{"background-color":[], "node-color": [], "node-size": [],"contain-pixel": [], "pixel-color": [],
                     "node-background-color":[],"node-spatial-relation":[],"node-diagonal":[]},
                      "goal": {"contain-pixel-output": [], "pixel-color": []}}


object = { "node": {"input-node": [], "output-node": []}, "pixel": [],
          "shape": [], "color": [], "size": [],"insert-flag":[]}

object_pixel = {  "node": {"input-node": [], "output-node": []},"pixel": [], "color": []}





def inital():
    global predicates

    global predicates_mul

    global predicates_same

    global predicates_pixel

    global predicates_none

    global predicates_node


    predicates = {"init": { "node-color": [], "node-size": [],"node-shape": [],
                           "contain-pixel": [], "contain-node": [], "pixel-color": [], "node-background-color": [],
                           "size-max": [], "size-min": [], "color-max": [], "color-min": [],
                           "odd-node": [], "even-node": [], "incomplete-contain-node": [], "node-spatial-relation": [],
                           "node-diagonal": [], "background-color": []},
                  "goal": {"contain-pixel-output": [], "pixel-color": []}}

    predicates_mul = {"init": { "node-color": [],"node-size": [], "node-shape": [],
                               "contain-pixel": [], "contain-node": [], "pixel-color": [], "node-background-color": [],
                               "size-max": [], "size-min": [],
                               "odd-node": [], "even-node": [], "incomplete-contain-node": [],
                               "node-spatial-relation": [], "node-diagonal": [], "background-color": []},
                      "goal": {"contain-pixel-output": [], "pixel-color": []}}

    predicates_same = {"init": { "node-color": [],"node-size": [],
                                "contain-pixel": [], "contain-node": [], "pixel-color": [], "node-background-color": [],
                                "color-min": [],
                                "same-node": [], "same-color": [], "incomplete-contain-node": [],
                                "node-spatial-relation": [], "background-color": []},
                       "goal": {"contain-pixel-output": [], "pixel-color": []}}

    predicates_pixel = {
        "init": {"pixel-color": [], "node-color": [], "node-similar": [], "contain-pixel": [], "background-color": [],
                 "color-most": [], "color-least": [], "left-diagonal-node": [], "right-diagonal-node": [],
                 "up-border-node": [],
                 "down-border-node": [], "left-border-node": [], "right-border-node": [],"vertical-middle-node":[],"horizontal-middle-node":[]},
        "goal": {"contain-pixel-output": [], "pixel-color": []}}

    predicates_none = {"init": {"node-size": [], "contain-pixel": [], "pixel-color": []},
                       "goal": {"contain-pixel-output": [], "pixel-color": []}}

    predicates_node = {
        "init": {"background-color": [], "node-color": [],"node-size": [], "contain-pixel": [], "pixel-color": [],
                 "node-background-color": [], "node-spatial-relation": [], "node-diagonal": []},
        "goal": {"contain-pixel-output": [], "pixel-color": []}}

    global object


    global object_pixel



    object = { "node": {"input-node": [], "output-node": []}, "pixel": [],
              "shape": [], "color": [], "size": [],"insert-flag":[]}

    object_pixel = { "node": {"input-node": [], "output-node": []},"pixel": [], "color": []}




def pddl_generate_traning(graph_input,graph_output,number,file_name,repeat_abstract):

    predicates_dict=dict()
    # generate the instance file for each input-output image pair with method instance_GP
    for i in range (len(graph_input.abstraction_key)):
        inital()
        if graph_input.abstraction_key[i] in repeat_abstract:
            predicates_dict[graph_input.abstraction_key[i]] = []
            continue
        input_nodes=graph_input.getNode()[graph_input.abstraction_key[i]]
        if len(input_nodes)==0:
            predicates_dict[graph_input.abstraction_key[i]] = []
            repeat_abstract.append(graph_input.abstraction_key[i])
            continue
        output_nodes = graph_output.getNode()[graph_input.abstraction_key[i]]
        if graph_input.abstraction_key[i]=="multicolor_connect_component_4" or graph_input.abstraction_key[i]=="multicolor_connect_component_8":
            object_predicate_simple(graph_input, graph_output, input_nodes,output_nodes, object, predicates_mul)
            instance_GP(file_name[i]+ "_" + "training", number, object, predicates_mul)
            predicates_dict[graph_input.abstraction_key[i]] = predicates_mul

        elif graph_input.abstraction_key[i]=="same_connect_component_4" or graph_input.abstraction_key[i]=="same_connect_component_8":
            object_predicate_simple(graph_input, graph_output, input_nodes, output_nodes,object, predicates_same)
            instance_GP(file_name[i]+ "_" + "training", number, object, predicates_same)
            predicates_dict[graph_input.abstraction_key[i]] = predicates_same

        elif graph_input.abstraction_key[i]=="recolor_connect_component_0":
            object_predicate_pixel(graph_input, graph_output, input_nodes, object_pixel, predicates_pixel)
            instance_pixel_GP(file_name[i] + "_" + "training", number, object_pixel, predicates_pixel)
            predicates_dict[graph_input.abstraction_key[i]] = predicates_pixel

        elif graph_input.abstraction_key[i]=="none_abstract":
            object_predicate_none(graph_input, graph_output, input_nodes, output_nodes,object, predicates_none)
            instance_none_GP(file_name[i] + "_" + "training", number, object, predicates_none)
            predicates_dict[graph_input.abstraction_key[i]] = predicates_none

        elif graph_input.abstraction_key[i]=="black_removed_node_position_4":
            object_predicate_simple(graph_input, graph_output, input_nodes, output_nodes,object, predicates_node)
            instance_node_relation_GP(file_name[i] + "_" + "training", number, object, predicates_node)
            predicates_dict[graph_input.abstraction_key[i]] = predicates_node

        elif graph_input.abstraction_key[i]=="color_vertical_connect_component_4":
            object_predicate_vertical(graph_input, graph_output, input_nodes, output_nodes,object, predicates)
            instance_GP(file_name[i] + "_" + "training", number, object, predicates)
            predicates_dict[graph_input.abstraction_key[i]] = predicates
        elif graph_input.abstraction_key[i]=="color_horizontal_connect_component_4":
            object_predicate_horizontal(graph_input, graph_output, input_nodes, output_nodes,object, predicates)
            instance_GP(file_name[i] + "_" + "training", number, object, predicates)
            predicates_dict[graph_input.abstraction_key[i]] = predicates
        else:
           object_predicate_simple(graph_input, graph_output, input_nodes, output_nodes,object, predicates)
           instance_GP(file_name[i]+ "_" + "training", number, object, predicates)

           predicates_dict[graph_input.abstraction_key[i]]=predicates
    return predicates_dict



def pddl_generate_testing(graph_input,graph_output,number,file_name,repeat_abstract):


    predicates_dict=dict()

    for i in range (len(graph_input.abstraction_key)):
        inital()
        if graph_input.abstraction_key[i] in repeat_abstract:
            continue
        input_nodes=graph_input.getNode()[graph_input.abstraction_key[i]]
        if len(input_nodes)==0:
            predicates_dict[graph_input.abstraction_key[i]] = []
            continue
        output_nodes = graph_output.getNode()[graph_input.abstraction_key[i]]
        if graph_input.abstraction_key[i]=="multicolor_connect_component_4" or graph_input.abstraction_key[i]=="multicolor_connect_component_8":
            object_predicate_simple(graph_input, graph_output, input_nodes, output_nodes, object, predicates_mul)
            instance_GP(file_name[i]+ "_" + "testing", number, object, predicates_mul)

            predicates_dict[graph_input.abstraction_key[i]] = predicates_mul

        elif graph_input.abstraction_key[i]=="same_connect_component_4" or graph_input.abstraction_key[i]=="same_connect_component_8":
            object_predicate_simple(graph_input, graph_output, input_nodes, output_nodes, object, predicates_same)
            instance_GP(file_name[i]+ "_" + "testing", number, object, predicates_same)

            predicates_dict[graph_input.abstraction_key[i]] = predicates_same


        elif graph_input.abstraction_key[i]=="recolor_connect_component_0":
            object_predicate_pixel(graph_input, graph_output, input_nodes, object_pixel, predicates_pixel)
            instance_pixel_GP(file_name[i] + "_" + "testing", number, object_pixel, predicates_pixel)

            predicates_dict[graph_input.abstraction_key[i]] = predicates_pixel

        elif graph_input.abstraction_key[i]=="none_abstract":
            object_predicate_none(graph_input, graph_output, input_nodes, output_nodes,object, predicates_none)
            instance_none_GP(file_name[i] + "_" + "testing", number, object, predicates_none)

            predicates_dict[graph_input.abstraction_key[i]] = predicates_none

        elif graph_input.abstraction_key[i]=="black_removed_node_position_4":
            object_predicate_simple(graph_input, graph_output, input_nodes, output_nodes,object, predicates_node)
            instance_node_relation_GP(file_name[i] + "_" + "testing", number, object, predicates_node)
            predicates_dict[graph_input.abstraction_key[i]] = predicates_node
        elif graph_input.abstraction_key[i]=="color_vertical_connect_component_4":
            object_predicate_vertical(graph_input, graph_output, input_nodes, output_nodes,object, predicates_node)
            instance_GP(file_name[i] + "_" + "testing", number, object, predicates_node)
            predicates_dict[graph_input.abstraction_key[i]] = predicates_node
        elif graph_input.abstraction_key[i]=="color_horizontal_connect_component_4":
            object_predicate_horizontal(graph_input, graph_output, input_nodes, output_nodes,object, predicates_node)
            instance_GP(file_name[i] + "_" + "testing", number, object, predicates_node)
            predicates_dict[graph_input.abstraction_key[i]] = predicates_node

        else:
           object_predicate_simple(graph_input, graph_output, input_nodes,output_nodes, object, predicates)
           instance_GP(file_name[i]+ "_" + "testing", number, object, predicates)
           predicates_dict[graph_input.abstraction_key[i]] = predicates

    return predicates_dict





def instance_GP(path,number,object_input,predicates_input):
    str_problem = "ARC-problem" + "\n"
    str_problem += "OBJECTS:"+"\n"

    def collect(object,str_problem):
        for key in object:
            if isinstance(object[key],dict):
                str_problem=collect(object[key],str_problem)
            else:
                values = list(object[key])
                if key=="input-node":
                    values.sort()
                    for node in values:
                        str_problem += "node"+str(node) + ":node\n"

                elif key=="output-node":
                    values.sort()
                    for node in values:
                        str_problem += "output-node"+str(node) + ":output-node\n"
                elif key=="insert-flag":
                    values.sort()
                    for flag in values:
                        str_problem += flag+ ":"+key +"\n"

                elif key=="shape":
                    if len(values)!=0:
                        values.sort()
                        for node in values:
                            str_problem += str(node) + ":" + key + "\n"
                elif key=="pixel":
                    sorted(values, key=lambda x: (x[0], -x[1]))
                    for node in values:
                        str_problem += key + str(node[0])+"-"+str(node[1]) + ":" + key + "\n"
                else:
                    if len(values) != 0:
                        values.sort()
                        for node in values:
                            str_problem += key+"-"+str(node) +":"+key+"\n"

        return  str_problem


    str_problem=collect(object_input,str_problem)


    rotation=["90:rotation", "180:rotation","270:rotation"]

    direction=["VERTICAL:flip-direction","HORIZONTAL:flip-direction","DIAGONAL_LEFT:flip-direction","DIAGONAL_RIGHT:flip-direction"]

    spatial=["left:move-direction","right:move-direction","up:move-direction","down:move-direction","left-up:move-direction", "left-down:move-direction", "right-up:move-direction", "right-down:move-direction"]


    for rot in rotation:
        str_problem+=rot +"\n"

    for dir in direction:
        str_problem+=dir+"\n"


    for spa in spatial:
        str_problem+=spa+"\n"


    str_problem+="INIT:" + "\n"
    for key,values in predicates_input["init"].items():
        for value in values:
            if key=="background-color":
                str_problem+="( "+str(key) +"("+value+") = 1 )" + "\n"
            elif len(value)==1:
                str_problem += "( " + str(key) + "(" + value[0] + ") = 1 )" + "\n"
            else:
                name=",".join(list(value))
                str_problem+="( "+str(key) +"("+name+") = 1 )" + "\n"

    str_problem += "GOAL:"+ "\n"

    for key,values in predicates_input["goal"].items():
        for value in values:
            if key=="background":
                str_problem+="( "+str(key) +"("+value+") = 1 )" + "\n"
            else:
                name=",".join(list(value))
                str_problem+="( "+str(key) +"("+name+") = 1 )" + "\n"

    file_path=str(number+1)+".txt"
    f_problem = open(os.path.join(path,file_path), "w")
    f_problem.write(str_problem)
    f_problem.close()



def instance_node_relation_GP(path,number,object_input,predicates_input):
    str_problem = "ARC-problem" + "\n"
    str_problem += "OBJECTS:"+"\n"

    def collect(object,str_problem):
        for key in object:
            if isinstance(object[key],dict):
                str_problem=collect(object[key],str_problem)
            else:
                values = list(object[key])
                if key=="input-node":
                    values.sort()
                    for node in values:

                        str_problem += "node"+str(node) + ":node\n"

                elif key=="output-node":
                    values.sort()
                    for node in values:

                        str_problem += "output-node"+str(node) + ":output-node\n"
                elif key=="insert-flag":
                    values.sort()
                    for flag in values:

                        str_problem += flag+ ":"+key +"\n"

                elif key=="shape":
                    if len(values)!=0:
                        values.sort()
                        for node in values:
                            str_problem += str(node) + ":" + key + "\n"
                elif key=="pixel":
                    sorted(values, key=lambda x: (x[0], -x[1]))
                    for node in values:
                        str_problem += key + str(node[0])+"-"+str(node[1]) + ":" + key + "\n"
                else:
                    if len(values) != 0:
                        values.sort()
                        for node in values:
                            str_problem += key+"-"+str(node) +":"+key+"\n"

        return  str_problem


    str_problem=collect(object_input,str_problem)




    spatial=["left:move-direction","right:move-direction","up:move-direction","down:move-direction","left-up:move-direction", "left-down:move-direction", "right-up:move-direction", "right-down:move-direction"]

    step=["one:step","max:step"]

    for spa in spatial:
        str_problem+=spa+"\n"

    for st in step:
        str_problem+=st+"\n"


    str_problem+="INIT:" + "\n"
    for key,values in predicates_input["init"].items():
        for value in values:
            if key=="background-color":
                str_problem+="( "+str(key) +"("+value+") = 1 )" + "\n"
            elif len(value)==1:
                str_problem += "( " + str(key) + "(" + value[0] + ") = 1 )" + "\n"
            else:
                name=",".join(list(value))
                str_problem+="( "+str(key) +"("+name+") = 1 )" + "\n"

    str_problem += "GOAL:"+ "\n"

    for key,values in predicates_input["goal"].items():
        for value in values:
            if key=="background":
                str_problem+="( "+str(key) +"("+value+") = 1 )" + "\n"
            else:
                name=",".join(list(value))
                str_problem+="( "+str(key) +"("+name+") = 1 )" + "\n"

    file_path=str(number+1)+".txt"
    f_problem = open(os.path.join(path,file_path), "w")
    f_problem.write(str_problem)
    f_problem.close()


def instance_pixel_GP(path,number,object_input,predicates_input):
    str_problem = "ARC-problem" + "\n"
    str_problem += "OBJECTS:"+"\n"

    def collect(object,str_problem):
        for key in object:
            if isinstance(object[key],dict):
                str_problem=collect(object[key],str_problem)
            else:
                values = list(object[key])
                if key=="input-node":
                    values.sort()
                    for node in values:
                        str_problem += "node"+str(node) + ":node\n"
                elif key=="shape":
                    values.sort()
                    for node in values:
                            str_problem += str(node) + ":" + key + "\n"
                elif key=="pixel":
                    sorted(values, key=lambda x: (x[0], -x[1]))
                    for node in values:
                        str_problem += key + str(node[0])+"-"+str(node[1]) + ":" + key + "\n"
                else:
                    values.sort()
                    for node in values:
                        str_problem += key+"-"+str(node) +":"+key+"\n"

        return  str_problem


    str_problem=collect(object_input,str_problem)




    str_problem+="INIT:" + "\n"
    for key,values in predicates_input["init"].items():
        for value in values:
            if key=="background-color":
                str_problem+="( "+str(key) +"("+value+") = 1 )" + "\n"
            elif key=="color-most":
                str_problem+="( "+str(key) +"("+value+") = 1 )" + "\n"
            elif key=="color-least":
                str_problem+="( "+str(key) +"("+value+") = 1 )" + "\n"
            elif len(value)==1:
                str_problem += "( " + str(key) + "(" + value[0] + ") = 1 )" + "\n"
            else:
                name=",".join(list(value))
                str_problem+="( "+str(key) +"("+name+") = 1 )" + "\n"

    str_problem += "GOAL:"+ "\n"

    for key,values in predicates_input["goal"].items():
        for value in values:
            if key=="background":
                str_problem+="( "+str(key) +"("+value+") = 1 )" + "\n"
            else:
                name=",".join(list(value))
                str_problem+="( "+str(key) +"("+name+") = 1 )" + "\n"

    file_path=str(number+1)+".txt"
    f_problem = open(os.path.join(path,file_path), "w")
    f_problem.write(str_problem)
    f_problem.close()


def instance_none_GP(path, number, object_input, predicates_input):
    str_problem = "ARC-problem" + "\n"
    str_problem += "OBJECTS:" + "\n"

    def collect(object, str_problem):
        for key in object:
            if isinstance(object[key], dict):
                str_problem = collect(object[key], str_problem)
            else:
                values = list(object[key])
                if key == "input-node":
                    values.sort()
                    for node in values:
                        str_problem += "node" + str(node) + ":node\n"

                elif key == "output-node":
                    values.sort()
                    for node in values:
                        str_problem += "output-node" + str(node) + ":output-node\n"

                elif key=="insert-flag":
                    values.sort()
                    for flag in values:
                        str_problem += flag+ ":"+key +"\n"

                elif key == "shape":
                    if len(values) != 0:
                        values.sort()
                        for node in values:
                            str_problem += str(node) + ":" + key + "\n"
                elif key == "pixel":
                    sorted(values, key=lambda x: (x[0], -x[1]))
                    for node in values:
                        str_problem += key + str(node[0]) + "-" + str(node[1]) + ":" + key + "\n"
                else:
                    if len(values) != 0:
                        values.sort()
                        for node in values:
                            str_problem += key + "-" + str(node) + ":" + key + "\n"

        return str_problem

    str_problem = collect(object_input, str_problem)

    rotation = ["90:rotation", "180:rotation", "270:rotation"]

    direction = ["VERTICAL:flip-direction", "HORIZONTAL:flip-direction", "DIAGONAL_LEFT:flip-direction",
                 "DIAGONAL_RIGHT:flip-direction"]


    for rot in rotation:
        str_problem += rot + "\n"

    for dir in direction:
        str_problem += dir + "\n"



    str_problem += "INIT:" + "\n"
    for key, values in predicates_input["init"].items():
        for value in values:
            if len(value) == 1:
                str_problem += "( " + str(key) + "(" + value[0] + ") = 1 )" + "\n"
            else:
                name = ",".join(list(value))
                str_problem += "( " + str(key) + "(" + name + ") = 1 )" + "\n"

    str_problem += "GOAL:" + "\n"

    for key, values in predicates_input["goal"].items():
        for value in values:
            if key == "background":
                str_problem += "( " + str(key) + "(" + value + ") = 1 )" + "\n"
            else:
                name = ",".join(list(value))
                str_problem += "( " + str(key) + "(" + name + ") = 1 )" + "\n"

    file_path = str(number + 1) + ".txt"
    f_problem = open(os.path.join(path, file_path), "w")
    f_problem.write(str_problem)
    f_problem.close()


def domain(name,predicates,pruned_actions):


    action_list = {"update-color":"no:node,c1:color,c2:color", "move-node":"no1:node,no2:node", "extend-node":"no1:node,no2:node", "rotate":"no:node,rot:rotation", "add-border":"no:node,co1:color", "mirror":"no1:node,no2:node",
                   "flip":"no:node,fd:flip-direction", "insert-input-node":"no1:node,no2:node","fill-rectangle":"no:node,c1:color","hollow-rectangle":"no:node,c1:color","swap-color":"no1:node,no2:node","copy-color":"no1:node,no2:node"}


    str_problem = "ARC" + "\n"

    str_problem += "STATE DESCRIPTOR:"+"\n"

    for var in types:
        str_problem+= var +":var_type"+ "\n"

    str_problem += "\n"

    for key, values in predicates_relation.items():

        key_flag=False

        if isinstance(predicates,list):
           for pre in predicates:
               if key in list(pre["init"].keys()) and  len(pre["init"][key])!=0:
                   key_flag=True
                   break
        else:
            if key in list(predicates["init"].keys()) and len(predicates["init"][key]) != 0:
                key_flag = True

        if key_flag:
            str_problem+=key +"("
            str_problem+=",".join(values)+"):pred_type"+ "\n"

    str_problem += "\n"



    for key,value in action_list.items():
        if key in pruned_actions:
            continue
        if key=="update-color":
            str_problem += "ACTION: @"+key+"("+value+")"+"\n"
            str_problem+= "TYPE: memory" +"\n"
            str_problem+="PRECONDITIONS:"+"\n"
            str_problem+="( node-color(no,c1) = 1 )"+"\n"
            str_problem+="EFFECTS:"+"\n"
        else:
            str_problem += "ACTION: @"+key+"("+value+")"+"\n"
            str_problem+= "TYPE: memory" +"\n"
            str_problem+="PRECONDITIONS:"+"\n"
            str_problem+="EFFECTS:"+"\n"



    f_domain = open( os.path.join(name,"domain.txt"), "w")
    f_domain.write(str_problem)
    f_domain.close



def domain_same(name,predicates,pruned_actions):


    action_list = {"update-color":"no:node,c1:color,c2:color", "move-node":"no1:node,no2:node","swap-color":"no1:node,no2:node","copy-color":"no1:node,no2:node"}

    str_problem = "ARC" + "\n"

    str_problem += "STATE DESCRIPTOR:"+"\n"

    for var in types:
        str_problem+= var +":var_type"+ "\n"

    str_problem += "\n"

    for key, values in predicates_relation_same.items():

        key_flag=False

        if isinstance(predicates,list):
           for pre in predicates:
               if key in list(pre["init"].keys()) and  len(pre["init"][key])!=0:
                   key_flag=True
                   break
               if key=="contain-pixel-output":
                    key_flag=True
                    break
        else:
            if key in list(predicates["init"].keys()) and len(predicates["init"][key]) != 0:
                key_flag = True


        if key_flag:
            str_problem+=key +"("
            str_problem+=",".join(values)+"):pred_type"+ "\n"

    str_problem += "\n"


    for key,value in action_list.items():
        if key in pruned_actions:
            continue
        if key=="update-color":
            str_problem += "ACTION: @"+key+"("+value+")"+"\n"
            str_problem+= "TYPE: memory" +"\n"
            str_problem+="PRECONDITIONS:"+"\n"
            str_problem+="( node-color(no,c1) = 1 )"+"\n"
            str_problem+="EFFECTS:"+"\n"
        else:
            str_problem += "ACTION: @"+key+"("+value+")"+"\n"
            str_problem+= "TYPE: memory" +"\n"
            str_problem+="PRECONDITIONS:"+"\n"
            str_problem+="EFFECTS:"+"\n"




    f_domain = open( os.path.join(name,"domain.txt"), "w")
    f_domain.write(str_problem)
    f_domain.close





def domain_node_position(name,predicates,pruned_actions):

    action_list = {"update-color":"no:node,c1:color,c2:color", "move-node-direction":"no:node,st:step:,dir:move-direction","move-node-max-direction":"no:node,dir:move-direction", "move-between-nodes-direction":"no1:node,no2:node","extend-node-direction":"no:node,dir:move-direction", "rotate":"no:node,rot:rotation", "add-border":"no:node,co1:color", "mirror":"no1:node,no2:node",
                   "flip":"no:node,fd:flip-direction", "insert-output-node":"no1:node,no2:output-node","insert-input-node":"no1:node,no2:node","fill-rectangle":"no:node,c1:color","hollow-rectangle":"no:node,c1:color"}

    str_problem = "ARC" + "\n"

    str_problem += "STATE DESCRIPTOR:"+"\n"

    for var in types:
        str_problem+= var +":var_type"+ "\n"

    str_problem+="step"+":var_type"+ "\n"

    str_problem += "\n"

    for key, values in predicates_node_relation.items():

        key_flag=False

        if key=="node-relative-position":
            key_flag=True

        if isinstance(predicates,list):
           for pre in predicates:
               if key in list(pre["init"].keys()) and  len(pre["init"][key])!=0:
                   key_flag=True
                   break
               if key=="contain-pixel-output":
                    key_flag=True
                    break
        else:
            if key in list(predicates["init"].keys()) and len(predicates["init"][key]) != 0:
                key_flag = True


        if key_flag:
            str_problem+=key +"("
            str_problem+=",".join(values)+"):pred_type"+ "\n"

    str_problem += "\n"

    for key,value in action_list.items():
        if key in pruned_actions:
            continue
        if key=="update-color":
            str_problem += "ACTION: @"+key+"("+value+")"+"\n"
            str_problem+= "TYPE: memory" +"\n"
            str_problem+="PRECONDITIONS:"+"\n"
            str_problem+="( node-color(no,c1) = 1 )"+"\n"
            str_problem+="EFFECTS:"+"\n"
        else:
            str_problem += "ACTION: @"+key+"("+value+")"+"\n"
            str_problem+= "TYPE: memory" +"\n"
            str_problem+="PRECONDITIONS:"+"\n"
            str_problem+="EFFECTS:"+"\n"

    f_domain = open( os.path.join(name,"domain.txt"), "w")
    f_domain.write(str_problem)
    f_domain.close




def object_predicate_simple(graph_input, graph_output,input_nodes,output_nodes,object_input,predicates_input):



    input_image = graph_input.getImage()
    output_image = graph_output.getImage()

    predicates_input["init"]["background-color"].append(("color-" + str(graph_input.background_color)))

    for x in range(graph_input.size[0]):
        for y in range(graph_input.size[1]):

            position = "pixel" + str(x) + "-" + str(y)
            color = input_image[x][y]
            object_input["pixel"].append((x, y))
            predicates_input["init"]["pixel-color"].append(((position), "color-" + str(input_image[x][y])))
            if color not in object_input["color"]:
                object_input["color"].append(color)

    for node in input_nodes:

            node_name = node.id

            object_input["node"]["input-node"].append(node_name)
            if node.size!=0 and node.size not in object_input["size"]:
                object_input["size"].append(node.size)
            if node.shape!="" and  node.shape not in object_input["shape"]:
                object_input["shape"].append(node.shape)

            x_values = node.p[0]
            y_values = node.p[1]
            pixels = zip(x_values, y_values)

            preidcates_keys=predicates_input["init"].keys()

            node_name = "node" + str(node.id)
            for pixel in pixels:
                predicates_input["init"]["contain-pixel"].append((node_name, "pixel" + str(pixel[0]) + "-" + str(pixel[1])))
            if "node-size" in preidcates_keys:
                predicates_input["init"]["node-size"].append((node_name, "size-" + str(node.size)))
            if "node-color" in preidcates_keys:
                predicates_input["init"]["node-color"].append((node_name, "color-" + str(node.color)))
            if "node-shape" in preidcates_keys:
                 predicates_input["init"]["node-shape"].append((node_name, str(node.shape)))




            if node.max_size_node  and "size-max" in preidcates_keys:
                predicates_input["init"]["size-max"].append([node_name])

            if node.odd and "odd-node" in preidcates_keys:
                predicates_input["init"]["odd-node"].append([node_name])

            if node.even and "even-node" in preidcates_keys:
                predicates_input["init"]["even-node"].append([node_name])

            if node.min_size_node and "size-min" in preidcates_keys:
                predicates_input["init"]["size-min"].append([node_name])

            if node.most_color_node and "color-max" in preidcates_keys:
                predicates_input["init"]["color-max"].append([node_name])

            if node.least_color_node and "color-min" in preidcates_keys:
                predicates_input["init"]["color-min"].append([node_name])


            if len(node.contain) != 0 and "contain-node" in preidcates_keys:
                for nodes in node.contain:
                    contain_node_name = "node" + str(nodes.id)
                    predicates_input["init"]["contain-node"].append((node_name, contain_node_name))

            if len(node.incomplete_contain) != 0 and "incomplete-contain-node" in preidcates_keys:
                for nodes in node.incomplete_contain:
                    incomplete_contain_node_name = "node" + str(nodes.id)
                    predicates_input["init"]["incomplete-contain-node"].append((node_name, incomplete_contain_node_name))


            if len(node.same) != 0 and "same-node" in preidcates_keys:
                if "same-node" in list(predicates_input["init"].keys()):
                     for nodes in node.same:
                        same_node_name = "node" + str(nodes.id)
                        predicates_input["init"]["same-node"].append((node_name, same_node_name))

            if len(node.same_color) != 0 and "same-color" in preidcates_keys:
                if "same-color" in list(predicates_input["init"].keys()):
                     for nodes in node.same_color:
                        same_node_name = "node" + str(nodes.id)
                        predicates_input["init"]["same-color"].append((node_name, same_node_name))

            if len(node.left) != 0 and "node-spatial-relation" in preidcates_keys:
                for nodes in node.left:
                    left_node_name = "node" + str(nodes.id)
                    predicates_input["init"]["node-spatial-relation"].append((node_name, left_node_name,"left"))

            if len(node.right) != 0 and "node-spatial-relation" in preidcates_keys:
                for nodes in node.right:
                    left_node_name = "node" + str(nodes.id)
                    predicates_input["init"]["node-spatial-relation"].append((node_name, left_node_name,"right"))

            if len(node.up) != 0 and "node-spatial-relation" in preidcates_keys:
                for nodes in node.up:
                    left_node_name = "node" + str(nodes.id)
                    predicates_input["init"]["node-spatial-relation"].append((node_name, left_node_name,"up"))

            if len(node.down) != 0 and "node-spatial-relation" in preidcates_keys:
                for nodes in node.down:
                    left_node_name = "node" + str(nodes.id)
                    predicates_input["init"]["node-spatial-relation"].append((node_name, left_node_name,"down"))


            if len(node.left_up) != 0 and "node-spatial-relation" in preidcates_keys:
                for nodes in node.left_up:
                    left_node_name = "node" + str(nodes.id)
                    predicates_input["init"]["node-spatial-relation"].append((node_name, left_node_name,"left-up"))

            if len(node.left_up) != 0 and "node-diagonal" in preidcates_keys:
                for nodes in node.left_up:
                    left_node_name = "node" + str(nodes.id)
                    predicates_input["init"]["node-diagonal"].append((node_name, left_node_name))


            if len(node.left_down) != 0 and "node-spatial-relation" in preidcates_keys:
                for nodes in node.left_down:
                    left_node_name = "node" + str(nodes.id)
                    predicates_input["init"]["node-spatial-relation"].append((node_name, left_node_name,"left-down"))


            if len(node.left_down) != 0 and "node-diagonal" in preidcates_keys:
                for nodes in node.left_down:
                    left_node_name = "node" + str(nodes.id)
                    predicates_input["init"]["node-diagonal"].append((node_name, left_node_name))


            if len(node.right_up) != 0 and "node-spatial-relation" in preidcates_keys:
                for nodes in node.right_up:
                    right_node_name = "node" + str(nodes.id)
                    predicates_input["init"]["node-spatial-relation"].append((node_name, right_node_name,"right-up"))

            if len(node.right_up) != 0 and "node-diagonal" in preidcates_keys:
                for nodes in node.right_up:
                    right_node_name = "node" + str(nodes.id)
                    predicates_input["init"]["node-diagonal"].append((node_name, right_node_name))

            if len(node.right_down) != 0 and "node-spatial-relation" in preidcates_keys:
                for nodes in node.right_down:
                    right_node_name = "node" + str(nodes.id)
                    predicates_input["init"]["node-spatial-relation"].append((node_name, right_node_name,"right-down"))


            if len(node.right_down) != 0 and "node-diagonal" in preidcates_keys:
                for nodes in node.right_down:
                    right_node_name = "node" + str(nodes.id)
                    predicates_input["init"]["node-diagonal"].append((node_name, right_node_name))



    for x in range(graph_output.size[0]):
        for y in range(graph_output.size[1]):

            color = output_image[x][y]

            predicates_input["goal"]["pixel-color"].append(
                (("pixel" + str(x) + "-" + str(y)), "color-" + str(output_image[x][y])))

            if color not in object_input["color"]:
                object_input["color"].append(color)



def object_predicate_vertical(graph_input, graph_output,input_nodes,output_nodes,object_input,predicates_input):



    input_image = graph_input.getImage()
    output_image = graph_output.getImage()

    predicates_input["init"]["background-color"].append(("color-" + str(graph_input.background_color)))

    for x in range(graph_input.size[0]):
        for y in range(graph_input.size[1]):

            position = "pixel" + str(x) + "-" + str(y)
            color = input_image[x][y]
            object_input["pixel"].append((x, y))
            predicates_input["init"]["pixel-color"].append(((position), "color-" + str(input_image[x][y])))
            if color not in object_input["color"]:
                object_input["color"].append(color)


    for node in input_nodes:

            node_name = node.id

            object_input["node"]["input-node"].append(node_name)

            if node.size!=0 and node.size not in object_input["size"]:
                object_input["size"].append(node.size)
            if node.shape!="" and  node.shape not in object_input["shape"]:
                object_input["shape"].append(node.shape)

            x_values = node.p[0]
            y_values = node.p[1]
            pixels = zip(x_values, y_values)

            preidcates_keys=predicates_input["init"].keys()

            node_name = "node" + str(node.id)
            for pixel in pixels:
                predicates_input["init"]["contain-pixel"].append((node_name, "pixel" + str(pixel[0]) + "-" + str(pixel[1])))
            if "node-size" in preidcates_keys:
                predicates_input["init"]["node-size"].append((node_name, "size-" + str(node.size)))
            if "node-color" in preidcates_keys:
                predicates_input["init"]["node-color"].append((node_name, "color-" + str(node.color)))
            if "node-shape" in preidcates_keys:
                 predicates_input["init"]["node-shape"].append((node_name, str(node.shape)))

            if node.max_size_node  and "size-max" in preidcates_keys:
                predicates_input["init"]["size-max"].append([node_name])

            if node.odd and "odd-node" in preidcates_keys:
                predicates_input["init"]["odd-node"].append([node_name])

            if node.even and "even-node" in preidcates_keys:
                predicates_input["init"]["even-node"].append([node_name])

            if node.min_size_node and "size-min" in preidcates_keys:
                predicates_input["init"]["size-min"].append([node_name])

            if node.most_color_node and "color-max" in preidcates_keys:
                predicates_input["init"]["color-max"].append([node_name])

            if node.least_color_node and "color-min" in preidcates_keys:
                predicates_input["init"]["color-min"].append([node_name])


            if len(node.contain) != 0 and "contain-node" in preidcates_keys:
                for nodes in node.contain:
                    contain_node_name = "node" + str(nodes.id)
                    predicates_input["init"]["contain-node"].append((node_name, contain_node_name))

            if len(node.incomplete_contain) != 0 and "incomplete-contain-node" in preidcates_keys:
                for nodes in node.incomplete_contain:
                    incomplete_contain_node_name = "node" + str(nodes.id)
                    predicates_input["init"]["incomplete-contain-node"].append((node_name, incomplete_contain_node_name))


            if len(node.same) != 0 and "same-node" in preidcates_keys:
                if "same-node" in list(predicates_input["init"].keys()):
                     for nodes in node.same:
                        same_node_name = "node" + str(nodes.id)
                        predicates_input["init"]["same-node"].append((node_name, same_node_name))

            if len(node.same_color) != 0 and "same-color" in preidcates_keys:
                if "same-color" in list(predicates_input["init"].keys()):
                     for nodes in node.same_color:
                        same_node_name = "node" + str(nodes.id)
                        predicates_input["init"]["same-color"].append((node_name, same_node_name))


            if len(node.up) != 0 and "node-spatial-relation" in preidcates_keys:
                for nodes in node.up:
                    left_node_name = "node" + str(nodes.id)
                    predicates_input["init"]["node-spatial-relation"].append((node_name, left_node_name,"up"))

            if len(node.down) != 0 and "node-spatial-relation" in preidcates_keys:
                for nodes in node.down:
                    left_node_name = "node" + str(nodes.id)
                    predicates_input["init"]["node-spatial-relation"].append((node_name, left_node_name,"down"))


    for x in range(graph_output.size[0]):
        for y in range(graph_output.size[1]):
            color = output_image[x][y]
            predicates_input["goal"]["pixel-color"].append(
                (("pixel" + str(x) + "-" + str(y)), "color-" + str(output_image[x][y])))

            if color not in object_input["color"]:
                object_input["color"].append(color)



def object_predicate_horizontal(graph_input, graph_output,input_nodes,output_nodes,object_input,predicates_input):



    input_image = graph_input.getImage()
    output_image = graph_output.getImage()

    predicates_input["init"]["background-color"].append(("color-" + str(graph_input.background_color)))

    for x in range(graph_input.size[0]):
        for y in range(graph_input.size[1]):

            position = "pixel" + str(x) + "-" + str(y)
            color = input_image[x][y]
            object_input["pixel"].append((x, y))
            predicates_input["init"]["pixel-color"].append(((position), "color-" + str(input_image[x][y])))
            if color not in object_input["color"]:
                object_input["color"].append(color)


    for node in input_nodes:
            node_name = node.id

            object_input["node"]["input-node"].append(node_name)
            if node.size!=0 and node.size not in object_input["size"]:
                object_input["size"].append(node.size)
            if node.shape!="" and  node.shape not in object_input["shape"]:
                object_input["shape"].append(node.shape)

            x_values = node.p[0]
            y_values = node.p[1]
            pixels = zip(x_values, y_values)

            preidcates_keys=predicates_input["init"].keys()

            node_name = "node" + str(node.id)
            for pixel in pixels:
                predicates_input["init"]["contain-pixel"].append((node_name, "pixel" + str(pixel[0]) + "-" + str(pixel[1])))
            if "node-size" in preidcates_keys:
                predicates_input["init"]["node-size"].append((node_name, "size-" + str(node.size)))
            if "node-color" in preidcates_keys:
                predicates_input["init"]["node-color"].append((node_name, "color-" + str(node.color)))
            if "node-shape" in preidcates_keys:
                 predicates_input["init"]["node-shape"].append((node_name, str(node.shape)))



            if node.max_size_node  and "size-max" in preidcates_keys:
                predicates_input["init"]["size-max"].append([node_name])

            if node.odd and "odd-node" in preidcates_keys:
                predicates_input["init"]["odd-node"].append([node_name])

            if node.even and "even-node" in preidcates_keys:
                predicates_input["init"]["even-node"].append([node_name])

            if node.min_size_node and "size-min" in preidcates_keys:
                predicates_input["init"]["size-min"].append([node_name])

            if node.most_color_node and "color-max" in preidcates_keys:
                predicates_input["init"]["color-max"].append([node_name])

            if node.least_color_node and "color-min" in preidcates_keys:
                predicates_input["init"]["color-min"].append([node_name])


            if len(node.contain) != 0 and "contain-node" in preidcates_keys:
                for nodes in node.contain:
                    contain_node_name = "node" + str(nodes.id)
                    predicates_input["init"]["contain-node"].append((node_name, contain_node_name))

            if len(node.incomplete_contain) != 0 and "incomplete-contain-node" in preidcates_keys:
                for nodes in node.incomplete_contain:
                    incomplete_contain_node_name = "node" + str(nodes.id)
                    predicates_input["init"]["incomplete-contain-node"].append((node_name, incomplete_contain_node_name))


            if len(node.same) != 0 and "same-node" in preidcates_keys:
                if "same-node" in list(predicates_input["init"].keys()):
                     for nodes in node.same:
                        same_node_name = "node" + str(nodes.id)
                        predicates_input["init"]["same-node"].append((node_name, same_node_name))

            if len(node.same_color) != 0 and "same-color" in preidcates_keys:
                if "same-color" in list(predicates_input["init"].keys()):
                     for nodes in node.same_color:
                        same_node_name = "node" + str(nodes.id)
                        predicates_input["init"]["same-color"].append((node_name, same_node_name))

            if len(node.left) != 0 and "node-spatial-relation" in preidcates_keys:
                for nodes in node.left:
                    left_node_name = "node" + str(nodes.id)
                    predicates_input["init"]["node-spatial-relation"].append((node_name, left_node_name,"left"))

            if len(node.right) != 0 and "node-spatial-relation" in preidcates_keys:
                for nodes in node.right:
                    left_node_name = "node" + str(nodes.id)
                    predicates_input["init"]["node-spatial-relation"].append((node_name, left_node_name,"right"))

    for x in range(graph_output.size[0]):
        for y in range(graph_output.size[1]):

            color = output_image[x][y]

            predicates_input["goal"]["pixel-color"].append(
                (("pixel" + str(x) + "-" + str(y)), "color-" + str(output_image[x][y])))

            if color not in object_input["color"]:
                object_input["color"].append(color)



def object_predicate_pixel(graph_input, graph_output,input_nodes,object_input,predicates_input):
    input_image = graph_input.getImage()
    output_image = graph_output.getImage()

    predicates_input["init"]["background-color"].append(("color-" + str(graph_input.background_color)))

    if graph_input.most_color!=-1:
        predicates_input["init"]["color-most"].append(("color-" + str(graph_input.most_color)))

    if graph_input.least_color!=-1:
        predicates_input["init"]["color-least"].append(("color-" + str(graph_input.least_color)))

    for x in range(graph_input.size[0]):
        for y in range(graph_input.size[1]):
            position = "pixel" + str(x) + "-" + str(y)
            color = input_image[x][y]
            object_input["pixel"].append((x, y))
            predicates_input["init"]["pixel-color"].append(((position), "color-" + str(input_image[x][y])))
            if color not in object_input["color"]:
                object_input["color"].append(color)


    for node in input_nodes:

        node_name = node.id

        object_input["node"]["input-node"].append(node_name)

        x_values = node.p[0]
        y_values = node.p[1]
        pixels = zip(x_values, y_values)

        node_name = "node" + str(node.id)
        for pixel in pixels:
            predicates_input["init"]["contain-pixel"].append((node_name, "pixel" + str(pixel[0]) + "-" + str(pixel[1])))

        predicates_input["init"]["node-color"].append((node_name, "color-" + str(node.color)))
        predicates_input["init"]["node-similar"].append((node_name, "color-" + str(node.similar_color)))

        if node.left_diagonal_node:
            predicates_input["init"]["left-diagonal-node"].append([node_name])

        if node.right_diagonal_node:
            predicates_input["init"]["right-diagonal-node"].append([node_name])

        if node.up_border:
            predicates_input["init"]["up-border-node"].append([node_name])

        if node.down_border:
            predicates_input["init"]["down-border-node"].append([node_name])

        if node.left_border:
            predicates_input["init"]["left-border-node"].append([node_name])

        if node.right_border:

            predicates_input["init"]["right-border-node"].append([node_name])

        if node.vertical_middle:
            predicates_input["init"]["vertical-middle-node"].append([node_name])

        if node.horizontal_middle:
            predicates_input["init"]["horizontal-middle-node"].append([node_name])



    for x in range(graph_output.size[0]):
        for y in range(graph_output.size[1]):

            color = output_image[x][y]

            predicates_input["goal"]["pixel-color"].append(
                (("pixel" + str(x) + "-" + str(y)), "color-" + str(output_image[x][y])))

            if color not in object_input["color"]:
                object_input["color"].append(color)



def object_predicate_none(graph_input, graph_output,input_nodes,output_nodes,object_input,predicates_input):



    input_image = graph_input.getImage()
    output_image = graph_output.getImage()

    for x in range(graph_input.size[0]):
        for y in range(graph_input.size[1]):
            position = "pixel" + str(x) + "-" + str(y)
            color = input_image[x][y]
            object_input["pixel"].append((x, y))
            predicates_input["init"]["pixel-color"].append(((position), "color-" + str(input_image[x][y])))
            if color not in object_input["color"]:
                object_input["color"].append(color)


    for node in input_nodes:
            node_name = node.id

            object_input["node"]["input-node"].append(node_name)
            if node.size!=0 and node.size not in object_input["size"]:
                object_input["size"].append(node.size)


            x_values = node.p[0]
            y_values = node.p[1]
            pixels = zip(x_values, y_values)

            node_name = "node" + str(node.id)
            for pixel in pixels:
                predicates_input["init"]["contain-pixel"].append((node_name, "pixel" + str(pixel[0]) + "-" + str(pixel[1])))
            predicates_input["init"]["node-size"].append((node_name, "size-" + str(node.size)))



    for x in range(graph_output.size[0]):
        for y in range(graph_output.size[1]):

            color = output_image[x][y]
            predicates_input["goal"]["pixel-color"].append(
                (("pixel" + str(x) + "-" + str(y)), "color-" + str(output_image[x][y])))

            if color not in object_input["color"]:
                object_input["color"].append(color)


def check_node_same(image1, node1,image2,node2):
    if node1.size!=node2.size:
        return False
    for i in range (node1.size):
        if image1[node1.p[0][i]][node1.p[1][i]]!=image2[node2.p[0][i]][node2.p[1][i]]:
            return False
    return True


def pruned_actions (input_graph_list,output_graph_list,abstraction_key):
    pruned_actions=defaultdict(list)

    for abs_i in range(len(abstraction_key)):
        no_movements = True
        same_color = True
        same_size = True
        same_position = True
        no_node = True
        same_number_object=True
        hollow_node_required = False
        move_condition_required=False
        fill_node_required = False
        output_all_nodes_list=[]
        input_all_nodes_list=[]
        
        for graph_i in range (len(input_graph_list)):
            input_graph=input_graph_list[graph_i]
            output_graph=output_graph_list[graph_i]

            background_color=input_graph.background_color

            input_image=input_graph.image

            output_image=output_graph.image

            x,y=input_image.shape

            #detect change in each pixel among input and output image
            for i in range (x):
              for j in range (y):
                    if (input_image[i][j]!=background_color and output_image[i][j]==background_color) or\
                            (input_image[i][j] == background_color and output_image[i][j] != background_color):
                        no_movements=False

            input_nodes = input_graph.getNode()[abstraction_key[abs_i]]
            output_nodes = output_graph.getNode()[abstraction_key[abs_i]]
            output_all_nodes_list.append((output_image,output_nodes))

            input_all_nodes_list.append((input_image, input_nodes))



            if len(input_nodes)==0 and len(output_nodes)==0:
                continue
            else:
                no_node=False

            input_node_color=[]
            output_node_color=[]

            input_node_size=[]

            output_node_size=[]

            input_node_position=[]
            output_node_position=[]

            input_node_shape=[]

            if (len(input_nodes) != len(output_nodes)):
                same_number_object=False



            for node in input_nodes:
                if node.color==0:
                    continue
                input_node_color.append(node.color)
                input_node_size.append(node.size)
                input_node_shape.append(node.shape)
                if node.color == background_color:
                    continue
                for node_x in range (len(node.p[0])):
                    x_location=node.p[0][node_x]
                    y_location = node.p[1][node_x]
                    input_node_position.append((x_location,y_location))

            for node in output_nodes:
                if node.color==0:
                    continue
                output_node_color.append(node.color)
                output_node_size.append(node.size)
                if node.color == background_color:
                    continue
                for node_x in range (len(node.p[0])):
                    x_location=node.p[0][node_x]
                    y_location = node.p[1][node_x]
                    output_node_position.append((x_location,y_location))

         # action hollow a node is allowed only when at least one object with shape  square or rectangle
            for i in range(len(input_node_size)):
                if input_node_size[i]>4 and (input_node_shape[i]=="square" or input_node_shape[i]=="rectangle"):
                    hollow_node_required=True

            # action fill a node is allowed when at least one object with shape  unknown
            for shape in input_node_shape:
                if shape=="unknown":
                    fill_node_required=True

            input_node_color.sort()
            output_node_color.sort()
            input_node_size.sort()
            output_node_size.sort()
            input_node_position.sort()
            output_node_position.sort()



            if (input_node_size!=output_node_size):
                same_size=False


            if (input_node_color!=output_node_color):
                same_color=False

            if (input_node_position!=output_node_position):
                same_position=False

            if len(input_nodes) == len(output_nodes):
                if (input_node_size==output_node_size):
                    if (input_node_position!=output_node_position):
                        move_condition_required=True

        # evaluate the insert action which only included when consistent pattern exists.
        insert_output_node_required=False

        removed_node_list=[]

        for i in range(len(output_all_nodes_list)):
            for node1 in output_all_nodes_list[i][1]:
                count=0
                for j in range(len(output_all_nodes_list)):
                    for node2 in output_all_nodes_list[j][1]:
                        if check_node_same(output_all_nodes_list[i][0],node1,output_all_nodes_list[j][0],node2):
                            count+=1
                            break

                if count==len(output_all_nodes_list):
                    insert_output_node_required=True
                if count<len(output_all_nodes_list):
                    removed_node_list.append(node1)

        for i in range(len(input_all_nodes_list)):
            for node1 in input_all_nodes_list[i][1]:
                count=0
                for j in range(len(input_all_nodes_list)):
                    for node2 in input_all_nodes_list[j][1]:
                        if check_node_same(input_all_nodes_list[i][0],node1,input_all_nodes_list[j][0],node2):
                            count+=1
                            break
                if count==len(input_all_nodes_list):
                    insert_output_node_required=True


        # can be deleted
        for removed_node in removed_node_list:
           for i in range(len(output_all_nodes_list)):
               for j in range (len(output_all_nodes_list[i][1])-1,-1,-1):
                   if removed_node==output_all_nodes_list[i][1][j]:
                       del output_all_nodes_list[i][1][j]

        # remove actions based on abstractions or rules (comparing input and output image)
        if abstraction_key[abs_i] == "same_color_node" or abstraction_key[abs_i] == "same_connect_component_4" or abstraction_key[abs_i] == "same_connect_component_8":
            pass
        else:
            pruned_actions[abstraction_key[abs_i]] += ["swap-color","copy-color"]

        if not insert_output_node_required:
            pruned_actions[abstraction_key[abs_i]]+=["insert-output-node"]

        if not insert_output_node_required:
            pruned_actions[abstraction_key[abs_i]]+=["insert-input-node"]


        if abstraction_key[abs_i] == "black_node_4":
            pruned_actions[abstraction_key[abs_i]] += ["swap-color","move-node", "move-node", "extend-node","rotate","flip","add-border", "add-border", "insert-output-node","insert-input-node","fill-rectangle","hollow-rectangle","mirror"]
            continue

        if abstraction_key[abs_i] == "none_abstract":
            pruned_actions[abstraction_key[abs_i]] += ["swap-color","update-color", "move-node", "move-node", "extend-node",
                                                       "add-border", "insert-output-node","insert-input-node","fill-rectangle","hollow-rectangle","mirror"]
            continue


        if abstraction_key[abs_i] == "recolor_connect_component_0":
            pruned_actions[abstraction_key[abs_i]] += ["swap-color","move-node", "move-node",  "extend-node","rotate","add-border",
                                                    "mirror", "flip", "insert-output-node","insert-input-node","fill-rectangle","hollow-rectangle"]
            continue

        if abstraction_key[abs_i] == "same_color_node":
            pruned_actions[abstraction_key[abs_i]] += ["move-node", "move-node",  "extend-node","rotate","add-border",
                                                    "mirror", "flip", "insert-output-node","insert-input-node","fill-rectangle","hollow-rectangle"]
            continue

        if no_node:
            pruned_actions[abstraction_key[abs_i]] += ["update-color", "move-node", "move-node", "extend-node","move-node-direction","move-node-max-direction","move-between-nodes-direction","move-to-node","extend-node-direction",
                                                      "rotate", "add-border", "mirror", "flip", "insert-output-node","insert-input-node","fill-rectangle","hollow-rectangle"]

        if abstraction_key[abs_i] == "multicolor_connect_component_8" or abstraction_key[abs_i] == "multicolor_connect_component_4":
            pruned_actions[abstraction_key[abs_i]] += ["update-color"]


        if no_movements:
            pruned_actions[abstraction_key[abs_i]]+=["move-node","move-node-direction","extend-node-direction","move-node-max-direction","move-between-nodes-direction","move-to-node","extend-node", "move-node", "add-border","insert-output-node","insert-input-node","fill-rectangle","mirror", "rotate", "flip"]

        if same_color:
            pruned_actions[abstraction_key[abs_i]] += ["update-color"]



        if same_size:
            pruned_actions[abstraction_key[abs_i]]+=["insert-output-node","insert-input-node","extend-node","add-border","fill-rectangle","hollow-rectangle"]

        if not hollow_node_required:
            pruned_actions[abstraction_key[abs_i]] += ["hollow-rectangle"]

        if not fill_node_required:
            pruned_actions[abstraction_key[abs_i]] += ["fill-rectangle"]

        if same_number_object:
            pruned_actions[abstraction_key[abs_i]] += [ "add-border", "fill-rectangle",
                                                       "hollow-rectangle"]



        if same_position:
            pruned_actions[abstraction_key[abs_i]]+=["insert-output-node","insert-input-node","move-node", "extend-node", "move-node","move-node-direction","extend-node-direction","move-node-max-direction","move-between-nodes-direction","move-to-node","add-border","fill-rectangle","mirror", "rotate", "flip"]

        if abstraction_key[abs_i] != "none_abstract":
            pruned_actions[abstraction_key[abs_i]] += ["rotate", "flip"]



        if abstraction_key[abs_i] == "black_removed_node_position_4":
            pruned_actions[abstraction_key[abs_i]] += [ "mirror", "flip","rotate","add-border", "insert-output-node","insert-input-node","fill-rectangle","hollow-rectangle"]

        if "multicolor_connect_component" in abstraction_key[abs_i]:
            for i in range (len(pruned_actions[abstraction_key[abs_i]])-1,-1,-1):
                if pruned_actions[abstraction_key[abs_i]][i]=="insert-input-node":
                    del pruned_actions[abstraction_key[abs_i]][i]



            
    return pruned_actions


def pruned_actions_different_size(input_graph_list, output_graph_list, abstraction_key):
    pruned_actions = defaultdict(list)

    for abs_i in range(len(abstraction_key)):
        no_movements = True
        same_color = True
        same_size = True
        same_position = True
        no_node = True
        same_number_object = True
        hollow_node_required = False
        move_condition_required = False
        fill_node_required = False
        output_all_nodes_list = []
        input_all_nodes_list = []

        for graph_i in range(len(input_graph_list)):
            input_graph = input_graph_list[graph_i]
            output_graph = output_graph_list[graph_i]

            background_color = input_graph.background_color

            input_image = input_graph.image

            output_image = output_graph.image

            x_input, y_input = input_image.shape
            x_output, y_output = output_image.shape

            x=x_input if x_input<=x_output else x_output
            y = y_input if y_input <= y_output else y_output

            for i in range(x):
                for j in range(y):
                    if (input_image[i][j] != background_color and output_image[i][j] == background_color) or \
                            (input_image[i][j] == background_color and output_image[i][j] != background_color):
                        no_movements = False

            input_nodes = input_graph.getNode()[abstraction_key[abs_i]]
            output_nodes = output_graph.getNode()[abstraction_key[abs_i]]
            output_all_nodes_list.append((output_image, output_nodes))

            input_all_nodes_list.append((input_image, input_nodes))

            if len(input_nodes) == 0 and len(output_nodes) == 0:
                continue
            else:
                no_node = False

            input_node_color = []
            output_node_color = []

            input_node_size = []

            output_node_size = []

            input_node_position = []
            output_node_position = []

            input_node_shape = []

            if (len(input_nodes) != len(output_nodes)):
                same_number_object = False

            for node in input_nodes:
                if node.color == 0:
                    continue
                input_node_color.append(node.color)
                input_node_size.append(node.size)
                input_node_shape.append(node.shape)
                if node.color == background_color:
                    continue
                for node_x in range(len(node.p[0])):
                    x_location = node.p[0][node_x]
                    y_location = node.p[1][node_x]
                    input_node_position.append((x_location, y_location))

            for node in output_nodes:
                if node.color == 0:
                    continue
                output_node_color.append(node.color)
                output_node_size.append(node.size)
                if node.color == background_color:
                    continue
                for node_x in range(len(node.p[0])):
                    x_location = node.p[0][node_x]
                    y_location = node.p[1][node_x]
                    output_node_position.append((x_location, y_location))

            for i in range(len(input_node_size)):
                if input_node_size[i] > 4 and (input_node_shape[i] == "square" or input_node_shape[i] == "rectangle"):
                    hollow_node_required = True

            for shape in input_node_shape:
                if shape == "unknown":
                    fill_node_required = True

            input_node_color.sort()
            output_node_color.sort()
            input_node_size.sort()
            output_node_size.sort()
            input_node_position.sort()
            output_node_position.sort()

            if (input_node_size != output_node_size):
                same_size = False

            if (input_node_color != output_node_color):
                same_color = False

            if (input_node_position != output_node_position):
                same_position = False

            if len(input_nodes) == len(output_nodes):
                if (input_node_size == output_node_size):
                    if (input_node_position != output_node_position):
                        move_condition_required = True

        insert_output_node_required = False

        removed_node_list = []

        for i in range(len(output_all_nodes_list)):
            for node1 in output_all_nodes_list[i][1]:
                count = 0
                for j in range(len(output_all_nodes_list)):
                    for node2 in output_all_nodes_list[j][1]:
                        if check_node_same(output_all_nodes_list[i][0], node1, output_all_nodes_list[j][0], node2):
                            count += 1
                            break

                if count == len(output_all_nodes_list):
                    insert_output_node_required = True
                if count < len(output_all_nodes_list):
                    removed_node_list.append(node1)

        for i in range(len(input_all_nodes_list)):
            for node1 in input_all_nodes_list[i][1]:
                count = 0
                for j in range(len(input_all_nodes_list)):
                    for node2 in input_all_nodes_list[j][1]:
                        if check_node_same(input_all_nodes_list[i][0], node1, input_all_nodes_list[j][0], node2):
                            count += 1
                            break
                if count == len(input_all_nodes_list):
                    insert_output_node_required = True

        for removed_node in removed_node_list:
            for i in range(len(output_all_nodes_list)):
                for j in range(len(output_all_nodes_list[i][1]) - 1, -1, -1):
                    if removed_node == output_all_nodes_list[i][1][j]:
                        del output_all_nodes_list[i][1][j]

        if abstraction_key[abs_i] == "same_color_node" or abstraction_key[abs_i] == "same_connect_component_4" or \
                abstraction_key[abs_i] == "same_connect_component_8":
            pass
        else:
            pruned_actions[abstraction_key[abs_i]] += ["swap-color", "copy-color"]

        if not insert_output_node_required:
            pruned_actions[abstraction_key[abs_i]] += ["insert-output-node"]

        if not insert_output_node_required:
            pruned_actions[abstraction_key[abs_i]] += ["insert-input-node"]

        if abstraction_key[abs_i] == "black_node_4":
            pruned_actions[abstraction_key[abs_i]] += ["swap-color", "move-node", "move-node", "extend-node", "rotate",
                                                       "flip", "add-border", "add-border", "insert-output-node",
                                                       "insert-input-node", "fill-rectangle", "hollow-rectangle",
                                                       "mirror"]
            continue

        if abstraction_key[abs_i] == "none_abstract":
            pruned_actions[abstraction_key[abs_i]] += ["swap-color", "update-color", "move-node", "move-node",
                                                       "extend-node",
                                                       "add-border", "insert-output-node", "insert-input-node",
                                                       "fill-rectangle", "hollow-rectangle", "mirror"]
            continue

        if abstraction_key[abs_i] == "recolor_connect_component_0":
            pruned_actions[abstraction_key[abs_i]] += ["swap-color", "move-node", "move-node", "extend-node", "rotate",
                                                       "add-border",
                                                       "mirror", "flip", "insert-output-node", "insert-input-node",
                                                       "fill-rectangle", "hollow-rectangle"]
            continue

        if abstraction_key[abs_i] == "same_color_node":
            pruned_actions[abstraction_key[abs_i]] += ["move-node", "move-node", "extend-node", "rotate", "add-border",
                                                       "mirror", "flip", "insert-output-node", "insert-input-node",
                                                       "fill-rectangle", "hollow-rectangle"]
            continue

        if no_node:
            pruned_actions[abstraction_key[abs_i]] += ["update-color", "move-node", "move-node", "extend-node",
                                                       "move-node-direction", "move-node-max-direction",
                                                       "move-between-nodes-direction", "move-to-node",
                                                       "extend-node-direction",
                                                       "rotate", "add-border", "mirror", "flip", "insert-output-node",
                                                       "insert-input-node", "fill-rectangle", "hollow-rectangle"]

        if abstraction_key[abs_i] == "multicolor_connect_component_8" or abstraction_key[
            abs_i] == "multicolor_connect_component_4":
            pruned_actions[abstraction_key[abs_i]] += ["update-color"]

        if no_movements:
            pruned_actions[abstraction_key[abs_i]] += ["move-node", "move-node-direction", "extend-node-direction",
                                                       "move-node-max-direction", "move-between-nodes-direction",
                                                       "move-to-node", "extend-node", "move-node", "add-border",
                                                       "insert-output-node", "insert-input-node", "fill-rectangle",
                                                       "mirror", "rotate", "flip"]

        if same_color:
            pruned_actions[abstraction_key[abs_i]] += ["update-color"]

        if same_size:
            pruned_actions[abstraction_key[abs_i]] += ["insert-output-node", "insert-input-node", "extend-node",
                                                       "add-border", "fill-rectangle", "hollow-rectangle"]

        if not hollow_node_required:
            pruned_actions[abstraction_key[abs_i]] += ["hollow-rectangle"]

        if not fill_node_required:
            pruned_actions[abstraction_key[abs_i]] += ["fill-rectangle"]

        if same_number_object:
            pruned_actions[abstraction_key[abs_i]] += ["add-border", "fill-rectangle",
                                                       "hollow-rectangle"]

        if same_position:
            pruned_actions[abstraction_key[abs_i]] += ["insert-output-node", "insert-input-node", "move-node",
                                                       "extend-node", "move-node", "move-node-direction",
                                                       "extend-node-direction", "move-node-max-direction",
                                                       "move-between-nodes-direction", "move-to-node", "add-border",
                                                       "fill-rectangle", "mirror", "rotate", "flip"]

        if abstraction_key[abs_i] != "none_abstract":
            pruned_actions[abstraction_key[abs_i]] += ["rotate", "flip"]


        if abstraction_key[abs_i] == "black_removed_node_position_4":
            pruned_actions[abstraction_key[abs_i]] += ["mirror", "flip", "rotate", "add-border", "insert-output-node",
                                                       "insert-input-node", "fill-rectangle", "hollow-rectangle"]

        if "multicolor_connect_component" in abstraction_key[abs_i]:
            for i in range(len(pruned_actions[abstraction_key[abs_i]]) - 1, -1, -1):
                if pruned_actions[abstraction_key[abs_i]][i] == "insert-input-node":
                    del pruned_actions[abstraction_key[abs_i]][i]


    return pruned_actions








