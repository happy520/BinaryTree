#include <iostream>
#include <cstdlib>
#include <vector>
#include <cassert>
#include <stack>
#include <queue>
#include <cstdarg>

using namespace std;

class BinaryTreeNode
{
public:
	BinaryTreeNode(int value=0):m_Value(value),m_Left(nullptr),m_Right(nullptr){}
	~BinaryTreeNode(){}

	int m_Value;
	BinaryTreeNode* m_Left;
	BinaryTreeNode* m_Right;
};

BinaryTreeNode* ConstructBinaryTreeCore(const vector<int>& preOrder, const int preStart, const int preEnd,
	const vector<int>& inOrder, const int inStart, const int inEnd)
{
	assert(preEnd-preStart == inEnd-inStart);
	assert(preEnd >= preStart);

	int rootValue = preOrder[preStart];
	BinaryTreeNode* root = new BinaryTreeNode(rootValue);

	if(preEnd == preStart)
	{
		return root;
	}

	int leftTreeLength = -1;
	for(int index=inStart; index<=inEnd; ++index)
	{
		if(inOrder[index] == rootValue)
		{
			leftTreeLength = index - inStart;
			break;
		}
	}

	if(leftTreeLength < 0)
		throw new exception("Invalid parameters!");

	if(leftTreeLength > 0)
		root->m_Left = ConstructBinaryTreeCore(preOrder, preStart+1, preStart+1+leftTreeLength-1, inOrder, inStart, inStart+leftTreeLength-1);
	if(preStart+1+leftTreeLength <= preEnd)
		root->m_Right = ConstructBinaryTreeCore(preOrder, preStart+1+leftTreeLength, preEnd, inOrder, inStart+leftTreeLength+1, inEnd);
	return root;
}

BinaryTreeNode* ConstructBinaryTree(const vector<int>& preOrder, const vector<int>& inOrder)
{
	if(preOrder.size()!=inOrder.size() || preOrder.empty())
		throw new exception("Invalid parameters!");

	const int length = preOrder.size();
	return ConstructBinaryTreeCore(preOrder, 0, length-1, inOrder, 0, length-1);
}

vector<BinaryTreeNode*> PreOrderTraverse_Recursion(BinaryTreeNode* root)
{
	vector<BinaryTreeNode*> result;

	if(root != nullptr)
	{
		result.push_back(root);

		auto leftResult = PreOrderTraverse_Recursion(root->m_Left);
		result.insert(result.end(), leftResult.begin(), leftResult.end());

		auto rightResult = PreOrderTraverse_Recursion(root->m_Right);
		result.insert(result.end(), rightResult.begin(), rightResult.end());
	}

	return result;
}

vector<BinaryTreeNode*> PreOrderTraverse_Iteration1(BinaryTreeNode* root)
{
	vector<BinaryTreeNode*> result;
	stack<BinaryTreeNode*> aux;

	if(root != nullptr)
	{
		aux.push(root);

		while(!aux.empty())
		{
			BinaryTreeNode* node = aux.top();
			aux.pop();
			result.push_back(node);

			if(node->m_Right) aux.push(node->m_Right);
			if(node->m_Left) aux.push(node->m_Left);
		}
	}

	return result;
}

void VisitAlongVine(BinaryTreeNode* node, vector<BinaryTreeNode*>& result, stack<BinaryTreeNode*>& aux)
{
	while(node)
	{
		result.push_back(node);
		aux.push(node->m_Right);
		node = node->m_Left;
	}
}

vector<BinaryTreeNode*> PreOrderTraverse_Iteration2(BinaryTreeNode* node)
{
	vector<BinaryTreeNode*> result;
	stack<BinaryTreeNode*> aux;

	while(true)
	{
		VisitAlongVine(node, result, aux);
		if(aux.empty()) break;
		node = aux.top();
		aux.pop();
	}

	return result;
}

vector<BinaryTreeNode*> InOrderTraverse_Recursion(BinaryTreeNode* node)
{
	vector<BinaryTreeNode*> result;

	if(node)
	{
		auto leftResult = InOrderTraverse_Recursion(node->m_Left);
		result.insert(result.end(), leftResult.begin(), leftResult.end());

		result.push_back(node);

		auto rightResult = InOrderTraverse_Recursion(node->m_Right);
		result.insert(result.end(), rightResult.begin(), rightResult.end());
	}

	return result;
}

void GoAlongVine(BinaryTreeNode* node, stack<BinaryTreeNode*>& aux)
{
	while(node)
	{
		aux.push(node);
		node = node->m_Left;
	}
}

vector<BinaryTreeNode*> InOrderTraverse_Iteration(BinaryTreeNode* node)
{
	vector<BinaryTreeNode*> result;
	stack<BinaryTreeNode*> aux;

	while(true)
	{
		GoAlongVine(node, aux);
		if(aux.empty()) break;
		node = aux.top();
		aux.pop();
		result.push_back(node);
		node = node->m_Right;
	}

	return result;
}

vector<BinaryTreeNode*> PostOrderTraversal_Recursion(BinaryTreeNode* node)
{
	vector<BinaryTreeNode*> result;

	if(node)
	{
		auto leftResult = PostOrderTraversal_Recursion(node->m_Left);
		result.insert(result.end(), leftResult.begin(), leftResult.end());

		auto rightResult = PostOrderTraversal_Recursion(node->m_Right);
		result.insert(result.end(), rightResult.begin(), rightResult.end());

		result.push_back(node);
	}

	return result;
}

