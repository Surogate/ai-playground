/* 
 * File:   TypedTree.hpp
 * Author: Francois Ancel (francoisancel [at] gmail.com)
 * Copyright : BSD license
 * Created on September 16, 2011
 */

#ifndef TYPEDTREE_HPP
#define TYPEDTREE_HPP

#include <boost/shared_ptr.hpp>
#include <memory>
#include <iostream>


namespace ID3 {
/*
	Hold decisional tree structure.
*/
	namespace TypedTree {

		template <typename T>
		struct Debugger {
		public:
			Debugger() {
				std::cout << "debug standard" << std::endl;
			}
			Debugger(const T value) {
				std::cout << " value " << static_cast<int>(value) << std::endl;
			}
		};

		/*
			Templated Namespace that contains each class used.
		*/
		template < typename Resu, 
			typename Fi, typename Se, typename Th, typename Fo, typename Fifth, typename Sixth, typename Seventh, typename Eighth, typename Ninth, typename Tenth, typename Eleventh, typename Twelveth >
		struct Tree13 {
			typedef Resu Result; 

			struct Answer {
				Answer() : valid(false), answer() {}
				Answer(const Result& value) : valid(true), answer(value) {}

				operator bool() { return valid; }
				operator Result&() { return answer; }
				operator const Result&() { return answer; }
				const Result& operator*() const { return answer; }
				bool valid;
				Result answer;
			};

			class ATree {
			public:
				virtual ~ATree() {}
				virtual Answer evaluate(const Fi&, const Se&, const Th&, const Fo&, const Fifth&, const Sixth&, const Seventh&, const Eighth&, const Ninth&, const Tenth&, const Eleventh&, const Twelveth&) = 0;
			};

			class Leaf : public ATree {
			private:
				Resu value_;

			public:
				Leaf(Resu value) : value_(value) {}

				Answer evaluate(const Fi&, const Se&, const Th&, const Fo&, const Fifth&, const Sixth&, const Seventh&, const Eighth&, const Ninth&, const Tenth&, const Eleventh&, const Twelveth&) {
					return value_;
				}
			};

			template< typename Specialized >
			class TreeNode : public ATree
			{
			public:
				typedef typename boost::shared_ptr< ATree > AutoTreePtr;
				typedef typename std::map< Specialized, AutoTreePtr > ChildMap;

				Answer evaluate(const Fi& val1, const Se& val2, const Th& val3, const Fo& val4, const Fifth& val5, const Sixth& val6, const Seventh& val7, const Eighth& val8, const Ninth& val9, const Tenth& val10, const Eleventh& val11, const Twelveth& val12)  
				{
					AutoTreePtr ptr;

					test(val1, ptr);
					test(val2, ptr);
					test(val3, ptr);
					test(val4, ptr);
					test(val5, ptr);
					test(val6, ptr);
					test(val7, ptr);
					test(val8, ptr);
					test(val9, ptr);
					test(val10, ptr);
					test(val11, ptr);
					test(val12, ptr);

					if (ptr) {
						return ptr->evaluate(val1, val2, val3, val4, val5, val6, val7, val8, val9, val10, val11, val12);
					} else {
						return Answer();
					}
				}

				void test(const Specialized& value, AutoTreePtr& ptr) {
					typename ChildMap::const_iterator it = child_.find(value);
			
					if (it != child_.end()) {
						ptr = it->second;
					}
				}

				template <typename T>
				void test(const T& value, AutoTreePtr& ptr) {}

				void AddSubNode(const Specialized& value, ATree* sub) 
				{
					child_[value] = AutoTreePtr(sub);
				}

			private:
				ChildMap child_; 
			};
		};

		template < typename Resu, typename Fi, typename Se, typename Th, typename Fo, typename Fifth, typename Sixth, typename Seventh, typename Eighth, typename Ninth, typename Tenth, typename Eleventh >
		struct Tree12 {
			typedef Resu Result; 

