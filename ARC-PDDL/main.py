from graph import *
from pddl import *
import json
import os
from tqdm import tqdm
import argparse

abstraction_ops = ["get_color_connect_component_4",
                     "get_color_connect_component_background_removed_4",
                     "get_color_connect_component_background_removed_edge_check_4",
                     "get_non_black_component_graph_4",
                     "get_color_vertical_connect_component_4",
                     "get_multicolor_connect_component_4",
                     "get_color_connect_component_8",
                     "get_color_connect_component_background_removed_8",
                     "get_color_connect_component_background_removed_edge_check_8",
                     "get_non_black_component_graph_8",
                     "get_color_horizontal_connect_component_4",
                     "get_multicolor_connect_component_8",
                     "get_max_rectangle_4",
                     "get_same_connect_component_4",
                     "get_same_connect_component_8",
                   "get_recolor_connect_component_0",
                   "get_none_abstract",
                   "get_black_removed_node_position_4",
                    "get_black_node_4",
                   "get_same_color_node"

                     ]


abstraction_name = ["cc4",
                     "cc4_background_removed_corner_check",
                     "cc4_background_removed_edge_check",
                     "cc4_black_removed",
                     "vcc4_background_removed",
                     "mcc4_black_removed",
                     "cc8",
                     "cc8_background_removed_corner_check",
                     "cc8_background_removed_edge_check",
                     "cc8_black_removed",
                     "hcc4_background_removed",
                     "mcc8_black_removed",
                      "re4",
                    "scc4_background_removed_edge_check",
                    "scc8_background_removed_edge_check",
                    "pixels",
                    "none",
                    "cc4_black_removed_node_position",
                    "black_node_4",
                    "same_color"
                     ]

abstraction_key = ["color_connect_component_4",
                   "color_connect_component_background_removed_4",
                   "color_connect_component_background_removed_edge_check_4",
                   "non_black_component_graph_4",
                   "color_vertical_connect_component_4",
                   "multicolor_connect_component_4",
                   "color_connect_component_8",
                   "color_connect_component_background_removed_8",
                   "color_connect_component_background_removed_edge_check_8",
                   "non_black_component_graph_8",
                   "color_horizontal_connect_component_4",
                   "multicolor_connect_component_8",
                   "max_rectangle_4",
                   "same_connect_component_4",
                   "same_connect_component_8",
                   "recolor_connect_component_0",
                   "none_abstract",
                   "black_removed_node_position_4",
                   "black_node_4",
                   "same_color_node"
                   ]


def mkdir(path):
    isExists = os.path.exists(path)
    if not isExists:
      os.makedirs(path)
    else:
      print(path + ' exist')


def node_eq(node1,node2):
        if node1.size==node2.size and node1.color==node2.color and node1.shape==node2.shape:
            if len(set(zip(node1.p[0],node1.p[1])) & set(zip(node2.p[0],node2.p[1])))==node1.size:
                return True
            else:
                return False
        else:
            return False

def check_repeat_node(node1, node2):
    if len(node1)!=len(node2):
        return False

    repeat=True
    for n1 in node1:
        node_repeat=False
        for n2 in node2:
            if node_eq(n1,n2):
                node_repeat=True
                break
        if not node_repeat:
            repeat=False
            break
    return repeat


