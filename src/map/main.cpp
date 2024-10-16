#include "rb_tree.h"
#include "time.h"
#include <map>

int main()
{
	RedBlackTree<int, int> tree;
	{
		clock_t start, finish;
		int count = 1;
		{
		for (int i = 0; i < 1000000; ++i)
		{
			tree.insert(i, i);
		}
		start = clock();
		while (count--)
		for (int i = 0; i < 1000000; ++i)
		{
			tree.find(i);
		}
		}
		finish = clock();
		tree.clear();
		//tree.printTree();
		// 使用 begin() 和 end() 遍历整个树
		/*for (auto it = tree.begin(); it != tree.end(); ++it) {
			std::cout << it->key << " -> " << it->value << std::endl;
		}*/
		std::cout << finish - start << std::endl;
	}
	std::map<int, int> _map;
	{
		clock_t start, finish;
		{
		for (int i = 0; i < 1000000; ++i)
		{
			_map.insert({ i, i });
		}
		int count = 1;
		start = clock();
		while (count--)
		for (int i = 0; i < 1000000; ++i)
		{
			tree.find(i);
		}
		}
		finish = clock();
		_map.clear();
		std::cout << finish - start << std::endl;
		//// 使用 begin() 和 end() 遍历整个树
		//for (auto it = _map.begin(); it != _map.end(); ++it) {
		//	std::cout << it->first << " -> " << it->second << std::endl;
		//}
	}

}