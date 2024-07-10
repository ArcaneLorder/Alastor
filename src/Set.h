#pragma once
#include "Alastor.h"
#include "BRTree.h"
namespace Alastor
{
	template<class K>
	class ALASTOR_API set
	{
		struct SetKeyOfT
		{
			const K& operator()(const K& key)
			{
				return key;
			}
		};
	public:
		typedef typename RBTree<K, K, SetKeyOfT>::iterator iterator;
		typedef typename RBTree<K, K, SetKeyOfT>::reverse_iterator reverse_iterator;
		reverse_iterator rbegin()
		{
			return _t.rbegin();
		}
		reverse_iterator rend()
		{
			return _t.rend();
		}
		bool insert(const K& k)
		{
			_t.insert(k);
			return true;
		}
		iterator end()
		{
			return _t.end();
		}
		iterator begin()
		{
			return _t.begin();
		}
	private:
		RBTree<K, K, SetKeyOfT> _t;
	};
}