#include "pppch.h"
#include "entt.hpp"

struct Comp_0 {
	float x = 0, y = 0;
};

struct Comp_1 {
	float dx = 0, dy = 0;
};

namespace EnTT_Wiki{
	// EnTT提供了一个没有bitset的实体组件系统，它不要求用户在编译时或运行时指定组件集。
	// 这就是为什么用户可以简单地实例化core类 :
	// entt::registry registry; // 创建 registry

	/* 按需使用
	    EnTT的设计完全围绕着这样一个原则:用户只需要为他们想要的东西付费。
	    当使用实体组件系统时，通常要在性能和内存使用之间进行权衡。速度越快，占用的内存就越多。
	    更糟糕的是，有些方法倾向于严重影响其他功能，如组件的构建和销毁，以支持迭代，即使它不是严格要求的。
	    事实上，为了减少内存使用量并有时获得更好的总体性能，在非关键路径上稍差的性能是正确的代价，我总是想知道为什么这种工具没有给我留下选择的余地。
	    EnTT采用了一种完全不同的方法。它从基本的数据结构中获得最好的效果，并让用户在需要时为更高的性能支付更多费用。
	    到目前为止，这个选择已经被证明是一个好的选择，我真的希望它可以对除了我以外的很多人。*/
	
 
	/* EnTT是这样的:在每一个时刻，一对(T* ， size)都可以直接访问给定组件类型T的所有实例。
	    这是一个指导方针和一个设计决策，它影响了许多选择，有好有坏。我不能说它对读者是否有用，但值得提一下，因为它是这个图书馆的基石之一。
	    下面描述的许多工具都提供了获取这些信息的可能性，并围绕这一需求进行了设计。
	    剩下的就是实验和发明新东西的欲望，希望能够成功。*/
	

	/* 无状态的系统
	    EnTT被设计成可以与无状态系统一起工作。换句话说，所有的系统都可以是自由的函数，并且不需要将它们定义为类(尽管没有什么可以阻止用户这样做)。
	    这是可能的，因为用户将与之一起工作的主类提供了作为应用程序唯一的真实来源所需的所有内容。*/


	/* 池
	    在EnTT中，组件池是通过稀疏集的专门化版本提供的。
	    每个池包含单个组件的所有实例，以及它所分配的所有实体。稀疏数组也被分页，以避免在某些情况下浪费内存，而打包的数组并没有明显的原因。
	    除了使用的元素数量之外，池还可以在任何时候提供指向它们所包含的实体和组件的打包列表的指针。出于这个原因，池可以重新排列它们的项，以保持内部数组的紧密排列并最大化性能。
	    目前，虽然正在开发一种更灵活和用户友好的模型，但在一定的限制范围内对池进行专门化是可能的。*/

	/* ECS
	    注册中心可以存储和管理实体，也可以创建视图和组来迭代底层数据结构。
        类模板basic_registry允许用户决定表示实体的首选类型。
        因为std::uint32_t对于几乎所有的情况都足够大，也存在enum类entt::entity包装它和别名entt::registry为entt::basic_registry<entt::entity>。
        实体由实体标识符表示。实体标识符携带有关实体本身及其版本的信息。
        用户定义的标识符可以通过枚举类和定制类型引入，这些类和定制类型专门化了entt_traits。为此，entt_traits也被定义为一个sfinae友好的类模板。
        理论上，整型也可以用作实体标识符，尽管这在将来可能会中断，一般不建议这样做。*/

