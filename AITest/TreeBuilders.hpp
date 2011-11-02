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

#include <random>

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

			template <typename BaseType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11>
			static boost::function< ATree*() > bindFunction(BaseType& base, const Arg1& val1, const Arg2& val2, const Arg3& val3, const Arg4& val4, const Arg5& val5, const Arg6& val6, const Arg7& val7, const Arg8& val8, const Arg9& val9, const Arg10& val10, const Arg11& val11) {
				return boost::bind(
							static_cast<ATree* (BaseType::*)(const Arg1&, const Arg2&, const Arg3&, const Arg4&, const Arg5&, const Arg6&, const Arg7&, const Arg8&, const Arg9&, const Arg10&, const Arg11&) >
							(&BaseType::template constructin<Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11>)
							, &base, boost::ref(val1), boost::ref(val2), boost::ref(val3), boost::ref(val4), boost::ref(val5), boost::ref(val6), boost::ref(val7), boost::ref(val8), boost::ref(val9), boost::ref(val10), boost::ref(val11));
			}

			template <typename BaseType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10>
			static boost::function< ATree*() > bindFunction(BaseType& base, const Arg1& val1, const Arg2& val2, const Arg3& val3, const Arg4& val4, const Arg5& val5, const Arg6& val6, const Arg7& val7, const Arg8& val8, const Arg9& val9, const Arg10& val10) {
				return boost::bind(
							static_cast<ATree* (BaseType::*)(const Arg1&, const Arg2&, const Arg3&, const Arg4&, const Arg5&, const Arg6&, const Arg7&, const Arg8&, const Arg9&, const Arg10&) >
							(&BaseType::template constructin<Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10>)
							, &base, boost::ref(val1), boost::ref(val2), boost::ref(val3), boost::ref(val4), boost::ref(val5), boost::ref(val6), boost::ref(val7), boost::ref(val8), boost::ref(val9), boost::ref(val10));
			}

			template <typename BaseType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9>
			static boost::function< ATree*() > bindFunction(BaseType& base, const Arg1& val1, const Arg2& val2, const Arg3& val3, const Arg4& val4, const Arg5& val5, const Arg6& val6, const Arg7& val7, const Arg8& val8, const Arg9& val9) {
				return boost::bind(
							static_cast<ATree* (BaseType::*)(const Arg1&, const Arg2&, const Arg3&, const Arg4&, const Arg5&, const Arg6&, const Arg7&, const Arg8&, const Arg9&) >
							(&BaseType::template constructin<Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9>)
							, &base, boost::ref(val1), boost::ref(val2), boost::ref(val3), boost::ref(val4), boost::ref(val5), boost::ref(val6), boost::ref(val7), boost::ref(val8), boost::ref(val9));
			}

			template <typename BaseType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
			static boost::function< ATree*() > bindFunction(BaseType& base, const Arg1& val1, const Arg2& val2, const Arg3& val3, const Arg4& val4, const Arg5& val5, const Arg6& val6, const Arg7& val7, const Arg8& val8) {
				return boost::bind(
							static_cast<ATree* (BaseType::*)(const Arg1&, const Arg2&, const Arg3&, const Arg4&, const Arg5&, const Arg6&, const Arg7&, const Arg8&) >
							(&BaseType::template constructin<Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8>)
							, &base, boost::ref(val1), boost::ref(val2), boost::ref(val3), boost::ref(val4), boost::ref(val5), boost::ref(val6), boost::ref(val7), boost::ref(val8));
			}

			template <typename BaseType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
			static boost::function< ATree*() > bindFunction(BaseType& base, const Arg1& val1, const Arg2& val2, const Arg3& val3, const Arg4& val4, const Arg5& val5, const Arg6& val6, const Arg7& val7) {
				return boost::bind(
							static_cast<ATree* (BaseType::*)(const Arg1&, const Arg2&, const Arg3&, const Arg4&, const Arg5&, const Arg6&, const Arg7&) >
							(&BaseType::template constructin<Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7>)
							, &base, boost::ref(val1), boost::ref(val2), boost::ref(val3), boost::ref(val4), boost::ref(val5), boost::ref(val6), boost::ref(val7));
			}

			template <typename BaseType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
			static boost::function< ATree*() > bindFunction(BaseType& base, const Arg1& val1, const Arg2& val2, const Arg3& val3, const Arg4& val4, const Arg5& val5, const Arg6& val6) {
				return boost::bind(
							static_cast<ATree* (BaseType::*)(const Arg1&, const Arg2&, const Arg3&, const Arg4&, const Arg5&, const Arg6&) >
							(&BaseType::template constructin<Arg1, Arg2, Arg3, Arg4, Arg5, Arg6>)
							, &base, boost::ref(val1), boost::ref(val2), boost::ref(val3), boost::ref(val4), boost::ref(val5), boost::ref(val6));
			}

			template <typename BaseType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
			static boost::function< ATree*() > bindFunction(BaseType& base, const Arg1& val1, const Arg2& val2, const Arg3& val3, const Arg4& val4, const Arg5& val5) {
				return boost::bind(
							static_cast<ATree* (BaseType::*)(const Arg1&, const Arg2&, const Arg3&, const Arg4&, const Arg5&) >
							(&BaseType::template constructin<Arg1, Arg2, Arg3, Arg4, Arg5>)
							, &base, boost::ref(val1), boost::ref(val2), boost::ref(val3), boost::ref(val4), boost::ref(val5));
			}

			template <typename BaseType, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
			static boost::function< ATree*() > bindFunction(BaseType& base, const Arg1& val1, const Arg2& val2, const Arg3& val3, const Arg4& val4) {
				return boost::bind(
							static_cast<ATree* (BaseType::*)(const Arg1&, const Arg2&, const Arg3&, const Arg4&) >
							(&BaseType::template constructin<Arg1, Arg2, Arg3, Arg4>)
							, &base, boost::ref(val1), boost::ref(val2), boost::ref(val3), boost::ref(val4));
			}

			template <typename BaseType, typename Arg1, typename Arg2, typename Arg3>
			static boost::function< ATree*() > bindFunction(BaseType& base, const Arg1& val1, const Arg2& val2, const Arg3& val3) {
				return boost::bind(
							static_cast<ATree* (BaseType::*)(const Arg1&, const Arg2&, const Arg3&) >
							(&BaseType::template constructin<Arg1, Arg2, Arg3>)
							, &base, boost::ref(val1), boost::ref(val2), boost::ref(val3));
			}

			template <typename BaseType, typename Arg1, typename Arg2>
			static boost::function< ATree*() > bindFunction(BaseType& base, const Arg1& val1, const Arg2& val2) {
				return boost::bind(
							static_cast<ATree* (BaseType::*)(const Arg1&, const Arg2&) >
							(&BaseType::template constructin<Arg1, Arg2>)
							, &base, boost::ref(val1), boost::ref(val2));
			}

			template <typename BaseType, typename Arg1>
			static boost::function< ATree*() > bindFunction(BaseType& base, const Arg1& val1) {
				return boost::bind(static_cast< ATree* (BaseType::*)(const Arg1&) >(&BaseType::template constructin<Arg1>), &base, boost::ref(val1));
			}

			template <typename BaseType>
			static boost::function< ATree*() > bindFunction(BaseType& base) {
				return boost::bind(static_cast<ATree* (BaseType::*)() >(&BaseType::constructin), &base);
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

			const SpecializedContainer& _attr;
			const ResultContainer& res_;
			ConditionnalFunctor* func_;
			Gain< Specialized, Result > gain_;

		public:
			template <typename Conditional>
			BuilderNode(const SpecializedContainer& attr, const ResultContainer& res, Conditional& func)
				: _attr(attr), res_(res), func_(&func), gain_(attr, res, func)
			{}

			BuilderNode(const SpecializedContainer& attr, const ResultContainer& res)
				: _attr(attr), res_(res), func_(0), gain_(attr, res) 
			{}

			BuilderNode(const BuilderNode& orig) 
				: _attr(orig._attr), res_(orig.res_), func_(orig.func_), gain_(orig.gain_)
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

			template <typename Cont1, typename Cont2, typename Cont3, typename Cont4, typename Cont5, typename Cont6, typename Cont7, typename Cont8, typename Cont9, typename Cont10, typename Cont11>
			ATree* constructin(const Cont1& val1, const Cont2& val2, const Cont3& val3, const Cont4& val4, const Cont5& val5, const Cont6& val6, const Cont7& val7, const Cont8& val8, const Cont9& val9, const Cont10& val10, const Cont11& val11) {
				TreeNode* root = new TreeNode();

				const AttribueMap& map = gain_.getAttribueMap();
				typename AttribueMap::const_iterator it = map.begin();
				typename AttribueMap::const_iterator ite = map.end();

				while (it != ite) {
					Builder< Tree >	 build(getMainValue());

					ConditionnalFunctorSpe func(_attr, it->first, func_);

					BuilderNode<Tree, Cont1, ResultContainer> build_1(val1, res_, func);
					build(build_1, Builder<Tree>::bindFunction(build_1, val2, val3, val4, val5, val6, val7, val8, val9, val10, val11));

					BuilderNode<Tree, Cont2, ResultContainer> build_2(val2, res_, func);
					build(build_2, Builder<Tree>::bindFunction(build_2, val1, val3, val4, val5, val6, val7, val8, val9, val10, val11));

					BuilderNode<Tree, Cont3, ResultContainer> build_3(val3, res_, func);
					build(build_3, Builder<Tree>::bindFunction(build_3, val1, val2, val4, val5, val6, val7, val8, val9, val10, val11));

					BuilderNode<Tree, Cont4, ResultContainer> build_4(val4, res_, func);
					build(build_4, Builder<Tree>::bindFunction(build_4, val1, val2, val3, val5, val6, val7, val8, val9, val10, val11));

					BuilderNode<Tree, Cont5, ResultContainer> build_5(val5, res_, func);
					build(build_5, Builder<Tree>::bindFunction(build_5, val1, val2, val3, val4, val6, val7, val8, val9, val10, val11));

					BuilderNode<Tree, Cont6, ResultContainer> build_6(val6, res_, func);
					build(build_6, Builder<Tree>::bindFunction(build_6, val1, val2, val3, val4, val5, val7, val8, val9, val10, val11));

					BuilderNode<Tree, Cont7, ResultContainer> build_7(val7, res_, func);
					build(build_7, Builder<Tree>::bindFunction(build_7, val1, val2, val3, val4, val5, val6, val8, val9, val10, val11));

					BuilderNode<Tree, Cont8, ResultContainer> build_8(val8, res_, func);
					build(build_8, Builder<Tree>::bindFunction(build_8, val1, val2, val3, val4, val5, val6, val7, val9, val10, val11));

					BuilderNode<Tree, Cont9, ResultContainer> build_9(val9, res_, func);
					build(build_9, Builder<Tree>::bindFunction(build_9, val1, val2, val3, val4, val5, val6, val7, val8, val10, val11));

					BuilderNode<Tree, Cont10, ResultContainer> build_10(val10, res_, func);
					build(build_10, Builder<Tree>::bindFunction(build_10, val1, val2, val3, val4, val5, val6, val7, val8, val9, val11));

					BuilderNode<Tree, Cont11, ResultContainer> build_11(val11, res_, func);
					build(build_11, Builder<Tree>::bindFunction(build_11, val1, val2, val3, val4, val5, val6, val7, val8, val9, val10));

					root->AddSubNode(it->first, build());
					++it;
				}

				return root;
			}

			template <typename Cont1, typename Cont2, typename Cont3, typename Cont4, typename Cont5, typename Cont6, typename Cont7, typename Cont8, typename Cont9, typename Cont10>
			ATree* constructin(const Cont1& val1, const Cont2& val2, const Cont3& val3, const Cont4& val4, const Cont5& val5, const Cont6& val6, const Cont7& val7, const Cont8& val8, const Cont9& val9, const Cont10& val10) {
				TreeNode* root = new TreeNode();

				const AttribueMap& map = gain_.getAttribueMap();
				typename AttribueMap::const_iterator it = map.begin();
				typename AttribueMap::const_iterator ite = map.end();

				while (it != ite) {
					Builder< Tree >	 build(getMainValue());

					ConditionnalFunctorSpe func(_attr, it->first, func_);

					BuilderNode<Tree, Cont1, ResultContainer> build_1(val1, res_, func);
					build(build_1, Builder<Tree>::bindFunction(build_1, val2, val3, val4, val5, val6, val7, val8, val9, val10));

					BuilderNode<Tree, Cont2, ResultContainer> build_2(val2, res_, func);
					build(build_2, Builder<Tree>::bindFunction(build_2, val1, val3, val4, val5, val6, val7, val8, val9, val10));

					BuilderNode<Tree, Cont3, ResultContainer> build_3(val3, res_, func);
					build(build_3, Builder<Tree>::bindFunction(build_3, val1, val2, val4, val5, val6, val7, val8, val9, val10));

					BuilderNode<Tree, Cont4, ResultContainer> build_4(val4, res_, func);
					build(build_4, Builder<Tree>::bindFunction(build_4, val1, val2, val3, val5, val6, val7, val8, val9, val10));

					BuilderNode<Tree, Cont5, ResultContainer> build_5(val5, res_, func);
					build(build_5, Builder<Tree>::bindFunction(build_5, val1, val2, val3, val4, val6, val7, val8, val9, val10));

					BuilderNode<Tree, Cont6, ResultContainer> build_6(val6, res_, func);
					build(build_6, Builder<Tree>::bindFunction(build_6, val1, val2, val3, val4, val5, val7, val8, val9, val10));

					BuilderNode<Tree, Cont7, ResultContainer> build_7(val7, res_, func);
					build(build_7, Builder<Tree>::bindFunction(build_7, val1, val2, val3, val4, val5, val6, val8, val9, val10));

					BuilderNode<Tree, Cont8, ResultContainer> build_8(val8, res_, func);
					build(build_8, Builder<Tree>::bindFunction(build_8, val1, val2, val3, val4, val5, val6, val7, val9, val10));

					BuilderNode<Tree, Cont9, ResultContainer> build_9(val9, res_, func);
					build(build_9, Builder<Tree>::bindFunction(build_9, val1, val2, val3, val4, val5, val6, val7, val8, val10));

					BuilderNode<Tree, Cont10, ResultContainer> build_10(val10, res_, func);
					build(build_10, Builder<Tree>::bindFunction(build_10, val1, val2, val3, val4, val5, val6, val7, val8, val9));

					root->AddSubNode(it->first, build());
					++it;
				}

				return root;
			}

			template <typename Cont1, typename Cont2, typename Cont3, typename Cont4, typename Cont5, typename Cont6, typename Cont7, typename Cont8, typename Cont9>
			ATree* constructin(const Cont1& val1, const Cont2& val2, const Cont3& val3, const Cont4& val4, const Cont5& val5, const Cont6& val6, const Cont7& val7, const Cont8& val8, const Cont9& val9) {
				TreeNode* root = new TreeNode();

				const AttribueMap& map = gain_.getAttribueMap();
				typename AttribueMap::const_iterator it = map.begin();
				typename AttribueMap::const_iterator ite = map.end();

				while (it != ite) {
					Builder< Tree >	 build(getMainValue());

					ConditionnalFunctorSpe func(_attr, it->first, func_);

					BuilderNode<Tree, Cont1, ResultContainer> build_1(val1, res_, func);
					build(build_1, Builder<Tree>::bindFunction(build_1, val2, val3, val4, val5, val6, val7, val8, val9));

					BuilderNode<Tree, Cont2, ResultContainer> build_2(val2, res_, func);
					build(build_2, Builder<Tree>::bindFunction(build_2, val1, val3, val4, val5, val6, val7, val8, val9));

					BuilderNode<Tree, Cont3, ResultContainer> build_3(val3, res_, func);
					build(build_3, Builder<Tree>::bindFunction(build_3, val1, val2, val4, val5, val6, val7, val8, val9));

					BuilderNode<Tree, Cont4, ResultContainer> build_4(val4, res_, func);
					build(build_4, Builder<Tree>::bindFunction(build_4, val1, val2, val3, val5, val6, val7, val8, val9));

					BuilderNode<Tree, Cont5, ResultContainer> build_5(val5, res_, func);
					build(build_5, Builder<Tree>::bindFunction(build_5, val1, val2, val3, val4, val6, val7, val8, val9));

					BuilderNode<Tree, Cont6, ResultContainer> build_6(val6, res_, func);
					build(build_6, Builder<Tree>::bindFunction(build_6, val1, val2, val3, val4, val5, val7, val8, val9));

					BuilderNode<Tree, Cont7, ResultContainer> build_7(val7, res_, func);
					build(build_7, Builder<Tree>::bindFunction(build_7, val1, val2, val3, val4, val5, val6, val8, val9));

					BuilderNode<Tree, Cont8, ResultContainer> build_8(val8, res_, func);
					build(build_8, Builder<Tree>::bindFunction(build_8, val1, val2, val3, val4, val5, val6, val7, val9));

					BuilderNode<Tree, Cont9, ResultContainer> build_9(val9, res_, func);
					build(build_9, Builder<Tree>::bindFunction(build_9, val1, val2, val3, val4, val5, val6, val7, val8));

					root->AddSubNode(it->first, build());
					++it;
				}

				return root;
			}

			template <typename Cont1, typename Cont2, typename Cont3, typename Cont4, typename Cont5, typename Cont6, typename Cont7, typename Cont8>
			ATree* constructin(const Cont1& val1, const Cont2& val2, const Cont3& val3, const Cont4& val4, const Cont5& val5, const Cont6& val6, const Cont7& val7, const Cont8& val8) {
				TreeNode* root = new TreeNode();

				const AttribueMap& map = gain_.getAttribueMap();
				typename AttribueMap::const_iterator it = map.begin();
				typename AttribueMap::const_iterator ite = map.end();

				while (it != ite) {
					Builder< Tree >	 build(getMainValue());

					ConditionnalFunctorSpe func(_attr, it->first, func_);

					BuilderNode<Tree, Cont1, ResultContainer> build_1(val1, res_, func);
					build(build_1, Builder<Tree>::bindFunction(build_1, val2, val3, val4, val5, val6, val7, val8));

					BuilderNode<Tree, Cont2, ResultContainer> build_2(val2, res_, func);
					build(build_2, Builder<Tree>::bindFunction(build_2, val1, val3, val4, val5, val6, val7, val8));

					BuilderNode<Tree, Cont3, ResultContainer> build_3(val3, res_, func);
					build(build_3, Builder<Tree>::bindFunction(build_3, val1, val2, val4, val5, val6, val7, val8));

					BuilderNode<Tree, Cont4, ResultContainer> build_4(val4, res_, func);
					build(build_4, Builder<Tree>::bindFunction(build_4, val1, val2, val3, val5, val6, val7, val8));

					BuilderNode<Tree, Cont5, ResultContainer> build_5(val5, res_, func);
					build(build_5, Builder<Tree>::bindFunction(build_5, val1, val2, val3, val4, val6, val7, val8));

					BuilderNode<Tree, Cont6, ResultContainer> build_6(val6, res_, func);
					build(build_6, Builder<Tree>::bindFunction(build_6, val1, val2, val3, val4, val5, val7, val8));

					BuilderNode<Tree, Cont7, ResultContainer> build_7(val7, res_, func);
					build(build_7, Builder<Tree>::bindFunction(build_7, val1, val2, val3, val4, val5, val6, val8));

					BuilderNode<Tree, Cont8, ResultContainer> build_8(val8, res_, func);
					build(build_8, Builder<Tree>::bindFunction(build_8, val1, val2, val3, val4, val5, val6, val7));

					root->AddSubNode(it->first, build());
					++it;
				}

				return root;
			}

			template <typename Cont1, typename Cont2, typename Cont3, typename Cont4, typename Cont5, typename Cont6, typename Cont7>
			ATree* constructin(const Cont1& val1, const Cont2& val2, const Cont3& val3, const Cont4& val4, const Cont5& val5, const Cont6& val6, const Cont7& val7) {
				TreeNode* root = new TreeNode();

				const AttribueMap& map = gain_.getAttribueMap();
				typename AttribueMap::const_iterator it = map.begin();
				typename AttribueMap::const_iterator ite = map.end();

				while (it != ite) {
					Builder< Tree >	 build(getMainValue());

					ConditionnalFunctorSpe func(_attr, it->first, func_);

					BuilderNode<Tree, Cont1, ResultContainer> build_1(val1, res_, func);
					build(build_1, Builder<Tree>::bindFunction(build_1, val2, val3, val4, val5, val6, val7));

					BuilderNode<Tree, Cont2, ResultContainer> build_2(val2, res_, func);
					build(build_2, Builder<Tree>::bindFunction(build_2, val1, val3, val4, val5, val6, val7));

					BuilderNode<Tree, Cont3, ResultContainer> build_3(val3, res_, func);
					build(build_3, Builder<Tree>::bindFunction(build_3, val1, val2, val4, val5, val6, val7));

					BuilderNode<Tree, Cont4, ResultContainer> build_4(val4, res_, func);
					build(build_4, Builder<Tree>::bindFunction(build_4, val1, val2, val3, val5, val6, val7));

					BuilderNode<Tree, Cont5, ResultContainer> build_5(val5, res_, func);
					build(build_5, Builder<Tree>::bindFunction(build_5, val1, val2, val3, val4, val6, val7));

					BuilderNode<Tree, Cont6, ResultContainer> build_6(val6, res_, func);
					build(build_6, Builder<Tree>::bindFunction(build_6, val1, val2, val3, val4, val5, val7));

					BuilderNode<Tree, Cont7, ResultContainer> build_7(val7, res_, func);
					build(build_7, Builder<Tree>::bindFunction(build_7, val1, val2, val3, val4, val5, val6));

					root->AddSubNode(it->first, build());
					++it;
				}

				return root;
			}

			template <typename Cont1, typename Cont2, typename Cont3, typename Cont4, typename Cont5, typename Cont6>
			ATree* constructin(const Cont1& val1, const Cont2& val2, const Cont3& val3, const Cont4& val4, const Cont5& val5, const Cont6& val6) {
				TreeNode* root = new TreeNode();

				const AttribueMap& map = gain_.getAttribueMap();
				typename AttribueMap::const_iterator it = map.begin();
				typename AttribueMap::const_iterator ite = map.end();

				while (it != ite) {
					Builder< Tree >	 build(getMainValue());

					ConditionnalFunctorSpe func(_attr, it->first, func_);

					BuilderNode<Tree, Cont1, ResultContainer> build_1(val1, res_, func);
					build(build_1, Builder<Tree>::bindFunction(build_1, val2, val3, val4, val5, val6));

					BuilderNode<Tree, Cont2, ResultContainer> build_2(val2, res_, func);
					build(build_2, Builder<Tree>::bindFunction(build_2, val1, val3, val4, val5, val6));

					BuilderNode<Tree, Cont3, ResultContainer> build_3(val3, res_, func);
					build(build_3, Builder<Tree>::bindFunction(build_3, val1, val2, val4, val5, val6));

					BuilderNode<Tree, Cont4, ResultContainer> build_4(val4, res_, func);
					build(build_4, Builder<Tree>::bindFunction(build_4, val1, val2, val3, val5, val6));

					BuilderNode<Tree, Cont5, ResultContainer> build_5(val5, res_, func);
					build(build_5, Builder<Tree>::bindFunction(build_5, val1, val2, val3, val4, val6));

					BuilderNode<Tree, Cont6, ResultContainer> build_6(val6, res_, func);
					build(build_6, Builder<Tree>::bindFunction(build_6, val1, val2, val3, val4, val5));

					root->AddSubNode(it->first, build());
					++it;
				}

				return root;
			}

			template <typename Cont1, typename Cont2, typename Cont3, typename Cont4, typename Cont5>
			ATree* constructin(const Cont1& val1, const Cont2& val2, const Cont3& val3, const Cont4& val4, const Cont5& val5) {
				TreeNode* root = new TreeNode();

				const AttribueMap& map = gain_.getAttribueMap();
				typename AttribueMap::const_iterator it = map.begin();
				typename AttribueMap::const_iterator ite = map.end();

				while (it != ite) {
					Builder< Tree >	 build(getMainValue());

					ConditionnalFunctorSpe func(_attr, it->first, func_);

					BuilderNode<Tree, Cont1, ResultContainer> build_1(val1, res_, func);
					build(build_1, Builder<Tree>::bindFunction(build_1, val2, val3, val4, val5));

					BuilderNode<Tree, Cont2, ResultContainer> build_2(val2, res_, func);
					build(build_2, Builder<Tree>::bindFunction(build_2, val1, val3, val4, val5));

					BuilderNode<Tree, Cont3, ResultContainer> build_3(val3, res_, func);
					build(build_3, Builder<Tree>::bindFunction(build_3, val1, val2, val4, val5));

					BuilderNode<Tree, Cont4, ResultContainer> build_4(val4, res_, func);
					build(build_4, Builder<Tree>::bindFunction(build_4, val1, val2, val3, val5));

					BuilderNode<Tree, Cont5, ResultContainer> build_5(val5, res_, func);
					build(build_5, Builder<Tree>::bindFunction(build_5, val1, val2, val3, val4));

					root->AddSubNode(it->first, build());
					++it;
				}

				return root;
			}

			template <typename Cont1, typename Cont2, typename Cont3, typename Cont4>
			ATree* constructin(const Cont1& val1, const Cont2& val2, const Cont3& val3, const Cont4& val4) {
				TreeNode* root = new TreeNode();

				const AttribueMap& map = gain_.getAttribueMap();
				typename AttribueMap::const_iterator it = map.begin();
				typename AttribueMap::const_iterator ite = map.end();

				while (it != ite) {
					Builder< Tree >	 build(getMainValue());

					ConditionnalFunctorSpe func(_attr, it->first, func_);

					BuilderNode<Tree, Cont1, ResultContainer> build_1(val1, res_, func);
					build(build_1, Builder<Tree>::bindFunction(build_1, val2, val3, val4));

					BuilderNode<Tree, Cont2, ResultContainer> build_2(val2, res_, func);
					build(build_2, Builder<Tree>::bindFunction(build_2, val1, val3, val4));

					BuilderNode<Tree, Cont3, ResultContainer> build_3(val3, res_, func);
					build(build_3, Builder<Tree>::bindFunction(build_3, val1, val2, val4));

					BuilderNode<Tree, Cont4, ResultContainer> build_4(val4, res_, func);
					build(build_4, Builder<Tree>::bindFunction(build_3, val1, val2, val3));

					root->AddSubNode(it->first, build());
					++it;
				}

				return root;
			}

			template <typename Cont1, typename Cont2, typename Cont3>
			ATree* constructin(const Cont1& val1, const Cont2& val2, const Cont3& val3) {
				TreeNode* root = new TreeNode();

				const AttribueMap& map = gain_.getAttribueMap();
				typename AttribueMap::const_iterator it = map.begin();
				typename AttribueMap::const_iterator ite = map.end();

				while (it != ite) {
					Builder< Tree >	 build(getMainValue());
					ConditionnalFunctorSpe func(_attr, it->first, func_);

					BuilderNode<Tree, Cont1, ResultContainer> build_f(val1, res_, func);
					build(build_f, Builder<Tree>::bindFunction(build_f, val2, val3));

					BuilderNode<Tree, Cont2, ResultContainer> build_s(val2, res_, func);
					build(build_s,  Builder<Tree>::bindFunction(build_s, val1, val3));

					BuilderNode<Tree, Cont3, ResultContainer> build_t(val3, res_, func);
					build(build_t,  Builder<Tree>::bindFunction(build_t, val1, val2));

					root->AddSubNode(it->first, build());
					++it;
				}

				return root;
			}

			template <typename Cont1, typename Cont2>
			ATree* constructin(const Cont1& val1, const Cont2& val2) {
				TreeNode* root = new TreeNode();

				const AttribueMap& map = gain_.getAttribueMap();
				typename AttribueMap::const_iterator it = map.begin();
				typename AttribueMap::const_iterator ite = map.end();

				//Evaluate every value of the current Node Attribute.
				while (it != ite) {
					Builder< Tree >	 build(getMainValue());

					ConditionnalFunctorSpe func(_attr, it->first, func_);

					BuilderNode<Tree, Cont1, ResultContainer> build_f(val1, res_, func);
					build(build_f,  Builder<Tree>::bindFunction(build_f, val2));

					BuilderNode<Tree, Cont2, ResultContainer> build_s(val2, res_, func);
					build(build_s,  Builder<Tree>::bindFunction(build_s, val1));

					root->AddSubNode(it->first, build());
					++it;
				}

				return root;
			}

			template <typename Cont1>
			ATree* constructin(const Cont1& val1) {
				TreeNode* root = new TreeNode();

				const AttribueMap& map = gain_.getAttribueMap();
				typename AttribueMap::const_iterator it = map.begin();
				typename AttribueMap::const_iterator ite = map.end();

				while (it != ite) {
					Builder< Tree > build(getMainValue());

					ConditionnalFunctorSpe func(_attr, it->first, func_);

					func.reset();
					BuilderNode<Tree, Cont1, ResultContainer> build_f(val1, res_, func);
					build(build_f, Builder<Tree>::bindFunction(build_f));

					root->AddSubNode(it->first, build());

					++it;
				}

				return root;
			}

			ATree* constructin() {
				TreeNode* root = new TreeNode();

				const AttribueMap& map = gain_.getAttribueMap();
				typename AttribueMap::const_iterator it = map.begin();
				typename AttribueMap::const_iterator ite = map.end();

				while (it != ite) {
					ConditionnalFunctorSpe func(_attr, it->first, func_);

					func.reset();
					Gain< Specialized, Result > gain;
					gain.getGain(_attr, res_, func);

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
			
			template <typename ResultContainer, typename Cont1, typename Cont2, typename Cont3, typename Cont4, typename Cont5, typename Cont6, typename Cont7, typename Cont8, typename Cont9, typename Cont10, typename Cont11>
			static ATree* construct(const ResultContainer& result, const Cont1& val1, const Cont2& val2, const Cont3& val3, const Cont4& val4, const Cont5& val5, const Cont6& val6, const Cont7& val7, const Cont8& val8, const Cont9& val9, const Cont10& val10, const Cont11& val11) {
				Builder<Tree> build(getRandomResult(result));

				BuilderNode<Tree, Cont1, ResultContainer> build_1(val1, result);
				build(build_1, Builder<Tree>::bindFunction(build_1, val2, val3, val4, val5, val6, val7, val8, val9, val10, val11));

				BuilderNode<Tree, Cont2, ResultContainer> build_2(val2, result);
				build(build_2, Builder<Tree>::bindFunction(build_2, val1, val3, val4, val5, val6, val7, val8, val9, val10, val11));

				BuilderNode<Tree, Cont3, ResultContainer> build_3(val3, result);
				build(build_3, Builder<Tree>::bindFunction(build_3, val1, val2, val4, val5, val6, val7, val8, val9, val10, val11));

				BuilderNode<Tree, Cont4, ResultContainer> build_4(val4, result);
				build(build_4, Builder<Tree>::bindFunction(build_4, val1, val2, val3, val5, val6, val7, val8, val9, val10, val11));
			
				BuilderNode<Tree, Cont5, ResultContainer> build_5(val5, result);
				build(build_5, Builder<Tree>::bindFunction(build_5, val1, val2, val3, val4, val6, val7, val8, val9, val10, val11));

				BuilderNode<Tree, Cont6, ResultContainer> build_6(val6, result);
				build(build_6, Builder<Tree>::bindFunction(build_6, val1, val2, val3, val4, val5, val7, val8, val9, val10, val11));

				BuilderNode<Tree, Cont7, ResultContainer> build_7(val7, result);
				build(build_7, Builder<Tree>::bindFunction(build_7, val1, val2, val3, val4, val5, val6, val8, val9, val10, val11));

				BuilderNode<Tree, Cont8, ResultContainer> build_8(val8, result);
				build(build_8, Builder<Tree>::bindFunction(build_8, val1, val2, val3, val4, val5, val6, val7, val9, val10, val11));

				BuilderNode<Tree, Cont9, ResultContainer> build_9(val9, result);
				build(build_9, Builder<Tree>::bindFunction(build_9, val1, val2, val3, val4, val5, val6, val7, val8, val10, val11));

				BuilderNode<Tree, Cont10, ResultContainer> build_10(val10, result);
				build(build_10, Builder<Tree>::bindFunction(build_10, val1, val2, val3, val4, val5, val6, val7, val8, val9, val11));

				BuilderNode<Tree, Cont11, ResultContainer> build_11(val10, result);
				build(build_11, Builder<Tree>::bindFunction(build_11, val1, val2, val3, val4, val5, val6, val7, val8, val9, val10));

				return build();
			}

			template <typename ResultContainer, typename Cont1, typename Cont2, typename Cont3, typename Cont4, typename Cont5, typename Cont6, typename Cont7, typename Cont8, typename Cont9, typename Cont10>
			static ATree* construct(const ResultContainer& result, const Cont1& val1, const Cont2& val2, const Cont3& val3, const Cont4& val4, const Cont5& val5, const Cont6& val6, const Cont7& val7, const Cont8& val8, const Cont9& val9, const Cont10& val10) {
				Builder<Tree> build(getRandomResult(result));

				BuilderNode<Tree, Cont1, ResultContainer> build_1(val1, result);
				build(build_1, Builder<Tree>::bindFunction(build_1, val2, val3, val4, val5, val6, val7, val8, val9, val10));

				BuilderNode<Tree, Cont2, ResultContainer> build_2(val2, result);
				build(build_2, Builder<Tree>::bindFunction(build_2, val1, val3, val4, val5, val6, val7, val8, val9, val10));

				BuilderNode<Tree, Cont3, ResultContainer> build_3(val3, result);
				build(build_3, Builder<Tree>::bindFunction(build_3, val1, val2, val4, val5, val6, val7, val8, val9, val10));

				BuilderNode<Tree, Cont4, ResultContainer> build_4(val4, result);
				build(build_4, Builder<Tree>::bindFunction(build_4, val1, val2, val3, val5, val6, val7, val8, val9, val10));
			
				BuilderNode<Tree, Cont5, ResultContainer> build_5(val5, result);
				build(build_5, Builder<Tree>::bindFunction(build_5, val1, val2, val3, val4, val6, val7, val8, val9, val10));

				BuilderNode<Tree, Cont6, ResultContainer> build_6(val6, result);
				build(build_6, Builder<Tree>::bindFunction(build_6, val1, val2, val3, val4, val5, val7, val8, val9, val10));

				BuilderNode<Tree, Cont7, ResultContainer> build_7(val7, result);
				build(build_7, Builder<Tree>::bindFunction(build_7, val1, val2, val3, val4, val5, val6, val8, val9, val10));

				BuilderNode<Tree, Cont8, ResultContainer> build_8(val8, result);
				build(build_8, Builder<Tree>::bindFunction(build_8, val1, val2, val3, val4, val5, val6, val7, val9, val10));

				BuilderNode<Tree, Cont9, ResultContainer> build_9(val9, result);
				build(build_9, Builder<Tree>::bindFunction(build_9, val1, val2, val3, val4, val5, val6, val7, val8, val10));

				BuilderNode<Tree, Cont10, ResultContainer> build_10(val10, result);
				build(build_10, Builder<Tree>::bindFunction(build_10, val1, val2, val3, val4, val5, val6, val7, val8, val9));

				return build();
			}

			template <typename ResultContainer, typename Cont1, typename Cont2, typename Cont3, typename Cont4, typename Cont5, typename Cont6, typename Cont7, typename Cont8, typename Cont9>
			static ATree* construct(const ResultContainer& result, const Cont1& val1, const Cont2& val2, const Cont3& val3, const Cont4& val4, const Cont5& val5, const Cont6& val6, const Cont7& val7, const Cont8& val8, const Cont9& val9) {
				Builder<Tree> build(getRandomResult(result));

				BuilderNode<Tree, Cont1, ResultContainer> build_1(val1, result);
				build(build_1, Builder<Tree>::bindFunction(build_1, val2, val3, val4, val5, val6, val7, val8, val9));

				BuilderNode<Tree, Cont2, ResultContainer> build_2(val2, result);
				build(build_2, Builder<Tree>::bindFunction(build_2, val1, val3, val4, val5, val6, val7, val8, val9));

				BuilderNode<Tree, Cont3, ResultContainer> build_3(val3, result);
				build(build_3, Builder<Tree>::bindFunction(build_3, val1, val2, val4, val5, val6, val7, val8, val9));

				BuilderNode<Tree, Cont4, ResultContainer> build_4(val4, result);
				build(build_4, Builder<Tree>::bindFunction(build_4, val1, val2, val3, val5, val6, val7, val8, val9));
			
				BuilderNode<Tree, Cont5, ResultContainer> build_5(val5, result);
				build(build_5, Builder<Tree>::bindFunction(build_5, val1, val2, val3, val4, val6, val7, val8, val9));

				BuilderNode<Tree, Cont6, ResultContainer> build_6(val6, result);
				build(build_6, Builder<Tree>::bindFunction(build_6, val1, val2, val3, val4, val5, val7, val8, val9));

				BuilderNode<Tree, Cont7, ResultContainer> build_7(val7, result);
				build(build_7, Builder<Tree>::bindFunction(build_7, val1, val2, val3, val4, val5, val6, val8, val9));

				BuilderNode<Tree, Cont8, ResultContainer> build_8(val8, result);
				build(build_8, Builder<Tree>::bindFunction(build_8, val1, val2, val3, val4, val5, val6, val7, val9));

				BuilderNode<Tree, Cont9, ResultContainer> build_9(val9, result);
				build(build_9, Builder<Tree>::bindFunction(build_9, val1, val2, val3, val4, val5, val6, val7, val8));

				return build();
			}

			template <typename ResultContainer, typename Cont1, typename Cont2, typename Cont3, typename Cont4, typename Cont5, typename Cont6, typename Cont7, typename Cont8>
			static ATree* construct(const ResultContainer& result, const Cont1& val1, const Cont2& val2, const Cont3& val3, const Cont4& val4, const Cont5& val5, const Cont6& val6, const Cont7& val7, const Cont8& val8) {
				Builder<Tree> build(getRandomResult(result));

				BuilderNode<Tree, Cont1, ResultContainer> build_1(val1, result);
				build(build_1, Builder<Tree>::bindFunction(build_1, val2, val3, val4, val5, val6, val7, val8));

				BuilderNode<Tree, Cont2, ResultContainer> build_2(val2, result);
				build(build_2, Builder<Tree>::bindFunction(build_2, val1, val3, val4, val5, val6, val7, val8));

				BuilderNode<Tree, Cont3, ResultContainer> build_3(val3, result);
				build(build_3, Builder<Tree>::bindFunction(build_3, val1, val2, val4, val5, val6, val7, val8));

				BuilderNode<Tree, Cont4, ResultContainer> build_4(val4, result);
				build(build_4, Builder<Tree>::bindFunction(build_4, val1, val2, val3, val5, val6, val7, val8));
			
				BuilderNode<Tree, Cont5, ResultContainer> build_5(val5, result);
				build(build_5, Builder<Tree>::bindFunction(build_5, val1, val2, val3, val4, val6, val7, val8));

				BuilderNode<Tree, Cont6, ResultContainer> build_6(val6, result);
				build(build_6, Builder<Tree>::bindFunction(build_6, val1, val2, val3, val4, val5, val7, val8));

				BuilderNode<Tree, Cont7, ResultContainer> build_7(val7, result);
				build(build_7, Builder<Tree>::bindFunction(build_7, val1, val2, val3, val4, val5, val6, val8));

				BuilderNode<Tree, Cont8, ResultContainer> build_8(val8, result);
				build(build_8, Builder<Tree>::bindFunction(build_8, val1, val2, val3, val4, val5, val6, val7));

				return build();
			}

			template <typename ResultContainer, typename Cont1, typename Cont2, typename Cont3, typename Cont4, typename Cont5, typename Cont6, typename Cont7>
			static ATree* construct(const ResultContainer& result, const Cont1& val1, const Cont2& val2, const Cont3& val3, const Cont4& val4, const Cont5& val5, const Cont6& val6, const Cont7& val7) {
				Builder<Tree> build(getRandomResult(result));

				BuilderNode<Tree, Cont1, ResultContainer> build_1(val1, result);
				build(build_1, Builder<Tree>::bindFunction(build_1, val2, val3, val4, val5, val6, val7));

				BuilderNode<Tree, Cont2, ResultContainer> build_2(val2, result);
				build(build_2, Builder<Tree>::bindFunction(build_2, val1, val3, val4, val5, val6, val7));

				BuilderNode<Tree, Cont3, ResultContainer> build_3(val3, result);
				build(build_3, Builder<Tree>::bindFunction(build_3, val1, val2, val4, val5, val6, val7));

				BuilderNode<Tree, Cont4, ResultContainer> build_4(val4, result);
				build(build_4, Builder<Tree>::bindFunction(build_4, val1, val2, val3, val5, val6, val7));
			
				BuilderNode<Tree, Cont5, ResultContainer> build_5(val5, result);
				build(build_5, Builder<Tree>::bindFunction(build_5, val1, val2, val3, val4, val6, val7));

				BuilderNode<Tree, Cont6, ResultContainer> build_6(val6, result);
				build(build_6, Builder<Tree>::bindFunction(build_6, val1, val2, val3, val4, val5, val7));

				BuilderNode<Tree, Cont7, ResultContainer> build_7(val7, result);
				build(build_7, Builder<Tree>::bindFunction(build_7, val1, val2, val3, val4, val5, val6));

				return build();
			}

			template <typename ResultContainer, typename Cont1, typename Cont2, typename Cont3, typename Cont4, typename Cont5, typename Cont6>
			static ATree* construct(const ResultContainer& result, const Cont1& val1, const Cont2& val2, const Cont3& val3, const Cont4& val4, const Cont5& val5, const Cont6& val6) {
				Builder<Tree> build(getRandomResult(result));

				BuilderNode<Tree, Cont1, ResultContainer> build_w(val1, result);
				build(build_w, Builder<Tree>::bindFunction(build_w, val2, val3, val4, val5, val6));

				BuilderNode<Tree, Cont2, ResultContainer> build_x(val2, result);
				build(build_x, Builder<Tree>::bindFunction(build_x, val1, val3, val4, val5, val6));

				BuilderNode<Tree, Cont3, ResultContainer> build_c(val3, result);
				build(build_c, Builder<Tree>::bindFunction(build_c, val1, val2, val4, val5, val6));

				BuilderNode<Tree, Cont4, ResultContainer> build_v(val4, result);
				build(build_v, Builder<Tree>::bindFunction(build_v, val1, val2, val3, val5, val6));
			
				BuilderNode<Tree, Cont5, ResultContainer> build_b(val5, result);
				build(build_b, Builder<Tree>::bindFunction(build_b, val1, val2, val3, val4, val6));

				BuilderNode<Tree, Cont6, ResultContainer> build_n(val6, result);
				build(build_n, Builder<Tree>::bindFunction(build_n, val1, val2, val3, val4, val5));

				return build();
			}

			template <typename ResultContainer, typename Cont1, typename Cont2, typename Cont3, typename Cont4, typename Cont5>
			static ATree* construct(const ResultContainer& result, const Cont1& val1, const Cont2& val2, const Cont3& val3, const Cont4& val4, const Cont5& val5) {
				Builder<Tree> build(getRandomResult(result));

				BuilderNode<Tree, Cont1, ResultContainer> build_w(val1, result);
				build(build_w, Builder<Tree>::bindFunction(build_w, val2, val3, val4, val5));

				BuilderNode<Tree, Cont2, ResultContainer> build_x(val2, result);
				build(build_x, Builder<Tree>::bindFunction(build_x, val1, val3, val4, val5));

				BuilderNode<Tree, Cont3, ResultContainer> build_c(val3, result);
				build(build_c, Builder<Tree>::bindFunction(build_c, val1, val2, val4, val5));

				BuilderNode<Tree, Cont4, ResultContainer> build_v(val4, result);
				build(build_v, Builder<Tree>::bindFunction(build_v, val1, val2, val3, val5));
			
				BuilderNode<Tree, Cont5, ResultContainer> build_b(val5, result);
				build(build_b, Builder<Tree>::bindFunction(build_b, val1, val2, val3, val4));

				return build();
			}

			template <typename ResultContainer, typename Cont1, typename Cont2, typename Cont3, typename Cont4>
			static ATree* construct(const ResultContainer& result, const Cont1& val1, const Cont2& val2, const Cont3& val3, const Cont4& val4 ) {
				Builder<Tree> build(getRandomResult(result));

				BuilderNode<Tree, Cont1, ResultContainer> build_w(val1, result);
				build(build_w, Builder<Tree>::bindFunction(build_w, val2, val3, val4));

				BuilderNode<Tree, Cont2, ResultContainer> build_x(val2, result);
				build(build_x, Builder<Tree>::bindFunction(build_x, val1, val3, val4));

				BuilderNode<Tree, Cont3, ResultContainer> build_c(val3, result);
				build(build_c, Builder<Tree>::bindFunction(build_c, val1, val2, val4));

				BuilderNode<Tree, Cont4, ResultContainer> build_v(val4, result);
				build(build_v, Builder<Tree>::bindFunction(build_c, val1, val2, val3));

				return build();
			}

			template <typename ResultContainer, typename Cont1, typename Cont2, typename Cont3>
			static ATree* construct(const ResultContainer& result, const Cont1& val1, const Cont2& val2, const Cont3& val3) {
				Builder<Tree> build(getRandomResult(result));

				BuilderNode<Tree, Cont1, ResultContainer> build_w(val1, result);
				build(build_w, Builder<Tree>::bindFunction(build_w, val2, val3));

				BuilderNode<Tree, Cont2, ResultContainer> build_x(val2, result);
				build(build_x, Builder<Tree>::bindFunction(build_x, val1, val3));

				BuilderNode<Tree, Cont3, ResultContainer> build_c(val3, result);
				build(build_c, Builder<Tree>::bindFunction(build_x, val1, val2));

				return build();
			}

			template <typename ResultContainer, typename Cont1, typename Cont2>
			static ATree* construct(const ResultContainer& result, const Cont1& val1, const Cont2& val2) {
				Builder<Tree> build(getRandomResult(result));

				BuilderNode<Tree, Cont1, ResultContainer> build_w(val1, result);
				build(build_w,  Builder<Tree>::bindFunction(build_w, val2));

				BuilderNode<Tree, Cont2, ResultContainer> build_x(val2, result);
				build(build_x, Builder<Tree>::bindFunction(build_x, val1));

				return build();
			}

			template <typename ResultContainer, typename Cont1>
			static ATree* construct(const ResultContainer& result, const Cont1& val1) {
				Builder<Tree> build(getRandomResult(result));

				BuilderNode<Tree, Cont1, ResultContainer> build_w(val1, result);
				build(build_w,  Builder<Tree>::bindFunction(build_w));

				return build();
			}
						
			template <typename ResultContainer>
			static typename ResultContainer::value_type getRandomResult(const ResultContainer& container) {
				std::random_device ranD;
				std::uniform_int_distribution<int> distribution(0, container.size() - 1);
				
				int value = distribution(ranD);

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