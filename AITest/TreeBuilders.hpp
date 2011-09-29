
#ifndef TREEBUILDER_HPP
#define TREEBUILDER_HPP

#include <functional>

#include "TypedTree.hpp"

namespace TreeBuilder {

	template <typename Tree> 
	class Builder {
	public:
		typedef typename Tree::ATree ATree;

		Builder() : higher_(), construct_() {}

		void operator() (const float& gain, const std::function< ATree*() >& constructor) {
			if (higher_ <= gain) {
				construct_ = constructor;
			}
		}

		ATree* operator()() {
			return construct_();
		}

		template <typename ResultContainer, typename ContW, typename ContX, typename ContC, typename ContV>
		static ATree* construct(const ResultContainer& result, const ContW& val1, const ContX& val2, const ContC& val3, const ContV& val4 ) {
			Builder<Tree> build;

			BuilderNode<Tree, ContW::value_type> build_w(val1, result);
			build(build_w.getResult(), std::bind(&BuilderNode<Tree, ContW::value_type>::construct<ContW, ContX, ContC, ContV>, &build_w,  std::ref(val1), std::ref(val2), std::ref(val3), std::ref(val4)));

			BuilderNode<Tree, ContX::value_type> build_x(val2, result);
			build(build_x.getResult(), std::bind(&BuilderNode<Tree, ContX::value_type>::construct<ContW, ContX, ContC, ContV>, &build_x, std::ref(val1), std::ref(val2), std::ref(val3), std::ref(val4)));

			BuilderNode<Tree, ContC::value_type> build_c(val3, result);
			build(build_c.getResult(), std::bind(&BuilderNode<Tree, ContC::value_type>::construct<ContW, ContX, ContC, ContV>, &build_c, std::ref(val1), std::ref(val2), std::ref(val3), std::ref(val4)));

			BuilderNode<Tree, ContV::value_type> build_v(val4, result);
			build(build_v.getResult(), std::bind(&BuilderNode<Tree, ContV::value_type>::construct<ContW, ContX, ContC, ContV>, &build_v, std::ref(val1), std::ref(val2), std::ref(val3), std::ref(val4)));

			return build();
		}

	private:
		float higher_;
		std::function< ATree*() > construct_;
	};

	template< typename Tree, typename Specialized >
	class BuilderNode {
	private:
		typedef typename Tree::Result Result;
		typedef typename Tree::ATree ATree;
		typedef typename Tree::TreeNode<Specialized> TreeNode;
		typedef typename Builder<Tree> Builder;
		typedef typename ConditionnalFunctor1< std::deque<Specialized>, Specialized > ConditionnalFunctor1;
		typedef typename ConditionnalFunctorVal< std::deque<Specialized>, Specialized > ConditionnalFunctorVal;

		typedef typename Gain< Specialized, Result >::AttribueMap AttribueMap;

		const std::deque<Specialized>& attr_;
		const std::deque<Result>& res_;
		ConditionnalFunctor* func_;
		Gain< Specialized, Result > gain_;

	public:
		template <typename Conditional>
		BuilderNode(const std::deque<Specialized>& attr, const std::deque<Result>& res, Conditional& func)
			: attr_(attr), res_(res), func_(&func), gain_(attr, res, func)
			{}

		BuilderNode(const std::deque<Specialized>& attr, const std::deque<Result>& res)
		: attr_(attr), res_(res), func_(0), gain_(attr, res)
		{}

		const float& getResult() const {
			return gain_.getResult();
		}

		template <typename ContW, typename ContX, typename ContC, typename ContV>
		ATree* construct(const ContW& val1, const ContX val2, const ContC val3, const ContV& val4)
		{
			return constructin(val1, val2, val3, val4);
		}

		template <typename ContW, typename ContX, typename ContC>
		ATree* construct(const ContW& val1, const ContX val2, const ContC val3)
		{
			return constructin(val1, val2, val3);
		}

