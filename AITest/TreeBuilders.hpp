/* 
* File:   TreeBuilders.hpp
* Author: Francois Ancel (francoisancel [at] gmail.com)
* Copyright : BSD license
* Created on September 16, 2011
*/

#ifndef TREEBUILDER_HPP
#define TREEBUILDER_HPP

#include <boost/function.hpp>
#include <boost/bind.hpp>

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

			Builder(Result defaultValue) : higher_(-1), defaultValue_(defaultValue), construct_(), node_(0) {}

			void operator() (const ABuilderNode< Tree >& node, const boost::function< ATree*() >& constructor) {
				if (higher_ < node.getResult()) {
					node_ = &node;
					higher_ = node.getResult();
					construct_ = constructor;
				}
			}

			ATree* operator()() {
				if (!node_->getSize())
					return new typename Tree::Leaf(defaultValue_);
				if (!higher_ || higher_ == 1.0f) {
					return new typename Tree::Leaf(node_->getMainValue());
				}
				return construct_();
			}

		private:
			float higher_;
			Result defaultValue_;
			boost::function< ATree*() > construct_;
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
			typedef typename Tree::template TreeNode< Specialized > TreeNode;
			typedef ConditionnalFunctor1< SpecializedContainer >		ConditionnalFunctorSpe;
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

			BuilderNode(const BuilderNode& orig) 
				: attr_(orig.attr_), res_(orig.res_), func_(orig.func_), gain_(orig.gain_)
			{}

			BuilderNode& operator=(const BuilderNode& orig) {
				// not permited
			}

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

				const AttribueMap& map = gain_.getAttribueMap();
				typename AttribueMap::const_iterator it = map.begin();
				typename AttribueMap::const_iterator ite = map.end();

				while (it != ite) {
					Builder< Tree >	 build(getMainValue());

					ConditionnalFunctorSpe func(spe, it->first, func_);

					func.reset();
					BuilderNode<Tree, ContX, ResultContainer> build_1(val1, res_, func);
					build(build_1, 
						boost::bind(
						static_cast<ATree* (BuilderNode<Tree, ContX, ResultContainer>::*)(const ContY&, const ContZ&, const ContW&, const ContX&) >
						(&BuilderNode<Tree, ContX, ResultContainer>::template constructin<ContY, ContZ, ContW>)
						, &build_1, boost::ref(val2), boost::ref(val3), boost::ref(val4), boost::ref(val1)));

					func.reset();
					BuilderNode<Tree, ContY, ResultContainer> build_2(val2, res_, func);
					build(build_2, 
						boost::bind(
						static_cast<ATree* (BuilderNode<Tree, ContY, ResultContainer>::*)(const ContX&, const ContZ&, const ContW&, const ContY&) >
						(&BuilderNode<Tree, ContY, ResultContainer>::template constructin<ContX, ContZ, ContW>)
						, &build_2, boost::ref(val1), boost::ref(val3), boost::ref(val4), boost::ref(val2)));

					func.reset();
					BuilderNode<Tree, ContZ, ResultContainer> build_3(val3, res_, func);
					build(build_3, 
						boost::bind(
						static_cast<ATree* (BuilderNode<Tree, ContZ, ResultContainer>::*)(const ContX&, const ContY&, const ContW&, const ContZ&) >
						(&BuilderNode<Tree, ContZ, ResultContainer>::template constructin<ContX, ContY, ContW>)
						, &build_3, boost::ref(val1), boost::ref(val2), boost::ref(val4), boost::ref(val3)));

					func.reset();
					BuilderNode<Tree, ContW, ResultContainer> build_4(val4, res_, func);
					build(build_4, 
						boost::bind(
						static_cast<ATree* (BuilderNode<Tree, ContW, ResultContainer>::*)(const ContX&, const ContY&, const ContZ&, const ContW&) >
						(&BuilderNode<Tree, ContW, ResultContainer>::template constructin<ContX, ContY, ContZ>)
						, &build_4, boost::ref(val1), boost::ref(val2), boost::ref(val3), boost::ref(val4)));

					root->AddSubNode(it->first, build());
					++it;
				}

				return root;
			}

			template <typename ContX, typename ContY, typename ContZ>
			ATree* constructin(const ContX& val1, const ContY& val2, const ContZ& val3, const SpecializedContainer& spe) {
				TreeNode* root = new TreeNode();

				const AttribueMap& map = gain_.getAttribueMap();
				typename AttribueMap::const_iterator it = map.begin();
				typename AttribueMap::const_iterator ite = map.end();

				while (it != ite) {
					Builder< Tree >	 build(getMainValue());
					ConditionnalFunctorSpe func(spe, it->first, func_);

					func.reset();
					BuilderNode<Tree, ContX, ResultContainer> build_f(val1, res_, func);
					build(build_f, 
						boost::bind(
						static_cast<ATree* (BuilderNode<Tree, ContX, ResultContainer>::*)(const ContY&, const ContZ&, const ContX&) >
						(&BuilderNode<Tree, ContX, ResultContainer>::template constructin<ContY, ContZ>)
						, &build_f, boost::ref(val2), boost::ref(val3), boost::ref(val1)));

					func.reset();
					BuilderNode<Tree, ContY, ResultContainer> build_s(val2, res_, func);
					build(build_s, 
						boost::bind(
						static_cast<ATree* (BuilderNode<Tree, ContY, ResultContainer>::*)(const ContX&, const ContZ&, const ContY&) >
						(&BuilderNode<Tree, ContY, ResultContainer>::template constructin<ContX, ContZ>)
						, &build_s, boost::ref(val1), boost::ref(val3), boost::ref(val2)));

					func.reset();
					BuilderNode<Tree, ContZ, ResultContainer> build_t(val3, res_, func);
					build(build_t, 
						boost::bind(
						static_cast<ATree* (BuilderNode<Tree, ContZ, ResultContainer>::*)(const ContX&, const ContY&, const ContZ&) >
						(&BuilderNode<Tree, ContZ, ResultContainer>::template constructin<ContX, ContY>)
						, &build_t, boost::ref(val1), boost::ref(val2), boost::ref(val3)));

					root->AddSubNode(it->first, build());
					++it;
				}

				return root;
			}

			template <typename ContX, typename ContY>
			ATree* constructin(const ContX& val1, const ContY& val2, const SpecializedContainer& spe) {
				TreeNode* root = new TreeNode();

				const AttribueMap& map = gain_.getAttribueMap();
				typename AttribueMap::const_iterator it = map.begin();
				typename AttribueMap::const_iterator ite = map.end();

				//Evaluate every value of the current Node Attribute.
				while (it != ite) {
					Builder< Tree >	 build(getMainValue());

					ConditionnalFunctorSpe func(spe, it->first, func_);

					func.reset();
					BuilderNode<Tree, ContX, ResultContainer> build_f(val1, res_, func);
					build(build_f, 
						boost::bind(
						static_cast<ATree* (BuilderNode<Tree, ContX, ResultContainer>::*)(const ContY&, const ContX&) >
						(&BuilderNode<Tree, ContX, ResultContainer>::template constructin<ContY>)
						, &build_f, boost::ref(val2), boost::ref(val1)));

					func.reset();
					BuilderNode<Tree, ContY, ResultContainer> build_s(val2, res_, func);
					build(build_s, 
						boost::bind(
						static_cast<ATree* (BuilderNode<Tree, ContY, ResultContainer>::*)(const ContX&, const ContY&) >
						(&BuilderNode<Tree, ContY, ResultContainer>::template constructin<ContX>)
						, &build_s, boost::ref(val1), boost::ref(val2)));

					root->AddSubNode(it->first, build());
					++it;
				}

				return root;
			}

			template <typename ContX>
			ATree* constructin(const ContX& val1, const SpecializedContainer& spe) {
				TreeNode* root = new TreeNode();

				const AttribueMap& map = gain_.getAttribueMap();
				typename AttribueMap::const_iterator it = map.begin();
				typename AttribueMap::const_iterator ite = map.end();

				while (it != ite) {
					Builder< Tree > build(getMainValue());

					ConditionnalFunctorSpe func(spe, it->first, func_);

					func.reset();
					BuilderNode<Tree, ContX, ResultContainer> build_f(val1, res_, func);
					build(build_f, 
						boost::bind(
						static_cast<ATree* (BuilderNode<Tree, ContX, ResultContainer>::*)(const ContX&) >
						(&BuilderNode<Tree, ContX, ResultContainer>::constructin)
						, &build_f, boost::ref(val1)));

					root->AddSubNode(it->first, build());

					++it;
				}

				return root;
			}

			ATree* constructin(const SpecializedContainer& spe) {
				TreeNode* root = new TreeNode();

				const AttribueMap& map = gain_.getAttribueMap();
				typename AttribueMap::const_iterator it = map.begin();
				typename AttribueMap::const_iterator ite = map.end();

				while (it != ite) {
					ConditionnalFunctorSpe func(spe, it->first, func_);

					func.reset();
					Gain< Specialized, Result > gain;
					gain.getGain(spe, res_, func);

					ATree* leaf;
					if (!gain.getSize()) {
						leaf = new typename Tree::Leaf(getMainValue());
					} else {
						leaf = new typename Tree::Leaf(gain.getMainResult());
					}

					root->AddSubNode(it->first, leaf);
					++it;
				}
				return root;
			}
		};

		//static function to start build the decisional tree.
		template < typename Tree >
		struct Starter {

			typedef typename Tree::ATree ATree;
			
			template <typename ResultContainer, typename ContW, typename ContX, typename ContC, typename ContV, typename ContB, typename ContN>
			static ATree* construct(const ResultContainer& result, const ContW& val1, const ContX& val2, const ContC& val3, const ContV& val4, const ContB& val5, const ContN& val6) {
				Builder<Tree> build(getRandomResult(result));

				BuilderNode<Tree, ContW, ResultContainer> build_w(val1, result);
				build(build_w, 
					boost::bind(
					static_cast<ATree* (BuilderNode<Tree, ContW, ResultContainer>::*)(const ContX&, const ContC&, const ContV&, const ContB&, const ContN&, const ContW&) >
					(&BuilderNode<Tree, ContW, ResultContainer>::template constructin<ContX, ContC, ContV, ContB, ContN>)
					, &build_w,  boost::ref(val2), boost::ref(val3), boost::ref(val4), boost::ref(val5), boost::ref(val6), boost::ref(val1)));

				BuilderNode<Tree, ContX, ResultContainer> build_x(val2, result);
				build(build_x, 
					boost::bind(
					static_cast<ATree* (BuilderNode<Tree, ContX, ResultContainer>::*)(const ContW&, const ContC&, const ContV&, const ContB&, const ContN&, const ContX&) >
					(&BuilderNode<Tree, ContX, ResultContainer>::template constructin<ContW, ContC, ContV, ContB, ContN>)
					, &build_x, boost::ref(val1), boost::ref(val3), boost::ref(val4), boost::ref(val5), boost::ref(val6), boost::ref(val2)));

				BuilderNode<Tree, ContC, ResultContainer> build_c(val3, result);
				build(build_c, 
					boost::bind(
					static_cast<ATree* (BuilderNode<Tree, ContC, ResultContainer>::*)(const ContW&, const ContX&, const ContV&, const ContB&, const ContN&, const ContC&) >
					(&BuilderNode<Tree, ContC, ResultContainer>::template constructin<ContW, ContX, ContV, ContB, ContN>)
					, &build_c, boost::ref(val1), boost::ref(val2), boost::ref(val4), boost::ref(val5), boost::ref(val6), boost::ref(val3)));

				BuilderNode<Tree, ContV, ResultContainer> build_v(val4, result);
				build(build_v, 
					boost::bind(
					static_cast<ATree* (BuilderNode<Tree, ContV, ResultContainer>::*)(const ContW&, const ContX&, const ContC&, const ContB&, const ContN&, const ContV&) >
					(&BuilderNode<Tree, ContV, ResultContainer>::template constructin<ContW, ContX, ContC, ContB, ContN>)
					, &build_v, boost::ref(val1), boost::ref(val2), boost::ref(val3), boost::ref(val5), boost::ref(val6), boost::ref(val4)));
			
				BuilderNode<Tree, ContB, ResultContainer> build_b(val5, result);
				build(build_b, 
					boost::bind(
					static_cast<ATree* (BuilderNode<Tree, ContB, ResultContainer>::*)(const ContW&, const ContX&, const ContC&, const ContV&, const ContN&, const ContB&) >
					(&BuilderNode<Tree, ContB, ResultContainer>::template constructin<ContW, ContX, ContC, ContV, ContN>)
					, &build_v, boost::ref(val1), boost::ref(val2), boost::ref(val3), boost::ref(val4), boost::ref(val6), boost::ref(val5)));

				BuilderNode<Tree, ContN, ResultContainer> build_n(val5, result);
				build(build_n, 
					boost::bind(
					static_cast<ATree* (BuilderNode<Tree, ContN, ResultContainer>::*)(const ContW&, const ContX&, const ContC&, const ContV&, const ContB&, const ContN&) >
					(&BuilderNode<Tree, ContN, ResultContainer>::template constructin<ContW, ContX, ContC, ContV, ContB>)
					, &build_v, boost::ref(val1), boost::ref(val2), boost::ref(val3), boost::ref(val4), boost::ref(val5), boost::ref(val6)));

				return build();
			}

			template <typename ResultContainer, typename ContW, typename ContX, typename ContC, typename ContV, typename ContB>
			static ATree* construct(const ResultContainer& result, const ContW& val1, const ContX& val2, const ContC& val3, const ContV& val4, const ContB& val5) {
				Builder<Tree> build(getRandomResult(result));

				BuilderNode<Tree, ContW, ResultContainer> build_w(val1, result);
				build(build_w, 
					boost::bind(
					static_cast<ATree* (BuilderNode<Tree, ContW, ResultContainer>::*)(const ContX&, const ContC&, const ContV&, const ContB&, const ContW&) >
					(&BuilderNode<Tree, ContW, ResultContainer>::template constructin<ContX, ContC, ContV, ContB>)
					, &build_w,  boost::ref(val2), boost::ref(val3), boost::ref(val4), boost::ref(val5), boost::ref(val1)));

				BuilderNode<Tree, ContX, ResultContainer> build_x(val2, result);
				build(build_x, 
					boost::bind(
					static_cast<ATree* (BuilderNode<Tree, ContX, ResultContainer>::*)(const ContW&, const ContC&, const ContV&, const ContB&, const ContX&) >
					(&BuilderNode<Tree, ContX, ResultContainer>::template constructin<ContW, ContC, ContV, ContB>)
					, &build_x, boost::ref(val1), boost::ref(val3), boost::ref(val4), boost::ref(val5), boost::ref(val2)));

				BuilderNode<Tree, ContC, ResultContainer> build_c(val3, result);
				build(build_c, 
					boost::bind(
					static_cast<ATree* (BuilderNode<Tree, ContC, ResultContainer>::*)(const ContW&, const ContX&, const ContV&, const ContB&, const ContC&) >
					(&BuilderNode<Tree, ContC, ResultContainer>::template constructin<ContW, ContX, ContV, ContB>)
					, &build_c, boost::ref(val1), boost::ref(val2), boost::ref(val4), boost::ref(val5), boost::ref(val3)));

				BuilderNode<Tree, ContV, ResultContainer> build_v(val4, result);
				build(build_v, 
					boost::bind(
					static_cast<ATree* (BuilderNode<Tree, ContV, ResultContainer>::*)(const ContW&, const ContX&, const ContC&, const ContB&, const ContV&) >
					(&BuilderNode<Tree, ContV, ResultContainer>::template constructin<ContW, ContX, ContC, ContB>)
					, &build_v, boost::ref(val1), boost::ref(val2), boost::ref(val3), boost::ref(val5), boost::ref(val4)));
			
				BuilderNode<Tree, ContB, ResultContainer> build_b(val5, result);
				build(build_b, 
					boost::bind(
					static_cast<ATree* (BuilderNode<Tree, ContB, ResultContainer>::*)(const ContW&, const ContX&, const ContC&, const ContV&, const ContB&) >
					(&BuilderNode<Tree, ContB, ResultContainer>::template constructin<ContW, ContX, ContC, ContV>)
					, &build_v, boost::ref(val1), boost::ref(val2), boost::ref(val3), boost::ref(val4), boost::ref(val5)));

				return build();
			}

			template <typename ResultContainer, typename ContW, typename ContX, typename ContC, typename ContV>
			static ATree* construct(const ResultContainer& result, const ContW& val1, const ContX& val2, const ContC& val3, const ContV& val4 ) {
				Builder<Tree> build(getRandomResult(result));

				BuilderNode<Tree, ContW, ResultContainer> build_w(val1, result);
				build(build_w, 
					boost::bind(
					static_cast<ATree* (BuilderNode<Tree, ContW, ResultContainer>::*)(const ContX&, const ContC&, const ContV&, const ContW&) >
					(&BuilderNode<Tree, ContW, ResultContainer>::template constructin<ContX, ContC, ContV>)
					, &build_w,  boost::ref(val2), boost::ref(val3), boost::ref(val4), boost::ref(val1)));

				BuilderNode<Tree, ContX, ResultContainer> build_x(val2, result);
				build(build_x, 
					boost::bind(
					static_cast<ATree* (BuilderNode<Tree, ContX, ResultContainer>::*)(const ContW&, const ContC&, const ContV&, const ContX&) >
					(&BuilderNode<Tree, ContX, ResultContainer>::template constructin<ContW, ContC, ContV>)
					, &build_x, boost::ref(val1), boost::ref(val3), boost::ref(val4), boost::ref(val2)));

				BuilderNode<Tree, ContC, ResultContainer> build_c(val3, result);
				build(build_c, 
					boost::bind(
					static_cast<ATree* (BuilderNode<Tree, ContC, ResultContainer>::*)(const ContW&, const ContX&, const ContV&, const ContC&) >
					(&BuilderNode<Tree, ContC, ResultContainer>::template constructin<ContW, ContX, ContV>)
					, &build_c, boost::ref(val1), boost::ref(val2), boost::ref(val4), boost::ref(val3)));

				BuilderNode<Tree, ContV, ResultContainer> build_v(val4, result);
				build(build_v, 
					boost::bind(
					static_cast<ATree* (BuilderNode<Tree, ContV, ResultContainer>::*)(const ContW&, const ContX&, const ContC&, const ContV&) >
					(&BuilderNode<Tree, ContV, ResultContainer>::template constructin<ContW, ContX, ContC>)
					, &build_v, boost::ref(val1), boost::ref(val2), boost::ref(val3), boost::ref(val4)));

				return build();
			}

			template <typename ResultContainer, typename ContW, typename ContX, typename ContC>
			static ATree* construct(const ResultContainer& result, const ContW& val1, const ContX& val2, const ContC& val3) {
				Builder<Tree> build(getRandomResult(result));

				BuilderNode<Tree, ContW, ResultContainer> build_w(val1, result);
				build(build_w, 
					boost::bind(
					static_cast<ATree* (BuilderNode<Tree, ContW, ResultContainer>::*)(const ContX&, const ContC&, const ContW&) >
					(&BuilderNode<Tree, ContW, ResultContainer>::template constructin<ContX, ContC>)
					, &build_w,  boost::ref(val2), boost::ref(val3), boost::ref(val1)));

				BuilderNode<Tree, ContX, ResultContainer> build_x(val2, result);
				build(build_x, 
					boost::bind(
					static_cast<ATree* (BuilderNode<Tree, ContX, ResultContainer>::*)(const ContW&, const ContC&, const ContX&) >
					(&BuilderNode<Tree, ContX, ResultContainer>::template constructin<ContW, ContC>)
					, &build_x, boost::ref(val1), boost::ref(val3), boost::ref(val2)));

				BuilderNode<Tree, ContC, ResultContainer> build_c(val3, result);
				build(build_c, 
					boost::bind(
					static_cast<ATree* (BuilderNode<Tree, ContC, ResultContainer>::*)(const ContW&, const ContX&, const ContC&) >
					(&BuilderNode<Tree, ContC, ResultContainer>::template constructin<ContW, ContX>)
					, &build_c, boost::ref(val1), boost::ref(val2), boost::ref(val3)));

				return build();
			}

			template <typename ResultContainer, typename ContW, typename ContX>
			static ATree* construct(const ResultContainer& result, const ContW& val1, const ContX& val2) {
				Builder<Tree> build(getRandomResult(result));

				BuilderNode<Tree, ContW, ResultContainer> build_w(val1, result);
				build(build_w, 
					boost::bind(
					static_cast<ATree* (BuilderNode<Tree, ContW, ResultContainer>::*)(const ContX&, const ContW&) >
					(&BuilderNode<Tree, ContW, ResultContainer>::template constructin<ContX>)
					, &build_w,  boost::ref(val2), boost::ref(val1)));

				BuilderNode<Tree, ContX, ResultContainer> build_x(val2, result);
				build(build_x, 
					boost::bind(
					static_cast<ATree* (BuilderNode<Tree, ContX, ResultContainer>::*)(const ContW&, const ContX&) >
					(&BuilderNode<Tree, ContX, ResultContainer>::template constructin<ContW>)
					, &build_x, boost::ref(val1), boost::ref(val2)));

				return build();
			}
						
			template <typename ResultContainer>
			static typename ResultContainer::value_type getRandomResult(const ResultContainer& container) {
				boost::random::random_device ranD;
				boost::random::mt19937 engine(ranD);
				boost::random::uniform_int_distribution<int> distribution(0, container.size() - 1);
				
				int value = distribution(engine);

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

		};

	}

}

#endif /* !TREEBUILDER_HPP */