
#ifndef TYPEDTREE_HPP
#define TYPEDTREE_HPP

#include <memory>

namespace TypedTree {

	template < typename Result, typename First, typename Second, typename Third, typename Fourth, typename Fifth, typename Sixth >
	struct Tree7 {
		typedef Result Result;

		class ATree {
		public:
			virtual ~ATree() {}
			virtual Result evaluate(const First& val1, const Second& val2, const Third& val3, const Fourth& val4, const Fifth& val5, const Sixth& val6) const = 0;
		};

		class Leaf : public ATree {
		private:
			Result value_;

		public:
			Leaf(Result value) : value_(value) {}

			Result evaluate(const First&, const Second&, const Third&, const Fourth&, const Fifth&, const Sixth&) const {
				return value_;
			}
		};

		template< typename Specialized >
		class TreeNode : public ATree
		{
		public:
			typedef std::auto_ptr< const ATree > AutoTreePtr;
			typedef std::map< Specialized, AutoTreePtr > ChildMap;

			Result evaluate(const First& val1, const Second& val2, const Third& val3, const Fourth& val4, const Fifth& val5, const Sixth& val6) const 
			{
				return test(val1, val2, val3, val4, val5, val6);
			}

			template <typename N, typename B, typename V, typename C, typename X>
			inline Result test(const Specialized& value, const N& val1, const B& val2, const V& val3, const Fifth& val4, const Sixth& val5) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(value, val1, val2, val3, val4, val5);
				}
				else 
					return Result();
			}

