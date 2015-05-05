class TreeNode():
    name = ''
    children = None

    def __init__(self, name):
        self.name = name
        self.children = []
    # end of __init__

    def nbr_of_children(self):
        if self.children:
            return len(self.children)
        else:
            return 0
    # end of nbr_of_children

    def add_child(self, new_node, i=0):
        self.children.insert(i, new_node)
    # end of add_child

    def populate_tree(self, node_list):
        """
        Called on the root_node 
        """
        for node in node_list:
            self.place_in_tree(node.split('.'))
    # end of populate_tree
   
    def place_in_tree(self, name_pieces):
        i = 0
        nbr_children = self.nbr_of_children()
        if nbr_children != 0:
            while name_pieces[0] >= self.children[i].name:
                if name_pieces[0] == self.children[i].name:
                    self.children[i].place_in_tree(name_pieces[1:])
                    return
                else:
                    i += 1
                    if i == nbr_children:
                        break
                
        new_node = TreeNode(name_pieces[0])
        self.add_child(new_node, i)
        node = new_node
        for name in name_pieces[1:]:
            new_node = TreeNode(name)
            node.add_child(new_node)
            node = new_node
    # end of place_in_tree

    def search_tree(self, search_name):
        """
        Called on root_node when search begins.

        """  
        name_pieces = search_name.split('.')
        for child in self.children:
            if name_pieces[0] < child.name:
                return False
            elif child.name == name_pieces[0]:
                if len(name_pieces) == 1:
                    return True
                else: 
                    return child.search_tree_rec(name_pieces[1:])
        return False
    # end of search_tree

    def search_tree_rec(self, name_pieces):
        nbr_children = self.nbr_of_children()
        if nbr_children == 0:
            if name_pieces:
                return name_pieces[0] == self.name
            else:
                return False
        else :
            if self.children[0].name == '':
                return True
            elif self.children[0].name == '*' and len(name_pieces) == 1:
                return True
            for child in self.children:
                if name_pieces[0] < child.name :
                    return False
                elif child.name == name_pieces[0]:
                    if len(name_pieces) == 1:
                        return True
                    else:
                        return child.search_tree_rec(name_pieces[1:])

            return False
        # end of search_tree_rec

# end of tree_node


  






