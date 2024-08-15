#include "mapTest.h"
#include "testHelper.h"
#include <assert.h>
#include "../functors.h"
namespace mySTL {
	namespace mapTest {
		void test1() {
			myMap<std::string, int> map1;
			map1["something"] = 69;
			map1["anything"] = 199;
			map1["that thing"] = 50;
			stdMap<std::string, int> map2;
			map2["something"] = 69;
			map2["anything"] = 199;
			map2["that thing"] = 50;
			assert(mySTL::testHelper::equal_map(map1, map2));

			myMap<std::string, int> iter1(map1.find("anything"), map1.end());
			stdMap<std::string, int> iter2(map2.find("anything"), map2.end());
			assert(mySTL::testHelper::equal_map(iter1, iter2));

			mySTL::map<std::string, int, mySTL::greater<std::string>> m1(map1.begin(), map1.end());
			std::map<std::string, int, mySTL::greater<std::string>> m2(map2.begin(), map2.end());
			assert(mySTL::testHelper::equal_map(m1, m2));

			myMap<std::string, int> m3(map1);
			stdMap<std::string, int> m4(map2);
			assert(mySTL::testHelper::equal_map(m3, m4));

			myMap<std::string, int> m5;
			m5 = m3;
			stdMap<std::string, int> m6;
			m6 = m4;
			assert(mySTL::testHelper::equal_map(m5, m6));
		}
		void test2() {
			myMap<int, int> m;
			assert(m.empty());
			m[42] = 13;
			assert(!m.empty());
			m[4] = 22;
			assert(m.size() == 2);
			m[4] = 33;
			assert(m.size() == 2);
			m.clear();
			assert(m.empty());
		}
		void test3() {
			myMap<int, int> m1;
			stdMap<int, int> m2;

			for (int i = 0; i < 2000; i++) {
				int x = rand();
				m1.insert({x, i});
				m2.insert({x, i});
			}
			m1.insert({ 102, 7 });
			m2.insert({ 102, 7 });
			m1.insert({ 447, 13 });
			m2.insert({ 447, 13 });
			assert(mySTL::testHelper::equal_map(m1, m2));
			myMap<int, int> m3;
			m3.insert(m1.begin(), m1.end());
			stdMap<int, int> m4;
			m4.insert(m2.begin(), m2.end());
			assert(mySTL::testHelper::equal_map(m3, m2));
			assert(mySTL::testHelper::equal_map(m1, m4));

			m1.erase(102);
			m2.erase(102);
			assert(mySTL::testHelper::equal_map(m1, m2));

			auto it1 = m1.begin();
			auto it2 = m2.begin();
			for (int i = 0; i < 30; i++) {
				it1++;
				it2++;
			}
			m1.erase(it1);
			m2.erase(it2);
			assert(mySTL::testHelper::equal_map(m1, m2));
			it1 = m1.begin();
			it2 = m2.begin();
			for (int i = 0; i < 30; i++) {
				it1++;
				it2++;
			}
			auto it3 = it1;
			auto it4 = it2;
			for (int i = 0; i < 70; i++) {
				it3++;
				it4++;
			}
			m1.erase(it1, it3);
			m2.erase(it2, it4);
			assert(mySTL::testHelper::equal_map(m1, m2));

			m1.swap(m3);
			m2.swap(m4);
			assert(mySTL::testHelper::equal_map(m1, m2));
			assert(m1.count(102) == m2.count(102));
			assert(m1.find(102) != m1.end());
			assert(m1.find(-102) == m1.end());
		}
		void test4() {
			const std::map<int, const char*> m
			{
				{0, "zero"},
				{1, "one"},
				{2, "two"}
			};

			auto p = m.equal_range(1);
			for (auto& q = p.first; q != p.second; ++q)
				assert(q->first == 1 && q->second == "one");

			assert(p.second == m.find(2));
			auto pp = m.equal_range(-1);
			assert(pp.first == m.begin());
			assert(pp.second == m.begin());
			auto ppp = m.equal_range(3);
			assert(ppp.first == m.end());
			assert(ppp.second == m.end());
		}
		struct ModCmp
		{
			bool operator()(int lhs, int rhs) const
			{
				return (lhs % 97) < (rhs % 97);
			}
		};
		void test5() {
			mySTL::map<int, char, ModCmp> cont;
			cont[1] = 'a';
			cont[2] = 'b';
			cont[3] = 'c';
			cont[4] = 'd';
			cont[6] = 'e';
			auto comp_func = cont.key_comp();
			for (const auto it : cont)
			{
				const bool before = comp_func(it.first, 100);
				const bool after = comp_func(100, it.first);

				std::cout << "Key (" << it.first << ',' << it.second << ") ";
				if (!before && !after)
					std::cout << "equivalent to key (100)\n";
				else if (before)
					std::cout << "goes before key (100)\n";
				else if (after)
					std::cout << "goes after key (100)\n";
				else
					assert(false);
			}
			auto comp_func1 = cont.value_comp();
			const std::pair<int, char> val(100, 'a');
			for (auto it : cont)
			{
				const bool before = comp_func1(it, val);
				const bool after = comp_func1(val, it);

				std::cout << '(' << it.first << ',' << it.second << ") ";
				if (!before && !after)
					std::cout << "equivalent to key (" << val.first << ")\n";
				else if (before)
					std::cout << "goes before key (" << val.first << ")\n";
				else if (after)
					std::cout << "goes after key (" << val.first << ")\n";
				else
					assert(false);
			}
		}
		void test() {
			test1();
			test2();
			test3();
			test4();
			test5();
		}
	}
}