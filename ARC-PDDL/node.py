

class Node:
    def __init__(self):

        self.id=0
        self.p = []
        self.size=0
        self.color=0
        self.similar_color=0
        self.vertical=[]
        self.horizontal=[]
        self.same=[]
        self.connected=[]
        self.shape=""
        self.boundary=None
        self.contain=[]
        self.max_size_node=False
        self.min_size_node=False
        self.most_color_node=False
        self.least_color_node=False
        self.max_size_double=False
        self.min_size_double=False
        self.most_color_double=False
        self.least_color_double=False
        self.node_background_color=0
        self.odd=False
        self.even = False

        self.left_diagonal_node=False
        self.right_diagonal_node = False

        self.left_border=False
        self.right_border = False
        self.up_border=False
        self.down_border = False

        self.vertical_middle=False
        self.horizontal_middle = False


        self.incomplete_contain=[]
        self.left= []
        self.right= []
        self.down= []
        self.up= []
        self.left_up=[]
        self.left_down = []
        self.right_up=[]
        self.right_down=[]
        self.same_color=[]


    def setNode(self, pixels):
        self.p=pixels
        self.size=len(pixels[0])
        if self.size%2==0:
            self.even=True
        else:
            self.odd=True

    def setID(self,number):
        self.id=number


    def setColor(self, c):
        self.color=c

    def setSimilarColor(self,c):
        self.similar_color=c

    def setVer(self,node):
        self.vertical.append(node)

    def setHor(self,node):
        self.horizontal.append(node)

    def setSame(self,node):
        self.same.append(node)

    def setCon(self,node):
        self.connected.append(node)

    def setContain(self,node):
        self.contain.append(node)

    def setInContain(self,node):
        self.incomplete_contain.append(node)


    def setMaxSize(self):
        self.max_size_node=True

    def setMinSize(self):
        self.min_size_node=True

    def setMostColor(self):
        self.most_color_node=True

    def setLeastColor(self):
        self.least_color_node=True

    def setMaxDouble(self):
        self.max_size_double=True

    def setMinDouble(self):
        self.min_size_double=True

    def setMostDouble(self):
        self.most_color_double=True

    def setLeftDiagonal(self):
        self.left_diagonal_node=True

    def setRightDiagonal(self):
        self.right_diagonal_node=True

    def setLeftBorder(self):
        self.left_border = True

    def setRightBorder(self):
        self.right_border = True

    def setUpBorder(self):
        self.up_border = True

    def setDownBorder(self):
        self.down_border = True


    def setVerticalMiddle(self):
        self.vertical_middle = True

    def setHorizontalMiddle(self):
        self.horizontal_middle = True


    # shape include "single_pixel","rectangle","square","vertical_line","horizontal_line","diagonal_line","unknown"
    def setShape(self,shape,boundary=None):
        self.shape=shape
        self.boundary=boundary

    def setNodeBackGroundColor(self,color):
        self.node_background_color=color


    def setLeft(self,node):
        self.left.append(node)

    def setRight(self,node):
        self.right.append(node)

    def setUp(self,node):
        self.up.append(node)

    def setDown(self,node):
        self.down.append(node)


    def setLeftUp(self,node):
        self.left_up.append(node)

    def setLeftDown(self,node):
        self.left_down.append(node)

    def setRightUp(self,node):
        self.right_up.append(node)

    def setRightDown(self,node):
        self.right_down.append(node)

    def setSameColor(self,node):
        self.same_color.append(node)