			template <typename N, typename B, typename V, typename C, typename X>
			inline Result test(const N& val1, const Specialized& value, const B& val2, const V& val3, const C& val4, const X& val5) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(val1, value, val2, val3, val4, val5);
				}
				else 
					return Result();
			}
	 
			template <typename N, typename B, typename V, typename C, typename X>
			inline Result test(const N& val1, const B& val2, const Specialized& value, const V& val3, const C& val4, const X& val5) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(val1, val2, value, val3, val4, val5);
				}
				else 
					return Result();
			}
	
			template <typename N, typename B, typename V, typename C, typename X>
			inline Result test(const N& val1, const V& val2, const B& val3, const Specialized& value, const C& val4, const X& val5) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(val1, val2, val3, value, val4, val5);
				}
				else 
					return Result();
			}

			template <typename N, typename B, typename V, typename C, typename X>
			inline Result test(const N& val1, const V& val2, const B& val3, const C& val4, const Specialized& value, const X& val5) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(val1, val2, val3, val4, value, val5);
				}
				else 
					return Result();
			}

			template <typename N, typename B, typename V, typename C, typename X>
			inline Result test(const N& val1, const V& val2, const B& val3, const C& val4, const X& val5, const Specialized& value) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(val1, val2, val3, val4, val5, value);
				}
				else 
					return Result();
			}

			void AddSubNode(const Specialized& value, const ATree* sub) 
			{
				child_[value] = AutoTreePtr(sub);
			}

		private:
			ChildMap child_; 
		};
	};

	template < typename Result, typename First, typename Second, typename Third, typename Fourth, typename Fifth >
	struct Tree6 {
		typedef Result Result;

		class ATree {
		public:
			virtual ~ATree() {}
			virtual Result evaluate(const First& val1, const Second& val2, const Third& val3, const Fourth& val4, const Fifth& val5) const = 0;
		};

		class Leaf : public ATree {
		private:
			Result value_;

		public:
			Leaf(Result value) : value_(value) {}

			Result evaluate(const First& val1, const Second& val2, const Third& val3, const Fourth& val4, const Fifth& val5) const {
				return value_;
			}
		};

		template< typename Specialized >
		class TreeNode : public ATree
		{
		public:
			typedef std::auto_ptr< const ATree > AutoTreePtr;
			typedef std::map< Specialized, AutoTreePtr > ChildMap;

			Result evaluate(const First& val1, const Second& val2, const Third& val3, const Fourth& val4, const Fifth& val5) const 
			{
				return test(val1, val2, val3, val4, val5);
			}

			template <typename N, typename B, typename V, typename C>
			inline Result test(const Specialized& value, const N& val1, const B& val2, const V& val3, const C& val4) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(value, val1, val2, val3, val4);
				}
				else 
					return Result();
			}

			template <typename N, typename B, typename V, typename C>
			inline Result test(const N& val1, const Specialized& value, const B& val2, const V& val3, const C& val4) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(val1, value, val2, val3, val4);
				}
				else 
					return Result();
			}
	 
			template <typename N, typename B, typename V, typename C>
			inline Result test(const N& val1, const B& val2, const Specialized& value, const V& val3, const C& val4) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(val1, val2, value, val3, val4);
				}
				else 
					return Result();
			}
	
			template <typename N, typename B, typename V, typename C>
			inline Result test(const N& val1, const V& val2, const B& val3, const Specialized& value, const C& val4) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(val1, val2, val3, value, val4);
				}
				else 
					return Result();
			}

			template <typename N, typename B, typename V, typename C>
			inline Result test(const N& val1, const V& val2, const B& val3, const C& val4, const Specialized& value) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(val1, val2, val3, val4, value);
				}
				else 
					return Result();
			}

			void AddSubNode(const Specialized& value, const ATree* sub) 
			{
				child_[value] = AutoTreePtr(sub);
			}

		private:
			ChildMap child_; 
		};
	};

	template < typename Result, typename First, typename Second, typename Third, typename Fourth>
	struct Tree5 {
		typedef Result Result;

		class ATree {
		public:
			virtual ~ATree() {}
			virtual Result evaluate(const First& val1, const Second& val2, const Third& val3, const Fourth& val4) const = 0;
		};

		class Leaf : public ATree {
		private:
			Result value_;

		public:
			Leaf(Result value) : value_(value) {}

			Result evaluate(const First& val1, const Second& val2, const Third& val3, const Fourth& val4) const {
				return value_;
			}
		};

		template< typename Specialized >
		class TreeNode : public ATree
		{
		public:
			typedef std::auto_ptr< const ATree > AutoTreePtr;
			typedef std::map< Specialized, AutoTreePtr > ChildMap;

			Result evaluate(const First& val1, const Second& val2, const Third& val3, const Fourth& val4) const {
				return test(val1, val2, val3, val4);
			}

			template <typename N, typename B, typename V>
			inline Result test(const Specialized& value, const N& val1, const B& val2, const V& val3) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(value, val1, val2, val3);
				}
				else 
					return Result();
			}

			template <typename N, typename B, typename V>
			inline Result test(const N& val1, const Specialized& value, const B& val2, const V& val3) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(val1, value, val2, val3);
				}
				else 
					return Result();
			}
	 
			template <typename N, typename B, typename V>
			inline Result test(const N& val1, const B& val2, const Specialized& value, const V& val3) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(val1, val2, value, val3);
				}
				else 
					return Result();
			}
	
			template <typename N, typename B, typename V>
			inline Result test(const N& val1, const V& val2, const B& val3, const Specialized& value) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(val1, val2, val3, value);
				}
				else 
					return Result();
			}

			void AddSubNode(const Specialized& value, const ATree* sub) 
			{
				child_[value] = AutoTreePtr(sub);
			}

		private:
			ChildMap child_; 
		};
	};

	template < typename Result, typename First, typename Second, typename Third>
	struct Tree4 {
		typedef Result Result;

		class ATree {
		public:
			virtual ~ATree() {}
			virtual Result evaluate(const First& val1, const Second& val2, const Third& val3) const = 0;
		};

		class Leaf : public ATree {
		private:
			Result value_;

		public:
			Leaf(Result value) : value_(value) {}

			Result evaluate(const First& val1, const Second& val2, const Third& val3) const {
				return value_;
			}
		};

		template< typename Specialized >
		class TreeNode : public ATree
		{
		public:
			typedef std::auto_ptr< const ATree > AutoTreePtr;
			typedef std::map< Specialized, AutoTreePtr > ChildMap;

			Result evaluate(const First& val1, const Second& val2, const Third& val3) const {
				return test(val1, val2, val3);
			}

			template <typename N, typename B>
			inline Result test(const Specialized& value, const N& val1, const B& val2) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(value, val1, val2);
				}
				else 
					return Result();
			}

			template <typename N, typename B>
			inline Result test(const N& val1, const Specialized& value, const B& val2) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(val1, value, val2);
				}
				else 
					return Result();
			}
	 
			template <typename N, typename B>
			inline Result test(const N& val1, const B& val2, const Specialized& value) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(val1, val2, value);
				}
				else 
					return Result();
			}
	
			void AddSubNode(const Specialized& value, const ATree* sub) {
				child_[value] = AutoTreePtr(sub);
			}

		private:
			ChildMap child_; 
		};
	};

	template < typename Result, typename First, typename Second>
	struct Tree3 {
		typedef Result Result;

		class ATree {
		public:
			virtual ~ATree() {}
			virtual Result evaluate(const First& val1, const Second& val2) const = 0;
		};

		class Leaf : public ATree {
		private:
			Result value_;

		public:
			Leaf(Result value) : value_(value) {}

			Result evaluate(const First& val1, const Second& val2) const {
				return value_;
			}
		};

		template< typename Specialized >
		class TreeNode : public ATree
		{
		public:
			typedef std::auto_ptr< const ATree > AutoTreePtr;
			typedef std::map< Specialized, AutoTreePtr > ChildMap;

			Result evaluate(const First& val1, const Second& val2) const {
				return test(val1, val2);
			}

			template <typename N>
			inline Result test(const Specialized& value, const N& val1) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(value, val1);
				}
				else 
					return Result();
			}

			template <typename N>
			inline Result test(const N& val1, const Specialized& value) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(val1, value);
				}
				else 
					return Result();
			}
	 
			void AddSubNode(const Specialized& value, const ATree* sub) {
				child_[value] = AutoTreePtr(sub);
			}

		private:
			ChildMap child_; 
		};
	};

	template < typename Result, typename First>
	struct Tree2 {
		typedef Result Result;

		class ATree {
		public:
			virtual ~ATree() {}
			virtual Result evaluate(const First& val1) const = 0;
		};

		class Leaf : public ATree {
		private:
			Result value_;

		public:
			Leaf(Result value) : value_(value) {}

			Result evaluate(const First& val1) const {
				return value_;
			}
		};

		template< typename Specialized >
		class TreeNode : public ATree
		{
		public:
			typedef std::auto_ptr< const ATree > AutoTreePtr;
			typedef std::map< Specialized, AutoTreePtr > ChildMap;

			Result evaluate(const First& val1) {
				return test(val1);
			}

			inline Result test(const Specialized& value) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(value, val1);
				}
				else 
					return Result();
			}

			void AddSubNode(const Specialized& value, const ATree* sub)  {
				child_[value] = AutoTreePtr(sub);
			}

		private:
			ChildMap child_; 
		};
	};

}

#endif /* !TYPEDTREE_HPP */