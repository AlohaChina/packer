#include <bws/packer/GAWrapperArray.hpp>

#include <bws/packer/Packer.hpp>
#include <bws/packer/WrapperGroup.hpp>
#include <bws/packer/InstanceArray.hpp>

#include <iostream>

using namespace std;
using namespace bws::packer;
using namespace samchon::library;
using namespace samchon::protocol;

/* -----------------------------------------------------------
	CONSTRUCTORS
----------------------------------------------------------- */
GAWrapperArray::GAWrapperArray(std::shared_ptr<InstanceArray> instanceArray)
	: super()
{
	this->instanceArray = instanceArray;
	valid = true;
	price = 0.0;
}
GAWrapperArray::GAWrapperArray(const GAWrapperArray &obj)
	: GAWrapperArray(obj.instanceArray)
{
	assign(obj.begin(), obj.end());
}

void GAWrapperArray::constructResult()
{
	if (result.empty() == false)
		return;
	
	// ��ǰ�� ������ �׷�, Product�� WrapperGroup�� 1:1 ��Ī
	for (size_t i = 0; i < size(); i++)
	{
		const shared_ptr<Wrapper> &wrapper = at(i);
		if (result.count(wrapper->key()) == 0)
		{
			WrapperGroup *wrapperGroup = new WrapperGroup(wrapper);
			result.insert({ wrapper->key(), shared_ptr<WrapperGroup>(wrapperGroup) });
		}

		shared_ptr<WrapperGroup> wrapperGroup = result.at(wrapper->key());
		shared_ptr<Instance> instance = instanceArray->at(i);
		
		if (wrapperGroup->allocate(instance) == false)
		{
			// �ϰ� ��ǰ ũ�Ⱑ ���������� Ŀ�� ������ �� ���� ���, 
			// ������ ���⼭���� ��ȿ���� ���Ͽ� ����
			valid = false;
			return;
		}
	}
	
	// ��ȿ�� ���⼭���� ��,
	for (auto it = result.begin(); it != result.end(); it++)
	{
		it->second->optimize(); // ������(�׷캰)���� bin-packing�� �ǽ���
		price += it->second->getPrice(); // ���Ҿ� ���ݵ� �ջ��صд�
	}
	valid = true;
}

/* -----------------------------------------------------------
	GETTERS
----------------------------------------------------------- */
auto GAWrapperArray::getResult() const -> const unordered_map<string, shared_ptr<WrapperGroup>>&
{
	((GAWrapperArray*)this)->constructResult();

	return result;
}

auto GAWrapperArray::operator<(const GAWrapperArray &obj) const -> bool
{
	((GAWrapperArray*)this)->constructResult();
	((GAWrapperArray&)obj) . constructResult();

	if (valid == true && obj.valid == true)
	{
		// ������ ���� ���� �켱
		return this->price < obj.price;
	}
	else if (valid == true && obj.valid == false)
	{
		// ��ȿ�� ���� �켱
		return true;
	}
	else
		return false;
}