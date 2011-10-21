/* 
* File:   TreeBuilders.hpp
* Author: Francois Ancel (francoisancel [at] gmail.com)
* Copyright : BSD license
* Created on September 16, 2011
*/

#ifndef TREEBUILDER_HPP
#define TREEBUILDER_HPP

#include <functional>

#include "TypedTree.hpp"
#include "ConditionnalFunctor.hpp"


namespace ID3 {

/*
Hold Class that construct Tree
*/
	namespace TreeBuilder {

		// BuilderNode's abstract interface
		template <typename Tree >
		class ABuilderNode {
		public:
			~ABuilderNode() {}
			// return inner gain value
			virtual const float& getResult() const = 0;
			// return inner size of dataset used
			virtual float getSize() const = 0;
			// return the main value of result on dataset
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

			Builder(Result defaultValue) : higher_(-1), construct_(), defaultValue_(defaultValue), node_(0) {}

			void operator() (const ABuilderNode< Tree >& node, const std::function< ATree*() >& constructor) {
				if (higher_ < node.getResult()) {
					node_ = &node;
					higher_ = node.getResult();
					construct_ = constructor;
				}
			}

			ATree* operator()() {
				if (!node_->getSize())
					return new Tree::Leaf(defaultValue_);
				if (!higher_ || higher_ == 1.0f) {
					return new Tree::Leaf(node_->getMainValue());
				}
				return construct_();
			}

			//static function to start build the decisional tree.
			template <typename ResultContainer, typename ContW, typename ContX, typename ContC, typename ContV, typename ContB, typename ContN>
			static ATree* construct(const ResultContainer& result, const ContW& val1, const ContX& val2, const ContC& val3, const ContV& val4, const ContB& val5, const ContN& val6) {
				Builder<Tree> build(getRandomResult(result));

				BuilderNode<Tree, ContW, ResultContainer> build_w(val1, result);
				build(build_w, 
					std::bind(&BuilderNode<Tree, ContW, ResultContainer>::constructin<ContX, ContC, ContV, ContB, ContN>, &build_w,  std::ref(val2), std::ref(val3), std::ref(val4), std::ref(val5), std::ref(val6), std::ref(val1)));

				BuilderNode<Tree, ContX, ResultContainer> build_x(val2, result);
				build(build_x, 
					std::bind(&BuilderNode<Tree, ContX, ResultContainer>::constructin<ContW, ContC, ContV, ContB, ContN>, &build_x, std::ref(val1), std::ref(val3), std::ref(val4), std::ref(val5), std::ref(val6), std::ref(val2)));

				BuilderNode<Tree, ContC, ResultContainer> build_c(val3, result);
				build(build_c, 
					std::bind(&BuilderNode<Tree, ContC, ResultContainer>::constructin<ContW, ContX, ContV, ContB, ContN>, &build_c, std::ref(val1), std::ref(val2), std::ref(val4), std::ref(val5), std::ref(val6), std::ref(val3)));

				BuilderNode<Tree, ContV, ResultContainer> build_v(val4, result);
				build(build_v, 
					std::bind(&BuilderNode<Tree, ContV, ResultContainer>::constructin<ContW, ContX, ContC, ContB, ContN>, &build_v, std::ref(val1), std::ref(val2), std::ref(val3), std::ref(val5), std::ref(val6), std::ref(val4)));
			
				BuilderNode<Tree, ContB, ResultContainer> build_b(val5, result);
				build(build_b, 
					std::bind(&BuilderNode<Tree, ContB, ResultContainer>::constructin<ContW, ContX, ContC, ContV, ContN>, &build_v, std::ref(val1), std::ref(val2), std::ref(val3), std::ref(val4), std::ref(val6), std::ref(val5)));

				BuilderNode<Tree, ContN, ResultContainer> build_n(val5, result);
				build(build_n, 
					std::bind(&BuilderNode<Tree, ContN, ResultContainer>::constructin<ContW, ContX, ContC, ContV, ContB>, &build_v, std::ref(val1), std::ref(val2), std::ref(val3), std::ref(val4), std::ref(val5), std::ref(val6)));

				return build();
			}

