#pragma once
#include<iostream>
using namespace std;
enum Colour
{
	RED,
	BLACK,
};
template<class T>              //这里的模板统一改成T,即要么是K，要么是pair<K,V>
struct RBTreeNode
{
	RBTreeNode<T>* _left;
	RBTreeNode<T>* _right;
	RBTreeNode<T>* _parent;
	T _data;
	Colour _col;              //给出个参考颜色（用于标识红或者黑）

	RBTreeNode(const T& x)  //红黑树结点的构造函数
		:_left(nullptr)
		, _right(nullptr)
		, _parent(nullptr)
		, _data(x)
		, _col(RED)
	{}

};




template<class T, class Ref, class Ptr>   //T T& T*
struct __TreeIterator
{
	typedef Ref reference;
	typedef Ptr pointer;

	typedef RBTreeNode<T> Node;
	typedef __TreeIterator<T, Ref, Ptr> Self;

	Node* _node;
	__TreeIterator(Node* node)
		:_node(node)
	{}


	Ref operator*()
	{
		return _node->_data;
	}

	Ptr operator->()
	{
		return &_node->_data;
	}

	bool operator != (const Self& s) const
	{
		return _node != s._node;
	}
	bool operator==(const Self& s) const
	{
		return !(_node != s._node);
	}
	Self operator++()                  //前置++
	{
		if (_node->_right)
		{
			Node* left = _node->_right;
			while (left->_left)
			{
				left = left->_left;
			}
			_node = left;
		}
		else
		{
			Node* cur = _node;
			Node* parent = cur->_parent;
			while (parent && cur == parent->_right)
			{
				cur = cur->_parent;
				parent = parent->_parent;
			}
			_node = parent;
		}
		return *this;
	}
	Self operator--()
	{
		if (_node->_left)//根结点的左子树不为空
		{
			Node* right = _node->_left;//那么就去找左子树的最右结点
			while (right->_right)
			{
				right = right->_right;
			}
			_node = right;
		}
		else
		{
			Node* cur = _node;
			Node* parent = cur->_parent;
			while (parent && cur == parent->_left)
			{
				cur = cur->_parent;
				parent = parent->_parent;
			}
			_node = parent;
		}
		return *this;
	}
};

//迭代器适配器
template <class Iterator>
struct ReverseIterator
{
	typedef typename Iterator::reference Ref;
	typedef typename Iterator::pointer Ptr;
	typedef ReverseIterator<Iterator> Self;
	//迭代器萃取?
	ReverseIterator(Iterator it)
		:_it(it)
	{}

	Ref operator*()
	{
		return *_it;
	}
	Ptr operator->()
	{
		return _it.operator->();//?
	}
	Self& operator--()
	{
		++_it;
		return *this;
	}
	Self& operator++()
	{
		--_it;
		return *this;
	}

	bool operator !=(const Self& s) const
	{
		return !(_it == s._it);
	}


	Iterator _it;
};


template<class K, class T, class KeyOfT>
class RBTree
{
	typedef RBTreeNode<T> Node;
public:
	typedef __TreeIterator<T, T&, T*> iterator;
	typedef __TreeIterator<T, const T&, const T*> const_iterator;
	typedef ReverseIterator<iterator> reverse_iterator;

	reverse_iterator rbegin()
	{
		Node* right = _root;
		while (right && right->_right)
		{
			right = right->_right;
		}
		return reverse_iterator(iterator(right));
	}

	reverse_iterator rend()
	{
		return  reverse_iterator(iterator(nullptr));
	}

	iterator begin()
	{
		Node* left = _root;
		while (left && left->_left)
		{
			left = left->_left;
		}
		return iterator(left);
	}

	iterator end()
	{
		return iterator(nullptr);
	}

	RBTree()
		:_root(nullptr)      //构造函数初始化
	{}


	//拷贝构造和operator自己实现

	void Destroy(Node* root)//销毁函数
	{

		if (root == nullptr)
			return;
		Destroy(root->_left);  //通过不断递归来去实现，类比二叉搜索树
		Destroy(root->_right);
		delete root;
	}
	~RBTree()            //析构函数——去调用销毁函数
	{
		Destroy(_root);
	}


	Node* Find(const K& key)  //查找（类比搜索二叉树）
	{
		KeyOfT oft;
		Node* cur = _root;
		while (cur)
		{
			if (oft(cur->_data) > key)
			{
				cur = cur->_left;
			}
			else if (oft(cur->_data) < key)
			{
				cur = cur->_right;
			}
			else
			{
				return cur;
			}
		}
		return nullptr;
	}