			struct Answer {
				Answer() : valid(false), answer() {}
				Answer(const Result& value) : valid(true), answer(value) {}

				operator bool() { return valid; }
				operator Result&() { return answer; }
				operator const Result&() { return answer; }
				const Result& operator*() const { return answer; }
				bool valid;
				Result answer;
			};

			class ATree {
			public:
				virtual ~ATree() {}
				virtual Answer evaluate(const Fi&, const Se&, const Th&, const Fo&, const Fifth&, const Sixth&, const Seventh&, const Eighth&, const Ninth&, const Tenth&, const Eleventh&) = 0;
			};

			class Leaf : public ATree {
			private:
				Resu value_;

			public:
				Leaf(Resu value) : value_(value) {}

				Answer evaluate(const Fi&, const Se&, const Th&, const Fo&, const Fifth&, const Sixth&, const Seventh&, const Eighth&, const Ninth&, const Tenth&, const Eleventh&) {
					return value_;
				}
			};

			template< typename Specialized >
			class TreeNode : public ATree
			{
			public:
				typedef typename boost::shared_ptr< ATree > AutoTreePtr;
				typedef typename std::map< Specialized, AutoTreePtr > ChildMap;

				Answer evaluate(const Fi& val1, const Se& val2, const Th& val3, const Fo& val4, const Fifth& val5, const Sixth& val6, const Seventh& val7, const Eighth& val8, const Ninth& val9, const Tenth& val10, const Eleventh& val11)  
				{
					AutoTreePtr ptr;

					test(val1, ptr);
					test(val2, ptr);
					test(val3, ptr);
					test(val4, ptr);
					test(val5, ptr);
					test(val6, ptr);
					test(val7, ptr);
					test(val8, ptr);
					test(val9, ptr);
					test(val10, ptr);
					test(val11, ptr);

					if (ptr) {
						return ptr->evaluate(val1, val2, val3, val4, val5, val6, val7, val8, val9, val10, val11);
					} else {
						return Answer();
					}
				}

				void test(const Specialized& value, AutoTreePtr& ptr) {
					typename ChildMap::const_iterator it = child_.find(value);
			
					if (it != child_.end()) {
						ptr = it->second;
					}
				}

				template <typename T>
				void test(const T& value, AutoTreePtr& ptr) {}

				void AddSubNode(const Specialized& value, ATree* sub) 
				{
					child_[value] = AutoTreePtr(sub);
				}

			private:
				ChildMap child_; 
			};
		};

		template < typename Resu, typename Fi, typename Se, typename Th, typename Fo, typename Fifth, typename Sixth, typename Seventh, typename Eighth, typename Ninth, typename Tenth >
		struct Tree11 {
			typedef Resu Result; 

			struct Answer {
				Answer() : valid(false), answer() {}
				Answer(const Result& value) : valid(true), answer(value) {}

				operator bool() { return valid; }
				operator Result&() { return answer; }
				operator const Result&() { return answer; }
				const Result& operator*() const { return answer; }
				bool valid;
				Result answer;
			};

			class ATree {
			public:
				virtual ~ATree() {}
				virtual Answer evaluate(const Fi&, const Se&, const Th&, const Fo&, const Fifth&, const Sixth&, const Seventh&, const Eighth&, const Ninth&, const Tenth&) = 0;
			};

			class Leaf : public ATree {
			private:
				Resu value_;

			public:
				Leaf(Resu value) : value_(value) {}

				Answer evaluate(const Fi&, const Se&, const Th&, const Fo&, const Fifth&, const Sixth&, const Seventh&, const Eighth&, const Ninth&, const Tenth&) {
					return value_;
				}
			};

			template< typename Specialized >
			class TreeNode : public ATree
			{
			public:
				typedef typename boost::shared_ptr< ATree > AutoTreePtr;
				typedef typename std::map< Specialized, AutoTreePtr > ChildMap;

