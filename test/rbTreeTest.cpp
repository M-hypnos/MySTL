#include "rbTreeTest.h"
#include "../functors.h"
namespace mySTL {
	namespace rbTreeTest {
		void test1() {
			rbTree<int, int, identify<int>, less<int>> tree;
			//std::cout << "size() == " << tree.size() << std::endl;
			tree.insert_unique(10);
			assert(tree.isBalance());
			tree.insert_unique(7);
			assert(tree.isBalance());
			tree.insert_unique(8);
			assert(tree.isBalance());
			tree.insert_unique(15);
			assert(tree.isBalance());
			tree.insert_unique(5);
			assert(tree.isBalance());
			tree.insert_unique(6);
			assert(tree.isBalance());
			tree.insert_unique(11);
			assert(tree.isBalance());
			tree.insert_unique(13);
			assert(tree.isBalance());
			tree.insert_unique(12);
			assert(tree.isBalance());
			tree.insert_unique(15);
			assert(tree.isBalance());
			tree.insert_unique(11);
			assert(tree.isBalance());

			rbTree<int, int, identify<int>, less<int>> tr = tree;
			tree.clear();
			assert(tree.isBalance());
			//std::cout << "size() == " << tree.size() << std::endl;
			tree = tr;
			assert(tree.isBalance());
			//std::cout << "size() == " << tree.size() << std::endl;

			auto it1 = tree.begin();
			/*for(; it1 != tree.end(); it1++)
				std::cout << *it1 << "(" << (it1.node->color == true ? "r" : "b") << ") ";
			std::cout << std::endl << std::endl;*/

			it1 = tree.find(10);
			auto it2 = tree.find(100);
			/*if (it1 != tree.end()) {
				std::cout << "find " << *it1 << "  color:" << (it1.node->color == true ? "red" : "black") << std::endl;
			}
			if (it2 == tree.end()) {
				std::cout << "not find 100" << std::endl << std::endl;
			}*/

			//std::cout << "tree.size() = " << tree.size() << "  count(11): " << tree.count(11) << std::endl;
			tree.insert_equal(11);
			assert(tree.isBalance());
			it1 = tree.begin();
			/*for (; it1 != tree.end(); ++it1)
				std::cout << *it1 << "(" << (it1.node->color == true ? "r" : "b") << ") ";
			std::cout << std::endl << std::endl;*/
			tree.insert_equal(11);
			assert(tree.isBalance());
			/*std::cout << "tree.size() = " << tree.size() << "  count(11): " << tree.count(11) << std::endl;


			it1 = tree.begin();
			for (; it1 != tree.end(); ++it1)
				std::cout << *it1 << "(" << (it1.node->color == true ? "r" : "b") << ") ";
			std::cout << std::endl << std::endl;*/

			tree.erase(tree.begin(), tree.end());
			assert(tree.isBalance());

			it1 = tree.begin();
			/*std::cout << "tree.size() = " << tree.size() << std::endl;

			for (; it1 != tree.end(); ++it1)
				std::cout << *it1 << "(" << (it1.node->color == true ? "r" : "b") << ") ";
			std::cout << std::endl << std::endl;*/
		}

		void test2() {
			typedef std::pair<int, std::string> Pair;
			rbTree<std::string, Pair, select2nd<Pair>, less<std::string>> stree;
			stree.insert_unique(Pair(2, "bat"));
			assert(stree.isBalance());
			stree.insert_unique(Pair(2, "bta"));
			assert(stree.isBalance());
			stree.insert_unique(Pair(2, "abt"));
			assert(stree.isBalance());
			stree.insert_unique(Pair(2, "aaa"));
			assert(stree.isBalance());
			stree.insert_unique(Pair(6, "c++"));
			assert(stree.isBalance());
			stree.insert_unique(Pair(7, "is"));
			assert(stree.isBalance());
			stree.insert_unique(Pair(10, "good"));
			assert(stree.isBalance());
			//std::cout << "stree.size() = " << stree.size() << std::endl;

			/*for (auto it = stree.begin(); it != stree.end(); ++it) {
				printf("(%d,%s:%s) ", it->first, it->second.c_str(), (it.node->color == true ? "r" : "b"));
			}
			printf("\n\n");*/

			auto it = stree.find("bat");
			if (it != stree.end()) {
				stree.erase(it);
			}
			assert(stree.isBalance());

			/*for (auto it = stree.begin(); it != stree.end(); ++it) {
				printf("(%d,%s:%s) ", it->first, it->second.c_str(), (it.node->color == true ? "r" : "b"));
			}
			printf("\n\n");*/

			stree.erase("aaa");
			/*for (auto it = stree.begin(); it != stree.end(); ++it) {
				printf("(%d,%s:%s) ", it->first, it->second.c_str(), (it.node->color == true ? "r" : "b"));
			}*/
			//printf("\n\n");
			assert(stree.isBalance());

			/*std::cout << "istr.size() = " << stree.size() << std::endl;
			for (auto it = stree.begin(); it != stree.end(); ++it) {
				printf("(%d,%s:%s) ", it->first, it->second.c_str(), (it.node->color == true ? "r" : "b"));
			}*/
			//printf("\n\n");
		}
		void test3() {
			int a[] = { 16, 3, 7, 11, 9, 26, 18, 14, 15 };
			rbTree<int, int, identify<int>, less<int>> tree;
			for (auto e : a) {
				tree.insert_unique(e);
			}
			assert(tree.isBalance());

			for (int i = 0; i < 100; i++) {
				int x = rand();
				tree.insert_equal(x);
			}
			assert(tree.isBalance());

			for (int i = 0; i < 30; i++) {
				int id = rand() % tree.size();
				auto it = tree.begin();
				while (id-- > 0) {
					it++;
				}
				tree.erase(it);
				assert(tree.isBalance());
			}
		}

		void test() {
			test1();
			test2();
			test3();
		}
	}
}