	pair<iterator, bool> insert(const T& data)//插入
	{
		KeyOfT oft;

		if (_root == nullptr)
		{
			_root = new Node(data);
			_root->_col = BLACK;
			return make_pair(_root, true);
		}

		Node* parent = nullptr;
		Node* cur = _root;
		while (cur)
		{
			if (oft(cur->_data) < oft(data))//如果要实现mutimap 和 mutiset ，那就是(oft(cur->_data) <= oft(data))
			{
				parent = cur;
				cur = cur->_right;
			}
			else if (oft(cur->_data) > oft(data))
			{
				parent = cur;
				cur = cur->_left;
			}
			else
			{
				return make_pair(iterator(_root), false);
			}
		}
		Node* newnode = new Node(data);
		newnode->_col = RED;               //标记为红

		if (oft(parent->_data) < oft(data))
		{
			parent->_right = newnode;
			newnode->_parent = parent;
		}
		else
		{
			parent->_left = newnode;
			newnode->_parent = parent;
		}
		cur = newnode;                         //前面的和搜索二叉树的插入完全一样，就标记了一下颜色。这里不再过多赘述


		while (parent && parent->_col == RED)  //如果父亲存在，且颜色为红色就要处理
		{
			//关键看叔叔
			Node* grandfather = parent->_parent;//并且祖父一定存在
			if (parent == grandfather->_left)   //如果父亲是祖父的左孩子
			{
				Node* uncle = grandfather->_right;    //那么叔叔就是祖父的右孩子
				if (uncle && uncle->_col == RED)      //如果叔叔存在且为红（情况一）
				{
					parent->_col = uncle->_col = BLACK;//把父亲和叔叔变成黑
					grandfather->_col = RED;    //祖父变成红

					//继续往上处理
					cur = grandfather;         //将祖父的位置给孙子
					parent = cur->_parent;     //父亲变为祖父的父亲
				}
				else  //情况2+3：叔叔不存在或者叔叔存在且为黑
				{
					//情况2：单旋
					if (cur == parent->_left)   //如果孩子是父亲的左孩子
					{
						RotateR(grandfather);  //右单旋
						grandfather->_col = RED;  //再变色
						parent->_col = BLACK;
					}
					else//情况3：双旋
					{
						RotateL(parent);
						RotateR(grandfather);
						cur->_col = BLACK;         //最终变的还是祖父的颜色和父亲的颜色
						grandfather->_col = RED;    //祖父的颜色变成红，父亲的颜色变成黑
					}
					break;
				}
			}
			else  //parent == grandparent -> _right   情况是相同的
			{
				Node* uncle = grandfather->_left;
				if (uncle && uncle->_col == RED)//情况1
				{
					uncle->_col = BLACK;
					grandfather->_col = RED;

					cur = grandfather;
					parent = cur->_parent;
				}
				else//情况2+3
				{
					if (cur == parent->_right)
					{
						RotateL(grandfather);
						parent->_col = BLACK;
						grandfather->_col = RED;
					}
					else //cur为父亲的左
					{
						//双旋
						RotateR(parent);
						RotateL(grandfather);
						cur->_col = BLACK;
						grandfather->_col = RED;
					}
					//插入结束
					break;
				}
			}
		}
		_root->_col = BLACK;
		return make_pair(iterator(newnode), true);
	}

	//剩余的就不解释了，和搜索二叉树、AVLTree都是一样的道理
	//只不过这里的旋转就不需要再调节平衡因子了。
	void RotateLR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;

		RotateL(parent->_left);
		RotateR(parent);
	}

	void RotateRL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		RotateR(parent->_right);
		RotateL(parent);

	}


	void RotateL(Node* parent)    //左单旋
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		Node* parentparent = parent->_parent;
		parent->_right = subRL;
		if (subRL)
		{
			subRL->_parent = parent;
		}   //两个一组
		subR->_left = parent;
		parent->_parent = subR;//两个一组

		//连接主体，找到组织
		if (parent == _root)
		{
			_root = subR;
			subR->_parent = nullptr;
		}
		else
		{
			if (parentparent->_left == parent)
			{
				parentparent->_left = subR;

			}
			else
			{
				parentparent->_right = subR;

			}
			subR->_parent = parentparent;
		}
	}

	void RotateR(Node* parent) //右单旋
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		parent->_left = subLR;
		if (subLR)
		{
			subLR->_parent = parent;
		}
		subL->_right = parent;
		Node* parentparent = parent->_parent;
		parent->_parent = subL;

		if (parent == _root)
		{
			_root = subL;
			_root->_parent = nullptr;
		}
		else
		{
			if (parentparent->_left == parent)
			{
				parentparent->_left = subL;
			}
			else
			{
				parentparent->_right = subL;
			}
			subL->_parent = parentparent;
		}
	}

	bool _CheckBlance(Node* root, int blackNum, int count)
	{
		if (root == nullptr)
		{
			if (count != blackNum)
			{
				cout << "黑色节点数量不等" << endl;
			}
			return true;
		}
		if (root->_col == RED && root->_parent->_col == RED)
		{
			cout << "存在连续的红色结点" << endl;
			return false;
		}
		if (root->_col == BLACK)
		{
			count++;
		}
		return _CheckBlance(root->_left, blackNum, count)
			&& _CheckBlance(root->_right, blackNum, count);
	}

	bool CheckBlance()
	{
		if (_root == nullptr)
		{
			return true;
		}

		if (_root->_col == RED)
		{
			cout << "根结点是红色的" << endl;
			return false;
		}

		//找最左路径做黑色结点的参考值
		int blackNum = 0;
		Node* left = _root;
		while (left)
		{
			if (left->_col == BLACK)
			{
				blackNum++;
			}

			left = left->_left;
		}
		int count = 0;
		return _CheckBlance(_root, blackNum, count);
	}
private:
	Node* _root;
};