		template <typename ContW, typename ContX>
		ATree* construct(const ContW& val1, const ContX val2)
		{
			return constructin(val1, val2);
		}

		template <typename ContW>
		ATree* construct(const ContW& val1) {
			return constructin(val1);
		}

		template <typename ContX, typename ContY, typename ContZ>
		ATree* constructin(const ContX& val1, const ContY& val2, const ContZ& val3, const std::deque<Specialized>& spe) {	
			if (gain_.getResult() == 1.f)
				return new Tree::Leaf(gain_.getMainResult());
			if (gain_.getSize() == 0)
				return new Tree::Leaf(gain_.getRandomResult(res_));

			TreeNode* root = new TreeNode();

			typename const AttribueMap& map = gain_.getAttribueMap();
			typename AttribueMap::const_iterator it = map.begin();
			typename AttribueMap::const_iterator ite = map.end();

			while (it != ite) {
				Builder build;
					
				if (!func_) {
					ConditionnalFunctor1 func(spe, it->first);

					BuilderNode<Tree, ContX::value_type> build_f(val1, res_, func);
					build(build_f.getResult(), std::bind(&BuilderNode<Tree, ContX::value_type>::construct<ContX, ContY, ContZ>, &build_f, std::ref(val1), std::ref(val2), std::ref(val3)));

					BuilderNode<Tree, ContY::value_type> build_s(val2, res_, func);
					build(build_s.getResult(), std::bind(&BuilderNode<Tree, ContY::value_type>::construct<ContX, ContY, ContZ>, &build_s, std::ref(val1), std::ref(val2), std::ref(val3)));

					BuilderNode<Tree, ContZ::value_type> build_t(val3, res_, func);
					build(build_t.getResult(), std::bind(&BuilderNode<Tree, ContZ::value_type>::construct<ContX, ContY, ContZ>, &build_t, std::ref(val1), std::ref(val2), std::ref(val3)));
				} else {
					ConditionnalFunctorVal func(spe, it->first, *func_);

					BuilderNode<Tree, ContX::value_type> build_f(val1, res_, func);
					build(build_f.getResult(), std::bind(&BuilderNode<Tree, ContX::value_type>::construct<ContX, ContY, ContZ>, &build_f, std::ref(val1), std::ref(val2), std::ref(val3)));

					BuilderNode<Tree, ContY::value_type> build_s(val2, res_, func);
					build(build_s.getResult(), std::bind(&BuilderNode<Tree, ContY::value_type>::construct<ContX, ContY, ContZ>, &build_s, std::ref(val1), std::ref(val2), std::ref(val3)));

					BuilderNode<Tree, ContZ::value_type> build_t(val3, res_, func);
					build(build_t.getResult(), std::bind(&BuilderNode<Tree, ContZ::value_type>::construct<ContX, ContY, ContZ>, &build_t, std::ref(val1), std::ref(val2), std::ref(val3)));
				}

				root->AddSubNode(it->first, build());
				++it;
			}
				
			return root;
		}

		template <typename ContX, typename ContY, typename ContZ>
		ATree* constructin(const ContX& val1, const ContY& val2, const std::deque<Specialized>& spe, const ContZ& val3) {
			return constructin(val1, val2, val3, spe);
		}

		template <typename ContX, typename ContY, typename ContZ>
		ATree* constructin(const ContX& val1, const std::deque<Specialized>& spe, const ContY& val2, const ContZ& val3) {
			return constructin(val1, val2, val3, spe);
		}

		template <typename ContX, typename ContY, typename ContZ>
		ATree* constructin(const std::deque<Specialized>& spe, const ContX& val1, const ContY& val2, const ContZ& val3) {
			return constructin(val1, val2, val3, spe);
		}