				Answer evaluate(const Fi& val1, const Se& val2, const Th& val3, const Fo& val4, const Fifth& val5, const Sixth& val6, const Seventh& val7, const Eighth& val8, const Ninth& val9, const Tenth& val10)  
				{
					AutoTreePtr ptr;

					test(val1, ptr);
					test(val2, ptr);
					test(val3, ptr);
					test(val4, ptr);
					test(val5, ptr);
					test(val6, ptr);
					test(val7, ptr);
					test(val8, ptr);
					test(val9, ptr);
					test(val10, ptr);

					if (ptr) {
						return ptr->evaluate(val1, val2, val3, val4, val5, val6, val7, val8, val9, val10);
					} else {
						return Answer();
					}
				}

				void test(const Specialized& value, AutoTreePtr& ptr) {
					typename ChildMap::const_iterator it = child_.find(value);
			
					if (it != child_.end()) {
						ptr = it->second;
					}
				}

				template <typename T>
				void test(const T& value, AutoTreePtr& ptr) {}

				void AddSubNode(const Specialized& value, ATree* sub) 
				{
					child_[value] = AutoTreePtr(sub);
				}

			private:
				ChildMap child_; 
			};
		};

		template < typename Resu, typename Fi, typename Se, typename Th, typename Fo, typename Fifth, typename Sixth, typename Seventh, typename Eighth, typename Ninth >
		struct Tree10 {
			typedef Resu Result; 

			struct Answer {
				Answer() : valid(false), answer() {}
				Answer(const Result& value) : valid(true), answer(value) {}

				operator bool() { return valid; }
				operator Result&() { return answer; }
				operator const Result&() { return answer; }
				const Result& operator*() const { return answer; }
				bool valid;
				Result answer;
			};

			class ATree {
			public:
				virtual ~ATree() {}
				virtual Answer evaluate(const Fi&, const Se&, const Th&, const Fo&, const Fifth&, const Sixth&, const Seventh&, const Eighth&, const Ninth&) = 0;
			};

			class Leaf : public ATree {
			private:
				Resu value_;

			public:
				Leaf(Resu value) : value_(value) {}

				Answer evaluate(const Fi&, const Se&, const Th&, const Fo&, const Fifth&, const Sixth&, const Seventh&, const Eighth&, const Ninth&) {
					return value_;
				}
			};

			template< typename Specialized >
			class TreeNode : public ATree
			{
			public:
				typedef typename boost::shared_ptr< ATree > AutoTreePtr;
				typedef typename std::map< Specialized, AutoTreePtr > ChildMap;

				Answer evaluate(const Fi& val1, const Se& val2, const Th& val3, const Fo& val4, const Fifth& val5, const Sixth& val6, const Seventh& val7, const Eighth& val8, const Ninth& val9)  
				{
					AutoTreePtr ptr;

					test(val1, ptr);
					test(val2, ptr);
					test(val3, ptr);
					test(val4, ptr);
					test(val5, ptr);
					test(val6, ptr);
					test(val7, ptr);
					test(val8, ptr);
					test(val9, ptr);

					if (ptr) {
						return ptr->evaluate(val1, val2, val3, val4, val5, val6, val7, val8, val9);
					} else {
						return Answer();
					}
				}

				void test(const Specialized& value, AutoTreePtr& ptr) {
					typename ChildMap::const_iterator it = child_.find(value);
			
					if (it != child_.end()) {
						ptr = it->second;
					}
				}

				template <typename T>
				void test(const T& value, AutoTreePtr& ptr) {}

				void AddSubNode(const Specialized& value, ATree* sub) 
				{
					child_[value] = AutoTreePtr(sub);
				}

			private:
				ChildMap child_; 
			};
		};

		template < typename Resu, typename Fi, typename Se, typename Th, typename Fo, typename Fifth, typename Sixth, typename Seventh, typename Eighth >
		struct Tree9 {
			typedef Resu Result; 

			struct Answer {
				Answer() : valid(false), answer() {}
				Answer(const Result& value) : valid(true), answer(value) {}

