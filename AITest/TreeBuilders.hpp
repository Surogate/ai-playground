/*
	Authors : Francois Ancel (francoisancel [at] gmail.com)
*/


#ifndef TREEBUILDER_HPP
#define TREEBUILDER_HPP

#include <functional>

#include "TypedTree.hpp"

namespace TreeBuilder {

	// Interface of BuilderNode
	template <typename Tree >
	class ABuilderNode {
	public:
		~ABuilderNode() {}
		virtual const float& getResult() const = 0;
		virtual float getSize() const = 0;
		virtual typename Tree::Result getMainValue() const = 0;
	};

	/*
		Builder :
		Can start tree creation with static ATree* construct(...)
		Compare each BuilderNode, chose to stop recurcing or not.
	*/
	template <typename Tree> 
	class Builder { 	
	public:
		typedef typename Tree::Result Result;
		typedef typename Tree::ATree ATree;

		Builder(Result defaultValue) : higher_(), construct_(), defaultValue_(defaultValue) {}

		void operator() (const ABuilderNode< Tree >& node, const std::function< ATree*() >& constructor) {
			size_ = node.getSize();
			if (size_) {
				defaultValue_ = node.getMainValue();
			}

			const float& gain = node.getResult();
			if (higher_ <= gain) {
				higher_ = gain;
				construct_ = constructor;
			}
		}

		ATree* operator()() {
			if (!size_ || !higher_ || higher_ == 1.0f) {
				return new Tree::Leaf(defaultValue_);
			}
			return construct_();
		}

		//fonction static pour lancer la construction de l'arbre
		template <typename ResultContainer, typename ContW, typename ContX, typename ContC, typename ContV>
		static ATree* construct(const ResultContainer& result, const ContW& val1, const ContX& val2, const ContC& val3, const ContV& val4 ) {
			Builder<Tree> build(getRandomResult(result));

			BuilderNode<Tree, ContW::value_type> build_w(val1, result);
			build(build_w, std::bind(&BuilderNode<Tree, ContW::value_type>::constructin<ContX, ContC, ContV>, &build_w,  std::ref(val2), std::ref(val3), std::ref(val4), std::ref(val1)));

			BuilderNode<Tree, ContX::value_type> build_x(val2, result);
			build(build_x, std::bind(&BuilderNode<Tree, ContX::value_type>::constructin<ContW, ContC, ContV>, &build_x, std::ref(val1), std::ref(val3), std::ref(val4), std::ref(val2)));

			BuilderNode<Tree, ContC::value_type> build_c(val3, result);
			build(build_c, std::bind(&BuilderNode<Tree, ContC::value_type>::constructin<ContW, ContX, ContV>, &build_c, std::ref(val1), std::ref(val2), std::ref(val4), std::ref(val3)));

			BuilderNode<Tree, ContV::value_type> build_v(val4, result);
			build(build_v, std::bind(&BuilderNode<Tree, ContV::value_type>::constructin<ContW, ContX, ContC>, &build_v, std::ref(val1), std::ref(val2), std::ref(val3), std::ref(val4)));

			return build();
		}

		template <typename ResultContainer>
		static Result getRandomResult(const ResultContainer& container) {
			std::uniform_int_distribution<int> distribution(0, container.size());
			int value = distribution(std::mt19937());

			typename ResultContainer::const_iterator it = container.begin();
			typename ResultContainer::const_iterator ite = container.end();

			int i = 0;
			while (it != ite && i < value)
			{
				++it;
				++i;
			}

			return *it;
		}

	private:
		float higher_;
		float size_;
		std::function< ATree*() > construct_;
		Result defaultValue_;
	};

	// BuilderNode : Structure qui construit l'arbre decisionnel
	// Tree : namespace template de l'arbre voulue
	// Specialized : Type du noeud actuellement evaluer
	template< typename Tree, typename Specialized > 
	class BuilderNode : public ABuilderNode< Tree > { 
	private:
		typedef typename Tree::Result Result;
		typedef typename Tree::ATree ATree;
		typedef typename Tree::TreeNode<Specialized> TreeNode;
		typedef typename Builder<Tree> Builder;
		typedef typename ConditionnalFunctor1< std::deque<Specialized> > ConditionnalFunctor1;
		typedef typename ConditionnalFunctorVal< std::deque<Specialized> > ConditionnalFunctorVal;

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

		float getSize() const {
			return gain_.getSize();
		}

		Result getMainValue() const {
			return gain_.getMainResult();
		}

