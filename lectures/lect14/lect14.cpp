class Graph {

	void connectNodes(int idx1, int idx2, float weight) {

	}

	std::vector<Node*> getPath(int start, int end) {
		std::vector<Node*> path;
		for nodes.size {
			nodes[i].visited = false;
			nodes[i].cameFrom = NULL;
		}

		//alot more stuff here
		
		Node* current = &nodes[goal];
		path.push_back(&nodes[goal]);

		while(current != &nodes[start]) {
			current = current->cameFrom;
			path.push_back(current);
		}

		return path;
	}
}