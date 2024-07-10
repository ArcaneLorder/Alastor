#pragma once
#include<iostream>
using namespace std;
enum Colour
{
	RED,
	BLACK,
};
template<class T>              //�����ģ��ͳһ�ĳ�T,��Ҫô��K��Ҫô��pair<K,V>
struct RBTreeNode
{
	RBTreeNode<T>* _left;
	RBTreeNode<T>* _right;
	RBTreeNode<T>* _parent;
	T _data;
	Colour _col;              //�������ο���ɫ�����ڱ�ʶ����ߺڣ�

	RBTreeNode(const T& x)  //��������Ĺ��캯��
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
	Self operator++()                  //ǰ��++
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
		if (_node->_left)//��������������Ϊ��
		{
			Node* right = _node->_left;//��ô��ȥ�������������ҽ��
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

//������������
template <class Iterator>
struct ReverseIterator
{
	typedef typename Iterator::reference Ref;
	typedef typename Iterator::pointer Ptr;
	typedef ReverseIterator<Iterator> Self;
	//��������ȡ?
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
		:_root(nullptr)      //���캯����ʼ��
	{}


	//���������operator�Լ�ʵ��

	void Destroy(Node* root)//���ٺ���
	{

		if (root == nullptr)
			return;
		Destroy(root->_left);  //ͨ�����ϵݹ���ȥʵ�֣���ȶ���������
		Destroy(root->_right);
		delete root;
	}
	~RBTree()            //������������ȥ�������ٺ���
	{
		Destroy(_root);
	}


	Node* Find(const K& key)  //���ң����������������
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

	pair<iterator, bool> insert(const T& data)//����
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
			if (oft(cur->_data) < oft(data))//���Ҫʵ��mutimap �� mutiset ���Ǿ���(oft(cur->_data) <= oft(data))
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
		newnode->_col = RED;               //���Ϊ��

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
		cur = newnode;                         //ǰ��ĺ������������Ĳ�����ȫһ�����ͱ����һ����ɫ�����ﲻ�ٹ���׸��


		while (parent && parent->_col == RED)  //������״��ڣ�����ɫΪ��ɫ��Ҫ����
		{
			//�ؼ�������
			Node* grandfather = parent->_parent;//�����游һ������
			if (parent == grandfather->_left)   //����������游������
			{
				Node* uncle = grandfather->_right;    //��ô��������游���Һ���
				if (uncle && uncle->_col == RED)      //������������Ϊ�죨���һ��
				{
					parent->_col = uncle->_col = BLACK;//�Ѹ��׺������ɺ�
					grandfather->_col = RED;    //�游��ɺ�

					//�������ϴ���
					cur = grandfather;         //���游��λ�ø�����
					parent = cur->_parent;     //���ױ�Ϊ�游�ĸ���
				}
				else  //���2+3�����岻���ڻ������������Ϊ��
				{
					//���2������
					if (cur == parent->_left)   //��������Ǹ��׵�����
					{
						RotateR(grandfather);  //�ҵ���
						grandfather->_col = RED;  //�ٱ�ɫ
						parent->_col = BLACK;
					}
					else//���3��˫��
					{
						RotateL(parent);
						RotateR(grandfather);
						cur->_col = BLACK;         //���ձ�Ļ����游����ɫ�͸��׵���ɫ
						grandfather->_col = RED;    //�游����ɫ��ɺ죬���׵���ɫ��ɺ�
					}
					break;
				}
			}
			else  //parent == grandparent -> _right   �������ͬ��
			{
				Node* uncle = grandfather->_left;
				if (uncle && uncle->_col == RED)//���1
				{
					uncle->_col = BLACK;
					grandfather->_col = RED;

					cur = grandfather;
					parent = cur->_parent;
				}
				else//���2+3
				{
					if (cur == parent->_right)
					{
						RotateL(grandfather);
						parent->_col = BLACK;
						grandfather->_col = RED;
					}
					else //curΪ���׵���
					{
						//˫��
						RotateR(parent);
						RotateL(grandfather);
						cur->_col = BLACK;
						grandfather->_col = RED;
					}
					//�������
					break;
				}
			}
		}
		_root->_col = BLACK;
		return make_pair(iterator(newnode), true);
	}

	//ʣ��ľͲ������ˣ���������������AVLTree����һ���ĵ���
	//ֻ�����������ת�Ͳ���Ҫ�ٵ���ƽ�������ˡ�
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


	void RotateL(Node* parent)    //����
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		Node* parentparent = parent->_parent;
		parent->_right = subRL;
		if (subRL)
		{
			subRL->_parent = parent;
		}   //����һ��
		subR->_left = parent;
		parent->_parent = subR;//����һ��

		//�������壬�ҵ���֯
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

	void RotateR(Node* parent) //�ҵ���
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
				cout << "��ɫ�ڵ���������" << endl;
			}
			return true;
		}
		if (root->_col == RED && root->_parent->_col == RED)
		{
			cout << "���������ĺ�ɫ���" << endl;
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
			cout << "������Ǻ�ɫ��" << endl;
			return false;
		}

		//������·������ɫ���Ĳο�ֵ
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