		template <typename ContX, typename ContY, typename ContZ, typename ContW>
		ATree* constructin(const ContX& val1, const ContY& val2, const ContZ& val3, const ContW& val4, const std::deque<Specialized>& spe) {
			TreeNode* root = new TreeNode();

			typename const AttribueMap& map = gain_.getAttribueMap();
			typename AttribueMap::const_iterator it = map.begin();
			typename AttribueMap::const_iterator ite = map.end();

			while (it != ite) {
				Builder build(getMainValue());

				if (!func_) {
					ConditionnalFunctor1 func(spe, it->first);

					func.reset();
					BuilderNode<Tree, ContX::value_type> build_1(val1, res_, func);
					build(build_1, 
						std::bind(&BuilderNode<Tree, ContX::value_type>::constructin<ContX, ContY, ContZ, ContW>, &build_1, std::ref(val2), std::ref(val3), std::ref(val4), std::ref(val1)));

					func.reset();
					BuilderNode<Tree, ContY::value_type> build_2(val2, res_, func);
					build(build_2, 
						std::bind(&BuilderNode<Tree, ContY::value_type>::constructin<ContX, ContY, ContZ, ContW>, &build_2, std::ref(val1), std::ref(val3), std::ref(val4), std::ref(val2)));

					func.reset();
					BuilderNode<Tree, ContZ::value_type> build_3(val3, res_, func);
					build(build_3, 
						std::bind(&BuilderNode<Tree, ContZ::value_type>::constructin<ContX, ContY, ContZ, ContW>, &build_3, std::ref(val1), std::ref(val2), std::ref(val4), std::ref(val3)));

					func.reset();
					BuilderNode<Tree, ContW::value_type> build_4(val4, res_, func);
					build(build_4, 
						std::bind(&BuilderNode<Tree, ContW::value_type>::constructin<ContX, ContY, ContZ, ContW>, &build_4, std::ref(val1), std::ref(val2), std::ref(val3), std::ref(val4)));

					root->AddSubNode(it->first, build());
				} else {
					ConditionnalFunctorVal func(spe, it->first, *func_);

					func.reset();
					BuilderNode<Tree, ContX::value_type> build_1(val1, res_, func);
					build(build_1, 
						std::bind(&BuilderNode<Tree, ContX::value_type>::constructin<ContX, ContY, ContZ, ContW>, &build_1, std::ref(val2), std::ref(val3), std::ref(val4), std::ref(val1)));

					func.reset();
					BuilderNode<Tree, ContY::value_type> build_2(val2, res_, func);
					build(build_2, 
						std::bind(&BuilderNode<Tree, ContY::value_type>::constructin<ContX, ContY, ContZ, ContW>, &build_2, std::ref(val1), std::ref(val3), std::ref(val4), std::ref(val2)));

					func.reset();
					BuilderNode<Tree, ContZ::value_type> build_3(val3, res_, func);
					build(build_3, 
						std::bind(&BuilderNode<Tree, ContZ::value_type>::constructin<ContX, ContY, ContZ, ContW>, &build_3, std::ref(val1), std::ref(val2), std::ref(val4), std::ref(val3)));

					func.reset();
					BuilderNode<Tree, ContW::value_type> build_4(val4, res_, func);
					build(build_4, 
						std::bind(&BuilderNode<Tree, ContW::value_type>::constructin<ContX, ContY, ContZ, ContW>, &build_4, std::ref(val1), std::ref(val2), std::ref(val3), std::ref(val4)));

					root->AddSubNode(it->first, build());
				}
				++it;
			}

			return root;
		}

		template <typename ContX, typename ContY, typename ContZ>
		ATree* constructin(const ContX& val1, const ContY& val2, const ContZ& val3, const std::deque<Specialized>& spe) {
			TreeNode* root = new TreeNode();

			typename const AttribueMap& map = gain_.getAttribueMap();
			typename AttribueMap::const_iterator it = map.begin();
			typename AttribueMap::const_iterator ite = map.end();

			while (it != ite) {
				Builder build(getMainValue());

				if (!func_) {
					ConditionnalFunctor1 func(spe, it->first);

					func.reset();
					BuilderNode<Tree, ContX::value_type> build_f(val1, res_, func);
					build(build_f, 
						std::bind(&BuilderNode<Tree, ContX::value_type>::constructin<ContY, ContZ>, &build_f, std::ref(val2), std::ref(val3), std::ref(val1)));

					func.reset();
					BuilderNode<Tree, ContY::value_type> build_s(val2, res_, func);
					build(build_s, std::bind(&BuilderNode<Tree, ContY::value_type>::constructin<ContX, ContZ>, &build_s, std::ref(val1), std::ref(val3), std::ref(val2)));

					func.reset();
					BuilderNode<Tree, ContZ::value_type> build_t(val3, res_, func);
					build(build_t, std::bind(&BuilderNode<Tree, ContZ::value_type>::constructin<ContX, ContY>, &build_t, std::ref(val1), std::ref(val2), std::ref(val3)));

					root->AddSubNode(it->first, build());
				} else {
					ConditionnalFunctorVal func(spe, it->first, *func_);

					func.reset();
					BuilderNode<Tree, ContX::value_type> build_f(val1, res_, func);
					build(build_f, std::bind(&BuilderNode<Tree, ContX::value_type>::constructin<ContY, ContZ>, &build_f, std::ref(val2), std::ref(val3), std::ref(val1)));

					func.reset();
					BuilderNode<Tree, ContY::value_type> build_s(val2, res_, func);
					build(build_s, std::bind(&BuilderNode<Tree, ContY::value_type>::constructin<ContX, ContZ>, &build_s, std::ref(val1), std::ref(val3), std::ref(val2)));

					func.reset();
					BuilderNode<Tree, ContZ::value_type> build_t(val3, res_, func);
					build(build_t, std::bind(&BuilderNode<Tree, ContZ::value_type>::constructin<ContX, ContY>, &build_t, std::ref(val1), std::ref(val2), std::ref(val3)));

					root->AddSubNode(it->first, build());
				}
				++it;
			}

			return root;
		}