				operator bool() { return valid; }
				operator Result&() { return answer; }
				operator const Result&() { return answer; }
				const Result& operator*() const { return answer; }
				bool valid;
				Result answer;
			};

			class ATree {
			public:
				virtual ~ATree() {}
				virtual Answer evaluate(const Fi& val1, const Se& val2, const Th& val3, const Fo& val4, const Fifth& val5, const Sixth& val6, const Seventh& val7, const Eighth& val8) = 0;
			};

			class Leaf : public ATree {
			private:
				Resu value_;

			public:
				Leaf(Resu value) : value_(value) {}

				Answer evaluate(const Fi&, const Se&, const Th&, const Fo&, const Fifth&, const Sixth&, const Seventh&, const Eighth&) {
					return value_;
				}
			};

			template< typename Specialized >
			class TreeNode : public ATree
			{
			public:
				typedef typename boost::shared_ptr< ATree > AutoTreePtr;
				typedef typename std::map< Specialized, AutoTreePtr > ChildMap;

				Answer evaluate(const Fi& val1, const Se& val2, const Th& val3, const Fo& val4, const Fifth& val5, const Sixth& val6, const Seventh& val7, const Eighth& val8)  
				{
					AutoTreePtr ptr;

					test(val1, ptr);
					test(val2, ptr);
					test(val3, ptr);
					test(val4, ptr);
					test(val5, ptr);
					test(val6, ptr);
					test(val7, ptr);
					test(val8, ptr);

					if (ptr) {
						return ptr->evaluate(val1, val2, val3, val4, val5, val6, val7, val8);
					} else {
						return Answer();
					}
				}

				void test(const Specialized& value, AutoTreePtr& ptr) {
					typename ChildMap::const_iterator it = child_.find(value);
			
					if (it != child_.end()) {
						ptr = it->second;
					}
				}

				template <typename T>
				void test(const T& value, AutoTreePtr& ptr) {}

				void AddSubNode(const Specialized& value, ATree* sub) 
				{
					child_[value] = AutoTreePtr(sub);
				}

			private:
				ChildMap child_; 
			};
		};

		template < typename Resu, typename Fi, typename Se, typename Th, typename Fo, typename Fifth, typename Sixth, typename Seventh >
		struct Tree8 {
			typedef Resu Result; 

			struct Answer {
				Answer() : valid(false), answer() {}
				Answer(const Result& value) : valid(true), answer(value) {}

				operator bool() { return valid; }
				operator Result&() { return answer; }
				operator const Result&() { return answer; }
				const Result& operator*() const { return answer; }
				bool valid;
				Result answer;
			};

			class ATree {
			public:
				virtual ~ATree() {}
				virtual Answer evaluate(const Fi& val1, const Se& val2, const Th& val3, const Fo& val4, const Fifth& val5, const Sixth& val6, const Seventh& val7) = 0;
			};

			class Leaf : public ATree {
			private:
				Resu value_;

			public:
				Leaf(Resu value) : value_(value) {}

				Answer evaluate(const Fi&, const Se&, const Th&, const Fo&, const Fifth&, const Sixth&, const Seventh&) {
					return value_;
				}
			};

			template< typename Specialized >
			class TreeNode : public ATree
			{
			public:
				typedef typename boost::shared_ptr< ATree > AutoTreePtr;
				typedef typename std::map< Specialized, AutoTreePtr > ChildMap;

				Answer evaluate(const Fi& val1, const Se& val2, const Th& val3, const Fo& val4, const Fifth& val5, const Sixth& val6, const Seventh& val7)  
				{
					AutoTreePtr ptr;

					test(val1, ptr);
					test(val2, ptr);
					test(val3, ptr);
					test(val4, ptr);
					test(val5, ptr);
					test(val6, ptr);
					test(val7, ptr);

					if (ptr) {
						return ptr->evaluate(val1, val2, val3, val4, val5, val6, val7);
					} else {
						return Answer();
					}
				}

				void test(const Specialized& value, AutoTreePtr& ptr) {
					typename ChildMap::const_iterator it = child_.find(value);
			
					if (it != child_.end()) {
						ptr = it->second;
					}
				}

