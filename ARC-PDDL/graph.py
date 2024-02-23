import copy
import itertools
import sys
from collections import defaultdict
import cc3d
import numpy as np
from node import *
from collections import Counter
import largestinteriorrectangle as lir
import re_detect

class Graph:

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
                       "get_same_color_node",
                       ]


    abstraction_select_task = ["get_color_connect_component_4",
                     "get_color_connect_component_background_removed_4",
                     "get_color_connect_component_background_removed_edge_check_4",
                     "get_non_black_component_graph_4",
                     "get_color_connect_component_8",
                     "get_color_connect_component_background_removed_8",
                     "get_color_connect_component_background_removed_edge_check_8",
                     "get_non_black_component_graph_8",
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




    def __init__(self):

        self.n = defaultdict(list)
        self.size=tuple()
        self.node_number=0
        self.image=np.array(0)
        self.background_color=0
        self.corners=set()
        self.most_color=-1
        self.least_color=-1
        self.pixel_similar=np.array(0)


    def setNode(self, name, nodes):
        self.n[name]=nodes


    def setLen(self,tuple):
        self.size=tuple

    def setImage(self,image):
        self.image=image
        self.setLen(self.image.shape)

        # find background color and colors with most appearance and least appearance

        self.setBackGround(image)
        self.setCorners(self.size)

        self.pixel_similar=copy.deepcopy(image)


    def setCorners(self,size):

        self.corners.add((0,0))
        self.corners.add((0,size[1]-1))
        self.corners.add((size[0]-1,size[1]-1))
        self.corners.add((size[0]-1,0))



    def getImage(self):
        return self.image

    def setBackGround(self,image):

        count=np.bincount(image.flatten())

        # if color black not exist, take the most frequent color are background
        if count[0]==0:
            self.background_color=np.argmax(count)

        count=count[1:]
        if len(count)==0:
            return

        # find the color with most appearance and least appearance
        most_color_number=count[np.argmax(count)]

        appear_times=0

        for i in count:
            if i==most_color_number:
                appear_times+=1

        if appear_times==1:
            self.most_color = np.argmax(count)+1

        least_color=-1
        least_number=1000

        for i in range(len(count)):
            if count[i]==0:
                continue
            if count[i]<least_number:
                least_color=i
                least_number=count[i]

        appear_times=0

        for i in count:
            if i==least_number:
                appear_times+=1


        if appear_times==1:
            self.least_color = least_color+1

        # if colors with most appearance and least appearance are the same we set both -1 representing most appearance and least appearance false.
        if self.most_color==self.least_color:
            self.most_color=-1
            self.least_color=-1


    def setNumber(self,number):
        self.node_number=number

    def getNumber(self):
        return self.node_number

    def getNode(self):
        return  self.n


    def find_relationship_4(self,node1, node2):
        self.hor_ver(node1, node2)

        self.adjacent_node(node1, node2)

        self.contain_node(node1, node2)

        self.same_node(node1,node2)

        self.spatial_realtions(node1,node2)

        self.same_color(node1, node2)

    def find_relationship_8(self,node1, node2):

        self.hor_ver(node1, node2)

        self.contain_node(node1, node2)

        self.same_node(node1,node2)

        self.spatial_realtions(node1,node2)

        self.same_color(node1, node2)


    def shape(self, node):

        # if node.color == 0:
        #     return

        if node.size == 1:
            node.setShape("single-pixel")
            return

        if len(set(node.p[0])) == 1:
            node.setShape("horizontal-line")
            return

        if len(set(node.p[1])) == 1:
            node.setShape("vertical-line")
            return

        if self.diagonal(node):
            return

        if self.rectangle(node):
            return

        node.setShape("unknown")

    def diagonal(self,node):

        seed_x = node.p[0][0]
        seed_y = node.p[1][0]
        x = seed_x
        y = seed_y

        diagonal_left = [(seed_x, seed_y)]
        diagonal_right = [(seed_x, seed_y)]

        while x <= self.size[0] and y <= self.size[1]:
            x = x + 1
            y = y + 1
            diagonal_right.append((x, y))

        x = seed_x
        y = seed_y

        while x >= 0 and y >= 0:
            x = x - 1
            y = x - 1
            diagonal_right = [(x, y)] + diagonal_right

        x = seed_x
        y = seed_y
        while x <= self.size[0] and y >= 0:
            x = x + 1
            y = y - 1
            diagonal_left.append((x, y))

        x = seed_x
        y = seed_y
        while x >= 0 and y <= self.size[1]:
            x = x - 1
            y = y + 1
            diagonal_left = [(x, y)] + diagonal_left

        right_flag = False

        left_flag = False

        for i in range(1, len(node.p[0])):
            if (node.p[0][i], node.p[1][i]) not in diagonal_right:
                right_flag = True
                break

        for i in range(1, len(node.p[0])):
            if (node.p[0][i], node.p[1][i]) not in diagonal_left:
                left_flag = True
                break

        if not right_flag:
            node.setShape("diagonal_right")

        if not left_flag:
            node.setShape("diagonal_left")

        return (not right_flag or not left_flag)

    def get_possible_edge(self,p):

            x_min = 100
            y_min = 100
            x_max = -1
            y_max = -1

            for i in range(len(p[0])):
                if p[0][i] <= x_min:
                    x_min = p[0][i]
                if p[0][i] >= x_max:
                    x_max = p[0][i]

            for i in range(len(p[1])):
                if p[1][i] <= y_min:
                    y_min = p[1][i]
                if p[1][i] >= y_max:
                    y_max = p[1][i]
            return x_min, x_max, y_min, y_max

    def rectangle(self,node):

        x_min,x_max,y_min,y_max=self.get_possible_edge(node.p)

        node_list_x_min = []
        node_list_x_max = []
        node_list_y_min = []
        node_list_y_max = []


        for i in range(len(node.p[0])):
            if node.p[0][i] == x_min:
                node_list_x_min.append((node.p[0][i], node.p[1][i]))
            if node.p[0][i] == x_max:
                node_list_x_max.append((node.p[0][i], node.p[1][i]))

        for i in range(len(node.p[0])):
            if node.p[1][i] == y_min:
                node_list_y_min.append((node.p[0][i], node.p[1][i]))
            if node.p[1][i] == y_max:
                node_list_y_max.append((node.p[0][i], node.p[1][i]))


        if len(node_list_x_min)==1 or len(node_list_x_max)==1 or len(node_list_y_min)==1 or len(node_list_y_max)==1:
            return False


        if len(node_list_x_min) == len(node_list_x_max) and len(node_list_y_min) == len(node_list_y_max):

            boundary = list(set(node_list_y_max + node_list_y_min + node_list_x_min + node_list_x_min))

            if len(node_list_x_min) == len(node_list_y_min):

                node.setShape("square", boundary=boundary)
            else:
                node.setShape("rectangle", boundary=boundary)
            return True
        return False

    def hor_ver(self,node1, node2):
        max_row1 = np.max(node1.p[0])
        min_row1 = np.min(node1.p[0])
        max_column1 = np.max(node1.p[1])
        min_column1 = np.min(node1.p[1])

        max_row2 = np.max(node2.p[0])
        min_row2 = np.min(node2.p[0])
        max_column2 = np.max(node2.p[1])
        min_column2 = np.min(node2.p[1])

        if (max_row1 >= min_row2 and max_row1 <= max_row2) or (max_row2 >= min_row1 and max_row2 <= max_row1):
            node1.setHor(node2)
            node2.setHor(node1)

        if (max_column1 >= min_column2 and max_column1 <= max_column2) or (
                max_column2 >= min_column1 and max_column2 <= max_column1):
            node1.setVer(node2)
            node2.setVer(node1)

    def spatial_realtions(self,node1,node2):

        max_row1 = np.max(node1.p[0])
        min_row1 = np.min(node1.p[0])
        max_column1 = np.max(node1.p[1])
        min_column1 = np.min(node1.p[1])

        max_row2 = np.max(node2.p[0])
        min_row2 = np.min(node2.p[0])
        max_column2 = np.max(node2.p[1])
        min_column2 = np.min(node2.p[1])

        if (max_row1 >= min_row2 and max_row1 <= max_row2) or (max_row2 >= min_row1 and max_row2 <= max_row1):
            if (max_column1<min_column2):
                node1.setRight(node2)
            if (min_column1>max_column2):
                node1.setLeft(node2)

            if (max_column2<min_column1):
                node2.setRight(node1)
            if (min_column2>max_column1):
                node2.setLeft(node1)

        if (max_column1 >= min_column2 and max_column1 <= max_column2) or (
                max_column2 >= min_column1 and max_column2 <= max_column1):
            if (max_row1<min_row2):
                node1.setDown(node2)
            if (min_row1>max_row2):
                node1.setUp(node2)
            if (max_row2<min_row1):
                node2.setDown(node1)
            if (min_row2>max_row1):
                node2.setUp(node1)

        if node1.shape=="single-pixel" and node2.shape=="single-pixel":
            dx=1
            dy=1
            x=node1.p[0][0]
            y=node1.p[1][0]
            while x>=0 and y>=0 and x<=self.image.shape[0]-1 and x<=self.image.shape[1]-1:
                x+=dx
                y+=dy
                if x in node2.p[0] and y in node2.p[1]:
                    node1.setRightDown(node2)
                    node2.setLeftUp(node1)

            dx=-1
            dy=-1
            x=node1.p[0][0]
            y=node1.p[1][0]
            while x>=0 and y>=0 and x<=self.image.shape[0]-1 and x<=self.image.shape[1]-1:
                x+=dx
                y+=dy
                if x in node2.p[0] and y in node2.p[1]:
                    node1.setLeftUp(node2)
                    node2.setRightDown(node1)

            dx=-1
            dy=1
            x=node1.p[0][0]
            y=node1.p[1][0]
            while x>=0 and y>=0 and x<=self.image.shape[0]-1 and x<=self.image.shape[1]-1:
                x+=dx
                y+=dy
                if x in node2.p[0] and y in node2.p[1]:
                    node1.setRightUp(node2)
                    node2.setLeftDown(node1)

            dx=1
            dy=-1
            x=node1.p[0][0]
            y=node1.p[1][0]
            while x>=0 and y>=0 and x<=self.image.shape[0]-1 and x<=self.image.shape[1]-1:
                x+=dx
                y+=dy
                if x in node2.p[0] and y in node2.p[1]:
                    node1.setLeftDown(node2)
                    node2.setRightUp(node1)


        if (node1.shape=="square" or node1.shape=="rectangle") and node2.shape=="single-pixel":


            min_x=min(node1.p[0])
            max_x=max(node1.p[0])
            min_y=min(node1.p[1])
            max_y=max(node1.p[1])

            leftup_x=copy.copy(min_x)
            leftup_y=copy.copy(min_y)

            dx=-1
            dy=-1
            x=leftup_x
            y=leftup_y
            while x>=0 and y>=0 and x<=self.image.shape[0]-1 and x<=self.image.shape[1]-1:
                x+=dx
                y+=dy
                if x in node2.p[0] and y in node2.p[1]:
                    node1.setLeftUp(node2)
                    node2.setRightDown(node1)

            leftdown_x=copy.copy(max_x)
            leftdonw_y=copy.copy(min_y)

            dx=1
            dy=-1
            x=leftdown_x
            y=leftdonw_y
            while x>=0 and y>=0 and x<=self.image.shape[0]-1 and x<=self.image.shape[1]-1:
                x+=dx
                y+=dy
                if x in node2.p[0] and y in node2.p[1]:
                    node1.setLeftDown(node2)
                    node2.setRightUp(node1)

            rightup_x=copy.copy(min_x)
            rightup_y=copy.copy(max_y)

            dx=-1
            dy=1
            x=rightup_x
            y=rightup_y
            while x>=0 and y>=0 and x<=self.image.shape[0]-1 and x<=self.image.shape[1]-1:
                x+=dx
                y+=dy
                if x in node2.p[0] and y in node2.p[1]:
                    node1.setRightUp(node2)
                    node2.setLeftDown(node1)

            rightdown_x=copy.copy(max_x)

            rightdown_y = copy.copy(max_y)

            dx=1
            dy=1
            x=rightdown_x
            y=rightdown_y
            while x>=0 and y>=0 and x<=self.image.shape[0]-1 and x<=self.image.shape[1]-1:
                x+=dx
                y+=dy
                if x in node2.p[0] and y in node2.p[1]:
                    node1.setRightDown(node2)
                    node2.setLeftUp(node1)


        if (node2.shape == "square" or node2.shape == "rectangle") and node1.shape == "single-pixel":

            min_x = min(node2.p[0])
            max_x = max(node2.p[0])
            min_y = min(node2.p[1])
            max_y = max(node2.p[1])

            leftup_x = copy.copy(min_x)
            leftup_y = copy.copy(min_y)

            dx = -1
            dy = -1
            x = leftup_x
            y = leftup_y
            while x >= 0 and y >= 0 and x <= self.image.shape[0] - 1 and x <= self.image.shape[1] - 1:
                x += dx
                y += dy
                if x in node1.p[0] and y in node1.p[1]:
                    node2.setLeftUp(node1)
                    node1.setRightDown(node2)

            leftdown_x = copy.copy(max_x)
            leftdonw_y = copy.copy(min_y)

            dx = 1
            dy = -1
            x = leftdown_x
            y = leftdonw_y
            while x >= 0 and y >= 0 and x <= self.image.shape[0] - 1 and x <= self.image.shape[1] - 1:
                x += dx
                y += dy
                if x in node1.p[0] and y in node1.p[1]:
                    node2.setLeftDown(node1)
                    node1.setRightUp(node2)

            rightup_x = copy.copy(min_x)
            rightup_y = copy.copy(max_y)

            dx = -1
            dy = 1
            x = rightup_x
            y = rightup_y
            while x >= 0 and y >= 0 and x <= self.image.shape[0] - 1 and x <= self.image.shape[1] - 1:
                x += dx
                y += dy
                if x in node1.p[0] and y in node1.p[1]:
                    node2.setRightUp(node1)
                    node1.setLeftDown(node2)

            rightdown_x = copy.copy(max_x)

            rightdown_y = copy.copy(max_y)

            dx = 1
            dy = 1
            x = rightdown_x
            y = rightdown_y
            while x >= 0 and y >= 0 and x <= self.image.shape[0] - 1 and x <= self.image.shape[1] - 1:
                x += dx
                y += dy
                if x in node1.p[0] and y in node1.p[1]:
                    node2.setRightDown(node1)
                    node1.setLeftUp(node2)







    def adjacent_node(self,node1, node2):
        dx = [1, -1, 0, 0, -1, 1, 1, -1]
        dy = [0, 0, 1, -1, -1, 1, -1, 1]
        for i in range(len(node1.p[0])):
            x = node1.p[0][i]
            y = node1.p[1][i]
            for j in range(8):
                a = x + dx[j]
                b = y + dy[j]
                if a in node2.p[0] and b in node2.p[1]:
                    node1.setCon(node2)
                    node2.setCon(node1)
                    return


    def contain_node_single(self,node1, node2):

        dx = [1, -1, 0, 0, -1, 1, 1, -1]
        dy = [0, 0, 1, -1, -1, 1, -1, 1]

        p1s=list(zip(node1.p[0], node1.p[1]))
        p2s = list(zip(node2.p[0], node2.p[1]))

        node2_x_min = min(node2.p[0])
        node2_y_min = min(node2.p[1])

        node2_x_max = max(node2.p[0])
        node2_y_max = max(node2.p[1])

        node1_x_min = min(node1.p[0])
        node1_y_min = min(node1.p[1])

        node1_x_max = max(node1.p[0])
        node1_y_max = max(node1.p[1])


        ## check size
        flag = False



        if (node1_x_min!=0 and node2_x_min!=0 and node1_y_min!=0 and node2_y_min!=0 and
           node1_x_max!=self.image.shape[0]-1 and node2_x_max!=self.image.shape[0]-1 and node1_y_max!=self.image.shape[1]-1 and node2_y_max!=self.image.shape[1]-1):

            if (node2_x_min <= node1_x_min) or (node2_x_max >= node1_x_max) or (node2_y_min <= node1_y_min) or (
                    node2_y_max >= node1_y_max):
                flag = True

        if len(node1.p[0])<len(node2.p[0]) or len(node1.p[1])<len(node2.p[1]):
            flag=True

        for i in range(len(node2.p[0])):
            x = node2.p[0][i]
            y = node2.p[1][i]
            for j in range(8):
                a = x + dx[j]
                b = y + dy[j]
                if a < 0 or b < 0 or a > self.size[0]-1 or b > self.size[1]-1:
                    continue
                if not ((a in node2.p[0] or a in node1.p[0]) and (b in node2.p[1] or b in node1.p[1])):
                    flag = True
                    break
            if flag:
                break

        if not flag:
            if self.node_cycles(node1):
                node1.setContain(node2)
            else:
                node1.setInContain(node2)

    # return the possible color may update given a pixel based on 4-connected rule ([[0, 1], [1, 0], [-1, 0], [0, -1]]), 4-connected rule with one step further  [[0, 1],[0, 2], [1, 0],[2, 0], [-1, 0],[-2, 0], [0, -1],[0, -2]], or 8-connected rule [[0, 1], [1, 0], [-1, 0], [0, -1],[1, 1], [1, -1], [-1, 1], [-1, -1]]

    def get_pixel_information(self, change_pixel):

        check_again=[]
        background_removed=False
        if self.background_color==0:
            background_removed=True

        for i in range(self.pixel_similar.shape[0]):
            for j in range(self.pixel_similar.shape[1]):
                if len(change_pixel)==0:
                    if background_removed and self.pixel_similar[i][j]==0:
                        continue
                if len(change_pixel)!=0:
                    if (i,j) not in change_pixel and background_removed and self.pixel_similar[i][j]==0:
                        continue
                color_record=dict()
                same_color=[]
                direction=[[0, 1], [1, 0], [-1, 0], [0, -1]]
                for dx, dy in direction:
                    x, y = i + dx, j + dy
                    if  x<0 or x>self.image.shape[0]-1 or y<0 or y>self.image.shape[1]-1:
                        continue
                    color_record[(x, y)] = self.pixel_similar[x][y]
                color=list(color_record.values())
                counter_color=Counter(color)
                max_time = max(counter_color.values())
                for key,value in counter_color.items():
                    if value==max_time:
                        same_color.append(key)
                if len(same_color)==1:
                    if same_color[0] !=self.pixel_similar[i][j]:
                        change_pixel.append((i,j))
                    self.pixel_similar[i][j]=same_color[0]

                else:
                    same_color_copy=copy.deepcopy(same_color)

                    for index in range (len(same_color_copy)-1,-1,-1):
                         if same_color_copy[index]==0:
                             del same_color_copy[index]
                    if len(same_color_copy)==1:
                        if self.pixel_similar[i][j] != same_color_copy[0]:
                            change_pixel.append((i, j))
                        self.pixel_similar[i][j] = same_color_copy[0]
                    else:
                        check_again.append((i,j))

        check_again_again=[]

        for locations in check_again:
            i,j=locations[0],locations[1]
            color_record=dict()
            same_color=[]
            direction=[[0, 1],[0, 2], [1, 0],[2, 0], [-1, 0],[-2, 0], [0, -1],[0, -2]]
            for dx, dy in direction:
                x, y = i + dx, j + dy
                if  x<0 or x>self.image.shape[0]-1 or y<0 or y>self.image.shape[1]-1:
                     continue
                color_record[(x, y)] = self.pixel_similar[x][y]


            color = list(color_record.values())
            counter_color = Counter(color)
            max_time = max(counter_color.values())
            for p, c in counter_color.items():
                if c == max_time:
                    same_color.append(p)
            if len(same_color) == 1:
                if self.pixel_similar[i][j]!=same_color[0]:
                    change_pixel.append((i, j))
                self.pixel_similar[i][j] = same_color[0]

            else:
                same_color_copy = copy.deepcopy(same_color)

                for index in range(len(same_color_copy) - 1, -1, -1):
                    if same_color_copy[index] == 0:
                        del same_color_copy[index]
                if len(same_color_copy) == 1:
                    if self.pixel_similar[i][j] != same_color_copy[0]:
                        change_pixel.append((i, j))
                    self.pixel_similar[i][j] = same_color_copy[0]
                else:
                    check_again_again.append((i,j))


        for locations in check_again_again:
            i,j=locations[0],locations[1]
            color_record=dict()
            same_color=[]
            direction=[[0, 1], [1, 0], [-1, 0], [0, -1],[1, 1], [1, -1], [-1, 1], [-1, -1]]
            for dx, dy in direction:
                x, y = i + dx, j + dy
                if  x<0 or x>self.image.shape[0]-1 or y<0 or y>self.image.shape[1]-1:
                     continue
                color_record[(x, y)] = self.pixel_similar[x][y]

            color = list(color_record.values())
            counter_color = Counter(color)
            max_time = max(counter_color.values())
            for p, c in counter_color.items():
                if c == max_time:
                    same_color.append(p)
            if len(same_color) == 1:
                if self.pixel_similar[i][j]!=same_color[0]:
                    change_pixel.append((i, j))
                self.pixel_similar[i][j] = same_color[0]

            else:
                same_color_copy = copy.deepcopy(same_color)

                for index in range(len(same_color_copy) - 1, -1, -1):
                    if same_color_copy[index] == 0:
                        del same_color_copy[index]
                if len(same_color_copy) == 1:
                    if self.pixel_similar[i][j] != same_color_copy[0]:
                        change_pixel.append((i, j))
                    self.pixel_similar[i][j] = same_color_copy[0]
                else:
                    change_pixel.append((i,j))
                    print("error for color similar")



        return change_pixel






    def contain_node(self,node1, node2):

        dx = [1, -1, 0, 0, -1, 1, 1, -1]
        dy = [0, 0, 1, -1, -1, 1, -1, 1]
        flag = False

        p1s = list(zip(node1.p[0], node1.p[1]))
        p2s = list(zip(node2.p[0], node2.p[1]))

        node2_x_min=min(node2.p[0])
        node2_y_min = min(node2.p[1])

        node2_x_max=max(node2.p[0])
        node2_y_max = max(node2.p[1])

        node1_x_min=min(node1.p[0])
        node1_y_min = min(node1.p[1])

        node1_x_max=max(node1.p[0])
        node1_y_max = max(node1.p[1])


        if (node1_x_min == node2_x_min) and (node1_x_max >= node2_x_max):
            flag = True

        if (node1_y_min == node2_y_min) and (node1_y_max >= node2_y_max):
            flag = True

        if (node1_x_max == node2_x_max) and (node1_x_min <= node2_x_min):
            flag = True

        if (node1_y_max == node2_y_max) and (node1_y_min <= node2_y_min):
            flag = True

        if (node1_x_min < node2_x_min) or (node1_x_max > node2_x_max) or (node1_y_min < node2_y_min) or (
                node1_y_max > node2_y_max):
            flag = True

        if len(node2.p[0])<len(node1.p[0]) or len(node2.p[1])<len(node1.p[1]):
            flag=True


        for i in range(len(node1.p[0])):
            x = node1.p[0][i]
            y = node1.p[1][i]
            for j in range(8):
                a = x + dx[j]
                b = y + dy[j]
                if a < 0 or b < 0 or a > self.size[0]-1 or b > self.size[1]-1:
                    continue
                if not ((a in node2.p[0] or a in node1.p[0]) and (b in node2.p[1] or b in node1.p[1])):
                    flag = True
                    break
            if flag:
                break

        if not flag:
            if self.node_cycles(node2):
                node2.setContain(node1)
            else:
                node2.setInContain(node1)


        flag = False



        if (node2_x_min==node1_x_min) and (node2_x_max>=node1_x_max):
            flag = True

        if (node2_y_min==node1_y_min) and (node2_y_max>=node1_y_max):
            flag = True

        if (node2_x_max == node1_x_max) and (node2_x_min <= node1_x_min):
            flag = True

        if (node2_y_max == node1_y_max) and (node2_y_min <= node1_y_min):
            flag = True


        if (node2_x_min < node1_x_min) or (node2_x_max > node1_x_max) or (node2_y_min < node1_y_min) or (
                node2_y_max > node1_y_max):
             flag=True



        if len(node1.p[0])<len(node2.p[0]) or len(node1.p[1])<len(node2.p[1]):
            flag=True


        for i in range(len(node2.p[0])):
            x = node2.p[0][i]
            y = node2.p[1][i]
            for j in range(8):
                a = x + dx[j]
                b = y + dy[j]
                if a < 0 or b < 0 or a > self.size[0]-1 or b > self.size[1]-1:
                    continue
                if not ((a in node2.p[0] or a in node1.p[0]) and (b in node2.p[1] or b in node1.p[1])):
                    flag = True
                    break
            if flag:
                break

        if not flag:
            if self.node_cycles(node1):
                node1.setContain(node2)
            else:
                node1.setInContain(node2)


    def same_color(self,node1,node2):
        co1=node1.color
        co2=node2.color
        if co1==co2:
            node1.setSameColor(node2)
            node2.setSameColor(node1)



    def same_node(self,node1,node2):
        p1=node1.p
        p2=node2.p

        x1_min=min(p1[0])
        x1_max=max(p1[0])

        y1_min=min(p1[1])
        y1_max=max(p1[1])

        x2_min = min(p2[0])
        x2_max = max(p2[0])

        y2_min = min(p2[1])
        y2_max = max(p2[1])

        p1s=list(zip(node1.p[0], node1.p[1]))
        p2s= list(zip(node2.p[0], node2.p[1]))

        x1_different=x1_max-x1_min
        y1_different=y1_max-y1_min

        x2_different=x2_max-x2_min
        y2_different=y2_max-y2_min

        same=True

        if len(p1[0])==len(p2[0]) and len(p1[1])==len(p2[1]):
            if x1_different==x2_different and y1_different==y2_different:
                for i in range (0,x1_different+1):
                    for j in range (0,y1_different+1):
                        if (i+x1_min,j+y1_min) in p1s and (i+x2_min,j+y2_min) in p2s:
                            continue
                        if (i+x1_min,j+y1_min) not in p1s and (i+x2_min,j+y2_min) not in p2s:
                            continue
                        same=False
            else:
                same=False
        else:
            same=False

        if same:
            node1.setSame(node2)
            node2.setSame(node1)


    # abstraction with pixel level
    def get_recolor_connect_component_0(self,im_data):

        color_cc_list = []


        diagonal_list_left = []

        diagonal_list_right = []


        i=0
        j=0
        dx=1
        dy=1

        while i<=self.image.shape[0] and j<self.image.shape[1]:
            diagonal_list_right.append((i,j))
            i+=dx
            j+=dy

        i=0
        j=self.image.shape[1]-1
        dx=1
        dy=-1


        while i<=self.image.shape[0] and j>=0:
            diagonal_list_left.append((i,j))
            i+=dx
            j+=dy




        for x in range (self.image.shape[0]):
            for y in range (self.image.shape[1]):
                node = Node()
                node_id = self.getNumber()+1
                node.setID(node_id)
                self.setNumber(node_id)
                c = self.image[x][y]
                node.setNode([[x],[y]])
                node.setColor(c)
                similar_c=self.pixel_similar[x][y]
                node.setSimilarColor(similar_c)
                color_cc_list.append(node)
                if (x,y) in diagonal_list_left:
                    node.setLeftDiagonal()
                if (x,y) in diagonal_list_right:
                    node.setRightDiagonal()
                if x==0:
                    node.setUpBorder()
                if x==im_data.shape[0]-1:
                    node.setDownBorder()
                if y==0:
                    node.setLeftBorder()
                if y==im_data.shape[1]-1:
                    node.setRightBorder()
                if self.image.shape[0]%2==1 and x==self.image.shape[0]//2:
                    node.setHorizontalMiddle()
                if self.image.shape[1]%2==1 and y==self.image.shape[1]//2:
                    node.setVerticalMiddle()





        self.setNode("recolor_connect_component_0", color_cc_list)

        return color_cc_list



    def get_color_connect_component_4(self,im_data):

        connectivity = 4
        im_data[im_data == 0] = 10
        labels_out = cc3d.connected_components(im_data, connectivity=connectivity)
        label_list=list(set(list(labels_out.flatten())))
        color_cc_list = []
        im_data[im_data == 10] =0
        max_size=0
        min_size=100

        size_list=[]
        color_list=[]

        for label in label_list:
            node = Node()
            node_id = self.getNumber()+1
            node.setID(node_id)
            self.setNumber(node_id)
            p = list(np.where(labels_out == label))
            c = im_data[p[0][0], p[1][0]]
            node.setNode(p)
            node.setColor(c)
            self.shape(node)
            color_cc_list.append(node)

            size_list.append(node.size)
            color_list.append(node.color)

            if node.size>max_size :
                max_size=node.size

            if node.size<min_size:
                min_size=node.size



        len_size=len(list(set(size_list)))

        color_least=self.get_color_least(color_cc_list)

        color_most=self.get_color_most(color_cc_list)

        if not (set(color_least).intersection(set(color_most))):
            self.set_color_least_node(color_cc_list, color_least)
            self.set_color_most_node(color_cc_list,color_most)



        for node in color_cc_list:
            if len_size!=1 and node.size==max_size:
                node.setMaxSize()

            if len_size!=1 and node.size==min_size:
                node.setMinSize()




        for i in range(0, len(color_cc_list) - 1):
            for j in range(i + 1, len(color_cc_list)):
                if color_cc_list[i].color==self.background_color or color_cc_list[j].color==self.background_color:
                    continue
                self.find_relationship_4(color_cc_list[i], color_cc_list[j])

        for i in range(0, len(color_cc_list) - 1):
            for j in range(i + 1, len(color_cc_list)):
                if color_cc_list[i].color==self.background_color or color_cc_list[j].color!=self.background_color:
                    continue
                self.contain_node_single(color_cc_list[i], color_cc_list[j])


        self.setNode("color_connect_component_4", color_cc_list)

        return color_cc_list

    def get_max_rectangle_4(self,im_data):
        connectivity = 4
        im_data[im_data == 0] = 10
        labels_out = cc3d.connected_components(im_data, connectivity=connectivity)
        im_data[im_data == 10] = 0
        label_list = list(set(list(labels_out.flatten())))
        color_cc_list = []

        max_size = 0
        min_size = 100

        size_list=[]

        color_list=[]
        for lable in label_list:
            label_tmp = np.zeros(labels_out.shape)
            label_tmp[labels_out==lable]=1
            label_tmp=np.array(label_tmp,"bool")

            result = re_detect.largest_interior_rectangle(label_tmp)



            if result[0][0]==99 and result[0][1]==99:
                continue

            p = [[], []]
            for i in range(result[0][1], result[0][1] + result[0][3]):
                for j in range(result[0][0], result[0][0] + result[0][2]):
                    p[0].append(i)
                    p[1].append(j)


            c = im_data[p[0][0], p[1][0]]



            if c == self.background_color:
                continue

            node = Node()
            node_id = self.getNumber() + 1
            node.setID(node_id)
            self.setNumber(node_id)
            node.setColor(c)
            node.setNode(p)
            self.shape(node)
            color_cc_list.append(node)
            size_list.append(node.size)
            color_list.append(node.color)

            if node.size > max_size:
                max_size = node.size

            if node.size < min_size:
                min_size = node.size

            p_zip=list(zip(p[0],p[1]))

            p_original = list(np.where(labels_out == lable))

            p_original_zip=list(zip(list(p_original[0]),list(p_original[1])))

            p_new=[[],[]]
            for p in p_original_zip:
                if p not in p_zip:
                    p_new[0].append(p[0])
                    p_new[1].append(p[1])

            if len(p_new[0])!=0:

                c = im_data[p_new[0][0], p_new[1][0]]

                if c == self.background_color:
                    continue

                node = Node()
                node_id = self.getNumber() + 1
                node.setID(node_id)
                self.setNumber(node_id)
                node.setColor(c)
                node.setNode(p_new)
                self.shape(node)
                color_cc_list.append(node)
                size_list.append(node.size)
                color_list.append(node.color)

                if node.size > max_size:
                    max_size = node.size

                if node.size < min_size:
                    min_size = node.size





        len_size=len(list(set(size_list)))

        color_least=self.get_color_least(color_cc_list)

        color_most=self.get_color_most(color_cc_list)

        if not (set(color_least).intersection(set(color_most))):
            self.set_color_least_node(color_cc_list, color_least)
            self.set_color_most_node(color_cc_list,color_most)


        for node in color_cc_list:
            if len_size!=1 and node.size == max_size:
                node.setMaxSize()

            if len_size!=1 and node.size == min_size:
                node.setMinSize()


        for i in range(0, len(color_cc_list) - 1):
            for j in range(i + 1, len(color_cc_list)):
                self.find_relationship_4(color_cc_list[i], color_cc_list[j])


        self.setNode("max_rectangle_4", color_cc_list)



        return color_cc_list

    def get_same_connect_component_4(self,im_data):

        connectivity = 4
        im_data[im_data == 0] = 10
        labels_out = cc3d.connected_components(im_data, connectivity=connectivity)
        label_list = list(set(list(labels_out.flatten())))
        color_cc_list = []
        im_data[im_data == 10] = 0
        max_size = 0
        min_size = 100

        size_list=[]

        color_list=[]

        for label in label_list:

            p = list(np.where(labels_out == label))

            c = im_data[p[0][0], p[1][0]]

            x_min, x_max, y_min, y_max = self.get_possible_edge(p)

            if c == self.background_color and (x_min==0 or x_max==self.image.shape[0]-1 or y_min==0 or y_max==self.image.shape[1]-1):
                continue

            node = Node()
            node_id = self.getNumber() + 1
            node.setID(node_id)
            self.setNumber(node_id)

            node.setNode(p)
            node.setColor(c)
            self.shape(node)
            color_cc_list.append(node)

            size_list.append(node.size)
            color_list.append(node.color)

            if node.size > max_size:
                max_size = node.size

            if node.size < min_size:
                min_size = node.size




        len_size=len(list(set(size_list)))

        color_least=self.get_color_least(color_cc_list)

        color_most=self.get_color_most(color_cc_list)

        if not (set(color_least).intersection(set(color_most))):
            self.set_color_least_node(color_cc_list, color_least)
            self.set_color_most_node(color_cc_list,color_most)



        for node in color_cc_list:
            if len_size!=1 and node.size == max_size:
                node.setMaxSize()

            if len_size!=1 and node.size == min_size:
                node.setMinSize()



        for i in range(0, len(color_cc_list) - 1):
            for j in range(i + 1, len(color_cc_list)):
                self.same_node(color_cc_list[i],color_cc_list[j])
                if color_cc_list[i].color == self.background_color or color_cc_list[j].color == self.background_color:
                    continue
                self.find_relationship_4(color_cc_list[i], color_cc_list[j])

        for i in range(0, len(color_cc_list) - 1):
            for j in range(i + 1, len(color_cc_list)):
                if color_cc_list[i].color==self.background_color or color_cc_list[j].color!=self.background_color:
                    continue
                self.contain_node_single(color_cc_list[i], color_cc_list[j])



        self.setNode("same_connect_component_4", color_cc_list)

        return color_cc_list

    def get_color_connect_component_8(self,im_data):

        connectivity = 8
        im_data[im_data == 0] = 10
        labels_out = cc3d.connected_components(im_data, connectivity=connectivity)
        label_list=list(set(list(labels_out.flatten())))
        color_cc_list = []
        im_data[im_data == 10] = 0

        max_size=0
        min_size=100

        size_list=[]

        color_list=[]

        for label in label_list:
            node = Node()
            node_id=self.getNumber()+1
            node.setID(node_id)
            self.setNumber(node_id)
            p = list(np.where(labels_out == label))
            c = im_data[p[0][0], p[1][0]]
            node.setNode(p)
            node.setColor(c)
            self.shape(node)
            color_cc_list.append(node)

            size_list.append(node.size)
            color_list.append(node.color)

            if node.size > max_size:
                max_size = node.size

            if node.size < min_size :
                min_size = node.size

        len_size=len(list(set(size_list)))




        len_size=len(list(set(size_list)))

        color_least=self.get_color_least(color_cc_list)

        color_most=self.get_color_most(color_cc_list)

        if not (set(color_least).intersection(set(color_most))):
            self.set_color_least_node(color_cc_list, color_least)
            self.set_color_most_node(color_cc_list,color_most)


        for node in color_cc_list:
            if len_size!=1 and node.size == max_size:
                node.setMaxSize()

            if len_size!=1 and node.size == min_size:
                node.setMinSize()



        for i in range(0, len(color_cc_list) - 1):
            for j in range(i + 1, len(color_cc_list)):
                if color_cc_list[i].color==self.background_color or color_cc_list[j].color==self.background_color:
                    continue
                self.find_relationship_8(color_cc_list[i], color_cc_list[j])


        for i in range(0, len(color_cc_list) - 1):
            for j in range(i + 1, len(color_cc_list)):
                if color_cc_list[i].color==self.background_color or color_cc_list[j].color!=self.background_color:
                    continue
                self.contain_node_single(color_cc_list[i], color_cc_list[j])



        self.setNode("color_connect_component_8", color_cc_list)

        return color_cc_list



    def get_same_connect_component_8(self,im_data):

        connectivity = 8
        im_data[im_data == 0] = 10
        labels_out = cc3d.connected_components(im_data, connectivity=connectivity)
        label_list=list(set(list(labels_out.flatten())))
        color_cc_list = []
        im_data[im_data == 10] = 0

        max_size=0
        min_size=100

        size_list=[]
        color_list=[]

        for label in label_list:

            p = list(np.where(labels_out == label))

            c = im_data[p[0][0], p[1][0]]

            x_min, x_max, y_min, y_max = self.get_possible_edge(p)

            if c == self.background_color and (
                    x_min == 0 or x_max == self.image.shape[0] - 1 or y_min == 0 or y_max == self.image.shape[1] - 1):
                continue



            node = Node()
            node_id=self.getNumber()+1
            node.setID(node_id)
            self.setNumber(node_id)


            node.setNode(p)
            node.setColor(c)
            self.shape(node)
            color_cc_list.append(node)

            size_list.append(node.size)
            color_list.append(node.color)

            if node.size > max_size:
                max_size = node.size

            if node.size < min_size :
                min_size = node.size




        len_size=len(list(set(size_list)))

        color_least=self.get_color_least(color_cc_list)

        color_most=self.get_color_most(color_cc_list)

        if not (set(color_least).intersection(set(color_most))):
            self.set_color_least_node(color_cc_list, color_least)
            self.set_color_most_node(color_cc_list,color_most)

        for node in color_cc_list:
            if len_size!=1 and node.size == max_size:
                node.setMaxSize()

            if len_size!=1 and node.size == min_size:
                node.setMinSize()



        for i in range(0, len(color_cc_list) - 1):
            for j in range(i + 1, len(color_cc_list)):
                self.same_node(color_cc_list[i],color_cc_list[j])

                if color_cc_list[i].color==self.background_color or color_cc_list[j].color==self.background_color:
                    continue
                self.find_relationship_8(color_cc_list[i], color_cc_list[j])

        for i in range(0, len(color_cc_list) - 1):
            for j in range(i + 1, len(color_cc_list)):
                if color_cc_list[i].color==self.background_color or color_cc_list[j].color!=self.background_color:
                    continue
                self.contain_node_single(color_cc_list[i], color_cc_list[j])

        self.setNode("same_connect_component_8", color_cc_list)

        return color_cc_list



    def get_color_connect_component_background_removed_4(self,im_data):

        connectivity = 4
        im_data[im_data == 0] = 10
        labels_out = cc3d.connected_components(im_data, connectivity=connectivity)
        im_data[im_data == 10] = 0
        label_list=list(set(list(labels_out.flatten())))
        color_cc_list = []

        max_size=0
        min_size=100

        size_list=[]
        color_list=[]
        for lable in label_list:
            p = list(np.where(labels_out == lable))
            c = im_data[p[0][0], p[1][0]]

            if c == self.background_color and len(self.corners & set(list(zip(p[0],p[1]))))!=0:
                continue

            node = Node()
            node_id=self.getNumber()+1
            node.setID(node_id)
            self.setNumber(node_id)
            node.setColor(c)
            node.setNode(p)
            self.shape(node)
            color_cc_list.append(node)

            size_list.append(node.size)
            color_list.append(node.color)

            if node.size > max_size:
                max_size = node.size

            if node.size < min_size:
                min_size = node.size

        len_size=len(list(set(size_list)))


        color_least=self.get_color_least(color_cc_list)

        color_most=self.get_color_most(color_cc_list)

        if not (set(color_least).intersection(set(color_most))):
            self.set_color_least_node(color_cc_list, color_least)
            self.set_color_most_node(color_cc_list,color_most)

        for node in color_cc_list:
            if len_size!=1 and node.size == max_size:
                node.setMaxSize()

            if len_size!=1 and node.size == min_size:
                node.setMinSize()






        for i in range(0, len(color_cc_list) - 1):
            for j in range(i + 1, len(color_cc_list)):
                self.find_relationship_4(color_cc_list[i], color_cc_list[j])

        self.setNode("color_connect_component_background_removed_4", color_cc_list)

        return color_cc_list

    def get_color_connect_component_background_removed_8(self, im_data):

        connectivity = 8
        im_data[im_data == 0] = 10
        labels_out = cc3d.connected_components(im_data, connectivity=connectivity)
        im_data[im_data == 10] = 0
        label_list=list(set(list(labels_out.flatten())))
        color_cc_list = []

        max_size=0
        min_size=100

        size_list=[]
        color_list=[]

        for label in label_list:
            p = list(np.where(labels_out == label))
            c = im_data[p[0][0], p[1][0]]

            if c == self.background_color and len(self.corners & set(list(zip(p[0], p[1])))) != 0:
                continue

            node = Node()
            node_id = self.getNumber() + 1
            node.setID(node_id)
            self.setNumber(node_id)
            node.setColor(c)
            node.setNode(p)
            self.shape(node)
            color_cc_list.append(node)
            size_list.append(node.size)
            color_list.append(node.color)

            if node.size > max_size:
                max_size = node.size

            if node.size < min_size:
                min_size = node.size



        len_size=len(list(set(size_list)))

        color_least=self.get_color_least(color_cc_list)

        color_most=self.get_color_most(color_cc_list)

        if not (set(color_least).intersection(set(color_most))):
            self.set_color_least_node(color_cc_list, color_least)
            self.set_color_most_node(color_cc_list,color_most)


        for node in color_cc_list:
            if len_size!=1 and node.size == max_size:
                node.setMaxSize()

            if len_size!=1 and node.size == min_size:
                node.setMinSize()



        for i in range(0, len(color_cc_list) - 1):
            for j in range(i + 1, len(color_cc_list)):
                self.find_relationship_4(color_cc_list[i], color_cc_list[j])

        self.setNode("color_connect_component_background_removed_8", color_cc_list)

        return color_cc_list

    def get_color_connect_component_background_removed_edge_check_4(self, im_data):

        connectivity = 4
        im_data[im_data == 0] = 10
        labels_out = cc3d.connected_components(im_data, connectivity=connectivity)
        im_data[im_data == 10] = 0
        label_list=list(set(list(labels_out.flatten())))
        color_cc_list = []

        max_size = 0
        min_size = 100

        size_list=[]
        color_list=[]

        for label in label_list:
            p = list(np.where(labels_out == label))
            c = im_data[p[0][0], p[1][0]]

            x_min, x_max, y_min, y_max = self.get_possible_edge(p)

            if c == self.background_color and (x_min==0 or x_max==self.image.shape[0]-1 or y_min==0 or y_max==self.image.shape[1]-1):
                continue

            node = Node()
            node_id = self.getNumber() + 1
            node.setID(node_id)
            self.setNumber(node_id)
            node.setColor(c)
            node.setNode(p)
            self.shape(node)
            color_cc_list.append(node)


            size_list.append(node.size)
            color_list.append(node.color)

            if node.size > max_size:
                max_size = node.size

            if node.size < min_size:
                min_size = node.size



        len_size=len(list(set(size_list)))

        color_least=self.get_color_least(color_cc_list)

        color_most=self.get_color_most(color_cc_list)

        if not (set(color_least).intersection(set(color_most))):
            self.set_color_least_node(color_cc_list, color_least)
            self.set_color_most_node(color_cc_list,color_most)


        for node in color_cc_list:
            if len_size!=1 and node.size == max_size:
                node.setMaxSize()

            if len_size!=1 and node.size == min_size:
                node.setMinSize()




        for i in range(0, len(color_cc_list) - 1):
            for j in range(i + 1, len(color_cc_list)):
                self.find_relationship_4(color_cc_list[i], color_cc_list[j])
        self.setNode("color_connect_component_background_removed_edge_check_4", color_cc_list)

        return color_cc_list

    def get_color_connect_component_background_removed_edge_check_8(self, im_data):

        connectivity = 8
        im_data[im_data == 0] = 10
        labels_out = cc3d.connected_components(im_data, connectivity=connectivity)
        im_data[im_data == 10] = 0
        label_list=list(set(list(labels_out.flatten())))
        color_cc_list = []

        max_size = 0
        min_size = 100

        size_list=[]

        color_list=[]

        for label in label_list:
            p = list(np.where(labels_out == label))
            c = im_data[p[0][0], p[1][0]]

            x_min, x_max, y_min, y_max = self.get_possible_edge(p)

            if c == self.background_color and (
                    x_min == 0 or x_max == self.image.shape[0]-1 or y_min == 0 or y_max == self.image.shape[1]-1):
                continue

            node = Node()
            node_id = self.getNumber() + 1
            node.setID(node_id)
            self.setNumber(node_id)
            node.setColor(c)
            node.setNode(p)
            self.shape(node)
            color_cc_list.append(node)

            size_list.append(node.size)
            color_list.append(node.color)

            if node.size > max_size:
                max_size = node.size

            if node.size < min_size:
                min_size = node.size

        len_size=len(list(set(size_list)))


        color_least=self.get_color_least(color_cc_list)

        color_most=self.get_color_most(color_cc_list)

        if not (set(color_least).intersection(set(color_most))):
            self.set_color_least_node(color_cc_list, color_least)
            self.set_color_most_node(color_cc_list,color_most)


        for node in color_cc_list:
            if len_size!=1 and node.size == max_size:
                node.setMaxSize()

            if len_size!=1 and node.size == min_size:
                node.setMinSize()



        for i in range(0, len(color_cc_list) - 1):
            for j in range(i + 1, len(color_cc_list)):
                self.find_relationship_8(color_cc_list[i], color_cc_list[j])

        self.setNode("color_connect_component_background_removed_edge_check_8", color_cc_list)

        return color_cc_list


    def get_color_vertical_connect_component_4(self, im_data):

        def vertical_connect(im_data):
            row,column=im_data.shape
            global_label=0
            labels_out=np.zeros(im_data.shape,dtype=int)
            for column_number in range (column):
                column_pointer = im_data[0][column_number]
                row_number = 0
                while row_number<row:
                    if column_pointer==im_data[row_number][column_number]:
                        labels_out[row_number][column_number]=global_label
                    else:
                        column_pointer = im_data[row_number][column_number]
                        global_label+=1
                        labels_out[row_number][column_number] = global_label
                    row_number+=1
                global_label+=1
            return labels_out


        labels_out = vertical_connect(im_data)
        label_list=list(set(list(labels_out.flatten())))
        color_cc_list = []

        max_size = 0
        min_size = 100

        for label in label_list:

            p = list(np.where(labels_out == label))
            c = im_data[p[0][0], p[1][0]]

            if c==self.background_color:
                continue

            node = Node()
            node_id = self.getNumber() + 1
            node.setID(node_id)
            self.setNumber(node_id)
            node.setNode(p)
            node.setColor(c)
            self.shape(node)
            color_cc_list.append(node)



        for i in range(0, len(color_cc_list) - 1):
            for j in range(i + 1, len(color_cc_list)):
                self.find_relationship_8(color_cc_list[i], color_cc_list[j])

        self.setNode("color_vertical_connect_component_4", color_cc_list)

        return color_cc_list

    def get_color_horizontal_connect_component_4(self, im_data):

        def horizontal_connect(im_data):
            row, column = im_data.shape
            global_label = 0
            labels_out = np.zeros(im_data.shape,dtype=int)
            for row_number in range(row):
                row_pointer = im_data[row_number][0]
                column_number = 0
                while column_number < column:
                    if row_pointer == im_data[row_number][column_number]:
                        labels_out[row_number][column_number] = global_label
                    else:
                        row_pointer = im_data[row_number][column_number]
                        global_label += 1
                        labels_out[row_number][column_number] = global_label
                    column_number += 1
                global_label += 1
            return labels_out


        labels_out = horizontal_connect(im_data)
        label_list=list(set(list(labels_out.flatten())))
        color_cc_list = []

        max_size = 0
        min_size = 100


        for label in label_list:
            p = list(np.where(labels_out == label))
            c = im_data[p[0][0], p[1][0]]

            if c==self.background_color:
                continue

            node = Node()
            node_id = self.getNumber() + 1
            node.setID(node_id)
            self.setNumber(node_id)
            node.setNode(p)
            node.setColor(c)
            self.shape(node)
            color_cc_list.append(node)


        for i in range(0, len(color_cc_list) - 1):
            for j in range(i + 1, len(color_cc_list)):
                self.find_relationship_8(color_cc_list[i], color_cc_list[j])

        self.setNode("color_horizontal_connect_component_4", color_cc_list)

        return color_cc_list



    def get_non_black_component_graph_4(self, im_data):

        connectivity = 4
        im_data[im_data == 0] = 10
        labels_out = cc3d.connected_components(im_data, connectivity=connectivity)
        im_data[im_data == 10] = 0
        label_list=list(set(list(labels_out.flatten())))
        color_cc_list = []

        max_size = 0
        min_size = 100

        size_list=[]

        color_list=[]

        for label in label_list:

            p = list(np.where(labels_out == label))
            c = im_data[p[0][0], p[1][0]]

            if c==0:
                continue

            node = Node()
            node_id = self.getNumber() + 1
            node.setID(node_id)
            self.setNumber(node_id)
            node.setNode(p)
            node.setColor(c)
            self.shape(node)
            color_cc_list.append(node)

            size_list.append(node.size)
            color_list.append(node.color)

            if node.size > max_size:
                max_size = node.size

            if node.size < min_size:
                min_size = node.size

        len_size=len(list(set(size_list)))

        color_least=self.get_color_least(color_cc_list)

        color_most=self.get_color_most(color_cc_list)

        if not (set(color_least).intersection(set(color_most))):
            self.set_color_least_node(color_cc_list, color_least)
            self.set_color_most_node(color_cc_list,color_most)


        for node in color_cc_list:
            if len_size!=1 and node.size == max_size:
                node.setMaxSize()

            if len_size!=1 and node.size == min_size:
                node.setMinSize()




        for i in range(0, len(color_cc_list) - 1):
            for j in range(i + 1, len(color_cc_list)):
                self.find_relationship_4(color_cc_list[i], color_cc_list[j])

        self.setNode("non_black_component_graph_4", color_cc_list)

        return color_cc_list


    def get_non_black_component_graph_8(self, im_data):

        connectivity = 8
        im_data[im_data == 0] = 10
        labels_out = cc3d.connected_components(im_data, connectivity=connectivity)
        im_data[im_data == 10] = 0
        label_list=list(set(list(labels_out.flatten())))
        color_cc_list = []

        max_size = 0
        min_size = 100

        size_list=[]
        color_list=[]

        for label in label_list:

            p = list(np.where(labels_out == label))
            c = im_data[p[0][0], p[1][0]]

            if c == 0:
                continue

            node = Node()
            node_id = self.getNumber() + 1
            node.setID(node_id)
            self.setNumber(node_id)
            node.setNode(p)
            node.setColor(c)
            self.shape(node)
            color_cc_list.append(node)

            size_list.append(node.size)
            color_list.append(node.color)

            if node.size > max_size:
                max_size = node.size

            if node.size < min_size:
                min_size = node.size

        len_size=len(list(set(size_list)))



        color_least=self.get_color_least(color_cc_list)

        color_most=self.get_color_most(color_cc_list)

        if not (set(color_least).intersection(set(color_most))):
            self.set_color_least_node(color_cc_list, color_least)
            self.set_color_most_node(color_cc_list,color_most)



        for node in color_cc_list:
            if len_size!=1 and node.size == max_size:
                node.setMaxSize()

            if len_size!=1 and node.size == min_size:
                node.setMinSize()



        for i in range(0, len(color_cc_list) - 1):
            for j in range(i + 1, len(color_cc_list)):
                self.find_relationship_8(color_cc_list[i], color_cc_list[j])

        self.setNode("non_black_component_graph_8", color_cc_list)

        return color_cc_list

    def get_multicolor_connect_component_4(self, im_data):

        binary = np.zeros(im_data.shape)
        binary[np.where(im_data != 0)] = 1
        binary[np.where(im_data == 0)] = 10



        connectivity = 4
        labels_out = cc3d.connected_components(binary, connectivity=connectivity)
        binary[np.where(im_data == self.background_color)] = 0
        label_list=list(set(list(labels_out.flatten())))
        binary_cc_list = []

        max_size = 0
        min_size = 100

        size_list=[]

        for label in label_list:

            p = list(np.where(labels_out == label))

            c = im_data[p[0][0], p[1][0]]

            if c == 0:
                continue

            node = Node()
            node_id = self.getNumber() + 1
            node.setID(node_id)
            self.setNumber(node_id)


            node.setNode(p)
            self.shape(node)
            binary_cc_list.append(node)


            color_dic=dict()
            for i in range (len(p[0])):
                x_location=p[0][i]
                y_location=p[1][i]
                color=im_data[x_location][y_location]
                if color not in color_dic:
                    color_dic[color ]=1
                else:
                    color_dic[color]+=1
            max_color=max(color_dic,key=lambda x:color_dic[x])
            node.setColor(max_color)

            size_list.append(node.size)

            if node.size > max_size:
                max_size = node.size

            if node.size < min_size:
                min_size = node.size

        len_size=len(list(set(size_list)))
        for node in binary_cc_list:
            if len_size!=1 and node.size == max_size:
                node.setMaxSize()

            if len_size!=1 and node.size == min_size:
                node.setMinSize()


        for i in range(0, len(binary_cc_list) - 1):
            for j in range(i + 1, len(binary_cc_list)):
                self.find_relationship_4(binary_cc_list[i], binary_cc_list[j])

        self.setNode("multicolor_connect_component_4", binary_cc_list)

        return binary_cc_list

    def get_multicolor_connect_component_grey_removed_4(self, im_data):

        binary = np.zeros(im_data.shape)
        binary[np.where((im_data != 0) & (im_data !=5))] = 1
        binary[np.where(im_data == 5)] = 2
        binary[np.where(im_data == 0)] = 10

        connectivity = 4
        labels_out = cc3d.connected_components(binary, connectivity=connectivity)
        binary[np.where(im_data == self.background_color)] = 0
        label_list = list(set(list(labels_out.flatten())))
        binary_cc_list = []

        max_size = 0
        min_size = 100

        size_list = []

        for label in label_list:

            p = list(np.where(labels_out == label))

            c = im_data[p[0][0], p[1][0]]

            if c == 0:
                continue

            c = im_data[p[0][0], p[1][0]]

            if c == 5:
                continue

            node = Node()
            node_id = self.getNumber() + 1
            node.setID(node_id)
            self.setNumber(node_id)

            node.setNode(p)
            self.shape(node)
            binary_cc_list.append(node)

            color_dic = dict()
            for i in range(len(p[0])):
                x_location = p[0][i]
                y_location = p[1][i]
                color = im_data[x_location][y_location]
                if color not in color_dic:
                    color_dic[color] = 1
                else:
                    color_dic[color] += 1
            max_color = max(color_dic, key=lambda x: color_dic[x])
            node.setColor(max_color)

            size_list.append(node.size)

            if node.size > max_size:
                max_size = node.size

            if node.size < min_size:
                min_size = node.size

        len_size = len(list(set(size_list)))
        for node in binary_cc_list:
            if len_size != 1 and node.size == max_size:
                node.setMaxSize()

            if len_size != 1 and node.size == min_size:
                node.setMinSize()

        for i in range(0, len(binary_cc_list) - 1):
            for j in range(i + 1, len(binary_cc_list)):
                self.find_relationship_4(binary_cc_list[i], binary_cc_list[j])

        self.setNode("multicolor_connect_component_grey_removed_4", binary_cc_list)

        return binary_cc_list



    def get_multicolor_connect_component_8(self, im_data):

        binary = np.zeros(im_data.shape)
        binary[np.where(im_data != 0)] = 1
        binary[np.where(im_data == 0)] = 10

        connectivity = 8
        labels_out = cc3d.connected_components(binary, connectivity=connectivity)
        binary[np.where(im_data == self.background_color)] = 0
        label_list=list(set(list(labels_out.flatten())))
        binary_cc_list = []

        max_size = 0
        min_size = 100

        size_list=[]

        for label in label_list:

            p = list(np.where(labels_out == label))

            c = im_data[p[0][0], p[1][0]]

            if c == 0:
                continue

            node = Node()
            node_id = self.getNumber() + 1
            node.setID(node_id)
            self.setNumber(node_id)


            node.setNode(p)
            self.shape(node)
            binary_cc_list.append(node)

            color_dic=dict()
            for i in range (len(p[0])):
                x_location=p[0][i]
                y_location=p[1][i]
                color=im_data[x_location][y_location]
                if color not in color_dic:
                    color_dic[color ]=1
                else:
                    color_dic[color]+=1
            max_color=max(color_dic,key=lambda x:color_dic[x])
            node.setColor(max_color)

            size_list.append(node.size)



            if node.size > max_size:
                max_size = node.size

            if node.size < min_size:
                min_size = node.size

        len_size = len(list(set(size_list)))
        for node in binary_cc_list:
            if len_size!=1 and node.size == max_size:
                node.setMaxSize()

            if len_size!=1 and node.size == min_size:
                node.setMinSize()




        for i in range(0, len(binary_cc_list) - 1):
            for j in range(i + 1, len(binary_cc_list)):
                self.find_relationship_8(binary_cc_list[i], binary_cc_list[j])

        self.setNode("multicolor_connect_component_8", binary_cc_list)

        return binary_cc_list

    def get_multicolor_connect_component_grey_removed_8(self, im_data):

        binary = np.zeros(im_data.shape)
        binary[np.where((im_data != 0) & (im_data !=5))] = 1
        binary[np.where(im_data == 5)] = 2
        binary[np.where(im_data == 0)] = 10

        connectivity = 8
        labels_out = cc3d.connected_components(binary, connectivity=connectivity)
        binary[np.where(im_data == self.background_color)] = 0
        label_list = list(set(list(labels_out.flatten())))
        binary_cc_list = []

        max_size = 0
        min_size = 100

        size_list = []

        for label in label_list:

            p = list(np.where(labels_out == label))

            c = im_data[p[0][0], p[1][0]]

            if c == 0:
                continue

            c = im_data[p[0][0], p[1][0]]

            if c == 5:
                continue

            node = Node()
            node_id = self.getNumber() + 1
            node.setID(node_id)
            self.setNumber(node_id)

            node.setNode(p)
            self.shape(node)
            binary_cc_list.append(node)

            color_dic = dict()
            for i in range(len(p[0])):
                x_location = p[0][i]
                y_location = p[1][i]
                color = im_data[x_location][y_location]
                if color not in color_dic:
                    color_dic[color] = 1
                else:
                    color_dic[color] += 1
            max_color = max(color_dic, key=lambda x: color_dic[x])
            node.setColor(max_color)

            size_list.append(node.size)

            if node.size > max_size:
                max_size = node.size

            if node.size < min_size:
                min_size = node.size

        len_size = len(list(set(size_list)))
        for node in binary_cc_list:
            if len_size != 1 and node.size == max_size:
                node.setMaxSize()

            if len_size != 1 and node.size == min_size:
                node.setMinSize()

        for i in range(0, len(binary_cc_list) - 1):
            for j in range(i + 1, len(binary_cc_list)):
                self.find_relationship_8(binary_cc_list[i], binary_cc_list[j])

        self.setNode("multicolor_connect_component_grey_removed_8", binary_cc_list)

        return binary_cc_list

    def get_black_removed_node_position_4(self,im_data):
        connectivity = 4
        im_data[im_data == 0] = 10
        labels_out = cc3d.connected_components(im_data, connectivity=connectivity)
        im_data[im_data == 10] = 0
        label_list=list(set(list(labels_out.flatten())))
        color_cc_list = []

        max_size = 0
        min_size = 100

        size_list=[]

        color_list=[]

        for label in label_list:

            p = list(np.where(labels_out == label))
            c = im_data[p[0][0], p[1][0]]

            if c==0:
                continue

            node = Node()
            node_id = self.getNumber() + 1
            node.setID(node_id)
            self.setNumber(node_id)
            node.setNode(p)
            node.setColor(c)
            self.shape(node)
            color_cc_list.append(node)

            size_list.append(node.size)
            color_list.append(node.color)

            if node.size > max_size:
                max_size = node.size

            if node.size < min_size:
                min_size = node.size

        len_size=len(list(set(size_list)))

        color_least=self.get_color_least(color_cc_list)

        color_most=self.get_color_most(color_cc_list)

        if not (set(color_least).intersection(set(color_most))):
            self.set_color_least_node(color_cc_list, color_least)
            self.set_color_most_node(color_cc_list,color_most)


        for node in color_cc_list:
            if len_size!=1 and node.size == max_size:
                node.setMaxSize()

            if len_size!=1 and node.size == min_size:
                node.setMinSize()




        for i in range(0, len(color_cc_list) - 1):
            for j in range(i + 1, len(color_cc_list)):
                self.find_relationship_4(color_cc_list[i], color_cc_list[j])

        self.setNode("black_removed_node_position_4", color_cc_list)

        return color_cc_list



    def get_black_node_4(self,im_data):
        connectivity = 4
        im_data[im_data == 0] = 10
        labels_out = cc3d.connected_components(im_data, connectivity=connectivity)
        im_data[im_data == 10] = 0
        label_list=list(set(list(labels_out.flatten())))
        color_cc_list = []

        max_size = 0
        min_size = 100

        size_list=[]

        color_list=[]

        for label in label_list:

            p = list(np.where(labels_out == label))
            c = im_data[p[0][0], p[1][0]]

            if c!=0:
                continue

            label_tmp = np.zeros(labels_out.shape)
            label_tmp[labels_out == label] = 1
            label_tmp = np.array(label_tmp, "bool")


            result=re_detect.largest_interior_rectangle(label_tmp)

            if result[0][0]==99 and result[0][1]==99:
                continue

            if len(result)==1:
                p = [[], []]
                for i in range(result[0][1], result[0][1] + result[0][3]):
                    for j in range(result[0][0], result[0][0] + result[0][2]):
                        p[0].append(i)
                        p[1].append(j)
            else:
                locations=[]
                remove_list=[]
                for removed in range (len(result)):
                    for i in range(result[removed][1], result[removed][1] + result[removed][3]):
                        for j in range(result[removed][0], result[removed][0] + result[removed][2]):
                            if (i,j) not in locations:
                                locations.append((i,j))
                            else:
                                remove_list.append(removed)

                for node in range (len(result)):
                    if node in remove_list:
                        continue
                    p = [[], []]
                    for i in range(result[node][1], result[node][1] + result[node][3]):
                        for j in range(result[node][0], result[node][0] + result[node][2]):
                            p[0].append(i)
                            p[1].append(j)

                    node = Node()
                    node_id = self.getNumber() + 1
                    node.setID(node_id)
                    self.setNumber(node_id)
                    node.setNode(p)
                    node.setColor(c)
                    self.shape(node)
                    color_cc_list.append(node)
                    size_list.append(node.size)

                    if node.size > max_size:
                        max_size = node.size

                    if node.size < min_size:
                        min_size = node.size
                continue


            node = Node()
            node_id = self.getNumber() + 1
            node.setID(node_id)
            self.setNumber(node_id)
            node.setNode(p)
            node.setColor(c)
            self.shape(node)
            color_cc_list.append(node)
            size_list.append(node.size)

            if node.size > max_size:
                max_size = node.size

            if node.size < min_size:
                min_size = node.size

        len_size=len(list(set(size_list)))


        for node in color_cc_list:
            if len_size==1:
                node.setMaxSize()
            elif len_size!=1 and node.size == max_size:
                node.setMaxSize()

            elif len_size!=1 and node.size == min_size:
                node.setMinSize()




        self.setNode("black_node_4", color_cc_list)

        return color_cc_list

    def get_same_color_node(self, im_data):


        max_size = 0
        min_size = 100

        size_list = []

        color_cc_list = []

        for label in range(0,10):

            p = list(np.where(im_data == label))
            if len(p[0])==0:
                continue
            c = im_data[p[0][0], p[1][0]]

            node = Node()
            node_id = self.getNumber() + 1
            node.setID(node_id)
            self.setNumber(node_id)
            node.setNode(p)
            node.setColor(c)
            self.shape(node)
            color_cc_list.append(node)

            size_list.append(node.size)

            if node.size > max_size:
                max_size = node.size

            if node.size < min_size:
                min_size = node.size

        len_size = len(list(set(size_list)))

        for node in color_cc_list:
            if len_size != 1 and node.size == max_size:
                node.setMaxSize()

            if len_size != 1 and node.size == min_size:
                node.setMinSize()

        self.setNode("same_color_node", color_cc_list)

        return color_cc_list

    def get_none_abstract(self, im_data):


        binary = np.zeros(im_data.shape)

        connectivity = 8
        labels_out = cc3d.connected_components(binary, connectivity=connectivity)
        label_list = list(set(list(labels_out.flatten())))

        p = list(np.where(labels_out == label_list[0]))

        binary_cc_list=[]

        node = Node()
        node_id = self.getNumber() + 1
        node.setID(node_id)
        self.setNumber(node_id)
        node.setNode(p)
        binary_cc_list.append(node)

        self.setNode("none_abstract", binary_cc_list)

        return binary_cc_list

    def findend(self,i, j, a, output, index):
        x = len(a)
        y = len(a[0])

        # flag to check column edge case,
        # initializing with 0
        flagc = 0

        # flag to check row edge case,
        # initializing with 0
        flagr = 0

        possible_end=[]
        current_n=0
        break_flag = False
        for m in range(i, x):

            # loop breaks where first 1 encounters
            if a[m][j] == 0:
                flagr = 1
                if flagc == 1:  # set the flag
                    possible_end.append((m - 1, current_n - 1))
                else:
                    possible_end.append((m-1, current_n ))# set the flag
                break

            # pass because already processed
            if a[m][j] == -1:
                pass


            for n in range(j, y):

                # loop breaks where first 1 encounters
                if a[m][n] == 0:
                    if n < y:
                        y = n + 1
                    flagc = 1
                    if flagr==1:# set the flag
                        possible_end.append((m-1,n-1))
                    else:
                        possible_end.append((m, n - 1))
                    current_n=n
                    break_flag=True
                    break

            if not break_flag:
                if flagr == 1:  # set the flag
                    possible_end.append((m - 1, y - 1))
                else:
                    possible_end.append((m, y - 1))

                # fill rectangle elements with any
                # number so that we can exclude
                # next time
        max_size=0
        result_x=-1
        result_y=-1
        for key in possible_end:
            m=key[0]
            n=key[1]
            if m==i or n==j:
                continue
            if (m-i+1)*(n-j+1)>max_size:
                max_size=(m-i+1)*(n-j+1)
                result_x=m
                result_y=n
        output[index].append(result_x)
        output[index].append(result_y)


    def get_rectangle_coordinates(self,a):
        # retrieving the column size of array
        size_of_array = len(a)

        # output array where we are going
        # to store our output
        output = []

        # It will be used for storing start
        # and end location in the same index
        index = -1

        for i in range(0, size_of_array):
            for j in range(0, len(a[0])):
                if a[i][j] == 1:
                    a[i][j] = -1

                    output.append([i, j])

                    # will be used for the
                    # last position
                    index = index + 1
                    self.findend(i, j, a, output, index)

        max_locations = []
        max_area = 0
        for i in range(len(output)):

            x_start = output[i][0]
            y_start = output[i][1]
            x_end = output[i][2]
            y_end = output[i][3]

            if x_end==-1 or y_end==-1:
                continue

            if x_start == x_end or y_start == y_end:
                continue
            x_dis = x_end - x_start +1
            y_dis = y_end - y_start +1
            if x_dis * y_dis > max_area:
                max_area = x_dis * y_dis

        for i in range(len(output)):

            x_start = output[i][0]
            y_start = output[i][1]
            x_end = output[i][2]
            y_end = output[i][3]

            if x_end==-1 or y_end==-1:
                continue

            if x_start == x_end or y_start == y_end:
                continue
            x_dis = x_end - x_start +1
            y_dis = y_end - y_start +1
            if x_dis * y_dis == max_area:
                if output[i] not in max_locations:
                    max_locations.append(output[i])

        if len(max_locations)==0:
            return [[[],[]]]
        final_result=[]
        all_location=[]
        for max_location in max_locations:
            append=True
            width = max_location[3] - max_location[1]
            height = max_location[2] - max_location[0]

            result = [[], []]
            for i in range(max_location[0], max_location[2] + 1):
                result[0] += [i] * (width + 1)
            for j in range(max_location[1], max_location[3] + 1):
                result[1] += [j]
            result[1] = result[1] * (height + 1)

            locations=list(zip(result[0],result[1]))
            if len(all_location)>0:
                for location in locations:
                    if location in all_location:
                        append=False
                        break
            if append:
                final_result.append(result)
                all_location+=locations
        return final_result


    def get_color_least(self,node_list):
        color_list=[]
        for node in node_list:
            color_list.append(node.color)
        if len(color_list)==0:
            return []
        counter_color = Counter(color_list)
        min_time = min(counter_color.values())
        min_color=[]
        for key, value in counter_color.items():
            if value == min_time:
                min_color.append(key)

        return min_color

    def set_color_least_node(self,node_list,color_least):

        for node in node_list:
            if node.color in color_least:
                node.setLeastColor()

    def get_color_most(self, node_list):
        color_list = []
        for node in node_list:
            color_list.append(node.color)
        if len(color_list)==0:
            return []
        counter_color = Counter(color_list)
        max_time = max(counter_color.values())
        max_color = []
        for key, value in counter_color.items():
            if value == max_time:
                max_color.append(key)

        return max_color

    def set_color_most_node(self, node_list, color_most):

        for node in node_list:
            if node.color in color_most:
                node.setMostColor()




    def graph_abstract(self,graph,im_data):
        self.setImage(im_data)

        current_pixel_similar=copy.deepcopy(self.pixel_similar)
        change_pixel=[]

        while (True):

            break_flag = True
            change_pixel=self.get_pixel_information(change_pixel)

            updated_pixel_similar = self.pixel_similar

            for i in range (len(updated_pixel_similar)):
                if break_flag:
                    for j in range (len(updated_pixel_similar[0])):
                        if updated_pixel_similar[i][j]!=current_pixel_similar[i][j]:
                            break_flag=False
                            break
                else:
                    break

            current_pixel_similar=updated_pixel_similar

            if break_flag:
                break

        only_pixel=False
        data=im_data.flatten()
        if len(list(set(data)))==1 and data[0]==0:
            only_pixel=True

        for i in range (len(self.abstraction_ops)):
            if only_pixel:
                if self.abstraction_ops[i]!="get_recolor_connect_component_0":
                    continue
            getattr(graph, self.abstraction_ops[i])(im_data)



    def graph_abstract_select_task(self, graph, im_data):
        self.setImage(im_data)

        for i in range(len(self.abstraction_select_task)):
            getattr(graph, self.abstraction_select_task[i])(im_data)






    def check_repeat_node(self,node1, node2):

        sys.setrecursionlimit(10000)


        repeat_node = []

        if len(node1)!=len(node2):
            return False

        repeat=True
        for n1 in node1:
            node_repeat=False
            for n2 in node2:
                if self.node_eq(n1,n2):
                    node_repeat=True
                    break
            if not node_repeat:
                repeat=False
                break
        return repeat

    def containsCycle(self, grid) -> bool:
        m, n, visited = len(grid), len(grid[0]), set()

        def dfs(i, j, px, py):
            visited.add((i, j))
            for dx, dy in [[0, 1], [1, 0], [-1, 0], [0, -1]]:
                x, y = i + dx, j + dy
                if 0 <= x < m and 0 <= y < n and (x, y) != (px, py) and grid[x][y]==1 and grid[x][y] == grid[i][j]:
                    if (x, y) in visited: return True
                    if dfs(x, y, i, j): return True
            return False

        for i in range(m):
            for j in range(n):
                if (i, j) not in visited:
                    if dfs(i, j, -1, -1):
                        return True
        return False

    def node_cycles(self,node):
        p=node.p
        x_min=min(p[0])
        x_max=max(p[0])
        y_min=min(p[1])
        y_max=max(p[1])

        p1s=list(zip(p[0],p[1]))

        grid=np.zeros((x_max-x_min+1,y_max-y_min+1))

        for i in range (x_min,x_max+1):
            for j in range (y_min,y_max+1):
                if (i,j) in p1s:
                    grid[i-x_min][j-y_min]=1
        return self.containsCycle(grid)

    # Python program to find all
    # rectangles filled with 0

    def node_replace (self, abs, node_list,ndic):

        for node1, node2 in node_list:

            if node1.max_size_node !=node2.max_size_node:
                node1.setMaxDouble()
            if node1.min_size_node !=node2.min_size_node:
                node1.setMinDouble()
            if node1.most_color_node !=node2.most_color_node:
                node1.setMostDouble()


            for node in ndic[abs]:
                for i in range (len(node.vertical)-1,-1,-1):
                    if node.vertical[i].id==node2.id:
                        del node.vertical[i]
                        node.vertical.append(node1)
                        break

            for node in ndic[abs]:
                for i in range (len(node.horizontal)-1,-1,-1):
                    if node.horizontal[i].id==node2.id:
                        del node.horizontal[i]
                        node.horizontal.append(node1)
                        break






    def node_eq(self,node1,node2):

        if len(set(list(zip(node1.p[0],node1.p[1]))) & set(list(zip(node2.p[0],node2.p[1]))))==node1.size:
            return True
        else:
            return False