		template <typename ContX, typename ContY>
		ATree* constructin(const ContX& val1, const ContY& val2, const std::deque<Specialized>& spe) {
			TreeNode* root = new TreeNode();

			typename const AttribueMap& map = gain_.getAttribueMap();
			typename AttribueMap::const_iterator it = map.begin();
			typename AttribueMap::const_iterator ite = map.end();

			//Evaluate every value of the current Node Attribute.
			while (it != ite) {
				Builder build(getMainValue());
				
				if (!func_) {
					ConditionnalFunctor1 func(spe, it->first);


					func.reset();
					BuilderNode<Tree, ContX::value_type> build_f(val1, res_, func);
					build(build_f, std::bind(&BuilderNode<Tree, ContX::value_type>::constructin<ContY>, &build_f, std::ref(val2), std::ref(val1)));

					func.reset();
					BuilderNode<Tree, ContY::value_type> build_s(val2, res_, func);
					build(build_s, std::bind(&BuilderNode<Tree, ContY::value_type>::constructin<ContX>, &build_s, std::ref(val1), std::ref(val2)));

					root->AddSubNode(it->first, build());
				} else {
					ConditionnalFunctorVal func(spe, it->first, *func_);

					func.reset();
					BuilderNode<Tree, ContX::value_type> build_f(val1, res_, func);
					build(build_f, std::bind(&BuilderNode<Tree, ContX::value_type>::constructin<ContY>, &build_f, std::ref(val2), std::ref(val1)));

					func.reset();
					BuilderNode<Tree, ContY::value_type> build_s(val2, res_, func);
					build(build_s, std::bind(&BuilderNode<Tree, ContY::value_type>::constructin<ContX>, &build_s, std::ref(val1), std::ref(val2)));

					root->AddSubNode(it->first, build());
				}
				++it;
			}

			return root;
		}

		template <typename ContX>
		ATree* constructin(const ContX& val1, const std::deque<Specialized>& spe) {
			TreeNode* root = new TreeNode();

			typename const AttribueMap& map = gain_.getAttribueMap();
			typename AttribueMap::const_iterator it = map.begin();
			typename AttribueMap::const_iterator ite = map.end();

			while (it != ite) {
				Builder build(getMainValue());

				if (!func_) {
					ConditionnalFunctor1 func(spe, it->first);

					func.reset();
					BuilderNode<Tree, ContX::value_type> build_f(val1, res_, func);
					build(build_f, std::bind(&BuilderNode<Tree, ContX::value_type>::constructin<ContX>, &build_f, std::ref(val1)));

					root->AddSubNode(it->first, build());
				} else {
					ConditionnalFunctorVal func(spe, it->first, *func_);

					func.reset();
					BuilderNode<Tree, ContX::value_type> build_f(val1, res_, func);
					build(build_f, std::bind(&BuilderNode<Tree, ContX::value_type>::constructin<ContX>, &build_f, std::ref(val1)));

					root->AddSubNode(it->first, build());
				}

				++it;
			}

			return root;
		}

		template <typename ContX>
		ATree* constructin(const ContX& spe) {
			TreeNode* root = new TreeNode();

			typename const AttribueMap& map = gain_.getAttribueMap();
			typename AttribueMap::const_iterator it = map.begin();
			typename AttribueMap::const_iterator ite = map.end();

			while (it != ite) {

				if (!func_) {
					ConditionnalFunctor1 func(spe, it->first);

					func.reset();
					Gain<Specialized, Result> gain;
					gain.getGain(spe, res_, func);

					ATree* leaf;
					if (!gain.getSize()) {
						std::cout << "@=@ random on leaf" << std::endl;
						leaf = new Tree::Leaf(gain_.getRandomResult(res_));
					} else {
						std::cout << "@@@ Main result on leaf" << std::endl;
						leaf = new Tree::Leaf(gain_.getMainResult());
					}

					root->AddSubNode(it->first, leaf);
				} else {
					ConditionnalFunctorVal func(spe, it->first, *func_);

					func.reset();
					Gain<Specialized, Result> gain;
					gain.getGain(spe, res_, func);

					ATree* leaf;
					if (!gain.getSize()) {
						std::cout << "@=@ random on leaf" << std::endl;
						leaf = new Tree::Leaf(gain_.getRandomResult(res_));
					} else {
						std::cout << "@@@ Main result on leaf" << std::endl;
						leaf = new Tree::Leaf(gain_.getMainResult());
					}

					root->AddSubNode(it->first, leaf);
				}
				++it;
			}
			return root;
		}
	};

}

#endif /* !TREEBUILDER_HPP */