				template <typename T>
				void test(const T& value, AutoTreePtr& ptr) {}

				void AddSubNode(const Specialized& value, ATree* sub) 
				{
					child_[value] = AutoTreePtr(sub);
				}

			private:
				ChildMap child_; 
			};
		};

		template < typename Resu, typename Fi, typename Se, typename Th, typename Fo, typename Fifth, typename Sixth >
		struct Tree7 {
			typedef Resu Result; 

			struct Answer {
				Answer() : valid(false), answer() {}
				Answer(const Result& value) : valid(true), answer(value) {}

				operator bool() { return valid; }
				operator Result&() { return answer; }
				operator const Result&() { return answer; }
				const Result& operator*() const { return answer; }
				bool valid;
				Result answer;
			};

			class ATree {
			public:
				virtual ~ATree() {}
				virtual Answer evaluate(const Fi& val1, const Se& val2, const Th& val3, const Fo& val4, const Fifth& val5, const Sixth& val6) = 0;
			};

			class Leaf : public ATree {
			private:
				Resu value_;

			public:
				Leaf(Resu value) : value_(value) {}

				Answer evaluate(const Fi&, const Se&, const Th&, const Fo&, const Fifth&, const Sixth&) {
					return value_;
				}
			};

			template< typename Specialized >
			class TreeNode : public ATree
			{
			public:
				typedef typename boost::shared_ptr< ATree > AutoTreePtr;
				typedef typename std::map< Specialized, AutoTreePtr > ChildMap;

				Answer evaluate(const Fi& val1, const Se& val2, const Th& val3, const Fo& val4, const Fifth& val5, const Sixth& val6)  
				{
					AutoTreePtr ptr;

					test(val1, ptr);
					test(val2, ptr);
					test(val3, ptr);
					test(val4, ptr);
					test(val5, ptr);
					test(val6, ptr);

					if (ptr) {
						return ptr->evaluate( val1, val2, val3, val4, val5, val6);
					} else {
						return Answer();
					}
				}

				void test(const Specialized& value, AutoTreePtr& ptr) {
					typename ChildMap::const_iterator it = child_.find(value);
			
					if (it != child_.end()) {
						ptr = it->second;
					}
				}

				template <typename T>
				void test(const T& value, AutoTreePtr& ptr) {}

				void AddSubNode(const Specialized& value, ATree* sub) 
				{
					child_[value] = AutoTreePtr(sub);
				}

			private:
				ChildMap child_; 
			};
		};

		template < typename Resu, typename Fi, typename Se, typename Th, typename Fo, typename Fifth >
		struct Tree6 {
			typedef Resu Result;

			struct Answer {
				Answer() : valid(false), answer() {}
				Answer(const Result& value) : valid(true), answer(value) {}

				operator bool() { return valid; }
				operator Result&() { return answer; }
				operator const Result&() { return answer; }
				const Result& operator*() const { return answer; }
				bool valid;
				Result answer;
			};

			class ATree {
			public:
				virtual ~ATree() {}
				virtual Answer evaluate(const Fi& val1, const Se& val2, const Th& val3, const Fo& val4, const Fifth& val5)  = 0;
			};

			class Leaf : public ATree {
			private:
				Resu value_;

			public:
				Leaf(Resu value) : value_(value) {}

				Answer evaluate(const Fi& , const Se& , const Th& , const Fo& , const Fifth& )  {
					return value_;
				}
			};

			template< typename Specialized >
			class TreeNode : public ATree
			{
			public:
				typedef boost::shared_ptr< ATree > AutoTreePtr;
				typedef std::map< Specialized, AutoTreePtr > ChildMap;

				Answer evaluate(const Fi& val1, const Se& val2, const Th& val3, const Fo& val4, const Fifth& val5)  
				{
					AutoTreePtr ptr;

					test(val1, ptr);
					test(val2, ptr);
					test(val3, ptr);
					test(val4, ptr);
					test(val5, ptr);

					if (ptr) {
						return ptr->evaluate(val1, val2, val3, val4, val5);
					} else {
						return Answer();
					}
				}

