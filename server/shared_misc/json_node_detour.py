
class JsonNodeDetour:
    def __init__(self, graph: dict, detour_node: str) -> None:
        self.graph = graph
        self.detour_node = detour_node

    def json_detour(self):
        detour_queue = [self.graph]
        iterator = 1
        while len(detour_queue) > 0:
            detour_node = detour_queue.pop(0)
            if self.detour_node in detour_node:
                detour_node['id'] = iterator
                iterator += 1
            for child in detour_node.values():
                if type(child) is list:
                    for item in child:
                        if type(item) is dict:
                            detour_queue.append(item)
                elif type(child) is dict:
                    detour_queue.append(child)
