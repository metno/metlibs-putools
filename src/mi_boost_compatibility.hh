
#ifndef MI_BOOST_COMPATIBILITY_HH
#define MI_BOOST_COMPATIBILITY_HH 1

#include <boost/shared_ptr.hpp>
#include <boost/version.hpp>
#include <iterator>

#if BOOST_VERSION >= 104300 && !defined(MI_BOOST_COMPATIBILITY_FORCE_MAP_ADAPTORS)
#include <boost/range/adaptor/map.hpp>
#endif

#if BOOST_VERSION >= 1043000 && !defined(MI_BOOST_COMPATIBILITY_FORCE_FIND_IF)
#include <boost/range/algorithm/find_if.hpp>
#else
#include <algorithm> // std::find_if
#endif

#if BOOST_VERSION >= 103900 && !defined(MI_BOOST_COMPATIBILITY_FORCE_SHARED_PTR)
#include <boost/make_shared.hpp>
namespace miutil {
using boost::make_shared;
}
#else
namespace miutil {
template<class C>
boost::shared_ptr<C> make_shared()
{
  return boost::shared_ptr<C>(new C);
}

template<class C, class A1>
boost::shared_ptr<C> make_shared(const A1& a1)
{
  return boost::shared_ptr<C>(new C(a1));
}

template<class C, class A1, class A2>
boost::shared_ptr<C> make_shared(const A1& a1, const A2& a2)
{
  return boost::shared_ptr<C>(new C(a1, a2));
}

template<class C, class A1, class A2, class A3>
boost::shared_ptr<C> make_shared(const A1& a1, const A2& a2, const A3& a3)
{
  return boost::shared_ptr<C>(new C(a1, a2, a3));
}

template<class C, class A1, class A2, class A3, class A4>
boost::shared_ptr<C> make_shared(const A1& a1, const A2& a2, const A3& a3, const A4& a4)
{
  return boost::shared_ptr<C>(new C(a1, a2, a3, a4));
}

template<class C, class A1, class A2, class A3, class A4, class A5>
boost::shared_ptr<C> make_shared(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5)
{
  return boost::shared_ptr<C>(new C(a1, a2, a3, a4, a5));
}

template<class C, class A1, class A2, class A3, class A4, class A5, class A6>
boost::shared_ptr<C> make_shared(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6)
{
  return boost::shared_ptr<C>(new C(a1, a2, a3, a4, a5, a6));
}

template<class C, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
boost::shared_ptr<C> make_shared(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7)
{
  return boost::shared_ptr<C>(new C(a1, a2, a3, a4, a5, a6, a7));
}

template<class C, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
boost::shared_ptr<C> make_shared(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8)
{
  return boost::shared_ptr<C>(new C(a1, a2, a3, a4, a5, a6, a7, a8));
}

template<class C, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
boost::shared_ptr<C> make_shared(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9)
{
  return boost::shared_ptr<C>(new C(a1, a2, a3, a4, a5, a6, a7, a8, a9));
}

} // namespace miutil
#endif

// ================================================================================
namespace miutil {

namespace adaptors {

namespace detail {
template<class M>
struct map_get_key {
  typedef typename M::value_type map_value_type;
  typedef typename M::key_type   value_type;
  const value_type& operator()(const map_value_type& v) const
    { return v.first; }
};

template<class M>
struct map_get_value {
  typedef typename M::value_type  map_value_type;
  typedef typename M::mapped_type value_type;
  const value_type& operator()(const map_value_type& v) const
    { return v.second; }
  value_type& operator()(map_value_type& v)
    { return v.second; }
};

template<class M, class G>
class map_wrapper
{
public:
  typedef typename M::iterator               m_iterator;
  typedef typename M::reverse_iterator       m_reverse_iterator;
  typedef typename M::const_iterator         m_const_iterator;
  typedef typename M::const_reverse_iterator m_const_reverse_iterator;
  typedef typename G::value_type             g_value_type;

  template<class I, class V>
  struct wrap_iterator : public std::iterator<std::bidirectional_iterator_tag, V>
  {
    typedef V value_type;
    typedef value_type* pointer;
    typedef value_type& reference;

