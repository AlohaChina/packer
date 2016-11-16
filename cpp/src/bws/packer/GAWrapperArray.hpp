#pragma once
#include <bws/packer/API.hpp>

#include <bws/packer/WrapperArray.hpp>

#include <unordered_map>
#include <bws/packer/WrapperGroup.hpp>
#include <bws/packer/InstanceArray.hpp>

namespace bws
{
namespace packer
{
	/**
	 * @brief Sequence list of Wrapper objects.
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class GAWrapperArray
		: public WrapperArray
	{
	private:
		typedef WrapperArray super;

	protected:
		/**
		 * @brief Instance objects to be wrapped.
		 */
		std::shared_ptr<InstanceArray> instanceArray;
		
		std::unordered_map<std::string, std::shared_ptr<WrapperGroup>> result;

		/**
		 * @brief Price of this sequence list.
		 *
		 * @details It stores price deducted by the packing process to block repeated operation.
		 */
		double price;

		/**
		 * @brief Validity of this sequence list.
		 *
		 * @details It stores validity of the sequence list to block repeated operation.
		 */
		bool valid;

	public:
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * @brief Construct from instances.
		 *
		 * @param instanceArray Instances to be wrapped.
		 */
		GAWrapperArray(std::shared_ptr<InstanceArray> instanceArray)
			: super()
		{
			this->instanceArray = instanceArray;
			valid = true;
			price = 0.0;
		};
		
		/**
		 * @brief Copy Constructor.
		 */
		GAWrapperArray(const GAWrapperArray &obj)
			: GAWrapperArray(obj.instanceArray)
		{
			assign(obj.begin(), obj.end());
		};

		virtual ~GAWrapperArray() = default;

	protected:
		/**
		 * @brief Construct result, by packing process.
		 */
		void constructResult()
		{
			if (result.empty() == false)
				return;

			// ��ǰ�� ������ �׷�, Product�� WrapperGroup�� 1:1 ��Ī
			for (size_t i = 0; i < size(); i++)
			{
				const std::shared_ptr<Wrapper> &wrapper = at(i);
				if (result.count(wrapper->key()) == 0)
				{
					WrapperGroup *wrapperGroup = new WrapperGroup(wrapper);
					result.insert({ wrapper->key(), std::shared_ptr<WrapperGroup>(wrapperGroup) });
				}

				std::shared_ptr<WrapperGroup> wrapperGroup = result.at(wrapper->key());
				std::shared_ptr<Instance> instance = instanceArray->at(i);

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
		};

	public:
		/* -----------------------------------------------------------
			GETTERS
		----------------------------------------------------------- */
		/**
		 * @brief Get optimization result.
		 *
		 * @return Const reference of result map.
		 */
		auto getResult() const -> const std::unordered_map<std::string, std::shared_ptr<WrapperGroup>>&
		{
			((GAWrapperArray*)this)->constructResult();

			return result;
		};

		/**
		 * @brief Whether this sequence list is inferior?
		 */
		auto operator<(const GAWrapperArray &obj) const -> bool
		{
			((GAWrapperArray*)this)->constructResult();
			((GAWrapperArray&)obj).constructResult();

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
		};
	};
};
};