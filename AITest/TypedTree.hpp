
#ifndef TYPEDTREE_HPP
#define TYPEDTREE_HPP

#include <memory>
#include <iostream>

namespace TypedTree { // namespace des structure qui compose les arbre de decision

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

	template < typename Resu, typename Fi, typename Se, typename Th, typename Fo, typename Fifth, typename Sixth >
	struct Tree7 { //namespace template des structure pour l'arbre a 6 attribue
		typedef Resu Result; // typedef pour retrouver la sortie, utile a la construction.

		class ATree {
		public:
			virtual ~ATree() {}
			virtual Resu evaluate(const Fi& val1, const Se& val2, const Th& val3, const Fo& val4, const Fifth& val5, const Sixth& val6) const = 0;
		};

		class Leaf : public ATree {
		private:
			Resu value_;

		public:
			Leaf(Resu value) : value_(value) {}

			Resu evaluate(const Fi&, const Se&, const Th&, const Fo&, const Fifth&, const Sixth&) const {
				return value_;
			}
		};

		template< typename Specialized >
		class TreeNode : public ATree
		{
		public:
			typedef std::auto_ptr< const ATree > AutoTreePtr;
			typedef std::map< Specialized, AutoTreePtr > ChildMap;

			Resu evaluate(const Fi& val1, const Se& val2, const Th& val3, const Fo& val4, const Fifth& val5, const Sixth& val6) const 
			{
				return test(val1, val2, val3, val4, val5, val6);
			}

			template <typename N, typename B, typename V, typename C, typename X>
			inline Resu test(const Specialized& value, const N& val1, const B& val2, const V& val3, const Fifth& val4, const Sixth& val5) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(value, val1, val2, val3, val4, val5);
				}
				else 
					return Resu();
			}