			template <typename ResultContainer, typename ContW, typename ContX, typename ContC, typename ContV, typename ContB>
			static ATree* construct(const ResultContainer& result, const ContW& val1, const ContX& val2, const ContC& val3, const ContV& val4, const ContB& val5) {
				Builder<Tree> build(getRandomResult(result));

				BuilderNode<Tree, ContW, ResultContainer> build_w(val1, result);
				build(build_w, std::bind(&BuilderNode<Tree, ContW, ResultContainer>::constructin<ContX, ContC, ContV, ContB>, &build_w,  std::ref(val2), std::ref(val3), std::ref(val4), std::ref(val5), std::ref(val1)));

				BuilderNode<Tree, ContX, ResultContainer> build_x(val2, result);
				build(build_x, std::bind(&BuilderNode<Tree, ContX, ResultContainer>::constructin<ContW, ContC, ContV, ContB>, &build_x, std::ref(val1), std::ref(val3), std::ref(val4), std::ref(val5), std::ref(val2)));

				BuilderNode<Tree, ContC, ResultContainer> build_c(val3, result);
				build(build_c, std::bind(&BuilderNode<Tree, ContC, ResultContainer>::constructin<ContW, ContX, ContV, ContB>, &build_c, std::ref(val1), std::ref(val2), std::ref(val4), std::ref(val5), std::ref(val3)));

				BuilderNode<Tree, ContV, ResultContainer> build_v(val4, result);
				build(build_v, std::bind(&BuilderNode<Tree, ContV, ResultContainer>::constructin<ContW, ContX, ContC, ContB>, &build_v, std::ref(val1), std::ref(val2), std::ref(val3), std::ref(val5), std::ref(val4)));
			
				BuilderNode<Tree, ContB, ResultContainer> build_b(val5, result);
				build(build_b, std::bind(&BuilderNode<Tree, ContB, ResultContainer>::constructin<ContW, ContX, ContC, ContV>, &build_v, std::ref(val1), std::ref(val2), std::ref(val3), std::ref(val4), std::ref(val5)));

				return build();
			}

			template <typename ResultContainer, typename ContW, typename ContX, typename ContC, typename ContV>
			static ATree* construct(const ResultContainer& result, const ContW& val1, const ContX& val2, const ContC& val3, const ContV& val4 ) {
				Builder<Tree> build(getRandomResult(result));

				BuilderNode<Tree, ContW, ResultContainer> build_w(val1, result);
				build(build_w, std::bind(&BuilderNode<Tree, ContW, ResultContainer>::constructin<ContX, ContC, ContV>, &build_w,  std::ref(val2), std::ref(val3), std::ref(val4), std::ref(val1)));

				BuilderNode<Tree, ContX, ResultContainer> build_x(val2, result);
				build(build_x, std::bind(&BuilderNode<Tree, ContX, ResultContainer>::constructin<ContW, ContC, ContV>, &build_x, std::ref(val1), std::ref(val3), std::ref(val4), std::ref(val2)));

				BuilderNode<Tree, ContC, ResultContainer> build_c(val3, result);
				build(build_c, std::bind(&BuilderNode<Tree, ContC, ResultContainer>::constructin<ContW, ContX, ContV>, &build_c, std::ref(val1), std::ref(val2), std::ref(val4), std::ref(val3)));

				BuilderNode<Tree, ContV, ResultContainer> build_v(val4, result);
				build(build_v, std::bind(&BuilderNode<Tree, ContV, ResultContainer>::constructin<ContW, ContX, ContC>, &build_v, std::ref(val1), std::ref(val2), std::ref(val3), std::ref(val4)));

				return build();
			}

			template <typename ResultContainer, typename ContW, typename ContX, typename ContC>
			static ATree* construct(const ResultContainer& result, const ContW& val1, const ContX& val2, const ContC& val3) {
				Builder<Tree> build(getRandomResult(result));

				BuilderNode<Tree, ContW, ResultContainer> build_w(val1, result);
				build(build_w, std::bind(&BuilderNode<Tree, ContW, ResultContainer>::constructin<ContX, ContC>, &build_w,  std::ref(val2), std::ref(val3), std::ref(val1)));

				BuilderNode<Tree, ContX, ResultContainer> build_x(val2, result);
				build(build_x, std::bind(&BuilderNode<Tree, ContX, ResultContainer>::constructin<ContW, ContC>, &build_x, std::ref(val1), std::ref(val3), std::ref(val2)));

				BuilderNode<Tree, ContC, ResultContainer> build_c(val3, result);
				build(build_c, std::bind(&BuilderNode<Tree, ContC, ResultContainer>::constructin<ContW, ContX>, &build_c, std::ref(val1), std::ref(val2), std::ref(val3)));

				return build();
			}