				void test(const Specialized& value, AutoTreePtr& ptr) {
					typename ChildMap::const_iterator it = child_.find(value);
			
					if (it != child_.end()) {
						ptr = it->second;
					}
				}

				template <typename T>
				void test(const T& value, AutoTreePtr& ptr) {}


				void AddSubNode(const Specialized& value, ATree* sub) 
				{
					child_[value] = AutoTreePtr(sub);
				}

			private:
				ChildMap child_; 
			};
		};

		template < typename Resu, typename Fi, typename Se, typename Th, typename Fo>
		struct Tree5 { 
			typedef Resu Result;

			struct Answer {
				Answer() : valid(false), answer() {}
				Answer(const Result& value) : valid(true), answer(value) {}

				operator bool() { return valid; }
				operator Result&() { return answer; }
				operator const Result&() { return answer; }
				const Result& operator*() const { return answer; }
				bool valid;
				Result answer;
			};

			class ATree {
			public:
				virtual ~ATree() {}
				virtual Answer evaluate(const Fi& val1, const Se& val2, const Th& val3, const Fo& val4) = 0;
			};

			class Leaf : public ATree {
			private:
				Resu value_;

			public:
				Leaf(Resu value) : value_(value) {}

				Answer evaluate(const Fi& , const Se& , const Th& , const Fo& ) {
					return value_;
				}
			};

			template< typename Specialized >
			class TreeNode : public ATree
			{
			public:
				typedef boost::shared_ptr< ATree > AutoTreePtr;
				typedef std::map< Specialized, AutoTreePtr > ChildMap;

				Answer evaluate(const Fi& val1, const Se& val2, const Th& val3, const Fo& val4) {
					AutoTreePtr ptr;

					test(val1, ptr);
					test(val2, ptr);
					test(val3, ptr);
					test(val4, ptr);

					if (ptr) {
						return ptr->evaluate(val1, val2, val3, val4);
					} else {
						return Answer();
					}
				}

				void test(const Specialized& value, AutoTreePtr& ptr) {
					typename ChildMap::const_iterator it = child_.find(value);
			
					if (it != child_.end()) {
						ptr = it->second;
					}
				}

				template <typename T>
				void test(const T& value, AutoTreePtr& ptr) {}

				void AddSubNode(const Specialized& value, ATree* sub) 
				{									
					child_[value] = AutoTreePtr(sub);
				}

			private:
				ChildMap child_; 
			};
		};

		template < typename Resu, typename Fi, typename Se, typename Th>
		struct Tree4 {
			typedef Resu Result;

			struct Answer {
				Answer() : valid(false), answer() {}
				Answer(const Result& value) : valid(true), answer(value) {}

				operator bool() { return valid; }
				operator Result&() { return answer; }
				operator const Result&() { return answer; }
				const Result& operator*() const { return answer; }
				bool valid;
				Result answer;
			};

			class ATree {
			public:
				virtual ~ATree() {}
				virtual Answer evaluate(const Fi& val1, const Se& val2, const Th& val3) = 0;
			};

			class Leaf : public ATree {
			private:
				Resu value_;

			public:
				Leaf(Resu value) : value_(value) {}

				Answer evaluate(const Fi& , const Se& , const Th& ) {
					return value_;
				}
			};

			template< typename Specialized >
			class TreeNode : public ATree
			{
			public:
				typedef boost::shared_ptr< ATree > AutoTreePtr;
				typedef std::map< Specialized, AutoTreePtr > ChildMap;

				Answer evaluate(const Fi& val1, const Se& val2, const Th& val3) {
					AutoTreePtr ptr;

					test(val1, ptr);
					test(val2, ptr);
					test(val3, ptr);

					if (ptr) {
						return ptr->evaluate(val1, val2, val3);
					} else {
						return Answer();
					}
				}