def main( arc_problem_path, dir_output, problem_type):

  file=open(problem_type)
  lines=file.readlines()

  for line in tqdm(lines):
      training_input = []
      training_output = []
      testing_input = []
      testing_output = []

      input_json=line.rstrip("\n")

      path = os.path.join(arc_problem_path, input_json)

      file_name_list=[]
      for name in  abstraction_name:
          file_name = os.path.join(dir_output, path.split("/")[-1].split(".")[0],name)
          file_name_list.append(file_name)

          mkdir(file_name + "_training")

          mkdir(file_name + "_testing")

      with open(path) as file:
        fcc_data = json.load(file)
      traning_data = fcc_data["train"]
      testing_data = fcc_data["test"]

      for data in traning_data:
        training_input.append(data["input"])
        training_output.append(data["output"])

      for data in testing_data:
        testing_input.append(data["input"])
        testing_output.append(data["output"])

      graph_input_list=[]
      graph_output_list=[]

      number_input_nodes=0
      number_output_nodes=0
      for i in range(len(traning_data)):
        graph_input = Graph()
        if i!=0:
            graph_input.setNumber(number_input_nodes)
        graph_input.graph_abstract(graph_input, np.array(training_input[i]))
        graph_input_list.append(graph_input)

        graph_out = Graph()
        if i!=0:
            graph_out.setNumber(number_output_nodes)

        graph_out.graph_abstract(graph_out, np.array(training_output[i]))
        graph_output_list.append(graph_out)
        number_input_nodes=graph_input.getNumber()
        number_output_nodes = graph_out.getNumber()

      abstract_dict=defaultdict(list)

      for i in range (len(graph_input_list)):
          for name in abstraction_key:
              abstract_dict[name]+=graph_input_list[i].getNode()[name]

      repeat_abstract=[]
      for i in range(len(abstraction_key)):
        for j in range(i+1, len(abstraction_key)):
            # the duplication checking  is avoided to following abstractions as they contain exclusive predicates or special node definitions
            if abstraction_key[i]=="same_connect_component_4" or  abstraction_key[j]=="same_connect_component_8" \
                    or abstraction_key[j]=="same_connect_component_4" or  abstraction_key[i]=="same_connect_component_8"\
                    or abstraction_key[i]=="pixels_similar" or  abstraction_key[j]=="pixels_similar"\
                    or abstraction_key[i] == "black_removed_node_position_4" or abstraction_key[j] == "black_removed_node_position_4":
                continue

            if abstraction_key[i]=="multicolor_connect_component_4" and  abstraction_key[j]!="multicolor_connect_component_8":
                continue

            if abstraction_key[i]!="multicolor_connect_component_4" and  abstraction_key[j]=="multicolor_connect_component_8":
                continue

            if  abstraction_key[i]=="multicolor_connect_component_8" and  abstraction_key[j]!="multicolor_connect_component_4":
                continue

            if  abstraction_key[i]!="multicolor_connect_component_8" and  abstraction_key[j]=="multicolor_connect_component_4":
                continue

            # check whether the different abstraction generate same PDDL (Remove Duplicates)
            results=check_repeat_node(abstract_dict[abstraction_key[i]],
                                    abstract_dict[abstraction_key[j]])
            if results:
                repeat_abstract.append(abstraction_key[j])

      results = check_repeat_node(abstract_dict["same_connect_component_4"],
                                  abstract_dict["same_connect_component_8"])
      if results:
          repeat_abstract.append("same_connect_component_8")



      for name in repeat_abstract:
        for graph in graph_input_list:
            graph.setNode(name, dict())
        for graph in graph_output_list:
            graph.setNode(name, dict())
      # Action Constraints Implementation
      pruned_action=pruned_actions(graph_input_list,graph_output_list,abstraction_key)
      predicates_dict_list=[]


      for i in range(len(graph_input_list)):
          # instance file generation
          predicates_dict=pddl_generate_traning(graph_input_list[i], graph_output_list[i],i,file_name_list,repeat_abstract)
          predicates_dict_list.append(predicates_dict)

      for i in range(len(file_name_list)):
          if abstraction_key[i] in repeat_abstract:
              continue
          pruned_action_abstract=pruned_action[abstraction_key[i]]
          predicates_list=[]

          for dic_i in range (len(predicates_dict_list)):
              predicates_list.append(predicates_dict_list[dic_i][abstraction_key[i]])

          # generate the domain file for traning
          if "scc8" in file_name_list[i] or "scc4" in file_name_list[i]:
              domain_same(file_name_list[i] + "_" + "training",predicates_list,pruned_action_abstract)
          elif "cc4_black_removed_node_position" in file_name_list[i] :
              domain_node_position(file_name_list[i] + "_" + "training",predicates_list,pruned_action_abstract)
          else:
              domain(file_name_list[i] + "_" + "training",predicates_list,pruned_action_abstract)
      test_number_input_nodes=0
      test_number_output_nodes=0

      #the same work for output
      for i in range(len(testing_data)):

          graph_input = Graph()
          if i!=0:
              graph_input.setNumber(test_number_input_nodes)
          graph_input.graph_abstract(graph_input, np.array(testing_input[i]))

          graph_out = Graph()
          if i!=0:
              graph_out.setNumber(test_number_output_nodes)
          graph_out.graph_abstract(graph_out, np.array(testing_output[i]))

          predicates_test=pddl_generate_testing(graph_input, graph_out, i,file_name_list,repeat_abstract)
          test_number_input_nodes = graph_input.getNumber()
          test_number_output_nodes=graph_out.getNumber()

      for i in range(len(file_name_list)):

          if abstraction_key[i] in repeat_abstract:
              continue
          pruned_action_abstract=pruned_action[abstraction_key[i]]
          predicates_test_abs=predicates_test[abstraction_key[i]]

          # we generate the domain for training and testing at the same time, while they are the same. The other way is copying the domain from traning
          if "scc8" in file_name_list[i] or "scc4" in file_name_list[i]:
              domain_same(file_name_list[i] + "_" + "testing",predicates_test_abs,pruned_action_abstract)
          elif "cc4_black_removed_node_position" in file_name_list[i] :
              domain_node_position(file_name_list[i] + "_" + "testing",predicates_test_abs,pruned_action_abstract)
          else:
              domain(file_name_list[i] + "_" + "testing",predicates_test_abs,pruned_action_abstract)



if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--problem', type=str, help='*REQUIRED* path where ARC problem saved')
    parser.add_argument('--out_dir', type=str, help='*REQUIRED* directory for saving  generated PDDL')
    parser.add_argument('--type', type=str, help='*REQUIRED* type of ARC problem: recolor, movement, augmentation')

    args = parser.parse_args()
    arc_problem_path = args.problem
    dir_output = args.out_dir
    problem_type = args.type

    main(arc_problem_path, dir_output, problem_type)