			template <typename ResultContainer, typename ContW, typename ContX>
			static ATree* construct(const ResultContainer& result, const ContW& val1, const ContX& val2) {
				Builder<Tree> build(getRandomResult(result));

				BuilderNode<Tree, ContW, ResultContainer> build_w(val1, result);
				build(build_w, std::bind(&BuilderNode<Tree, ContW, ResultContainer>::constructin<ContX>, &build_w,  std::ref(val2), std::ref(val1)));

				BuilderNode<Tree, ContX, ResultContainer> build_x(val2, result);
				build(build_x, std::bind(&BuilderNode<Tree, ContX, ResultContainer>::constructin<ContW>, &build_x, std::ref(val1), std::ref(val2)));

				return build();
			}

			template <typename ResultContainer, typename ContW>
			static ATree* construct(const ResultContainer& result, const ContW& val1) {
				Builder<Tree> build(getRandomResult(result));

				BuilderNode<Tree, ContW, ResultContainer> build_w(val1, result);
				build(build_w, std::bind(&BuilderNode<Tree, ContW, ResultContainer>::constructin, &build_w, std::ref(val1)));

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
			std::function< ATree*() > construct_;
			Result defaultValue_;
			const ABuilderNode< Tree >* node_;
		};

		/* 
		BuilderNode : Class design to construct each node
		Tree : Templated Class that contain each Tree Class
		SpecializedContainer : Type of the container of evaluate node
		ResultContainer : Type of the container of Result
		*/
		template< typename Tree, typename SpecializedContainer, typename ResultContainer > 
		class BuilderNode : public ABuilderNode< Tree > { 
		private:

			typedef typename SpecializedContainer::value_type					Specialized;
			typedef typename ResultContainer::value_type						Result;
			typedef typename Tree::ATree										ATree;
			typedef typename Tree::template TreeNode< SpecializedContainer::value_type > TreeNode;
			typedef typename ConditionnalFunctor1< SpecializedContainer >		ConditionnalFunctor1;
			typedef typename Gain< Specialized, Result >::AttribueMap			AttribueMap;

			const SpecializedContainer& attr_;
			const ResultContainer& res_;
			ConditionnalFunctor* func_;
			Gain< Specialized, Result > gain_;

		public:
			template <typename Conditional>
			BuilderNode(const SpecializedContainer& attr, const ResultContainer& res, Conditional& func)
				: attr_(attr), res_(res), func_(&func), gain_(attr, res, func)
			{}

			BuilderNode(const SpecializedContainer& attr, const ResultContainer& res)
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
			ATree* constructin(const ContX& val1, const ContY& val2, const ContZ& val3, const ContW& val4, const SpecializedContainer& spe) {
				TreeNode* root = new TreeNode();

				typename const AttribueMap& map = gain_.getAttribueMap();
				typename AttribueMap::const_iterator it = map.begin();
				typename AttribueMap::const_iterator ite = map.end();

				while (it != ite) {
					Builder< Tree >	 build(getMainValue());

					ConditionnalFunctor1 func(spe, it->first, func_);

					func.reset();
					BuilderNode<Tree, ContX, ResultContainer> build_1(val1, res_, func);
					build(build_1, 
						std::bind(&BuilderNode<Tree, ContX, ResultContainer>::constructin<ContX, ContY, ContZ, ContW>, &build_1, std::ref(val2), std::ref(val3), std::ref(val4), std::ref(val1)));

					func.reset();
					BuilderNode<Tree, ContY, ResultContainer> build_2(val2, res_, func);
					build(build_2, 
						std::bind(&BuilderNode<Tree, ContY, ResultContainer>::constructin<ContX, ContY, ContZ, ContW>, &build_2, std::ref(val1), std::ref(val3), std::ref(val4), std::ref(val2)));

					func.reset();
					BuilderNode<Tree, ContZ, ResultContainer> build_3(val3, res_, func);
					build(build_3, 
						std::bind(&BuilderNode<Tree, ContZ, ResultContainer>::constructin<ContX, ContY, ContZ, ContW>, &build_3, std::ref(val1), std::ref(val2), std::ref(val4), std::ref(val3)));

					func.reset();
					BuilderNode<Tree, ContW, ResultContainer> build_4(val4, res_, func);
					build(build_4, 
						std::bind(&BuilderNode<Tree, ContW, ResultContainer>::constructin<ContX, ContY, ContZ, ContW>, &build_4, std::ref(val1), std::ref(val2), std::ref(val3), std::ref(val4)));

					root->AddSubNode(it->first, build());
					++it;
				}

				return root;
			}