				void test(const Specialized& value, AutoTreePtr& ptr) {
					typename ChildMap::const_iterator it = child_.find(value);
			
					if (it != child_.end()) {
						ptr = it->second;
					}
				}

				template <typename T>
				void test(const T& value, AutoTreePtr& ptr) {}
	
				void AddSubNode(const Specialized& value, ATree* sub) {
					child_[value] = AutoTreePtr(sub);
				}

			private:
				ChildMap child_; 
			};
		};

		template < typename Resu, typename Fi, typename Se>
		struct Tree3 {
			typedef Resu Result;

			struct Answer {
				Answer() : valid(false), answer() {}
				Answer(const Result& value) : valid(true), answer(value) {}

				operator bool() { return valid; }
				operator Result&() { return answer; }
				operator const Result&() { return answer; }
				const Result& operator*() const { return answer; }
				bool valid;
				Result answer;
			};

			class ATree {
			public:
				virtual ~ATree() {}
				virtual Answer evaluate(const Fi& val1, const Se& val2) = 0;
			};

			class Leaf : public ATree {
			private:
				Resu value_;

			public:
				Leaf(Resu value) : value_(value) {}

				Answer evaluate(const Fi& , const Se& ) {
					return value_;
				}
			};

			template< typename Specialized >
			class TreeNode : public ATree
			{
			public:
				typedef boost::shared_ptr< ATree > AutoTreePtr;
				typedef std::map< Specialized, AutoTreePtr > ChildMap;

				Answer evaluate(const Fi& val1, const Se& val2) {
					AutoTreePtr ptr;

					test(val1, ptr);
					test(val2, ptr);

					if (ptr) {
						return ptr->evaluate(val1, val2);
					} else {
						return Answer();
					}
				}

				void test(const Specialized& value, AutoTreePtr& ptr) {
					typename ChildMap::const_iterator it = child_.find(value);
			
					if (it != child_.end()) {
						ptr = it->second;
					}
				}

				template <typename T>
				void test(const T& value, AutoTreePtr& ptr) {}
	 
				void AddSubNode(const Specialized& value, ATree* sub) {
					child_[value] = AutoTreePtr(sub);
				}

			private:
				ChildMap child_; 
			};
		};

		template < typename Resu, typename Fi>
		struct Tree2 {
			typedef Resu Result;

			struct Answer {
				Answer() : valid(false), answer() {}
				Answer(const Result& value) : valid(true), answer(value) {}

				operator bool() { return valid; }
				operator Result&() { return answer; }
				operator const Result&() { return answer; }
				const Result& operator*() const { return answer; }
				bool valid;
				Result answer;
			};

			class ATree {
			public:
				virtual ~ATree() {}
				virtual Answer evaluate(const Fi& val1) = 0;
			};

			class Leaf : public ATree {
			private:
				Resu value_;

			public:
				Leaf(Resu value) : value_(value) {}

				Answer evaluate(const Fi& ) {
					return value_;
				}
			};

			template< typename Specialized >
			class TreeNode : public ATree
			{
			public:
				typedef std::auto_ptr< ATree > AutoTreePtr;
				typedef std::map< Specialized, AutoTreePtr > ChildMap;

				Answer evaluate(const Fi& val1) {
					AutoTreePtr ptr;

					test(val1, ptr);

					if (ptr) {
						return ptr->evaluate(value, val1, val2, val3, val4, val5);
					} else {
						return Answer();
					}
				}

				void test(const Specialized& value, AutoTreePtr& ptr) {
					typename ChildMap::const_iterator it = child_.find(value);
			
					if (it != child_.end()) {
						ptr = it->second;
					}
				}

				template <typename T>
				void test(const T& value, AutoTreePtr& ptr) {}

				void AddSubNode(const Specialized& value, ATree* sub)  {
					child_[value] = AutoTreePtr(sub);
				}

			private:
				ChildMap child_; 
			};
		};

	}

}

#endif /* !TYPEDTREE_HPP */