		template <typename ContX, typename ContY>
		ATree* constructin(const ContX& val1, const ContY& val2, const std::deque<Specialized>& spe) {
			if (gain_.getResult() == 1.f)
				return new Tree::Leaf(gain_.getMainResult());
			if (gain_.getSize() == 0)
				return new Tree::Leaf(gain_.getRandomResult(res_));

			TreeNode* root = new TreeNode();

			typename const AttribueMap& map = gain_.getAttribueMap();
			typename AttribueMap::const_iterator it = map.begin();
			typename AttribueMap::const_iterator ite = map.end();

			while (it != ite) {
				Builder build;
					
				if (!func_) {
					ConditionnalFunctor1 func(spe, it->first);

					BuilderNode<Tree, ContX::value_type> build_f(val1, res_, func);
					build(build_f.getResult(), std::bind(&BuilderNode<Tree, ContX::value_type>::construct<ContX, ContY>, &build_f, std::ref(val1), std::ref(val2)));

					BuilderNode<Tree, ContY::value_type> build_s(val2, res_, func);
					build(build_s.getResult(), std::bind(&BuilderNode<Tree, ContY::value_type>::construct<ContX, ContY>, &build_s, std::ref(val1), std::ref(val2)));
				} else {
					ConditionnalFunctorVal func(spe, it->first, *func_);

					BuilderNode<Tree, ContX::value_type> build_f(val1, res_, func);
					build(build_f.getResult(), std::bind(&BuilderNode<Tree, ContX::value_type>::construct<ContX, ContY>, &build_f, std::ref(val1), std::ref(val2)));

					BuilderNode<Tree, ContY::value_type> build_s(val2, res_, func);
					build(build_s.getResult(), std::bind(&BuilderNode<Tree, ContY::value_type>::construct<ContX, ContY>, &build_s, std::ref(val1), std::ref(val2)));
				}

				root->AddSubNode(it->first, build());
				++it;
			}
				
			return root;
		}

		template <typename ContX, typename ContY>
		ATree* constructin(const ContX& val1, const std::deque<Specialized>& spe, const ContY& val2) {
			return constructin(val1, val2, spe);
		}

		template <typename ContX, typename ContY>
		ATree* constructin(const std::deque<Specialized>& spe, const ContX& val1, const ContY& val2) {
			return constructin(val1, val2, spe);
		}

		template <typename ContX>
		ATree* constructin(const ContX& val1, const std::deque<Specialized>& spe) {
			if (gain_.getResult() == 1.f)
				return new Tree::Leaf(gain_.getMainResult());
			if (gain_.getSize() == 0)
				return new Tree::Leaf(gain_.getRandomResult(res_));

			TreeNode* root = new TreeNode();

			typename const AttribueMap& map = gain_.getAttribueMap();
			typename AttribueMap::const_iterator it = map.begin();
			typename AttribueMap::const_iterator ite = map.end();

			while (it != ite) {
				Builder build;
					
				if (!func_) {
					ConditionnalFunctor1 func(spe, it->first);

					BuilderNode<Tree, ContX::value_type> build_f(val1, res_, func);
					build(build_f.getResult(), std::bind(&BuilderNode<Tree, ContX::value_type>::construct<ContX>, &build_f, std::ref(val1)));
				} else {
					ConditionnalFunctorVal func(spe, it->first, *func_);

					BuilderNode<Tree, ContX::value_type> build_f(val1, res_, func);
					build(build_f.getResult(), std::bind(&BuilderNode<Tree, ContX::value_type>::construct<ContX>, &build_f, std::ref(val1)));
				}

				root->AddSubNode(it->first, build());
				++it;
			}

			return root;
		}

		template <typename ContX>
		ATree* constructin(const std::deque<Specialized>& spe, const ContX& val1) {
			return constructin(val1, spe);
		}

		template <typename ContX>
		ATree* constructin(const ContX& spe) {
			if (gain_.getResult() == 1.f)
				return new Tree::Leaf(gain_.getMainResult());
			return new Tree::Leaf(gain_.getRandomResult(res_));
		}
	};

}

#endif /* !TREEBUILDER_HPP */