			template <typename N, typename B, typename V, typename C, typename X>
			inline Resu test(const N& val1, const Specialized& value, const B& val2, const V& val3, const C& val4, const X& val5) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(val1, value, val2, val3, val4, val5);
				}
				else 
					return Resu();
			}
	 
			template <typename N, typename B, typename V, typename C, typename X>
			inline Resu test(const N& val1, const B& val2, const Specialized& value, const V& val3, const C& val4, const X& val5) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(val1, val2, value, val3, val4, val5);
				}
				else 
					return Resu();
			}
	
			template <typename N, typename B, typename V, typename C, typename X>
			inline Resu test(const N& val1, const V& val2, const B& val3, const Specialized& value, const C& val4, const X& val5) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(val1, val2, val3, value, val4, val5);
				}
				else 
					return Resu();
			}

			template <typename N, typename B, typename V, typename C, typename X>
			inline Resu test(const N& val1, const V& val2, const B& val3, const C& val4, const Specialized& value, const X& val5) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(val1, val2, val3, val4, value, val5);
				}
				else 
					return Resu();
			}

			template <typename N, typename B, typename V, typename C, typename X>
			inline Resu test(const N& val1, const V& val2, const B& val3, const C& val4, const X& val5, const Specialized& value) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(val1, val2, val3, val4, val5, value);
				}
				else 
					return Resu();
			}

			void AddSubNode(const Specialized& value, const ATree* sub) 
			{
				child_[value] = AutoTreePtr(sub);
			}

		private:
			ChildMap child_; 
		};
	};

	template < typename Resu, typename Fi, typename Se, typename Th, typename Fo, typename Fifth >
	struct Tree6 { //namespace template pour les structure de l'arbre a 5 attribue
		typedef Resu Result;

		class ATree {
		public:
			virtual ~ATree() {}
			virtual Resu evaluate(const Fi& val1, const Se& val2, const Th& val3, const Fo& val4, const Fifth& val5) const = 0;
		};

		class Leaf : public ATree {
		private:
			Resu value_;

		public:
			Leaf(Resu value) : value_(value) {}

			Resu evaluate(const Fi& val1, const Se& val2, const Th& val3, const Fo& val4, const Fifth& val5) const {
				return value_;
			}
		};

		template< typename Specialized >
		class TreeNode : public ATree
		{
		public:
			typedef std::auto_ptr< const ATree > AutoTreePtr;
			typedef std::map< Specialized, AutoTreePtr > ChildMap;

			Resu evaluate(const Fi& val1, const Se& val2, const Th& val3, const Fo& val4, const Fifth& val5) const 
			{
				return test(val1, val2, val3, val4, val5);
			}

			template <typename N, typename B, typename V, typename C>
			inline Resu test(const Specialized& value, const N& val1, const B& val2, const V& val3, const C& val4) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(value, val1, val2, val3, val4);
				}
				else 
					return Resu();
			}

			template <typename N, typename B, typename V, typename C>
			inline Resu test(const N& val1, const Specialized& value, const B& val2, const V& val3, const C& val4) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(val1, value, val2, val3, val4);
				}
				else 
					return Resu();
			}
	 
			template <typename N, typename B, typename V, typename C>
			inline Resu test(const N& val1, const B& val2, const Specialized& value, const V& val3, const C& val4) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(val1, val2, value, val3, val4);
				}
				else 
					return Resu();
			}
	
			template <typename N, typename B, typename V, typename C>
			inline Resu test(const N& val1, const V& val2, const B& val3, const Specialized& value, const C& val4) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(val1, val2, val3, value, val4);
				}
				else 
					return Resu();
			}

			template <typename N, typename B, typename V, typename C>
			inline Resu test(const N& val1, const V& val2, const B& val3, const C& val4, const Specialized& value) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(val1, val2, val3, val4, value);
				}
				else 
					return Resu();
			}

			void AddSubNode(const Specialized& value, const ATree* sub) 
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

		class ATree {
		public:
			virtual ~ATree() {}
			virtual Resu evaluate(const Fi& val1, const Se& val2, const Th& val3, const Fo& val4) const = 0;
			virtual Resu evaluateDebug(const Fi& val1, const Se& val2, const Th& val3, const Fo& val4) const = 0;
		};

		class Leaf : public ATree {
		private:
			Resu value_;

		public:
			Leaf(Resu value) : value_(value) {}

			Resu evaluate(const Fi& val1, const Se& val2, const Th& val3, const Fo& val4) const {
				return value_;
			}

			Resu evaluateDebug(const Fi& val1, const Se& val2, const Th& val3, const Fo& val4) const {
				std::cout << "wait what ?" << std::endl;
				Debugger<Resu> debug(value_);
				return value_;
			}
		};

		template< typename Specialized >
		class TreeNode : public ATree
		{
		public:
			typedef std::auto_ptr< const ATree > AutoTreePtr;
			typedef std::map< Specialized, AutoTreePtr > ChildMap;

			Resu evaluate(const Fi& val1, const Se& val2, const Th& val3, const Fo& val4) const {
				Debugger<Specialized> debug;
				return test(val1, val2, val3, val4);
			}

			Resu evaluateDebug(const Fi& val1, const Se& val2, const Th& val3, const Fo& val4) const {
				Debugger<Specialized> debug;
				return test(val1, val2, val3, val4);
			}

			template <typename N, typename B, typename V>
			inline Resu test(const Specialized& value, const N& val1, const B& val2, const V& val3) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(value, val1, val2, val3);
				}
				else 
					return Resu();
			}

			template <typename N, typename B, typename V>
			inline Resu test(const N& val1, const Specialized& value, const B& val2, const V& val3) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(val1, value, val2, val3);
				}
				else 
					return Resu();
			}
	 
			template <typename N, typename B, typename V>
			inline Resu test(const N& val1, const B& val2, const Specialized& value, const V& val3) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(val1, val2, value, val3);
				}
				else 
					return Resu();
			}
	
			template <typename N, typename B, typename V>
			inline Resu test(const N& val1, const V& val2, const B& val3, const Specialized& value) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(val1, val2, val3, value);
				}
				else 
					return Resu();
			}

			void AddSubNode(const Specialized& value, const ATree* sub) 
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

		class ATree {
		public:
			virtual ~ATree() {}
			virtual Resu evaluate(const Fi& val1, const Se& val2, const Th& val3) const = 0;
		};

		class Leaf : public ATree {
		private:
			Resu value_;

		public:
			Leaf(Resu value) : value_(value) {}

			Resu evaluate(const Fi& val1, const Se& val2, const Th& val3) const {
				return value_;
			}
		};

		template< typename Specialized >
		class TreeNode : public ATree
		{
		public:
			typedef std::auto_ptr< const ATree > AutoTreePtr;
			typedef std::map< Specialized, AutoTreePtr > ChildMap;

			Resu evaluate(const Fi& val1, const Se& val2, const Th& val3) const {
				return test(val1, val2, val3);
			}

			template <typename N, typename B>
			inline Resu test(const Specialized& value, const N& val1, const B& val2) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(value, val1, val2);
				}
				else 
					return Resu();
			}

			template <typename N, typename B>
			inline Resu test(const N& val1, const Specialized& value, const B& val2) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(val1, value, val2);
				}
				else 
					return Resu();
			}
	 
			template <typename N, typename B>
			inline Resu test(const N& val1, const B& val2, const Specialized& value) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(val1, val2, value);
				}
				else 
					return Resu();
			}
	
			void AddSubNode(const Specialized& value, const ATree* sub) {
				child_[value] = AutoTreePtr(sub);
			}

		private:
			ChildMap child_; 
		};
	};

	template < typename Resu, typename Fi, typename Se>
	struct Tree3 {
		typedef Resu Result;

		class ATree {
		public:
			virtual ~ATree() {}
			virtual Resu evaluate(const Fi& val1, const Se& val2) const = 0;
		};

		class Leaf : public ATree {
		private:
			Resu value_;

		public:
			Leaf(Resu value) : value_(value) {}

			Resu evaluate(const Fi& val1, const Se& val2) const {
				return value_;
			}
		};

		template< typename Specialized >
		class TreeNode : public ATree
		{
		public:
			typedef std::auto_ptr< const ATree > AutoTreePtr;
			typedef std::map< Specialized, AutoTreePtr > ChildMap;

			Resu evaluate(const Fi& val1, const Se& val2) const {
				return test(val1, val2);
			}

			template <typename N>
			inline Resu test(const Specialized& value, const N& val1) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(value, val1);
				}
				else 
					return Resu();
			}

			template <typename N>
			inline Resu test(const N& val1, const Specialized& value) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(val1, value);
				}
				else 
					return Resu();
			}
	 
			void AddSubNode(const Specialized& value, const ATree* sub) {
				child_[value] = AutoTreePtr(sub);
			}

		private:
			ChildMap child_; 
		};
	};

	template < typename Resu, typename Fi>
	struct Tree2 {
		typedef Resu Result;

		class ATree {
		public:
			virtual ~ATree() {}
			virtual Resu evaluate(const Fi& val1) const = 0;
		};

		class Leaf : public ATree {
		private:
			Resu value_;

		public:
			Leaf(Resu value) : value_(value) {}

			Resu evaluate(const Fi& val1) const {
				return value_;
			}
		};

		template< typename Specialized >
		class TreeNode : public ATree
		{
		public:
			typedef std::auto_ptr< const ATree > AutoTreePtr;
			typedef std::map< Specialized, AutoTreePtr > ChildMap;

			Resu evaluate(const Fi& val1) {
				return test(val1);
			}

			inline Resu test(const Specialized& value) const {
				ChildMap::const_iterator it = child_.find(value);
			
				if (it != child_.end()) {
					return it->second->evaluate(value, val1);
				}
				else 
					return Resu();
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