// HLVFL: highest leaf visible from left
void GotoHLVFL(stack<BinaryTreeNode*>& aux)
{
	assert(!aux.empty());

	while(true)
	{
		auto node = aux.top();
		if(node->m_Left)
		{
			if(node->m_Right)
				aux.push(node->m_Right);
			aux.push(node->m_Left);
		}
		else
		{
			if(node->m_Right) 
				aux.push(node->m_Right);
			else
				break;
		}
	}
}

vector<BinaryTreeNode*> PostOrderTraversal_Iteration(BinaryTreeNode* node)
{
	vector<BinaryTreeNode*> result;
	stack<BinaryTreeNode*> aux;

	if(node)
	{
		aux.push(node);
		while(!aux.empty())
		{
			const auto current = aux.top();
			if(current->m_Right!=node && current->m_Left!=node)
				GotoHLVFL(aux);
			node = aux.top();
			aux.pop();
			result.push_back(node);
		}
	}

	return result;
}

vector<BinaryTreeNode*> LevelTraverse(BinaryTreeNode* node)
{
	vector<BinaryTreeNode*> result;
	queue<BinaryTreeNode*> aux;

	if(node)
	{
		aux.push(node);
		while(!aux.empty())
		{
			node = aux.front();
			aux.pop();
			result.push_back(node);

			if(node->m_Left) aux.push(node->m_Left);
			if(node->m_Right) aux.push(node->m_Right);
		}
	}

	return result;
}

void CheckMultipleVector(vector<int>& theoreticalValue, vector<BinaryTreeNode*>* v, ...)
{
	int count = 0;

	auto CheckVector = [&count, &theoreticalValue](vector<BinaryTreeNode*>& test)
	{
		bool isPass = true;
		if(test.size() == theoreticalValue.size())
		{
			for(int i=0; i<test.size(); ++i)
			{
				if(test[i]->m_Value != theoreticalValue[i])
				{
					isPass = false;
					break;
				}
			}
		}
		else
		{
			isPass = false;
		}

		cout<<"check vector "<<++count<<" : ";
		if(isPass)
			cout<<"Pass!"<<endl;
		else
			cout<<"Failed!"<<endl;
	};

	CheckVector(*v);

	va_list ap;
	va_start(ap, v);
	v = va_arg(ap, vector<BinaryTreeNode*>*);
	while(v) //nullptr作为不定参数的终止符
	{
		CheckVector(*v);
		v = va_arg(ap, vector<BinaryTreeNode*>*);
	}

	cout<<endl;
}

void PrintVector(vector<BinaryTreeNode*>& v)
{
	for(auto it=v.cbegin(); it!=v.end(); ++it)
	{
		cout<<(*it)->m_Value<<endl;
	}
	cout<<endl;
}

void Test1()
{
	const int length = 8;
	int preOrderInput[length] = {1,2,4,7,3,5,6,8};
	vector<int> preOrder(preOrderInput, preOrderInput+length);
	int inOrderInput[length] = {4,7,2,1,5,3,8,6};
	vector<int> inOrder(inOrderInput, inOrderInput+length);
	int postOrderInput[length] = {7,4,2,5,8,6,3,1};
	vector<int> postOrder(postOrderInput, postOrderInput+length);
	int levelOrderInput[length] = {1,2,3,4,5,6,7,8};
	vector<int> levelOrder(levelOrderInput, levelOrderInput+length);

	try
	{
		BinaryTreeNode* root = ConstructBinaryTree(preOrder, inOrder);

		cout<<"*** PreOrder Traversal ***"<<endl;
		auto preOrderRecursion = PreOrderTraverse_Recursion(root);
		PrintVector(preOrderRecursion);

		auto preOrderI1 = PreOrderTraverse_Iteration1(root);
		PrintVector(preOrderI1);

		auto preOrderI2 = PreOrderTraverse_Iteration2(root);
		PrintVector(preOrderI2);

		CheckMultipleVector(preOrder, &preOrderRecursion, &preOrderI1, &preOrderI2, nullptr);

		cout<<"*** InOrder Traversal ***"<<endl;
		auto inOrderRecursion = InOrderTraverse_Recursion(root);
		PrintVector(inOrderRecursion);

		auto inOrderIteration = InOrderTraverse_Iteration(root);
		PrintVector(inOrderIteration);

		CheckMultipleVector(inOrder, &inOrderRecursion, &inOrderIteration, nullptr);

		cout<<"*** PostOrder Traversal ***"<<endl;
		auto postOrderRecursion = PostOrderTraversal_Recursion(root);
		PrintVector(postOrderRecursion);

		auto postOrderIteration = PostOrderTraversal_Iteration(root);
		PrintVector(postOrderIteration);

		CheckMultipleVector(postOrder, &postOrderRecursion, &postOrderIteration, nullptr);

		cout<<"*** Level Traversal ***"<<endl;
		auto levelOrderIteration = LevelTraverse(root);
		PrintVector(levelOrderIteration);

		CheckMultipleVector(levelOrder, &levelOrderIteration, nullptr);
	}
	catch (exception& ex)
	{
		cout<<ex.what()<<endl;
	}
}

int main(int argc, char* argv[])
{
	Test1();

	system("pause");
	return 0;
}