			template <typename ContX, typename ContY, typename ContZ>
			ATree* constructin(const ContX& val1, const ContY& val2, const ContZ& val3, const SpecializedContainer& spe) {
				TreeNode* root = new TreeNode();

				typename const AttribueMap& map = gain_.getAttribueMap();
				typename AttribueMap::const_iterator it = map.begin();
				typename AttribueMap::const_iterator ite = map.end();

				while (it != ite) {
					Builder< Tree >	 build(getMainValue());
					ConditionnalFunctor1 func(spe, it->first, func_);

					func.reset();
					BuilderNode<Tree, ContX, ResultContainer> build_f(val1, res_, func);
					build(build_f, std::bind(&BuilderNode<Tree, ContX, ResultContainer>::constructin<ContY, ContZ>, &build_f, std::ref(val2), std::ref(val3), std::ref(val1)));

					func.reset();
					BuilderNode<Tree, ContY, ResultContainer> build_s(val2, res_, func);
					build(build_s, std::bind(&BuilderNode<Tree, ContY, ResultContainer>::constructin<ContX, ContZ>, &build_s, std::ref(val1), std::ref(val3), std::ref(val2)));

					func.reset();
					BuilderNode<Tree, ContZ, ResultContainer> build_t(val3, res_, func);
					build(build_t, std::bind(&BuilderNode<Tree, ContZ, ResultContainer>::constructin<ContX, ContY>, &build_t, std::ref(val1), std::ref(val2), std::ref(val3)));

					root->AddSubNode(it->first, build());
					++it;
				}

				return root;
			}

			template <typename ContX, typename ContY>
			ATree* constructin(const ContX& val1, const ContY& val2, const SpecializedContainer& spe) {
				TreeNode* root = new TreeNode();

				typename const AttribueMap& map = gain_.getAttribueMap();
				typename AttribueMap::const_iterator it = map.begin();
				typename AttribueMap::const_iterator ite = map.end();

				//Evaluate every value of the current Node Attribute.
				while (it != ite) {
					Builder< Tree >	 build(getMainValue());

					ConditionnalFunctor1 func(spe, it->first, func_);

					func.reset();
					BuilderNode<Tree, ContX, ResultContainer> build_f(val1, res_, func);
					build(build_f, std::bind(&BuilderNode<Tree, ContX, ResultContainer>::constructin<ContY>, &build_f, std::ref(val2), std::ref(val1)));

					func.reset();
					BuilderNode<Tree, ContY, ResultContainer> build_s(val2, res_, func);
					build(build_s, std::bind(&BuilderNode<Tree, ContY, ResultContainer>::constructin<ContX>, &build_s, std::ref(val1), std::ref(val2)));

					root->AddSubNode(it->first, build());
					++it;
				}

				return root;
			}

			template <typename ContX>
			ATree* constructin(const ContX& val1, const SpecializedContainer& spe) {
				TreeNode* root = new TreeNode();

				typename const AttribueMap& map = gain_.getAttribueMap();
				typename AttribueMap::const_iterator it = map.begin();
				typename AttribueMap::const_iterator ite = map.end();

				while (it != ite) {
					Builder< Tree > build(getMainValue());

					ConditionnalFunctor1 func(spe, it->first, func_);

					func.reset();
					BuilderNode<Tree, ContX, ResultContainer> build_f(val1, res_, func);
					build(build_f, std::bind(&BuilderNode<Tree, ContX, ResultContainer>::constructin, &build_f, std::ref(val1)));

					root->AddSubNode(it->first, build());

					++it;
				}

				return root;
			}

			ATree* constructin(const SpecializedContainer& spe) {
				TreeNode* root = new TreeNode();

				typename const AttribueMap& map = gain_.getAttribueMap();
				typename AttribueMap::const_iterator it = map.begin();
				typename AttribueMap::const_iterator ite = map.end();

				while (it != ite) {
					ConditionnalFunctor1 func(spe, it->first, func_);

					func.reset();
					Gain< Specialized, Result > gain;
					gain.getGain(spe, res_, func);

					ATree* leaf;
					if (!gain.getSize()) {
						leaf = new Tree::Leaf(getMainValue());
					} else {
						leaf = new Tree::Leaf(gain.getMainResult());
					}

					root->AddSubNode(it->first, leaf);
					++it;
				}
				return root;
			}
		};

	}

}

#endif /* !TREEBUILDER_HPP */