    wrap_iterator(const wrap_iterator& o)
      : iter(o.iter) { }

    wrap_iterator& operator=(const wrap_iterator& o)
      { iter = o.iter; return *this; }

    bool operator==(const wrap_iterator& o) const
      { return iter == o.iter; }

    bool operator!=(const wrap_iterator& o) const
      { return iter != o.iter; }

    wrap_iterator& operator++()
      { ++iter; return *this; }

    wrap_iterator& operator++(int)
      { iter++; return *this; }
        
    wrap_iterator& operator--()
      { --iter; return *this; }

    wrap_iterator& operator--(int)
      { iter--; return *this; }
        
    reference operator*()
      { return G()(*iter); }

    pointer operator->()
      { return &G()(*iter); }

  private:
    friend class map_wrapper;
    wrap_iterator(const I& i)
      : iter(i) { }
        
  private:
    I iter;
  };

  template<class I, class NCI, class V>
  struct wrap_const_iterator : public wrap_iterator<I, V>
  {
    wrap_const_iterator(const wrap_const_iterator& o)
      : wrap_iterator<I,V>(o) { }

    wrap_const_iterator(const wrap_iterator<I,V>& o)
      : wrap_iterator<I,V>(o.iter) { }

  private:
    friend class map_wrapper;
    wrap_const_iterator(const NCI& i)
      : wrap_iterator<I,V>(i) { }
  };

  typedef wrap_iterator<m_iterator, g_value_type> iterator;
  typedef wrap_const_iterator<m_const_iterator, m_iterator, const g_value_type> const_iterator;

  typedef wrap_iterator<m_reverse_iterator, g_value_type> reverse_iterator;
  typedef wrap_const_iterator<m_const_reverse_iterator, m_reverse_iterator, const g_value_type> const_reverse_iterator;

  map_wrapper(M& m)
    : m_wrapped(m) { }

  iterator begin()
    { return iterator(m_wrapped.begin()); }

  iterator end()
    { return iterator(m_wrapped.end()); }
    
  const_iterator begin() const
    { return const_iterator(m_wrapped.begin()); }

  const_iterator end() const
    { return const_iterator(m_wrapped.end()); }
    

  reverse_iterator rbegin()
    { return reverse_iterator(m_wrapped.rbegin()); }

  reverse_iterator rend()
    { return reverse_iterator(m_wrapped.rend()); }
    
  const_reverse_iterator rbegin() const
    { return const_reverse_iterator(m_wrapped.rbegin()); }

  const_reverse_iterator rend() const
    { return const_reverse_iterator(m_wrapped.rend()); }
    
private:
  M& m_wrapped;
};

} // namespace detail

template<class M>
struct map {
  typedef detail::map_wrapper< M, detail::map_get_key<M>   > keys;
  typedef detail::map_wrapper< M, detail::map_get_value<M> > values;
};

#if BOOST_VERSION >= 104300 && !defined(MI_BOOST_COMPATIBILITY_FORCE_MAP_ADAPTORS)
using boost::adaptors::keys;
using boost::adaptors::values;
#else
template<class M>
typename map<M>::keys keys(M& m)
{
  return typename map<M>::keys(m);
}

template<class M>
typename map<const M>::keys keys(const M& m)
{
  return typename map<const M>::keys(m);
}

template<class M>
typename map<M>::values values(M& m)
{
  return typename map<M>::values(m);
}

template<class M>
typename map<const M>::values values(const M& m)
{
  return typename map<const M>::values(m);
}
#endif // boost with map adaptors
} // namespace adaptors

#if BOOST_VERSION >= 1043000 && !defined(MI_BOOST_COMPATIBILITY_FORCE_FIND_IF)
using boost::adaptors::find_if;
#else
template<class C, class P>
typename C::iterator find_if(C& c, const P& p)
{
  return std::find_if(c.begin(), c.end(), p);
}

template<class C, class P>
typename C::const_iterator find_if(const C& c, const P& p)
{
  return std::find_if(c.begin(), c.end(), p);
}
#endif // boost with find_if

} // namespace miutil

#endif /* MI_BOOST_COMPATIBILITY_HH */