	void Wiki(){
		entt::registry registry;
		///************///
		///***Entity***///
		///************/// 
		// 构造一个没有组件的实体，并返回它的标识符
			entt::entity entity = registry.create();

		// 销毁一个实体及其所有组件
			registry.destroy(entity);

		// create成员函数也接受提示，并具有一个重载，该重载获得两个迭代器，可用于一次高效地生成多个实体。类似地，destroy成员函数也适用于一系列实体:
			// 销毁范围内的所有实体
			auto view = registry.view<Comp_0, Comp_1>();
			registry.destroy(view.begin(), view.end());

		// 当一个实体被销毁时，注册中心可以在内部用一个稍微不同的标识符自由地重用它。特别是，实体的版本在销毁后会增加(除非使用强制版本的重载而不是默认版本)。
	    // 用户可以通过探测标识符来了解标识符所携带的信息:
			// 如果实体仍然有效则返回true，否则返回false
			bool b = registry.valid(entity);

			// 获取实体标识符中包含的版本
			auto version = registry.version(entity);

			//获取给定实体的实际版本
			auto current = registry.current(entity);


		///***************///
		///***Component***///
		///***************/// 
		// 组件可以在任何时候分配给实体或从实体中删除。至于实体，注册中心提供了一组用于处理组件的函数。
			// emplace成员函数模板创建、初始化给定组件，并将其赋值给实体。如果存在，它接受可变数量的参数来构造组件本身:
			registry.emplace<Comp_0>(entity, 0.0f, 0.0f);
			// ...
			auto &vel = registry.emplace<Comp_0>(entity);
			vel.x = 0.0;
			vel.y = 0.0;
			/* 默认存储在内部检测聚合类型，并在可能时利用聚合初始化。
			   因此，根据该语言的规则，没有必要为每种类型定义构造函数。*/

			// 另一方面，insert可以使用范围，可以用于:
				// 当一个类型被指定为模板参数或一个实例被作为参数传递时，将同一个组件一次性赋值给所有实体:
					// 默认初始化类型由复制分配给所有实体
					//registry.insert(first, last);
					// 用户定义的实例复制分配给所有实体
					//registry.insert(from, to, position{ 0.0f, 0.0f });
				
				// 当提供了一个范围时，为实体分配一组组件(组件范围的长度必须与实体的长度相同) :
					// first和last指定实体的范围，instances指向组件范围的第一个元素
					//registry.insert<position>(first, last, instances);

			// 如果一个实体已经拥有给定的组件，可以使用replace和patch成员函数模板来更新它:
				// 替换组件的位置
				registry.patch<Comp_0>(entity, [](auto& pos) { pos.x = pos.y = 0.0f; });
				// 从参数列表中构造一个新的实例，并替换该组件
				registry.replace<Comp_0>(entity, 0.0f, 0.0f);
				// 当不知道某个实体是否已经拥有某个组件的实例时，可以使用emplace_or_replace函数代替:
				registry.emplace_or_replace<Comp_0>(entity, 0.f, 0.0f);
				// 这是一个稍微快一点的替代方案:
				if (registry.has<Comp_0>(entity)) {
					registry.replace<Comp_0>(entity, 0.0f, 0.0f);
				} else {
					registry.emplace<Comp_0>(entity, 0.0f, 0.0f);
				}
			// 成员函数all_of和any_of返回 "是否有集合中的所有组件" 或 "有其中任何一个组件":
				// 如果实体拥有所有给定的组件，则为true
				//bool all = registry.all_of<Comp_0, Comp_1>(entity);

				// 如果实体至少有一个给定的组件，则为true
				//bool any = registry.any_of<Comp_0, Comp_1>(entity);

			// 如果目标是从拥有组件的实体中删除组件，可以使用remove成员函数模板:
			registry.remove<Comp_0>(entity);
			// 当怀疑实体是否拥有组件时，请使用remove_if_exists成员函数。如果没有组件它会安全地返回给调用者:
			registry.remove_if_exists<Comp_0>(entity);

			// clear成员函数的工作原理类似，可以用于以下两种情况:
				// 从拥有这些组件的实体中移除所有实例:
				registry.clear<Comp_0>();
				// 或立即销毁注册表中的所有实体:
				registry.clear();

			// 最后，对组件的引用可以这样简单地检索:
				const auto& c_registry = registry;
				// const 和 非const 引用
				const auto& c_comp0 = c_registry.get<Comp_0>(entity);
				auto& r_comp0 = registry.get<Comp_0>(entity);
				// const 和 非const 引用
				const auto [C_comp0, C_comp1] = c_registry.get<Comp_0, Comp_1>(entity);
				auto [comp0, comp1] = registry.get<Comp_0, Comp_1>(entity);

				// get成员函数模板可以直接访问存储在注册表底层数据结构中的实体的组件。
				// 还有一个名为try_get的替代成员函数，它返回一个指向实体拥有的组件的指针(如果有的话)，否则返回一个空指针。


		// 

		// registry.group<Comp_0>()
	}
}