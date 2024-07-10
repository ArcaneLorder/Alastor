#pragma once
#include "Alastor.h"
#include "BRTree.h"
namespace Alastor
{
	template<class K, class V>
	class ALASTOR_API Map
	{
	public:
		struct MapKeyOfT
		{
			const K& operator()(const pair<const K, V>& kv)
			{
				return kv.first;
			}
		};
		typedef typename RBTree<K, pair<const K, V>, MapKeyOfT>::iterator iterator;
		typedef typename RBTree<K, pair<const K, V>, MapKeyOfT>::reverse_iterator reverse_iterator;

		iterator begin()
		{
			return _t.begin();
		}
		iterator end()
		{
			return _t.end();
		}
		reverse_iterator rbegin()
		{
			return _t.rbegin();
		}
		reverse_iterator rend()
		{
			return _t.rend();
		}

		pair<iterator, bool> insert(const pair<const K, V>& kv)
		{
			return	_t.insert(kv);
		}

		pair<iterator, bool> insert(const pair<K, V>& kv)
		{
			return _t.insert(kv);
		}

		V& operator[](const K& key)
		{
			pair<iterator, bool> ret = insert(make_pair(key, V()));
			return ret.first->second;
		}
	private:
		RBTree<K, pair<const K, V>, MapKeyOfT> _t;
	};
}